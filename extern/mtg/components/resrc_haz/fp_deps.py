
from core.mtg import *
from components.randinst.randinst import RANDINST
from dataclasses import fields

@param_block
class ReadFS1(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'fs1' in x.__dataclass_fields__ and x.__dataclass_fields__['fs1'].type == Freg and x.exclude == False]

   def __init__(self, reg: Freg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(fs1 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadFS2(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'fs2' in x.__dataclass_fields__ and x.__dataclass_fields__['fs2'].type == Freg and x.exclude == False]

   def __init__(self, reg: Freg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(fs2 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadFS3(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'fs3' in x.__dataclass_fields__ and x.__dataclass_fields__['fs3'].type == Freg and x.exclude == False]

   def __init__(self, reg: Freg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(fs3 = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class ReadFreg(Component, AsmCode):

   comps = [ReadFS1, ReadFS2, ReadFS3]
   bias  = [len(ReadFS1.subcls), len(ReadFS2.subcls), len(ReadFS3.subcls)]

   def __init__(self, fsx: Freg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one_weight(self.comps, self.bias)(fsx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@param_block
class WriteFreg(Component, AsmCode):

   subcls = [x for x in RANDINST.subcls if 'fd' in x.__dataclass_fields__ and x.__dataclass_fields__['fd'].type == Freg and x.exclude == False]

   def __init__(self, reg: Freg, test_helper: TestHelper = None):
      self.insn = test_helper.pick_one(self.subcls)(fd = reg, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn)

@register
class FPReadAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.fsx = Freg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadFreg(self.fsx, test_helper = test_helper)
      self.insn2 = ReadFreg(self.fsx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class FPWriteAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.fd = Freg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteFreg(self.fd, test_helper = test_helper)
      self.insn2 = WriteFreg(self.fd, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class FPWriteAfterRead(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.fx = Freg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = ReadFreg(self.fx, test_helper = test_helper)
      self.insn2 = WriteFreg(self.fx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)

@register
class FPReadAfterWrite(Component, AsmCode):
   soft_alloc = True

   def __init__(self, test_helper: TestHelper = None):
      self.fx = Freg()
      self.num_inst = test_helper.get_rand_range(0, 5)

      self.insn1 = WriteFreg(self.fx, test_helper = test_helper)
      self.insn2 = ReadFreg(self.fx, test_helper = test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.insn1)
      content_list.append(RANDINST(test_helper = test_helper, num_inst = self.num_inst))
      content_list.append(self.insn2)
