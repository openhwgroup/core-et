//--------------------------------------------------------------------------------------------------
//
// Class: evl_ms_config_test
//
class evl_ms_config_test extends evl_stimulus;

   `uvm_object_utils(evl_ms_config_test)

   typedef class evl_tcl_test_seq;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_ms_config_test m_ms_config_test = new();
   static bit                m_do_ms_config   = 1'b0;
   static bit                m_do_ddrc_init   = 1'b0;
   static bit                m_do_pll_init    = 1'b0;
   static int                m_use_pll_beh[evl_shire_id_t];

   `evl_param_key(pk_memshire_use_pll_beh)
   `evl_param_key(pk_memshire_num_addr_bits_to_remove)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_verif_comp    m_seq_agent_list[$];
   evl_shire_id_t    m_seq_shire_id_list[$];
   evl_tcl_handler   m_tcl_handler_list[$];
   evl_sequence_mdir m_config_sequences[evl_shire_id_t];


   //-----------------------------------------------------------------------------------------------
   typedef enum int {
         EVL_MS_mem_ctl      = 0,
         EVL_MS_clk_gate_ctl = 1
      } evl_ms_reg_type_t;


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ms_tcl_handler
   //
   class evl_ms_tcl_handler extends evl_tcl_handler;

      evl_shire_id_t m_ms_shire_id   = 232;
      int            m_ms_seq_count  = 1;
      int            m_ms_seq_number = 1;


      //--------------------------------------------------------------------------------------------
      //
      // Function: add_initial_commands
      //
      // The following code provides a way to set TCL variables before the meat of the script is
      // performed.
      //
      virtual function int add_initial_commands(input int thread_num);
         evl_tcl_eval_string(thread_num, $sformatf("set ms_shire_id {%0d};", m_ms_shire_id));
         evl_tcl_eval_string(thread_num, $sformatf("set ms_seq_count {%0d};", m_ms_seq_count));
         evl_tcl_eval_string(thread_num, $sformatf("set ms_seq_number {%0d};", m_ms_seq_number));
         evl_tcl_eval_string(thread_num, $sformatf("set ms_init_ddrc {%0d};", m_do_ddrc_init));
         evl_tcl_eval_string(thread_num, $sformatf("set ms_init_pll {%0d};", m_do_pll_init));
         if (m_use_pll_beh.exists(m_ms_shire_id)) begin
            evl_tcl_eval_string(thread_num, $sformatf("set ms_use_pll_beh {%0d};", m_use_pll_beh[m_ms_shire_id]));
         end
      endfunction : add_initial_commands


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_tcl_handler");
         super.new(name_in);
      endfunction : new

   endclass : evl_ms_tcl_handler


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_tcl_test
   //
   class evl_tcl_test_seq extends evl_sequence_mdir;

      //--------------------------------------------------------------------------------------------
      //
      // Static Member Variables and Objects
      //
      static int m_sequence_finished = 0;
      static int m_sequence          = 0;

      `evl_param_key(pk_sequence_done)


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      string             m_file_name;
      evl_ms_tcl_handler m_tcl_handler;
      int                m_sequence_number;
      int                m_sequence_limit;
      evl_sequence_mdir  m_prev_sequence;


      //--------------------------------------------------------------------------------------------
      //
      // Function: configure_phase
      //
      virtual task configure_phase(uvm_phase phase);
         if (m_tcl_handler == null) begin
            super.configure_phase(phase);
         end
         else begin
            phase.raise_objection(this);
            fork
               begin
                  super.configure_phase(phase);
               end
               begin
                  //
                  // Perform DDR init sequence after configuring the memshires.
                  //
                  if (m_prev_sequence != null) begin
                     m_prev_sequence.wait_for_int_param(pk_sequence_done, 1, 1);
                  end
                  m_tcl_handler.start_script();
                  enable_stimulus(`EVL_PHASE_DISABLE);
                  #1ns;
                  m_tcl_handler.wait_for_script_done();
                  m_sequence_finished++;
               end
            join
            phase.drop_objection(this);
            set_int_param(pk_sequence_done, 1);
         end
      endtask : configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_tcl_test_seq");
         super.new(name_in);
         disable_stimulus(`EVL_PHASE_DISABLE);
         m_background_seq = 1'b1;
         m_sequence_number = m_sequence++;
         set_int_param(pk_sequence_done, 0);
      endfunction

   endclass : evl_tcl_test_seq



   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ms_config_reg
   //
   class evl_ms_config_reg extends evl_object;
      evl_ms_reg_type_t m_reg_type;
      evl_paddr_t       m_addr;
      evl_dword_t       m_default;
      evl_dword_t       m_read_only;
      evl_dword_t       m_force_zero;
      evl_dword_t       m_force_one;
      evl_dword_t       m_value;


      //--------------------------------------------------------------------------------------------
      function evl_ms_reg_type_t get_type();
         return m_reg_type;
      endfunction : get_type


      //--------------------------------------------------------------------------------------------
      function evl_paddr_t get_paddr();
         return m_addr;
      endfunction : get_paddr


      //--------------------------------------------------------------------------------------------
      function void set_default(input evl_dword_t dword_in);
         m_default = (dword_in & (~m_force_zero)) | m_force_one;
      endfunction : set_default


      //--------------------------------------------------------------------------------------------
      function evl_dword_t get_default();
         return m_default;
      endfunction : get_default


      //--------------------------------------------------------------------------------------------
      function void set_value(input evl_dword_t dword_in);
         m_value = (dword_in & (~m_force_zero)) | m_force_one;
      endfunction : set_value


      //--------------------------------------------------------------------------------------------
      function evl_dword_t get_value();
         return m_value;
      endfunction : get_value


      //--------------------------------------------------------------------------------------------
      function new(input string            name_in       = "",
                   input evl_ms_reg_type_t reg_type_in   = EVL_MS_mem_ctl,
                   input evl_paddr_t       addr_in       = evl_paddr_t'(0),
                   input evl_dword_t       default_in    = evl_dword_t'(0),
                   input evl_dword_t       read_only_in  = evl_dword_t'(0),
                   input evl_dword_t       force_zero_in = evl_dword_t'(0),
                   input evl_dword_t       force_one_in  = evl_dword_t'(0));
         super.new(name_in);
         set_abstract_name(name_in);
         m_reg_type   = reg_type_in;
         m_addr       = addr_in;
         m_read_only  = read_only_in;
         m_force_zero = force_zero_in;
         m_force_one  = force_one_in;
         set_default(default_in);
         set_value(default_in);
      endfunction : new
   endclass : evl_ms_config_reg


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ms_rand_config_object
   //
   class evl_ms_rand_config_object extends evl_object;

      localparam ESR_MS_MC_EN_DEFAULT                   = 2'b11;
      localparam ESR_MS_ADDR_MASK_DEFAULT               = 12'h07f;
      localparam ESR_MS_ADDR_REMOVE_BIT_SEL3_DEFAULT    = 9;
      localparam ESR_MS_ADDR_REMOVE_BIT_SEL2_DEFAULT    = 8;
      localparam ESR_MS_ADDR_REMOVE_BIT_SEL1_DEFAULT    = 7;
      localparam ESR_MS_ADDR_REMOVE_BIT_SEL0_DEFAULT    = 6;
      localparam ESR_MS_ADDR_MC_BIT_SEL_DEFAULT         = 9;
      localparam ESR_MS_ADDR_DEBUG_MC_BIT_SEL_DEFAULT   = 6;
      localparam NUM_ADDR_BITS_TO_REMOVE_DEFAULT        = 4;
      localparam NUM_ADDR_MASK_BITS_DEFAULT             = 5;
      localparam DEBUG_CLOCK_DISABLE_DEFAULT            = 1'b0;

      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      static semaphore m_print_info = new(1);


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      bit [11:0]        r_ms_addr_mask             = ESR_MS_ADDR_MASK_DEFAULT;

      rand bit [1:0]    r_ms_mc_en                 = ESR_MS_MC_EN_DEFAULT;
      rand bit [5:0]    r_ms_addr_debug_mc_bit_sel = ESR_MS_ADDR_DEBUG_MC_BIT_SEL_DEFAULT;
      rand bit [5:0]    r_ms_addr_mc_bit_sel       = ESR_MS_ADDR_MC_BIT_SEL_DEFAULT;
      rand bit [5:0]    r_ms_addr_remove_bit_sel3  = ESR_MS_ADDR_REMOVE_BIT_SEL3_DEFAULT;
      rand bit [5:0]    r_ms_addr_remove_bit_sel2  = ESR_MS_ADDR_REMOVE_BIT_SEL2_DEFAULT;
      rand bit [5:0]    r_ms_addr_remove_bit_sel1  = ESR_MS_ADDR_REMOVE_BIT_SEL1_DEFAULT;
      rand bit [5:0]    r_ms_addr_remove_bit_sel0  = ESR_MS_ADDR_REMOVE_BIT_SEL0_DEFAULT;

      rand int unsigned r_num_addr_bits_to_remove = NUM_ADDR_BITS_TO_REMOVE_DEFAULT;
      rand int unsigned r_num_addr_mask_bits      = NUM_ADDR_MASK_BITS_DEFAULT;

      rand bit          r_debug_clock_disable = DEBUG_CLOCK_DISABLE_DEFAULT;

           bit          m_global_config;
           bit          m_use_rand_mode;


      //--------------------------------------------------------------------------------------------
      //
      // Register Storage (for fixing configurations)
      //
      evl_esr_ms_mem_ctl_t      m_ms_mem_ctl;
      evl_esr_ms_clk_gate_ctl_t m_ms_clk_gate_ctl;


      //--------------------------------------------------------------------------------------------
      //
      // Constraints
      //
      constraint c_ms_mc_en_order {solve r_ms_mc_en before r_num_addr_bits_to_remove;
                                   solve r_ms_mc_en before r_ms_addr_mc_bit_sel;
      }
      constraint c_num_addr_mask_bits_order {solve r_num_addr_mask_bits before r_num_addr_bits_to_remove;
                                             solve r_num_addr_mask_bits before r_ms_addr_mc_bit_sel;
                                             solve r_num_addr_mask_bits before r_ms_addr_debug_mc_bit_sel;
      }
      constraint c_num_addr_bits_to_remove_order {solve r_num_addr_bits_to_remove before r_ms_addr_remove_bit_sel3;
                                                  solve r_num_addr_bits_to_remove before r_ms_addr_remove_bit_sel2;
                                                  solve r_num_addr_bits_to_remove before r_ms_addr_remove_bit_sel1;
                                                  solve r_num_addr_bits_to_remove before r_ms_addr_remove_bit_sel0;
      }

      constraint c_ms_mc_en { r_ms_mc_en dist {2'b01 := 15, 2'b10 := 15, 2'b11 := 70}; }
      constraint c_num_addr_mask_bits { r_num_addr_mask_bits dist { 0:= 30, [1:12] :/ 70}; }

      constraint c_ms_addr_remove_bit_sel3 { if (r_num_addr_bits_to_remove > 3)
                                                r_ms_addr_remove_bit_sel3 < (`EVL_PADDR_WIDTH - r_num_addr_mask_bits);
                                             r_ms_addr_remove_bit_sel3 > r_ms_addr_remove_bit_sel2;
      }
      constraint c_ms_addr_remove_bit_sel2 { if (r_num_addr_bits_to_remove > 2)
                                                r_ms_addr_remove_bit_sel2 < (`EVL_PADDR_WIDTH - r_num_addr_mask_bits);
                                             r_ms_addr_remove_bit_sel2 > r_ms_addr_remove_bit_sel1;
      }
      constraint c_ms_addr_remove_bit_sel1 { if (r_num_addr_bits_to_remove > 1)
                                                r_ms_addr_remove_bit_sel1 < (`EVL_PADDR_WIDTH - r_num_addr_mask_bits);
                                             r_ms_addr_remove_bit_sel1 > r_ms_addr_remove_bit_sel0;
      }
      constraint c_ms_addr_remove_bit_sel0 { if (r_num_addr_bits_to_remove > 0)
                                                r_ms_addr_remove_bit_sel0 < (`EVL_PADDR_WIDTH - r_num_addr_mask_bits);
                                             r_ms_addr_remove_bit_sel0 > 5;
      }

      // if ecc is enabled then some address space is reserved for in-line ecc
      // storage.  We can't issue requests to those addresses.  So the ms_rand
      // test ensures those addresses are not selected, and the ms_config test
      // ensures we only remove address bits below the bits that are used to
      // determine ecc space.
      constraint c_ms_addr_remove_bit_sel_ecc { r_ms_addr_remove_bit_sel3  < 29;
                                                r_ms_addr_remove_bit_sel2  < 28;
                                                r_ms_addr_remove_bit_sel1  < 27;
                                                r_ms_addr_remove_bit_sel0  < 26;
                                                r_ms_addr_debug_mc_bit_sel < 26;
      }

      constraint c_num_addr_bits_to_remove { r_num_addr_bits_to_remove dist { 4 := 80, [0:3] :/ 20};
                                             if (r_ms_mc_en == 2'b11) {
                                                r_num_addr_bits_to_remove > 0;
                                                r_num_addr_bits_to_remove <= 4;
                                             }
                                             else
                                                r_num_addr_bits_to_remove <= 3;
      }

      // seems like mc_sel can't be bit 39
      constraint c_ms_addr_mc_bit_sel { r_ms_addr_mc_bit_sel < (`EVL_PADDR_WIDTH - 1 - r_num_addr_mask_bits);
                                        if (r_ms_mc_en == 2'b11) {
                                           if (r_num_addr_bits_to_remove == 4)
                                              r_ms_addr_mc_bit_sel inside { r_ms_addr_remove_bit_sel3, r_ms_addr_remove_bit_sel2, r_ms_addr_remove_bit_sel1, r_ms_addr_remove_bit_sel0};
                                           else if (r_num_addr_bits_to_remove == 3)
                                              r_ms_addr_mc_bit_sel inside {                            r_ms_addr_remove_bit_sel2, r_ms_addr_remove_bit_sel1, r_ms_addr_remove_bit_sel0};
                                           else if (r_num_addr_bits_to_remove == 2)
                                              r_ms_addr_mc_bit_sel inside {                                                       r_ms_addr_remove_bit_sel1, r_ms_addr_remove_bit_sel0};
                                           else if (r_num_addr_bits_to_remove == 1)
                                              r_ms_addr_mc_bit_sel inside {                                                                                  r_ms_addr_remove_bit_sel0};
                                        }
      }

      constraint c_ms_addr_debug_mc_bit_sel { r_ms_addr_debug_mc_bit_sel < (`EVL_PADDR_WIDTH - r_num_addr_mask_bits);
                                              r_ms_addr_debug_mc_bit_sel > 5;
      }


      //--------------------------------------------------------------------------------------------
      function void post_randomize();
         m_ms_mem_ctl.esr_ms_mc_en = r_ms_mc_en;
         m_ms_mem_ctl.esr_ms_addr_mask = (1 << (12-r_num_addr_mask_bits)) - 1;
         m_ms_mem_ctl.esr_ms_addr_mc_bit_sel = r_ms_addr_mc_bit_sel;
         m_ms_mem_ctl.esr_ms_addr_debug_mc_bit_sel = r_ms_addr_debug_mc_bit_sel;

         if (r_num_addr_bits_to_remove > 3) begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel3 = r_ms_addr_remove_bit_sel3;
         end
         else begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel3 = 6'b111111;
         end

         if (r_num_addr_bits_to_remove > 2) begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel2 = r_ms_addr_remove_bit_sel2;
         end
         else begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel2 = 6'b111111;
         end

         if (r_num_addr_bits_to_remove > 1) begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel1 = r_ms_addr_remove_bit_sel1;
         end
         else begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel1 = 6'b111111;
         end

         if (r_num_addr_bits_to_remove > 0) begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel0 = r_ms_addr_remove_bit_sel0;
         end
         else begin
            m_ms_mem_ctl.esr_ms_addr_remove_bit_sel0 = 6'b111111;
         end

         m_ms_clk_gate_ctl.debug_clock_disable = r_debug_clock_disable;
      endfunction : post_randomize


      //--------------------------------------------------------------------------------------------
      task print_config();
         m_print_info.get();
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("Memshire Config:"))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_mc_en:                       0b%2b", m_ms_mem_ctl.esr_ms_mc_en                ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_mask:                   0x%3x", m_ms_mem_ctl.esr_ms_addr_mask            ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_debug_mc_bit_sel:       %0d",   m_ms_mem_ctl.esr_ms_addr_debug_mc_bit_sel))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_mc_bit_sel:             %0d",   m_ms_mem_ctl.esr_ms_addr_mc_bit_sel      ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_remove_bit_sel3:        %0d",   m_ms_mem_ctl.esr_ms_addr_remove_bit_sel3 ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_remove_bit_sel2:        %0d",   m_ms_mem_ctl.esr_ms_addr_remove_bit_sel2 ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_remove_bit_sel1:        %0d",   m_ms_mem_ctl.esr_ms_addr_remove_bit_sel1 ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   esr_ms_addr_remove_bit_sel0:        %0d",   m_ms_mem_ctl.esr_ms_addr_remove_bit_sel0 ))
         `evl_log(UVM_NONE, VMOD_CONFIG, get_abstract_name(), $sformatf("   debug_clock_disable:                %0b",   m_ms_clk_gate_ctl.debug_clock_disable    ))
         `evl_log_no_time(UVM_NONE, VMOD_CONFIG, "")
         m_print_info.put();
      endtask : print_config


      function int unsigned get_num_addr_bits_to_remove();
         return r_num_addr_bits_to_remove;
      endfunction


      //--------------------------------------------------------------------------------------------
      function new(input string name_in = "evl_ms_rand_config_object", input bit global_config_in = 1'b0);
         longint value;

         super.new(name_in);
         set_abstract_name(name_in);
         m_global_config = global_config_in;

         if ((`et_test_plusargs("msConfig=fixed") != 0) || (`et_test_plusargs("msConfig=random") == 0)) begin
            m_use_rand_mode = 1'b0;
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("ms_rand_config");
            if (value == -1) begin
               m_use_rand_mode = 1'b1;
            end
            else begin
               m_use_rand_mode = value[0:0];
            end
         end

         // usually when running a power test the debug clock should be disabled
         if (m_global_attrs.plus_arg_match("ms_pwr_run") == 1'b1) begin
            r_debug_clock_disable = 1;
            r_debug_clock_disable.rand_mode(0);
         end

         // If we're running a debug test, keep the clock on!
         if (m_global_attrs.plus_arg_match("py_test") == 1'b1) begin
            r_debug_clock_disable = 0;
            r_debug_clock_disable.rand_mode(0);
         end

         // if ecc is enabled we need these further constraints on where the
         // bit removals can be within the address
         if (`et_test_plusargs("ddr_ecc_en=1") != 0) begin
            c_ms_addr_remove_bit_sel_ecc.constraint_mode(1);
         end
         else begin
            c_ms_addr_remove_bit_sel_ecc.constraint_mode(0);
         end

         // plusargs to override individual register settings
         // NOTE: these have checks to make sure the plusargs are in-bounds
         // BUT there are not checks that the manually-specified values are
         // legal in reference to each other!
         // (E.g. no check to ensure remove_bit_sel3 > remove_bit_sel2)
         //
         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_mc_en");
         if (value != -1) begin
            if (value < 1) begin
               r_ms_mc_en = 2'b01;
            end
            else if (value > 3) begin
               r_ms_mc_en = 2'b11;
            end
            else begin
               r_ms_mc_en = value[1:0];
            end
            r_ms_mc_en.rand_mode(0);
            c_ms_mc_en.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_num_addr_mask_bits");
         if (value != -1) begin
            if (value > 12) begin
               r_num_addr_mask_bits = 12;
            end
            else begin
               r_num_addr_mask_bits = value[3:0];
            end
            r_num_addr_mask_bits.rand_mode(0);
            c_num_addr_mask_bits.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_debug_mc_bit_sel");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_debug_mc_bit_sel = 63;
            end
            else if (value < 6) begin
               r_ms_addr_debug_mc_bit_sel = 6;
            end
            else begin
               r_ms_addr_debug_mc_bit_sel = value[5:0];
            end
            r_ms_addr_debug_mc_bit_sel.rand_mode(0);
            c_ms_addr_debug_mc_bit_sel.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_mc_bit_sel");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_mc_bit_sel = 63;
            end
            else if (value < 6) begin
               r_ms_addr_mc_bit_sel = 6;
            end
            else begin
               r_ms_addr_mc_bit_sel = value[5:0];
            end
            r_ms_addr_mc_bit_sel.rand_mode(0);
            c_ms_addr_mc_bit_sel.constraint_mode(0);
         end

         // Note: there are switches for both the number of address bits to
         // remove, as well as one for each address bits.  It's not
         // recommended to use both the "num_addr_bits" and the
         // "remove_bit_sel" at the same time!
         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_num_addr_bits_to_remove");
         if (value != -1) begin
            if (value > 4) begin
               r_num_addr_bits_to_remove = 4;
            end
            else begin
               r_num_addr_bits_to_remove = value[2:0];
            end
            r_num_addr_bits_to_remove.rand_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_remove_bit_sel0");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_remove_bit_sel0 = 63;
            end
            else if (value < 6) begin
               r_ms_addr_remove_bit_sel0 = 6;
            end
            else begin
               r_ms_addr_remove_bit_sel0 = value[5:0];
            end
            r_ms_addr_remove_bit_sel0.rand_mode(0);
            c_ms_addr_remove_bit_sel0.constraint_mode(0);
            r_num_addr_bits_to_remove = 1;
            r_num_addr_bits_to_remove.rand_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_remove_bit_sel1");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_remove_bit_sel1 = 63;
            end
            else if (value < 6) begin
               r_ms_addr_remove_bit_sel1 = 6;
            end
            else begin
               r_ms_addr_remove_bit_sel1 = value[5:0];
            end
            r_ms_addr_remove_bit_sel1.rand_mode(0);
            c_ms_addr_remove_bit_sel1.constraint_mode(0);
            r_num_addr_bits_to_remove = 2;
            r_num_addr_bits_to_remove.rand_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_remove_bit_sel2");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_remove_bit_sel2 = 63;
            end
            else if (value < 6) begin
               r_ms_addr_remove_bit_sel2 = 6;
            end
            else begin
               r_ms_addr_remove_bit_sel2 = value[5:0];
            end
            r_ms_addr_remove_bit_sel2.rand_mode(0);
            c_ms_addr_remove_bit_sel2.constraint_mode(0);
            r_num_addr_bits_to_remove = 3;
            r_num_addr_bits_to_remove.rand_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
         end

         value = m_global_attrs.get_plus_arg_value_as_int("esr_ms_mem_ctl_remove_bit_sel3");
         if (value != -1) begin
            if (value > 63) begin
               r_ms_addr_remove_bit_sel3 = 63;
            end
            else if (value < 6) begin
               r_ms_addr_remove_bit_sel3 = 6;
            end
            else begin
               r_ms_addr_remove_bit_sel3 = value[5:0];
            end
            r_ms_addr_remove_bit_sel3.rand_mode(0);
            c_ms_addr_remove_bit_sel3.constraint_mode(0);
            r_num_addr_bits_to_remove = 4;
            r_num_addr_bits_to_remove.rand_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
         end

         if (m_use_rand_mode == 1'b0) begin
            r_ms_addr_mask.rand_mode(0);
            r_ms_mc_en.rand_mode(0);
            r_ms_addr_debug_mc_bit_sel.rand_mode(0);
            r_ms_addr_mc_bit_sel.rand_mode(0);
            r_ms_addr_remove_bit_sel3.rand_mode(0);
            r_ms_addr_remove_bit_sel2.rand_mode(0);
            r_ms_addr_remove_bit_sel1.rand_mode(0);
            r_ms_addr_remove_bit_sel0.rand_mode(0);
            r_num_addr_bits_to_remove.rand_mode(0);
            r_num_addr_mask_bits.rand_mode(0);
            r_debug_clock_disable.rand_mode(0);

            c_ms_mc_en_order.constraint_mode(0);
            c_num_addr_mask_bits_order.constraint_mode(0);
            c_num_addr_bits_to_remove_order.constraint_mode(0);
            c_ms_mc_en.constraint_mode(0);
            c_num_addr_mask_bits.constraint_mode(0);
            c_ms_addr_remove_bit_sel3.constraint_mode(0);
            c_ms_addr_remove_bit_sel2.constraint_mode(0);
            c_ms_addr_remove_bit_sel1.constraint_mode(0);
            c_ms_addr_remove_bit_sel0.constraint_mode(0);
            c_num_addr_bits_to_remove.constraint_mode(0);
            c_ms_addr_mc_bit_sel.constraint_mode(0);
            c_ms_addr_debug_mc_bit_sel.constraint_mode(0);
         end
      endfunction : new

   endclass : evl_ms_rand_config_object


   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ms_config_test_seq
   //
   class evl_ms_config_test_seq extends evl_sequence_mdir;

      `evl_param_key(pk_mem_req_delay)
      `evl_param_key(pk_mem_bus_rsp_delay)
      `evl_param_key(pk_sequence_done)

      //--------------------------------------------------------------------------------------------
      //
      // Static Member Variables and Objects
      //
      static int                       m_global_config_randomized = 0;
      static evl_ms_rand_config_object m_global_config_object     = new("MS_Rand", 1'b1);


      //--------------------------------------------------------------------------------------------
      //
      // Member Variables and Objects
      //
      evl_verif_comp                    m_ms_rtl_agent;
      bit                               m_use_rand_mode;
      evl_ms_config_reg                 m_config_regs[$];
      evl_ms_rand_config_object         m_config_object;
      evl_shire_id_t                    m_ms_shire_id;


      //--------------------------------------------------------------------------------------------
      function void create_ms_mem_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_ms_mem_ctl_t default_val    = evl_esr_ms_mem_ctl_t'(0);
         evl_esr_ms_mem_ctl_t read_only_val  = evl_esr_ms_mem_ctl_t'(0);
         evl_esr_ms_mem_ctl_t force_zero_val = evl_esr_ms_mem_ctl_t'(0);
         evl_esr_ms_mem_ctl_t force_one_val  = evl_esr_ms_mem_ctl_t'(0);
         evl_ms_config_reg    config_reg;

         default_val.esr_ms_mc_en                 = 2'b11;
         default_val.esr_ms_addr_mask             = 12'h07f;
         default_val.esr_ms_addr_debug_mc_bit_sel = 6'd6;
         default_val.esr_ms_addr_mc_bit_sel       = 6'd6;
         default_val.esr_ms_addr_remove_bit_sel3  = 6'd9;
         default_val.esr_ms_addr_remove_bit_sel2  = 6'd8;
         default_val.esr_ms_addr_remove_bit_sel1  = 6'd7;
         default_val.esr_ms_addr_remove_bit_sel0  = 6'd6;

         config_reg = new(name_in,
                          EVL_MS_mem_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_ms_mem_ctl


      //--------------------------------------------------------------------------------------------
      function void create_ms_clk_gate_ctl(input evl_paddr_t addr, input string name_in);
         evl_esr_ms_clk_gate_ctl_t default_val    = evl_esr_ms_clk_gate_ctl_t'(0);
         evl_esr_ms_clk_gate_ctl_t read_only_val  = evl_esr_ms_clk_gate_ctl_t'(0);
         evl_esr_ms_clk_gate_ctl_t force_zero_val = evl_esr_ms_clk_gate_ctl_t'(0);
         evl_esr_ms_clk_gate_ctl_t force_one_val  = evl_esr_ms_clk_gate_ctl_t'(0);
         evl_ms_config_reg         config_reg;

         default_val.debug_clock_disable = 1'b0;

         config_reg = new(name_in,
                          EVL_MS_clk_gate_ctl,
                          addr,
                          evl_dword_t'(default_val),
                          evl_dword_t'(read_only_val),
                          evl_dword_t'(force_zero_val),
                          evl_dword_t'(force_one_val));
         m_config_regs.push_back(config_reg);
      endfunction : create_ms_clk_gate_ctl


      //--------------------------------------------------------------------------------------------
      //
      // Task: build_registers
      //
      function bit build_registers(input evl_shire_id_t shire_id_in);
         uvm_reg        register;
         evl_verif_comp ms_agent;
         evl_reg_block  ms_reg_block;
         uvm_reg_block  reg_block;
         uvm_reg_block  reg_blocks[$];

         ms_agent = evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id_in);
         if (ms_agent == null) begin
            return 1'b0;
         end

         ms_reg_block = ms_agent.get_evl_reg_block();
         if (ms_reg_block == null) begin
            return 1'b0;
         end

         reg_block = ms_reg_block.get_block_by_name("memshire");
         if (reg_block == null) begin
            return 1'b0;
         end
         reg_blocks.push_back(reg_block);

         register = reg_blocks[0].get_reg_by_name("ms_mem_ctl");
         if (register == null) begin
            return 1'b0;
         end
         create_ms_mem_ctl(register.get_address(), register.get_full_name());

         register = reg_blocks[0].get_reg_by_name("ms_clk_gate_ctl");
         if (register == null) begin
            return 1'b0;
         end
         create_ms_clk_gate_ctl(register.get_address(), register.get_full_name());

         return 1'b1;
      endfunction : build_registers


      //--------------------------------------------------------------------------------------------
      //
      // Task: configure_me
      //
      virtual task configure_me(output bit result);
         evl_paddr_t    addr;
         evl_trans_desc trans_desc;
         evl_trans_desc trans_descs[$];
         evl_dword_t    dword;

         if (build_registers(m_ms_shire_id) == 1'b0) begin
            result = 1'b0;
            return;
         end
         result = 1'b1;

         foreach (m_config_regs[ii]) begin
            addr = m_config_regs[ii].get_paddr();
            issue(trans_desc, LD, addr, 8, evl_bus_req_source_t'(0), {}, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
            trans_descs.push_back(trans_desc);
         end
         trans_descs[trans_descs.size() - 1].wait_for_done();
         foreach (m_config_regs[ii]) begin
            trans_descs[ii].wait_for_done();
            dword = trans_descs[ii].get_read_dword(0);
            `evl_log_mdesc(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), trans_descs[ii], $sformatf("Read 0x%x (0x%x) for %0s", dword, m_config_regs[ii].get_default(), m_config_regs[ii].get_abstract_name()))
         end

         if (`et_test_plusargs("msConfig=random") != 0) begin
            m_config_object.randomize();
         end
         else begin
            m_config_object.post_randomize();
         end

         foreach (m_config_regs[ii]) begin
            evl_ms_reg_type_t reg_type = m_config_regs[ii].get_type();

            if (reg_type == EVL_MS_mem_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_config_object.m_ms_mem_ctl));
            end
            else if (reg_type == EVL_MS_clk_gate_ctl) begin
               m_config_regs[ii].set_value(evl_dword_t'(m_config_object.m_ms_clk_gate_ctl));
            end
         end

         // the evl_ms_rand_test needs to know how many address bits are
         // removed, for when ecc is enabled (certain addresses can't be used)
         m_ms_rtl_agent = evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), m_ms_shire_id);
         if (m_ms_rtl_agent != null) begin
            m_ms_rtl_agent.set_rtl_int_param_by_name("pk_memshire_num_addr_bits_to_remove", m_config_object.get_num_addr_bits_to_remove());
         end
      endtask : configure_me


      //--------------------------------------------------------------------------------------------
      //
      // Task: write_configuration
      //
      task write_configuration();
         evl_trans_desc    trans_desc;
         evl_trans_desc    trans_descs[$];
         evl_dword_t       dword;
         evl_ms_config_reg config_reg;
         evl_ms_reg_type_t reg_type;

         //
         // Read the default values twice ... this is required to ensure there is enough time for the
         // debug reset to be deasserted (32 ref clocks when the PLL is enabled).
         //
         repeat (2) begin
            foreach (m_config_regs[ii]) begin
               issue(trans_desc, LD, m_config_regs[ii].get_paddr(), 8, evl_bus_req_source_t'(0), { }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
               trans_descs.push_back(trans_desc);
            end
            foreach (trans_descs[ii]) begin
               trans_descs[ii].wait_for_done();
            end
            foreach (m_config_regs[ii]) begin
               dword = trans_descs[ii].get_read_dword(0);
               `evl_log_mdesc(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), trans_descs[ii], $sformatf("Read Default Value 0x%x for %0s", dword, m_config_regs[ii].get_abstract_name()))
            end
         end

         // now write the values
         foreach (m_config_regs[ii]) begin
            config_reg = m_config_regs[ii];
            reg_type   = config_reg.get_type();

            if (reg_type inside { EVL_MS_mem_ctl, EVL_MS_clk_gate_ctl}) begin
               dword = config_reg.get_value();
               `evl_log(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("Writing 0x%x to %0s (0x%x)", dword, config_reg.get_abstract_name(), config_reg.get_paddr()))
               issue(trans_desc, ST, config_reg.get_paddr(), 8, evl_bus_req_source_t'(0), {dword}, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
               trans_descs.push_back(trans_desc);
            end
         end
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end
         trans_descs = {};

         foreach (m_config_regs[ii]) begin
            issue(trans_desc, LD, m_config_regs[ii].get_paddr(), 8, evl_bus_req_source_t'(0), { }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
            trans_descs.push_back(trans_desc);
         end
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end
         foreach (m_config_regs[ii]) begin
            dword = trans_descs[ii].get_read_dword(0);
            `evl_log_mdesc(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), trans_descs[ii], $sformatf("Read 0x%x for %0s", dword, m_config_regs[ii].get_abstract_name()))
         end
         blocking_issue(trans_desc, BARRIER, 0);
      endtask : write_configuration


      //--------------------------------------------------------------------------------------------
      //
      // Task: pre_configure_phase
      //
      virtual task pre_configure_phase(input uvm_phase phase);
         evl_verif_comp mesh_atomic_agent;

         //
         // Clear the req/rsp delays of the MeshAtomic slave agent.  The memshire does nothing with
         // the AXI B Response so there is no additional value in providing backpressure here.
         // However the delays can in some instances cause a sim to fail (phase timeout) so we might
         // as well just eliminate the delays.  Also, it's done here because this occurs after the
         // random delays are already added.
         //
         mesh_atomic_agent = evl_verif_comp_map::get_verif_comp(MESH_SLAVE_STIM_AGENT);
         if (mesh_atomic_agent != null) begin
            mesh_atomic_agent.reset_delay(pk_mem_req_delay);
            mesh_atomic_agent.reset_delay(pk_mem_bus_rsp_delay);
         end

         super.pre_configure_phase(phase);
      endtask : pre_configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Task: configure_phase
      //
      virtual task configure_phase(input uvm_phase phase);
         int value;

         phase.raise_objection(phase);

         if (`et_test_plusargs("msConfig=random") == 0) begin
            m_use_rand_mode = 1'b0;
         end
         else begin
            value = m_global_attrs.get_plus_arg_value_as_int("ms_rand_config");
            if (value == -1) begin
               m_use_rand_mode = 1'b1;
            end
            else begin
               m_use_rand_mode = value[0:0];
            end
         end

         fork
            begin
               super.configure_phase(phase);
            end
            begin
               bit result;

               configure_me(result);
               if (result == 1'b1) begin
                  write_configuration();
               end
            end
         join
         phase.drop_objection(phase);
         set_int_param(pk_sequence_done, 1);
      endtask : configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Task: post_configure_phase
      //
      virtual task post_configure_phase(input uvm_phase phase);
         disable_stimulus(`EVL_ON_ICE_DISABLE);
         m_config_object.print_config();
         super.configure_phase(phase);
      endtask : post_configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_ms_config_test_seq", input evl_verif_comp agent_in = null, input evl_shire_id_t shire_id_in);
         super.new(name_in);
         m_config_object = new(name_in);
         set_int_param(pk_sequence_done, 0);
         m_ms_shire_id = shire_id_in;
      endfunction

   endclass : evl_ms_config_test_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_agent
   //
   function void prepare_tcl_for_agent(input evl_tcl_test_seq test_seq,
                                       input string           file_name,
                                       input evl_shire_id_t   ms_shire_id,
                                       input int              seq_count,
                                       input int              seq_number);
      evl_verif_comp         agent;
      evl_tcl_mem_seq_parser tcl_mem_seq_parser;
      evl_ms_tcl_handler     tcl_handler;

      if (test_seq == null) begin
         return;
      end

      agent = test_seq.get_agent();
      `evl_log(UVM_DEBUG, VMOD_TCL, get_abstract_name(), $sformatf("TCL file found: %s for agent %s", file_name, agent.get_abstract_name()))

      //
      // We have found a TCL file to use, so create a TCL handler for it.
      //
      tcl_handler                 = new();
      tcl_mem_seq_parser          = new();
      tcl_handler.m_ms_shire_id   = ms_shire_id;
      tcl_handler.m_ms_seq_count  = seq_count;
      tcl_handler.m_ms_seq_number = seq_number;
      test_seq.m_tcl_handler      = tcl_handler;
      test_seq.m_file_name        = file_name;
      tcl_handler.set_agent(agent, test_seq.m_sequence_number);
      tcl_handler.set_semaphore_wrapper(m_semaphore_wrapper);
      tcl_handler.set_sequencer_id(test_seq.m_sequence_number);
      tcl_handler.add_sequence_wrapper_for_agent(agent, test_seq.get_wrapper(), test_seq.m_sequence_number);
      tcl_handler.add_tcl_cmd_parser(tcl_mem_seq_parser);
      tcl_handler.set_file_name(file_name);
      tcl_handler.set_starting_phase(ConfigurePhase);
      tcl_handler.set_abstract_name($sformatf("TCL Handler Sequence for %0s (%0s)", agent.get_abstract_name(), file_name));
      m_tcl_handler_list.push_back(tcl_handler);
   endfunction : prepare_tcl_for_agent


   //--------------------------------------------------------------------------------------------
   //
   // Function: get_setup_file_name
   //
   function string get_setup_file_name(input string base_file_name);
      string parse_string;
      string file_name;
      string full_file_name;

      //
      // Allow the user to specify a different setup TCL file.
      //
      parse_string = $sformatf("%s=%ss", base_file_name, "%");
      if (`et_value_plusargs(parse_string, file_name) == 0) begin
         file_name = $sformatf("lib/%s.tcl", base_file_name);
      end

      full_file_name = evl_tcl_find_file(file_name, 0);
      if (full_file_name == "") begin
         full_file_name = evl_tcl_find_file($sformatf("dv/tests/memshire/tcl_tests/tcl_scripts/%s", file_name), 0);
      end
      if (full_file_name == "") begin
         full_file_name = evl_tcl_find_file(file_name, 1);
      end

      if (full_file_name == "") begin
         `dut_error("MS-Config", $sformatf("unable to find setup file \"%s\"", file_name))
      end
      else begin
         `evl_log(UVM_MEDIUM, VMOD_TEST, get_abstract_name(), $sformatf("Using %0s as a setup TCL file", full_file_name))
      end
      return full_file_name;
   endfunction : get_setup_file_name


   //--------------------------------------------------------------------------------------------
   //
   // Function: common_tcl_setup
   //
   function void common_tcl_setup(input bit do_pll_init, input bit do_ddrc_init);
      int                       seq_count;
      int                       seq_number;
      string                    seq_name;
      string                    file_name;
      string                    file_name_list[$];
      evl_verif_comp            agent;
      evl_shire_id_t            shire_id;
      evl_tcl_test_seq          test_seq;
      evl_sequence_wrapper_base sequence_wrapper;

      if (do_pll_init != 1'b0) begin
         file_name = get_setup_file_name("pll_setup");
         if (file_name == "") begin
            return;
         end
         file_name_list.push_back(file_name);
      end

      if (do_ddrc_init != 1'b0) begin
         file_name = get_setup_file_name("ddr_setup");
         if (file_name == "") begin
            return;
         end
         file_name_list.push_back(file_name);
      end

      //
      // For each shire ID, create sequences for each new file.
      //
      seq_count = m_seq_shire_id_list.size();
      foreach (m_seq_shire_id_list[ii]) begin
         int file_name_count;

         seq_number      = 0;
         agent           = m_seq_agent_list[ii];
         shire_id        = m_seq_shire_id_list[ii];
         file_name_count = file_name_list.size();
         for (int jj = 0; jj < file_name_count; jj++) begin
            seq_name = $sformatf("Memshire Configuration TCL Sequence #%0d for S%0d", jj + 1, shire_id);
            `evl_log(UVM_LOW, VMOD_TEST, "MS-Config", $sformatf("creating \"%s\"", seq_name))

            test_seq         = new();
            sequence_wrapper = test_seq.get_wrapper();
            test_seq.set_abstract_name(seq_name);
            sequence_wrapper.set_abstract_name(seq_name);
            agent.add_sequence_wrapper(sequence_wrapper, 0);

            //
            // Link this sequence to a previous sequence for the same memory shire.
            //
            if (m_config_sequences.exists(shire_id)) begin
               test_seq.m_prev_sequence = m_config_sequences[shire_id];
            end
            m_config_sequences[shire_id] = test_seq;

            prepare_tcl_for_agent(test_seq, file_name_list[jj], shire_id, seq_count, seq_number);
         end
         seq_number++;
      end
   endfunction : common_tcl_setup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      int                       ddr_count = 0;
      evl_verif_comp            agent;
      evl_verif_comp            agent_list[];
      evl_shire_id_t            shire_id;
      evl_shire_id_t            shire_id_list[$];
      bit                       allow_pll_init;
      string                    seq_name;
      evl_ms_config_test_seq    test_seq;
      evl_sequence_wrapper_base sequence_wrapper;

      //
      // Because of the way our system is created, we will always make DDR_SHIRE_RTL_AGENT agents
      // for each memory shire.  For memory shire gate simulations or full-chip simulations, we will
      // not have SBM_MASTER_STIM_AGENT agents, but we may have an IO_PM_MASTER_STIM_AGENT agent.
      //
      // Start by searching for DDR RTL agents.  For each DDR RTL agent, there should be a memory
      // shire.
      //
      void'(evl_verif_comp_map::get_verif_comp_list(agent_list, DDR_SHIRE_RTL_AGENT));
      foreach (agent_list[jj]) begin
         shire_id_list.push_back(agent_list[jj].get_shire_id());
         ddr_count++;
      end

      //
      // Now see if there is a corresponding SBM_MASTER_STIM_AGENT for each DDR RTL agent.
      //
      void'(evl_verif_comp_map::get_verif_comp_list(agent_list, SBM_MASTER_STIM_AGENT));
      foreach (agent_list[ii]) begin
         if (agent_list[ii].get_shire_id() inside { shire_id_list }) begin
            m_seq_agent_list.push_back(agent_list[ii]);
            m_seq_shire_id_list.push_back(agent_list[ii].get_shire_id());
         end
      end

      //
      // Now remove all shire IDs that have an SBM_MASTER_STIM_AGENT.
      //
      for (int ii = shire_id_list.size() - 1; ii >= 0; ii--) begin
         if (shire_id_list[ii] inside { m_seq_shire_id_list }) begin
            shire_id_list.delete(ii);
         end
      end

      //
      // If there are any memory shires not accounted for, attempt to create sequences on the
      // IO_PM_MASTER_STIM_AGENT stimulus agent.
      //
      if ((shire_id_list.size() > 0) && (evl_verif_comp_map::get_verif_comp_list(agent_list, IO_PM_MASTER_STIM_AGENT) > 0)) begin
         foreach (shire_id_list[ii]) begin
            m_seq_agent_list.push_back(agent_list[0]);
            m_seq_shire_id_list.push_back(shire_id_list[ii]);
         end
      end

      //
      // Find out if we're using the behavioral PLL or the real model for each possible memshire ID.
      //
      allow_pll_init = (`et_test_plusargs("skip_pll_init") == 0) ? 1'b1 : 1'b0;
      foreach (m_seq_shire_id_list[ii]) begin
         shire_id = m_seq_shire_id_list[ii];
         agent    = evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id);
         if (agent == null) begin
            agent = evl_verif_comp_map::get_verif_comp(DDR_SHIRE_RTL_AGENT, ~evl_agent_id_t'(0), shire_id);
         end
         if (agent != null) begin
            m_use_pll_beh[shire_id] = agent.get_int_param(pk_memshire_use_pll_beh, -1);
         end
         else begin
            `ifdef DV_MEMSHIRE_USE_PLL_BEH
               m_use_pll_beh[shire_id] = `DV_MEMSHIRE_USE_PLL_BEH;
            `else
               m_use_pll_beh[shire_id] = 0;
            `endif
         end
         if (m_use_pll_beh[shire_id] > 0) begin
            m_use_pll_beh[shire_id] = 1;
         end
         else begin
            m_use_pll_beh[shire_id] = 0;
            m_do_pll_init = allow_pll_init;
         end
      end

      //
      // If we are using real DDR, it may be necessary to initialize the DDR controller.  If
      // MEMSHIRE_AUTO_INIT_ENABLE is defined and the run-time argument +ddr_auto_init=1 is defined,
      // we will not initialize the DDR controller with TCL.  In addition, if the run-time argument
      // +skip_ddr_init is defined, we will not initialize the DDR controller with TCL.
      //
      `ifdef USE_REAL_DDRC
         if (`et_test_plusargs("skip_ddr_init") == 0) begin
            `ifdef MEMSHIRE_AUTO_INIT_ENABLE
               if (`et_test_plusargs("ddr_auto_init=1") == 0) begin
                  m_do_ddrc_init = 1'b1;
               end
               else begin
                  m_do_ddrc_init = 1'b0;
                  m_do_pll_init  = 1'b0;
               end
            `else // ifdef MEMSHIRE_AUTO_INIT_ENABLE
               m_do_ddrc_init = 1'b1;
            `endif // !ifdef MEMSHIRE_AUTO_INIT_ENABLE
         end
      `endif // ifdef USE_REAL_DDRC

      //
      // We need to go through the appropriate order of events.  The first is performing any memory
      // shire configuration (if desired).  Note that this is reversed when DDR auto-init is
      // enabled (not sure if this causes problems).
      //
      // Also note that we will not do any memshire config if there is more than 1 DDR or memory
      // agent OR if there is at least 1 DDR memory agent and +msConfig=fixed is used.
      //
      if ( (`et_test_plusargs("noMSConfig") == 0) && (`et_test_plusargs("msConfig=none") == 0) &&
           ((ddr_count == 1) || ((ddr_count >= 1) && (`et_test_plusargs("msConfig=fixed") != 0))) ) begin
         //
         // For each agent in the respective lists, create a memory initialization sequence.
         //
         foreach (m_seq_agent_list[ii]) begin
            agent    = m_seq_agent_list[ii];
            shire_id = m_seq_shire_id_list[ii];
            seq_name = $sformatf("Memshire Configuration Sequence for S%0d", shire_id);
            `evl_log(UVM_LOW, VMOD_TEST, "MS-Config", $sformatf("creating \"%s\"", seq_name))

            test_seq         = new($sformatf("MS_Config-%0d", shire_id), agent, shire_id);
            sequence_wrapper = test_seq.get_wrapper();
            test_seq.set_abstract_name(seq_name);
            sequence_wrapper.set_abstract_name(seq_name);
            agent.add_sequence_wrapper(sequence_wrapper);
            if (`et_test_plusargs("no_wait_for_config_done") == 0) begin
               m_config_sequences[shire_id] = test_seq;
            end
         end
         m_do_ms_config = 1'b1;
      end

      if (m_do_ddrc_init == 1'b1) begin
         common_tcl_setup(1'b0, 1'b1);
      end
      else if (m_do_pll_init == 1'b1) begin
         common_tcl_setup(1'b1, 1'b0);
      end
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_ms_config_test");
      super.new(name_in);
   endfunction

endclass : evl_ms_config_test
