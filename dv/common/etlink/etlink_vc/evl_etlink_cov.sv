class evl_etlink_cov #(parameter etlink_cov_type_t ETLINK_COV_TYPE = EVL_COV_ETLINK_NONE);

   localparam int DATA_WIDTH = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 64 : 32;

   bit                     m_cover_data;

   int                     req_idle_count;
   int                     req_busy_count;
   evl_etlink_size_t       req_size;
   bit [7:0]               req_id;
   bit [4:0]               req_id_miss;
   bit [3:0]               req_source;
   evl_etlink_req_opcode_t req_opcode;
   bit [6:0]               req_subopcode;
   bit [3:0]               req_qwen;
   bit [3:0]               req_addr_7;
   bit [3:0]               req_addr_6;
   bit [3:0]               req_addr_5;
   bit [3:0]               req_addr_4;
   bit [3:0]               req_addr_3;
   bit [3:0]               req_addr_2;
   bit [3:0]               req_addr_1;
   bit [3:0]               req_addr_0;
   bit [1:0]               req_addr_edge_7;
   bit [1:0]               req_addr_edge_6;
   bit [1:0]               req_addr_edge_5;
   bit [1:0]               req_addr_edge_4;
   bit [1:0]               req_addr_edge_3;
   bit [1:0]               req_addr_edge_2;
   bit [1:0]               req_addr_edge_1;

   int                     rsp_idle_count;
   int                     rsp_busy_count;
   evl_etlink_size_t       rsp_size;
   bit [7:0]               rsp_id;
   bit [3:0]               rsp_dest;
   evl_etlink_rsp_opcode_t rsp_opcode;
   bit [3:0]               rsp_qwen;


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Requests
   //
   // This cover group is used to provide coverage for requests on the ET link interface.  It
   // consists of cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back requests (as
   //    well as some limited number of cycles between requests).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    request ready has been deasserted for some number of cycles.
   //
   // size:
   //    There is a cover point for all of the valid request sizes for this interface.
   //
   // addr:
   //    There are cover points for address bits.  Rather than getting coverage for all addresses,
   //    we group address bits and look for all combinations within a group (addr[3:0], addr[7:4],
   //    addr[11:8], addr[15:12], addr[19:16], addr[23:20], addr[27:24], addr[31:28] ... addr[39:32]
   //    are not captured).  In addition, the address "edges" between these groups are checked.
   //
   covergroup cg_etlink_req;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_req_valid_to_valid: coverpoint req_idle_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      cp_req_not_ready: coverpoint req_busy_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      cp_req_size: coverpoint req_size {
         bins sizes[] = { [0:6] };
      }

      cp_req_id_neigh_sc: coverpoint req_id {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         bins req_ids[] = { [0:255] };
      }

      cp_req_id_miss: coverpoint req_id_miss {
         bins        Tensor_Load_0     = { 0 };
         bins        Tensor_Load_1     = { 1 };
         bins        Miss_Handler_0    = { 2 };
         bins        Miss_Handler_1    = { 3 };
         bins        Tbox              = { 4 };
         bins        Reduce            = { 5 };
         bins        Cache_Ops         = { 6 };
         bins        WriteBack         = { 7 };
         bins        Miss_Handler_UC_0 = { 10 };
         bins        Miss_Handler_UC_1 = { 11 };
         bins        Prefetch_0        = { 12 };
         bins        Prefetch_1        = { 13 };
         bins        Prefetch_2        = { 14 };
         bins        Prefetch_3        = { 15 };
         bins        Tensor_Load_Ext_0 = { 16 };
         bins        Tensor_Load_Ext_1 = { 17 };
         bins        Tensor_Load_Ext_2 = { 18 };
         bins        Tensor_Load_Ext_3 = { 19 };
         bins        Tensor_Load_Ext_4 = { 20 };
         bins        Tensor_Load_Ext_5 = { 21 };
         bins        Tensor_Load_Ext_6 = { 22 };
         bins        Tensor_Load_Ext_7 = { 23 };
         ignore_bins ignore_values     = { [8:9], [24:31] };
      }

      cp_req_source_normal: coverpoint req_source iff ((req_opcode != ET_LINK_REQ_ReadCoop) && (req_opcode != ET_LINK_REQ_MsgSendData)) {
         bins        sources[]     = { [0:11] };
         ignore_bins ignore_values = { [12:15] };
      }

      cp_req_source_coop: coverpoint req_source iff (req_opcode == ET_LINK_REQ_ReadCoop) {
         bins        cooperative   = { 5'b01100 };
         ignore_bins ignore_values = { [0:11], [13:15] };
      }

      cp_req_source_msg: coverpoint req_source iff (req_opcode == ET_LINK_REQ_MsgSendData) {
         bins        sources[]     = { [0:15] };
      }

      cp_req_qwen_wr: coverpoint req_qwen iff (req_opcode inside { ET_LINK_REQ_Write }) {
         bins        qwens[]       = { 1, 2, 3, 4, 8, 12, 15 };
         ignore_bins ignore_values = { 0, [5:7], [9:11], [13:14] };
      }

      cp_req_qwen_wa: coverpoint req_qwen iff (req_opcode inside { ET_LINK_REQ_WriteAround }) {
         bins        qwens[]       = { [1:15] };
         ignore_bins ignore_values = { 0 };
      }

      cp_req_qwen_atomic: coverpoint req_qwen iff (req_opcode inside { ET_LINK_REQ_Atomic }) {
         bins        qwens[]       = { 1, 3 };
         ignore_bins ignore_values = { 0, 2, [4:15] };
      }

      cp_req_opcode_neigh_sc: coverpoint req_opcode {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         bins        write         = { ET_LINK_REQ_Write };
         bins        read          = { ET_LINK_REQ_Read };
         bins        writearound   = { ET_LINK_REQ_WriteAround };
         bins        readcoop      = { ET_LINK_REQ_ReadCoop };
         bins        msgsend       = { ET_LINK_REQ_MsgSendData };
         bins        atomic        = { ET_LINK_REQ_Atomic };
         bins        flush         = { ET_LINK_REQ_Flush };
         bins        evict         = { ET_LINK_REQ_Evict };
         bins        lock          = { ET_LINK_REQ_Lock };
         bins        unlock        = { ET_LINK_REQ_Unlock };
         bins        scpfill       = { ET_LINK_REQ_ScpFill };
         bins        prefetch      = { ET_LINK_REQ_Prefetch };
         ignore_bins ignore_values = { ET_LINK_REQ_ScpRead,
                                       ET_LINK_REQ_ScpWrite,
                                       ET_LINK_REQ_FlushToMem,
                                       ET_LINK_REQ_EvictToMem,
                                       ET_LINK_REQ_AtomicRsp };
      }

      cp_req_subopcode_mem: coverpoint req_subopcode iff (req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_Read }) {
         bins        subopcodes[]  = { [0:3] };
         ignore_bins ignore_values = { [4:127] };
      }

      cp_req_subopcode_wa: coverpoint req_subopcode iff (req_opcode inside { ET_LINK_REQ_WriteAround }) {
         bins        subopcodes[]  = { [0:2] };
         ignore_bins ignore_values = { [3:127] };
      }

      cp_req_subopcode_atomic: coverpoint req_subopcode iff (req_opcode inside { ET_LINK_REQ_Atomic }) {
         bins        subopcodes[]  = { [0:11],  [16:26], [32:43], [64:75], [80:90], [96:107]  };
         ignore_bins ignore_values = { [12:15], [25:31], [44:63], [76:79], [91:95], [108:127] };
      }

      cp_req_addr_7: coverpoint req_addr_7 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_6: coverpoint req_addr_6 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_5: coverpoint req_addr_5 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_4: coverpoint req_addr_4 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_3: coverpoint req_addr_3 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_2: coverpoint req_addr_2 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_1: coverpoint req_addr_1 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_0: coverpoint req_addr_0 {
         bins values[] = { [0:15] };
      }
      cp_req_addr_edge_7: coverpoint req_addr_edge_7 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_6: coverpoint req_addr_edge_6 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_5: coverpoint req_addr_edge_5 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_4: coverpoint req_addr_edge_4 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_3: coverpoint req_addr_edge_3 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_2: coverpoint req_addr_edge_2 {
         bins values[] = { [0:3] };
      }
      cp_req_addr_edge_1: coverpoint req_addr_edge_1 {
         bins values[] = { [0:3] };
      }

      cp_neigh_sc_cross_opcode_id: cross cp_req_id_miss, cp_req_opcode_neigh_sc iff (req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_Read, ET_LINK_REQ_Atomic }) {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         ignore_bins ignore_values = !binsof(cp_req_opcode_neigh_sc) intersect { ET_LINK_REQ_Write, ET_LINK_REQ_Read, ET_LINK_REQ_Atomic };
      }

      cp_neigh_sc_cross_opcode_source: cross cp_req_source_normal, cp_req_opcode_neigh_sc iff (!(req_opcode inside { ET_LINK_REQ_MsgSendData })) {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         ignore_bins ignore_values = binsof(cp_req_opcode_neigh_sc) intersect { ET_LINK_REQ_MsgSendData };
      }

      cp_neigh_sc_cross_opcode_size: cross cp_req_size, cp_req_opcode_neigh_sc iff (req_opcode inside { ET_LINK_REQ_Write, ET_LINK_REQ_Read, ET_LINK_REQ_ReadCoop }) {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         ignore_bins ignore_values = !binsof(cp_req_opcode_neigh_sc) intersect { ET_LINK_REQ_Write, ET_LINK_REQ_Read, ET_LINK_REQ_ReadCoop };
      }
   endgroup : cg_etlink_req


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Responses
   //
   // This cover group is used to provide coverage for responses on the ET link interface.  It
   // consists of cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back responses (as
   //    well as some limited number of cycles between responses).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    response ready has been deasserted for some number of cycles.
   //
   // size:
   //    There is a cover point for all of the valid request sizes for this interface.
   //
   covergroup cg_etlink_rsp;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_rsp_valid_to_valid: coverpoint rsp_idle_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      cp_rsp_not_ready: coverpoint rsp_busy_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      cp_rsp_size: coverpoint rsp_size {
         bins B1_8b    = { 3'b000 };
         bins B2_16b   = { 3'b001 };
         bins B4_32b   = { 3'b010 };
         bins B8_64b   = { 3'b011 };
         bins B16_128b = { 3'b100 };
         bins B32_256b = { 3'b101 };
         bins B64_512b = { 3'b110 };
      }

      cp_rsp_id_neigh_sc: coverpoint rsp_id {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_NEIGH_SC) ? 100 : 0;

         bins rsp_ids[] = { [0:255] };
      }

      cp_rsp_id_msg: coverpoint rsp_id {
         option.weight      = (ETLINK_COV_TYPE == EVL_COV_ETLINK_MINION) ? 1   : 0;
         option.goal        = (ETLINK_COV_TYPE == EVL_COV_ETLINK_MINION) ? 100 : 0;
         type_option.weight = (ETLINK_COV_TYPE == EVL_COV_ETLINK_MINION) ? 1   : 0;
         type_option.goal   = (ETLINK_COV_TYPE == EVL_COV_ETLINK_MINION) ? 100 : 0;

         bins        Regular_Port    = { 0 };
         bins        values1_31[]    = { [1:31] };
         bins        Tbox_Reply_Port = { 128 };
         bins        Reduce_Ready    = { 192 };
         bins        Reduce_Data     = { 193 };
         bins        Tbox_Pull       = { 194 };
         ignore_bins ignore_values   = { [32:127], [129:191], [195:255] };
      }

      cp_rsp_dest: coverpoint rsp_dest iff (rsp_opcode == 1) {
         bins        minion_0      = { 4'b0000 };
         bins        minion_1      = { 4'b0001 };
         bins        minion_2      = { 4'b0010 };
         bins        minion_3      = { 4'b0011 };
         bins        minion_4      = { 4'b0100 };
         bins        minion_5      = { 4'b0101 };
         bins        minion_6      = { 4'b0110 };
         bins        minion_7      = { 4'b0111 };
         bins        icache_0      = { 4'b1000 };
         bins        ptw_0         = { 4'b1001 };
         bins        ptw_1         = { 4'b1010 };
         bins        tbox          = { 4'b1011 };
         bins        cooperative   = { 4'b1100 };
         ignore_bins ignore_values = { [13:15] };
      }

      cp_rsp_dest_msg: coverpoint rsp_dest iff (rsp_opcode == 2) {
         bins minion_0_thread_0 = { 4'b0000 };
         bins minion_1_thread_0 = { 4'b0001 };
         bins minion_2_thread_0 = { 4'b0010 };
         bins minion_3_thread_0 = { 4'b0011 };
         bins minion_4_thread_0 = { 4'b0100 };
         bins minion_5_thread_0 = { 4'b0101 };
         bins minion_6_thread_0 = { 4'b0110 };
         bins minion_7_thread_0 = { 4'b0111 };
         bins minion_0_thread_1 = { 4'b1000 };
         bins minion_1_thread_1 = { 4'b1001 };
         bins minion_2_thread_1 = { 4'b1010 };
         bins minion_3_thread_1 = { 4'b1011 };
         bins minion_4_thread_1 = { 4'b1100 };
         bins minion_5_thread_1 = { 4'b1101 };
         bins minion_6_thread_1 = { 4'b1110 };
         bins minion_7_thread_1 = { 4'b1111 };
      }

      cp_rsp_qwen_rd: coverpoint rsp_qwen iff (rsp_opcode == 1) {
         bins        qwens[]       = { 1, 2, 3, 4, 8, 12, 15 };
         ignore_bins ignore_values = { 0, [5:7], [9:11], [13:14] };
      }

      // message data is always aligned to the lsb
      cp_rsp_qwen_msg: coverpoint rsp_qwen iff (rsp_opcode == 2) {
         bins        qwens[]       = { 1, 3, 15 };
         ignore_bins ignore_values = { 0, 2, [4:14] };
      }

      cp_rsp_opcode: coverpoint rsp_opcode {
         bins rsp_ack          = { 0 };
         bins rsp_ack_data     = { 1 };
         bins rsp_msg_rcv_data = { 2 };
         bins rsp_err          = { 3 };
      }
   endgroup : cg_etlink_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_req
   //
   function void sample_req(input evl_paddr_t             req_addr_in,
                            input evl_etlink_req_opcode_t req_opcode_in,
                            input bit [7:0]               req_id_in,
                            input bit [2:0]               req_size_in,
                            input bit [3:0]               req_source_in,
                            input bit [3:0]               req_qwen_in,
                            input bit [6:0]               req_subopcode_in,
                            input int                     req_idle_count_in,
                            input int                     req_busy_count_in);
      req_id          = req_id_in;
      req_size        = req_size_in;
      req_opcode      = req_opcode_in;
      req_id          = req_id_in;
      req_id_miss     = req_id_in[4:0];
      req_addr_7      = req_addr_in[31:28];
      req_addr_6      = req_addr_in[27:24];
      req_addr_5      = req_addr_in[23:20];
      req_addr_4      = req_addr_in[19:16];
      req_addr_3      = req_addr_in[15:12];
      req_addr_2      = req_addr_in[11: 8];
      req_addr_1      = req_addr_in[ 7: 4];
      req_addr_0      = req_addr_in[ 3: 0];
      req_addr_edge_7 = req_addr_in[28:27];
      req_addr_edge_6 = req_addr_in[24:23];
      req_addr_edge_5 = req_addr_in[20:19];
      req_addr_edge_4 = req_addr_in[16:15];
      req_addr_edge_3 = req_addr_in[12:11];
      req_addr_edge_2 = req_addr_in[ 8: 7];
      req_addr_edge_1 = req_addr_in[ 4: 3];
      req_source      = req_source_in;
      req_qwen        = req_qwen_in;
      req_idle_count  = req_idle_count_in;
      req_busy_count  = req_busy_count_in;
      if (evl_etlink_req_opcode_t'(req_opcode_in) inside { ET_LINK_REQ_Write, ET_LINK_REQ_Read, ET_LINK_REQ_WriteAround }) begin
         req_subopcode = { 5'b00000, req_subopcode_in[1:0] };
      end
      else if (evl_etlink_req_opcode_t'(req_opcode_in) inside { ET_LINK_REQ_Atomic }) begin
         req_subopcode = req_subopcode_in;
         if (req_subopcode_in[4:4] == 1'b1) begin
            req_subopcode[5:5] = 1'b0;
         end
      end
      else begin
         req_subopcode = 7'b0000000;
      end
      cg_etlink_req.sample();
   endfunction : sample_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_rsp
   //
   function void sample_rsp(input evl_etlink_rsp_opcode_t rsp_opcode_in,
                            input bit [7:0]               rsp_id_in,
                            input bit [2:0]               rsp_size_in,
                            input bit [3:0]               rsp_dest_in,
                            input bit [3:0]               rsp_qwen_in,
                            input int                     rsp_idle_count_in,
                            input int                     rsp_busy_count_in);
      rsp_id         = rsp_id_in;
      rsp_size       = rsp_size_in;
      rsp_opcode     = rsp_opcode_in;
      rsp_id         = rsp_id_in;
      rsp_dest       = rsp_dest_in;
      rsp_qwen       = rsp_qwen_in;
      rsp_idle_count = rsp_idle_count_in;
      rsp_busy_count = rsp_busy_count_in;
      cg_etlink_rsp.sample();
   endfunction : sample_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      int value;

      if (`et_value_plusargs("data_cover_on=%d", value) == 0) begin
         m_cover_data = 1'b0;
      end
      else begin
         m_cover_data = (value == 0) ? 1'b0 : 1'b1;
      end
      cg_etlink_req = new();
      cg_etlink_rsp = new();
//    if (m_cover_data == 1'b1) begin
//       cg_axi_w_data = new();
//    end
   endfunction

endclass : evl_etlink_cov
