SECTIONS
{ 
  /* by default, mprot configures size to be 16G */
  DRAM_END = 0x8400000000;
  HI_TEXT_SIZE = 32*4;
  HI_TEXT = DRAM_END - HI_TEXT_SIZE;

  .hi_text HI_TEXT : {*(.hi_text) }

}

INSERT AFTER stack
