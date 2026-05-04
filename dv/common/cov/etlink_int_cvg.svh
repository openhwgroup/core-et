`ifndef __ETLINK_INT_CVG
`define __ETLINK_INT_CVG

   wire [4:0] req_info_source_tmp;
   wire [4:0] rsp_info_dest_tmp;
   wire [3:0] req_info_source;
   wire [3:0] rsp_info_dest;

   assign req_info_source_tmp = { {(5-$bits(req_info.source)){1'b0}}, req_info.source };
   assign rsp_info_dest_tmp   = { {(5-$bits(rsp_info.dest)){1'b0}}, rsp_info.dest };
   assign req_info_source = req_info_source_tmp[3:0];
   assign rsp_info_dest   = rsp_info_dest_tmp[3:0];

   covergroup etlink_cg () @(posedge m_clock);

      option.name = "etlink_rqchnsig_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;


      req_id_cvg: coverpoint req_info.id iff (req_select_mon != 0);

      rsp_id_cvg: coverpoint rsp_info.id iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1));


      req_opcode_cvg: coverpoint req_info.opcode iff (req_select_mon != 0) {
         bins rq_read         = {5'b00000};
         bins rq_write        = {5'b00001};
         bins rq_write_around = {5'b00010};
         bins rq_read_coop    = {5'b00011};
         bins msg_send_data   = {5'b00100};
         bins atomic          = {5'b00101};
         bins flush           = {5'b01000};
         bins flush_to_mem    = {5'b01001};
         bins evict           = {5'b01010};
         bins evict_to_mem    = {5'b01011};
         bins lock            = {5'b01100};
         bins unlock          = {5'b01101};
         bins scp_fill        = {5'b01110};
         bins prefetch        = {5'b01111};
         bins other           = default;
      }

      rsp_opcode_cvg: coverpoint rsp_info.opcode iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) {
         bins rsp_ack          = {2'b00};
         bins rsp_ack_data     = {2'b01};
         bins rsp_msg_rcv_data = {2'b10};
         bins rsp_err          = {2'b11};
         bins other            = default;
      }

      req_source_cvg: coverpoint req_info_source iff ((req_select_mon != 0) && ($bits(req_info.source) === 4)) {
         bins minion_0    = {4'b0000};
         bins minion_1    = {4'b0001};
         bins minion_2    = {4'b0010};
         bins minion_3    = {4'b0011};
         bins minion_4    = {4'b0100};
         bins minion_5    = {4'b0101};
         bins minion_6    = {4'b0110};
         bins minion_7    = {4'b0111};
         bins icache_0    = {4'b1000};
         bins ptw_0       = {4'b1001};
         bins ptw_1       = {4'b1010};
         bins tbox        = {4'b1011};
         bins cooperative = {4'b1100};
         bins other       = default;
      }


      rsp_dest_cvg: coverpoint rsp_info_dest iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1) && (rsp_info.opcode[1:0] !== 2'b10) && ($bits(rsp_info.dest) == 5)) {
         bins minion_0    = {4'b0000};
         bins minion_1    = {4'b0001};
         bins minion_2    = {4'b0010};
         bins minion_3    = {4'b0011};
         bins minion_4    = {4'b0100};
         bins minion_5    = {4'b0101};
         bins minion_6    = {4'b0110};
         bins minion_7    = {4'b0111};
         bins icache_0    = {4'b1000};
         bins ptw_0       = {4'b1001};
         bins ptw_1       = {4'b1010};
         bins tbox        = {4'b1011};
         bins cooperative = {4'b1100};
         bins other       = default;
      }


      rsp_dest_msg_cvg: coverpoint rsp_info_dest iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1) && (rsp_info.opcode[1:0] === 2'b10) && ($bits(rsp_info.dest) == 5)) {
         bins minion_0_thread_0 = {4'b0000};
         bins minion_1_thread_0 = {4'b0001};
         bins minion_2_thread_0 = {4'b0010};
         bins minion_3_thread_0 = {4'b0011};
         bins minion_4_thread_0 = {4'b0100};
         bins minion_5_thread_0 = {4'b0101};
         bins minion_6_thread_0 = {4'b0110};
         bins minion_7_thread_0 = {4'b0111};
         bins minion_0_thread_1 = {4'b1000};
         bins minion_1_thread_1 = {4'b1001};
         bins minion_2_thread_1 = {4'b1010};
         bins minion_3_thread_1 = {4'b1011};
         bins minion_4_thread_1 = {4'b1100};
         bins minion_5_thread_1 = {4'b1101};
         bins minion_6_thread_1 = {4'b1110};
         bins minion_7_thread_1 = {4'b1111};
         bins other             = default;
      }


      req_wdata_cvg: coverpoint req_info.wdata iff (req_select_mon != 0);

      rsp_msg_id_cvg: coverpoint rsp_info.id iff (((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) && (rsp_info.opcode === 2'b10)) {
         bins Regular_Port    = {8'b00000000};
         bins Tbox_Reply_Port = {8'b10000000};
         bins Reduce_Ready    = {8'b11000000};
         bins Reduce_Data     = {8'b11000001};
         bins Tbox_Pull       = {8'b11000010};
         bins b1              = {8'b00000001};
         bins b2              = {8'b00000010};
         bins b3              = {8'b00000011};
         bins b4              = {8'b00000100};
         bins b5              = {8'b00000101};
         bins b6              = {8'b00000110};
         bins b7              = {8'b00000111};
         bins b8              = {8'b00001000};
         bins b9              = {8'b00001001};
         bins b10             = {8'b00001010};
         bins b11             = {8'b00001011};
         bins b12             = {8'b00001100};
         bins b13             = {8'b00001101};
         bins b14             = {8'b00001110};
         bins b15             = {8'b00001111};
         bins b16             = {8'b00010000};
         bins b17             = {8'b00010001};
         bins b18             = {8'b00010010};
         bins b19             = {8'b00010011};
         bins b20             = {8'b00010100};
         bins b21             = {8'b00010101};
         bins b22             = {8'b00010110};
         bins b23             = {8'b00010111};
         bins b24             = {8'b00011000};
         bins b25             = {8'b00011001};
         bins b26             = {8'b00011010};
         bins b27             = {8'b00011011};
         bins b28             = {8'b00011100};
         bins b29             = {8'b00011101};
         bins b30             = {8'b00011110};
         bins b31             = {8'b00011111};
         bins other           = default;

      }

      req_miss_id_cvg: coverpoint req_info.id iff ((req_select_mon != 0) && ($bits(req_info.id) == 5)) {
         bins Tensor_Load_0     = {5'b00000};
         bins Tensor_Load_1     = {5'b00001};
         bins Miss_Handler_0    = {5'b00010};
         bins Miss_Handler_1    = {5'b00011};
         bins Tbox              = {5'b00100};
         bins Reduce            = {5'b00101};
         bins Cache_Ops         = {5'b00110};
         bins WriteBack         = {5'b00111};
         bins Miss_Handler_UC_0 = {5'b01010};
         bins Miss_Handler_UC_1 = {5'b01011};
         bins Prefetch_0        = {5'b01100};
         bins Prefetch_1        = {5'b01101};
         bins Prefetch_2        = {5'b01110};
         bins Prefetch_3        = {5'b01111};
         bins Tensor_Load_Ext_0 = {5'b10000};
         bins Tensor_Load_Ext_1 = {5'b10001};
         bins Tensor_Load_Ext_2 = {5'b10010};
         bins Tensor_Load_Ext_3 = {5'b10011};
         bins Tensor_Load_Ext_4 = {5'b10100};
         bins Tensor_Load_Ext_5 = {5'b10101};
         bins Tensor_Load_Ext_6 = {5'b10110};
         bins Tensor_Load_Ext_7 = {5'b10111};
         bins other          = default;

      }

      rsp_wdata_cvg: coverpoint rsp_info.wdata iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1));


      req_address_cvg: coverpoint req_info.address iff (req_select_mon != 0);


      req_data_cvg: coverpoint req_info.data iff (req_select_mon != 0);


      rsp_data_cvg: coverpoint rsp_info.data iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1));


      req_size_cvg: coverpoint req_info.size iff (req_select_mon != 0) {
         bins B1_8b    = {3'b000};
         bins B2_16b   = {3'b001};
         bins B4_32b   = {3'b010};
         bins B8_64b   = {3'b011};
         bins B16_128b = {3'b100};
         bins B32_256b = {3'b101};
         bins B64_512b = {3'b110};
         bins other    = default;
      }


      rsp_size_cvg: coverpoint rsp_info.size iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) {
         bins B1_8b    = {3'b000};
         bins B2_16b   = {3'b001};
         bins B4_32b   = {3'b010};
         bins B8_64b   = {3'b011};
         bins B16_128b = {3'b100};
         bins B32_256b = {3'b101};
         bins B64_512b = {3'b110};
         bins other    = default;
      }


      req_subopcode_cvg: coverpoint req_info.subopcode iff (req_select_mon != 0);

      req_qwen_cvg: coverpoint req_info.qwen iff (req_select_mon != 0) {
//       wildcard bins lane0 = {4'b???1};
//       wildcard bins lane1 = {4'b??1?};
//       wildcard bins lane2 = {4'b?1??};
//       wildcard bins lane3 = {4'b1???};
         bins no_lanes     = {4'b0000};
         bins lane_0       = {4'b0001};
         bins lane_1       = {4'b0010};
         bins lane_2       = {4'b0100};
         bins lane_3       = {4'b1000};
         bins lane_0_1     = {4'b0011};
         bins lane_0_2     = {4'b0101};
         bins lane_0_3     = {4'b1001};
         bins lane_1_2     = {4'b0110};
         bins lane_1_3     = {4'b1010};
         bins lane_2_3     = {4'b1100};
         bins lane_0_1_2   = {4'b0111};
         bins lane_0_1_3   = {4'b1011};
         bins lane_0_2_3   = {4'b1101};
         bins lane_1_2_3   = {4'b1110};
         bins lane_0_1_2_3 = {4'b1111};
         bins other        = default;
      }

      rsp_qwen_cvg: coverpoint rsp_info.qwen iff ((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) {
//       wildcard bins lane0 = {4'b???1};
//       wildcard bins lane1 = {4'b??1?};
//       wildcard bins lane2 = {4'b?1??};
//       wildcard bins lane3 = {4'b1???};
         bins no_lanes     = {4'b0000};
         bins lane_0       = {4'b0001};
         bins lane_1       = {4'b0010};
         bins lane_2       = {4'b0100};
         bins lane_3       = {4'b1000};
         bins lane_0_1     = {4'b0011};
         bins lane_0_2     = {4'b0101};
         bins lane_0_3     = {4'b1001};
         bins lane_1_2     = {4'b0110};
         bins lane_1_3     = {4'b1010};
         bins lane_2_3     = {4'b1100};
         bins lane_0_1_2   = {4'b0111};
         bins lane_0_1_3   = {4'b1011};
         bins lane_0_2_3   = {4'b1101};
         bins lane_1_2_3   = {4'b1110};
         bins lane_0_1_2_3 = {4'b1111};
         bins other        = default;
      }


      req_atomic_subopcode_cvg: coverpoint req_info.subopcode iff ((req_select_mon != 0) && (req_info.opcode == 5'b00101)) {
         wildcard bins atomic_dest_L2   = {7'b0xxxxxx};
         wildcard bins atomic_dest_L3   = {7'b1xxxxxx};
         wildcard bins atomic_size_32b  = {7'bx0xxxxx};
         wildcard bins atomic_size_64b  = {7'bx1xxxxx};
         wildcard bins atomic_scalar    = {7'bxx0xxxx};
         wildcard bins atomic_vector    = {7'bxx1xxxx};
         wildcard bins atomic_amoswap   = {7'bxx00000};
         wildcard bins atomic_amoadd    = {7'bxx00001};
         wildcard bins atomic_amoxor    = {7'bxx00010};
         wildcard bins atomic_amoand    = {7'bxx00011};
         wildcard bins atomic_amoor     = {7'bxx00100};
         wildcard bins atomic_amomin    = {7'bxx00101};
         wildcard bins atomic_amomax    = {7'bxx00110};
         wildcard bins atomic_amominu   = {7'bxx00111};
         wildcard bins atomic_amomaxu   = {7'bxx01000};
         wildcard bins atomic_famoswap  = {7'bxx10000};
         wildcard bins atomic_famoadd   = {7'bxx10001};
         wildcard bins atomic_famoxor   = {7'bxx10010};
         wildcard bins atomic_famoand   = {7'bxx10011};
         wildcard bins atomic_famoor    = {7'bxx10100};
         wildcard bins atomic_famomin   = {7'bxx10101};
         wildcard bins atomic_famomax   = {7'bxx10110};
         wildcard bins atomic_famominu  = {7'bxx10111};
         wildcard bins atomic_famomaxu  = {7'bxx11000};
         wildcard bins atomic_famominps = {7'bxx11001};
         wildcard bins atomic_famomaxps = {7'bxx11010};
         bins other                     = default;
      }

   endgroup

   etlink_cg etlink_cg_inst = new();

`endif
