
from core.mtg import *
from components.tensor.tensor_base import TensorBase
from components.randinst.rv32i import LI
from components.randinst.randinst import RANDINST

@param_block
class RD_CSR_RS1(Component, AsmCode):
   _ops   = ["csrrw",  "csrrs",  "csrrc"]

   def __init__(self, test_helper: TestHelper = None, mnemonic: str = None, rd: Xreg = None, rs1: Xreg = None):
      self.base_op = test_helper.pick_one(self._ops)
      self.mnemonic = mnemonic
      self.rd = rd
      self.rs1 = rs1

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(f"{self.base_op} {{rd}}, {self.mnemonic}, {{rs1}}")

@param_block
class RD_CSR_IMM(Component, AsmCode):
   _ops_i = ["csrrwi", "csrrsi", "csrrci"]

   def __init__(self, test_helper: TestHelper = None, mnemonic: str = None, rd: Xreg = None, imm: Imm = None):
      self.base_op = test_helper.pick_one(self._ops_i)
      self.mnemonic = mnemonic
      self.rd = rd
      if imm is None:
         self.imm = test_helper.get_rand_kbits(5)
      else:
         self.imm = imm

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(f"{self.base_op} {{rd}}, {self.mnemonic}, {{imm}}")

@register
class TrapTensor(TensorBase, ComponentHandler):

   _mnemonics = ["tensor_coop", "tensor_fma", "tensor_load", "tensor_quant", "tensor_reduce", "tensor_store"]

   def __init__(self, test_helper: TestHelper = None, mnemonic = None):
      self.regx1 = Xreg()

      self.rand_num = test_helper.get_rand_num()
      if mnemonic is None:
         self.mnemonic = test_helper.pick_one(self._mnemonics)
      else:
         assert mnenomic in self._mnemonics
         self.mnemonic = mnemonic

      use_imm = test_helper.pick_one([True, False])
      if use_imm:
         self.csr_op = RD_CSR_IMM(test_helper=test_helper, mnemonic=self.mnemonic, rd=self.regx1)
      else:
         self.regx2 = Xreg()
         self.csr_op = RD_CSR_RS1(test_helper=test_helper, mnemonic=self.mnemonic, rd=self.regx1, rs1=self.regx2)

   @only_thread(1)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run.__wrapped__(self, content_list, test_helper=test_helper)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      if isinstance(self.csr_op, RD_CSR_RS1):
         content_list.append(LI(rd=self.regx2, imm=self.rand_num))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.csr_op)

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TEST_FAIL(test_helper=test_helper)),
      content_list.append("selfcheck_pass:")

   @set_mepc_to("selfcheck_pass")
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   @set_sepc_to("selfcheck_pass")
   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

@register
class TrapTensorSequence(TensorBase, ComponentHandler):

   def __init__(self, test_helper: TestHelper = None, mnemonic = None):
      self.seq_len = test_helper.get_rand_range(3, 12)
      self.insns = list()
      for _ in range(self.seq_len):
         self.insns.append(TrapTensor(test_helper=test_helper))

   @only_thread(1)
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run.__wrapped__(self, content_list, test_helper=test_helper)

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      for insn in self.insns:
         insn._tensor_conf(content_list, test_helper=test_helper)

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      for insn in self.insns:
         insn._tensor_csr(content_list, test_helper=test_helper)

   @mepc_plus(4)
   def _mhandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

   @sepc_plus(4)
   def _shandler(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

@register
class TrapAnySequence(TrapTensorSequence):

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(RANDINST(test_helper = test_helper, num_inst = 1))
      for insn in self.insns:
         insn._tensor_csr(content_list, test_helper=test_helper)
         content_list.append(RANDINST(test_helper = test_helper, num_inst = 1))
