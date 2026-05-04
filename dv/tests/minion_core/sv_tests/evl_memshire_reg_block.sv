//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_reg_block
//
class evl_memshire_reg_block extends ral_block_ms_regs;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_memshire_reg_block)

   evl_shire_id_t m_shire_id = ~evl_shire_id_t'(0);
   int            m_built = 0;
   evl_dword_t    m_reg_field_masks[string];
   evl_dword_t    m_reg_bits_no_test[string];


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
      if (m_built++ != 0) begin
         return;
      end
      super.build(evl_paddr_t'(`EVL_MEMSHIRE_BASE_ESR), offset_addr);

      if (`et_test_plusargs("ms_regs_disable_compare")) begin
         disable_field_checks();
      end

      `ifdef USE_REAL_DDRC
         ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_DDRC.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U0_CACTIVE_DDRC.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_DDRC.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U1_CACTIVE_DDRC.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_LO.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U0_CSYSACK_HI.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_LO.set_reset(1);
         ddrc_clock_ctl.ESR_DDRC_U1_CSYSACK_HI.set_reset(1);
      `endif

      // fix the reset value of the memshire_id field based on the ID value [0-7]
      // this is the lower three bits of the evl_shire_id, so just mask/keep those
      begin
         int unsigned memshire_id;
         memshire_id = (get_shire_id() & evl_shire_id_t'(7)) ;
         ms_mem_revision_id.ESR_MEMSHIRE_ID.set_reset(memshire_id);
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

      // Don't write the bit that starts the cache-op state machine as part of
      // the auto r/w test.  This will be tested separately
      m_reg_bits_no_test["ms_atomic_sm_ctl"] |= (1'b1 << ms_atomic_sm_ctl.ESR_MS_ATOMIC_EVICT_START.get_lsb_pos());

      // When using the Real DDR don't mess with the resets or clock controls
      // without the real DDR these don't do anything so it's good to test the r/w-ability then
      `ifdef USE_REAL_DDRC
         m_reg_bits_no_test["ddrc_reset_ctl"] |= (1'b1 << ddrc_reset_ctl.ESR_DDRC_SUBSYSTEM_RESET.get_lsb_pos());
         m_reg_bits_no_test["ddrc_reset_ctl"] |= (1'b1 << ddrc_reset_ctl.ESR_DDRC_APB_RESET.get_lsb_pos());
         m_reg_bits_no_test["ddrc_reset_ctl"] |= (1'b1 << ddrc_reset_ctl.ESR_DDRC_PUB_RESET.get_lsb_pos());
         m_reg_bits_no_test["ddrc_reset_ctl"] |= (1'b1 << ddrc_reset_ctl.ESR_DDRC_POWER_OK.get_lsb_pos());
         m_reg_bits_no_test["ddrc_reset_ctl"] |= (1'b1 << ddrc_reset_ctl.ESR_DDRC_PLL_CLK_SEL.get_lsb_pos());
      `endif

      m_reg_bits_no_test["ddrc_perfmon_ctl_status"] |= (1'b1 << ddrc_perfmon_ctl_status.ESR_CYC_S.get_lsb_pos());
      m_reg_bits_no_test["ddrc_perfmon_ctl_status"] |= (1'b1 << ddrc_perfmon_ctl_status.ESR_P0_S.get_lsb_pos());
      m_reg_bits_no_test["ddrc_perfmon_ctl_status"] |= (1'b1 << ddrc_perfmon_ctl_status.ESR_P1_S.get_lsb_pos());
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
   // Function: get_shire_id
   //
   // This function is used to get the shire ID for this component.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the shire ID of this component.
   //
   function evl_shire_id_t get_shire_id();
      return m_shire_id;
   endfunction : get_shire_id


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
   // Constructor
   //
   function new(input string name_in = "evl_memshire_reg_block", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction : new

endclass
