
from core.mtg import *
from components.encoding.enc import *
from components.randinst.rv64i import SD

@register
class FCC(CSRRWI):
   pass

@param_block
class SendFCC(SD):
   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SD(rs2=self.rs2, off=self.off,  rs1=self.rs1))
