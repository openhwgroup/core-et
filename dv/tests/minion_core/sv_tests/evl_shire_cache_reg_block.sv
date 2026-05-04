//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_reg_block
//
class evl_shire_cache_reg_block extends ral_block_sc_bank_regs;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_shire_cache_reg_block)

   evl_shire_id_t m_shire_id = ~evl_shire_id_t'(0);
   evl_agent_id_t m_agent_id = ~evl_agent_id_t'(0);

   evl_dword_t    m_reg_field_masks[string];
   evl_dword_t    m_reg_bits_no_test[string];

   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   // This function is used to set the shire ID for this component.
   //
   // Input Variables:
   //
   // shire_id_in: This is the shire ID to set for this component.
   //
   virtual function void set_shire_id(input evl_shire_id_t shire_id_in);
      m_shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   // This function is used to get the shire ID for this component.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the shire ID of this component.
   //
   virtual function evl_shire_id_t get_shire_id();
      //return m_shire_id;
      return m_global_attrs.get_current_shire_id(m_shire_id);
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_id
   //
   // This function is used to set the virtual agent ID for this component.
   //
   // Input Variables:
   //
   // agent_id_in: This is the virtual agent ID to use for this component.
   //
   virtual function void set_agent_id(input evl_agent_id_t agent_id_in);
      m_agent_id = agent_id_in;
   endfunction : set_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_id
   //
   // This function is used to get the virtual agent ID for this component.
   //
   // Return Value:
   //
   // evl_agent_id_t: This is the virtual agent ID of this component.
   //
   virtual function evl_agent_id_t get_agent_id();
      return m_agent_id;
   endfunction : get_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   // This function calls the uvm_reg_block build() function and is also used
   // to override any reset/default values.  It can also disable error checks
   // when registers are read (by using plusarg).
   //
   // 
   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      evl_dword_t  l2_tag_mask;
      evl_dword_t  l2_set_mask;
      int unsigned l2_set_size;
      int unsigned l2_set_base;
      evl_dword_t  l3_tag_mask;
      evl_dword_t  l3_set_mask;
      int unsigned l3_set_size;
      int unsigned l3_set_base;
      evl_dword_t  scp_tag_mask;
      evl_dword_t  scp_set_mask;
      int unsigned scp_set_size;
      int unsigned scp_set_base;

      super.build();

      set_l3_shire_swizzle_reset_val();

      set_l3_reqq_entries_reset_val();

      if (`SC_CACHE_SIZE_IN_MB == 1) begin
         compute_cache_ctl_reset_val(12, 16,  l2_tag_mask,  l2_set_mask,  l2_set_size,  l2_set_base);
         compute_cache_ctl_reset_val(28,  4,  l3_tag_mask,  l3_set_mask,  l3_set_size,  l3_set_base);
         compute_cache_ctl_reset_val( 0, 12, scp_tag_mask, scp_set_mask, scp_set_size, scp_set_base);
      end
      else if ((`SC_CACHE_SIZE_IN_MB == 2) || (`SC_CACHE_SIZE_IN_MB == 3)) begin
         compute_cache_ctl_reset_val(16,  8,  l2_tag_mask,  l2_set_mask,  l2_set_size,  l2_set_base);
         compute_cache_ctl_reset_val(24,  8,  l3_tag_mask,  l3_set_mask,  l3_set_size,  l3_set_base);
         compute_cache_ctl_reset_val( 0, 16, scp_tag_mask, scp_set_mask, scp_set_size, scp_set_base);
      end
      else begin
         compute_cache_ctl_reset_val(20,  4,  l2_tag_mask,  l2_set_mask,  l2_set_size,  l2_set_base);
         compute_cache_ctl_reset_val(24,  8,  l3_tag_mask,  l3_set_mask,  l3_set_size,  l3_set_base);
         compute_cache_ctl_reset_val( 0, 20, scp_tag_mask, scp_set_mask, scp_set_size, scp_set_base);
      end
      sc_l2_cache_ctl.TAG_MASK.set_reset(l2_tag_mask);
      sc_l2_cache_ctl.SET_MASK.set_reset(l2_set_mask);
      sc_l2_cache_ctl.SET_SIZE.set_reset(l2_set_size);
      sc_l2_cache_ctl.SET_BASE.set_reset(l2_set_base);
      sc_l3_cache_ctl.TAG_MASK.set_reset(l3_tag_mask);
      sc_l3_cache_ctl.SET_MASK.set_reset(l3_set_mask);
      sc_l3_cache_ctl.SET_SIZE.set_reset(l3_set_size);
      sc_l3_cache_ctl.SET_BASE.set_reset(l3_set_base);
      sc_scp_cache_ctl.TAG_MASK.set_reset(scp_tag_mask);
      sc_scp_cache_ctl.SET_MASK.set_reset(scp_set_mask);
      sc_scp_cache_ctl.SET_SIZE.set_reset(scp_set_size);
      sc_scp_cache_ctl.SET_BASE.set_reset(scp_set_base);

      if (`et_test_plusargs("sc_regs_disable_compare")) begin
         disable_field_checks();
      end

      setup_reg_field_masks();
      setup_bit_test_masks();

   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_field_checks
   //
   // This function loops through every register field in the reg block and
   // disables the comparison mechanism between the expected value in the
   // model against the actual register read data
   // 
   function void disable_field_checks();
      uvm_reg_field reg_fields[$];

      get_fields(reg_fields);
      foreach (reg_fields[jj]) begin
         reg_fields[jj].set_compare(UVM_NO_CHECK);
      end
   endfunction : disable_field_checks


   //-----------------------------------------------------------------------------------------------
   //
   // Function: adjust_field_access
   //
   // This function loops through every field in the block and adjusts the
   // access permission to the given setting
   // 
   // Reason: the register block is defined based on the specification of
   //         shire cache bank registers.  I.e. fields are RW, WC, etc. for
   //         checking of register settings.  If we are using the reg_block
   //         for test-writing purposes we want to be able to write & read
   //         fields freely (i.e. no write-to-clear or read-0)
   //
   //         The uvm_reg_field class checks to see if a legal access policy
   // 
   function void adjust_field_access(input string access);
      uvm_reg_field reg_fields[$];

      get_fields(reg_fields);

      foreach (reg_fields[jj]) begin
         reg_fields[jj].set_access(access);
      end
   endfunction : adjust_field_access


   //-----------------------------------------------------------------------------------------------
   //
   // Function: adjust_field_offsets
   //
   // This function loops through every register in the block and adjusts the
   // address offset value of each register by the specified amount
   // 
   // Reason: the register block is defined based on the specification of
   //         shire cache bank registers.  I.e. registers are addressed 0, 1, 2,
   //         3, etc.   If using the reg block to address registers elsewhere,
   //         (e.g. from a test using the axi agent) then the address offsets 
   //         need to be byte-addressed e.g. 0, 8, 10, etc.
   // 
   function void adjust_field_offsets(input int unsigned offset_val_in);
      uvm_reg regs[$];

      get_registers(regs);

      // work through the addresses backwards to avoid overlap warnings!
      for (int jj = (regs.size() - 1); jj >= 0; jj--) begin
         int offset;
         offset = regs[jj].get_offset();
         regs[jj].set_offset(get_default_map(), offset << offset_val_in);
      end
   endfunction : adjust_field_offsets


   //-----------------------------------------------------------------------------------------------
   //
   // Function: setup_reg_field_masks
   //
   // For some reason the uvm_reg class doesn't create a user-accessible mask
   // for the given fields in the register.  So let's make one here
   //
   function void setup_reg_field_masks();
      uvm_reg regs[$];

      get_registers(regs);

      foreach (regs[ii]) begin
         uvm_reg_field fields[$];
         evl_dword_t reg_mask;

         fields = {};
         regs[ii].get_fields(fields);

         foreach (fields[jj]) begin
            evl_dword_t field_mask = ((1 << fields[jj].get_n_bits) - 1);
            reg_mask |= (field_mask << fields[jj].get_lsb_pos());
         end

         m_reg_field_masks[regs[ii].get_name()] = reg_mask;
      end
   endfunction : setup_reg_field_masks


   //-----------------------------------------------------------------------------------------------
   //
   // Function: setup_bit_test_masks
   //
   // this function isn't intended to be used as an all-encompassing mask for
   // which fields are testable.  But intended to be used as a supplement to
   // a reg field mask for manually adding in bits to skip testing of.
   // For example: some fields physically change size based on build defines,
   // but the register model is created based on the largest size that can be built.
   //
   function void setup_bit_test_masks();
      evl_dword_t orig_mask;
      evl_dword_t new_mask;

      // Don't write the bit that starts the cache-op state machine
      m_reg_bits_no_test["sc_idx_cop_sm_ctl"] |= (1'b1 << sc_idx_cop_sm_ctl.GO.get_lsb_pos());
      m_reg_bits_no_test["sc_idx_cop_sm_ctl_user"] |= (1'b1 << sc_idx_cop_sm_ctl_user.GO.get_lsb_pos());

      // This OPCODE field has special usage
      new_mask = (1'b1 << sc_idx_cop_sm_ctl_user.OPCODE.get_n_bits()) - 1;
      new_mask <<= sc_idx_cop_sm_ctl_user.OPCODE.get_lsb_pos();
      m_reg_bits_no_test["sc_idx_cop_sm_ctl_user"] |= new_mask;

      // Don't write the bit that starts the performance counters
      m_reg_bits_no_test["sc_perfmon_ctl_status"] |= (1'b1 << sc_perfmon_ctl_status.ESR_PERFMON_CYCLE_CNTR_START.get_lsb_pos());
      m_reg_bits_no_test["sc_perfmon_ctl_status"] |= (1'b1 << sc_perfmon_ctl_status.ESR_PERFMON_P0_CNTR_START.get_lsb_pos());
      m_reg_bits_no_test["sc_perfmon_ctl_status"] |= (1'b1 << sc_perfmon_ctl_status.ESR_PERFMON_P1_CNTR_START.get_lsb_pos());

      // this field can change width based on build setting
      orig_mask = (1'b1 << sc_idx_cop_sm_physical_index.SUB_BANK.get_n_bits()) - 1;
      new_mask  = (1'b1 << `SC_SUB_BANK_ID_SIZE) - 1;
      new_mask ^= orig_mask;
      new_mask <<= sc_idx_cop_sm_physical_index.SUB_BANK.get_lsb_pos();
      m_reg_bits_no_test["sc_idx_cop_sm_physical_index"] |= new_mask;

      // this field can change width based on build setting
      orig_mask = (1'b1 << sc_idx_cop_sm_physical_index.SET.get_n_bits()) - 1;
      new_mask  = (1'b1 << `SC_SET_ID_SIZE) - 1;
      new_mask ^= orig_mask;
      new_mask <<= sc_idx_cop_sm_physical_index.SET.get_lsb_pos();
      m_reg_bits_no_test["sc_idx_cop_sm_physical_index"] |= new_mask;
   endfunction : setup_bit_test_masks


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reg_mask
   //
   // Returns the field mask for the given register
   //
   virtual function evl_dword_t get_reg_mask(input string name);
      if (m_reg_field_masks.exists(name)) begin
         return m_reg_field_masks[name];
      end
      return evl_dword_t'(0);
   endfunction : get_reg_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reg_test_mask
   //
   // Returns the test mask for the given register.  
   // For example can be used in a test to determine if there are any bits in
   // the register that should not be written
   //
   virtual function evl_dword_t get_reg_test_mask(input string name);
      if (m_reg_bits_no_test.exists(name)) begin
         return m_reg_bits_no_test[name];
      end
      return evl_dword_t'(0);
   endfunction : get_reg_test_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_shire_swizzle_reset_val
   //
   function void set_l3_shire_swizzle_reset_val();
      int unsigned l3_shire_id_size;
      int unsigned bank_id_size;
      int unsigned sub_bank_id_size;
      int unsigned shire_base_default;
      int unsigned bank_base_default;
      int unsigned sub_bank_base_default;

      l3_shire_id_size = `SC_L3_SHIRE_ID_SIZE;
      bank_id_size     = `SC_BANK_ID_SIZE;
      sub_bank_id_size = `SC_SUB_BANK_ID_SIZE;

      shire_base_default    = 0;
      bank_base_default     = shire_base_default + l3_shire_id_size;
      sub_bank_base_default = bank_base_default + bank_id_size;

      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B0.set_reset(shire_base_default + 0);
      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B1.set_reset(shire_base_default + 1);
      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B2.set_reset(shire_base_default + 2);
      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B3.set_reset(shire_base_default + 3);
      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B4.set_reset(shire_base_default + 4);
      sc_l3_shire_swizzle_ctl.ESR_SC_SHIRE_SEL_B5.set_reset(shire_base_default + 5);

      sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B0.set_reset(bank_base_default + 0);
      sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B1.set_reset(bank_base_default + 1);
      sc_l3_shire_swizzle_ctl.ESR_SC_BANK_SEL_B2.set_reset(bank_base_default + 2);

      sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B0.set_reset(sub_bank_base_default + 0);
      sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B1.set_reset(sub_bank_base_default + 1);
      sc_l3_shire_swizzle_ctl.ESR_SC_SUB_BANK_SEL_B2.set_reset(sub_bank_base_default + 2);
   endfunction : set_l3_shire_swizzle_reset_val


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_l3_reqq_entries_reset_val
   //
   function void set_l3_reqq_entries_reset_val();
      int unsigned num_entries;
      `ifdef IOSHIRE_SC_SETS_PER_SUB_BANK
         num_entries = 16;
      `else
         num_entries = `SC_REQQ_DEPTH;
      `endif
      num_entries /= 3;

      sc_reqq_ctl.ESR_SC_NUM_L3_REQQ_ENTRIES.set_reset(num_entries);
   endfunction : set_l3_reqq_entries_reset_val


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compute_cache_ctl_reset_val
   //
   function void compute_cache_ctl_reset_val(input  int unsigned base_multiplier_in,
                                             input  int unsigned size_multiplier_in,
                                             output  evl_dword_t tag_mask,
                                             output  evl_dword_t set_mask,
                                             output int unsigned set_size,
                                             output int unsigned set_base
                                             );
      int unsigned sets_per_sub_bank;
      int unsigned base_default;
      int unsigned size_default;
      int unsigned size_log2;

      `ifdef IOSHIRE_SC_SETS_PER_SUB_BANK
         sets_per_sub_bank = `IOSHIRE_SC_SETS_PER_SUB_BANK;
      `else
         sets_per_sub_bank = `SC_SETS_PER_SUB_BANK;
      `endif

      base_default = sets_per_sub_bank * base_multiplier_in / 32;
      size_default = sets_per_sub_bank * size_multiplier_in / 32;

      size_log2 = $clog2(size_default);

      set_mask = (1'b1 << size_log2) - 1;

      if ((1'b1 << size_log2) == size_default) begin
         tag_mask = (1'b1 <<  size_log2)   - 1;
      end
      else begin
         tag_mask = (1'b1 << (size_log2-1)) -1;
      end

      set_size = size_default;
      set_base = base_default;

   endfunction : compute_cache_ctl_reset_val


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_cache_reg_block", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction : new

endclass : evl_shire_cache_reg_block
