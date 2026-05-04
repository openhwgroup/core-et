
from core.mtg import *
from components.randinst.randinst import RANDINST
from dataclasses import fields
from components.resrc_haz.int_deps import ReadXreg, WriteXreg, ReadRS1, ReadRS2
from components.randinst.amo import AMOCMPSWAPL_W, AMOCMPSWAPL_D, AMOCMPSWAPG_W, AMOCMPSWAPG_D

_implX31_comps = [AMOCMPSWAPL_W, AMOCMPSWAPL_D, AMOCMPSWAPG_W, AMOCMPSWAPG_D]

@param_block
class ImplReadX31(Component, AsmCode):

   subcls = [x for x in _implX31_comps if x.exclude == False]
   def __init__(self, x31: Xreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(test_helper = test_helper, x31 = x31)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class WriteXregExcl(WriteXreg):
   subcls = [x for x in RANDINST.subcls if 'rd' in x.__dataclass_fields__ and x.__dataclass_fields__['rd'].type == Xreg and x.exclude == False and x not in _implX31_comps]

@param_block
class ReadRS1Excl(ReadRS1):
   subcls = [x for x in RANDINST.subcls if 'rs1' in x.__dataclass_fields__ and x.__dataclass_fields__['rs1'].type == Xreg and x.exclude == False and x not in _implX31_comps]

@param_block
class ReadRS2Excl(ReadRS2):
   subcls = [x for x in RANDINST.subcls if 'rs2' in x.__dataclass_fields__ and x.__dataclass_fields__['rs2'].type == Xreg and x.exclude == False and x not in _implX31_comps]

@param_block
class ReadXregExcl(ReadXreg):
   comps = [ReadRS1Excl, ReadRS2Excl]
   bias  = [len(ReadRS1Excl.subcls), len(ReadRS2Excl.subcls)]

@register
class X31ReadAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rsx = Xreg(31)
      self.num_inst = test_helper.get_rand_range(0, 5)
      self.bias = test_helper.get_rand_range(0,1)

      if self.bias == 0:
         self.insn1 = ImplReadX31(self.rsx, test_helper = test_helper)
         self.insn2 = ReadXregExcl(self.rsx, test_helper = test_helper)
      else:
         self.insn1 = ReadXregExcl(self.rsx, test_helper = test_helper)
         self.insn2 = ImplReadX31(self.rsx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['x31']))
      content_list.append(self.insn2)

@register
class X31WriteAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rx = Xreg(31)
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ImplReadX31(self.rx, test_helper = test_helper)
      self.insn2 = WriteXregExcl(self.rx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['x31']))
      content_list.append(self.insn2)

@register
class X31ReadAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rx = Xreg(31)
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteXregExcl(self.rx, test_helper = test_helper)
      self.insn2 = ImplReadX31(self.rx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['x31']))
      content_list.append(self.insn2)
