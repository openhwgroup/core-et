//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_scoreboard_cover
//
class evl_memshire_scoreboard_cover extends evl_void;
   evl_atomic_op_t                                      atomic_op;
   bit                                                  atomic_sc_vec;
   bit                                                  atomic_size;
   bit [((`EVL_LINE_ADDR_LSB-1)-`EVL_WORD_ADDR_LSB):0]  mem_offset_32b;
   bit [((`EVL_LINE_ADDR_LSB-1)-`EVL_DWORD_ADDR_LSB):0] mem_offset_64b;
   bit                                                  mem_offset_oword;
   bit [2:0]                                            vector_offset;
   bit                                                  req_val_is_snan;
   bit                                                  mem_val_is_snan;
   bit                                                  req_val_is_qnan;
   bit                                                  mem_val_is_qnan;
   bit                                                  req_val_is_zero_plus;
   bit                                                  mem_val_is_zero_plus;
   bit                                                  req_val_is_zero_minus;
   bit                                                  mem_val_is_zero_minus;
   bit [3:0]                                            mem_freq;

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_ms_frequency
   //
   covergroup cg_ms_frequency();
      option.per_instance = 0;
      coverpoint mem_freq {
         bins        freq_333mhz  = {0};
         bins        freq_400mhz  = {1};
         bins        freq_498mhz  = {2};
         bins        freq_500mhz  = {3};
         bins        freq_502mhz  = {4};
         bins        freq_600mhz  = {5};
         bins        freq_750mhz  = {6};
         bins        freq_800mhz  = {7};
         bins        freq_933mhz  = {8};
         bins        freq_1000mhz = {9};
         bins        freq_1010mhz = {10};
         bins        freq_1066mhz = {11};
         ignore_bins unused_ops   = {12, 13, 14, 15};
      }
   endgroup // cg_ms_frequency


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_ms_atomics
   //
   covergroup cg_ms_atomics();
      option.per_instance = 1;
      coverpoint atomic_op{ ignore_bins unused_ops = {SC_AMO_0C,SC_AMO_0D,SC_AMO_0E,SC_AMO_0F};}
      coverpoint atomic_sc_vec;
      coverpoint atomic_size;
      atomic_conf_cross: cross atomic_op, atomic_sc_vec, atomic_size {
         ignore_bins no_64b_vec = binsof(atomic_sc_vec) intersect {1} && binsof(atomic_size) intersect {1};
      }
   endgroup // cg_ms_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_ms_scalar_32b_fp_atomics
   //
   covergroup cg_ms_scalar_32b_fp_atomics();
      option.per_instance = 1;
      mem_offset: coverpoint mem_offset_32b;

      // Cover that interesting types of data were accessed at different memory offsets
      mem_offset_x_nan: cross mem_offset, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      mem_offset_x_zero_plus: cross mem_offset, mem_val_is_zero_plus;
      mem_offset_x_zero_minus: cross mem_offset, mem_val_is_zero_plus;

      // Cover combinations of data types in in the request operand and the memory operand
      req_x_mem_nan: cross req_val_is_snan, req_val_is_qnan, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid_req = binsof(req_val_is_snan) intersect {1} && binsof(req_val_is_qnan) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      req_x_mem_zero: cross req_val_is_zero_plus, req_val_is_zero_minus, mem_val_is_zero_plus, mem_val_is_zero_minus{
         ignore_bins invalid_req = binsof(req_val_is_zero_plus) intersect {1} && binsof(req_val_is_zero_minus) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_zero_plus) intersect {1} && binsof(mem_val_is_zero_minus) intersect {1};
      }
   endgroup // cg_ms_scalar_32b_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_ms_scalar_64b_fp_atomics
   //
   covergroup cg_ms_scalar_64b_fp_atomics();
      option.per_instance = 1;
      mem_offset: coverpoint mem_offset_64b;

      // Cover that interesting types of data were accessed at different memory offsets
      mem_offset_x_nan: cross mem_offset, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      mem_offset_x_zero_plus: cross mem_offset, mem_val_is_zero_plus;
      mem_offset_x_zero_minus: cross mem_offset, mem_val_is_zero_plus;

      // Cover combinations of data types in in the request operand and the memory operand
      req_x_mem_nan: cross req_val_is_snan, req_val_is_qnan, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid_req = binsof(req_val_is_snan) intersect {1} && binsof(req_val_is_qnan) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      req_x_mem_zero: cross req_val_is_zero_plus, req_val_is_zero_minus, mem_val_is_zero_plus, mem_val_is_zero_minus{
         ignore_bins invalid_req = binsof(req_val_is_zero_plus) intersect {1} && binsof(req_val_is_zero_minus) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_zero_plus) intersect {1} && binsof(mem_val_is_zero_minus) intersect {1};
      }
   endgroup // cg_ms_scalar_64b_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_ms_vector_fp_atomics
   //
   covergroup cg_ms_vector_fp_atomics();
      option.per_instance = 1;
      mem_offset: coverpoint mem_offset_oword;
      coverpoint vector_offset;

      // Cover that interesting types of data were accessed at different memory offsets
      mem_offsets_x_nan: cross mem_offset, vector_offset, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      mem_offsets_x_zero_plus: cross mem_offset, vector_offset, mem_val_is_zero_plus;
      mem_offsets_x_zero_minus: cross mem_offset, vector_offset, mem_val_is_zero_plus;

      // Cover that interesting type of data were accessed at different req offsets
      req_offset_x_nan: cross vector_offset, req_val_is_snan, req_val_is_qnan{
         ignore_bins invalid = binsof(req_val_is_snan) intersect {1} && binsof(req_val_is_qnan) intersect {1};
      }
      req_offset_x_zero_plus: cross vector_offset, req_val_is_zero_plus;
      req_offset_x_zero_minus: cross vector_offset, req_val_is_zero_minus;

      // Cover combinations of data types in in the request operand and the memory operand
      req_x_mem_nan: cross req_val_is_snan, req_val_is_qnan, mem_val_is_snan, mem_val_is_qnan{
         ignore_bins invalid_req = binsof(req_val_is_snan) intersect {1} && binsof(req_val_is_qnan) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_snan) intersect {1} && binsof(mem_val_is_qnan) intersect {1};
      }
      req_x_mem_zero: cross req_val_is_zero_plus, req_val_is_zero_minus, mem_val_is_zero_plus, mem_val_is_zero_minus{
         ignore_bins invalid_req = binsof(req_val_is_zero_plus) intersect {1} && binsof(req_val_is_zero_minus) intersect {1};
         ignore_bins invalid_mem = binsof(mem_val_is_zero_plus) intersect {1} && binsof(mem_val_is_zero_minus) intersect {1};
      }
   endgroup // cg_ms_vector_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ms_atomics
   //
   function void sample_ms_atomics(input evl_req_desc req_desc_in, input evl_line_desc line_desc_in);
      evl_bus_atomic_conf_t atomic_conf;
      evl_paddr_t addr;
      atomic_conf = req_desc_in.get_bus_atomic_conf();
      atomic_op = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      atomic_size = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);

      cg_ms_atomics.sample();

      // More coverage on floating point operand values
      if (atomic_op inside {SC_AMOMAX_PS,SC_AMOMIN_PS}) begin
         evl_dword_t req_operand;
         evl_dword_t mem_operand;

         addr        = req_desc_in.get_bus_atomic_offset();
         if (atomic_sc_vec == 1) begin //vector atomics
            mem_offset_oword = addr[`EVL_LINE_ADDR_LSB-1];
            for(int ii=0; ii<8; ii++) begin
               vector_offset = ii;

               mem_operand = evl_dword_t'(line_desc_in.get_word((mem_offset_oword << (`EVL_LINE_ADDR_LSB-1)) + ii));
               mem_val_is_snan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b0) && (mem_operand[21:0] != 0));
               mem_val_is_qnan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b1));
               mem_val_is_zero_plus = (mem_operand == 64'h0000000000000000);
               mem_val_is_zero_minus = (mem_operand == 64'h0000000080000000);

               req_operand = evl_dword_t'(req_desc_in.get_req_word(ii));
               req_val_is_snan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b0) && (req_operand[21:0] != 0));
               req_val_is_qnan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b1));
               req_val_is_zero_plus = (req_operand == 64'h0000000000000000);
               req_val_is_zero_minus = (req_operand == 64'h0000000080000000);
               cg_ms_vector_fp_atomics.sample();
            end
         end
         else begin //scalar atomics
            if (atomic_size == 1'b0) begin //32b scalar
               mem_offset_32b = addr[`EVL_LINE_ADDR_LSB-1:`EVL_WORD_ADDR_LSB];
               req_operand = evl_dword_t'(req_desc_in.get_req_word(0));
               mem_operand = evl_dword_t'(line_desc_in.get_word(mem_offset_32b));
               req_val_is_snan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b0) && (req_operand[21:0] != 0));
               mem_val_is_snan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b0) && (mem_operand[21:0] != 0));
               req_val_is_qnan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b1));
               mem_val_is_qnan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b1));
               req_val_is_zero_plus = (req_operand == 64'h0000000000000000);
               mem_val_is_zero_plus = (mem_operand == 64'h0000000000000000);
               req_val_is_zero_minus = (req_operand == 64'h0000000080000000);
               mem_val_is_zero_minus = (mem_operand == 64'h0000000080000000);
               cg_ms_scalar_32b_fp_atomics.sample();
            end
            else begin //64b scalar
               mem_offset_64b = addr[`EVL_LINE_ADDR_LSB-1:`EVL_DWORD_ADDR_LSB];
               req_operand = req_desc_in.get_req_dword(0);
               mem_operand = line_desc_in.get_dword(mem_offset_64b);
               req_val_is_snan = ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b0) && (req_operand[50:0] != 0));
               mem_val_is_snan = ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b0) && (mem_operand[50:0] != 0));
               req_val_is_qnan = ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b1));
               mem_val_is_qnan = ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b1));
               req_val_is_zero_plus = (req_operand == 64'h0000000000000000);
               mem_val_is_zero_plus = (mem_operand == 64'h0000000000000000);
               req_val_is_zero_minus = (req_operand == 64'h8000000000000000);
               mem_val_is_zero_minus = (mem_operand == 64'h8000000000000000);
               cg_ms_scalar_64b_fp_atomics.sample();
            end
         end // else: !if(atomic_sc_vec == 1)
      end // if (atomic_op inside {SC_AMOMAX_PS,SC_AMOMIN_PS})
   endfunction : sample_ms_atomics


   evl_memshire_reg_block m_regs;
   bit is_debug;
   bit [1:0] esr_mc_en;
   bit [5:0] esr_mc_bit_sel;
   bit [5:0] esr_addr_remove_bit_sel3;
   bit [5:0] esr_addr_remove_bit_sel2;
   bit [5:0] esr_addr_remove_bit_sel1;
   bit [5:0] esr_addr_remove_bit_sel0;
   bit [5:0] esr_debug_mc_bit_sel;
   bit [11:0] esr_addr_mask;
   bit        bit_sel_eq_remove_bit_0;
   bit        bit_sel_eq_remove_bit_1;
   bit        bit_sel_eq_remove_bit_2;
   bit        bit_sel_eq_remove_bit_3;
   bit [2:0]  num_addr_remove_bits;

   covergroup cg_ms_predict_downstream_request();
      coverpoint esr_mc_en { ignore_bins neither = {2'b00};}
      coverpoint esr_mc_bit_sel iff(is_debug == 0){
         bins valid[] = {[6:39]};
      }

      coverpoint esr_addr_remove_bit_sel0 iff(is_debug == 0){
         bins valid[] = {[6:39],63};
      }
      coverpoint esr_addr_remove_bit_sel1 iff(is_debug == 0){
         bins valid[] = {[7:39],63};
      }
      coverpoint esr_addr_remove_bit_sel2 iff(is_debug == 0){
         bins valid[] = {[8:39],63};
      }
      coverpoint esr_addr_remove_bit_sel3 iff(is_debug == 0){
         bins valid[] = {[9:39],63};
      }

      coverpoint num_addr_remove_bits{
         bins valid[] = {[0:4]};
      }

      coverpoint esr_debug_mc_bit_sel iff(is_debug == 1){
         bins valid[] = {[6:39]};
      }

      coverpoint esr_addr_mask{ bins valid[] = {12'h000, 12'h001, 12'h003, 12'h007, 12'h00f,
                                                12'h01f, 12'h03f, 12'h07f, 12'h0ff,
                                                12'h1ff, 12'h3ff, 12'h7ff, 12'hfff};}
      coverpoint bit_sel_eq_remove_bit_0{bins valid[] = {1'b1};}
      coverpoint bit_sel_eq_remove_bit_1{bins valid[] = {1'b1};}
      coverpoint bit_sel_eq_remove_bit_2{bins valid[] = {1'b1};}
      coverpoint bit_sel_eq_remove_bit_3{bins valid[] = {1'b1};}
   endgroup // cg_ms_predict_downstream_request


   function void sample_predict_downstream_request(input evl_memshire_reg_block m_regs_in, input evl_req_desc req_desc_in, input bit is_debug_in);
      m_regs = m_regs_in;
      is_debug = is_debug_in;
      esr_mc_en = check_reg_field_value(ESR_MS_MC_EN);
      esr_mc_bit_sel = check_reg_field_value(ESR_MS_ADDR_MC_BIT_SEL);
      esr_addr_remove_bit_sel3 = check_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL3);
      esr_addr_remove_bit_sel2 = check_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL2);
      esr_addr_remove_bit_sel1 = check_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL1);
      esr_addr_remove_bit_sel0 = check_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL0);
      esr_debug_mc_bit_sel = check_reg_field_value(ESR_MS_ADDR_DEBUG_MC_BIT_SEL);
      esr_addr_mask = check_reg_field_value(ESR_MS_ADDR_MASK);

      bit_sel_eq_remove_bit_0 = (esr_mc_bit_sel == esr_addr_remove_bit_sel0) ? 1'b1 : 1'b0;
      bit_sel_eq_remove_bit_1 = (esr_mc_bit_sel == esr_addr_remove_bit_sel1) ? 1'b1 : 1'b0;
      bit_sel_eq_remove_bit_2 = (esr_mc_bit_sel == esr_addr_remove_bit_sel2) ? 1'b1 : 1'b0;
      bit_sel_eq_remove_bit_3 = (esr_mc_bit_sel == esr_addr_remove_bit_sel3) ? 1'b1 : 1'b0;

      if ((esr_addr_remove_bit_sel0 == 63) && (esr_addr_remove_bit_sel1 == 63) && (esr_addr_remove_bit_sel2 == 63) && (esr_addr_remove_bit_sel3 == 63)) begin
         num_addr_remove_bits = 0;
      end
      else if ((esr_addr_remove_bit_sel0 != 63) && (esr_addr_remove_bit_sel1 == 63) && (esr_addr_remove_bit_sel2 == 63) && (esr_addr_remove_bit_sel3 == 63)) begin
         num_addr_remove_bits = 1;
      end
      else if ((esr_addr_remove_bit_sel0 != 63) && (esr_addr_remove_bit_sel1 != 63) && (esr_addr_remove_bit_sel2 == 63) && (esr_addr_remove_bit_sel3 == 63)) begin
         num_addr_remove_bits = 2;
      end
      else if ((esr_addr_remove_bit_sel0 != 63) && (esr_addr_remove_bit_sel1 != 63) && (esr_addr_remove_bit_sel2 != 63) && (esr_addr_remove_bit_sel3 == 63)) begin
         num_addr_remove_bits = 3;
      end
      else if ((esr_addr_remove_bit_sel0 != 63) && (esr_addr_remove_bit_sel1 != 63) && (esr_addr_remove_bit_sel2 != 63) && (esr_addr_remove_bit_sel3 != 63)) begin
         num_addr_remove_bits = 4;
      end
      cg_ms_predict_downstream_request.sample();
   endfunction : sample_predict_downstream_request


   function evl_dword_t check_reg_field_value(input evl_memshire_reg_field_names_t reg_field);
      uvm_reg_field field;
      evl_dword_t   field_value;

      field = m_regs.get_field_by_name($sformatf("%0s", reg_field.name()));

      if (field == null) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: tried to lookup the value for field %0s, but the field was not found!", reg_field.name()))
         field_value = ~evl_dword_t'(0);
      end
      else begin
         field_value = field.get();
      end

      return field_value;
   endfunction : check_reg_field_value


   //-----------------------------------------------
   //
   // Atomic Cache Coverage
   //
   bit            atomic_cache_hit;
   evl_bus_cmd_t  atomic_cache_cmd;
   bit            atomic_cache_priority;

   covergroup cg_ms_atomic_cache();
      coverpoint atomic_cache_hit;
      coverpoint atomic_cache_cmd{
         bins read = {ET_LINK_REQ_Read};
         bins write = {ET_LINK_REQ_Write};
         bins atomic = {ET_LINK_REQ_Atomic};
      }
      coverpoint atomic_cache_priority;

      atomic_cache_cross: cross atomic_cache_hit, atomic_cache_cmd, atomic_cache_priority;
   endgroup // cg_ms_atomic_cache


   //-------------------------------------------
   //
   // Function: sample_ms_atomic_cache
   //
   function void sample_ms_atomic_cache(input bit atomic_cache_hit_in, input evl_bus_cmd_t atomic_cache_cmd_in, input bit atomic_cache_priority_in);
      atomic_cache_hit = atomic_cache_hit_in;
      atomic_cache_cmd = atomic_cache_cmd_in;
      atomic_cache_priority = atomic_cache_priority_in;
      cg_ms_atomic_cache.sample();
   endfunction : sample_ms_atomic_cache


   //-------------------------------------------
   //
   // Task: sample_ms_frequency_task
   //
   task sample_ms_frequency_task(input evl_rtl_port rtl_port);
      realtime start_time;
      realtime end_time;
      realtime time_diff_20_cyc;
      int      freq;

      rtl_port.wait_cycles(1);
      start_time = $realtime();
      rtl_port.wait_cycles(20);
      end_time = $realtime();
      time_diff_20_cyc = (20000.0 / (end_time - start_time));
      freq = int'(time_diff_20_cyc);
      if (freq inside { 333, 332, 334 }) begin
         mem_freq = 4'h0;
      end
      else if (freq inside { 399, 400, 401 }) begin
         mem_freq = 4'h1;
      end
      else if (freq inside { 497, 498 }) begin
         mem_freq = 4'h2;
      end
      else if (freq == 500) begin
         mem_freq = 4'h3;
      end
      else if (freq inside { 502, 503 }) begin
         mem_freq = 4'h4;
      end
      else if (freq inside { 599, 600, 601 }) begin
         mem_freq = 4'h5;
      end
      else if (freq inside { 749, 750, 751 }) begin
         mem_freq = 4'h6;
      end
      else if (freq inside { 799, 800, 801 }) begin
         mem_freq = 4'h7;
      end
      else if (freq inside { 932, 933, 934 }) begin
         mem_freq = 4'h8;
      end
      else if (freq inside { 999, 1000, 1001 }) begin
         mem_freq = 4'h9;
      end
      else if (freq inside { 1065, 1066, 1067 }) begin
         mem_freq = 4'ha;
      end
      else begin
         mem_freq = 4'hf;
      end
      cg_ms_frequency.sample();
   endtask : sample_ms_frequency_task


   //-------------------------------------------
   //
   // Function: sample_ms_frequency
   //
   function void sample_ms_frequency(input evl_rtl_port rtl_port);
      fork
         sample_ms_frequency_task(rtl_port);
      join_none
   endfunction : sample_ms_frequency


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      cg_ms_atomics = new();
      cg_ms_scalar_32b_fp_atomics = new();
      cg_ms_scalar_64b_fp_atomics = new();
      cg_ms_vector_fp_atomics = new();
      cg_ms_predict_downstream_request = new();
      cg_ms_atomic_cache = new();
      cg_ms_frequency = new();
   endfunction : new

endclass : evl_memshire_scoreboard_cover
