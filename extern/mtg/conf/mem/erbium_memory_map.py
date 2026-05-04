from core.models.memorymodel import MemoryRegion
from core.test_conf import TEST_CONF

# Erbium Memory Map
#
# +---------------------------------+---------+-------------------+
# |      Address range              |         |                   |
# |      From      |      To        |   Size  | Maps to           |
# +----------------+----------------+---------+-------------------+
# | 0x00_0200_0000 | 0x00_0200_0FFF |  4KB    | SystemRegisters   |
# | 0x00_0200_A000 | 0x00_0200_BFFF |  8KB    | Boot ROM          |
# | 0x00_0200_E000 | 0x00_0200_E7FF |  2KB    | Scratch SRAM      |
# | 0x00_4000_0000 | 0x00_40FF_FFFF | 16MB    | MRAM              |
# | 0x00_8000_0000 | 0x00_80FF_FFFF | 16MB    | ESR Registers     |
# | 0x00_C000_0000 | 0x00_C3FF_FFFF | 64MB    | PLIC              |
# +----------------+----------------+---------+-------------------+

class SYSREG_REGION(MemoryRegion):
   paddr = 0x02000000
   size  = 0x1000  # 4KB

class BOOTROM_REGION(MemoryRegion):
   paddr = 0x0200A000
   size  = 0x2000  # 8KB

class SRAM_REGION(MemoryRegion):
   paddr = 0x0200E000
   size  = 0x800  # 2KB

class ESR_REGION(MemoryRegion):
   paddr = 0x80000000
   size  = 0x1000000  # 16MB

class PLIC_REGION(MemoryRegion):
   paddr = 0xC0000000
   size  = 0x4000000  # 64MB

# MRAM layout (16MiB total at 0x40000000):
#   0x40000000 - 0x40000400: bootrom (1KB)
#   0x40000400 - 0x40200000: code/rodata/data (~2MB reserved)
#   0x40200000 - 0x40FC0000: MTG data regions (~14MB)
#   0x40FC0000 - 0x41000000: stack (256KB)
MRAM_REGION_BASE = 0x40000000
MRAM_REGION_SIZE = 0x1000000  # 16MB
MRAM_CODE_END = 0x40200000    # Reserve 2MB for bootrom + code
MRAM_STACK_START = 0x40FC0000 # Stack at end
MRAM_REGION_HEAD = MRAM_CODE_END
MRAM_REGION_TAIL = MRAM_STACK_START

class ZERO_REGION(MemoryRegion):
   paddr = MRAM_REGION_HEAD
   size  = 128  # 2 cache lines

   @classmethod
   def dflt_value(cls):
      return 0

MRAM_REGION_HEAD = MRAM_REGION_HEAD + ZERO_REGION.size

if TEST_CONF.SEQUENTIAL_MODE == 0x0:
   SCRATCH_REGION_SIZE = 64 * TEST_CONF.NUM_HARTS
else:
   SCRATCH_REGION_SIZE = 64 * TEST_CONF.SEQ_NUM_HARTS

class SCRATCH_REGION(MemoryRegion):
   paddr = MRAM_REGION_HEAD
   size  = SCRATCH_REGION_SIZE

MRAM_REGION_HEAD = MRAM_REGION_HEAD + SCRATCH_REGION.size

# CRC16 checking region
CRC16_REGION_SIZE = int((TEST_CONF.NUM_COMPS / TEST_CONF.CHK_FREQ + 10) * (16 / 8) * TEST_CONF.NUM_HARTS)
CRC16_REGION_SIZE = CRC16_REGION_SIZE + (64 - (CRC16_REGION_SIZE % 64))

class CRC16_REGION(MemoryRegion):
   paddr = MRAM_REGION_HEAD
   size  = CRC16_REGION_SIZE

MRAM_REGION_HEAD = MRAM_REGION_HEAD + CRC16_REGION_SIZE

# LHID mapping region
ERBIUM_MAX_HARTS = 8 * 2  # 8 cores × 2 threads
LHID_MAP_REGION_SIZE = ERBIUM_MAX_HARTS * 2
LHID_MAP_REGION_SIZE = LHID_MAP_REGION_SIZE + (64 - (LHID_MAP_REGION_SIZE % 64))

class LHID_MAP_REGION(MemoryRegion):
   paddr = MRAM_REGION_HEAD
   size  = LHID_MAP_REGION_SIZE
   export = False

MRAM_REGION_HEAD = MRAM_REGION_HEAD + LHID_MAP_REGION_SIZE

# Main DRAM region (rest of MRAM)
class DRAM_REGION(MemoryRegion):
   paddr = MRAM_REGION_HEAD
   size  = MRAM_REGION_TAIL - MRAM_REGION_HEAD
