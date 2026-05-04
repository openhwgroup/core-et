
from core.mtg import *
from components.randinst.amo import AMOADDG_D, AMOADDL_D, AMOORG_D, AMOORL_D
from components.randinst.rv32i import LI, FENCE, ADD
from components.randinst.rv32m import REM
from components.flb_fcc.flb_fcc_sync_threads import FLBFCCSyncShireThreads, AMOFLBFCCSyncThreads
from components.et_csr import SET_EXCL_MODE, UNSET_EXCL_MODE
from components.evict_all_l1d import EvictAllL1D
from components.cacheops import SET_DCACHE_SHARED
from components.tensor_wait import WAIT_CACHEOPS
from components.randinst.rv import FENCE
from components.clear_tensor_error import ClearTensorError
from components.tensor.tensor_mask import TensorMask, TensorConvSize, TensorConvCtrl
from components.cops.coherency_barrier import CoherencyBarrier
from components.flb_fcc.fcc_barrier import FCCBarrier
from components.preamble.rand_xregs import RandXregs

@register
class RepeatTestTop(Component, AsmCode):

   head_label = "repeat_test_start"

   @only_thread(0)
   def _disable_l1scp(self, content_list: list, test_helper: TestHelper = None) -> None:
      content_list.append(SET_EXCL_MODE(test_helper = test_helper))

      content_list.append(EvictAllL1D(test_helper = test_helper))

      content_list.append(SET_DCACHE_SHARED(test_helper = test_helper, rd=self.zero))
      content_list.append(FENCE(test_helper = test_helper))
      content_list.append(WAIT_CACHEOPS(test_helper = test_helper))

      content_list.append(UNSET_EXCL_MODE(test_helper = test_helper))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      self.zero = ABIreg('zero')
      content_list.append(f"{self.head_label}:")
      content_list.append(f"add x1, x0, x0")
      content_list.append(f"csrrwi x0, fcsr, 0x1")
      content_list.append(f"csrrwi x0, fflags, 0x1")
      content_list.append(ClearTensorError(test_helper=test_helper))
      content_list.append("mova.m.x {zero}")
      content_list.append(TensorConvSize(test_helper=test_helper, value=0))
      content_list.append(TensorConvCtrl(test_helper=test_helper, value=0))
      content_list.append(TensorMask(test_helper=test_helper, value=0))

if TEST_CONF.agnostic_mode() != 0:
   class RepeatBarrier(FLBFCCSyncShireThreads, AsmCode):
      pass
else:
   class RepeatBarrier(AMOFLBFCCSyncThreads, AsmCode):
      pass

@register
class RepeatTestBottom(RepeatBarrier, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      super().__init__.__wrapped__(self, test_helper)
      self.rt_mem = StaticGMalloc(f"{self.__class__.__name__}_rt_mem",  ZERO_REGION, 8, align=8, align_cl=False)
      assert TEST_CONF.REPEAT_COUNT > 0, "Test repeat count must be greater than 0"
      self.rt_reg0 = UXreg()
      self.rt_reg1 = UXreg()
      self.rt_reg2 = UXreg(10)
      self.rt_reg3 = UXreg()
      self.zero = ABIreg('zero')

   def _lead_hart_code(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._lead_hart_code(content_list, test_helper=test_helper)

      content_list.append(LI(rd=self.rt_reg2, imm=self.rt_mem))
      content_list.append(JumpToCFunct(test_helper=test_helper, funct_name="print_loop_iteration"))

      content_list += [
         LI(rd=self.rt_reg0, imm=1),
         LI(rd=self.rt_reg1, imm=self.rt_mem)
      ]
      if TEST_CONF.POSTSI_L2_ONLY == True:
         content_list.append(AMOADDL_D(rd=self.zero, rs2=self.rt_reg0, rs1=self.rt_reg1))
      else:
         content_list.append(AMOADDG_D(rd=self.zero, rs2=self.rt_reg0, rs1=self.rt_reg1))
      content_list.append(FENCE())

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:
      super()._run(content_list, test_helper=test_helper)
      content_list.append(RandXregs(test_helper=test_helper))
      content_list += [
         LI(rd=self.rt_reg1, imm=self.rt_mem)
      ]
      if TEST_CONF.POSTSI_L2_ONLY == True:
         content_list.append(AMOORL_D(rd=self.rt_reg0, rs2=self.zero, rs1=self.rt_reg1))
      else:
         content_list.append(AMOORG_D(rd=self.rt_reg0, rs2=self.zero, rs1=self.rt_reg1))
      content_list.append(CoherencyBarrier(test_helper = test_helper))
      content_list.append(FCCBarrier(test_helper=test_helper))

      if TEST_CONF.EXTRA_CHECKS != 0:
         # This bubble of instructions is used to add some delay for some harts, this way
         # we can catch unpredictable register issues when the repeat flow is used
         content_list += [
            LI(rd=self.rt_reg3, imm=test_helper.get_rand_range(2, TEST_CONF.NUM_HARTS)),
            ADDI(rd=self.rt_reg1, rs1=ABIreg('lhid'), imm=(3)),
            "bubble:",
            ADDI(rd=self.rt_reg1, rs1=self.rt_reg1, imm=(1)),
            REM(rd=self.rt_reg2, rs1=self.rt_reg1, rs2=self.rt_reg3),
            "bne x0, {rt_reg2}, bubble"
         ]

      content_list += [
         "#ifdef GEN_CHECKPOINT",
         LI(rd=self.rt_reg1, imm=(1)),
         "#else",
         "#ifdef FINAL_ELF",
         LI(rd=self.rt_reg1, imm=(TEST_CONF.REPEAT_COUNT)),
         "#else",
         LI(rd=self.rt_reg1, imm=(2)),
         "#endif",
         "#endif",
         "remu {rt_reg0}, {rt_reg0}, {rt_reg1}",
         f"beq {{rt_reg0}}, {{zero}}, finish",
         LI(rd=self.rt_reg0, imm=0x3),
         LI(rd=self.rt_reg1, imm=0x12),
         f"call {RepeatTestTop.head_label}",
         "finish:",
         LI(rd=self.rt_reg0, imm=0x12),
         LI(rd=self.rt_reg1, imm=0x27),
      ]

TEST_CONF.STATIC_LABELS.append(RepeatTestTop.head_label)

if TEST_CONF.REPEAT_COUNT == -1:
   TEST_CONF.EXCL_COMPS.add(RepeatTestTop)
   TEST_CONF.EXCL_COMPS.add(RepeatTestBottom)
