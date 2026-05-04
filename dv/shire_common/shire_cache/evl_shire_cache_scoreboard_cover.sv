//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_scoreboard_cover
//
class evl_shire_cache_scoreboard_cover extends evl_void;

   bit cache_data_zero;
   bit wr_data_zero;
   bit fill_data_zero;
   bit zero_state_enable;

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_cache_wr
   //
   covergroup cg_sc_cache_wr();
      option.per_instance = 1;
      coverpoint cache_data_zero;
      coverpoint wr_data_zero;
      coverpoint zero_state_enable;
      cache_wr_cross: cross cache_data_zero,wr_data_zero,zero_state_enable;
   endgroup


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_cache_fill
   //
   covergroup cg_sc_cache_fill();
      option.per_instance = 1;
      coverpoint cache_data_zero;
      coverpoint fill_data_zero;
      coverpoint zero_state_enable;
      cache_fill_cross: cross cache_data_zero, fill_data_zero, zero_state_enable;
   endgroup


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_cache_rd
   //
   covergroup cg_sc_cache_rd();
      option.per_instance = 1;
      coverpoint cache_data_zero;
      coverpoint zero_state_enable;
      cache_rd_cross: cross cache_data_zero, zero_state_enable;
   endgroup


   evl_atomic_op_t atomic_op;
   bit atomic_sc_vec;
   bit atomic_size;
   bit [((`EVL_LINE_ADDR_LSB-1)-`EVL_WORD_ADDR_LSB):0] mem_offset_32b;
   bit [((`EVL_LINE_ADDR_LSB-1)-`EVL_DWORD_ADDR_LSB):0] mem_offset_64b;
   bit                                                  mem_offset_oword;
   bit [2:0] vector_offset;
   bit req_val_is_snan;
   bit mem_val_is_snan;
   bit req_val_is_qnan;
   bit mem_val_is_qnan;
   bit req_val_is_zero_plus;
   bit mem_val_is_zero_plus;
   bit req_val_is_zero_minus;
   bit mem_val_is_zero_minus;

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_atomics
   //
   covergroup cg_sc_atomics();
      option.per_instance = 1;
      coverpoint atomic_op{ ignore_bins unused_ops = {SC_AMO_0C,SC_AMO_0D,SC_AMO_0E,SC_AMO_0F};}
      coverpoint atomic_sc_vec;
      coverpoint atomic_size;
      atomic_conf_cross: cross atomic_op, atomic_sc_vec, atomic_size {
         ignore_bins no_64b_vec = binsof(atomic_sc_vec) intersect {1} && binsof(atomic_size) intersect {1};
                                                                      }
   endgroup // cg_sc_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_scalar_32b_fp_atomics
   //
   covergroup cg_sc_scalar_32b_fp_atomics();
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
   endgroup // cg_sc_scalar_32b_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_scalar_64b_fp_atomics
   //
   covergroup cg_sc_scalar_64b_fp_atomics();
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
   endgroup // cg_sc_scalar_64b_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_vector_fp_atomics
   //
   covergroup cg_sc_vector_fp_atomics();
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
   endgroup // cg_sc_vector_fp_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_messages
   //
   evl_bus_req_size_t    msg_req_size;
   evl_neighborhood_id_t msg_src_port;
   evl_neighborhood_id_t msg_dst_port;
   int                   msg_num_same_src_dst;

   covergroup cg_sc_messages();
      // this one is not per instance.
      coverpoint msg_req_size{
         bins valid[] = {[0:6]};
      }
      coverpoint msg_src_port{
         bins valid[] = {[0:7]};
      }
      coverpoint msg_dst_port{
         bins valid[] = {[0:7]};
      }
      msg_src_x_dst: cross msg_src_port,msg_dst_port;
      coverpoint msg_num_same_src_dst{
         bins valid[] = {[1:20]};
      }
   endgroup // cg_sc_messages


   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sc_remote_shire_id
   //
   evl_shire_id_t remote_shire_id;
   bit            remote_enable;

   covergroup cg_sc_remote_shire_id();
      coverpoint remote_enable;
      coverpoint remote_shire_id{
         bins valid = {[0:31]};
      }
      remote_shire_id_x_enable: cross remote_enable, remote_shire_id;

   endgroup // cg_sc_messages


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_cache_wr
   //
   function void sample_sc_cache_wr(input evl_line_data line_data_in,
                                    input evl_line_data wr_data_in,
                                    input bit           zero_st_en_in);
      zero_state_enable = zero_st_en_in;
      cache_data_zero   = line_data_in.data_is_all_zero();
      wr_data_zero      = wr_data_in.data_is_all_zero();
      cg_sc_cache_wr.sample();
   endfunction : sample_sc_cache_wr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_cache_fill
   //
   function void sample_sc_cache_fill(input evl_line_data line_data_in,
                                      input evl_line_data fill_data_in,
                                      input bit           zero_st_en_in);
      if (fill_data_in != null) begin
         zero_state_enable = zero_st_en_in;
         cache_data_zero   = line_data_in.data_is_all_zero();
         fill_data_zero    = fill_data_in.data_is_all_zero();
         cg_sc_cache_fill.sample();
      end
   endfunction : sample_sc_cache_fill


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_cache_rd
   //
   function void sample_sc_cache_rd(input evl_line_data line_data_in,
                                    input bit           zero_st_en_in);
      zero_state_enable = zero_st_en_in;
      cache_data_zero   = line_data_in.data_is_all_zero();
      cg_sc_cache_rd.sample();
   endfunction : sample_sc_cache_rd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_atomics
   //
   function void sample_sc_atomics(input evl_req_desc  req_desc_in,
                                   input evl_line_desc line_desc_in);
      evl_bus_atomic_conf_t atomic_conf;
      evl_paddr_t           addr;

      atomic_conf   = req_desc_in.get_bus_atomic_conf();
      atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);

      cg_sc_atomics.sample();

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
               cg_sc_vector_fp_atomics.sample();
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
               cg_sc_scalar_32b_fp_atomics.sample();
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
               cg_sc_scalar_64b_fp_atomics.sample();
            end
         end // else: !if(atomic_sc_vec == 1)
      end // if (atomic_op inside {SC_AMOMAX_PS,SC_AMOMIN_PS})
   endfunction : sample_sc_atomics


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_messages
   //
   function void sample_sc_messages(input evl_bus_req_size_t    req_size_in,
                                    input evl_neighborhood_id_t src_port_in,
                                    input evl_neighborhood_id_t dst_port_in,
                                    input int                   num_same_src_dst_in);
      msg_req_size = req_size_in;
      msg_src_port = src_port_in;
      msg_dst_port = dst_port_in;
      msg_num_same_src_dst = num_same_src_dst_in;
      cg_sc_messages.sample();
   endfunction : sample_sc_messages


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sc_remote_shire_id
   //
   function void sample_sc_remote_shire_id(input evl_shire_id_t shire_id_in, input bit remote_enable_in);
      remote_shire_id = shire_id_in;
      remote_enable   = remote_enable_in;
      cg_sc_remote_shire_id.sample();
   endfunction : sample_sc_remote_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      cg_sc_cache_wr = new();
      cg_sc_cache_fill = new();
      cg_sc_cache_rd = new();
      cg_sc_atomics = new();
      cg_sc_scalar_32b_fp_atomics = new();
      cg_sc_scalar_64b_fp_atomics = new();
      cg_sc_vector_fp_atomics = new();
      cg_sc_messages = new();
      cg_sc_remote_shire_id = new();
   endfunction : new

endclass : evl_shire_cache_scoreboard_cover
