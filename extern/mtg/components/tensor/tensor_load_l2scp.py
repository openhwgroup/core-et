
from core.mtg import *
from core.models.tensorloadl2scpmodel import TensorLoadL2SCPEncoding
from core.from_future import FromFuture
from components.randinst.rv32i import LI, AND, OR, ADDI, SLLI, SRLI
from components.randinst.rv32m import MUL, REMU
from components.tensor.tensor_base import TensorBase
from components.tensor.tensor import TENSOR_LOAD_L2
from components.tensor_wait import WAIT_TENSOR_LOAD_L2_0, WAIT_TENSOR_LOAD_L2_1

@register
class TensorLoadL2SCP(TensorBase):

   def __init__(self, test_helper: TestHelper = None, x31: Xreg = None, x31_val: int = None, enc: int = None):
      if x31 is None:
         self.x31 = Xreg(31)
      else:
         self.x31 = x31

      self.rx1 = Xreg()
      self.rx2 = Xreg()
      self.rx3 = Xreg()
      self.rx4 = Xreg()

      self.lhid = ABIreg('lhid')

      if x31_val is None:
         x31_val = test_helper.get_rand_kbits(8)

      self.tl_l2_id = x31_val & 0x1

      self.tl_l2scp_enc = TensorLoadL2SCPEncoding(x31_val, value = enc)
      self.mem = StaticMalloc(MIN_SHIRE_MEM_SET, self.tl_l2scp_enc.memory_size, align=64)
      self.dst = self.tl_l2scp_enc.l2scp_dst

   def _tensor_x31(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Set X31
      content_list.append(LI(rd=self.x31, imm=self.tl_l2scp_enc.x31_val))

   def _tensor_conf(self, content_list: list, test_helper: TestHelper = None) -> None:
      # Load the memory address
      content_list.append(LI(rd=self.rx1, imm=self.mem))
      content_list.append(LI(rd=self.rx2, imm=hex(0xffffffffffc0)))
      content_list.append(AND(rd=self.rx1, rs1=self.rx1, rs2=self.rx2))

      # Setup the Tensor encoding
      content_list.append(LI(rd=self.rx3, imm=self.tl_l2scp_enc))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))

      # Setup the 'dst' field
      content_list.append(f"/*dst is {self.dst}*/")
      content_list.append(LI(rd=self.rx1, imm=self.dst))
      content_list.append(ADDI(rd=self.rx2, rs1=self.lhid, imm=0))

      num_harts_local_shire = get_num_harts_local_shire()
      content_list.append(LI(rd=self.rx4, imm=num_harts_local_shire))
      content_list.append(REMU(rd=self.rx2, rs1=self.rx2, rs2=self.rx4))
      if TEST_CONF.THREAD_MASK == 0x3:
         content_list.append(SRLI(rd=self.rx2, rs1=self.rx2, imm=1))

      content_list.append(LI(rd=self.rx4, imm=L2SCP_CONF.tensor_slice_size))
      content_list.append(MUL(rd=self.rx4, rs1=self.rx4, rs2=self.rx2))
      content_list.append(ADD(rd=self.rx2, rs1=self.rx1, rs2=self.rx4))

      # DST[1:0]
      content_list.append(SLLI(rd=self.rx1, rs1=self.rx2, imm=62))
      content_list.append(SRLI(rd=self.rx1, rs1=self.rx1, imm=58))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))
      # DST[16:2]
      content_list.append(SRLI(rd=self.rx1, rs1=self.rx2, imm=2))
      content_list.append(SLLI(rd=self.rx1, rs1=self.rx1, imm=48))
      content_list.append(OR(rd=self.rx3, rs1=self.rx3, rs2=self.rx1))

   def _tensor_csr(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(TENSOR_LOAD_L2(enc=self.tl_l2scp_enc, rs1=self.rx3, x31=self.x31))
      if (self.tl_l2_id & 0x1) == 0:
         content_list.append(WAIT_TENSOR_LOAD_L2_0())
      else:
         content_list.append(WAIT_TENSOR_LOAD_L2_1())

   def _tensor_post(self, content_list: list, test_helper: TestHelper = None) -> None:
      pass

if TEST_CONF.POSTSI_L2_ONLY == True:
   TEST_CONF.EXCL_COMPS.add(TensorLoadL2SCP)
