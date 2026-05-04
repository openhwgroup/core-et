
from core.mtg import *
from components.randinst.randinst import RANDINST
from dataclasses import fields

@param_block
class ReadMS1(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'ms1' in x.__dataclass_fields__ and x.__dataclass_fields__['ms1'].type == Mreg and x.exclude == False]

   def __init__(self, reg: Mreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(ms1 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadMS2(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'ms2' in x.__dataclass_fields__ and x.__dataclass_fields__['ms2'].type == Mreg and x.exclude == False]

   def __init__(self, reg: Mreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(ms2 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadMreg(Component, AsmCode):

   comps = [ReadMS1, ReadMS2]
   bias  = [len(ReadMS1.subcls), len(ReadMS2.subcls)]

   def __init__(self, msx: Mreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one_weight(self.comps, self.bias)(msx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class WriteMreg(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'md' in x.__dataclass_fields__ and x.__dataclass_fields__['md'].type == Mreg and x.exclude == False]

   def __init__(self, reg: Mreg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(md = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@register
class MaskReadAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.msx = Mreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadMreg(self.msx, test_helper = test_helper)
      self.insn2 = ReadMreg(self.msx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['m0']))
      content_list.append(self.insn2)

@register
class MaskWriteAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.md = Mreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteMreg(self.md, test_helper = test_helper)
      self.insn2 = WriteMreg(self.md, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['m0']))
      content_list.append(self.insn2)

@register
class MaskWriteAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.mx = Mreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadMreg(self.mx, test_helper = test_helper)
      self.insn2 = WriteMreg(self.mx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['m0']))
      content_list.append(self.insn2)

@register
class MaskReadAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.mx = Mreg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteMreg(self.mx, test_helper = test_helper)
      self.insn2 = ReadMreg(self.mx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst, skip_res=['m0']))
      content_list.append(self.insn2)
