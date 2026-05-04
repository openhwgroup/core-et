
from core.mtg import *
from components.randinst.mask import MOV_M_X
from components.randinst.packedsingle import FBCX_PS
from components.randinst.rv32i import LI
from components.common import BackupMregs, RestoreMregs

@register
class InitFPABI(Component, AsmCode):

   def __init__(self, test_helper: TestHelper = None):
      self.zero = ABIreg('zero')
      self.amo = FPABIreg('amo', ignore_check=True)
      self.non_amo = FPABIreg('off', ignore_check=True)
      self.m0 = Mreg(0)
      self.m1 = Mreg(1)
      self.m2 = Mreg(2)
      self.m3 = Mreg(3)
      self.m4 = Mreg(4)
      self.m5 = Mreg(5)
      self.m6 = Mreg(6)
      self.m7 = Mreg(7)
      self.rx0 = Xreg()

      self.amo_off = list()
      # Set a random amo_offset for each lane (8 lanes in total)
      for _ in range(8):
         # Aligned to 4 Bytes (32 bits)
         rand_amo_off = (test_helper.get_rand_range(-TEST_CONF.MAX_AMO_OFF, TEST_CONF.MAX_AMO_OFF) >> 2) << 2
         self.amo_off.append(Imm32(rand_amo_off))

      self.off = list()
      # Set a random offset for each lane (8 lanes in total)
      for _ in range(8):
         # Aligned to 4 Bytes (32 bits)
         rand_off = test_helper.get_rand_range(-TEST_CONF.MAX_OFF, TEST_CONF.MAX_OFF)
         self.off.append(Imm12(rand_off))

   def _run(self, content_list: list, test_helper: TestHelper = None) -> None:

      # Backup the previous M0 to the stack
      content_list.append(BackupMregs(m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))

      for i in range(8):
         content_list.append(MOV_M_X(md=self.m0, rs1=self.zero, imm8=Zimm8(1 << i)))

         content_list.append(LI(rd=self.rx0, imm=self.amo_off[i]))
         content_list.append(FBCX_PS(fd=self.amo, rs1=self.rx0, m0=self.m0))

         content_list.append(LI(rd=self.rx0, imm=self.off[i]))
         content_list.append(FBCX_PS(fd=self.non_amo, rs1=self.rx0, m0=self.m0))

      # Restore the previous M0 to the stack
      content_list.append(RestoreMregs(m0=self.m0, m1=self.m1, m2=self.m2, m3=self.m3, m4=self.m4, m5=self.m5, m6=self.m6, m7=self.m7))

   def render_if(self, model: 'Model') -> bool:
      model.fp_amo_is_set = True
      model.fp_off_is_set = True
      return super().render_if(model)
