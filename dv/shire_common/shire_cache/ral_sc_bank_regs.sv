class ral_reg_sc_l3_shire_swizzle_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B0;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B1;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B2;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B3;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B4;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B5;
   rand uvm_reg_field ESR_SC_BANK_SEL_B0;
   rand uvm_reg_field ESR_SC_BANK_SEL_B1;
   rand uvm_reg_field ESR_SC_BANK_SEL_B2;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B0;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B1;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B2;
   rand uvm_reg_field ESR_SC_ALL_SHIRE_ALIASING;
   rand uvm_reg_field ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB;

   function new(string name = "sc_l3_shire_swizzle_ctl");
      super.new(name, 56, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_SHIRE_SEL_B0 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B0", , get_full_name());
      ESR_SC_SHIRE_SEL_B0.configure(this, 4, 0, "RW", 0, 4'h0, 1, 0, 0);
      ESR_SC_SHIRE_SEL_B1 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B1", , get_full_name());
      ESR_SC_SHIRE_SEL_B1.configure(this, 4, 4, "RW", 0, 4'h1, 1, 0, 0);
      ESR_SC_SHIRE_SEL_B2 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B2", , get_full_name());
      ESR_SC_SHIRE_SEL_B2.configure(this, 4, 8, "RW", 0, 4'h2, 1, 0, 0);
      ESR_SC_SHIRE_SEL_B3 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B3", , get_full_name());
      ESR_SC_SHIRE_SEL_B3.configure(this, 4, 12, "RW", 0, 4'h3, 1, 0, 0);
      ESR_SC_SHIRE_SEL_B4 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B4", , get_full_name());
      ESR_SC_SHIRE_SEL_B4.configure(this, 4, 16, "RW", 0, 4'h4, 1, 0, 0);
      ESR_SC_SHIRE_SEL_B5 = uvm_reg_field::type_id::create("ESR_SC_SHIRE_SEL_B5", , get_full_name());
      ESR_SC_SHIRE_SEL_B5.configure(this, 4, 20, "RW", 0, 4'h5, 1, 0, 0);
      ESR_SC_BANK_SEL_B0 = uvm_reg_field::type_id::create("ESR_SC_BANK_SEL_B0", , get_full_name());
      ESR_SC_BANK_SEL_B0.configure(this, 4, 24, "RW", 0, 4'h5, 1, 0, 0);
      ESR_SC_BANK_SEL_B1 = uvm_reg_field::type_id::create("ESR_SC_BANK_SEL_B1", , get_full_name());
      ESR_SC_BANK_SEL_B1.configure(this, 4, 28, "RW", 0, 4'h6, 1, 0, 0);
      ESR_SC_BANK_SEL_B2 = uvm_reg_field::type_id::create("ESR_SC_BANK_SEL_B2", , get_full_name());
      ESR_SC_BANK_SEL_B2.configure(this, 4, 32, "RW", 0, 4'h7, 1, 0, 0);
      ESR_SC_SUB_BANK_SEL_B0 = uvm_reg_field::type_id::create("ESR_SC_SUB_BANK_SEL_B0", , get_full_name());
      ESR_SC_SUB_BANK_SEL_B0.configure(this, 4, 36, "RW", 0, 4'h7, 1, 0, 0);
      ESR_SC_SUB_BANK_SEL_B1 = uvm_reg_field::type_id::create("ESR_SC_SUB_BANK_SEL_B1", , get_full_name());
      ESR_SC_SUB_BANK_SEL_B1.configure(this, 4, 40, "RW", 0, 4'h8, 1, 0, 0);
      ESR_SC_SUB_BANK_SEL_B2 = uvm_reg_field::type_id::create("ESR_SC_SUB_BANK_SEL_B2", , get_full_name());
      ESR_SC_SUB_BANK_SEL_B2.configure(this, 4, 44, "RW", 0, 4'h9, 1, 0, 0);
      ESR_SC_ALL_SHIRE_ALIASING = uvm_reg_field::type_id::create("ESR_SC_ALL_SHIRE_ALIASING", , get_full_name());
      ESR_SC_ALL_SHIRE_ALIASING.configure(this, 1, 48, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB = uvm_reg_field::type_id::create("ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB", , get_full_name());
      ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB.configure(this, 1, 49, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_l3_shire_swizzle_ctl)

endclass : ral_reg_sc_l3_shire_swizzle_ctl


class ral_reg_sc_reqq_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_L3_YIELD_PRIORITY;
   rand uvm_reg_field ESR_SC_ECC_SCRUB_ENABLE;
   rand uvm_reg_field ESR_SC_REQQ_NO_LINK_LIST;
   rand uvm_reg_field ESR_SC_NUM_L3_REQQ_ENTRIES;
   rand uvm_reg_field ESR_SC_L3_BYPASS;
   rand uvm_reg_field ESR_SC_L2_BYPASS;
   rand uvm_reg_field ESR_SC_REMOTE_SCP_ENABLE;
   rand uvm_reg_field ESR_SC_REMOTE_L3_ENABLE;
   rand uvm_reg_field ESR_SC_CBUF_ENABLE;
   rand uvm_reg_field ESR_SC_AXI_QOS;
   rand uvm_reg_field ESR_SC_CLK_GATE_DISABLE;

   function new(string name = "sc_reqq_ctl");
      super.new(name, 32, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_L3_YIELD_PRIORITY = uvm_reg_field::type_id::create("ESR_SC_L3_YIELD_PRIORITY", , get_full_name());
      ESR_SC_L3_YIELD_PRIORITY.configure(this, 5, 0, "RW", 0, 5'h0, 1, 0, 0);
      ESR_SC_ECC_SCRUB_ENABLE = uvm_reg_field::type_id::create("ESR_SC_ECC_SCRUB_ENABLE", , get_full_name());
      ESR_SC_ECC_SCRUB_ENABLE.configure(this, 1, 5, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_REQQ_NO_LINK_LIST = uvm_reg_field::type_id::create("ESR_SC_REQQ_NO_LINK_LIST", , get_full_name());
      ESR_SC_REQQ_NO_LINK_LIST.configure(this, 1, 6, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_NUM_L3_REQQ_ENTRIES = uvm_reg_field::type_id::create("ESR_SC_NUM_L3_REQQ_ENTRIES", , get_full_name());
      ESR_SC_NUM_L3_REQQ_ENTRIES.configure(this, 6, 7, "RW", 0, 6'h15, 1, 0, 0);
      ESR_SC_L3_BYPASS = uvm_reg_field::type_id::create("ESR_SC_L3_BYPASS", , get_full_name());
      ESR_SC_L3_BYPASS.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_L2_BYPASS = uvm_reg_field::type_id::create("ESR_SC_L2_BYPASS", , get_full_name());
      ESR_SC_L2_BYPASS.configure(this, 1, 14, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_REMOTE_SCP_ENABLE = uvm_reg_field::type_id::create("ESR_SC_REMOTE_SCP_ENABLE", , get_full_name());
      ESR_SC_REMOTE_SCP_ENABLE.configure(this, 1, 15, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_REMOTE_L3_ENABLE = uvm_reg_field::type_id::create("ESR_SC_REMOTE_L3_ENABLE", , get_full_name());
      ESR_SC_REMOTE_L3_ENABLE.configure(this, 1, 16, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_CBUF_ENABLE = uvm_reg_field::type_id::create("ESR_SC_CBUF_ENABLE", , get_full_name());
      ESR_SC_CBUF_ENABLE.configure(this, 1, 17, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_AXI_QOS = uvm_reg_field::type_id::create("ESR_SC_AXI_QOS", , get_full_name());
      ESR_SC_AXI_QOS.configure(this, 4, 18, "RW", 0, 4'h0, 1, 0, 0);
      ESR_SC_CLK_GATE_DISABLE = uvm_reg_field::type_id::create("ESR_SC_CLK_GATE_DISABLE", , get_full_name());
      ESR_SC_CLK_GATE_DISABLE.configure(this, 8, 22, "RW", 0, 8'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_ctl)

endclass : ral_reg_sc_reqq_ctl


class ral_reg_sc_pipe_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_CBUF_ENTRY_ENABLE;
   rand uvm_reg_field ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK;
   rand uvm_reg_field ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK;
   rand uvm_reg_field ESR_SC_ZERO_STATE_ENABLE;
   rand uvm_reg_field ESR_SC_SCP_RBUF_ENABLE;
   rand uvm_reg_field ESR_SC_L2_RBUF_ENABLE;
   rand uvm_reg_field ESR_SC_RAM_DELAY;
   rand uvm_reg_field ESR_SC_RAM_SHUT_DOWN;
   rand uvm_reg_field ESR_SC_RAM_DEEP_SLEEP;
   rand uvm_reg_field ESR_SC_IDX_COP_SM_CTL_USER_EN;

   function new(string name = "sc_pipe_ctl");
      super.new(name, 48, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_CBUF_ENTRY_ENABLE = uvm_reg_field::type_id::create("ESR_SC_CBUF_ENTRY_ENABLE", , get_full_name());
      ESR_SC_CBUF_ENTRY_ENABLE.configure(this, 32, 0, "RW", 0, 32'hffffffff, 1, 0, 1);
      ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK = uvm_reg_field::type_id::create("ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK", , get_full_name());
      ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK.configure(this, 1, 32, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK = uvm_reg_field::type_id::create("ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK", , get_full_name());
      ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK.configure(this, 1, 33, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_ZERO_STATE_ENABLE = uvm_reg_field::type_id::create("ESR_SC_ZERO_STATE_ENABLE", , get_full_name());
      ESR_SC_ZERO_STATE_ENABLE.configure(this, 1, 34, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_SCP_RBUF_ENABLE = uvm_reg_field::type_id::create("ESR_SC_SCP_RBUF_ENABLE", , get_full_name());
      ESR_SC_SCP_RBUF_ENABLE.configure(this, 1, 35, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_L2_RBUF_ENABLE = uvm_reg_field::type_id::create("ESR_SC_L2_RBUF_ENABLE", , get_full_name());
      ESR_SC_L2_RBUF_ENABLE.configure(this, 1, 36, "RW", 0, 1'h1, 1, 0, 0);
      ESR_SC_RAM_DELAY = uvm_reg_field::type_id::create("ESR_SC_RAM_DELAY", , get_full_name());
      ESR_SC_RAM_DELAY.configure(this, 3, 37, "RW", 0, 3'h2, 1, 0, 0);
      ESR_SC_RAM_SHUT_DOWN = uvm_reg_field::type_id::create("ESR_SC_RAM_SHUT_DOWN", , get_full_name());
      ESR_SC_RAM_SHUT_DOWN.configure(this, 1, 40, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_RAM_DEEP_SLEEP = uvm_reg_field::type_id::create("ESR_SC_RAM_DEEP_SLEEP", , get_full_name());
      ESR_SC_RAM_DEEP_SLEEP.configure(this, 1, 41, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_IDX_COP_SM_CTL_USER_EN = uvm_reg_field::type_id::create("ESR_SC_IDX_COP_SM_CTL_USER_EN", , get_full_name());
      ESR_SC_IDX_COP_SM_CTL_USER_EN.configure(this, 1, 42, "RW", 0, 1'h1, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_pipe_ctl)

endclass : ral_reg_sc_pipe_ctl


class ral_reg_sc_l2_cache_ctl extends uvm_reg;
   rand uvm_reg_field TAG_MASK;
   uvm_reg_field TAG_MASK_RSVD;
   rand uvm_reg_field SET_MASK;
   uvm_reg_field SET_MASK_RSVD;
   rand uvm_reg_field SET_SIZE;
   uvm_reg_field SET_SIZE_RSVD;
   rand uvm_reg_field SET_BASE;
   uvm_reg_field SET_BASE_RSVD;

   function new(string name = "sc_l2_cache_ctl");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      TAG_MASK = uvm_reg_field::type_id::create("TAG_MASK", , get_full_name());
      TAG_MASK.configure(this, 12, 0, "RW", 0, 12'h7f, 1, 0, 0);
      TAG_MASK_RSVD = uvm_reg_field::type_id::create("TAG_MASK_RSVD", , get_full_name());
      TAG_MASK_RSVD.configure(this, 4, 12, "RO", 0, 4'h0, 1, 0, 0);
      SET_MASK = uvm_reg_field::type_id::create("SET_MASK", , get_full_name());
      SET_MASK.configure(this, 12, 16, "RW", 0, 12'h7f, 1, 0, 0);
      SET_MASK_RSVD = uvm_reg_field::type_id::create("SET_MASK_RSVD", , get_full_name());
      SET_MASK_RSVD.configure(this, 4, 28, "RO", 0, 4'h0, 1, 0, 0);
      SET_SIZE = uvm_reg_field::type_id::create("SET_SIZE", , get_full_name());
      SET_SIZE.configure(this, 13, 32, "RW", 0, 13'h80, 1, 0, 0);
      SET_SIZE_RSVD = uvm_reg_field::type_id::create("SET_SIZE_RSVD", , get_full_name());
      SET_SIZE_RSVD.configure(this, 3, 45, "RO", 0, 3'h0, 1, 0, 0);
      SET_BASE = uvm_reg_field::type_id::create("SET_BASE", , get_full_name());
      SET_BASE.configure(this, 12, 48, "RW", 0, 12'h280, 1, 0, 0);
      SET_BASE_RSVD = uvm_reg_field::type_id::create("SET_BASE_RSVD", , get_full_name());
      SET_BASE_RSVD.configure(this, 4, 60, "RO", 0, 4'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_l2_cache_ctl)

endclass : ral_reg_sc_l2_cache_ctl


class ral_reg_sc_l3_cache_ctl extends uvm_reg;
   rand uvm_reg_field TAG_MASK;
   uvm_reg_field TAG_MASK_RSVD;
   rand uvm_reg_field SET_MASK;
   uvm_reg_field SET_MASK_RSVD;
   rand uvm_reg_field SET_SIZE;
   uvm_reg_field SET_SIZE_RSVD;
   rand uvm_reg_field SET_BASE;
   uvm_reg_field SET_BASE_RSVD;

   function new(string name = "sc_l3_cache_ctl");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      TAG_MASK = uvm_reg_field::type_id::create("TAG_MASK", , get_full_name());
      TAG_MASK.configure(this, 12, 0, "RW", 0, 12'hff, 1, 0, 0);
      TAG_MASK_RSVD = uvm_reg_field::type_id::create("TAG_MASK_RSVD", , get_full_name());
      TAG_MASK_RSVD.configure(this, 4, 12, "RO", 0, 4'h0, 1, 0, 0);
      SET_MASK = uvm_reg_field::type_id::create("SET_MASK", , get_full_name());
      SET_MASK.configure(this, 12, 16, "RW", 0, 12'hff, 1, 0, 0);
      SET_MASK_RSVD = uvm_reg_field::type_id::create("SET_MASK_RSVD", , get_full_name());
      SET_MASK_RSVD.configure(this, 4, 28, "RO", 0, 4'h0, 1, 0, 0);
      SET_SIZE = uvm_reg_field::type_id::create("SET_SIZE", , get_full_name());
      SET_SIZE.configure(this, 13, 32, "RW", 0, 13'h100, 1, 0, 0);
      SET_SIZE_RSVD = uvm_reg_field::type_id::create("SET_SIZE_RSVD", , get_full_name());
      SET_SIZE_RSVD.configure(this, 3, 45, "RO", 0, 3'h0, 1, 0, 0);
      SET_BASE = uvm_reg_field::type_id::create("SET_BASE", , get_full_name());
      SET_BASE.configure(this, 12, 48, "RW", 0, 12'h300, 1, 0, 0);
      SET_BASE_RSVD = uvm_reg_field::type_id::create("SET_BASE_RSVD", , get_full_name());
      SET_BASE_RSVD.configure(this, 4, 60, "RO", 0, 4'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_l3_cache_ctl)

endclass : ral_reg_sc_l3_cache_ctl


class ral_reg_sc_scp_cache_ctl extends uvm_reg;
   rand uvm_reg_field TAG_MASK;
   uvm_reg_field TAG_MASK_RSVD;
   rand uvm_reg_field SET_MASK;
   uvm_reg_field SET_MASK_RSVD;
   rand uvm_reg_field SET_SIZE;
   uvm_reg_field SET_SIZE_RSVD;
   rand uvm_reg_field SET_BASE;
   uvm_reg_field SET_BASE_RSVD;

   function new(string name = "sc_scp_cache_ctl");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      TAG_MASK = uvm_reg_field::type_id::create("TAG_MASK", , get_full_name());
      TAG_MASK.configure(this, 12, 0, "RW", 0, 12'h1ff, 1, 0, 0);
      TAG_MASK_RSVD = uvm_reg_field::type_id::create("TAG_MASK_RSVD", , get_full_name());
      TAG_MASK_RSVD.configure(this, 4, 12, "RO", 0, 4'h0, 1, 0, 0);
      SET_MASK = uvm_reg_field::type_id::create("SET_MASK", , get_full_name());
      SET_MASK.configure(this, 12, 16, "RW", 0, 12'h3ff, 1, 0, 0);
      SET_MASK_RSVD = uvm_reg_field::type_id::create("SET_MASK_RSVD", , get_full_name());
      SET_MASK_RSVD.configure(this, 4, 28, "RO", 0, 4'h0, 1, 0, 0);
      SET_SIZE = uvm_reg_field::type_id::create("SET_SIZE", , get_full_name());
      SET_SIZE.configure(this, 13, 32, "RW", 0, 13'h280, 1, 0, 0);
      SET_SIZE_RSVD = uvm_reg_field::type_id::create("SET_SIZE_RSVD", , get_full_name());
      SET_SIZE_RSVD.configure(this, 3, 45, "RO", 0, 3'h0, 1, 0, 0);
      SET_BASE = uvm_reg_field::type_id::create("SET_BASE", , get_full_name());
      SET_BASE.configure(this, 12, 48, "RW", 0, 12'h0, 1, 0, 0);
      SET_BASE_RSVD = uvm_reg_field::type_id::create("SET_BASE_RSVD", , get_full_name());
      SET_BASE_RSVD.configure(this, 4, 60, "RO", 0, 4'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_scp_cache_ctl)

endclass : ral_reg_sc_scp_cache_ctl


class ral_reg_sc_idx_cop_sm_ctl extends uvm_reg;
   rand uvm_reg_field GO;
   rand uvm_reg_field ABORT;
   rand uvm_reg_field OPCODE;
   rand uvm_reg_field ECC_WR_EN;
   rand uvm_reg_field LOGICAL_RAM;
   uvm_reg_field IDX_COP_SM_STATE;

   function new(string name = "sc_idx_cop_sm_ctl");
      super.new(name, 32, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      GO = uvm_reg_field::type_id::create("GO", , get_full_name());
      GO.configure(this, 1, 0, "WC", 0, 1'h0, 1, 0, 0);
      ABORT = uvm_reg_field::type_id::create("ABORT", , get_full_name());
      ABORT.configure(this, 1, 1, "WC", 0, 1'h0, 1, 0, 0);
      OPCODE = uvm_reg_field::type_id::create("OPCODE", , get_full_name());
      OPCODE.configure(this, 4, 8, "RW", 0, 4'h0, 1, 0, 0);
      ECC_WR_EN = uvm_reg_field::type_id::create("ECC_WR_EN", , get_full_name());
      ECC_WR_EN.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      LOGICAL_RAM = uvm_reg_field::type_id::create("LOGICAL_RAM", , get_full_name());
      LOGICAL_RAM.configure(this, 2, 14, "RW", 0, 2'h0, 1, 0, 0);
      IDX_COP_SM_STATE = uvm_reg_field::type_id::create("IDX_COP_SM_STATE", , get_full_name());
      IDX_COP_SM_STATE.configure(this, 8, 24, "RO", m_reg_is_volatile, 8'h4, 1, 0, 1);
      IDX_COP_SM_STATE.set_compare(UVM_NO_CHECK);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_ctl)

endclass : ral_reg_sc_idx_cop_sm_ctl


class ral_reg_sc_idx_cop_sm_ctl_user extends uvm_reg;
   rand uvm_reg_field GO;
   rand uvm_reg_field ABORT;
   rand uvm_reg_field OPCODE;
   rand uvm_reg_field ECC_WR_EN;
   rand uvm_reg_field LOGICAL_RAM;
   uvm_reg_field IDX_COP_SM_STATE;

   function new(string name = "sc_idx_cop_sm_ctl_user");
      super.new(name, 32, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      GO = uvm_reg_field::type_id::create("GO", , get_full_name());
      GO.configure(this, 1, 0, "WC", 0, 1'h0, 1, 0, 0);
      ABORT = uvm_reg_field::type_id::create("ABORT", , get_full_name());
      ABORT.configure(this, 1, 1, "WC", 0, 1'h0, 1, 0, 0);
      OPCODE = uvm_reg_field::type_id::create("OPCODE", , get_full_name());
      OPCODE.configure(this, 4, 8, "RW", 0, 4'h0, 1, 0, 0);
      ECC_WR_EN = uvm_reg_field::type_id::create("ECC_WR_EN", , get_full_name());
      ECC_WR_EN.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      LOGICAL_RAM = uvm_reg_field::type_id::create("LOGICAL_RAM", , get_full_name());
      LOGICAL_RAM.configure(this, 2, 14, "RW", 0, 2'h0, 1, 0, 0);
      IDX_COP_SM_STATE = uvm_reg_field::type_id::create("IDX_COP_SM_STATE", , get_full_name());
      IDX_COP_SM_STATE.configure(this, 8, 24, "RO", m_reg_is_volatile, 8'h4, 1, 0, 1);
      IDX_COP_SM_STATE.set_compare(UVM_NO_CHECK);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_ctl_user)

endclass : ral_reg_sc_idx_cop_sm_ctl_user

// These classes implement the aliasing between idx_cop_sm_ctl and idx_cop_sm_ctl_user. When one is written
// the fields are updated in the other as well.
// (See uvm_users_guide section 5.7.3)
class update_other_regs_field extends uvm_reg_cbs;
   local uvm_reg_field m_field_to_update;
   int     count;
   function new(uvm_reg_field field_to_update);
      m_field_to_update = field_to_update;
   endfunction

   virtual function void post_predict(input uvm_reg_field fld,
                                      input uvm_reg_data_t previous,
                                      inout uvm_reg_data_t value,
                                      input uvm_predict_e kind,
                                      input uvm_path_e path,
                                      input uvm_reg_map map);
      if (kind != UVM_PREDICT_WRITE) return;
      
      // This count is basically a way to avoid an infinite loop of these guys updating each other
      if (count == 0) begin
         count ++;
         void'(m_field_to_update.predict(value, -1, UVM_PREDICT_WRITE, path, map));
      end
      if (count > 0) begin
         count--;
      end
   endfunction // post_predict
endclass // update_other_regs_field

class alias_reg_ctl_user extends uvm_object;
   protected ral_reg_sc_idx_cop_sm_ctl      m_reg_ctl;
   protected ral_reg_sc_idx_cop_sm_ctl_user m_reg_ctl_user;

   `uvm_object_utils(alias_reg_ctl_user)

   function new(string name = "alias_reg_ctl_user");
      super.new(name);
   endfunction: new

   function void configure(ral_reg_sc_idx_cop_sm_ctl reg_ctl, ral_reg_sc_idx_cop_sm_ctl_user reg_ctl_user);
      update_other_regs_field update_opcode_reg_ctl;
      update_other_regs_field update_opcode_reg_ctl_user;
      update_other_regs_field update_ram_reg_ctl;
      update_other_regs_field update_ram_reg_ctl_user;
      update_other_regs_field update_ecc_reg_ctl;
      update_other_regs_field update_ecc_reg_ctl_user;
      m_reg_ctl = reg_ctl;
      m_reg_ctl_user = reg_ctl_user;
      // Callbacks for when the ctl_user register is written to update the ctl register fields
      update_opcode_reg_ctl = new(m_reg_ctl.OPCODE);
      uvm_reg_field_cb::add(m_reg_ctl_user.OPCODE, update_opcode_reg_ctl);
      update_ram_reg_ctl = new(m_reg_ctl.LOGICAL_RAM);
      uvm_reg_field_cb::add(m_reg_ctl_user.LOGICAL_RAM, update_ram_reg_ctl);
      update_ecc_reg_ctl = new(m_reg_ctl.ECC_WR_EN);
      uvm_reg_field_cb::add(m_reg_ctl_user.ECC_WR_EN, update_ecc_reg_ctl);
      // Callbacks for when the ctl register is written to update the ctl_user register fields
      update_opcode_reg_ctl_user = new(m_reg_ctl_user.OPCODE);
      uvm_reg_field_cb::add(m_reg_ctl.OPCODE, update_opcode_reg_ctl_user);
      update_ram_reg_ctl_user = new(m_reg_ctl_user.LOGICAL_RAM);
      uvm_reg_field_cb::add(m_reg_ctl.LOGICAL_RAM, update_ram_reg_ctl_user);
      update_ecc_reg_ctl_user = new(m_reg_ctl_user.ECC_WR_EN);
      uvm_reg_field_cb::add(m_reg_ctl.ECC_WR_EN, update_ecc_reg_ctl_user);
   endfunction : configure
endclass : alias_reg_ctl_user
      

class ral_reg_sc_idx_cop_sm_physical_index extends uvm_reg;
   rand uvm_reg_field QW;
   rand uvm_reg_field SUB_BANK;
   rand uvm_reg_field WAY;
   rand uvm_reg_field SET;

   function new(string name = "sc_idx_cop_sm_physical_index");
      super.new(name, 48, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      QW = uvm_reg_field::type_id::create("QW", , get_full_name());
      QW.configure(this, 2, 0, "RW", 0, 2'h0, 1, 0, 1);
      SUB_BANK = uvm_reg_field::type_id::create("SUB_BANK", , get_full_name());
      SUB_BANK.configure(this, 3, 8, "RW", 0, 3'h0, 1, 0, 1);
      WAY = uvm_reg_field::type_id::create("WAY", , get_full_name());
      WAY.configure(this, 2, 16, "RW", 0, 2'h0, 1, 0, 1);
      SET = uvm_reg_field::type_id::create("SET", , get_full_name());
      SET.configure(this, 16, 32, "RW", 0, 16'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_physical_index)

endclass : ral_reg_sc_idx_cop_sm_physical_index


class ral_reg_sc_idx_cop_sm_data0 extends uvm_reg;
   rand uvm_reg_field SC_IDX_COP_SM_DATA0;

   function new(string name = "sc_idx_cop_sm_data0");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_IDX_COP_SM_DATA0 = uvm_reg_field::type_id::create("SC_IDX_COP_SM_DATA0", , get_full_name());
      SC_IDX_COP_SM_DATA0.configure(this, 64, 0, "RW", m_reg_is_volatile, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_data0)

endclass : ral_reg_sc_idx_cop_sm_data0


class ral_reg_sc_idx_cop_sm_data1 extends uvm_reg;
   rand uvm_reg_field SC_IDX_COP_SM_DATA1;

   function new(string name = "sc_idx_cop_sm_data1");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_IDX_COP_SM_DATA1 = uvm_reg_field::type_id::create("SC_IDX_COP_SM_DATA1", , get_full_name());
      SC_IDX_COP_SM_DATA1.configure(this, 64, 0, "RW", m_reg_is_volatile, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_data1)

endclass : ral_reg_sc_idx_cop_sm_data1


class ral_reg_sc_idx_cop_sm_ecc extends uvm_reg;
   rand uvm_reg_field SC_IDX_COP_SM_ECC;

   function new(string name = "sc_idx_cop_sm_ecc");
      super.new(name, 24, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_IDX_COP_SM_ECC = uvm_reg_field::type_id::create("SC_IDX_COP_SM_ECC", , get_full_name());
      SC_IDX_COP_SM_ECC.configure(this, 24, 0, "RW", m_reg_is_volatile, 24'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_idx_cop_sm_ecc)

endclass : ral_reg_sc_idx_cop_sm_ecc


class ral_reg_sc_err_log_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_ERR_INTERRUPT_ENABLE;
   rand uvm_reg_field ESR_SC_ERR_RSP_ENABLE;

   function new(string name = "sc_err_log_ctl");
      super.new(name, 16, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_ERR_INTERRUPT_ENABLE = uvm_reg_field::type_id::create("ESR_SC_ERR_INTERRUPT_ENABLE", , get_full_name());
      ESR_SC_ERR_INTERRUPT_ENABLE.configure(this, 8, 0, "RW", 0, 8'hfe, 1, 0, 1);
      ESR_SC_ERR_RSP_ENABLE = uvm_reg_field::type_id::create("ESR_SC_ERR_RSP_ENABLE", , get_full_name());
      ESR_SC_ERR_RSP_ENABLE.configure(this, 1, 8, "RW", 0, 1'h1, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_err_log_ctl)

endclass : ral_reg_sc_err_log_ctl


class ral_reg_sc_err_log_info extends uvm_reg;
   rand uvm_reg_field SC_ERR_LOG_INFO;

   function new(string name = "sc_err_log_info");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_ERR_LOG_INFO = uvm_reg_field::type_id::create("SC_ERR_LOG_INFO", , get_full_name());
      SC_ERR_LOG_INFO.configure(this, 64, 0, "RW", m_reg_is_volatile, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_err_log_info)

endclass : ral_reg_sc_err_log_info


class ral_reg_sc_err_log_address extends uvm_reg;
   uvm_reg_field SC_ERR_LOG_ADDRESS;

   function new(string name = "sc_err_log_address");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_ERR_LOG_ADDRESS = uvm_reg_field::type_id::create("SC_ERR_LOG_ADDRESS", , get_full_name());
      SC_ERR_LOG_ADDRESS.configure(this, 64, 0, "RO", m_reg_is_volatile, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_err_log_address)

endclass : ral_reg_sc_err_log_address


class ral_reg_sc_sbe_dbe_counts extends uvm_reg;
   rand uvm_reg_field SC_SBE_DBE_COUNTS;

   function new(string name = "sc_sbe_dbe_counts");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_SBE_DBE_COUNTS = uvm_reg_field::type_id::create("SC_SBE_DBE_COUNTS", , get_full_name());
      SC_SBE_DBE_COUNTS.configure(this, 64, 0, "WC", m_reg_is_volatile, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_sbe_dbe_counts)

endclass : ral_reg_sc_sbe_dbe_counts


class ral_reg_sc_reqq_debug_ctl extends uvm_reg;
   rand uvm_reg_field SC_REQQ_DEBUG_CTL;

   function new(string name = "sc_reqq_debug_ctl");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_REQQ_DEBUG_CTL = uvm_reg_field::type_id::create("SC_REQQ_DEBUG_CTL", , get_full_name());
      SC_REQQ_DEBUG_CTL.configure(this, 64, 0, "WC", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_debug_ctl)

endclass : ral_reg_sc_reqq_debug_ctl


class ral_reg_sc_reqq_debug0 extends uvm_reg;
   uvm_reg_field SC_REQQ_DEBUG0;

   function new(string name = "sc_reqq_debug0");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_REQQ_DEBUG0 = uvm_reg_field::type_id::create("SC_REQQ_DEBUG0", , get_full_name());
      SC_REQQ_DEBUG0.configure(this, 64, 0, "RO", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_debug0)

endclass : ral_reg_sc_reqq_debug0


class ral_reg_sc_reqq_debug1 extends uvm_reg;
   uvm_reg_field SC_REQQ_DEBUG1;

   function new(string name = "sc_reqq_debug1");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_REQQ_DEBUG1 = uvm_reg_field::type_id::create("SC_REQQ_DEBUG1", , get_full_name());
      SC_REQQ_DEBUG1.configure(this, 64, 0, "RO", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_debug1)

endclass : ral_reg_sc_reqq_debug1


class ral_reg_sc_reqq_debug2 extends uvm_reg;
   uvm_reg_field SC_REQQ_DEBUG2;

   function new(string name = "sc_reqq_debug2");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_REQQ_DEBUG2 = uvm_reg_field::type_id::create("SC_REQQ_DEBUG2", , get_full_name());
      SC_REQQ_DEBUG2.configure(this, 64, 0, "RO", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_debug2)

endclass : ral_reg_sc_reqq_debug2


class ral_reg_sc_reqq_debug3 extends uvm_reg;
   uvm_reg_field SC_REQQ_DEBUG3;

   function new(string name = "sc_reqq_debug3");
      super.new(name, 64, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_REQQ_DEBUG3 = uvm_reg_field::type_id::create("SC_REQQ_DEBUG3", , get_full_name());
      SC_REQQ_DEBUG3.configure(this, 64, 0, "RO", 0, 64'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_reqq_debug3)

endclass : ral_reg_sc_reqq_debug3


class ral_reg_sc_eco_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_ECO_CTL;

   function new(string name = "sc_eco_ctl");
      super.new(name, 8, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_ECO_CTL = uvm_reg_field::type_id::create("ESR_SC_ECO_CTL", , get_full_name());
      ESR_SC_ECO_CTL.configure(this, 8, 0, "RW", 0, 8'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_eco_ctl)

endclass : ral_reg_sc_eco_ctl


class ral_reg_sc_trace_address_enable extends uvm_reg;
   rand uvm_reg_field ESR_SC_TRACE_ADDRESS_ENABLE;

   function new(string name = "sc_trace_address_enable");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_TRACE_ADDRESS_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_ADDRESS_ENABLE", , get_full_name());
      ESR_SC_TRACE_ADDRESS_ENABLE.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_trace_address_enable)

endclass : ral_reg_sc_trace_address_enable


class ral_reg_sc_trace_address_value extends uvm_reg;
   rand uvm_reg_field ESR_SC_TRACE_ADDRESS_VALUE;

   function new(string name = "sc_trace_address_value");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_TRACE_ADDRESS_VALUE = uvm_reg_field::type_id::create("ESR_SC_TRACE_ADDRESS_VALUE", , get_full_name());
      ESR_SC_TRACE_ADDRESS_VALUE.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_trace_address_value)

endclass : ral_reg_sc_trace_address_value


class ral_reg_sc_trace_ctl extends uvm_reg;
   rand uvm_reg_field ESR_SC_TRACE_TYPE_HOT_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_FSM_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_L3_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_L2_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_SOURCE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_SOURCE_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_PORT;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_PORT_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_REQQ_ID;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE;

   function new(string name = "sc_trace_ctl");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_SC_TRACE_TYPE_HOT_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_TYPE_HOT_ENABLE", , get_full_name());
      ESR_SC_TRACE_TYPE_HOT_ENABLE.configure(this, 12, 0, "RW", 0, 12'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_FSM_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_FSM_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_FSM_ENABLE.configure(this, 1, 12, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_L3_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_L3_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_L3_ENABLE.configure(this, 1, 13, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_L2_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_L2_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_L2_ENABLE.configure(this, 1, 14, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_SOURCE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_SOURCE", , get_full_name());
      ESR_SC_TRACE_FILTER_SOURCE.configure(this, 10, 15, "RW", 0, 10'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_SOURCE_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_SOURCE_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_SOURCE_ENABLE.configure(this, 1, 25, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_PORT = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_PORT", , get_full_name());
      ESR_SC_TRACE_FILTER_PORT.configure(this, 3, 26, "RW", 0, 3'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_PORT_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_PORT_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_PORT_ENABLE.configure(this, 1, 29, "RW", 0, 1'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_REQQ_ID = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_REQQ_ID", , get_full_name());
      ESR_SC_TRACE_FILTER_REQQ_ID.configure(this, 6, 30, "RW", 0, 6'h0, 1, 0, 0);
      ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE = uvm_reg_field::type_id::create("ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE", , get_full_name());
      ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE.configure(this, 1, 36, "RW", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_trace_ctl)

endclass : ral_reg_sc_trace_ctl


class ral_reg_sc_perfmon_ctl_status extends uvm_reg;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_EVENT;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_MODE;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_EVENT;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_MODE;
   rand uvm_reg_field ESR_PERFMON_ANY_OVERFLOW;
   uvm_reg_field ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW;
   uvm_reg_field ESR_PERFMON_P0_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW;
   uvm_reg_field ESR_PERFMON_P1_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW;

   function new(string name = "sc_perfmon_ctl_status");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      ESR_PERFMON_CYCLE_CNTR_START = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_START", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_START.configure(this, 1, 0, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_CYCLE_CNTR_RESET = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_RESET", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_RESET.configure(this, 1, 1, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_CYCLE_CNTR_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_OVERFLOW", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_OVERFLOW.configure(this, 1, 2, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_CYCLE_CNTR_INTERRUPT = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_INTERRUPT", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_INTERRUPT.configure(this, 1, 3, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_START = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_START", , get_full_name());
      ESR_PERFMON_P0_CNTR_START.configure(this, 1, 4, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_RESET = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_RESET", , get_full_name());
      ESR_PERFMON_P0_CNTR_RESET.configure(this, 1, 5, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_OVERFLOW", , get_full_name());
      ESR_PERFMON_P0_CNTR_OVERFLOW.configure(this, 1, 6, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_INTERRUPT = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_INTERRUPT", , get_full_name());
      ESR_PERFMON_P0_CNTR_INTERRUPT.configure(this, 1, 7, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_EVENT = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_EVENT", , get_full_name());
      ESR_PERFMON_P0_CNTR_EVENT.configure(this, 1, 8, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_MODE = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_MODE", , get_full_name());
      ESR_PERFMON_P0_CNTR_MODE.configure(this, 8, 9, "RW", 0, 8'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_START = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_START", , get_full_name());
      ESR_PERFMON_P1_CNTR_START.configure(this, 1, 17, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_RESET = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_RESET", , get_full_name());
      ESR_PERFMON_P1_CNTR_RESET.configure(this, 1, 18, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_OVERFLOW", , get_full_name());
      ESR_PERFMON_P1_CNTR_OVERFLOW.configure(this, 1, 19, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_INTERRUPT = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_INTERRUPT", , get_full_name());
      ESR_PERFMON_P1_CNTR_INTERRUPT.configure(this, 1, 20, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_EVENT = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_EVENT", , get_full_name());
      ESR_PERFMON_P1_CNTR_EVENT.configure(this, 1, 21, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_MODE = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_MODE", , get_full_name());
      ESR_PERFMON_P1_CNTR_MODE.configure(this, 8, 22, "RW", 0, 8'h0, 1, 0, 0);
      ESR_PERFMON_ANY_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_ANY_OVERFLOW", , get_full_name());
      ESR_PERFMON_ANY_OVERFLOW.configure(this, 1, 30, "RW", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE.configure(this, 1, 32, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW", , get_full_name());
      ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW.configure(this, 1, 33, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_STATUS_ACTIVE = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_STATUS_ACTIVE", , get_full_name());
      ESR_PERFMON_P0_CNTR_STATUS_ACTIVE.configure(this, 1, 34, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW", , get_full_name());
      ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW.configure(this, 1, 35, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_STATUS_ACTIVE = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_STATUS_ACTIVE", , get_full_name());
      ESR_PERFMON_P1_CNTR_STATUS_ACTIVE.configure(this, 1, 36, "RO", 0, 1'h0, 1, 0, 0);
      ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW = uvm_reg_field::type_id::create("ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW", , get_full_name());
      ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW.configure(this, 1, 37, "RO", 0, 1'h0, 1, 0, 0);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_ctl_status)

endclass : ral_reg_sc_perfmon_ctl_status


class ral_reg_sc_perfmon_cyc_cntr extends uvm_reg;
   rand uvm_reg_field SC_PERFMON_CYC_CNTR;

   function new(string name = "sc_perfmon_cyc_cntr");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_PERFMON_CYC_CNTR = uvm_reg_field::type_id::create("SC_PERFMON_CYC_CNTR", , get_full_name());
      SC_PERFMON_CYC_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_cyc_cntr)

endclass : ral_reg_sc_perfmon_cyc_cntr


class ral_reg_sc_perfmon_p0_cntr extends uvm_reg;
   rand uvm_reg_field SC_PERFMON_P0_CNTR;

   function new(string name = "sc_perfmon_p0_cntr");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_PERFMON_P0_CNTR = uvm_reg_field::type_id::create("SC_PERFMON_P0_CNTR", , get_full_name());
      SC_PERFMON_P0_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_p0_cntr)

endclass : ral_reg_sc_perfmon_p0_cntr


class ral_reg_sc_perfmon_p1_cntr extends uvm_reg;
   rand uvm_reg_field SC_PERFMON_P1_CNTR;

   function new(string name = "sc_perfmon_p1_cntr");
      super.new(name, 40, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_PERFMON_P1_CNTR = uvm_reg_field::type_id::create("SC_PERFMON_P1_CNTR", , get_full_name());
      SC_PERFMON_P1_CNTR.configure(this, 40, 0, "RW", 0, 40'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_p1_cntr)

endclass : ral_reg_sc_perfmon_p1_cntr


class ral_reg_sc_perfmon_p0_qual extends uvm_reg;
   rand uvm_reg_field SC_PERFMON_P0_QUAL;

   function new(string name = "sc_perfmon_p0_qual");
      super.new(name, 56, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_PERFMON_P0_QUAL = uvm_reg_field::type_id::create("SC_PERFMON_P0_QUAL", , get_full_name());
      SC_PERFMON_P0_QUAL.configure(this, 51, 0, "RW", 0, 51'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_p0_qual)

endclass : ral_reg_sc_perfmon_p0_qual


class ral_reg_sc_perfmon_p1_qual extends uvm_reg;
   rand uvm_reg_field SC_PERFMON_P1_QUAL;

   function new(string name = "sc_perfmon_p1_qual");
      super.new(name, 56, UVM_NO_COVERAGE);
   endfunction: new

   virtual function void build();
      SC_PERFMON_P1_QUAL = uvm_reg_field::type_id::create("SC_PERFMON_P1_QUAL", , get_full_name());
      SC_PERFMON_P1_QUAL.configure(this, 51, 0, "RW", 0, 51'h0, 1, 0, 1);
   endfunction: build

   `uvm_object_utils(ral_reg_sc_perfmon_p1_qual)

endclass : ral_reg_sc_perfmon_p1_qual


class ral_block_sc_bank_regs extends evl_reg_block;
   rand ral_reg_sc_l3_shire_swizzle_ctl sc_l3_shire_swizzle_ctl;
   rand ral_reg_sc_reqq_ctl sc_reqq_ctl;
   rand ral_reg_sc_pipe_ctl sc_pipe_ctl;
   rand ral_reg_sc_l2_cache_ctl sc_l2_cache_ctl;
   rand ral_reg_sc_l3_cache_ctl sc_l3_cache_ctl;
   rand ral_reg_sc_scp_cache_ctl sc_scp_cache_ctl;
   rand ral_reg_sc_idx_cop_sm_ctl sc_idx_cop_sm_ctl;
   rand ral_reg_sc_idx_cop_sm_ctl_user sc_idx_cop_sm_ctl_user;
   rand ral_reg_sc_idx_cop_sm_physical_index sc_idx_cop_sm_physical_index;
   rand ral_reg_sc_idx_cop_sm_data0 sc_idx_cop_sm_data0;
   rand ral_reg_sc_idx_cop_sm_data1 sc_idx_cop_sm_data1;
   rand ral_reg_sc_idx_cop_sm_ecc sc_idx_cop_sm_ecc;
   rand ral_reg_sc_err_log_ctl sc_err_log_ctl;
   rand ral_reg_sc_err_log_info sc_err_log_info;
   rand ral_reg_sc_err_log_address sc_err_log_address;
   rand ral_reg_sc_sbe_dbe_counts sc_sbe_dbe_counts;
   rand ral_reg_sc_reqq_debug_ctl sc_reqq_debug_ctl;
   rand ral_reg_sc_reqq_debug0 sc_reqq_debug0;
   rand ral_reg_sc_reqq_debug1 sc_reqq_debug1;
   rand ral_reg_sc_reqq_debug2 sc_reqq_debug2;
   rand ral_reg_sc_reqq_debug3 sc_reqq_debug3;
   rand ral_reg_sc_eco_ctl sc_eco_ctl;
   rand ral_reg_sc_trace_address_enable sc_trace_address_enable;
   rand ral_reg_sc_trace_address_value sc_trace_address_value;
   rand ral_reg_sc_trace_ctl sc_trace_ctl;
   rand ral_reg_sc_perfmon_ctl_status sc_perfmon_ctl_status;
   rand ral_reg_sc_perfmon_cyc_cntr sc_perfmon_cyc_cntr;
   rand ral_reg_sc_perfmon_p0_cntr sc_perfmon_p0_cntr;
   rand ral_reg_sc_perfmon_p1_cntr sc_perfmon_p1_cntr;
   rand ral_reg_sc_perfmon_p0_qual sc_perfmon_p0_qual;
   rand ral_reg_sc_perfmon_p1_qual sc_perfmon_p1_qual;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B0;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B0;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B1;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B1;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B2;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B2;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B3;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B3;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B4;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B4;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B5;
   rand uvm_reg_field ESR_SC_SHIRE_SEL_B5;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B0;
   rand uvm_reg_field ESR_SC_BANK_SEL_B0;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B1;
   rand uvm_reg_field ESR_SC_BANK_SEL_B1;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B2;
   rand uvm_reg_field ESR_SC_BANK_SEL_B2;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B0;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B0;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B1;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B1;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B2;
   rand uvm_reg_field ESR_SC_SUB_BANK_SEL_B2;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_ALL_SHIRE_ALIASING;
   rand uvm_reg_field ESR_SC_ALL_SHIRE_ALIASING;
   rand uvm_reg_field sc_l3_shire_swizzle_ctl_ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB;
   rand uvm_reg_field ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_L3_YIELD_PRIORITY;
   rand uvm_reg_field ESR_SC_L3_YIELD_PRIORITY;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_ECC_SCRUB_ENABLE;
   rand uvm_reg_field ESR_SC_ECC_SCRUB_ENABLE;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_REQQ_NO_LINK_LIST;
   rand uvm_reg_field ESR_SC_REQQ_NO_LINK_LIST;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_NUM_L3_REQQ_ENTRIES;
   rand uvm_reg_field ESR_SC_NUM_L3_REQQ_ENTRIES;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_L3_BYPASS;
   rand uvm_reg_field ESR_SC_L3_BYPASS;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_L2_BYPASS;
   rand uvm_reg_field ESR_SC_L2_BYPASS;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_REMOTE_SCP_ENABLE;
   rand uvm_reg_field ESR_SC_REMOTE_SCP_ENABLE;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_REMOTE_L3_ENABLE;
   rand uvm_reg_field ESR_SC_REMOTE_L3_ENABLE;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_CBUF_ENABLE;
   rand uvm_reg_field ESR_SC_CBUF_ENABLE;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_AXI_QOS;
   rand uvm_reg_field ESR_SC_AXI_QOS;
   rand uvm_reg_field sc_reqq_ctl_ESR_SC_CLK_GATE_DISABLE;
   rand uvm_reg_field ESR_SC_CLK_GATE_DISABLE;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_CBUF_ENTRY_ENABLE;
   rand uvm_reg_field ESR_SC_CBUF_ENTRY_ENABLE;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK;
   rand uvm_reg_field ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK;
   rand uvm_reg_field ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_ZERO_STATE_ENABLE;
   rand uvm_reg_field ESR_SC_ZERO_STATE_ENABLE;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_SCP_RBUF_ENABLE;
   rand uvm_reg_field ESR_SC_SCP_RBUF_ENABLE;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_L2_RBUF_ENABLE;
   rand uvm_reg_field ESR_SC_L2_RBUF_ENABLE;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_RAM_DELAY;
   rand uvm_reg_field ESR_SC_RAM_DELAY;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_RAM_SHUT_DOWN;
   rand uvm_reg_field ESR_SC_RAM_SHUT_DOWN;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_RAM_DEEP_SLEEP;
   rand uvm_reg_field ESR_SC_RAM_DEEP_SLEEP;
   rand uvm_reg_field sc_pipe_ctl_ESR_SC_IDX_COP_SM_CTL_USER_EN;
   rand uvm_reg_field ESR_SC_IDX_COP_SM_CTL_USER_EN;
   rand uvm_reg_field sc_l2_cache_ctl_TAG_MASK;
   uvm_reg_field sc_l2_cache_ctl_TAG_MASK_RSVD;
   rand uvm_reg_field sc_l2_cache_ctl_SET_MASK;
   uvm_reg_field sc_l2_cache_ctl_SET_MASK_RSVD;
   rand uvm_reg_field sc_l2_cache_ctl_SET_SIZE;
   uvm_reg_field sc_l2_cache_ctl_SET_SIZE_RSVD;
   rand uvm_reg_field sc_l2_cache_ctl_SET_BASE;
   uvm_reg_field sc_l2_cache_ctl_SET_BASE_RSVD;
   rand uvm_reg_field sc_l3_cache_ctl_TAG_MASK;
   uvm_reg_field sc_l3_cache_ctl_TAG_MASK_RSVD;
   rand uvm_reg_field sc_l3_cache_ctl_SET_MASK;
   uvm_reg_field sc_l3_cache_ctl_SET_MASK_RSVD;
   rand uvm_reg_field sc_l3_cache_ctl_SET_SIZE;
   uvm_reg_field sc_l3_cache_ctl_SET_SIZE_RSVD;
   rand uvm_reg_field sc_l3_cache_ctl_SET_BASE;
   uvm_reg_field sc_l3_cache_ctl_SET_BASE_RSVD;
   rand uvm_reg_field sc_scp_cache_ctl_TAG_MASK;
   uvm_reg_field sc_scp_cache_ctl_TAG_MASK_RSVD;
   rand uvm_reg_field sc_scp_cache_ctl_SET_MASK;
   uvm_reg_field sc_scp_cache_ctl_SET_MASK_RSVD;
   rand uvm_reg_field sc_scp_cache_ctl_SET_SIZE;
   uvm_reg_field sc_scp_cache_ctl_SET_SIZE_RSVD;
   rand uvm_reg_field sc_scp_cache_ctl_SET_BASE;
   uvm_reg_field sc_scp_cache_ctl_SET_BASE_RSVD;
   rand uvm_reg_field sc_idx_cop_sm_ctl_GO;
   rand uvm_reg_field GO;
   rand uvm_reg_field sc_idx_cop_sm_ctl_ABORT;
   rand uvm_reg_field ABORT;
   rand uvm_reg_field sc_idx_cop_sm_ctl_OPCODE;
   rand uvm_reg_field OPCODE;
   rand uvm_reg_field sc_idx_cop_sm_ctl_ECC_WR_EN;
   rand uvm_reg_field ECC_WR_EN;
   rand uvm_reg_field sc_idx_cop_sm_ctl_LOGICAL_RAM;
   rand uvm_reg_field LOGICAL_RAM;
   uvm_reg_field sc_idx_cop_sm_ctl_IDX_COP_SM_STATE;
   uvm_reg_field IDX_COP_SM_STATE;
   rand uvm_reg_field sc_idx_cop_sm_ctl_user_GO;
   //rand uvm_reg_field GO;
   rand uvm_reg_field sc_idx_cop_sm_ctl_user_ABORT;
   //rand uvm_reg_field ABORT;
   rand uvm_reg_field sc_idx_cop_sm_ctl_user_OPCODE;
   //rand uvm_reg_field OPCODE;
   rand uvm_reg_field sc_idx_cop_sm_ctl_user_ECC_WR_EN;
   //rand uvm_reg_field ECC_WR_EN;
   rand uvm_reg_field sc_idx_cop_sm_ctl_user_LOGICAL_RAM;
   //rand uvm_reg_field LOGICAL_RAM;
   uvm_reg_field sc_idx_cop_sm_ctl_user_IDX_COP_SM_STATE;
   //uvm_reg_field IDX_COP_SM_STATE;
   rand uvm_reg_field sc_idx_cop_sm_physical_index_QW;
   rand uvm_reg_field QW;
   rand uvm_reg_field sc_idx_cop_sm_physical_index_SUB_BANK;
   rand uvm_reg_field SUB_BANK;
   rand uvm_reg_field sc_idx_cop_sm_physical_index_WAY;
   rand uvm_reg_field WAY;
   rand uvm_reg_field sc_idx_cop_sm_physical_index_SET;
   rand uvm_reg_field SET;
   rand uvm_reg_field sc_idx_cop_sm_data0_SC_IDX_COP_SM_DATA0;
   rand uvm_reg_field SC_IDX_COP_SM_DATA0;
   rand uvm_reg_field sc_idx_cop_sm_data1_SC_IDX_COP_SM_DATA1;
   rand uvm_reg_field SC_IDX_COP_SM_DATA1;
   rand uvm_reg_field sc_idx_cop_sm_ecc_SC_IDX_COP_SM_ECC;
   rand uvm_reg_field SC_IDX_COP_SM_ECC;
   rand uvm_reg_field sc_err_log_ctl_ESR_SC_ERR_INTERRUPT_ENABLE;
   rand uvm_reg_field ESR_SC_ERR_INTERRUPT_ENABLE;
   rand uvm_reg_field sc_err_log_ctl_ESR_SC_ERR_RSP_ENABLE;
   rand uvm_reg_field ESR_SC_ERR_RSP_ENABLE;
   rand uvm_reg_field sc_err_log_info_SC_ERR_LOG_INFO;
   rand uvm_reg_field SC_ERR_LOG_INFO;
   uvm_reg_field sc_err_log_address_SC_ERR_LOG_ADDRESS;
   uvm_reg_field SC_ERR_LOG_ADDRESS;
   rand uvm_reg_field sc_sbe_dbe_counts_SC_SBE_DBE_COUNTS;
   rand uvm_reg_field SC_SBE_DBE_COUNTS;
   rand uvm_reg_field sc_reqq_debug_ctl_SC_REQQ_DEBUG_CTL;
   rand uvm_reg_field SC_REQQ_DEBUG_CTL;
   uvm_reg_field sc_reqq_debug0_SC_REQQ_DEBUG0;
   uvm_reg_field SC_REQQ_DEBUG0;
   uvm_reg_field sc_reqq_debug1_SC_REQQ_DEBUG1;
   uvm_reg_field SC_REQQ_DEBUG1;
   uvm_reg_field sc_reqq_debug2_SC_REQQ_DEBUG2;
   uvm_reg_field SC_REQQ_DEBUG2;
   uvm_reg_field sc_reqq_debug3_SC_REQQ_DEBUG3;
   uvm_reg_field SC_REQQ_DEBUG3;
   rand uvm_reg_field sc_eco_ctl_ESR_SC_ECO_CTL;
   rand uvm_reg_field ESR_SC_ECO_CTL;
   rand uvm_reg_field sc_trace_address_enable_ESR_SC_TRACE_ADDRESS_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_ADDRESS_ENABLE;
   rand uvm_reg_field sc_trace_address_value_ESR_SC_TRACE_ADDRESS_VALUE;
   rand uvm_reg_field ESR_SC_TRACE_ADDRESS_VALUE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_TYPE_HOT_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_TYPE_HOT_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_FSM_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_FSM_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_L3_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_L3_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_L2_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_L2_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_SOURCE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_SOURCE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_SOURCE_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_SOURCE_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_PORT;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_PORT;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_PORT_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_PORT_ENABLE;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_REQQ_ID;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_REQQ_ID;
   rand uvm_reg_field sc_trace_ctl_ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE;
   rand uvm_reg_field ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_START;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_RESET;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_OVERFLOW;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_CYCLE_CNTR_INTERRUPT;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_START;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_RESET;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_OVERFLOW;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_INTERRUPT;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_EVENT;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_EVENT;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_MODE;
   rand uvm_reg_field ESR_PERFMON_P0_CNTR_MODE;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_START;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_START;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_RESET;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_RESET;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_OVERFLOW;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_INTERRUPT;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_INTERRUPT;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_EVENT;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_EVENT;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_MODE;
   rand uvm_reg_field ESR_PERFMON_P1_CNTR_MODE;
   rand uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_ANY_OVERFLOW;
   rand uvm_reg_field ESR_PERFMON_ANY_OVERFLOW;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW;
   uvm_reg_field ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_P0_CNTR_STATUS_ACTIVE;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW;
   uvm_reg_field ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_STATUS_ACTIVE;
   uvm_reg_field ESR_PERFMON_P1_CNTR_STATUS_ACTIVE;
   uvm_reg_field sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW;
   uvm_reg_field ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW;
   rand uvm_reg_field sc_perfmon_cyc_cntr_SC_PERFMON_CYC_CNTR;
   rand uvm_reg_field SC_PERFMON_CYC_CNTR;
   rand uvm_reg_field sc_perfmon_p0_cntr_SC_PERFMON_P0_CNTR;
   rand uvm_reg_field SC_PERFMON_P0_CNTR;
   rand uvm_reg_field sc_perfmon_p1_cntr_SC_PERFMON_P1_CNTR;
   rand uvm_reg_field SC_PERFMON_P1_CNTR;
   rand uvm_reg_field sc_perfmon_p0_qual_SC_PERFMON_P0_QUAL;
   rand uvm_reg_field SC_PERFMON_P0_QUAL;
   rand uvm_reg_field sc_perfmon_p1_qual_SC_PERFMON_P1_QUAL;
   rand uvm_reg_field SC_PERFMON_P1_QUAL;

   function new(input string name_in = "sc_bank_regs", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      uvm_reg_map apb_map;
      uvm_reg_map axi_map;

      axi_map = create_map("AXI", uvm_reg_addr_t'(base_addr), 8, UVM_LITTLE_ENDIAN, 0);
      apb_map = create_map("APB_ESR", uvm_reg_addr_t'(base_addr), 8, UVM_LITTLE_ENDIAN, 0);

      sc_l3_shire_swizzle_ctl = ral_reg_sc_l3_shire_swizzle_ctl::type_id::create("sc_l3_shire_swizzle_ctl", , get_full_name());
      sc_l3_shire_swizzle_ctl.configure(this, null, "reg_sc_l3_shire_swizzle_ctl");
      sc_l3_shire_swizzle_ctl.build();
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B0;
      ESR_SC_SHIRE_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B0;
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B1;
      ESR_SC_SHIRE_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B1;
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B2;
      ESR_SC_SHIRE_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B2;
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B3 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B3;
      ESR_SC_SHIRE_SEL_B3 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B3;
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B4 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B4;
      ESR_SC_SHIRE_SEL_B4 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B4;
      sc_l3_shire_swizzle_ctl_ESR_SC_SHIRE_SEL_B5 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B5;
      ESR_SC_SHIRE_SEL_B5 = sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B5;
      sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B0;
      ESR_SC_BANK_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B0;
      sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B1;
      ESR_SC_BANK_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B1;
      sc_l3_shire_swizzle_ctl_ESR_SC_BANK_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B2;
      ESR_SC_BANK_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B2;
      sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B0;
      ESR_SC_SUB_BANK_SEL_B0 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B0;
      sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B1;
      ESR_SC_SUB_BANK_SEL_B1 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B1;
      sc_l3_shire_swizzle_ctl_ESR_SC_SUB_BANK_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B2;
      ESR_SC_SUB_BANK_SEL_B2 = sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B2;
      sc_l3_shire_swizzle_ctl_ESR_SC_ALL_SHIRE_ALIASING = sc_l3_shire_swizzle_ctl.ESR_SC_ALL_SHIRE_ALIASING;
      ESR_SC_ALL_SHIRE_ALIASING = sc_l3_shire_swizzle_ctl.ESR_SC_ALL_SHIRE_ALIASING;
      sc_l3_shire_swizzle_ctl_ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB = sc_l3_shire_swizzle_ctl.ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB;
      ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB = sc_l3_shire_swizzle_ctl.ESR_SC_TWO_SHIRE_ALIASING_USE_SHIRE_LSB;
      apb_map.add_reg(sc_l3_shire_swizzle_ctl, `UVM_REG_ADDR_WIDTH'h00000c00, "RW", 0);
      axi_map.add_reg(sc_l3_shire_swizzle_ctl, `UVM_REG_ADDR_WIDTH'h18000000 << 3, "RW", 0);

      sc_reqq_ctl = ral_reg_sc_reqq_ctl::type_id::create("sc_reqq_ctl", , get_full_name());
      sc_reqq_ctl.configure(this, null, "reg_sc_reqq_ctl");
      sc_reqq_ctl.build();
      sc_reqq_ctl_ESR_SC_L3_YIELD_PRIORITY = sc_reqq_ctl.ESR_SC_L3_YIELD_PRIORITY;
      ESR_SC_L3_YIELD_PRIORITY = sc_reqq_ctl.ESR_SC_L3_YIELD_PRIORITY;
      sc_reqq_ctl_ESR_SC_ECC_SCRUB_ENABLE = sc_reqq_ctl.ESR_SC_ECC_SCRUB_ENABLE;
      ESR_SC_ECC_SCRUB_ENABLE = sc_reqq_ctl.ESR_SC_ECC_SCRUB_ENABLE;
      sc_reqq_ctl_ESR_SC_REQQ_NO_LINK_LIST = sc_reqq_ctl.ESR_SC_REQQ_NO_LINK_LIST;
      ESR_SC_REQQ_NO_LINK_LIST = sc_reqq_ctl.ESR_SC_REQQ_NO_LINK_LIST;
      sc_reqq_ctl_ESR_SC_NUM_L3_REQQ_ENTRIES = sc_reqq_ctl.ESR_SC_NUM_L3_REQQ_ENTRIES;
      ESR_SC_NUM_L3_REQQ_ENTRIES = sc_reqq_ctl.ESR_SC_NUM_L3_REQQ_ENTRIES;
      sc_reqq_ctl_ESR_SC_L3_BYPASS = sc_reqq_ctl.ESR_SC_L3_BYPASS;
      ESR_SC_L3_BYPASS = sc_reqq_ctl.ESR_SC_L3_BYPASS;
      sc_reqq_ctl_ESR_SC_L2_BYPASS = sc_reqq_ctl.ESR_SC_L2_BYPASS;
      ESR_SC_L2_BYPASS = sc_reqq_ctl.ESR_SC_L2_BYPASS;
      sc_reqq_ctl_ESR_SC_REMOTE_SCP_ENABLE = sc_reqq_ctl.ESR_SC_REMOTE_SCP_ENABLE;
      ESR_SC_REMOTE_SCP_ENABLE = sc_reqq_ctl.ESR_SC_REMOTE_SCP_ENABLE;
      sc_reqq_ctl_ESR_SC_REMOTE_L3_ENABLE = sc_reqq_ctl.ESR_SC_REMOTE_L3_ENABLE;
      ESR_SC_REMOTE_L3_ENABLE = sc_reqq_ctl.ESR_SC_REMOTE_L3_ENABLE;
      sc_reqq_ctl_ESR_SC_CBUF_ENABLE = sc_reqq_ctl.ESR_SC_CBUF_ENABLE;
      ESR_SC_CBUF_ENABLE = sc_reqq_ctl.ESR_SC_CBUF_ENABLE;
      sc_reqq_ctl_ESR_SC_AXI_QOS = sc_reqq_ctl.ESR_SC_AXI_QOS;
      ESR_SC_AXI_QOS = sc_reqq_ctl.ESR_SC_AXI_QOS;
      sc_reqq_ctl_ESR_SC_AXI_QOS = sc_reqq_ctl.ESR_SC_CLK_GATE_DISABLE;
      ESR_SC_CLK_GATE_DISABLE = sc_reqq_ctl.ESR_SC_CLK_GATE_DISABLE;
      apb_map.add_reg(sc_reqq_ctl, `UVM_REG_ADDR_WIDTH'h00000c01, "RW", 0);
      axi_map.add_reg(sc_reqq_ctl, `UVM_REG_ADDR_WIDTH'h18000001 << 3, "RW", 0);

      sc_pipe_ctl = ral_reg_sc_pipe_ctl::type_id::create("sc_pipe_ctl", , get_full_name());
      sc_pipe_ctl.configure(this, null, "reg_sc_pipe_ctl");
      sc_pipe_ctl.build();
      sc_pipe_ctl_ESR_SC_CBUF_ENTRY_ENABLE = sc_pipe_ctl.ESR_SC_CBUF_ENTRY_ENABLE;
      ESR_SC_CBUF_ENTRY_ENABLE = sc_pipe_ctl.ESR_SC_CBUF_ENTRY_ENABLE;
      sc_pipe_ctl_ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK = sc_pipe_ctl.ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK;
      ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK = sc_pipe_ctl.ESR_SC_ALLOW_ONLY_1_REQ_PER_BANK;
      sc_pipe_ctl_ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK = sc_pipe_ctl.ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK;
      ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK = sc_pipe_ctl.ESR_SC_ALLOW_ONLY_1_REQ_PER_SUB_BANK;
      sc_pipe_ctl_ESR_SC_ZERO_STATE_ENABLE = sc_pipe_ctl.ESR_SC_ZERO_STATE_ENABLE;
      ESR_SC_ZERO_STATE_ENABLE = sc_pipe_ctl.ESR_SC_ZERO_STATE_ENABLE;
      sc_pipe_ctl_ESR_SC_SCP_RBUF_ENABLE = sc_pipe_ctl.ESR_SC_SCP_RBUF_ENABLE;
      ESR_SC_SCP_RBUF_ENABLE = sc_pipe_ctl.ESR_SC_SCP_RBUF_ENABLE;
      sc_pipe_ctl_ESR_SC_L2_RBUF_ENABLE = sc_pipe_ctl.ESR_SC_L2_RBUF_ENABLE;
      ESR_SC_L2_RBUF_ENABLE = sc_pipe_ctl.ESR_SC_L2_RBUF_ENABLE;
      sc_pipe_ctl_ESR_SC_RAM_DELAY = sc_pipe_ctl.ESR_SC_RAM_DELAY;
      ESR_SC_RAM_DELAY = sc_pipe_ctl.ESR_SC_RAM_DELAY;
      sc_pipe_ctl_ESR_SC_RAM_SHUT_DOWN = sc_pipe_ctl.ESR_SC_RAM_SHUT_DOWN;
      ESR_SC_RAM_SHUT_DOWN = sc_pipe_ctl.ESR_SC_RAM_SHUT_DOWN;
      sc_pipe_ctl_ESR_SC_RAM_DEEP_SLEEP = sc_pipe_ctl.ESR_SC_RAM_DEEP_SLEEP;
      ESR_SC_RAM_DEEP_SLEEP = sc_pipe_ctl.ESR_SC_RAM_DEEP_SLEEP;
      sc_pipe_ctl_ESR_SC_IDX_COP_SM_CTL_USER_EN = sc_pipe_ctl.ESR_SC_IDX_COP_SM_CTL_USER_EN;
      ESR_SC_IDX_COP_SM_CTL_USER_EN = sc_pipe_ctl.ESR_SC_IDX_COP_SM_CTL_USER_EN;

      apb_map.add_reg(sc_pipe_ctl, `UVM_REG_ADDR_WIDTH'h00000c02, "RW", 0);
      axi_map.add_reg(sc_pipe_ctl, `UVM_REG_ADDR_WIDTH'h18000002 << 3, "RW", 0);

      sc_l2_cache_ctl = ral_reg_sc_l2_cache_ctl::type_id::create("sc_l2_cache_ctl", , get_full_name());
      sc_l2_cache_ctl.configure(this, null, "reg_sc_l2_cache_ctl");
      sc_l2_cache_ctl.build();
      sc_l2_cache_ctl_TAG_MASK = sc_l2_cache_ctl.TAG_MASK;
      sc_l2_cache_ctl_TAG_MASK_RSVD = sc_l2_cache_ctl.TAG_MASK_RSVD;
      sc_l2_cache_ctl_SET_MASK = sc_l2_cache_ctl.SET_MASK;
      sc_l2_cache_ctl_SET_MASK_RSVD = sc_l2_cache_ctl.SET_MASK_RSVD;
      sc_l2_cache_ctl_SET_SIZE = sc_l2_cache_ctl.SET_SIZE;
      sc_l2_cache_ctl_SET_SIZE_RSVD = sc_l2_cache_ctl.SET_SIZE_RSVD;
      sc_l2_cache_ctl_SET_BASE = sc_l2_cache_ctl.SET_BASE;
      sc_l2_cache_ctl_SET_BASE_RSVD = sc_l2_cache_ctl.SET_BASE_RSVD;
      apb_map.add_reg(sc_l2_cache_ctl, `UVM_REG_ADDR_WIDTH'h00000c03, "RW", 0);
      axi_map.add_reg(sc_l2_cache_ctl, `UVM_REG_ADDR_WIDTH'h18000003 << 3, "RW", 0);

      sc_l3_cache_ctl = ral_reg_sc_l3_cache_ctl::type_id::create("sc_l3_cache_ctl", , get_full_name());
      sc_l3_cache_ctl.configure(this, null, "reg_sc_l3_cache_ctl");
      sc_l3_cache_ctl.build();
      sc_l3_cache_ctl_TAG_MASK = sc_l3_cache_ctl.TAG_MASK;
      sc_l3_cache_ctl_TAG_MASK_RSVD = sc_l3_cache_ctl.TAG_MASK_RSVD;
      sc_l3_cache_ctl_SET_MASK = sc_l3_cache_ctl.SET_MASK;
      sc_l3_cache_ctl_SET_MASK_RSVD = sc_l3_cache_ctl.SET_MASK_RSVD;
      sc_l3_cache_ctl_SET_SIZE = sc_l3_cache_ctl.SET_SIZE;
      sc_l3_cache_ctl_SET_SIZE_RSVD = sc_l3_cache_ctl.SET_SIZE_RSVD;
      sc_l3_cache_ctl_SET_BASE = sc_l3_cache_ctl.SET_BASE;
      sc_l3_cache_ctl_SET_BASE_RSVD = sc_l3_cache_ctl.SET_BASE_RSVD;
      apb_map.add_reg(sc_l3_cache_ctl, `UVM_REG_ADDR_WIDTH'h00000c04, "RW", 0);
      axi_map.add_reg(sc_l3_cache_ctl, `UVM_REG_ADDR_WIDTH'h18000004 << 3, "RW", 0);

      sc_scp_cache_ctl = ral_reg_sc_scp_cache_ctl::type_id::create("sc_scp_cache_ctl", , get_full_name());
      sc_scp_cache_ctl.configure(this, null, "reg_sc_scp_cache_ctl");
      sc_scp_cache_ctl.build();
      sc_scp_cache_ctl_TAG_MASK = sc_scp_cache_ctl.TAG_MASK;
      sc_scp_cache_ctl_TAG_MASK_RSVD = sc_scp_cache_ctl.TAG_MASK_RSVD;
      sc_scp_cache_ctl_SET_MASK = sc_scp_cache_ctl.SET_MASK;
      sc_scp_cache_ctl_SET_MASK_RSVD = sc_scp_cache_ctl.SET_MASK_RSVD;
      sc_scp_cache_ctl_SET_SIZE = sc_scp_cache_ctl.SET_SIZE;
      sc_scp_cache_ctl_SET_SIZE_RSVD = sc_scp_cache_ctl.SET_SIZE_RSVD;
      sc_scp_cache_ctl_SET_BASE = sc_scp_cache_ctl.SET_BASE;
      sc_scp_cache_ctl_SET_BASE_RSVD = sc_scp_cache_ctl.SET_BASE_RSVD;
      apb_map.add_reg(sc_scp_cache_ctl, `UVM_REG_ADDR_WIDTH'h00000c05, "RW", 0);
      axi_map.add_reg(sc_scp_cache_ctl, `UVM_REG_ADDR_WIDTH'h18000005 << 3, "RW", 0);

      sc_idx_cop_sm_ctl = ral_reg_sc_idx_cop_sm_ctl::type_id::create("sc_idx_cop_sm_ctl", , get_full_name());
      sc_idx_cop_sm_ctl.configure(this, null, "esr_status.pipe_idx_cop_sm.ctl");
      sc_idx_cop_sm_ctl.build();
      sc_idx_cop_sm_ctl_GO = sc_idx_cop_sm_ctl.GO;
      GO = sc_idx_cop_sm_ctl.GO;
      sc_idx_cop_sm_ctl_ABORT = sc_idx_cop_sm_ctl.ABORT;
      ABORT = sc_idx_cop_sm_ctl.ABORT;
      sc_idx_cop_sm_ctl_OPCODE = sc_idx_cop_sm_ctl.OPCODE;
      OPCODE = sc_idx_cop_sm_ctl.OPCODE;
      sc_idx_cop_sm_ctl_ECC_WR_EN = sc_idx_cop_sm_ctl.ECC_WR_EN;
      ECC_WR_EN = sc_idx_cop_sm_ctl.ECC_WR_EN;
      sc_idx_cop_sm_ctl_LOGICAL_RAM = sc_idx_cop_sm_ctl.LOGICAL_RAM;
      LOGICAL_RAM = sc_idx_cop_sm_ctl.LOGICAL_RAM;
      sc_idx_cop_sm_ctl_IDX_COP_SM_STATE = sc_idx_cop_sm_ctl.IDX_COP_SM_STATE;
      IDX_COP_SM_STATE = sc_idx_cop_sm_ctl.IDX_COP_SM_STATE;
      apb_map.add_reg(sc_idx_cop_sm_ctl, `UVM_REG_ADDR_WIDTH'h00000c06, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_ctl, `UVM_REG_ADDR_WIDTH'h18000006 << 3, "RW", 0);

      sc_idx_cop_sm_ctl_user = ral_reg_sc_idx_cop_sm_ctl_user::type_id::create("sc_idx_cop_sm_ctl_user", , get_full_name());
      sc_idx_cop_sm_ctl_user.configure(this, null, "esr_status.pipe_idx_cop_sm.ctl_user");
      sc_idx_cop_sm_ctl_user.build();
      sc_idx_cop_sm_ctl_user_GO = sc_idx_cop_sm_ctl_user.GO;
      //GO = sc_idx_cop_sm_ctl_user.GO;
      sc_idx_cop_sm_ctl_user_ABORT = sc_idx_cop_sm_ctl_user.ABORT;
      //ABORT = sc_idx_cop_sm_ctl_user.ABORT;
      sc_idx_cop_sm_ctl_user_OPCODE = sc_idx_cop_sm_ctl_user.OPCODE;
      //OPCODE = sc_idx_cop_sm_ctl_user.OPCODE;
      sc_idx_cop_sm_ctl_user_ECC_WR_EN = sc_idx_cop_sm_ctl_user.ECC_WR_EN;
      //ECC_WR_EN = sc_idx_cop_sm_ctl_user.ECC_WR_EN;
      sc_idx_cop_sm_ctl_user_LOGICAL_RAM = sc_idx_cop_sm_ctl_user.LOGICAL_RAM;
      //LOGICAL_RAM = sc_idx_cop_sm_ctl_user.LOGICAL_RAM;
      sc_idx_cop_sm_ctl_user_IDX_COP_SM_STATE = sc_idx_cop_sm_ctl_user.IDX_COP_SM_STATE;
      //IDX_COP_SM_STATE = sc_idx_cop_sm_ctl_user.IDX_COP_SM_STATE;
      apb_map.add_reg(sc_idx_cop_sm_ctl_user, `UVM_REG_ADDR_WIDTH'h00000020, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_ctl_user, `UVM_REG_ADDR_WIDTH'h00000020 << 3, "RW", 0);

      //Special stuff for aliasing between user and non-user versions of idx_cop_sm_ctl
      begin
         alias_reg_ctl_user alias_ctl_user;
         alias_ctl_user = alias_reg_ctl_user::type_id::create("alias_ctl_user",,get_full_name());
         alias_ctl_user.configure(sc_idx_cop_sm_ctl,sc_idx_cop_sm_ctl_user);
      end

      sc_idx_cop_sm_physical_index = ral_reg_sc_idx_cop_sm_physical_index::type_id::create("sc_idx_cop_sm_physical_index", , get_full_name());
      sc_idx_cop_sm_physical_index.configure(this, null, "esr_status.pipe_idx_cop_sm.physical_index");
      sc_idx_cop_sm_physical_index.build();
      sc_idx_cop_sm_physical_index_QW = sc_idx_cop_sm_physical_index.QW;
      QW = sc_idx_cop_sm_physical_index.QW;
      sc_idx_cop_sm_physical_index_SUB_BANK = sc_idx_cop_sm_physical_index.SUB_BANK;
      SUB_BANK = sc_idx_cop_sm_physical_index.SUB_BANK;
      sc_idx_cop_sm_physical_index_WAY = sc_idx_cop_sm_physical_index.WAY;
      WAY = sc_idx_cop_sm_physical_index.WAY;
      sc_idx_cop_sm_physical_index_SET = sc_idx_cop_sm_physical_index.SET;
      SET = sc_idx_cop_sm_physical_index.SET;
      apb_map.add_reg(sc_idx_cop_sm_physical_index, `UVM_REG_ADDR_WIDTH'h00000c07, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_physical_index, `UVM_REG_ADDR_WIDTH'h18000007 << 3, "RW", 0);

      sc_idx_cop_sm_data0 = ral_reg_sc_idx_cop_sm_data0::type_id::create("sc_idx_cop_sm_data0", , get_full_name());
      sc_idx_cop_sm_data0.configure(this, null, "esr_status.pipe_idx_cop_sm.data0");
      sc_idx_cop_sm_data0.build();
      sc_idx_cop_sm_data0_SC_IDX_COP_SM_DATA0 = sc_idx_cop_sm_data0.SC_IDX_COP_SM_DATA0;
      SC_IDX_COP_SM_DATA0 = sc_idx_cop_sm_data0.SC_IDX_COP_SM_DATA0;
      apb_map.add_reg(sc_idx_cop_sm_data0, `UVM_REG_ADDR_WIDTH'h00000c08, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_data0, `UVM_REG_ADDR_WIDTH'h18000008 << 3, "RW", 0);

      sc_idx_cop_sm_data1 = ral_reg_sc_idx_cop_sm_data1::type_id::create("sc_idx_cop_sm_data1", , get_full_name());
      sc_idx_cop_sm_data1.configure(this, null, "esr_status.pipe_idx_cop_sm.data1");
      sc_idx_cop_sm_data1.build();
      sc_idx_cop_sm_data1_SC_IDX_COP_SM_DATA1 = sc_idx_cop_sm_data1.SC_IDX_COP_SM_DATA1;
      SC_IDX_COP_SM_DATA1 = sc_idx_cop_sm_data1.SC_IDX_COP_SM_DATA1;
      apb_map.add_reg(sc_idx_cop_sm_data1, `UVM_REG_ADDR_WIDTH'h00000c09, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_data1, `UVM_REG_ADDR_WIDTH'h18000009 << 3, "RW", 0);

      sc_idx_cop_sm_ecc = ral_reg_sc_idx_cop_sm_ecc::type_id::create("sc_idx_cop_sm_ecc", , get_full_name());
      sc_idx_cop_sm_ecc.configure(this, null, "esr_status.pipe_idx_cop_sm.ecc");
      sc_idx_cop_sm_ecc.build();
      sc_idx_cop_sm_ecc_SC_IDX_COP_SM_ECC = sc_idx_cop_sm_ecc.SC_IDX_COP_SM_ECC;
      SC_IDX_COP_SM_ECC = sc_idx_cop_sm_ecc.SC_IDX_COP_SM_ECC;
      apb_map.add_reg(sc_idx_cop_sm_ecc, `UVM_REG_ADDR_WIDTH'h00000c0a, "RW", 0);
      axi_map.add_reg(sc_idx_cop_sm_ecc, `UVM_REG_ADDR_WIDTH'h1800000a << 3, "RW", 0);

      sc_err_log_ctl = ral_reg_sc_err_log_ctl::type_id::create("sc_err_log_ctl", , get_full_name());
      sc_err_log_ctl.configure(this, null, "reg_sc_err_log_ctl");
      sc_err_log_ctl.build();
      sc_err_log_ctl_ESR_SC_ERR_INTERRUPT_ENABLE = sc_err_log_ctl.ESR_SC_ERR_INTERRUPT_ENABLE;
      ESR_SC_ERR_INTERRUPT_ENABLE = sc_err_log_ctl.ESR_SC_ERR_INTERRUPT_ENABLE;
      sc_err_log_ctl_ESR_SC_ERR_RSP_ENABLE = sc_err_log_ctl.ESR_SC_ERR_RSP_ENABLE;
      ESR_SC_ERR_RSP_ENABLE = sc_err_log_ctl.ESR_SC_ERR_RSP_ENABLE;
      apb_map.add_reg(sc_err_log_ctl, `UVM_REG_ADDR_WIDTH'h00000c0b, "RW", 0);
      axi_map.add_reg(sc_err_log_ctl, `UVM_REG_ADDR_WIDTH'h1800000b << 3, "RW", 0);

      sc_err_log_info = ral_reg_sc_err_log_info::type_id::create("sc_err_log_info", , get_full_name());
      sc_err_log_info.configure(this, null, "esr_status.err_log.info");
      sc_err_log_info.build();
      sc_err_log_info_SC_ERR_LOG_INFO = sc_err_log_info.SC_ERR_LOG_INFO;
      SC_ERR_LOG_INFO = sc_err_log_info.SC_ERR_LOG_INFO;
      apb_map.add_reg(sc_err_log_info, `UVM_REG_ADDR_WIDTH'h00000c0c, "RW", 0);
      axi_map.add_reg(sc_err_log_info, `UVM_REG_ADDR_WIDTH'h1800000c << 3, "RW", 0);

      sc_err_log_address = ral_reg_sc_err_log_address::type_id::create("sc_err_log_address", , get_full_name());
      sc_err_log_address.configure(this, null, "esr_status.err_log.address");
      sc_err_log_address.build();
      sc_err_log_address_SC_ERR_LOG_ADDRESS = sc_err_log_address.SC_ERR_LOG_ADDRESS;
      SC_ERR_LOG_ADDRESS = sc_err_log_address.SC_ERR_LOG_ADDRESS;
      apb_map.add_reg(sc_err_log_address, `UVM_REG_ADDR_WIDTH'h00000c0d, "RO", 0);
      axi_map.add_reg(sc_err_log_address, `UVM_REG_ADDR_WIDTH'h1800000d << 3, "RO", 0);

      sc_sbe_dbe_counts = ral_reg_sc_sbe_dbe_counts::type_id::create("sc_sbe_dbe_counts", , get_full_name());
      sc_sbe_dbe_counts.configure(this, null, "esr_status.sbe_dbe_counts");
      sc_sbe_dbe_counts.build();
      sc_sbe_dbe_counts_SC_SBE_DBE_COUNTS = sc_sbe_dbe_counts.SC_SBE_DBE_COUNTS;
      SC_SBE_DBE_COUNTS = sc_sbe_dbe_counts.SC_SBE_DBE_COUNTS;
      apb_map.add_reg(sc_sbe_dbe_counts, `UVM_REG_ADDR_WIDTH'h00000c0e, "RW", 0);
      axi_map.add_reg(sc_sbe_dbe_counts, `UVM_REG_ADDR_WIDTH'h1800000e << 3, "RW", 0);

      sc_reqq_debug_ctl = ral_reg_sc_reqq_debug_ctl::type_id::create("sc_reqq_debug_ctl", , get_full_name());
      sc_reqq_debug_ctl.configure(this, null, "");
      sc_reqq_debug_ctl.build();
      sc_reqq_debug_ctl_SC_REQQ_DEBUG_CTL = sc_reqq_debug_ctl.SC_REQQ_DEBUG_CTL;
      SC_REQQ_DEBUG_CTL = sc_reqq_debug_ctl.SC_REQQ_DEBUG_CTL;
      apb_map.add_reg(sc_reqq_debug_ctl, `UVM_REG_ADDR_WIDTH'h00000c0f, "WO", 0);
      axi_map.add_reg(sc_reqq_debug_ctl, `UVM_REG_ADDR_WIDTH'h1800000f << 3, "WO", 0);

      sc_reqq_debug0 = ral_reg_sc_reqq_debug0::type_id::create("sc_reqq_debug0", , get_full_name());
      sc_reqq_debug0.configure(this, null, "esr_status.err_log.reqq_debug0");
      sc_reqq_debug0.build();
      sc_reqq_debug0_SC_REQQ_DEBUG0 = sc_reqq_debug0.SC_REQQ_DEBUG0;
      SC_REQQ_DEBUG0 = sc_reqq_debug0.SC_REQQ_DEBUG0;
      apb_map.add_reg(sc_reqq_debug0, `UVM_REG_ADDR_WIDTH'h00000c10, "RO", 0);
      axi_map.add_reg(sc_reqq_debug0, `UVM_REG_ADDR_WIDTH'h18000010 << 3, "RO", 0);

      sc_reqq_debug1 = ral_reg_sc_reqq_debug1::type_id::create("sc_reqq_debug1", , get_full_name());
      sc_reqq_debug1.configure(this, null, "esr_status.err_log.reqq_debug1");
      sc_reqq_debug1.build();
      sc_reqq_debug1_SC_REQQ_DEBUG1 = sc_reqq_debug1.SC_REQQ_DEBUG1;
      SC_REQQ_DEBUG1 = sc_reqq_debug1.SC_REQQ_DEBUG1;
      apb_map.add_reg(sc_reqq_debug1, `UVM_REG_ADDR_WIDTH'h00000c11, "RO", 0);
      axi_map.add_reg(sc_reqq_debug1, `UVM_REG_ADDR_WIDTH'h18000011 << 3, "RO", 0);

      sc_reqq_debug2 = ral_reg_sc_reqq_debug2::type_id::create("sc_reqq_debug2", , get_full_name());
      sc_reqq_debug2.configure(this, null, "esr_status.err_log.reqq_debug2");
      sc_reqq_debug2.build();
      sc_reqq_debug2_SC_REQQ_DEBUG2 = sc_reqq_debug2.SC_REQQ_DEBUG2;
      SC_REQQ_DEBUG2 = sc_reqq_debug2.SC_REQQ_DEBUG2;
      apb_map.add_reg(sc_reqq_debug2, `UVM_REG_ADDR_WIDTH'h00000c12, "RO", 0);
      axi_map.add_reg(sc_reqq_debug2, `UVM_REG_ADDR_WIDTH'h18000012 << 3, "RO", 0);

      sc_reqq_debug3 = ral_reg_sc_reqq_debug3::type_id::create("sc_reqq_debug3", , get_full_name());
      sc_reqq_debug3.configure(this, null, "esr_status.err_log.reqq_debug3");
      sc_reqq_debug3.build();
      sc_reqq_debug3_SC_REQQ_DEBUG3 = sc_reqq_debug3.SC_REQQ_DEBUG3;
      SC_REQQ_DEBUG3 = sc_reqq_debug3.SC_REQQ_DEBUG3;
      apb_map.add_reg(sc_reqq_debug3, `UVM_REG_ADDR_WIDTH'h00000c13, "RO", 0);
      axi_map.add_reg(sc_reqq_debug3, `UVM_REG_ADDR_WIDTH'h18000013 << 3, "RO", 0);

      sc_eco_ctl = ral_reg_sc_eco_ctl::type_id::create("sc_eco_ctl", , get_full_name());
      sc_eco_ctl.configure(this, null, "reg_sc_eco_ctl");
      sc_eco_ctl.build();
      sc_eco_ctl_ESR_SC_ECO_CTL = sc_eco_ctl.ESR_SC_ECO_CTL;
      ESR_SC_ECO_CTL = sc_eco_ctl.ESR_SC_ECO_CTL;
      apb_map.add_reg(sc_eco_ctl, `UVM_REG_ADDR_WIDTH'h00000c14, "RW", 0);
      axi_map.add_reg(sc_eco_ctl, `UVM_REG_ADDR_WIDTH'h18000014 << 3, "RW", 0);

      sc_perfmon_ctl_status = ral_reg_sc_perfmon_ctl_status::type_id::create("sc_perfmon_ctl_status", , get_full_name());
      sc_perfmon_ctl_status.configure(this, null, "esr_status.perfmon.ctl_status");
      sc_perfmon_ctl_status.build();
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_START;
      ESR_PERFMON_CYCLE_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_START;
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_RESET;
      ESR_PERFMON_CYCLE_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_RESET;
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_OVERFLOW;
      ESR_PERFMON_CYCLE_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_INTERRUPT;
      ESR_PERFMON_CYCLE_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_INTERRUPT;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_START;
      ESR_PERFMON_P0_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_START;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_RESET;
      ESR_PERFMON_P0_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_RESET;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_OVERFLOW;
      ESR_PERFMON_P0_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_INTERRUPT;
      ESR_PERFMON_P0_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_INTERRUPT;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_EVENT = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_EVENT;
      ESR_PERFMON_P0_CNTR_EVENT = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_EVENT;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_MODE = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_MODE;
      ESR_PERFMON_P0_CNTR_MODE = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_MODE;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_START;
      ESR_PERFMON_P1_CNTR_START = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_START;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_RESET;
      ESR_PERFMON_P1_CNTR_RESET = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_RESET;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_OVERFLOW;
      ESR_PERFMON_P1_CNTR_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_INTERRUPT;
      ESR_PERFMON_P1_CNTR_INTERRUPT = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_INTERRUPT;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_EVENT = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_EVENT;
      ESR_PERFMON_P1_CNTR_EVENT = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_EVENT;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_MODE = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_MODE;
      ESR_PERFMON_P1_CNTR_MODE = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_MODE;
      sc_perfmon_ctl_status_ESR_PERFMON_ANY_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_ANY_OVERFLOW;
      ESR_PERFMON_ANY_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_ANY_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE;
      ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_STATUS_ACTIVE;
      sc_perfmon_ctl_status_ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW;
      ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_STATUS_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_STATUS_ACTIVE;
      ESR_PERFMON_P0_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_STATUS_ACTIVE;
      sc_perfmon_ctl_status_ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW;
      ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_STATUS_OVERFLOW;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_STATUS_ACTIVE;
      ESR_PERFMON_P1_CNTR_STATUS_ACTIVE = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_STATUS_ACTIVE;
      sc_perfmon_ctl_status_ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW;
      ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW = sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_STATUS_OVERFLOW;
      apb_map.add_reg(sc_perfmon_ctl_status, `UVM_REG_ADDR_WIDTH'h00000c17, "RW", 0);
      axi_map.add_reg(sc_perfmon_ctl_status, `UVM_REG_ADDR_WIDTH'h18000017 << 3, "RW", 0);

      sc_perfmon_cyc_cntr = ral_reg_sc_perfmon_cyc_cntr::type_id::create("sc_perfmon_cyc_cntr", , get_full_name());
      sc_perfmon_cyc_cntr.configure(this, null, "esr_status.perfmon.cyc_cntr");
      sc_perfmon_cyc_cntr.build();
      sc_perfmon_cyc_cntr_SC_PERFMON_CYC_CNTR = sc_perfmon_cyc_cntr.SC_PERFMON_CYC_CNTR;
      SC_PERFMON_CYC_CNTR = sc_perfmon_cyc_cntr.SC_PERFMON_CYC_CNTR;
      apb_map.add_reg(sc_perfmon_cyc_cntr, `UVM_REG_ADDR_WIDTH'h00000c18, "RW", 0);
      axi_map.add_reg(sc_perfmon_cyc_cntr, `UVM_REG_ADDR_WIDTH'h18000018 << 3, "RW", 0);

      sc_perfmon_p0_cntr = ral_reg_sc_perfmon_p0_cntr::type_id::create("sc_perfmon_p0_cntr", , get_full_name());
      sc_perfmon_p0_cntr.configure(this, null, "esr_status.perfmon.p0_cntr");
      sc_perfmon_p0_cntr.build();
      sc_perfmon_p0_cntr_SC_PERFMON_P0_CNTR = sc_perfmon_p0_cntr.SC_PERFMON_P0_CNTR;
      SC_PERFMON_P0_CNTR = sc_perfmon_p0_cntr.SC_PERFMON_P0_CNTR;
      apb_map.add_reg(sc_perfmon_p0_cntr, `UVM_REG_ADDR_WIDTH'h00000c19, "RW", 0);
      axi_map.add_reg(sc_perfmon_p0_cntr, `UVM_REG_ADDR_WIDTH'h18000019 << 3, "RW", 0);

      sc_perfmon_p1_cntr = ral_reg_sc_perfmon_p1_cntr::type_id::create("sc_perfmon_p1_cntr", , get_full_name());
      sc_perfmon_p1_cntr.configure(this, null, "esr_status.perfmon.p1_cntr");
      sc_perfmon_p1_cntr.build();
      sc_perfmon_p1_cntr_SC_PERFMON_P1_CNTR = sc_perfmon_p1_cntr.SC_PERFMON_P1_CNTR;
      SC_PERFMON_P1_CNTR = sc_perfmon_p1_cntr.SC_PERFMON_P1_CNTR;
      apb_map.add_reg(sc_perfmon_p1_cntr, `UVM_REG_ADDR_WIDTH'h00000c1a, "RW", 0);
      axi_map.add_reg(sc_perfmon_p1_cntr, `UVM_REG_ADDR_WIDTH'h1800001a << 3, "RW", 0);

      sc_perfmon_p0_qual = ral_reg_sc_perfmon_p0_qual::type_id::create("sc_perfmon_p0_qual", , get_full_name());
      sc_perfmon_p0_qual.configure(this, null, "esr_status.perfmon.p0_qual");
      sc_perfmon_p0_qual.build();
      sc_perfmon_p0_qual_SC_PERFMON_P0_QUAL = sc_perfmon_p0_qual.SC_PERFMON_P0_QUAL;
      SC_PERFMON_P0_QUAL = sc_perfmon_p0_qual.SC_PERFMON_P0_QUAL;
      apb_map.add_reg(sc_perfmon_p0_qual, `UVM_REG_ADDR_WIDTH'h00000c1b, "RW", 0);
      axi_map.add_reg(sc_perfmon_p0_qual, `UVM_REG_ADDR_WIDTH'h1800001b << 3, "RW", 0);

      sc_perfmon_p1_qual = ral_reg_sc_perfmon_p1_qual::type_id::create("sc_perfmon_p1_qual", , get_full_name());
      sc_perfmon_p1_qual.configure(this, null, "esr_status.perfmon.p1_qual");
      sc_perfmon_p1_qual.build();
      sc_perfmon_p1_qual_SC_PERFMON_P1_QUAL = sc_perfmon_p1_qual.SC_PERFMON_P1_QUAL;
      SC_PERFMON_P1_QUAL = sc_perfmon_p1_qual.SC_PERFMON_P1_QUAL;
      apb_map.add_reg(sc_perfmon_p1_qual, `UVM_REG_ADDR_WIDTH'h00000c1c, "RW", 0);
      axi_map.add_reg(sc_perfmon_p1_qual, `UVM_REG_ADDR_WIDTH'h1800001c << 3, "RW", 0);

      sc_trace_address_enable = ral_reg_sc_trace_address_enable::type_id::create("sc_trace_address_enable", , get_full_name());
      sc_trace_address_enable.configure(this, null, "reg_sc_trace_address_enable");
      sc_trace_address_enable.build();
      sc_trace_address_enable_ESR_SC_TRACE_ADDRESS_ENABLE = sc_trace_address_enable.ESR_SC_TRACE_ADDRESS_ENABLE;
      ESR_SC_TRACE_ADDRESS_ENABLE = sc_trace_address_enable.ESR_SC_TRACE_ADDRESS_ENABLE;
      apb_map.add_reg(sc_trace_address_enable, `UVM_REG_ADDR_WIDTH'h00000bf0, "RW", 0);
      axi_map.add_reg(sc_trace_address_enable, `UVM_REG_ADDR_WIDTH'h100003f0 << 3, "RW", 0);

      sc_trace_address_value = ral_reg_sc_trace_address_value::type_id::create("sc_trace_address_value", , get_full_name());
      sc_trace_address_value.configure(this, null, "reg_sc_trace_address_value");
      sc_trace_address_value.build();
      sc_trace_address_value_ESR_SC_TRACE_ADDRESS_VALUE = sc_trace_address_value.ESR_SC_TRACE_ADDRESS_VALUE;
      ESR_SC_TRACE_ADDRESS_VALUE = sc_trace_address_value.ESR_SC_TRACE_ADDRESS_VALUE;
      apb_map.add_reg(sc_trace_address_value, `UVM_REG_ADDR_WIDTH'h00000BF1, "RW", 0);
      axi_map.add_reg(sc_trace_address_value, `UVM_REG_ADDR_WIDTH'h100003f1 << 3, "RW", 0);

      sc_trace_ctl = ral_reg_sc_trace_ctl::type_id::create("sc_trace_ctl", , get_full_name());
      sc_trace_ctl.configure(this, null, "reg_sc_trace_ctl");
      sc_trace_ctl.build();
      sc_trace_ctl_ESR_SC_TRACE_TYPE_HOT_ENABLE = sc_trace_ctl.ESR_SC_TRACE_TYPE_HOT_ENABLE;
      ESR_SC_TRACE_TYPE_HOT_ENABLE = sc_trace_ctl.ESR_SC_TRACE_TYPE_HOT_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_FSM_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_FSM_ENABLE;
      ESR_SC_TRACE_FILTER_FSM_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_FSM_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_L3_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_L3_ENABLE;
      ESR_SC_TRACE_FILTER_L3_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_L3_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_L2_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_L2_ENABLE;
      ESR_SC_TRACE_FILTER_L2_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_L2_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_SOURCE = sc_trace_ctl.ESR_SC_TRACE_FILTER_SOURCE;
      ESR_SC_TRACE_FILTER_SOURCE = sc_trace_ctl.ESR_SC_TRACE_FILTER_SOURCE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_SOURCE_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_SOURCE_ENABLE;
      ESR_SC_TRACE_FILTER_SOURCE_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_SOURCE_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_PORT = sc_trace_ctl.ESR_SC_TRACE_FILTER_PORT;
      ESR_SC_TRACE_FILTER_PORT = sc_trace_ctl.ESR_SC_TRACE_FILTER_PORT;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_PORT_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_PORT_ENABLE;
      ESR_SC_TRACE_FILTER_PORT_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_PORT_ENABLE;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_REQQ_ID = sc_trace_ctl.ESR_SC_TRACE_FILTER_REQQ_ID;
      ESR_SC_TRACE_FILTER_REQQ_ID = sc_trace_ctl.ESR_SC_TRACE_FILTER_REQQ_ID;
      sc_trace_ctl_ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE;
      ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE = sc_trace_ctl.ESR_SC_TRACE_FILTER_REQQ_ID_ENABLE;
      apb_map.add_reg(sc_trace_ctl, `UVM_REG_ADDR_WIDTH'h00000bf2, "RW", 0);
      axi_map.add_reg(sc_trace_ctl, `UVM_REG_ADDR_WIDTH'h100003f2 << 3, "RW", 0);


   endfunction : build

   `uvm_object_utils(ral_block_sc_bank_regs)

endclass : ral_block_sc_bank_regs
