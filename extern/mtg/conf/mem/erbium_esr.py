
from core.models.memorymodel import MemoryRegion

# Erbium ESR definitions

class ESR_REGION(MemoryRegion):
   size  = 0x000000000008
   exclusive = False
   export = False

# Neighborhood ESRs
class ESR_MPROT(ESR_REGION):
   paddr = 0x0080D00020

class ESR_IPI_REDIRECT_PC(ESR_REGION):
   paddr = 0x0080100040

# Shire ESRs
class ESR_LOCAL_SHIRE_COOP_MODE(ESR_REGION):
   paddr = 0x0080740290

class ESR_IPI_REDIRECT_TRIGGER(ESR_REGION):
   paddr = 0x0080340080

class ESR_LOCAL_IPI_REDIRECT_FILTER(ESR_REGION):
   paddr = 0x0080F40088

class ESR_LOCAL_IPI_TRIGGER(ESR_REGION):
   paddr = 0x0080F40090

class ESR_LOCAL_IPI_TRIGGER_CLEAR(ESR_REGION):
   paddr = 0x0080F40098

# FCC Credit Increment
class ESR_FCC_CREDINC_0(ESR_REGION):
   paddr = 0x00803400C0

class ESR_FCC_CREDINC_1(ESR_REGION):
   paddr = 0x00803400C8

class ESR_FCC_CREDINC_2(ESR_REGION):
   paddr = 0x00803400D0

class ESR_FCC_CREDINC_3(ESR_REGION):
   paddr = 0x00803400D8

# Fast Local Barriers
class ESR_FAST_LOCAL_BARRIER0(ESR_REGION):
   paddr = 0x0080340100

class ESR_FAST_LOCAL_BARRIER1(ESR_REGION):
   paddr = 0x0080340108

# Timer ESRs
class ESR_MTIME(ESR_REGION):
   paddr = 0x0080F40200

class ESR_MTIMECMP(ESR_REGION):
   paddr = 0x0080F40208

# Stub definitions below allow component imports without errors

# Shire Cache stubs (not functional on Erbium)
# Using unmapped region 0x5000_0000
_STUB_BASE = 0x50000000

class ESR_SC_IDX_COP_SM_CTL_B0(ESR_REGION):
   paddr = _STUB_BASE + 0x00

class ESR_SC_IDX_COP_SM_CTL_B1(ESR_REGION):
   paddr = _STUB_BASE + 0x10

class ESR_SC_IDX_COP_SM_CTL_B2(ESR_REGION):
   paddr = _STUB_BASE + 0x20

class ESR_SC_IDX_COP_SM_CTL_B3(ESR_REGION):
   paddr = _STUB_BASE + 0x30

class ESR_SC_IDX_COP_SM_CTL_BCAST(ESR_REGION):
   paddr = _STUB_BASE + 0x40

class ESR_SC_REQQ_CTL_B0(ESR_REGION):
   paddr = _STUB_BASE + 0x50

class ESR_SC_REQQ_CTL_B1(ESR_REGION):
   paddr = _STUB_BASE + 0x60

class ESR_SC_REQQ_CTL_B2(ESR_REGION):
   paddr = _STUB_BASE + 0x70

class ESR_SC_REQQ_CTL_B3(ESR_REGION):
   paddr = _STUB_BASE + 0x80

class ESR_SC_REQQ_CTL_BCAST(ESR_REGION):
   paddr = _STUB_BASE + 0x90

# MPROT stubs for neighborhoods
class ESR_MPROT_NEIGH0(ESR_REGION):
   paddr = 0x0080D00020

class ESR_MPROT_NEIGH1(ESR_REGION):
   paddr = 0x0080D10020

class ESR_MPROT_NEIGH2(ESR_REGION):
   paddr = 0x0080D20020

class ESR_MPROT_NEIGH3(ESR_REGION):
   paddr = 0x0080D30020

class ESR_MPROT_BCAST(ESR_REGION):
   paddr = 0x0080DF0020

class ESR_LOCAL_IPI_REDIRECT(ESR_REGION):
   paddr = 0x0080340080

class ESR_LOCAL_IPI_REDIRECT_PC_BCAST(ESR_REGION):
   paddr = 0x0080100040
