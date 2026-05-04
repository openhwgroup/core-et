
from core.mtg import *
from components.randinst.randinst import RANDINST
from dataclasses import fields

@param_block
class ReadRS1(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'rs1' in x.__dataclass_fields__ and x.__dataclass_fields__['rs1'].type == Xreg and x.exclude == False]

   def __init__(self, reg: Xreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(rs1 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadRS2(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'rs2' in x.__dataclass_fields__ and x.__dataclass_fields__['rs2'].type == Xreg and x.exclude == False]

   def __init__(self, reg: Xreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(rs2 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadXreg(Component, AsmCode):

   comps = [ReadRS1, ReadRS2]
   bias  = [len(ReadRS1.subcls), len(ReadRS2.subcls)]

   def __init__(self, rsx: Xreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one_weight(self.comps, self.bias)(rsx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class WriteXreg(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'rd' in x.__dataclass_fields__ and x.__dataclass_fields__['rd'].type == Xreg and x.exclude == False]

   def __init__(self, reg: Xreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(rd = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@register
class IntReadAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rsx = Xreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadXreg(self.rsx, test_helper = test_helper)
      self.insn2 = ReadXreg(self.rsx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class IntWriteAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rd = Xreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteXreg(self.rd, test_helper = test_helper)
      self.insn2 = WriteXreg(self.rd, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class IntWriteAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rx = Xreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadXreg(self.rx, test_helper = test_helper)
      self.insn2 = WriteXreg(self.rx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class IntReadAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.rx = Xreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteXreg(self.rx, test_helper = test_helper)
      self.insn2 = ReadXreg(self.rx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)
