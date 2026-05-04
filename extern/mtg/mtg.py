#!/usr/bin/env -S python3 -u

import sys
import os
import random
import argparse
from core.test_conf import TEST_CONF, L2SCP_CONF

seed = random.getrandbits(64)
BASE_DIR = os.path.dirname(os.path.realpath(__file__))
config = "rand.py"
num_comps = TEST_CONF.NUM_COMPS
chk_freq = TEST_CONF.CHK_FREQ


## Python Argparse code
################################################################
parser = argparse.ArgumentParser()
parser.add_argument("snip_dir", type=str, help="Path where to write the generated components (*.h files)")
parser.add_argument("-c", "--config", type=str, help=f"File containing the components to be included in the test. If not specified, the default config is '{config}'. Configs must be inside '{BASE_DIR}/conf/regress/'", action="store")
parser.add_argument("-s", "--seed", type=int, help=f"Initial seed for the random number generator. If not specified, the default seed is random", action="store")
parser.add_argument("-thr", "--threads", type=str, help=f"A mask of Threads that should be enabled in each Minion (default: {hex(TEST_CONF.THREAD_MASK)})", action="store")
parser.add_argument("-min", "--minions", type=str, help="A mask of Minions that should be enabled in each Shire (default: 1 Minion/Shire)", action="store")
parser.add_argument("-shi", "--shires", type=str, help="A mask of Shires that should be enabled. (default: 1 Shire)", action="store")
parser.add_argument("-ncomps", "--num_components", type=int, help=f"Number of components. By default the value is {num_comps}", action="store")
parser.add_argument("-chkfreq", "--checking_freq", type=int, help=f"Number of components between CRC16 checkpoints. By default the value is {chk_freq}", action="store")
parser.add_argument("-wa", "--workarounds", type=str, help=f"Path to the workarounds file", action="store")
parser.add_argument("-ec", "--extra_checks", help=f"Add extra checks in the final generated ELF. This also triggers sys_emu memory checker.", action="store_true")
parser.add_argument("-dr", "--disable_randomization", help=f"By default, MTG initializes the memory with random data built into the ELF. This flag disables the randomization.", action="store_true")
parser.add_argument("-tm", "--tensor_mask", help=f"By default, Tensor Load, Load L2SCP and FMA assume tensor_mask is always set to 0xffff. This flag forces these 3 operations to use the tensor_mask value in the MTG model", action="store_true")
parser.add_argument("-ps", "--post_silicon", help=f"Compile ELF in post-silicon mode, for use in bringup diag flow", action="store_true")
parser.add_argument("-dbg", "--debug_prints", help=f"Add include the UART Debug prints", action="store_true")
parser.add_argument("-l2", "--postsi_l2", help=f"Constrain MTG to generate an ELF that fits in the L2 and does not generate any request to the L3/NoC", action="store_true")
parser.add_argument("-ma", "--minion_agnostic", help="If minion agnostic is set, only one Minion can be active at a time and the generated ELF will not be binded to any HART ID (Minion agnostic)", action="store_true")
parser.add_argument("-na", "--neigh_agnostic", help="If neigh agnostic is set, only one full Neighborhood (16 threads) can be active at a time and the generated ELF will not be binded to any HART ID (Neigh agnostic)", action="store_true")
parser.add_argument("-sa", "--shire_agnostic", help="If shire agnostic is set, only one full Shire (64 threads) can be active at a time and the generated ELF will not be binded to any HART ID (Shire agnostic)", action="store_true")
parser.add_argument("-tr", "--target_randomization", help="If the target randomization flag is passed, the data randomization will be done by the target, instead of preloading the data into the ELF", action="store_true")
parser.add_argument("-rc", "--repeat_count", type=int, help="If the repeat count is set, the generated ELF will be hardcoded to run the same test X number of times, where X is the repeat count.", action="store")
parser.add_argument("-sc", "--self_check", type=int, help="Enable self-check with checkpoints in the ELF. By default is set to {TEST_CONF.ADD_SELF_CHK}", action="store")
args = parser.parse_args()

if args.self_check != None:
   assert args.self_check in (0, 1), f"--self_check must be 0 or 1, found {args.self_check}"
   TEST_CONF.ADD_SELF_CHK = args.self_check
print(f"MTG: Add self-checking: {TEST_CONF.ADD_SELF_CHK}...")

if args.repeat_count != None:
   assert args.repeat_count > 1, f"Repeat count must be greater than 1, found {args.repeat_count}"
   TEST_CONF.REPEAT_COUNT = int(args.repeat_count)
   print(f"MTG: Repeat count set to {TEST_CONF.REPEAT_COUNT}...")
else:
   TEST_CONF.REPEAT_COUNT = -1

if args.debug_prints:
   TEST_CONF.DEBUG_PRINTS = True
   print("MTG: Enabling UART debug prints...")

use_silicon_flow = False
if args.post_silicon:
   use_silicon_flow = True
   TEST_CONF.POST_SILICON = 1
   print("MTG: Post Silicon mode enabled...")

   if args.postsi_l2:
      TEST_CONF.POSTSI_L2_ONLY = True
      print("MTG: Constrain MTG to not generate requests beyond the L2...")
else:
   assert args.postsi_l2 is False, "'--postsi_l2' arg requires '--post_silicon' arg"

if args.minion_agnostic:
   TEST_CONF.MINION_AGNOSTIC = 1
   print("MTG: Minion Agnostic enabled...")

if args.neigh_agnostic:
   TEST_CONF.NEIGH_AGNOSTIC = 1
   print("MTG: Neigh Agnostic enabled...")

if args.shire_agnostic:
   TEST_CONF.SHIRE_AGNOSTIC = 1
   print("MTG: Shire Agnostic enabled...")

assert (TEST_CONF.MINION_AGNOSTIC + TEST_CONF.NEIGH_AGNOSTIC + TEST_CONF.SHIRE_AGNOSTIC) <= 1, f"Only one agnostic mode can be enabled at a time. MINION_AGNOSTIC={TEST_CONF.MINION_AGNOSTIC}. NEIGH_AGNOSTIC={TEST_CONF.NEIGH_AGNOSTIC}. SHIRE_AGNOSTIC={TEST_CONF.SHIRE_AGNOSTIC}"

if args.tensor_mask:
   TEST_CONF.USE_TENSOR_MASK_MODEL = 1
   print("MTG: Tensor Load, Load L2SCP and FMA models will use the MTG tensor_mask model...")
else:
   TEST_CONF.USE_TENSOR_MASK_MODEL = 0
   print("MTG: Tensor Load, Load L2SCP and FMA models will always assume tensor_mask 0xffff...")

randomize_data = True
if args.disable_randomization:
   randomize_data = False
   print("MTG: ELF data randomization disabled...")
else:
   print("MTG: ELF data randomization enabled...")

if args.target_randomization:
   TEST_CONF.TARGET_RANDOMIZATION = True
   print("MTG: Enabling randomization of the data in the target...")
   assert randomize_data == False, "ELF Randomization is set to True, but Target randomization is also True. Only one of the two can be enabled. To disable ELF randomization use the flag '-dr'."

workarounds = None
if args.workarounds:
   workarounds = args.workarounds

python3 = sys.executable

if args.config:
   config = args.config

if args.seed is not None:
   seed = args.seed

print(f"MTG: initial seed set to {seed}")

snip_dir = args.snip_dir
if snip_dir[-1] == '/':
   snip_dir = snip_dir[:-1]

if args.threads:
   assert "0x" in str(args.threads), "Threads mask must be in hex format (including '0x')"
   thr_mask = str(args.threads)
   assert int(thr_mask, 0) == 3 or TEST_CONF.MINION_AGNOSTIC == 0
else:
   thr_mask = hex(TEST_CONF.THREAD_MASK)

if args.minions:
   assert "0x" in str(args.minions), "Minions mask must be in hex format (including '0x')"
   min_mask = str(args.minions)
else:
   min_mask = hex(TEST_CONF.MINION_MASK)

if args.shires:
   assert "0x" in str(args.shires), "Shires mask must be in hex format (including '0x')"
   shi_mask = str(args.shires)
else:
   shi_mask = hex(TEST_CONF.SHIRE_MASK)

active_harts_per_minion = bin(int(thr_mask, 0)).count('1')
active_minions_per_shire = bin(int(min_mask, 0)).count('1')
active_shires = bin(int(shi_mask, 0)).count('1')

def fcc_relay_in_minion_agnostic():
   return (TEST_CONF.MINION_AGNOSTIC == 1) and ((active_harts_per_minion*active_minions_per_shire*active_shires) > 2)

def fcc_relay_in_neigh_agnostic():
   condition = (TEST_CONF.NEIGH_AGNOSTIC == 1) and ((active_harts_per_minion*active_minions_per_shire*active_shires) > 16)
   if condition:
      def full_neigh(valid_mask):
         tmp = int(min_mask, 16)&valid_mask
         return tmp == 0 or tmp == valid_mask
      assert full_neigh(0xff) and full_neigh(0xff00) and full_neigh(0xff0000) and full_neigh(0xff000000), "In neigh agnostic mode, only one full Neigh can be active at a time"
   return condition

def fcc_relay_in_shire_agnostic():
   condition = (TEST_CONF.SHIRE_AGNOSTIC == 1) and ((active_harts_per_minion*active_minions_per_shire*active_shires) > 64)
   if condition:
      assert int(min_mask, 16) == 0xffffffff, "In shire agnostic mode, only one full Shire can be active at a time"
   return condition

if fcc_relay_in_minion_agnostic() or fcc_relay_in_neigh_agnostic() or fcc_relay_in_shire_agnostic():
   TEST_CONF.SEQUENTIAL_MODE = 0x1
   assert TEST_CONF.POSTSI_L2_ONLY == False, "Agnostic mode with FCC relay will generate traffic beyond the L2 but '--postsi_l2' option is set."

   TEST_CONF.TARGET_RANDOMIZATION = True
   print("MTG: Enabling target randomization of the data because Minion Agnostic is enabled with more than one Minion active")

   randomize_data = False
   print("MTG: ELF data randomization disabled because target randomization is enabled")

   TEST_CONF.SEQ_MINION_MASK = int(min_mask, 0)
   # Position of the LSB that is set
   min_mask = (1 + (TEST_CONF.SEQ_MINION_MASK ^ (TEST_CONF.SEQ_MINION_MASK-1))) >> 1

   TEST_CONF.SEQ_SHIRE_MASK = int(shi_mask, 0)
   # Position of the LSB that is set
   shi_mask = hex((1 + (TEST_CONF.SEQ_SHIRE_MASK ^ (TEST_CONF.SEQ_SHIRE_MASK-1))) >> 1)

   if TEST_CONF.MINION_AGNOSTIC == 1:
      min_mask = int(TEST_CONF.SEQ_MINION_MASK/min_mask) & 0x1

   if TEST_CONF.NEIGH_AGNOSTIC == 1:
      min_mask = int(TEST_CONF.SEQ_MINION_MASK/min_mask) & 0xff

   if TEST_CONF.SHIRE_AGNOSTIC == 1:
      min_mask = int(TEST_CONF.SEQ_MINION_MASK/min_mask) & 0xffffffff
   min_mask = hex(min_mask)

   import math
   def getLSB(n: int) -> int:
     assert n != 0
     return int(math.log2(n&-n))
   TEST_CONF.SEQ_BASE_HART = (getLSB(TEST_CONF.SEQ_MINION_MASK)*2) + (getLSB(TEST_CONF.SEQ_SHIRE_MASK) * 64)

   TEST_CONF.SEQ_NUM_HARTS = active_shires*active_minions_per_shire*2
   active_harts_per_minion = bin(int(thr_mask, 0)).count('1')
   active_minions_per_shire = bin(int(min_mask, 0)).count('1')
   active_shires = bin(int(shi_mask, 0)).count('1')

print(f"MTG: Shires Mask: {shi_mask}")
print(f"MTG: Minions Mask: {min_mask}")
print(f"MTG: Threads Mask: {thr_mask}")

if TEST_CONF.POSTSI_L2_ONLY == True:
    assert active_shires == 1, f"PostSi L2 only option is set, but the number of enabled shires is greater than 1 (number of active shires: {active_shires})"

TEST_CONF.NUM_HARTS = active_shires*active_minions_per_shire*active_harts_per_minion
TEST_CONF.THREAD_MASK = int(thr_mask, 0)
TEST_CONF.MINION_MASK = int(min_mask, 0)
TEST_CONF.SHIRE_MASK = int(shi_mask, 0)
TEST_CONF.NUM_ACTIVE_HARTS_PER_SHIRE = active_minions_per_shire*active_harts_per_minion

if TEST_CONF.MINION_AGNOSTIC == 1:
   assert TEST_CONF.NUM_HARTS == 2, "In minion agnostic mode, only one Minion can be active at a time"
   assert TEST_CONF.THREAD_MASK == 0x3, "In minion agnostic mode, only one Minion can be active at a time"

if TEST_CONF.NEIGH_AGNOSTIC == 1:
   assert bin(TEST_CONF.SHIRE_MASK).count('1') == 1, "In neigh agnostic mode, only one full Neigh can be active at a time"
   assert TEST_CONF.THREAD_MASK == 0x3, "In neigh agnostic mode, only one full Neigh can be active at a time"

if TEST_CONF.SHIRE_AGNOSTIC == 1:
   assert bin(TEST_CONF.SHIRE_MASK).count('1') == 1, "In shire agnostic mode, only one full Shire can be active at a time"
   assert TEST_CONF.THREAD_MASK == 0x3, "In shire agnostic mode, only one full Shire can be active at a time"

if TEST_CONF.SEQUENTIAL_MODE == 0x1:
   print (f"MTG: Sequential base HART is {TEST_CONF.SEQ_BASE_HART}")
   print(f"MTG: Number of participant HARTS: {TEST_CONF.SEQ_NUM_HARTS}")

def get_bits_pos(mask):
   pos = 0
   ret = list()
   while mask != 0:
      if (mask & 0x1) == 1:
         ret.append(pos)
      mask = mask >> 1
      pos = pos + 1
   return ret

if TEST_CONF.SEQUENTIAL_MODE == 0x1:
   tids = get_bits_pos(TEST_CONF.THREAD_MASK)
   mids = get_bits_pos(TEST_CONF.SEQ_MINION_MASK)
   sids = get_bits_pos(TEST_CONF.SEQ_SHIRE_MASK)
   active_hids = [m*2 + t for m in mids for t in tids]
   active_hids = [s*64 + a for s in sids for a in active_hids]
   print(f"MTG: Participant HART ID's: {active_hids}")

tids = get_bits_pos(TEST_CONF.THREAD_MASK)
mids = get_bits_pos(TEST_CONF.MINION_MASK)
sids = get_bits_pos(TEST_CONF.SHIRE_MASK)

active_hids = [m*2 + t for m in mids for t in tids]
active_hids = [s*64 + a for s in sids for a in active_hids]
TEST_CONF.ACTIVE_HART_IDS = active_hids

print(f"MTG: Number of active HARTS: {TEST_CONF.NUM_HARTS}")
print(f"MTG: Active HART ID's: {TEST_CONF.ACTIVE_HART_IDS}")

if args.num_components != None:
   num_comps = args.num_components
TEST_CONF.NUM_COMPS = num_comps

print(f"MTG: Number of components: {TEST_CONF.NUM_COMPS}")

if args.checking_freq:
   chk_freq = args.checking_freq
TEST_CONF.CHK_FREQ = chk_freq

print(f"MTG: CRC16 checking frequency: 1 checkpoint every {TEST_CONF.CHK_FREQ} Components")

if args.extra_checks:
   TEST_CONF.EXTRA_CHECKS = 1
   print("Adding extra checks into the ELF")
else:
   TEST_CONF.EXTRA_CHECKS = 0


## Instantiate a TestHelper object
################################################################
from core.models.model import Model
model = Model(seed = seed)
L2SCP_CONF.set_l2scp_conf(model.test_helper)
L2SCP_CONF.print_conf()

from core.mtg import *

## Load the Components Data Base from the 'components.py' file
################################################################
components_dict = {}

def t(snip_name, snip_path):
   components_dict[str(snip_name)] = str(snip_path)

def import_components():
   for snip in components_dict:
      module_name = os.path.splitext((components_dict[snip]).replace("/","."))[0];
      exec(f"from {module_name} import {snip}", globals())
      component = get_component(snip)

exec(open(BASE_DIR+"/conf/components.py").read())

print(f"Importing Components database in {BASE_DIR}/conf/components.py ...")
import_components()


## Workarounds
################################################################
exec(open(BASE_DIR+"/conf/mtg_workarounds.py").read())
if workarounds:
   exec(open(workarounds).read())

for _comp in TEST_CONF.EXCL_COMPS:
   _comp.exclude = True

excl_comp_names = [comp.__name__ for comp in TEST_CONF.EXCL_COMPS]
print(f"Excluded Components: {excl_comp_names}")

for i in TEST_CONF.EXCL_INTREGS:
   model.integer.exclude_reg(i)

for i in TEST_CONF.EXCL_FPREGS:
   model.floatpoint.exclude_reg(i)

for i in TEST_CONF.EXCL_MREGS:
   model.mask.exclude_reg(i)


## Read the config file and instantiate the config components
################################################################
preamble_components = []
def preamble(snip_name):
   component = get_component(snip_name)
   if component.exclude is False:
      preamble_components.append((component, 1))
   else:
      assert component in TEST_CONF.EXCL_COMPS, f"{component.__name__} is excluded but not in the TEST_CONF.EXCL_COMPS list"

start_components = []
def start(snip_name):
   component = get_component(snip_name)
   if component.exclude is False:
      start_components.append((component, 1))
   else:
      assert component in TEST_CONF.EXCL_COMPS, f"{component.__name__} is excluded but not in the TEST_CONF.EXCL_COMPS list"

main_components = []
def main(snip_name, bias):
   component = get_component(snip_name)
   if component.exclude is False:
      main_components.append((component, bias))
   else:
      assert component in TEST_CONF.EXCL_COMPS, f"{component.__name__} is excluded but not in the TEST_CONF.EXCL_COMPS list"

end_components = []
def end(snip_name):
   component = get_component(snip_name)
   if component.exclude is False:
      end_components.append((component, 1))
   else:
      assert component in TEST_CONF.EXCL_COMPS, f"{component.__name__} is excluded but not in the TEST_CONF.EXCL_COMPS list"

print(f"Importing test Components from config {BASE_DIR}/conf/regress/{config} ...")
if TEST_CONF.TARGET == 'erbium':
   exec(open(f"{BASE_DIR}/conf/regress/erbium_base.py").read())
else:
   exec(open(f"{BASE_DIR}/conf/regress/base.py").read())
exec(open(f"{BASE_DIR}/conf/regress/{config}").read())

print(f"MTG: static labels are: {TEST_CONF.STATIC_LABELS}")

if TEST_CONF.TARGET_RANDOMIZATION == False:
   assert TEST_CONF.REPEAT_COUNT == -1, "Test repeat feature is only available when target randomization is enabled (-tr)"

## Generate the .*h files
################################################################

if not os.path.exists(snip_dir+"/components"):
    os.makedirs(snip_dir+"/components")

from tools.randgen.randgen import PreambleComponents, MixComponents, MixComponentsHandlers, MemCodeGen, EndComponents

print("Generating the Preamble Components...")
preamble = PreambleComponents(model.test_helper, "preamble", preamble_components, snip_dir)
preamble.gen_snippets(model, shuffle = False, ignore_num_comps = True)
preamble.export_order()

print("Generating the Start Components...")
start = MixComponents(model.test_helper, "start", start_components, snip_dir)
start.gen_snippets(model, shuffle = False, ignore_num_comps = True)
start.export_order()

print("Generating the Main Components...")
main = MixComponents(model.test_helper, "main", main_components, snip_dir)
main.gen_snippets(model)
main.export_order()

print("Generating the End Components...")
end = EndComponents(model.test_helper, "end", end_components, snip_dir)
end.gen_snippets(model, shuffle = False, ignore_num_comps = True, sequential_mode = TEST_CONF.SEQUENTIAL_MODE)
end.export_order()

# Generate Trap Handlers order file
hand = MixComponentsHandlers(model.test_helper, "handlers", snip_dir, shuffle=True)
hand.export_order()

# Export memory regions
mem = MemCodeGen(snip_dir)
mem.export_mem(model.test_helper, randomize = randomize_data, gen_in_target = TEST_CONF.TARGET_RANDOMIZATION)

## Overwrite STACK Size
################################################################
if TEST_CONF.POSTSI_L2_ONLY == True or TEST_CONF.agnostic_mode() == True:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write("EXTRA_CFLAGS+=' -DSTACK_SIZE_LOG2=10'\n")

## Set UART debug prints
################################################################
if TEST_CONF.DEBUG_PRINTS:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write("EXTRA_CFLAGS+=' -DUART_PRINTS'\n")

## Set POSTSI UST flow
################################################################
if use_silicon_flow:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write("EXTRA_CFLAGS+=' -DPOSTSI_UST'\n")

## Set AGNOSTIC MODE defines
################################################################
if TEST_CONF.MINION_AGNOSTIC == 1:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write(f"EXTRA_CFLAGS+=' -DMIN_AGNOSTIC_MODE'\n")

if TEST_CONF.NEIGH_AGNOSTIC == 1:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write(f"EXTRA_CFLAGS+=' -DNEIGH_AGNOSTIC_MODE'\n")

if TEST_CONF.SHIRE_AGNOSTIC == 1:
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write(f"EXTRA_CFLAGS+=' -DSHIRE_AGNOSTIC_MODE'\n")

## Generate the Physical HID to Logical HID mapping
################################################################
print("Calling gen_lhid_mapping...")
args_list = " ".join(map(str, TEST_CONF.ACTIVE_HART_IDS))
if TEST_CONF.agnostic_mode() == False:
   cmd = f"{python3} {BASE_DIR}/tools/gen_lhid_map/gen_lhid_mapping.py -a {args_list} -- {snip_dir}"
else:
   cmd = f"{python3} {BASE_DIR}/tools/gen_lhid_map/gen_lhid_mapping.py -am -a {args_list} -- {snip_dir}"
os.system(cmd)

## Call the stitcher
################################################################
print("Calling stitcher...")
template = "base.cc.mako"
mako_l2 = ""
if TEST_CONF.POSTSI_L2_ONLY == True:
   mako_l2 = "-l2"
repeat_count = TEST_CONF.REPEAT_COUNT
cmd = f"{python3} {BASE_DIR}/tools/stitcher/stitcher.py -t {BASE_DIR}/tools/stitcher/templates/{template} -l {ABIreg.regs['lhid'][0]} {mako_l2} -rc {repeat_count} -sc {TEST_CONF.ADD_SELF_CHK} {snip_dir}"
os.system(cmd)

## Generate the code for CRC16 self-checking
################################################################
import subprocess
test_name = os.path.splitext(template)[0]
if TEST_CONF.ADD_SELF_CHK:
   print("Generating the CRC16 signatures...")
   crc_mem = CRC16_REGION(model.test_helper)
   base_addr = crc_mem.paddr
   mem_size = crc_mem.get_used_size()

   mem_chk = ""
   if TEST_CONF.EXTRA_CHECKS == 1:
      mem_chk = "-ec"
   exit_code = os.system(f"{python3} {BASE_DIR}/tools/selfcheck/checkpoint/checkpoint.py {snip_dir} {base_addr} {mem_size} -t {test_name} -min {min_mask} -shi {shi_mask} {mem_chk}")
   assert exit_code == 0

## Generate the ELF
################################################################
if TEST_CONF.ADD_SELF_CHK:
   print("Force .text section PC...")
   cmd = 'riscv64-unknown-elf-objdump -h base | grep ".text"'
   output = subprocess.check_output(cmd, shell=True)
   text_pc = output.split()[4].decode("utf-8")
   ld_text_pc = f",--section-start=.text=0x{text_pc}"
   with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
      sourceme.write(f"MTG_LD_ARGS=${{MTG_LD_ARGS}}{ld_text_pc}")

print("Generating the final ELF...")
elf_name = os.path.splitext(test_name)[0]
compile_cmd = f"{BASE_DIR}/tools/compile/compile.sh {snip_dir} {elf_name}"
print(f"Compiling with: {compile_cmd}")
os.system(compile_cmd)

## Run the ELF in emulator
################################################################

if TEST_CONF.ADD_SELF_CHK:
    NUM_ACTIVE_HARTS = active_shires*active_minions_per_shire*2

    # Select emulator based on target
    if TEST_CONF.TARGET == 'erbium':
       EMU_BIN = f"{BASE_DIR}/tools/sys_emu/erbium_emu"
    else:
       EMU_BIN = f"{BASE_DIR}/tools/sys_emu/sys_emu"

    def run_sys_emu(min_mask, shi_mask, max_cycles=TEST_CONF.SYS_EMU_MAX_CYCLES):
       if TEST_CONF.REPEAT_COUNT > 0:
          max_cycles = max_cycles * TEST_CONF.REPEAT_COUNT
       ec = ""
       if TEST_CONF.TARGET == 'erbium':
          sys_emu_cmd = f"{EMU_BIN} -elf_load {elf_name} -reset_pc 0x40000000 -max_cycles {max_cycles} > sysemu.log"
          sys_emu_cmd = f"{EMU_BIN} -elf_load {elf_name} -reset_pc 0x40000000 -minions {min_mask} -max_cycles {max_cycles} > sysemu.log"
       else:
          if TEST_CONF.EXTRA_CHECKS == 1:
             ec = "-vpurf_check -mem_check -mem_check_minion -1 -l1_scp_check -l1_scp_check_minion -1 -l2_scp_check -l2_scp_check_shire -1 -flb_check -flb_check_shire -1 -tstore_check -tstore_check_thread -1"
          sys_emu_cmd = f"{EMU_BIN} -minions {min_mask} -shires {shi_mask} -elf {elf_name} -max_cycles {max_cycles} {ec} > sysemu.log"
       print(f"Running in emulator using: {sys_emu_cmd}")
       os.system(sys_emu_cmd)

       pass_marks = int(os.popen('grep -c "PASS" sysemu.log').read())
       assert pass_marks == NUM_ACTIVE_HARTS, f"MTG: SYS_EMU execution failed ({pass_marks}/{NUM_ACTIVE_HARTS})"

    if TEST_CONF.SEQUENTIAL_MODE == 0x1:
       NUM_ACTIVE_HARTS = TEST_CONF.SEQ_NUM_HARTS
       run_sys_emu(hex(TEST_CONF.SEQ_MINION_MASK), hex(TEST_CONF.SEQ_SHIRE_MASK), max_cycles=TEST_CONF.SYS_EMU_MAX_CYCLES*NUM_ACTIVE_HARTS)
    else:
       if TEST_CONF.MINION_AGNOSTIC == 1:
          shire_masks = [hex(1 << i) for i in range(34)]
          minion_masks = [hex(1 << i) for i in range(8*4)]
          for shi_mask in shire_masks:
             for min_mask in minion_masks:
                run_sys_emu(min_mask, shi_mask)
       elif TEST_CONF.NEIGH_AGNOSTIC == 1:
          shire_masks = [hex(1 << i) for i in range(34)]
          minion_masks = [hex(0xff << (i*8)) for i in range(4)]
          for shi_mask in shire_masks:
             for min_mask in minion_masks:
                run_sys_emu(min_mask, shi_mask)
       elif TEST_CONF.SHIRE_AGNOSTIC == 1:
          shire_masks = [hex(1 << i) for i in range(34)]
          for shi_mask in shire_masks:
             run_sys_emu(hex(0xffffffff), shi_mask)
       else:
          run_sys_emu(min_mask, shi_mask)

    if TEST_CONF.REPEAT_COUNT != -1:
       os.system(f"mv {elf_name} {elf_name}_rc2")
       with open(f"{snip_dir}/sourceme.sh", "a") as sourceme:
          sourceme.write(f"\nEXTRA_CFLAGS+=' -DFINAL_ELF'")
       compile_cmd = f"{BASE_DIR}/tools/compile/compile.sh {snip_dir} {elf_name}"
       print(f"Compiling final ELF with: {compile_cmd}")
       os.system(compile_cmd)
