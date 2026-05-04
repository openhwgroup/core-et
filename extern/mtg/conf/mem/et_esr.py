
from core.models.memorymodel import MemoryRegion

class ESR_REGION(MemoryRegion):
   size  = 0x000000000008
   exclusive = False
   export = False

class ESR_MPROT_NEIGH0(ESR_REGION):
   paddr = 0x0001ffd00020

class ESR_MPROT_NEIGH1(ESR_REGION):
   paddr = 0x0001ffd10020

class ESR_MPROT_NEIGH2(ESR_REGION):
   paddr = 0x0001ffd20020

class ESR_MPROT_NEIGH3(ESR_REGION):
   paddr = 0x0001ffd30020

class ESR_MPROT_BCAST(ESR_REGION):
   paddr = 0x0001ffdf0020

class ESR_SC_IDX_COP_SM_CTL_B0(ESR_REGION):
   paddr = 0x0001fff00030

class ESR_SC_IDX_COP_SM_CTL_B1(ESR_REGION):
   paddr = 0x0001fff02030

class ESR_SC_IDX_COP_SM_CTL_B2(ESR_REGION):
   paddr = 0x0001fff04030

class ESR_SC_IDX_COP_SM_CTL_B3(ESR_REGION):
   paddr = 0x0001fff06030

class ESR_SC_IDX_COP_SM_CTL_BCAST(ESR_REGION):
   paddr = 0x0001fff1e030

class ESR_LOCAL_SHIRE_COOP_MODE(ESR_REGION):
   paddr = 0x00017ff40290

class ESR_LOCAL_IPI_TRIGGER(ESR_REGION):
   paddr = 0x0001fff40090

class ESR_LOCAL_IPI_TRIGGER_CLEAR(ESR_REGION):
   paddr = 0x0001fff40098

class ESR_LOCAL_IPI_REDIRECT_FILTER(ESR_REGION):
   paddr = 0x1fff40088

class ESR_LOCAL_IPI_REDIRECT(ESR_REGION):
   paddr = 0x13ff40080

class ESR_LOCAL_IPI_REDIRECT_PC_BCAST(ESR_REGION):
   paddr = 0x13fdf0040

class ESR_SC_REQQ_CTL_B0(ESR_REGION):
   paddr = 0x0001fff00008

class ESR_SC_REQQ_CTL_B1(ESR_REGION):
   paddr = 0x0001fff02008

class ESR_SC_REQQ_CTL_B2(ESR_REGION):
   paddr = 0x0001fff04008

class ESR_SC_REQQ_CTL_B3(ESR_REGION):
   paddr = 0x0001fff06008

class ESR_SC_REQQ_CTL_BCAST(ESR_REGION):
   paddr = 0x0001fff1e008

# FCC Credit Increment ESRs (ET-SoC-1)
class ESR_FCC_CREDINC_0(ESR_REGION):
   paddr = 0x1003400C0

class ESR_FCC_CREDINC_1(ESR_REGION):
   paddr = 0x1003400C8

class ESR_FCC_CREDINC_2(ESR_REGION):
   paddr = 0x1003400D0

class ESR_FCC_CREDINC_3(ESR_REGION):
   paddr = 0x1003400D8
