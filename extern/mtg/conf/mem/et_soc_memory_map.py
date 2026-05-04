from core.models.memorymodel import MemoryRegion
from core.test_conf import TEST_CONF, L2SCP_CONF

class MAXION_REGION(MemoryRegion):
   paddr = 0x000000000000
   size  = 0x000010000000

class IO_REGION(MemoryRegion):
   paddr = 0x000010000000
   size  = 0x000010000000

class MAILBOX(MemoryRegion):
   paddr = 0x000020000000
   size  = 0x000020000000

class SP_REGION(MemoryRegion):
   paddr = 0x000040000000
   size  = 0x000040000000

class ESR_REGION(MemoryRegion):
   paddr = 0x000100000000
   size  = 0x000100000000

class RESERVED_REGION(MemoryRegion):
   paddr = 0x000200000000
   size  = 0x003E00000000

class MCODE_REGION(MemoryRegion):
   paddr = 0x008000000000
   size  = 0x000000800000

class OS_REGION(MemoryRegion):
   paddr = 0x008000800000
   size  = 0x0000FF800000

DRAM_REGION_HEAD = 0x008100000000
DRAM_REGION_TAIL = 0x008400000000

class ZERO_REGION(MemoryRegion):
   paddr = DRAM_REGION_HEAD
   size  = 128 # 2 cache lines

   @classmethod
   def dflt_value(cls):
      return 0

DRAM_REGION_HEAD = DRAM_REGION_HEAD + ZERO_REGION.size
if TEST_CONF.SEQUENTIAL_MODE  == 0x0:
   SCRATCH_REGION_SIZE = 64*TEST_CONF.NUM_HARTS
else:
   SCRATCH_REGION_SIZE = 64*TEST_CONF.SEQ_NUM_HARTS
class SCRATCH_REGION(MemoryRegion):
   paddr = DRAM_REGION_HEAD
   size  = SCRATCH_REGION_SIZE

DRAM_REGION_HEAD = DRAM_REGION_HEAD + SCRATCH_REGION.size

# 64 KBytes for CRC16 checking
# This corresponds to the bottom of the DRAM
CRC16_REGION_SIZE = int((TEST_CONF.NUM_COMPS/TEST_CONF.CHK_FREQ + 10)*(16/8)*TEST_CONF.NUM_HARTS)
CRC16_REGION_SIZE = CRC16_REGION_SIZE + (64-(CRC16_REGION_SIZE%64))
class CRC16_REGION(MemoryRegion):
   paddr = DRAM_REGION_HEAD
   size  = CRC16_REGION_SIZE

DRAM_REGION_HEAD = DRAM_REGION_HEAD + CRC16_REGION_SIZE
LHID_MAP_REGION_SIZE = (2*8*4*34)*2
LHID_MAP_REGION_SIZE = LHID_MAP_REGION_SIZE + (64-(LHID_MAP_REGION_SIZE%64))
class LHID_MAP_REGION(MemoryRegion):
   paddr = DRAM_REGION_HEAD
   size  = LHID_MAP_REGION_SIZE
   export = False

DRAM_REGION_HEAD = DRAM_REGION_HEAD + LHID_MAP_REGION_SIZE
class DRAM_REGION(MemoryRegion):
   paddr = DRAM_REGION_HEAD
   size  = DRAM_REGION_TAIL-DRAM_REGION_HEAD

class PCIE_REGION(MemoryRegion):
   paddr = 0x004000000000
   size  = 0x004000000000

class LOCAL_SCP_REGION(MemoryRegion):
   paddr  = 0x0000bf800000
   size   = L2SCP_CONF.mem_size
   export = False

class SCPMemoryRegion(MemoryRegion):
   def sanity_check(self, model: 'Model'):
      assert model.l2scp_initialized == True, "Trying to use L2 SCP before initializing it first"
      super().sanity_check(model)

# 32 Compute shires + 1 Master shire
NUM_SCP_SHIRES = 33
for i in range(NUM_SCP_SHIRES):
   name = f"SCP_SHIRE{i}_REGION"
   base_addr = 0x000080000000 + (0x0800000*i) + L2SCP_CONF.mem_head
   cls = type(name, (SCPMemoryRegion,), {'__module__': 'conf.mem.et_soc_memory_map', 'shireid' : i, 'paddr' : base_addr, 'size' : L2SCP_CONF.mem_size, 'export' : False})
   globals()[name] = cls
