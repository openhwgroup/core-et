class ral_reg_ms_mem_ctl extends uvm_reg;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL0;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL1;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL2;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL3;
   rand uvm_reg_field ESR_MS_ADDR_MC_BIT_SEL;
   rand uvm_reg_field ESR_MS_ADDR_DEBUG_MC_BIT_SEL;
   rand uvm_reg_field ESR_MS_ADDR_MASK;
   rand uvm_reg_field ESR_MS_MC_EN;

   function new(string name = "ms_mem_ctl");
      super.new(name, 56, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_MS_ADDR_REMOVE_BIT_SEL0 = uvm_reg_field::type_id::create("ESR_MS_ADDR_REMOVE_BIT_SEL0", , get_full_name());
      ESR_MS_ADDR_REMOVE_BIT_SEL0.configure(this, 6, 0, "RW", 0, 6'h6, 1, 0, 0);
      ESR_MS_ADDR_REMOVE_BIT_SEL1 = uvm_reg_field::type_id::create("ESR_MS_ADDR_REMOVE_BIT_SEL1", , get_full_name());
      ESR_MS_ADDR_REMOVE_BIT_SEL1.configure(this, 6, 6, "RW", 0, 6'h7, 1, 0, 0);
      ESR_MS_ADDR_REMOVE_BIT_SEL2 = uvm_reg_field::type_id::create("ESR_MS_ADDR_REMOVE_BIT_SEL2", , get_full_name());
      ESR_MS_ADDR_REMOVE_BIT_SEL2.configure(this, 6, 12, "RW", 0, 6'h8, 1, 0, 0);
      ESR_MS_ADDR_REMOVE_BIT_SEL3 = uvm_reg_field::type_id::create("ESR_MS_ADDR_REMOVE_BIT_SEL3", , get_full_name());
      ESR_MS_ADDR_REMOVE_BIT_SEL3.configure(this, 6, 18, "RW", 0, 6'h9, 1, 0, 0);
      ESR_MS_ADDR_MC_BIT_SEL = uvm_reg_field::type_id::create("ESR_MS_ADDR_MC_BIT_SEL", , get_full_name());
      ESR_MS_ADDR_MC_BIT_SEL.configure(this, 6, 24, "RW", 0, 6'h9, 1, 0, 0);
      ESR_MS_ADDR_DEBUG_MC_BIT_SEL = uvm_reg_field::type_id::create("ESR_MS_ADDR_DEBUG_MC_BIT_SEL", , get_full_name());
      ESR_MS_ADDR_DEBUG_MC_BIT_SEL.configure(this, 6, 30, "RW", 0, 6'h6, 1, 0, 0);
      ESR_MS_ADDR_MASK = uvm_reg_field::type_id::create("ESR_MS_ADDR_MASK", , get_full_name());
      ESR_MS_ADDR_MASK.configure(this, 12, 36, "RW", 0, 12'h07f, 1, 0, 0);
      ESR_MS_MC_EN = uvm_reg_field::type_id::create("ESR_MS_MC_EN", , get_full_name());
      ESR_MS_MC_EN.configure(this, 2, 48, "RW", 0, 2'h3, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ms_mem_ctl)

endclass : ral_reg_ms_mem_ctl


class ral_reg_ms_atomic_sm_ctl extends uvm_reg;
   rand uvm_reg_field ESR_MS_ATOMIC_EVICT_START;
   uvm_reg_field ESR_MS_ATOMIC_STATE;

   function new(string name = "ms_atomic_sm_ctl");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_MS_ATOMIC_EVICT_START = uvm_reg_field::type_id::create("ESR_MS_ATOMIC_EVICT_START", , get_full_name());
      ESR_MS_ATOMIC_EVICT_START.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MS_ATOMIC_STATE = uvm_reg_field::type_id::create("ESR_MS_ATOMIC_STATE", , get_full_name());
      ESR_MS_ATOMIC_STATE.configure(this, 4, 4, "RO", 0, 4'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ms_atomic_sm_ctl)

endclass : ral_reg_ms_atomic_sm_ctl


class ral_reg_ms_mem_revision_id extends uvm_reg;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B0;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B1;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B2;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B3;
   uvm_reg_field ESR_MEMSHIRE_ID;

   function new(string name = "ms_mem_revision_id");
      super.new(name, 48, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_MEMSHIRE_REVISION_B0 = uvm_reg_field::type_id::create("ESR_MEMSHIRE_REVISION_B0", , get_full_name());
      ESR_MEMSHIRE_REVISION_B0.configure(this, 8, 0, "RO", 0, 8'h1, 1, 0, 1);
      ESR_MEMSHIRE_REVISION_B1 = uvm_reg_field::type_id::create("ESR_MEMSHIRE_REVISION_B1", , get_full_name());
      ESR_MEMSHIRE_REVISION_B1.configure(this, 8, 8, "RO", 0, 8'h0, 1, 0, 1);
      ESR_MEMSHIRE_REVISION_B2 = uvm_reg_field::type_id::create("ESR_MEMSHIRE_REVISION_B2", , get_full_name());
      ESR_MEMSHIRE_REVISION_B2.configure(this, 8, 16, "RO", 0, 8'h0, 1, 0, 1);
      ESR_MEMSHIRE_REVISION_B3 = uvm_reg_field::type_id::create("ESR_MEMSHIRE_REVISION_B3", , get_full_name());
      ESR_MEMSHIRE_REVISION_B3.configure(this, 8, 24, "RO", 0, 8'h0, 1, 0, 1);
      ESR_MEMSHIRE_ID = uvm_reg_field::type_id::create("ESR_MEMSHIRE_ID", , get_full_name());
      ESR_MEMSHIRE_ID.configure(this, 8, 40, "RO", 0, 8'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ms_mem_revision_id)

endclass : ral_reg_ms_mem_revision_id


class ral_reg_ms_clk_gate_ctl extends uvm_reg;
   rand uvm_reg_field ESR_MS_DEBUG_CLOCK_DISABLE;

   function new(string name = "ms_clk_gate_ctl");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_MS_DEBUG_CLOCK_DISABLE = uvm_reg_field::type_id::create("ESR_MS_DEBUG_CLOCK_DISABLE", , get_full_name());
      ESR_MS_DEBUG_CLOCK_DISABLE.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ms_clk_gate_ctl)

endclass : ral_reg_ms_clk_gate_ctl


class ral_reg_ms_mem_status extends uvm_reg;
   rand uvm_reg_field ESR_MS_PLL_LOCK_DETECT;
   rand uvm_reg_field ESR_MS_PLL_LOCK_LOST;

   function new(string name = "ms_mem_status");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_MS_PLL_LOCK_DETECT = uvm_reg_field::type_id::create("ESR_MS_PLL_LOCK_DETECT", , get_full_name());
      ESR_MS_PLL_LOCK_DETECT.configure(this, 1, 0, "RO", 1, 1'h0, 1, 0, 0);
      ESR_MS_PLL_LOCK_LOST = uvm_reg_field::type_id::create("ESR_MS_PLL_LOCK_LOST", , get_full_name());
      ESR_MS_PLL_LOCK_LOST.configure(this, 1, 1, "RO", 1, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ms_mem_status)

endclass : ral_reg_ms_mem_status


class ral_reg_ddrc_reset_ctl extends uvm_reg;
   rand uvm_reg_field ESR_DDRC_SUBSYSTEM_RESET;
   rand uvm_reg_field ESR_DDRC_APB_RESET;
   rand uvm_reg_field ESR_DDRC_PUB_RESET;
   rand uvm_reg_field ESR_DDRC_POWER_OK;
   uvm_reg_field ESR_DDRC_RESET_CTL_RESERVED;
   rand uvm_reg_field ESR_DDRC_PLL_CLK_SEL;

   function new(string name = "ddrc_reset_ctl");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_SUBSYSTEM_RESET = uvm_reg_field::type_id::create("ESR_DDRC_SUBSYSTEM_RESET", , get_full_name());
      ESR_DDRC_SUBSYSTEM_RESET.configure(this, 1, 0, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_APB_RESET = uvm_reg_field::type_id::create("ESR_DDRC_APB_RESET", , get_full_name());
      ESR_DDRC_APB_RESET.configure(this, 1, 1, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_PUB_RESET = uvm_reg_field::type_id::create("ESR_DDRC_PUB_RESET", , get_full_name());
      ESR_DDRC_PUB_RESET.configure(this, 1, 2, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_POWER_OK = uvm_reg_field::type_id::create("ESR_DDRC_POWER_OK", , get_full_name());
      ESR_DDRC_POWER_OK.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_RESET_CTL_RESERVED = uvm_reg_field::type_id::create("ESR_DDRC_RESET_CTL_RESERVED", , get_full_name());
      ESR_DDRC_RESET_CTL_RESERVED.configure(this, 4, 4, "RO", 0, 4'h0, 1, 0, 0);
      ESR_DDRC_PLL_CLK_SEL = uvm_reg_field::type_id::create("ESR_DDRC_PLL_CLK_SEL", , get_full_name());
      ESR_DDRC_PLL_CLK_SEL.configure(this, 1, 8, "RW", 0, 1'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_reset_ctl)

endclass : ral_reg_ddrc_reset_ctl


class ral_reg_ddrc_clock_ctl extends uvm_reg;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_DDRC;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_DDRC;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_DDRC;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_DDRC;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_DDRC;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_DDRC;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_LO;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_LO;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_LO;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_LO;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_LO;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_LO;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_HI;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_HI;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_HI;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_HI;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_HI;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_HI;

   function new(string name = "ddrc_clock_ctl");
      super.new(name, 24, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_U0_CACTIVE_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U0_CACTIVE_DDRC", , get_full_name());
      ESR_DDRC_U0_CACTIVE_DDRC.configure(this, 1, 0, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSACK_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSACK_DDRC", , get_full_name());
      ESR_DDRC_U0_CSYSACK_DDRC.configure(this, 1, 1, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSREQ_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSREQ_DDRC", , get_full_name());
      ESR_DDRC_U0_CSYSREQ_DDRC.configure(this, 1, 2, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_U1_CACTIVE_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U1_CACTIVE_DDRC", , get_full_name());
      ESR_DDRC_U1_CACTIVE_DDRC.configure(this, 1, 3, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSACK_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSACK_DDRC", , get_full_name());
      ESR_DDRC_U1_CSYSACK_DDRC.configure(this, 1, 4, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSREQ_DDRC = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSREQ_DDRC", , get_full_name());
      ESR_DDRC_U1_CSYSREQ_DDRC.configure(this, 1, 5, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_U0_CACTIVE_LO = uvm_reg_field::type_id::create("ESR_DDRC_U0_CACTIVE_LO", , get_full_name());
      ESR_DDRC_U0_CACTIVE_LO.configure(this, 1, 6, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSACK_LO = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSACK_LO", , get_full_name());
      ESR_DDRC_U0_CSYSACK_LO.configure(this, 1, 7, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSREQ_LO = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSREQ_LO", , get_full_name());
      ESR_DDRC_U0_CSYSREQ_LO.configure(this, 1, 8, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_U1_CACTIVE_LO = uvm_reg_field::type_id::create("ESR_DDRC_U1_CACTIVE_LO", , get_full_name());
      ESR_DDRC_U1_CACTIVE_LO.configure(this, 1, 9, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSACK_LO = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSACK_LO", , get_full_name());
      ESR_DDRC_U1_CSYSACK_LO.configure(this, 1, 10, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSREQ_LO = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSREQ_LO", , get_full_name());
      ESR_DDRC_U1_CSYSREQ_LO.configure(this, 1, 11, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_U0_CACTIVE_HI = uvm_reg_field::type_id::create("ESR_DDRC_U0_CACTIVE_HI", , get_full_name());
      ESR_DDRC_U0_CACTIVE_HI.configure(this, 1, 12, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSACK_HI = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSACK_HI", , get_full_name());
      ESR_DDRC_U0_CSYSACK_HI.configure(this, 1, 13, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U0_CSYSREQ_HI = uvm_reg_field::type_id::create("ESR_DDRC_U0_CSYSREQ_HI", , get_full_name());
      ESR_DDRC_U0_CSYSREQ_HI.configure(this, 1, 14, "RW", 0, 1'h1, 1, 0, 0);
      ESR_DDRC_U1_CACTIVE_HI = uvm_reg_field::type_id::create("ESR_DDRC_U1_CACTIVE_HI", , get_full_name());
      ESR_DDRC_U1_CACTIVE_HI.configure(this, 1, 15, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSACK_HI = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSACK_HI", , get_full_name());
      ESR_DDRC_U1_CSYSACK_HI.configure(this, 1, 16, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_CSYSREQ_HI = uvm_reg_field::type_id::create("ESR_DDRC_U1_CSYSREQ_HI", , get_full_name());
      ESR_DDRC_U1_CSYSREQ_HI.configure(this, 1, 17, "RW", 0, 1'h1, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_clock_ctl)

endclass : ral_reg_ddrc_clock_ctl


class ral_reg_ddrc_main_ctl extends uvm_reg;
   rand uvm_reg_field ESR_DDRC_WRITE_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ESR_DDRC_WRITE_AUTO_PRECHARGE_HI;
   rand uvm_reg_field ESR_DDRC_READ_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ESR_DDRC_READ_AUTO_PRECHARGE_HI;

   function new(string name = "ddrc_main_ctl");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_WRITE_AUTO_PRECHARGE_LO = uvm_reg_field::type_id::create("ESR_DDRC_WRITE_AUTO_PRECHARGE_LO", , get_full_name());
      ESR_DDRC_WRITE_AUTO_PRECHARGE_LO.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_WRITE_AUTO_PRECHARGE_HI = uvm_reg_field::type_id::create("ESR_DDRC_WRITE_AUTO_PRECHARGE_HI", , get_full_name());
      ESR_DDRC_WRITE_AUTO_PRECHARGE_HI.configure(this, 1, 1, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_READ_AUTO_PRECHARGE_LO = uvm_reg_field::type_id::create("ESR_DDRC_READ_AUTO_PRECHARGE_LO", , get_full_name());
      ESR_DDRC_READ_AUTO_PRECHARGE_LO.configure(this, 1, 2, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_READ_AUTO_PRECHARGE_HI = uvm_reg_field::type_id::create("ESR_DDRC_READ_AUTO_PRECHARGE_HI", , get_full_name());
      ESR_DDRC_READ_AUTO_PRECHARGE_HI.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_main_ctl)

endclass : ral_reg_ddrc_main_ctl


class ral_reg_ddrc_scrub1 extends uvm_reg;
   rand uvm_reg_field ESR_DDRC_SCRUB_ADDR_START_MASK;

   function new(string name = "ddrc_scrub1");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_SCRUB_ADDR_START_MASK = uvm_reg_field::type_id::create("ESR_DDRC_SCRUB_ADDR_START_MASK", , get_full_name());
      ESR_DDRC_SCRUB_ADDR_START_MASK.configure(this, 36, 0, "RW", 0, 36'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_scrub1)

endclass : ral_reg_ddrc_scrub1


class ral_reg_ddrc_scrub2 extends uvm_reg;
   rand uvm_reg_field ESR_DDRC_SCRUB_ADDR_RANGE_MASK;

   function new(string name = "ddrc_scrub2");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_SCRUB_ADDR_RANGE_MASK = uvm_reg_field::type_id::create("ESR_DDRC_SCRUB_ADDR_RANGE_MASK", , get_full_name());
      ESR_DDRC_SCRUB_ADDR_RANGE_MASK.configure(this, 36, 0, "RW", 0, 36'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_scrub2)

endclass : ral_reg_ddrc_scrub2


class ral_reg_ddrc_u0_mrr_data extends uvm_reg;
   uvm_reg_field ESR_DDRC_U0_MRR_DATA;

   function new(string name = "ddrc_u0_mrr_data");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_U0_MRR_DATA = uvm_reg_field::type_id::create("ESR_DDRC_U0_MRR_DATA", , get_full_name());
      ESR_DDRC_U0_MRR_DATA.configure(this, 16, 0, "RO", 0, 16'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_u0_mrr_data)

endclass : ral_reg_ddrc_u0_mrr_data


class ral_reg_ddrc_u1_mrr_data extends uvm_reg;
   uvm_reg_field ESR_DDRC_U1_MRR_DATA;

   function new(string name = "ddrc_u1_mrr_data");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_U1_MRR_DATA = uvm_reg_field::type_id::create("ESR_DDRC_U1_MRR_DATA", , get_full_name());
      ESR_DDRC_U1_MRR_DATA.configure(this, 16, 0, "RO", 0, 16'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_u1_mrr_data)

endclass : ral_reg_ddrc_u1_mrr_data


class ral_reg_ddrc_mrr_status extends uvm_reg;
   rand uvm_reg_field ESR_DDRC_U0_MRR_DATA_VALID;
   rand uvm_reg_field ESR_DDRC_U1_MRR_DATA_VALID;

   function new(string name = "ddrc_mrr_status");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DDRC_U0_MRR_DATA_VALID = uvm_reg_field::type_id::create("ESR_DDRC_U0_MRR_DATA_VALID", , get_full_name());
      ESR_DDRC_U0_MRR_DATA_VALID.configure(this, 1, 0, "WC", 0, 1'h0, 1, 0, 0);
      ESR_DDRC_U1_MRR_DATA_VALID = uvm_reg_field::type_id::create("ESR_DDRC_U1_MRR_DATA_VALID", , get_full_name());
      ESR_DDRC_U1_MRR_DATA_VALID.configure(this, 1, 1, "WC", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_mrr_status)

endclass : ral_reg_ddrc_mrr_status


class ral_reg_ddrc_int_status extends uvm_reg;
   uvm_reg_field ESR_CYC_COUNT_OVERFLOW_INTR;
   uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_INTR;
   uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_INTR;
   uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ESR_MC1_SBR_DONE_INTR;
   uvm_reg_field ESR_MC0_SBR_DONE_INTR;
   uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ESR_DDRPHY_INTR;
   uvm_reg_field ESR_DFI1_ERR_INTR;
   uvm_reg_field ESR_DFI0_ERR_INTR;
   uvm_reg_field ESR_MC1_DFI_ALERT_ERR_INTR;
   uvm_reg_field ESR_MC0_DFI_ALERT_ERR_INTR;
   uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_INTR;

   function new(string name = "ddrc_int_status");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_CYC_COUNT_OVERFLOW_INTR = uvm_reg_field::type_id::create("ESR_CYC_COUNT_OVERFLOW_INTR", , get_full_name());
      ESR_CYC_COUNT_OVERFLOW_INTR.configure(this, 1, 15, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT1_OVERFLOW_INTR = uvm_reg_field::type_id::create("ESR_PERF_COUNT1_OVERFLOW_INTR", , get_full_name());
      ESR_PERF_COUNT1_OVERFLOW_INTR.configure(this, 1, 14, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT0_OVERFLOW_INTR = uvm_reg_field::type_id::create("ESR_PERF_COUNT0_OVERFLOW_INTR", , get_full_name());
      ESR_PERF_COUNT0_OVERFLOW_INTR.configure(this, 1, 13, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DERATE_TEMP_LIMIT_INTR = uvm_reg_field::type_id::create("ESR_MC1_DERATE_TEMP_LIMIT_INTR", , get_full_name());
      ESR_MC1_DERATE_TEMP_LIMIT_INTR.configure(this, 1, 12, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DERATE_TEMP_LIMIT_INTR = uvm_reg_field::type_id::create("ESR_MC0_DERATE_TEMP_LIMIT_INTR", , get_full_name());
      ESR_MC0_DERATE_TEMP_LIMIT_INTR.configure(this, 1, 11, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC1_SBR_DONE_INTR = uvm_reg_field::type_id::create("ESR_MC1_SBR_DONE_INTR", , get_full_name());
      ESR_MC1_SBR_DONE_INTR.configure(this, 1, 10, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC0_SBR_DONE_INTR = uvm_reg_field::type_id::create("ESR_MC0_SBR_DONE_INTR", , get_full_name());
      ESR_MC0_SBR_DONE_INTR.configure(this, 1, 9, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_CORRECTED_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC1_ECC_CORRECTED_ERR_INTR", , get_full_name());
      ESR_MC1_ECC_CORRECTED_ERR_INTR.configure(this, 1, 8, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_CORRECTED_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC0_ECC_CORRECTED_ERR_INTR", , get_full_name());
      ESR_MC0_ECC_CORRECTED_ERR_INTR.configure(this, 1, 7, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DDRPHY_INTR = uvm_reg_field::type_id::create("ESR_DDRPHY_INTR", , get_full_name());
      ESR_DDRPHY_INTR.configure(this, 1, 6, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DFI1_ERR_INTR = uvm_reg_field::type_id::create("ESR_DFI1_ERR_INTR", , get_full_name());
      ESR_DFI1_ERR_INTR.configure(this, 1, 5, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR_INTR = uvm_reg_field::type_id::create("ESR_DFI0_ERR_INTR", , get_full_name());
      ESR_DFI0_ERR_INTR.configure(this, 1, 4, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DFI_ALERT_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC1_DFI_ALERT_ERR_INTR", , get_full_name());
      ESR_MC1_DFI_ALERT_ERR_INTR.configure(this, 1, 3, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DFI_ALERT_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC0_DFI_ALERT_ERR_INTR", , get_full_name());
      ESR_MC0_DFI_ALERT_ERR_INTR.configure(this, 1, 2, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_UNCORRECTED_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC1_ECC_UNCORRECTED_ERR_INTR", , get_full_name());
      ESR_MC1_ECC_UNCORRECTED_ERR_INTR.configure(this, 1, 1, "RO", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_UNCORRECTED_ERR_INTR = uvm_reg_field::type_id::create("ESR_MC0_ECC_UNCORRECTED_ERR_INTR", , get_full_name());
      ESR_MC0_ECC_UNCORRECTED_ERR_INTR.configure(this, 1, 0, "RO", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_int_status)

endclass : ral_reg_ddrc_int_status


class ral_reg_ddrc_int_critical_en extends uvm_reg;
   rand uvm_reg_field ESR_CYC_COUNT_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ESR_MC1_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ESR_MC0_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_DDRPHY_CRIT_EN;
   rand uvm_reg_field ESR_DFI1_ERR_CRIT_EN;
   rand uvm_reg_field ESR_DFI0_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC1_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN;

   function new(string name = "ddrc_int_critical_en");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_CYC_COUNT_OVERFLOW_CRIT_EN = uvm_reg_field::type_id::create("ESR_CYC_COUNT_OVERFLOW_CRIT_EN", , get_full_name());
      ESR_CYC_COUNT_OVERFLOW_CRIT_EN.configure(this, 1, 15, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT1_OVERFLOW_CRIT_EN = uvm_reg_field::type_id::create("ESR_PERF_COUNT1_OVERFLOW_CRIT_EN", , get_full_name());
      ESR_PERF_COUNT1_OVERFLOW_CRIT_EN.configure(this, 1, 14, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT0_OVERFLOW_CRIT_EN = uvm_reg_field::type_id::create("ESR_PERF_COUNT0_OVERFLOW_CRIT_EN", , get_full_name());
      ESR_PERF_COUNT0_OVERFLOW_CRIT_EN.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN", , get_full_name());
      ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN.configure(this, 1, 12, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN", , get_full_name());
      ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN.configure(this, 1, 11, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_SBR_DONE_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC1_SBR_DONE_CRIT_EN", , get_full_name());
      ESR_MC1_SBR_DONE_CRIT_EN.configure(this, 1, 10, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_SBR_DONE_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC0_SBR_DONE_CRIT_EN", , get_full_name());
      ESR_MC0_SBR_DONE_CRIT_EN.configure(this, 1, 9, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN", , get_full_name());
      ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN.configure(this, 1, 8, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN", , get_full_name());
      ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN.configure(this, 1, 7, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRPHY_CRIT_EN = uvm_reg_field::type_id::create("ESR_DDRPHY_CRIT_EN", , get_full_name());
      ESR_DDRPHY_CRIT_EN.configure(this, 1, 6, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DFI1_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_DFI1_ERR_CRIT_EN", , get_full_name());
      ESR_DFI1_ERR_CRIT_EN.configure(this, 1, 5, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_DFI0_ERR_CRIT_EN", , get_full_name());
      ESR_DFI0_ERR_CRIT_EN.configure(this, 1, 4, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DFI_ALERT_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC1_DFI_ALERT_ERR_CRIT_EN", , get_full_name());
      ESR_MC1_DFI_ALERT_ERR_CRIT_EN.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DFI_ALERT_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC0_DFI_ALERT_ERR_CRIT_EN", , get_full_name());
      ESR_MC0_DFI_ALERT_ERR_CRIT_EN.configure(this, 1, 2, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN", , get_full_name());
      ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN.configure(this, 1, 1, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN = uvm_reg_field::type_id::create("ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN", , get_full_name());
      ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_int_critical_en)

endclass : ral_reg_ddrc_int_critical_en


class ral_reg_ddrc_int_normal_en extends uvm_reg;
   rand uvm_reg_field ESR_CYC_COUNT_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ESR_MC1_SBR_DONE_NORM_EN;
   rand uvm_reg_field ESR_MC0_SBR_DONE_NORM_EN;
   rand uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_DDRPHY_NORM_EN;
   rand uvm_reg_field ESR_DFI1_ERR_NORM_EN;
   rand uvm_reg_field ESR_DFI0_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC1_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN;

   function new(string name = "ddrc_int_normal_en");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_CYC_COUNT_OVERFLOW_NORM_EN = uvm_reg_field::type_id::create("ESR_CYC_COUNT_OVERFLOW_NORM_EN", , get_full_name());
      ESR_CYC_COUNT_OVERFLOW_NORM_EN.configure(this, 1, 15, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT1_OVERFLOW_NORM_EN = uvm_reg_field::type_id::create("ESR_PERF_COUNT1_OVERFLOW_NORM_EN", , get_full_name());
      ESR_PERF_COUNT1_OVERFLOW_NORM_EN.configure(this, 1, 14, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERF_COUNT0_OVERFLOW_NORM_EN = uvm_reg_field::type_id::create("ESR_PERF_COUNT0_OVERFLOW_NORM_EN", , get_full_name());
      ESR_PERF_COUNT0_OVERFLOW_NORM_EN.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN = uvm_reg_field::type_id::create("ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN", , get_full_name());
      ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN.configure(this, 1, 12, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN = uvm_reg_field::type_id::create("ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN", , get_full_name());
      ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN.configure(this, 1, 11, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_SBR_DONE_NORM_EN = uvm_reg_field::type_id::create("ESR_MC1_SBR_DONE_NORM_EN", , get_full_name());
      ESR_MC1_SBR_DONE_NORM_EN.configure(this, 1, 10, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_SBR_DONE_NORM_EN = uvm_reg_field::type_id::create("ESR_MC0_SBR_DONE_NORM_EN", , get_full_name());
      ESR_MC0_SBR_DONE_NORM_EN.configure(this, 1, 9, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_CORRECTED_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC1_ECC_CORRECTED_ERR_NORM_EN", , get_full_name());
      ESR_MC1_ECC_CORRECTED_ERR_NORM_EN.configure(this, 1, 8, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_CORRECTED_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC0_ECC_CORRECTED_ERR_NORM_EN", , get_full_name());
      ESR_MC0_ECC_CORRECTED_ERR_NORM_EN.configure(this, 1, 7, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DDRPHY_NORM_EN = uvm_reg_field::type_id::create("ESR_DDRPHY_NORM_EN", , get_full_name());
      ESR_DDRPHY_NORM_EN.configure(this, 1, 6, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DFI1_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_DFI1_ERR_NORM_EN", , get_full_name());
      ESR_DFI1_ERR_NORM_EN.configure(this, 1, 5, "RW", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_DFI0_ERR_NORM_EN", , get_full_name());
      ESR_DFI0_ERR_NORM_EN.configure(this, 1, 4, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_DFI_ALERT_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC1_DFI_ALERT_ERR_NORM_EN", , get_full_name());
      ESR_MC1_DFI_ALERT_ERR_NORM_EN.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_DFI_ALERT_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC0_DFI_ALERT_ERR_NORM_EN", , get_full_name());
      ESR_MC0_DFI_ALERT_ERR_NORM_EN.configure(this, 1, 2, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN", , get_full_name());
      ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN.configure(this, 1, 1, "RW", 0, 1'h0, 1, 0, 0);
      ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN = uvm_reg_field::type_id::create("ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN", , get_full_name());
      ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_int_normal_en)

endclass : ral_reg_ddrc_int_normal_en


class ral_reg_ddrc_err_int_log extends uvm_reg;
	rand uvm_reg_field ESR_DFI1_ERR_INFO;
	rand uvm_reg_field ESR_DFI1_ERR_SET;
	rand uvm_reg_field ESR_DFI1_ERR_CLR;
	uvm_reg_field ESR_DFI1_ERR;
	rand uvm_reg_field ESR_DFI0_ERR_INFO;
	rand uvm_reg_field ESR_DFI0_ERR_SET;
	rand uvm_reg_field ESR_DFI0_ERR_CLR;
	uvm_reg_field ESR_DFI0_ERR;

	function new(string name = "ddrc_err_int_log");
		super.new(name, 16, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_DFI1_ERR_INFO = uvm_reg_field::type_id::create("ESR_DFI1_ERR_INFO", , get_full_name());
      ESR_DFI1_ERR_INFO.configure(this, 4, 12, "RW", 0, 4'h0, 1, 0, 0);
      ESR_DFI1_ERR_SET = uvm_reg_field::type_id::create("ESR_DFI1_ERR_SET", , get_full_name());
      ESR_DFI1_ERR_SET.configure(this, 1, 10, "W1C", 0, 1'h0, 1, 0, 0);
      ESR_DFI1_ERR_CLR = uvm_reg_field::type_id::create("ESR_DFI1_ERR_CLR", , get_full_name());
      ESR_DFI1_ERR_CLR.configure(this, 1, 9, "W1C", 0, 1'h0, 1, 0, 0);
      ESR_DFI1_ERR = uvm_reg_field::type_id::create("ESR_DFI1_ERR", , get_full_name());
      ESR_DFI1_ERR.configure(this, 1, 8, "RO", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR_INFO = uvm_reg_field::type_id::create("ESR_DFI0_ERR_INFO", , get_full_name());
      ESR_DFI0_ERR_INFO.configure(this, 4, 4, "RW", 0, 4'h0, 1, 0, 0);
      ESR_DFI0_ERR_SET = uvm_reg_field::type_id::create("ESR_DFI0_ERR_SET", , get_full_name());
      ESR_DFI0_ERR_SET.configure(this, 1, 2, "W1C", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR_CLR = uvm_reg_field::type_id::create("ESR_DFI0_ERR_CLR", , get_full_name());
      ESR_DFI0_ERR_CLR.configure(this, 1, 1, "W1C", 0, 1'h0, 1, 0, 0);
      ESR_DFI0_ERR = uvm_reg_field::type_id::create("ESR_DFI0_ERR", , get_full_name());
      ESR_DFI0_ERR.configure(this, 1, 0, "RO", 0, 1'h0, 1, 0, 0);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_err_int_log)

endclass : ral_reg_ddrc_err_int_log


class ral_reg_ddrc_debug_sigs_mask0 extends uvm_reg;
   rand uvm_reg_field ESR_DEBUG_SIGS_MASK0;

   function new(string name = "ddrc_debug_sigs_mask0");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DEBUG_SIGS_MASK0 = uvm_reg_field::type_id::create("ESR_DEBUG_SIGS_MASK0", , get_full_name());
      ESR_DEBUG_SIGS_MASK0.configure(this, 64, 0, "RW", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_debug_sigs_mask0)

endclass : ral_reg_ddrc_debug_sigs_mask0


class ral_reg_ddrc_debug_sigs_mask1 extends uvm_reg;
   rand uvm_reg_field ESR_DEBUG_SIGS_MASK1;

   function new(string name = "ddrc_debug_sigs_mask1");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_DEBUG_SIGS_MASK1 = uvm_reg_field::type_id::create("ESR_DEBUG_SIGS_MASK1", , get_full_name());
      ESR_DEBUG_SIGS_MASK1.configure(this, 64, 0, "RW", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_debug_sigs_mask1)

endclass : ral_reg_ddrc_debug_sigs_mask1


class ral_reg_ddrc_scratch extends uvm_reg;
   rand uvm_reg_field ESR_SCRATCH;

   function new(string name = "ddrc_scratch");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SCRATCH = uvm_reg_field::type_id::create("ESR_SCRATCH", , get_full_name());
      ESR_SCRATCH.configure(this, 64, 0, "RW", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_scratch)

endclass : ral_reg_ddrc_scratch


class ral_reg_ddrc_trace_ctl extends uvm_reg;
   rand uvm_reg_field ESR_PERF_OP_SIGS_MASK;
   rand uvm_reg_field ESR_TRACE_EN;

   function new(string name = "ddrc_trace_ctl");
      super.new(name, 32, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_PERF_OP_SIGS_MASK = uvm_reg_field::type_id::create("ESR_PERF_OP_SIGS_MASK", , get_full_name());
      ESR_PERF_OP_SIGS_MASK.configure(this, 24, 8, "RW", 0, 24'h0, 1, 0, 1);
      ESR_TRACE_EN = uvm_reg_field::type_id::create("ESR_TRACE_EN", , get_full_name());
      ESR_TRACE_EN.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_ddrc_trace_ctl)

endclass : ral_reg_ddrc_trace_ctl


class ral_reg_ddrc_perfmon_ctl_status extends uvm_reg;
	uvm_reg_field ESR_P1_SO;
	uvm_reg_field ESR_P1_SA;
	uvm_reg_field ESR_P0_SO;
	uvm_reg_field ESR_P0_SA;
	uvm_reg_field ESR_CYC_SO;
	uvm_reg_field ESR_CYC_SA;
	uvm_reg_field ESR_AO;
	rand uvm_reg_field ESR_P1_MODE;
	rand uvm_reg_field ESR_P1_E;
	rand uvm_reg_field ESR_P1_L;
	rand uvm_reg_field ESR_P1_O;
	rand uvm_reg_field ESR_P1_R;
	rand uvm_reg_field ESR_P1_S;
	rand uvm_reg_field ESR_P0_MODE;
	rand uvm_reg_field ESR_P0_E;
	rand uvm_reg_field ESR_P0_L;
	rand uvm_reg_field ESR_P0_O;
	rand uvm_reg_field ESR_P0_R;
	rand uvm_reg_field ESR_P0_S;
	rand uvm_reg_field ESR_CYC_L;
	rand uvm_reg_field ESR_CYC_O;
	rand uvm_reg_field ESR_CYC_R;
	rand uvm_reg_field ESR_CYC_S;

	function new(string name = "ddrc_perfmon_ctl_status");
		super.new(name, 40, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P1_SO = uvm_reg_field::type_id::create("ESR_P1_SO", , get_full_name());
      ESR_P1_SO.configure(this, 1, 37, "RO", 0, 1'h0, 1, 0, 0);
      ESR_P1_SA = uvm_reg_field::type_id::create("ESR_P1_SA", , get_full_name());
      ESR_P1_SA.configure(this, 1, 36, "RO", 0, 1'h0, 1, 0, 0);
      ESR_P0_SO = uvm_reg_field::type_id::create("ESR_P0_SO", , get_full_name());
      ESR_P0_SO.configure(this, 1, 35, "RO", 0, 1'h0, 1, 0, 0);
      ESR_P0_SA = uvm_reg_field::type_id::create("ESR_P0_SA", , get_full_name());
      ESR_P0_SA.configure(this, 1, 34, "RO", 0, 1'h0, 1, 0, 0);
      ESR_CYC_SO = uvm_reg_field::type_id::create("ESR_CYC_SO", , get_full_name());
      ESR_CYC_SO.configure(this, 1, 33, "RO", 0, 1'h0, 1, 0, 0);
      ESR_CYC_SA = uvm_reg_field::type_id::create("ESR_CYC_SA", , get_full_name());
      ESR_CYC_SA.configure(this, 1, 32, "RO", 0, 1'h0, 1, 0, 0);
      ESR_AO = uvm_reg_field::type_id::create("ESR_AO", , get_full_name());
      ESR_AO.configure(this, 1, 30, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P1_MODE = uvm_reg_field::type_id::create("ESR_P1_MODE", , get_full_name());
      ESR_P1_MODE.configure(this, 8, 22, "RW", 0, 8'h0, 1, 0, 0);
      ESR_P1_E = uvm_reg_field::type_id::create("ESR_P1_E", , get_full_name());
      ESR_P1_E.configure(this, 1, 21, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P1_L = uvm_reg_field::type_id::create("ESR_P1_L", , get_full_name());
      ESR_P1_L.configure(this, 1, 20, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P1_O = uvm_reg_field::type_id::create("ESR_P1_O", , get_full_name());
      ESR_P1_O.configure(this, 1, 19, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P1_R = uvm_reg_field::type_id::create("ESR_P1_R", , get_full_name());
      ESR_P1_R.configure(this, 1, 18, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P1_S = uvm_reg_field::type_id::create("ESR_P1_S", , get_full_name());
      ESR_P1_S.configure(this, 1, 17, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P0_MODE = uvm_reg_field::type_id::create("ESR_P0_MODE", , get_full_name());
      ESR_P0_MODE.configure(this, 8, 9, "RW", 0, 8'h0, 1, 0, 0);
      ESR_P0_E = uvm_reg_field::type_id::create("ESR_P0_E", , get_full_name());
      ESR_P0_E.configure(this, 1, 8, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P0_L = uvm_reg_field::type_id::create("ESR_P0_L", , get_full_name());
      ESR_P0_L.configure(this, 1, 7, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P0_O = uvm_reg_field::type_id::create("ESR_P0_O", , get_full_name());
      ESR_P0_O.configure(this, 1, 6, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P0_R = uvm_reg_field::type_id::create("ESR_P0_R", , get_full_name());
      ESR_P0_R.configure(this, 1, 5, "RW", 0, 1'h0, 1, 0, 0);
      ESR_P0_S = uvm_reg_field::type_id::create("ESR_P0_S", , get_full_name());
      ESR_P0_S.configure(this, 1, 4, "RW", 0, 1'h0, 1, 0, 0);
      ESR_CYC_L = uvm_reg_field::type_id::create("ESR_CYC_L", , get_full_name());
      ESR_CYC_L.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
      ESR_CYC_O = uvm_reg_field::type_id::create("ESR_CYC_O", , get_full_name());
      ESR_CYC_O.configure(this, 1, 2, "RW", 0, 1'h0, 1, 0, 0);
      ESR_CYC_R = uvm_reg_field::type_id::create("ESR_CYC_R", , get_full_name());
      ESR_CYC_R.configure(this, 1, 1, "RW", 0, 1'h0, 1, 0, 0);
      ESR_CYC_S = uvm_reg_field::type_id::create("ESR_CYC_S", , get_full_name());
      ESR_CYC_S.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_ctl_status)

endclass : ral_reg_ddrc_perfmon_ctl_status


class ral_reg_ddrc_perfmon_cyc_cntr extends uvm_reg;
	rand uvm_reg_field ESR_CYC_CNTR;

	function new(string name = "ddrc_perfmon_cyc_cntr");
		super.new(name, 40, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_CYC_CNTR = uvm_reg_field::type_id::create("ESR_CYC_CNTR", , get_full_name());
      ESR_CYC_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_cyc_cntr)

endclass : ral_reg_ddrc_perfmon_cyc_cntr


class ral_reg_ddrc_perfmon_p0_cntr extends uvm_reg;
	rand uvm_reg_field ESR_P0_CNTR;

	function new(string name = "ddrc_perfmon_p0_cntr");
		super.new(name, 40, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P0_CNTR = uvm_reg_field::type_id::create("ESR_P0_CNTR", , get_full_name());
      ESR_P0_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p0_cntr)

endclass : ral_reg_ddrc_perfmon_p0_cntr


class ral_reg_ddrc_perfmon_p1_cntr extends uvm_reg;
	rand uvm_reg_field ESR_P1_CNTR;

	function new(string name = "ddrc_perfmon_p1_cntr");
		super.new(name, 40, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P1_CNTR = uvm_reg_field::type_id::create("ESR_P1_CNTR", , get_full_name());
      ESR_P1_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p1_cntr)

endclass : ral_reg_ddrc_perfmon_p1_cntr


class ral_reg_ddrc_perfmon_p0_qual extends uvm_reg;
	rand uvm_reg_field ESR_P0_QUAL;

	function new(string name = "ddrc_perfmon_p0_qual");
		super.new(name, 64, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P0_QUAL = uvm_reg_field::type_id::create("ESR_P0_QUAL", , get_full_name());
      ESR_P0_QUAL.configure(this, 64, 0, "RW", 0, 64'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p0_qual)

endclass : ral_reg_ddrc_perfmon_p0_qual


class ral_reg_ddrc_perfmon_p1_qual extends uvm_reg;
	rand uvm_reg_field ESR_P1_QUAL;

	function new(string name = "ddrc_perfmon_p1_qual");
		super.new(name, 64, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P1_QUAL = uvm_reg_field::type_id::create("ESR_P1_QUAL", , get_full_name());
      ESR_P1_QUAL.configure(this, 64, 0, "RW", 0, 64'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p1_qual)

endclass : ral_reg_ddrc_perfmon_p1_qual


class ral_reg_ddrc_perfmon_p0_qual2 extends uvm_reg;
	rand uvm_reg_field ESR_P0_QUAL2;

	function new(string name = "ddrc_perfmon_p0_qual2");
		super.new(name, 32, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P0_QUAL2 = uvm_reg_field::type_id::create("ESR_P0_QUAL2", , get_full_name());
      ESR_P0_QUAL2.configure(this, 32, 0, "RW", 0, 32'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p0_qual2)

endclass : ral_reg_ddrc_perfmon_p0_qual2


class ral_reg_ddrc_perfmon_p1_qual2 extends uvm_reg;
	rand uvm_reg_field ESR_P1_QUAL2;

	function new(string name = "ddrc_perfmon_p1_qual2");
		super.new(name, 32, UVM_NO_COVERAGE);
	endfunction: new

   virtual function void build();
      ESR_P1_QUAL2 = uvm_reg_field::type_id::create("ESR_P1_QUAL2", , get_full_name());
      ESR_P1_QUAL2.configure(this, 32, 0, "RW", 0, 32'h0, 1, 0, 1);
   endfunction: build

	`uvm_object_utils(ral_reg_ddrc_perfmon_p1_qual2)

endclass : ral_reg_ddrc_perfmon_p1_qual2


class ral_block_ms_regs extends evl_reg_block;
   rand ral_reg_ms_mem_ctl ms_mem_ctl;
   rand ral_reg_ms_atomic_sm_ctl ms_atomic_sm_ctl;
   rand ral_reg_ms_mem_revision_id ms_mem_revision_id;
   rand ral_reg_ms_clk_gate_ctl ms_clk_gate_ctl;
   rand ral_reg_ms_mem_status ms_mem_status;
   rand ral_reg_ddrc_reset_ctl ddrc_reset_ctl;
   rand ral_reg_ddrc_clock_ctl ddrc_clock_ctl;
   rand ral_reg_ddrc_main_ctl ddrc_main_ctl;
   rand ral_reg_ddrc_scrub1 ddrc_scrub1;
   rand ral_reg_ddrc_scrub2 ddrc_scrub2;
   rand ral_reg_ddrc_u0_mrr_data ddrc_u0_mrr_data;
   rand ral_reg_ddrc_u1_mrr_data ddrc_u1_mrr_data;
   rand ral_reg_ddrc_mrr_status ddrc_mrr_status;
   rand ral_reg_ddrc_int_status ddrc_int_status;
   rand ral_reg_ddrc_int_critical_en ddrc_int_critical_en;
   rand ral_reg_ddrc_int_normal_en ddrc_int_normal_en;
   rand ral_reg_ddrc_err_int_log ddrc_err_int_log;
   rand ral_reg_ddrc_debug_sigs_mask0 ddrc_debug_sigs_mask0;
   rand ral_reg_ddrc_debug_sigs_mask1 ddrc_debug_sigs_mask1;
   rand ral_reg_ddrc_scratch ddrc_scratch;
   rand ral_reg_ddrc_trace_ctl ddrc_trace_ctl;
   rand ral_reg_ddrc_perfmon_ctl_status ddrc_perfmon_ctl_status;
   rand ral_reg_ddrc_perfmon_cyc_cntr ddrc_perfmon_cyc_cntr;
   rand ral_reg_ddrc_perfmon_p0_cntr ddrc_perfmon_p0_cntr;
   rand ral_reg_ddrc_perfmon_p1_cntr ddrc_perfmon_p1_cntr;
   rand ral_reg_ddrc_perfmon_p0_qual ddrc_perfmon_p0_qual;
   rand ral_reg_ddrc_perfmon_p1_qual ddrc_perfmon_p1_qual;
   rand ral_reg_ddrc_perfmon_p1_qual2 ddrc_perfmon_p1_qual2;
   rand ral_reg_ddrc_perfmon_p0_qual2 ddrc_perfmon_p0_qual2;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL0;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL0;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL1;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL1;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL2;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL2;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL3;
   rand uvm_reg_field ESR_MS_ADDR_REMOVE_BIT_SEL3;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_MC_BIT_SEL;
   rand uvm_reg_field ESR_MS_ADDR_MC_BIT_SEL;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_DEBUG_MC_BIT_SEL;
   rand uvm_reg_field ESR_MS_ADDR_DEBUG_MC_BIT_SEL;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_ADDR_MASK;
   rand uvm_reg_field ESR_MS_ADDR_MASK;
   rand uvm_reg_field ms_mem_ctl_ESR_MS_MC_EN;
   rand uvm_reg_field ESR_MS_MC_EN;
   rand uvm_reg_field ms_atomic_sm_ctl_ESR_MS_ATOMIC_EVICT_START;
   rand uvm_reg_field ESR_MS_ATOMIC_EVICT_START;
   uvm_reg_field ms_atomic_sm_ctl_ESR_MS_ATOMIC_STATE;
   uvm_reg_field ESR_MS_ATOMIC_STATE;
   uvm_reg_field ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B0;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B0;
   uvm_reg_field ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B1;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B1;
   uvm_reg_field ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B2;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B2;
   uvm_reg_field ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B3;
   uvm_reg_field ESR_MEMSHIRE_REVISION_B3;
   uvm_reg_field ms_mem_revision_id_ESR_MEMSHIRE_ID;
   uvm_reg_field ESR_MEMSHIRE_ID;
   rand uvm_reg_field ms_clk_gate_ctl_ESR_MS_DEBUG_CLOCK_DISABLE;
   rand uvm_reg_field ESR_MS_DEBUG_CLOCK_DISABLE;
   rand uvm_reg_field ms_mem_status_ESR_MS_PLL_LOCK_DETECT;
   rand uvm_reg_field ESR_MS_PLL_LOCK_DETECT;
   rand uvm_reg_field ms_mem_status_ESR_MS_PLL_LOCK_LOST;
   rand uvm_reg_field ESR_MS_PLL_LOCK_LOST;
   rand uvm_reg_field ddrc_reset_ctl_ESR_DDRC_SUBSYSTEM_RESET;
   rand uvm_reg_field ESR_DDRC_SUBSYSTEM_RESET;
   rand uvm_reg_field ddrc_reset_ctl_ESR_DDRC_APB_RESET;
   rand uvm_reg_field ESR_DDRC_APB_RESET;
   rand uvm_reg_field ddrc_reset_ctl_ESR_DDRC_PUB_RESET;
   rand uvm_reg_field ESR_DDRC_PUB_RESET;
   rand uvm_reg_field ddrc_reset_ctl_ESR_DDRC_POWER_OK;
   rand uvm_reg_field ESR_DDRC_POWER_OK;
   uvm_reg_field ddrc_reset_ctl_ESR_DDRC_RESET_CTL_RESERVED;
   uvm_reg_field ESR_DDRC_RESET_CTL_RESERVED;
   rand uvm_reg_field ddrc_reset_ctl_ESR_DDRC_PLL_CLK_SEL;
   rand uvm_reg_field ESR_DDRC_PLL_CLK_SEL;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_DDRC;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_DDRC;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_DDRC;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_DDRC;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_DDRC;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_DDRC;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_DDRC;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_DDRC;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_DDRC;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_DDRC;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_DDRC;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_DDRC;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_LO;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_LO;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_LO;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_LO;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_LO;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_LO;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_LO;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_LO;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_LO;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_LO;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_LO;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_LO;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_HI;
   uvm_reg_field ESR_DDRC_U0_CACTIVE_HI;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_HI;
   uvm_reg_field ESR_DDRC_U0_CSYSACK_HI;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_HI;
   rand uvm_reg_field ESR_DDRC_U0_CSYSREQ_HI;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_HI;
   uvm_reg_field ESR_DDRC_U1_CACTIVE_HI;
   uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_HI;
   uvm_reg_field ESR_DDRC_U1_CSYSACK_HI;
   rand uvm_reg_field ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_HI;
   rand uvm_reg_field ESR_DDRC_U1_CSYSREQ_HI;
   rand uvm_reg_field ddrc_main_ctl_ESR_DDRC_WRITE_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ESR_DDRC_WRITE_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ddrc_main_ctl_ESR_DDRC_WRITE_AUTO_PRECHARGE_HI;
   rand uvm_reg_field ESR_DDRC_WRITE_AUTO_PRECHARGE_HI;
   rand uvm_reg_field ddrc_main_ctl_ESR_DDRC_READ_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ESR_DDRC_READ_AUTO_PRECHARGE_LO;
   rand uvm_reg_field ddrc_main_ctl_ESR_DDRC_READ_AUTO_PRECHARGE_HI;
   rand uvm_reg_field ESR_DDRC_READ_AUTO_PRECHARGE_HI;
   rand uvm_reg_field ddrc_scrub1_ESR_DDRC_SCRUB_ADDR_START_MASK;
   rand uvm_reg_field ESR_DDRC_SCRUB_ADDR_START_MASK;
   rand uvm_reg_field ddrc_scrub2_ESR_DDRC_SCRUB_ADDR_RANGE_MASK;
   rand uvm_reg_field ESR_DDRC_SCRUB_ADDR_RANGE_MASK;
   uvm_reg_field ddrc_u0_mrr_data_ESR_DDRC_U0_MRR_DATA;
   uvm_reg_field ESR_DDRC_U0_MRR_DATA;
   uvm_reg_field ddrc_u1_mrr_data_ESR_DDRC_U1_MRR_DATA;
   uvm_reg_field ESR_DDRC_U1_MRR_DATA;
   rand uvm_reg_field ddrc_mrr_status_ESR_DDRC_U0_MRR_DATA_VALID;
   rand uvm_reg_field ESR_DDRC_U0_MRR_DATA_VALID;
   rand uvm_reg_field ddrc_mrr_status_ESR_DDRC_U1_MRR_DATA_VALID;
   rand uvm_reg_field ESR_DDRC_U1_MRR_DATA_VALID;
   uvm_reg_field ddrc_int_status_ESR_CYC_COUNT_OVERFLOW_INTR;
   uvm_reg_field ESR_CYC_COUNT_OVERFLOW_INTR;
   uvm_reg_field ddrc_int_status_ESR_PERF_COUNT1_OVERFLOW_INTR;
   uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_INTR;
   uvm_reg_field ddrc_int_status_ESR_PERF_COUNT0_OVERFLOW_INTR;
   uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC1_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC0_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC1_SBR_DONE_INTR;
   uvm_reg_field ESR_MC1_SBR_DONE_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC0_SBR_DONE_INTR;
   uvm_reg_field ESR_MC0_SBR_DONE_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC1_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC0_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_DDRPHY_INTR;
   uvm_reg_field ESR_DDRPHY_INTR;
   uvm_reg_field ddrc_int_status_ESR_DFI1_ERR_INTR;
   uvm_reg_field ESR_DFI1_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_DFI0_ERR_INTR;
   uvm_reg_field ESR_DFI0_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC1_DFI_ALERT_ERR_INTR;
   uvm_reg_field ESR_MC1_DFI_ALERT_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC0_DFI_ALERT_ERR_INTR;
   uvm_reg_field ESR_MC0_DFI_ALERT_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC1_ECC_UNCORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_INTR;
   uvm_reg_field ddrc_int_status_ESR_MC0_ECC_UNCORRECTED_ERR_INTR;
   uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_INTR;
   rand uvm_reg_field ddrc_int_critical_en_ESR_CYC_COUNT_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_CYC_COUNT_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_PERF_COUNT1_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_PERF_COUNT0_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC1_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ESR_MC1_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC0_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ESR_MC0_SBR_DONE_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_DDRPHY_CRIT_EN;
   rand uvm_reg_field ESR_DDRPHY_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_DFI1_ERR_CRIT_EN;
   rand uvm_reg_field ESR_DFI1_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_DFI0_ERR_CRIT_EN;
   rand uvm_reg_field ESR_DFI0_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC1_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC1_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC0_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_DFI_ALERT_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_critical_en_ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_CYC_COUNT_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_CYC_COUNT_OVERFLOW_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_PERF_COUNT1_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_PERF_COUNT1_OVERFLOW_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_PERF_COUNT0_OVERFLOW_NORM_EN;
   rand uvm_reg_field ESR_PERF_COUNT0_OVERFLOW_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC1_SBR_DONE_NORM_EN;
   rand uvm_reg_field ESR_MC1_SBR_DONE_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC0_SBR_DONE_NORM_EN;
   rand uvm_reg_field ESR_MC0_SBR_DONE_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC1_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC1_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC0_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_ECC_CORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_DDRPHY_NORM_EN;
   rand uvm_reg_field ESR_DDRPHY_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_DFI1_ERR_NORM_EN;
   rand uvm_reg_field ESR_DFI1_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_DFI0_ERR_NORM_EN;
   rand uvm_reg_field ESR_DFI0_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC1_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC1_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC0_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_DFI_ALERT_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ddrc_int_normal_en_ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI1_ERR_INFO;
   rand uvm_reg_field ESR_DFI1_ERR_INFO;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI1_ERR_SET;
   rand uvm_reg_field ESR_DFI1_ERR_SET;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI1_ERR_CLR;
   rand uvm_reg_field ESR_DFI1_ERR_CLR;
   uvm_reg_field ddrc_err_int_log_ESR_DFI1_ERR;
   uvm_reg_field ESR_DFI1_ERR;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI0_ERR_INFO;
   rand uvm_reg_field ESR_DFI0_ERR_INFO;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI0_ERR_SET;
   rand uvm_reg_field ESR_DFI0_ERR_SET;
   rand uvm_reg_field ddrc_err_int_log_ESR_DFI0_ERR_CLR;
   rand uvm_reg_field ESR_DFI0_ERR_CLR;
   uvm_reg_field ddrc_err_int_log_ESR_DFI0_ERR;
   uvm_reg_field ESR_DFI0_ERR;
   rand uvm_reg_field ddrc_debug_sigs_mask0_ESR_DEBUG_SIGS_MASK0;
   rand uvm_reg_field ESR_DEBUG_SIGS_MASK0;
   rand uvm_reg_field ddrc_debug_sigs_mask1_ESR_DEBUG_SIGS_MASK1;
   rand uvm_reg_field ESR_DEBUG_SIGS_MASK1;
   rand uvm_reg_field ddrc_scratch_ESR_SCRATCH;
   rand uvm_reg_field ESR_SCRATCH;
   rand uvm_reg_field ddrc_trace_ctl_ESR_PERF_OP_SIGS_MASK;
   rand uvm_reg_field ESR_PERF_OP_SIGS_MASK;
   rand uvm_reg_field ddrc_trace_ctl_ESR_TRACE_EN;
   rand uvm_reg_field ESR_TRACE_EN;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_SO;
   uvm_reg_field ESR_P1_SO;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_SA;
   uvm_reg_field ESR_P1_SA;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_SO;
   uvm_reg_field ESR_P0_SO;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_SA;
   uvm_reg_field ESR_P0_SA;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_SO;
   uvm_reg_field ESR_CYC_SO;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_SA;
   uvm_reg_field ESR_CYC_SA;
   uvm_reg_field ddrc_perfmon_ctl_status_ESR_AO;
   uvm_reg_field ESR_AO;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_MODE;
   rand uvm_reg_field ESR_P1_MODE;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_E;
   rand uvm_reg_field ESR_P1_E;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_L;
   rand uvm_reg_field ESR_P1_L;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_O;
   rand uvm_reg_field ESR_P1_O;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_R;
   rand uvm_reg_field ESR_P1_R;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P1_S;
   rand uvm_reg_field ESR_P1_S;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_MODE;
   rand uvm_reg_field ESR_P0_MODE;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_E;
   rand uvm_reg_field ESR_P0_E;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_L;
   rand uvm_reg_field ESR_P0_L;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_O;
   rand uvm_reg_field ESR_P0_O;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_R;
   rand uvm_reg_field ESR_P0_R;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_P0_S;
   rand uvm_reg_field ESR_P0_S;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_L;
   rand uvm_reg_field ESR_CYC_L;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_O;
   rand uvm_reg_field ESR_CYC_O;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_R;
   rand uvm_reg_field ESR_CYC_R;
   rand uvm_reg_field ddrc_perfmon_ctl_status_ESR_CYC_S;
   rand uvm_reg_field ESR_CYC_S;
   rand uvm_reg_field ddrc_perfmon_cyc_cntr_ESR_CYC_CNTR;
   rand uvm_reg_field ESR_CYC_CNTR;
   rand uvm_reg_field ddrc_perfmon_p0_cntr_ESR_P0_CNTR;
   rand uvm_reg_field ESR_P0_CNTR;
   rand uvm_reg_field ddrc_perfmon_p1_cntr_ESR_P1_CNTR;
   rand uvm_reg_field ESR_P1_CNTR;
   rand uvm_reg_field ddrc_perfmon_p0_qual_ESR_P0_QUAL;
   rand uvm_reg_field ESR_P0_QUAL;
   rand uvm_reg_field ddrc_perfmon_p1_qual_ESR_P1_QUAL;
   rand uvm_reg_field ESR_P1_QUAL;
   rand uvm_reg_field ddrc_perfmon_p1_qual2_ESR_P1_QUAL2;
   rand uvm_reg_field ESR_P1_QUAL2;
   rand uvm_reg_field ddrc_perfmon_p0_qual2_ESR_P0_QUAL2;
   rand uvm_reg_field ESR_P0_QUAL2;

   function new(string name = "ms_regs", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name, has_coverage);
   endfunction: new

   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      uvm_reg_map axi_map;
      uvm_reg_map apb_map;

      axi_map = create_map("AXI", uvm_reg_addr_t'(base_addr), 8, UVM_LITTLE_ENDIAN, 0);
      apb_map = create_map("APB_ESR", uvm_reg_addr_t'(0), 8, UVM_LITTLE_ENDIAN, 0);

      ms_mem_ctl = ral_reg_ms_mem_ctl::type_id::create("ms_mem_ctl", , get_full_name());
      ms_mem_ctl.configure(this, null, "esr_memshire.reg_ms_mem_ctl");
      ms_mem_ctl.build();
      ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL0 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL0;
      ESR_MS_ADDR_REMOVE_BIT_SEL0 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL0;
      ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL1 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL1;
      ESR_MS_ADDR_REMOVE_BIT_SEL1 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL1;
      ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL2 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL2;
      ESR_MS_ADDR_REMOVE_BIT_SEL2 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL2;
      ms_mem_ctl_ESR_MS_ADDR_REMOVE_BIT_SEL3 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL3;
      ESR_MS_ADDR_REMOVE_BIT_SEL3 = ms_mem_ctl.ESR_MS_ADDR_REMOVE_BIT_SEL3;
      ms_mem_ctl_ESR_MS_ADDR_MC_BIT_SEL = ms_mem_ctl.ESR_MS_ADDR_MC_BIT_SEL;
      ESR_MS_ADDR_MC_BIT_SEL = ms_mem_ctl.ESR_MS_ADDR_MC_BIT_SEL;
      ms_mem_ctl_ESR_MS_ADDR_DEBUG_MC_BIT_SEL = ms_mem_ctl.ESR_MS_ADDR_DEBUG_MC_BIT_SEL;
      ESR_MS_ADDR_DEBUG_MC_BIT_SEL = ms_mem_ctl.ESR_MS_ADDR_DEBUG_MC_BIT_SEL;
      ms_mem_ctl_ESR_MS_ADDR_MASK = ms_mem_ctl.ESR_MS_ADDR_MASK;
      ESR_MS_ADDR_MASK = ms_mem_ctl.ESR_MS_ADDR_MASK;
      ms_mem_ctl_ESR_MS_MC_EN = ms_mem_ctl.ESR_MS_MC_EN;
      ESR_MS_MC_EN = ms_mem_ctl.ESR_MS_MC_EN;
      apb_map.add_reg(ms_mem_ctl, `UVM_REG_ADDR_WIDTH'h04000800, "RW", 0);
      axi_map.add_reg(ms_mem_ctl, `UVM_REG_ADDR_WIDTH'h80000000, "RW", 0);

      ms_atomic_sm_ctl = ral_reg_ms_atomic_sm_ctl::type_id::create("ms_atomic_sm_ctl", , get_full_name());
      ms_atomic_sm_ctl.configure(this, null, "esr_memshire.esr_memshire_status.atomic_sm");
      ms_atomic_sm_ctl.build();
      ms_atomic_sm_ctl_ESR_MS_ATOMIC_EVICT_START = ms_atomic_sm_ctl.ESR_MS_ATOMIC_EVICT_START;
      ESR_MS_ATOMIC_EVICT_START = ms_atomic_sm_ctl.ESR_MS_ATOMIC_EVICT_START;
      ms_atomic_sm_ctl_ESR_MS_ATOMIC_STATE = ms_atomic_sm_ctl.ESR_MS_ATOMIC_STATE;
      ESR_MS_ATOMIC_STATE = ms_atomic_sm_ctl.ESR_MS_ATOMIC_STATE;
      apb_map.add_reg(ms_atomic_sm_ctl, `UVM_REG_ADDR_WIDTH'h04000808, "RW", 0);
      axi_map.add_reg(ms_atomic_sm_ctl, `UVM_REG_ADDR_WIDTH'h80000008, "RW", 0);

      ms_mem_revision_id = ral_reg_ms_mem_revision_id::type_id::create("ms_mem_revision_id", , get_full_name());
      ms_mem_revision_id.configure(this, null, "esr_memshire.esr_memshire_status.mem_revision_id");
      ms_mem_revision_id.build();
      ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B0 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B0;
      ESR_MEMSHIRE_REVISION_B0 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B0;
      ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B1 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B1;
      ESR_MEMSHIRE_REVISION_B1 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B1;
      ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B2 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B2;
      ESR_MEMSHIRE_REVISION_B2 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B2;
      ms_mem_revision_id_ESR_MEMSHIRE_REVISION_B3 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B3;
      ESR_MEMSHIRE_REVISION_B3 = ms_mem_revision_id.ESR_MEMSHIRE_REVISION_B3;
      ms_mem_revision_id_ESR_MEMSHIRE_ID = ms_mem_revision_id.ESR_MEMSHIRE_ID;
      ESR_MEMSHIRE_ID = ms_mem_revision_id.ESR_MEMSHIRE_ID;
      apb_map.add_reg(ms_mem_revision_id, `UVM_REG_ADDR_WIDTH'h04000810, "RO", 0);
      axi_map.add_reg(ms_mem_revision_id, `UVM_REG_ADDR_WIDTH'h80000010, "RO", 0);

      ms_clk_gate_ctl = ral_reg_ms_clk_gate_ctl::type_id::create("ms_clk_gate_ctl", , get_full_name());
      ms_clk_gate_ctl.configure(this, null, "esr_memshire.reg_ms_clk_gate_ctl");
      ms_clk_gate_ctl.build();
      ms_clk_gate_ctl_ESR_MS_DEBUG_CLOCK_DISABLE = ms_clk_gate_ctl.ESR_MS_DEBUG_CLOCK_DISABLE;
      ESR_MS_DEBUG_CLOCK_DISABLE = ms_clk_gate_ctl.ESR_MS_DEBUG_CLOCK_DISABLE;
      apb_map.add_reg(ms_clk_gate_ctl, `UVM_REG_ADDR_WIDTH'h04000818, "RW", 0);
      axi_map.add_reg(ms_clk_gate_ctl, `UVM_REG_ADDR_WIDTH'h80000018, "RW", 0);

      ms_mem_status = ral_reg_ms_mem_status::type_id::create("ms_mem_status", , get_full_name());
      ms_mem_status.configure(this, null, "esr_memshire.reg_ms_mem_status");
      ms_mem_status.build();
      ms_mem_status_ESR_MS_PLL_LOCK_DETECT = ms_mem_status.ESR_MS_PLL_LOCK_DETECT;
      ESR_MS_PLL_LOCK_DETECT = ms_mem_status.ESR_MS_PLL_LOCK_DETECT;
      ms_mem_status_ESR_MS_PLL_LOCK_DETECT = ms_mem_status.ESR_MS_PLL_LOCK_LOST;
      ESR_MS_PLL_LOCK_LOST = ms_mem_status.ESR_MS_PLL_LOCK_LOST;
      apb_map.add_reg(ms_mem_status, `UVM_REG_ADDR_WIDTH'h04000820, "RO", 0);
      axi_map.add_reg(ms_mem_status, `UVM_REG_ADDR_WIDTH'h80000020, "RO", 0);

      ddrc_reset_ctl = ral_reg_ddrc_reset_ctl::type_id::create("ddrc_reset_ctl", , get_full_name());
      ddrc_reset_ctl.configure(this, null, "esr_ddrc.reg_ddrc_reset_ctl");
      ddrc_reset_ctl.build();
      ddrc_reset_ctl_ESR_DDRC_SUBSYSTEM_RESET = ddrc_reset_ctl.ESR_DDRC_SUBSYSTEM_RESET;
      ESR_DDRC_SUBSYSTEM_RESET = ddrc_reset_ctl.ESR_DDRC_SUBSYSTEM_RESET;
      ddrc_reset_ctl_ESR_DDRC_APB_RESET = ddrc_reset_ctl.ESR_DDRC_APB_RESET;
      ESR_DDRC_APB_RESET = ddrc_reset_ctl.ESR_DDRC_APB_RESET;
      ddrc_reset_ctl_ESR_DDRC_PUB_RESET = ddrc_reset_ctl.ESR_DDRC_PUB_RESET;
      ESR_DDRC_PUB_RESET = ddrc_reset_ctl.ESR_DDRC_PUB_RESET;
      ddrc_reset_ctl_ESR_DDRC_POWER_OK = ddrc_reset_ctl.ESR_DDRC_POWER_OK;
      ESR_DDRC_POWER_OK = ddrc_reset_ctl.ESR_DDRC_POWER_OK;
      ddrc_reset_ctl_ESR_DDRC_RESET_CTL_RESERVED = ddrc_reset_ctl.ESR_DDRC_RESET_CTL_RESERVED;
      ESR_DDRC_RESET_CTL_RESERVED = ddrc_reset_ctl.ESR_DDRC_RESET_CTL_RESERVED;
      ddrc_reset_ctl_ESR_DDRC_PLL_CLK_SEL = ddrc_reset_ctl.ESR_DDRC_PLL_CLK_SEL;
      ESR_DDRC_PLL_CLK_SEL = ddrc_reset_ctl.ESR_DDRC_PLL_CLK_SEL;
      apb_map.add_reg(ddrc_reset_ctl, `UVM_REG_ADDR_WIDTH'h04000a00, "RW", 0);
      axi_map.add_reg(ddrc_reset_ctl, `UVM_REG_ADDR_WIDTH'h80000200, "RW", 0);

      ddrc_clock_ctl = ral_reg_ddrc_clock_ctl::type_id::create("ddrc_clock_ctl", , get_full_name());
      ddrc_clock_ctl.configure(this, null, "esr_ddrc.reg_ddrc_clock_ctl");
      ddrc_clock_ctl.build();
      ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_DDRC;
      ESR_DDRC_U0_CACTIVE_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_DDRC;
      ESR_DDRC_U0_CSYSACK_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_DDRC;
      ESR_DDRC_U0_CSYSREQ_DDRC = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_DDRC;
      ESR_DDRC_U1_CACTIVE_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_DDRC;
      ESR_DDRC_U1_CSYSACK_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_DDRC;
      ESR_DDRC_U1_CSYSREQ_DDRC = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_DDRC;
      ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_LO = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_LO;
      ESR_DDRC_U0_CACTIVE_LO = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_LO;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_LO = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_LO;
      ESR_DDRC_U0_CSYSACK_LO = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_LO;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_LO = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_LO;
      ESR_DDRC_U0_CSYSREQ_LO = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_LO;
      ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_LO = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_LO;
      ESR_DDRC_U1_CACTIVE_LO = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_LO;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_LO = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_LO;
      ESR_DDRC_U1_CSYSACK_LO = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_LO;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_LO = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_LO;
      ESR_DDRC_U1_CSYSREQ_LO = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_LO;
      ddrc_clock_ctl_ESR_DDRC_U0_CACTIVE_HI = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_HI;
      ESR_DDRC_U0_CACTIVE_HI = ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_HI;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSACK_HI = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_HI;
      ESR_DDRC_U0_CSYSACK_HI = ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_HI;
      ddrc_clock_ctl_ESR_DDRC_U0_CSYSREQ_HI = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_HI;
      ESR_DDRC_U0_CSYSREQ_HI = ddrc_clock_ctl.ESR_DDRC_U0_CSYSREQ_HI;
      ddrc_clock_ctl_ESR_DDRC_U1_CACTIVE_HI = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_HI;
      ESR_DDRC_U1_CACTIVE_HI = ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_HI;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSACK_HI = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_HI;
      ESR_DDRC_U1_CSYSACK_HI = ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_HI;
      ddrc_clock_ctl_ESR_DDRC_U1_CSYSREQ_HI = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_HI;
      ESR_DDRC_U1_CSYSREQ_HI = ddrc_clock_ctl.ESR_DDRC_U1_CSYSREQ_HI;
      apb_map.add_reg(ddrc_clock_ctl, `UVM_REG_ADDR_WIDTH'h04000a08, "RW", 0);
      axi_map.add_reg(ddrc_clock_ctl, `UVM_REG_ADDR_WIDTH'h80000208, "RW", 0);

      ddrc_main_ctl = ral_reg_ddrc_main_ctl::type_id::create("ddrc_main_ctl", , get_full_name());
      ddrc_main_ctl.configure(this, null, "esr_ddrc.reg_ddrc_main_ctl");
      ddrc_main_ctl.build();
      ddrc_main_ctl_ESR_DDRC_WRITE_AUTO_PRECHARGE_LO = ddrc_main_ctl.ESR_DDRC_WRITE_AUTO_PRECHARGE_LO;
      ESR_DDRC_WRITE_AUTO_PRECHARGE_LO = ddrc_main_ctl.ESR_DDRC_WRITE_AUTO_PRECHARGE_LO;
      ddrc_main_ctl_ESR_DDRC_WRITE_AUTO_PRECHARGE_HI = ddrc_main_ctl.ESR_DDRC_WRITE_AUTO_PRECHARGE_HI;
      ESR_DDRC_WRITE_AUTO_PRECHARGE_HI = ddrc_main_ctl.ESR_DDRC_WRITE_AUTO_PRECHARGE_HI;
      ddrc_main_ctl_ESR_DDRC_READ_AUTO_PRECHARGE_LO = ddrc_main_ctl.ESR_DDRC_READ_AUTO_PRECHARGE_LO;
      ESR_DDRC_READ_AUTO_PRECHARGE_LO = ddrc_main_ctl.ESR_DDRC_READ_AUTO_PRECHARGE_LO;
      ddrc_main_ctl_ESR_DDRC_READ_AUTO_PRECHARGE_HI = ddrc_main_ctl.ESR_DDRC_READ_AUTO_PRECHARGE_HI;
      ESR_DDRC_READ_AUTO_PRECHARGE_HI = ddrc_main_ctl.ESR_DDRC_READ_AUTO_PRECHARGE_HI;
      apb_map.add_reg(ddrc_main_ctl, `UVM_REG_ADDR_WIDTH'h04000a10, "RW", 0);
      axi_map.add_reg(ddrc_main_ctl, `UVM_REG_ADDR_WIDTH'h80000210, "RW", 0);

      ddrc_scrub1 = ral_reg_ddrc_scrub1::type_id::create("ddrc_scrub1", , get_full_name());
      ddrc_scrub1.configure(this, null, "esr_ddrc.reg_ddrc_scrub1");
      ddrc_scrub1.build();
      ddrc_scrub1_ESR_DDRC_SCRUB_ADDR_START_MASK = ddrc_scrub1.ESR_DDRC_SCRUB_ADDR_START_MASK;
      ESR_DDRC_SCRUB_ADDR_START_MASK = ddrc_scrub1.ESR_DDRC_SCRUB_ADDR_START_MASK;
      apb_map.add_reg(ddrc_scrub1, `UVM_REG_ADDR_WIDTH'h04000a18, "RW", 0);
      axi_map.add_reg(ddrc_scrub1, `UVM_REG_ADDR_WIDTH'h80000218, "RW", 0);

      ddrc_scrub2 = ral_reg_ddrc_scrub2::type_id::create("ddrc_scrub2", , get_full_name());
      ddrc_scrub2.configure(this, null, "esr_ddrc.reg_ddrc_scrub2");
      ddrc_scrub2.build();
      ddrc_scrub2_ESR_DDRC_SCRUB_ADDR_RANGE_MASK = ddrc_scrub2.ESR_DDRC_SCRUB_ADDR_RANGE_MASK;
      ESR_DDRC_SCRUB_ADDR_RANGE_MASK = ddrc_scrub2.ESR_DDRC_SCRUB_ADDR_RANGE_MASK;
      apb_map.add_reg(ddrc_scrub2, `UVM_REG_ADDR_WIDTH'h04000a20, "RW", 0);
      axi_map.add_reg(ddrc_scrub2, `UVM_REG_ADDR_WIDTH'h80000220, "RW", 0);

      ddrc_u0_mrr_data = ral_reg_ddrc_u0_mrr_data::type_id::create("ddrc_u0_mrr_data", , get_full_name());
      ddrc_u0_mrr_data.configure(this, null, "esr_ddrc.esr_ddrc_status.ddrc_u0_mrr_data");
      ddrc_u0_mrr_data.build();
      ddrc_u0_mrr_data_ESR_DDRC_U0_MRR_DATA = ddrc_u0_mrr_data.ESR_DDRC_U0_MRR_DATA;
      ESR_DDRC_U0_MRR_DATA = ddrc_u0_mrr_data.ESR_DDRC_U0_MRR_DATA;
      apb_map.add_reg(ddrc_u0_mrr_data, `UVM_REG_ADDR_WIDTH'h04000a28, "RO", 0);
      axi_map.add_reg(ddrc_u0_mrr_data, `UVM_REG_ADDR_WIDTH'h80000228, "RO", 0);

      ddrc_u1_mrr_data = ral_reg_ddrc_u1_mrr_data::type_id::create("ddrc_u1_mrr_data", , get_full_name());
      ddrc_u1_mrr_data.configure(this, null, "esr_ddrc.esr_ddrc_status.ddrc_u1_mrr_data");
      ddrc_u1_mrr_data.build();
      ddrc_u1_mrr_data_ESR_DDRC_U1_MRR_DATA = ddrc_u1_mrr_data.ESR_DDRC_U1_MRR_DATA;
      ESR_DDRC_U1_MRR_DATA = ddrc_u1_mrr_data.ESR_DDRC_U1_MRR_DATA;
      apb_map.add_reg(ddrc_u1_mrr_data, `UVM_REG_ADDR_WIDTH'h04000a30, "RO", 0);
      axi_map.add_reg(ddrc_u1_mrr_data, `UVM_REG_ADDR_WIDTH'h80000230, "RO", 0);

      ddrc_mrr_status = ral_reg_ddrc_mrr_status::type_id::create("ddrc_mrr_status", , get_full_name());
      ddrc_mrr_status.configure(this, null, "esr_ddrc.esr_ddrc_status.ddrc_mrr_status");
      ddrc_mrr_status.build();
      ddrc_mrr_status_ESR_DDRC_U0_MRR_DATA_VALID = ddrc_mrr_status.ESR_DDRC_U0_MRR_DATA_VALID;
      ESR_DDRC_U0_MRR_DATA_VALID = ddrc_mrr_status.ESR_DDRC_U0_MRR_DATA_VALID;
      ddrc_mrr_status_ESR_DDRC_U1_MRR_DATA_VALID = ddrc_mrr_status.ESR_DDRC_U1_MRR_DATA_VALID;
      ESR_DDRC_U1_MRR_DATA_VALID = ddrc_mrr_status.ESR_DDRC_U1_MRR_DATA_VALID;
      apb_map.add_reg(ddrc_mrr_status, `UVM_REG_ADDR_WIDTH'h04000a38, "RW", 0);
      axi_map.add_reg(ddrc_mrr_status, `UVM_REG_ADDR_WIDTH'h80000238, "RW", 0);

      ddrc_int_status = ral_reg_ddrc_int_status::type_id::create("ddrc_int_status", , get_full_name());
      ddrc_int_status.configure(this, null, "esr_ddrc.esr_ddrc_status.ddrc_int_status");
      ddrc_int_status.build();
      ddrc_int_status_ESR_CYC_COUNT_OVERFLOW_INTR = ddrc_int_status.ESR_CYC_COUNT_OVERFLOW_INTR;
      ESR_CYC_COUNT_OVERFLOW_INTR = ddrc_int_status.ESR_CYC_COUNT_OVERFLOW_INTR;
      ddrc_int_status_ESR_PERF_COUNT1_OVERFLOW_INTR = ddrc_int_status.ESR_PERF_COUNT1_OVERFLOW_INTR;
      ESR_PERF_COUNT1_OVERFLOW_INTR = ddrc_int_status.ESR_PERF_COUNT1_OVERFLOW_INTR;
      ddrc_int_status_ESR_PERF_COUNT0_OVERFLOW_INTR = ddrc_int_status.ESR_PERF_COUNT0_OVERFLOW_INTR;
      ESR_PERF_COUNT0_OVERFLOW_INTR = ddrc_int_status.ESR_PERF_COUNT0_OVERFLOW_INTR;
      ddrc_int_status_ESR_MC1_DERATE_TEMP_LIMIT_INTR = ddrc_int_status.ESR_MC1_DERATE_TEMP_LIMIT_INTR;
      ESR_MC1_DERATE_TEMP_LIMIT_INTR = ddrc_int_status.ESR_MC1_DERATE_TEMP_LIMIT_INTR;
      ddrc_int_status_ESR_MC0_DERATE_TEMP_LIMIT_INTR = ddrc_int_status.ESR_MC0_DERATE_TEMP_LIMIT_INTR;
      ESR_MC0_DERATE_TEMP_LIMIT_INTR = ddrc_int_status.ESR_MC0_DERATE_TEMP_LIMIT_INTR;
      ddrc_int_status_ESR_MC1_SBR_DONE_INTR = ddrc_int_status.ESR_MC1_SBR_DONE_INTR;
      ESR_MC1_SBR_DONE_INTR = ddrc_int_status.ESR_MC1_SBR_DONE_INTR;
      ddrc_int_status_ESR_MC0_SBR_DONE_INTR = ddrc_int_status.ESR_MC0_SBR_DONE_INTR;
      ESR_MC0_SBR_DONE_INTR = ddrc_int_status.ESR_MC0_SBR_DONE_INTR;
      ddrc_int_status_ESR_MC1_ECC_CORRECTED_ERR_INTR = ddrc_int_status.ESR_MC1_ECC_CORRECTED_ERR_INTR;
      ESR_MC1_ECC_CORRECTED_ERR_INTR = ddrc_int_status.ESR_MC1_ECC_CORRECTED_ERR_INTR;
      ddrc_int_status_ESR_MC0_ECC_CORRECTED_ERR_INTR = ddrc_int_status.ESR_MC0_ECC_CORRECTED_ERR_INTR;
      ESR_MC0_ECC_CORRECTED_ERR_INTR = ddrc_int_status.ESR_MC0_ECC_CORRECTED_ERR_INTR;
      ddrc_int_status_ESR_DDRPHY_INTR = ddrc_int_status.ESR_DDRPHY_INTR;
      ESR_DDRPHY_INTR = ddrc_int_status.ESR_DDRPHY_INTR;
      ddrc_int_status_ESR_DFI1_ERR_INTR = ddrc_int_status.ESR_DFI1_ERR_INTR;
      ESR_DFI1_ERR_INTR = ddrc_int_status.ESR_DFI1_ERR_INTR;
      ddrc_int_status_ESR_DFI0_ERR_INTR = ddrc_int_status.ESR_DFI0_ERR_INTR;
      ESR_DFI0_ERR_INTR = ddrc_int_status.ESR_DFI0_ERR_INTR;
      ddrc_int_status_ESR_MC1_DFI_ALERT_ERR_INTR = ddrc_int_status.ESR_MC1_DFI_ALERT_ERR_INTR;
      ESR_MC1_DFI_ALERT_ERR_INTR = ddrc_int_status.ESR_MC1_DFI_ALERT_ERR_INTR;
      ddrc_int_status_ESR_MC0_DFI_ALERT_ERR_INTR = ddrc_int_status.ESR_MC0_DFI_ALERT_ERR_INTR;
      ESR_MC0_DFI_ALERT_ERR_INTR = ddrc_int_status.ESR_MC0_DFI_ALERT_ERR_INTR;
      ddrc_int_status_ESR_MC1_ECC_UNCORRECTED_ERR_INTR = ddrc_int_status.ESR_MC1_ECC_UNCORRECTED_ERR_INTR;
      ESR_MC1_ECC_UNCORRECTED_ERR_INTR = ddrc_int_status.ESR_MC1_ECC_UNCORRECTED_ERR_INTR;
      ddrc_int_status_ESR_MC0_ECC_UNCORRECTED_ERR_INTR = ddrc_int_status.ESR_MC0_ECC_UNCORRECTED_ERR_INTR;
      ESR_MC0_ECC_UNCORRECTED_ERR_INTR = ddrc_int_status.ESR_MC0_ECC_UNCORRECTED_ERR_INTR;
      apb_map.add_reg(ddrc_int_status, `UVM_REG_ADDR_WIDTH'h04000a40, "RO", 0);
      axi_map.add_reg(ddrc_int_status, `UVM_REG_ADDR_WIDTH'h80000240, "RO", 0);

      ddrc_int_critical_en = ral_reg_ddrc_int_critical_en::type_id::create("ddrc_int_critical_en", , get_full_name());
      ddrc_int_critical_en.configure(this, null, "esr_ddrc.reg_ddrc_int_critical_en");
      ddrc_int_critical_en.build();
      ddrc_int_critical_en_ESR_CYC_COUNT_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_CYC_COUNT_OVERFLOW_CRIT_EN;
      ESR_CYC_COUNT_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_CYC_COUNT_OVERFLOW_CRIT_EN;
      ddrc_int_critical_en_ESR_PERF_COUNT1_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_PERF_COUNT1_OVERFLOW_CRIT_EN;
      ESR_PERF_COUNT1_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_PERF_COUNT1_OVERFLOW_CRIT_EN;
      ddrc_int_critical_en_ESR_PERF_COUNT0_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_PERF_COUNT0_OVERFLOW_CRIT_EN;
      ESR_PERF_COUNT0_OVERFLOW_CRIT_EN = ddrc_int_critical_en.ESR_PERF_COUNT0_OVERFLOW_CRIT_EN;
      ddrc_int_critical_en_ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN = ddrc_int_critical_en.ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN;
      ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN = ddrc_int_critical_en.ESR_MC1_DERATE_TEMP_LIMIT_CRIT_EN;
      ddrc_int_critical_en_ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN = ddrc_int_critical_en.ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN;
      ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN = ddrc_int_critical_en.ESR_MC0_DERATE_TEMP_LIMIT_CRIT_EN;
      ddrc_int_critical_en_ESR_MC1_SBR_DONE_CRIT_EN = ddrc_int_critical_en.ESR_MC1_SBR_DONE_CRIT_EN;
      ESR_MC1_SBR_DONE_CRIT_EN = ddrc_int_critical_en.ESR_MC1_SBR_DONE_CRIT_EN;
      ddrc_int_critical_en_ESR_MC0_SBR_DONE_CRIT_EN = ddrc_int_critical_en.ESR_MC0_SBR_DONE_CRIT_EN;
      ESR_MC0_SBR_DONE_CRIT_EN = ddrc_int_critical_en.ESR_MC0_SBR_DONE_CRIT_EN;
      ddrc_int_critical_en_ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN;
      ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_ECC_CORRECTED_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN;
      ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_ECC_CORRECTED_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_DDRPHY_CRIT_EN = ddrc_int_critical_en.ESR_DDRPHY_CRIT_EN;
      ESR_DDRPHY_CRIT_EN = ddrc_int_critical_en.ESR_DDRPHY_CRIT_EN;
      ddrc_int_critical_en_ESR_DFI1_ERR_CRIT_EN = ddrc_int_critical_en.ESR_DFI1_ERR_CRIT_EN;
      ESR_DFI1_ERR_CRIT_EN = ddrc_int_critical_en.ESR_DFI1_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_DFI0_ERR_CRIT_EN = ddrc_int_critical_en.ESR_DFI0_ERR_CRIT_EN;
      ESR_DFI0_ERR_CRIT_EN = ddrc_int_critical_en.ESR_DFI0_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_MC1_DFI_ALERT_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_DFI_ALERT_ERR_CRIT_EN;
      ESR_MC1_DFI_ALERT_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_DFI_ALERT_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_MC0_DFI_ALERT_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_DFI_ALERT_ERR_CRIT_EN;
      ESR_MC0_DFI_ALERT_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_DFI_ALERT_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN;
      ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC1_ECC_UNCORRECTED_ERR_CRIT_EN;
      ddrc_int_critical_en_ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN;
      ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN = ddrc_int_critical_en.ESR_MC0_ECC_UNCORRECTED_ERR_CRIT_EN;
      apb_map.add_reg(ddrc_int_critical_en, `UVM_REG_ADDR_WIDTH'h04000a48, "RW", 0);
      axi_map.add_reg(ddrc_int_critical_en, `UVM_REG_ADDR_WIDTH'h80000248, "RW", 0);

      ddrc_int_normal_en = ral_reg_ddrc_int_normal_en::type_id::create("ddrc_int_normal_en", , get_full_name());
      ddrc_int_normal_en.configure(this, null, "esr_ddrc.reg_ddrc_int_normal_en");
      ddrc_int_normal_en.build();
      ddrc_int_normal_en_ESR_CYC_COUNT_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_CYC_COUNT_OVERFLOW_NORM_EN;
      ESR_CYC_COUNT_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_CYC_COUNT_OVERFLOW_NORM_EN;
      ddrc_int_normal_en_ESR_PERF_COUNT1_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_PERF_COUNT1_OVERFLOW_NORM_EN;
      ESR_PERF_COUNT1_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_PERF_COUNT1_OVERFLOW_NORM_EN;
      ddrc_int_normal_en_ESR_PERF_COUNT0_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_PERF_COUNT0_OVERFLOW_NORM_EN;
      ESR_PERF_COUNT0_OVERFLOW_NORM_EN = ddrc_int_normal_en.ESR_PERF_COUNT0_OVERFLOW_NORM_EN;
      ddrc_int_normal_en_ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN = ddrc_int_normal_en.ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN;
      ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN = ddrc_int_normal_en.ESR_MC1_DERATE_TEMP_LIMIT_NORM_EN;
      ddrc_int_normal_en_ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN = ddrc_int_normal_en.ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN;
      ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN = ddrc_int_normal_en.ESR_MC0_DERATE_TEMP_LIMIT_NORM_EN;
      ddrc_int_normal_en_ESR_MC1_SBR_DONE_NORM_EN = ddrc_int_normal_en.ESR_MC1_SBR_DONE_NORM_EN;
      ESR_MC1_SBR_DONE_NORM_EN = ddrc_int_normal_en.ESR_MC1_SBR_DONE_NORM_EN;
      ddrc_int_normal_en_ESR_MC0_SBR_DONE_NORM_EN = ddrc_int_normal_en.ESR_MC0_SBR_DONE_NORM_EN;
      ESR_MC0_SBR_DONE_NORM_EN = ddrc_int_normal_en.ESR_MC0_SBR_DONE_NORM_EN;
      ddrc_int_normal_en_ESR_MC1_ECC_CORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_ECC_CORRECTED_ERR_NORM_EN;
      ESR_MC1_ECC_CORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_ECC_CORRECTED_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_MC0_ECC_CORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_ECC_CORRECTED_ERR_NORM_EN;
      ESR_MC0_ECC_CORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_ECC_CORRECTED_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_DDRPHY_NORM_EN = ddrc_int_normal_en.ESR_DDRPHY_NORM_EN;
      ESR_DDRPHY_NORM_EN = ddrc_int_normal_en.ESR_DDRPHY_NORM_EN;
      ddrc_int_normal_en_ESR_DFI1_ERR_NORM_EN = ddrc_int_normal_en.ESR_DFI1_ERR_NORM_EN;
      ESR_DFI1_ERR_NORM_EN = ddrc_int_normal_en.ESR_DFI1_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_DFI0_ERR_NORM_EN = ddrc_int_normal_en.ESR_DFI0_ERR_NORM_EN;
      ESR_DFI0_ERR_NORM_EN = ddrc_int_normal_en.ESR_DFI0_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_MC1_DFI_ALERT_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_DFI_ALERT_ERR_NORM_EN;
      ESR_MC1_DFI_ALERT_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_DFI_ALERT_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_MC0_DFI_ALERT_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_DFI_ALERT_ERR_NORM_EN;
      ESR_MC0_DFI_ALERT_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_DFI_ALERT_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN;
      ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC1_ECC_UNCORRECTED_ERR_NORM_EN;
      ddrc_int_normal_en_ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN;
      ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN = ddrc_int_normal_en.ESR_MC0_ECC_UNCORRECTED_ERR_NORM_EN;
      apb_map.add_reg(ddrc_int_normal_en, `UVM_REG_ADDR_WIDTH'h04000a50, "RW", 0);
      axi_map.add_reg(ddrc_int_normal_en, `UVM_REG_ADDR_WIDTH'h80000250, "RW", 0);

      ddrc_err_int_log = ral_reg_ddrc_err_int_log::type_id::create("ddrc_err_int_log", , get_full_name());
      ddrc_err_int_log.configure(this, null, "esr_ddrc.esr_ddrc_status.ddrc_err_int_log");
      ddrc_err_int_log.build();
      ddrc_err_int_log_ESR_DFI1_ERR_INFO = ddrc_err_int_log.ESR_DFI1_ERR_INFO;
      ESR_DFI1_ERR_INFO = ddrc_err_int_log.ESR_DFI1_ERR_INFO;
      ddrc_err_int_log_ESR_DFI1_ERR_SET = ddrc_err_int_log.ESR_DFI1_ERR_SET;
      ESR_DFI1_ERR_SET = ddrc_err_int_log.ESR_DFI1_ERR_SET;
      ddrc_err_int_log_ESR_DFI1_ERR_CLR = ddrc_err_int_log.ESR_DFI1_ERR_CLR;
      ESR_DFI1_ERR_CLR = ddrc_err_int_log.ESR_DFI1_ERR_CLR;
      ddrc_err_int_log_ESR_DFI1_ERR = ddrc_err_int_log.ESR_DFI1_ERR;
      ESR_DFI1_ERR = ddrc_err_int_log.ESR_DFI1_ERR;
      ddrc_err_int_log_ESR_DFI0_ERR_INFO = ddrc_err_int_log.ESR_DFI0_ERR_INFO;
      ESR_DFI0_ERR_INFO = ddrc_err_int_log.ESR_DFI0_ERR_INFO;
      ddrc_err_int_log_ESR_DFI0_ERR_SET = ddrc_err_int_log.ESR_DFI0_ERR_SET;
      ESR_DFI0_ERR_SET = ddrc_err_int_log.ESR_DFI0_ERR_SET;
      ddrc_err_int_log_ESR_DFI0_ERR_CLR = ddrc_err_int_log.ESR_DFI0_ERR_CLR;
      ESR_DFI0_ERR_CLR = ddrc_err_int_log.ESR_DFI0_ERR_CLR;
      ddrc_err_int_log_ESR_DFI0_ERR = ddrc_err_int_log.ESR_DFI0_ERR;
      ESR_DFI0_ERR = ddrc_err_int_log.ESR_DFI0_ERR;
      apb_map.add_reg(ddrc_err_int_log, `UVM_REG_ADDR_WIDTH'h04000a58, "RW", 0);
      axi_map.add_reg(ddrc_err_int_log, `UVM_REG_ADDR_WIDTH'h80000258, "RW", 0);

      ddrc_debug_sigs_mask0 = ral_reg_ddrc_debug_sigs_mask0::type_id::create("ddrc_debug_sigs_mask0", , get_full_name());
      ddrc_debug_sigs_mask0.configure(this, null, "esr_ddrc.reg_ddrc_debug_sigs_mask0");
      ddrc_debug_sigs_mask0.build();
      ddrc_debug_sigs_mask0_ESR_DEBUG_SIGS_MASK0 = ddrc_debug_sigs_mask0.ESR_DEBUG_SIGS_MASK0;
      ESR_DEBUG_SIGS_MASK0 = ddrc_debug_sigs_mask0.ESR_DEBUG_SIGS_MASK0;
      apb_map.add_reg(ddrc_debug_sigs_mask0, `UVM_REG_ADDR_WIDTH'h04000a60, "RW", 0);
      axi_map.add_reg(ddrc_debug_sigs_mask0, `UVM_REG_ADDR_WIDTH'h80000260, "RW", 0);

      ddrc_debug_sigs_mask1 = ral_reg_ddrc_debug_sigs_mask1::type_id::create("ddrc_debug_sigs_mask1", , get_full_name());
      ddrc_debug_sigs_mask1.configure(this, null, "esr_ddrc.reg_ddrc_debug_sigs_mask1");
      ddrc_debug_sigs_mask1.build();
      ddrc_debug_sigs_mask1_ESR_DEBUG_SIGS_MASK1 = ddrc_debug_sigs_mask1.ESR_DEBUG_SIGS_MASK1;
      ESR_DEBUG_SIGS_MASK1 = ddrc_debug_sigs_mask1.ESR_DEBUG_SIGS_MASK1;
      apb_map.add_reg(ddrc_debug_sigs_mask1, `UVM_REG_ADDR_WIDTH'h04000a68, "RW", 0);
      axi_map.add_reg(ddrc_debug_sigs_mask1, `UVM_REG_ADDR_WIDTH'h80000268, "RW", 0);

      ddrc_scratch = ral_reg_ddrc_scratch::type_id::create("ddrc_scratch", , get_full_name());
      ddrc_scratch.configure(this, null, "esr_ddrc.reg_ddrc_scratch");
      ddrc_scratch.build();
      ddrc_scratch_ESR_SCRATCH = ddrc_scratch.ESR_SCRATCH;
      ESR_SCRATCH = ddrc_scratch.ESR_SCRATCH;
      apb_map.add_reg(ddrc_scratch, `UVM_REG_ADDR_WIDTH'h04000e70, "RW", 0);
      axi_map.add_reg(ddrc_scratch, `UVM_REG_ADDR_WIDTH'hc0000270, "RW", 0);

      ddrc_trace_ctl = ral_reg_ddrc_trace_ctl::type_id::create("ddrc_trace_ctl", , get_full_name());
      ddrc_trace_ctl.configure(this, null, "esr_ddrc.reg_ddrc_trace_ctl");
      ddrc_trace_ctl.build();
      ddrc_trace_ctl_ESR_PERF_OP_SIGS_MASK = ddrc_trace_ctl.ESR_PERF_OP_SIGS_MASK;
      ESR_PERF_OP_SIGS_MASK = ddrc_trace_ctl.ESR_PERF_OP_SIGS_MASK;
      ddrc_trace_ctl_ESR_TRACE_EN = ddrc_trace_ctl.ESR_TRACE_EN;
      ESR_TRACE_EN = ddrc_trace_ctl.ESR_TRACE_EN;
      apb_map.add_reg(ddrc_trace_ctl, `UVM_REG_ADDR_WIDTH'h04000a78, "RW", 0);
      axi_map.add_reg(ddrc_trace_ctl, `UVM_REG_ADDR_WIDTH'h80000278, "RW", 0);

      ddrc_perfmon_ctl_status = ral_reg_ddrc_perfmon_ctl_status::type_id::create("ddrc_perfmon_ctl_status", , get_full_name());
      ddrc_perfmon_ctl_status.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_ctl_status");
      ddrc_perfmon_ctl_status.build();
      ddrc_perfmon_ctl_status_ESR_P1_SO = ddrc_perfmon_ctl_status.ESR_P1_SO;
      ESR_P1_SO = ddrc_perfmon_ctl_status.ESR_P1_SO;
      ddrc_perfmon_ctl_status_ESR_P1_SA = ddrc_perfmon_ctl_status.ESR_P1_SA;
      ESR_P1_SA = ddrc_perfmon_ctl_status.ESR_P1_SA;
      ddrc_perfmon_ctl_status_ESR_P0_SO = ddrc_perfmon_ctl_status.ESR_P0_SO;
      ESR_P0_SO = ddrc_perfmon_ctl_status.ESR_P0_SO;
      ddrc_perfmon_ctl_status_ESR_P0_SA = ddrc_perfmon_ctl_status.ESR_P0_SA;
      ESR_P0_SA = ddrc_perfmon_ctl_status.ESR_P0_SA;
      ddrc_perfmon_ctl_status_ESR_CYC_SO = ddrc_perfmon_ctl_status.ESR_CYC_SO;
      ESR_CYC_SO = ddrc_perfmon_ctl_status.ESR_CYC_SO;
      ddrc_perfmon_ctl_status_ESR_CYC_SA = ddrc_perfmon_ctl_status.ESR_CYC_SA;
      ESR_CYC_SA = ddrc_perfmon_ctl_status.ESR_CYC_SA;
      ddrc_perfmon_ctl_status_ESR_AO = ddrc_perfmon_ctl_status.ESR_AO;
      ESR_AO = ddrc_perfmon_ctl_status.ESR_AO;
      ddrc_perfmon_ctl_status_ESR_P1_MODE = ddrc_perfmon_ctl_status.ESR_P1_MODE;
      ESR_P1_MODE = ddrc_perfmon_ctl_status.ESR_P1_MODE;
      ddrc_perfmon_ctl_status_ESR_P1_E = ddrc_perfmon_ctl_status.ESR_P1_E;
      ESR_P1_E = ddrc_perfmon_ctl_status.ESR_P1_E;
      ddrc_perfmon_ctl_status_ESR_P1_L = ddrc_perfmon_ctl_status.ESR_P1_L;
      ESR_P1_L = ddrc_perfmon_ctl_status.ESR_P1_L;
      ddrc_perfmon_ctl_status_ESR_P1_O = ddrc_perfmon_ctl_status.ESR_P1_O;
      ESR_P1_O = ddrc_perfmon_ctl_status.ESR_P1_O;
      ddrc_perfmon_ctl_status_ESR_P1_R = ddrc_perfmon_ctl_status.ESR_P1_R;
      ESR_P1_R = ddrc_perfmon_ctl_status.ESR_P1_R;
      ddrc_perfmon_ctl_status_ESR_P1_S = ddrc_perfmon_ctl_status.ESR_P1_S;
      ESR_P1_S = ddrc_perfmon_ctl_status.ESR_P1_S;
      ddrc_perfmon_ctl_status_ESR_P0_MODE = ddrc_perfmon_ctl_status.ESR_P0_MODE;
      ESR_P0_MODE = ddrc_perfmon_ctl_status.ESR_P0_MODE;
      ddrc_perfmon_ctl_status_ESR_P0_E = ddrc_perfmon_ctl_status.ESR_P0_E;
      ESR_P0_E = ddrc_perfmon_ctl_status.ESR_P0_E;
      ddrc_perfmon_ctl_status_ESR_P0_L = ddrc_perfmon_ctl_status.ESR_P0_L;
      ESR_P0_L = ddrc_perfmon_ctl_status.ESR_P0_L;
      ddrc_perfmon_ctl_status_ESR_P0_O = ddrc_perfmon_ctl_status.ESR_P0_O;
      ESR_P0_O = ddrc_perfmon_ctl_status.ESR_P0_O;
      ddrc_perfmon_ctl_status_ESR_P0_R = ddrc_perfmon_ctl_status.ESR_P0_R;
      ESR_P0_R = ddrc_perfmon_ctl_status.ESR_P0_R;
      ddrc_perfmon_ctl_status_ESR_P0_S = ddrc_perfmon_ctl_status.ESR_P0_S;
      ESR_P0_S = ddrc_perfmon_ctl_status.ESR_P0_S;
      ddrc_perfmon_ctl_status_ESR_CYC_L = ddrc_perfmon_ctl_status.ESR_CYC_L;
      ESR_CYC_L = ddrc_perfmon_ctl_status.ESR_CYC_L;
      ddrc_perfmon_ctl_status_ESR_CYC_O = ddrc_perfmon_ctl_status.ESR_CYC_O;
      ESR_CYC_O = ddrc_perfmon_ctl_status.ESR_CYC_O;
      ddrc_perfmon_ctl_status_ESR_CYC_R = ddrc_perfmon_ctl_status.ESR_CYC_R;
      ESR_CYC_R = ddrc_perfmon_ctl_status.ESR_CYC_R;
      ddrc_perfmon_ctl_status_ESR_CYC_S = ddrc_perfmon_ctl_status.ESR_CYC_S;
      ESR_CYC_S = ddrc_perfmon_ctl_status.ESR_CYC_S;
      apb_map.add_reg(ddrc_perfmon_ctl_status, `UVM_REG_ADDR_WIDTH'h04000e80, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_ctl_status, `UVM_REG_ADDR_WIDTH'hc0000280, "RW", 0);

      ddrc_perfmon_cyc_cntr = ral_reg_ddrc_perfmon_cyc_cntr::type_id::create("ddrc_perfmon_cyc_cntr", , get_full_name());
      ddrc_perfmon_cyc_cntr.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_cyc_cntr");
      ddrc_perfmon_cyc_cntr.build();
      ddrc_perfmon_cyc_cntr_ESR_CYC_CNTR = ddrc_perfmon_cyc_cntr.ESR_CYC_CNTR;
      ESR_CYC_CNTR = ddrc_perfmon_cyc_cntr.ESR_CYC_CNTR;
      apb_map.add_reg(ddrc_perfmon_cyc_cntr, `UVM_REG_ADDR_WIDTH'h04000e88, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_cyc_cntr, `UVM_REG_ADDR_WIDTH'hc0000288, "RW", 0);

      ddrc_perfmon_p0_cntr = ral_reg_ddrc_perfmon_p0_cntr::type_id::create("ddrc_perfmon_p0_cntr", , get_full_name());
      ddrc_perfmon_p0_cntr.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p0_cntr");
      ddrc_perfmon_p0_cntr.build();
      ddrc_perfmon_p0_cntr_ESR_P0_CNTR = ddrc_perfmon_p0_cntr.ESR_P0_CNTR;
      ESR_P0_CNTR = ddrc_perfmon_p0_cntr.ESR_P0_CNTR;
      apb_map.add_reg(ddrc_perfmon_p0_cntr, `UVM_REG_ADDR_WIDTH'h04000e90, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p0_cntr, `UVM_REG_ADDR_WIDTH'hc0000290, "RW", 0);

      ddrc_perfmon_p1_cntr = ral_reg_ddrc_perfmon_p1_cntr::type_id::create("ddrc_perfmon_p1_cntr", , get_full_name());
      ddrc_perfmon_p1_cntr.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p1_cntr");
      ddrc_perfmon_p1_cntr.build();
      ddrc_perfmon_p1_cntr_ESR_P1_CNTR = ddrc_perfmon_p1_cntr.ESR_P1_CNTR;
      ESR_P1_CNTR = ddrc_perfmon_p1_cntr.ESR_P1_CNTR;
      apb_map.add_reg(ddrc_perfmon_p1_cntr, `UVM_REG_ADDR_WIDTH'h04000e98, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p1_cntr, `UVM_REG_ADDR_WIDTH'hc0000298, "RW", 0);

      ddrc_perfmon_p0_qual = ral_reg_ddrc_perfmon_p0_qual::type_id::create("ddrc_perfmon_p0_qual", , get_full_name());
      ddrc_perfmon_p0_qual.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p0_qual");
      ddrc_perfmon_p0_qual.build();
      ddrc_perfmon_p0_qual_ESR_P0_QUAL = ddrc_perfmon_p0_qual.ESR_P0_QUAL;
      ESR_P0_QUAL = ddrc_perfmon_p0_qual.ESR_P0_QUAL;
      apb_map.add_reg(ddrc_perfmon_p0_qual, `UVM_REG_ADDR_WIDTH'h04000ea0, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p0_qual, `UVM_REG_ADDR_WIDTH'hc00002a0, "RW", 0);

      ddrc_perfmon_p1_qual = ral_reg_ddrc_perfmon_p1_qual::type_id::create("ddrc_perfmon_p1_qual", , get_full_name());
      ddrc_perfmon_p1_qual.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p1_qual");
      ddrc_perfmon_p1_qual.build();
      ddrc_perfmon_p1_qual_ESR_P1_QUAL = ddrc_perfmon_p1_qual.ESR_P1_QUAL;
      ESR_P1_QUAL = ddrc_perfmon_p1_qual.ESR_P1_QUAL;
      apb_map.add_reg(ddrc_perfmon_p1_qual, `UVM_REG_ADDR_WIDTH'h04000ea8, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p1_qual, `UVM_REG_ADDR_WIDTH'hc00002a8, "RW", 0);

      ddrc_perfmon_p0_qual2 = ral_reg_ddrc_perfmon_p0_qual2::type_id::create("ddrc_perfmon_p0_qual2", , get_full_name());
      ddrc_perfmon_p0_qual2.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p0_qual2");
      ddrc_perfmon_p0_qual2.build();
      ddrc_perfmon_p0_qual2_ESR_P0_QUAL2 = ddrc_perfmon_p0_qual2.ESR_P0_QUAL2;
      ESR_P0_QUAL2 = ddrc_perfmon_p0_qual2.ESR_P0_QUAL2;
      apb_map.add_reg(ddrc_perfmon_p0_qual2, `UVM_REG_ADDR_WIDTH'h04000eb0, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p0_qual2, `UVM_REG_ADDR_WIDTH'hc00002b0, "RW", 0);

      ddrc_perfmon_p1_qual2 = ral_reg_ddrc_perfmon_p1_qual2::type_id::create("ddrc_perfmon_p1_qual2", , get_full_name());
      ddrc_perfmon_p1_qual2.configure(this, null, "esr_ddrc.esr_ddrc_status.perfmon_p1_qual2");
      ddrc_perfmon_p1_qual2.build();
      ddrc_perfmon_p1_qual2_ESR_P1_QUAL2 = ddrc_perfmon_p1_qual2.ESR_P1_QUAL2;
      ESR_P1_QUAL2 = ddrc_perfmon_p1_qual2.ESR_P1_QUAL2;
      apb_map.add_reg(ddrc_perfmon_p1_qual2, `UVM_REG_ADDR_WIDTH'h04000eb8, "RW", 0);
      axi_map.add_reg(ddrc_perfmon_p1_qual2, `UVM_REG_ADDR_WIDTH'hc00002b8, "RW", 0);

   endfunction : build

   `uvm_object_utils(ral_block_ms_regs)

endclass : ral_block_ms_regs
