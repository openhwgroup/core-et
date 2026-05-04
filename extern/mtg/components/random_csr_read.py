
from core.mtg import *

from components.et_csr import SET_EXCL_MODE, UNSET_EXCL_MODE
from components.randinst.rv32i import *

@param_block
class RandomUCSRRead(Component, AsmCode):
   csr_id_vals     = ["fflags", "frm", "fcsr"]
   csr_id_weights  = [      10,     5,      5]
   csr_id_excl     = list()

   def __init__(self, test_helper: TestHelper = None):
      self.rx1 = Xreg()

      # Select the CSR to read
      self.csr_id = test_helper.pick_one_weight(self.csr_id_vals, self.csr_id_weights, exclude = self.csr_id_excl)


   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      content_list.append('csrr {rx1}, {csr_id}')

@param_block
class RandomMCSRRead(RandomUCSRRead, AsmCode):
   csr_id_vals     = ["mstatus", "misa", "medeleg", "mideleg", "mie", "mtvec", "mscratch", "mepc", "mcause", "mtval", "mip"]
   csr_id_weights  = [       10,      1,         3,         3,     3,       1,          1,      3,        3,       3,     3]
   csr_id_excl     = list()

   def __init__(self, test_helper: TestHelper = None):
      if TEST_CONF.REPEAT_COUNT != -1:
         self.csr_id_excl.append("mstatus")
      super().__init__.__wrapped__(self, test_helper=test_helper)

   @machine_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, test_helper=test_helper)

@param_block
class RandomSCSRRead(RandomUCSRRead, AsmCode):
   csr_id_vals     = ["sstatus", "sie", "stvec", "sscratch", "sepc", "scause", "stval", "sip", "satp"]
   csr_id_weights  = [       10,     2,       1,          1,      2,        2,       2,     2,      2]
   csr_id_excl     = list()

   @machine_supervisor_code
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, test_helper=test_helper)

@register
class RandomCSRRead(Component, AsmCode):
   def __init__(self, test_helper: TestHelper = None):
      comps = [RandomUCSRRead, RandomMCSRRead, RandomSCSRRead]
      self.comp = test_helper.pick_one(comps)(test_helper=test_helper)

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(self.comp)
