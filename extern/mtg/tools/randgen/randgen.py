from typing import Type
from core.constrain_regs import ConstrainRegs
from core.component import Component
from core.models.model import Model
from core.models.memorymodel import MemoryMeta, CRC16Malloc, GMalloc
from core.test_conf import TEST_CONF

# Dynamic memory map import based on target
if TEST_CONF.TARGET == 'erbium':
    from conf.mem.erbium_mem import DRAM_REGION, CRC16_REGION, LHID_MAP_REGION
else:
    from conf.mem.et_mem import DRAM_REGION, CRC16_REGION, LHID_MAP_REGION

from core.models.test_helper import TestHelper
from components.checking.gen_signature import GenSignature
from components.flb_fcc.fcc_barrier import FCCBarrier
from core.snippet.snippet import Snippet
import glob

class MixComponentsHandlers:

   def __init__(self, test_helper: TestHelper, name: str, output_dir: str, shuffle: bool = True):
      self.name = name
      self.output_dir = output_dir
      self.test_helper = test_helper
      self.shuffle = shuffle

      self.mhandler_list = glob.glob(f"{output_dir}/components/*_mhandler.h")
      self.shandler_list = glob.glob(f"{output_dir}/components/*_shandler.h")
      self.uhandler_list = glob.glob(f"{output_dir}/components/*_uhandler.h")

   def export_order(self) -> None:
      name = self.name.upper()
      output_filename = f"{self.output_dir}/{name}.out"
      f = open(output_filename,"w+")

      def _export_handler_order(order_list: list) -> None:
         if order_list is not None:
            for i in order_list:
               f.write(f"{i}\n")

      if self.shuffle:
         handler_order = self.mhandler_list + self.shandler_list + self.uhandler_list
         self.test_helper.shuffle(handler_order)
         _export_handler_order(handler_order)
      else:
         _export_handler_order(self.mhandler_list)
         _export_handler_order(self.shandler_list)
         _export_handler_order(self.uhandler_list)
      f.close()

class MixComponents:

   add_checking = TEST_CONF.ADD_SELF_CHK
   chk_freq = TEST_CONF.CHK_FREQ

   def __init__(self, test_helper: TestHelper, name: str, comp_cls: (Type[Component], int), output_dir: str):
      self.name = name
      self.output_dir = output_dir
      self.content = list()
      self.content_order = list()
      self.test_helper = test_helper
      self.chk_freq = self.chk_freq + 1

      for comp, bias in comp_cls:
         for i in range(bias):
            self.content.append(comp)

   def write_snip(self, snip: Snippet, suffix: str):
      snip.export_content(f"{self.output_dir}/components", prefix = self.name, suffix = suffix)

      path = snip.get_export_format(f"{self.output_dir}/components", prefix = self.name, suffix = suffix)
      self.content_order.append(path)

   def _add_checkpoint(self, model: Model, suffix: str) -> str:
         crc16_mem = CRC16Malloc(CRC16_REGION, 2, align=2, align_cl=False)
         info_mem = GMalloc('GenSignature_info_mem', DRAM_REGION, 16, align=64)

         component = GenSignature(test_helper = None, crc16_mem = crc16_mem, info_mem = info_mem)
         snip = component.render(model)
         snip.export_content(f"{self.output_dir}/components", prefix = self.name, suffix = suffix)

         path = snip.get_export_format(f"{self.output_dir}/components", prefix = self.name, suffix = suffix)

         component = GenSignature(test_helper = None, crc16_mem = crc16_mem, info_mem = info_mem, check = True)
         snip = component.render(model)
         snip.export_content(f"{self.output_dir}/components", prefix = f".{self.name}", suffix = suffix)

         return path

   def gen_snippets(self, model: Model, shuffle = True, ignore_num_comps = False) -> None:
      if shuffle:
         self.test_helper.shuffle(self.content)

      num_constrain_regs = 0
      idx = 0
      if ignore_num_comps:
         num_components = len(self.content)
      else:
         num_components = TEST_CONF.NUM_COMPS
         indexes = list(range(len(self.content)))

      i = 0
      while i < num_components:

         if ignore_num_comps:
            comp_cls = self.content[i]
         else:
            rand_idx = self.test_helper.pick_one(indexes)
            comp_cls = self.content[rand_idx]

         component = comp_cls(test_helper = model.test_helper)

         if not isinstance(component, ConstrainRegs):
            # Add checkpoints
            if self.add_checking:
               if (idx%self.chk_freq) == 0:
                  path = self._add_checkpoint(model, str(idx))
                  self.content_order.append(path)
                  idx += 1
            snip = component.render(model)
            self.write_snip(snip, suffix=str(idx))
            idx += 1
            i += 1
         else:
            num_constrain_regs += 1
            component.render(model)

      # Force a checkpoint at the end
      if self.add_checking:
         path = self._add_checkpoint(model, str(idx))
         self.content_order.append(path)

      # Add an FCC Sync barrier at the end
      component = FCCBarrier(test_helper = model.test_helper)
      snip = component.render(model)
      self.write_snip(snip, suffix=str(idx))
      print(f"Number of ConstrainRegs: {num_constrain_regs}\n")

   def export_order(self) -> None:
      name = self.name.upper()
      output_filename = f"{self.output_dir}/{name}.out"
      f = open(output_filename,"w+")
      for i in self.content_order:
         f.write(f"{i}\n")
      f.close()

class PreambleComponents(MixComponents):
   add_checking = False

class EndComponents(MixComponents):
   def gen_snippets(self, model: Model, shuffle = True, ignore_num_comps = False, sequential_mode = True) -> None:
      super().gen_snippets(model, shuffle=shuffle, ignore_num_comps=ignore_num_comps)
      if sequential_mode:
         from components.cops.coherency_barrier import CoherencyBarrier
         coh_barrier = CoherencyBarrier(test_helper=model.test_helper)
         snip = coh_barrier.render(model)
         self.write_snip(snip, suffix="")

         from components.flb_fcc.fcc_chain import PassFccRelay
         component = PassFccRelay()
         snip = component.render(model)
         self.write_snip(snip, suffix="")

class MemCodeGen:

   _prefix = "mtg"
   _skipped_prefix = "mtgsk"
   _skip_candidates = ["DRAM_REGION"]
   _line_count = 27

   def __init__(self, output_dir: str):
      self.output_dir = output_dir

   def gen_mem_content(self, test_helper: TestHelper, size: int, gen_val) -> str:
      ret = ""
      if size > 0:
         count = 1
         rand = gen_val()
         rand = format(rand,"02X")
         ret = f"\n   0x{rand}"

         for _ in range(1, size):
            rand = gen_val()
            rand = format(rand,"02X")
            if count%self._line_count == 0:
               ret += f",\n   0x{rand}"
            else:
               ret += f", 0x{rand}"
            count += 1

      return ret

   def _gen_target_data_rand(self, mem_regions, file_name):
      output_filename = f"{self.output_dir}/{file_name}"
      f = open(output_filename,"w+")

      c_code = ( "uint64_t lcg(uint64_t& seed) {\n"
                 "   return (seed = (164603309694725029ul * seed) % 14738995463583502973ul);\n"
                 "}\n\n"
               )
      f.write(c_code)

      c_code = ("extern \"C\" void init_rand_data(int lhid, uint64_t seed);\n"
                "void init_rand_data(int lhid, uint64_t seed) {\n"
               )

      rem = list()

      for reg in mem_regions:
         mem_size = int((reg[2]/64)/TEST_CONF.NUM_HARTS)
         done_size = (mem_size*TEST_CONF.NUM_HARTS* 64)
         pending_size = reg[2] - done_size
         if pending_size > 0:
            rem.append((reg[0], reg[1], done_size, pending_size))

         if mem_size > 0:
            c_code += (
                         "\n"
                        f"   /*{reg[0]} -> Starts at {reg[1]}. Total size is {reg[2]} Bytes, but we are only initializing {done_size}*/\n"
                        f"   uint64_t (&{reg[0].lower()})[{TEST_CONF.NUM_HARTS}][{mem_size}][8] = *reinterpret_cast<uint64_t (*)[{TEST_CONF.NUM_HARTS}][{mem_size}][8]> ((uint64_t*) {reg[1]});\n"
                        f"   for (int i = 0; i < {mem_size}; i++) {{\n"
                         "      for (int b = 0; b < 8; b++) {\n"
                        f"         {reg[0].lower()}[lhid][i][b] = lcg(seed);\n"
                         "      }\n"
                         "   }\n"
                      )
      c_code += "\n}\n"
      f.write(c_code)

      if len(rem) > 0:
         c_code = ("\nextern \"C\" void init_rem_rand_data(uint64_t seed);\n"
                   "void init_rem_rand_data(uint64_t seed) {\n"
                  )
      else:
         c_code = ("\nextern \"C\" void init_rem_rand_data();\n"
                   "void init_rem_rand_data() {\n"
                  )
      for r in rem:
         done_size = r[2]
         pending_size = r[3]

         c_code += (
                      "\n"
                     f"   /*{r[0]} -> Remaining memory size is {pending_size} Bytes*/\n"
                     f"   uint8_t (&{reg[0].lower()})[{pending_size}] = *reinterpret_cast<uint8_t (*)[{pending_size}]> ((uint8_t*) {hex(int(r[1], 16) + done_size)});\n"
                     f"   for (int i = 0; i < {pending_size}; i++) {{\n"
                     f"      {reg[0].lower()}[i] = lcg(seed);\n"
                      "   }\n"
                   )
      c_code += "\n}\n"
      f.write(c_code)
      f.close()

   def export_mem(self, test_helper: TestHelper, randomize = False, gen_in_target = False) -> None:
      excl_rand = ["CRC16_REGION", "ZERO_REGION", "SCRATCH_REGION"]
      sections = list()
      export_list =  list()
      mem_regions = list()
      for i in MemoryMeta._instances.values():
         if i.used:
            size = i.get_used_size()
            region_name = i.__class__.__name__
            sections.append((region_name, hex(i.paddr), size, i.dflt_value()))
            if i.export:
               export_list.append(region_name)

      # Write the MTG memory map into a file. This file will be included from the C code
      output_filename = f"{self.output_dir}/mtg_memory_map.h"
      f = open(output_filename,"w+")
      env_var = f'-Wl,--section-start={self._prefix}_LHID_MAP_REGION={hex(LHID_MAP_REGION.paddr)}'
      for i in sections:
         if TEST_CONF.TARGET_RANDOMIZATION == True and i[0] in self._skip_candidates:
            prefix = self._skipped_prefix
         else:
            prefix = self._prefix
         if i[0] in export_list:
            if i[3] != None:
               content = self.gen_mem_content(test_helper, i[2], lambda : i[3])
               f.write(f'uint8_t __attribute__((section ("{prefix}_{i[0]}"))) {i[0]}[{i[2]}] = {{ {content} }};\n')
            elif i[0] not in excl_rand and randomize:
               content = self.gen_mem_content(test_helper, i[2], lambda : test_helper.get_rand_kbits(8))
               f.write(f'uint8_t __attribute__((section ("{prefix}_{i[0]}"))) {i[0]}[{i[2]}] = {{ {content} }};\n')
            else:
               f.write(f'uint8_t __attribute__((section ("{prefix}_{i[0]}"))) {i[0]}[{i[2]}];\n')
            env_var += f',--section-start={prefix}_{i[0]}={i[1]}'
            if i[0] not in excl_rand:
               mem_regions.append((i[0], i[1], i[2]))
         else:
            # Add the region commented to help debug
            f.write(f'/*uint8_t __attribute__((section ("{prefix}_{i[0]}"))) {i[0]}[{i[2]}];*/\n')
      f.close()

      tgt_rand_filename = "mtg_target_data_rand.h"
      if gen_in_target:
         self._gen_target_data_rand(mem_regions, tgt_rand_filename)
      else:
         from pathlib import Path
         Path(f"{self.output_dir}/{tgt_rand_filename}").touch()

      # Write the Linker script arguments in a file
      output_filename = f"{self.output_dir}/sourceme.sh"
      f = open(output_filename,"w+")
      f.write(f'#!/usr/bin/bash\nMTG_LD_ARGS={env_var}\n')
      f.close()
