
from conf.mem.erbium_memory_map import *
from conf.mem.erbium_esr import *
from core.models.memorymodel import MemorySet

class MIN_SHIRE_MEM_SET(MemorySet):
   mem_regions = {
      DRAM_REGION: 1
   }
