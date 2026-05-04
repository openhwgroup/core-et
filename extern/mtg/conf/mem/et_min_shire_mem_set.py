
from core.models.memorymodel import MemorySet
from conf.mem.et_soc_memory_map import *
from core.test_conf import TEST_CONF

class L2_SCP_MEM_SET(MemorySet):
   mem_regions = dict()

for i in range(NUM_SCP_SHIRES):
   bit = (TEST_CONF.SHIRE_MASK >> i) & 0x1
   if bit == 1:
      name = f"SCP_SHIRE{i}_REGION"
      cls = globals()[name]
      L2_SCP_MEM_SET.mem_regions[cls] = 1

class MIN_SHIRE_MEM_SET(MemorySet):
   mem_regions = {
      DRAM_REGION : 1
   }

if TEST_CONF.agnostic_mode() == False:
   MIN_SHIRE_MEM_SET.mem_regions[L2_SCP_MEM_SET] = 1
