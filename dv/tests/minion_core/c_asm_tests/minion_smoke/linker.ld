INCLUDE "scratch_size.ld"

SECTIONS
{ 
  /* by default, mprot configures size to be 16G */
  DRAM_START = 0x40000000 + 32K;
  DRAM_END   = DRAM_START + 16M - 32K;
  /*DRAM_START = 0x1000000000;
  DRAM_END = DRAM_START + 16M; */
  GSC_OFFSETS_START = DRAM_START;
  TEST_START = DRAM_START + 1M;
  /* TEST_START = DRAM_START + 1M; */  
  SCRATCH_START = DRAM_END - SCRATCH_SIZE;

  .gsc_offsets GSC_OFFSETS_START : { *(.gsc_offsets) }
  .test TEST_START : { *(.test) }
  .scratch SCRATCH_START : {*(.scratch) }

}

INSERT AFTER .stack
