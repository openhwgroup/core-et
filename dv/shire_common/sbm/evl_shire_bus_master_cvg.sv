//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_bus_master_cvg
//
class evl_shire_bus_master_cvg extends evl_void;

   parameter sbm_ifc = 10; // 4 neigh, 4 sc_banks, shire_other, rbox
   parameter fifo_almost_full = 14; // max_conc_req - 2
   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sbm_decode
   //
   logic[39:0] m_sbm_wr_req_addr;
   logic[39:0] m_sbm_rd_req_addr;
   logic       m_sbm_wr_req_valid; 
   logic       m_sbm_rd_req_valid; 
   logic       m_sbm_rd_req_ready;
   logic       m_sbm_rd_req_valid_and_ready;

   logic m_dec_err_msb;
   logic m_dec_err_shire;
   logic m_dec_err_rbox;
   logic m_dec_err_neigh;
   logic m_dec_err_minion;
   logic m_dec_err_sc_bank;
   logic m_dec_err;

   logic m_neigh_req;
   logic m_minion_req;
   logic m_sc_req;
   logic m_rbox_req;
   logic m_shire_req;

   logic m_simultaneous_rw_req;
   logic m_simultaneous_rw_req_same_addr;
   logic m_simultaneous_rw_req_same_endpoint;
   logic m_fifo_almost_full_rw_req_same_endpoint;
   logic[9:0][3:0] m_counter;
   logic[4:0] m_sbm_buffer_count;

   logic m_neigh_bdc_req;
   logic m_sc_bdc_req;
   logic m_bdc_req;

   logic m_neigh_in_reset;

   logic[9:0] m_sbm_fifo_req_not_empty;
   logic[9:0] m_axi_to_apb_not_ready;
   logic[9:0] m_sbm_fifo_req_push;

   logic[$clog2(`SBM_WRITE_CREDITS+1) : 0] m_reserved_wr_pos;

   int max_conc_req = int'(`SBM_CREDIT_NUMBER); //16
   logic[9:0] m_req_fifo_almost_full;

   logic[3:0] m_reset_neigh;
   logic m_reset_sc;
   logic m_reset_rbox;
   logic m_req_to_neigh_in_reset;
   logic m_req_to_sc_in_reset;
   logic m_req_to_rbox_in_reset;
   logic m_req_to_endpoint_in_reset;

   covergroup cg_sbm_decode ();
      option.name = "cg_sbm_decode";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_sbm_wr_request : coverpoint m_sbm_wr_req_addr[21:20] iff (m_sbm_wr_req_valid == 1) {
         bins neigh_esr[1]  = { [0:1] };
         bins l2bank_esr[1] = { 3 } iff (m_sbm_wr_req_addr[19:17] == 0);
         bins rbox_esr[1]  = { 3 } iff (m_sbm_wr_req_addr[19:17] == 1);
         bins shire_esr[1]   = { 3 } iff (m_sbm_wr_req_addr[19:17] == 2);
      }
      cp_sbm_rd_request : coverpoint m_sbm_rd_req_addr[21:20] iff (m_sbm_rd_req_valid == 1) {
         bins neigh_esr[1]  = { [0:1] };
         bins l2bank_esr[1] = { 3 } iff (m_sbm_rd_req_addr[19:17] == 0);
         bins rbox_esr[1]  = { 3 } iff (m_sbm_rd_req_addr[19:17] == 1);
         bins shire_esr[1]   = { 3 } iff (m_sbm_rd_req_addr[19:17] == 2);
      }
      cp_sbm_decode_error : coverpoint m_dec_err {
         bins dec_err_msb     = {1} iff (m_dec_err_msb); 
         bins dec_err_shire   = {1} iff (m_dec_err_shire); 
         bins dec_err_rbox    = {1} iff (m_dec_err_rbox); 
         bins dec_err_neigh   = {1} iff (m_dec_err_neigh); 
         bins dec_err_minion  = {1} iff (m_dec_err_minion); 
         bins dec_err_sc_bank = {1} iff (m_dec_err_sc_bank); 
      }
      cp_sbm_simultaneous_rw_req : coverpoint m_simultaneous_rw_req iff (m_dec_err == 0) {
         bins rw_req_diff_addr = {1} iff (m_simultaneous_rw_req_same_addr == 0);
         bins rw_req_same_addr = {1} iff (m_simultaneous_rw_req_same_addr == 1);
      }
      cp_sbm_bdc_req : coverpoint m_bdc_req {
         bins neigh_bdc_req = {1} iff (m_neigh_bdc_req && (m_dec_err_neigh == 0));
         bins sc_bdc_req    = {1} iff (m_sc_bdc_req && (m_dec_err_sc_bank == 0));
      }
      cp_sbm_broadcast_neigh_reset : coverpoint m_neigh_bdc_req iff (m_neigh_in_reset);
      //FUTURE create cp for each of the sbm fifos, loop
      cp_sbm_fifo_store_cases : coverpoint m_sbm_fifo_req_push[0] iff (m_bdc_req == 0) {
         bins req_in_fifo_not_empty = {1} iff (m_sbm_fifo_req_not_empty[0]);
         bins req_in_fifo_not_ready = {1} iff (m_axi_to_apb_not_ready[0]);
      }
      cp_sbm_buffer_full : coverpoint m_sbm_buffer_count {
         wildcard bins sbm_buffer_almost_full = {14};
         wildcard bins sbm_buffer_counts15    = {15};
         wildcard bins sbm_buffer_full        = {16};
      }
      //Check item
      cp_sbm_fifo_empty : coverpoint m_sbm_fifo_req_not_empty {
         wildcard bins sbm_fifo0_empty = {10'b??_????_???0};
         wildcard bins sbm_fifo1_empty = {10'b??_????_??0?};
         wildcard bins sbm_fifo2_empty = {10'b??_????_?0??};
         wildcard bins sbm_fifo3_empty = {10'b??_????_0???};
         wildcard bins sbm_fifo4_empty = {10'b??_???0_????};
         wildcard bins sbm_fifo5_empty = {10'b??_??0?_????};
         wildcard bins sbm_fifo6_empty = {10'b??_?0??_????};
         wildcard bins sbm_fifo7_empty = {10'b??_0???_????};
         wildcard bins sbm_fifo8_empty = {10'b?0_????_????};
         wildcard bins sbm_fifo9_empty = {10'b0?_????_????};
      }
      cp_sbm_fifos_not_empty : coverpoint $countones(m_sbm_fifo_req_not_empty) {
         bins fifos_empty = {0};
         bins non_empty_fifo = {1};
         bins non_empty_fifos_2 = {2};
         bins non_empty_fifos_3 = {3};
         bins non_empty_fifos_4 = {4};
         bins non_empty_fifos_5 = {5};
         bins non_empty_fifos_6 = {6};
         bins non_empty_fifos_7 = {7};
         bins non_empty_fifos_8 = {8};
         bins non_empty_fifos_9 = {9};
         bins non_empty_fifos_10 = {10};
      }
      cp_wr_credits_used_up_read_req : coverpoint m_sbm_rd_req_valid_and_ready iff (m_reserved_wr_pos == 0) {
         bins sbm_wr_credits_used_up_read_req = {1};
      }
      cp_eight_write_req : coverpoint m_reserved_wr_pos {
         bins sbm_eight_write_req = {0};
      }
      cp_sbm_req_to_endpoint_in_reset : coverpoint m_req_to_endpoint_in_reset {
         bins req_to_neigh_in_reset = {1} iff (m_req_to_neigh_in_reset);
         bins req_to_sc_in_reset    = {1} iff (m_req_to_sc_in_reset);
         bins req_to_rbox_in_reset  = {1} iff (m_req_to_rbox_in_reset);
      }
      cp_fifo_almost_full_simultaneous_rw_req : coverpoint m_fifo_almost_full_rw_req_same_endpoint iff (m_simultaneous_rw_req_same_endpoint) {
         bins neigh_req = {1} iff (m_neigh_req || m_minion_req);
         bins sc_req    = {1} iff (m_sc_req);
         bins rbox_req  = {1} iff (m_rbox_req);
         bins shire_req = {1} iff (m_shire_req);
      }
   endgroup : cg_sbm_decode



   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_sbm_rsp
   //
   logic m_wr_rsp_fifo_not_empty;
   logic m_rd_rsp_fifo_not_empty;
   logic[1:0] m_decode_error_return;
   logic[1:0] m_bdc_error_search;
   logic      m_neigh_bdc_valid;
   logic      m_sc_bdc_valid;
   logic m_sbm_mesh_slave_axi_BREADY;
   logic m_sbm_mesh_slave_axi_RREADY;
   logic [sbm_ifc-1 : 0] m_mesh_slave_axi_BVALID;
   logic [sbm_ifc-1 : 0] m_mesh_slave_axi_RVALID;
   covergroup cg_sbm_rsp ();
      option.name = "cg_sbm_rsp";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_sbm_fifo_not_empty_error_rsp : coverpoint m_decode_error_return {
         wildcard bins write_rsp_error_fifo_not_empty = {2'b1?} iff (m_wr_rsp_fifo_not_empty);
         wildcard bins read_rsp_error_fifo_not_empty  = {2'b?1} iff (m_rd_rsp_fifo_not_empty);
      }
      cp_sbm_bdc_returns_error_rsp : coverpoint m_bdc_error_search {
         wildcard bins neigh_bdc_err_rsp = {2'b?1} iff (m_neigh_bdc_valid);
         wildcard bins sc_bdc_err_rsp    = {2'b1?} iff (m_sc_bdc_valid);
      }
      cp_multiple_simultaneous_responses_write_rsp : coverpoint $countones(m_mesh_slave_axi_BVALID) iff (m_sbm_mesh_slave_axi_BREADY) {
         bins one_write_rsp     = {1};
         bins more_write_rsp[1] = {[2:10]};
      }
      cp_multiple_simultaneous_responses_read_rsp : coverpoint $countones(m_mesh_slave_axi_RVALID) iff (m_sbm_mesh_slave_axi_RREADY) {
         bins one_read_rsp     = {1};
         bins more_read_rsp[1] = {[2:10]};
      }
      cp_decode_error_return : coverpoint m_decode_error_return {
         wildcard bins error_return_read = {2'b?1};
         wildcard bins error_return_write = {2'b1?};
      }
      cp_multiple_simultaneous_responses: cross cp_multiple_simultaneous_responses_write_rsp, cp_multiple_simultaneous_responses_read_rsp, cp_decode_error_return;
   endgroup : cg_sbm_rsp

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_axi_to_apb
   //
   logic[$clog2(sbm_ifc)-1:0] m_sum_axi_to_apb_busy;
   logic[1:0]                 m_axi_aw_offset;
   logic                      m_axi_aw_valid;
   covergroup cg_axi_to_apb ();
      option.name = "cg_axi_to_apb";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_different_axi_offsets : coverpoint m_axi_aw_offset {
          bins data_chunk_0 = {0};
          bins data_chunk_1 = {1};
          bins data_chunk_2 = {2};
          bins data_chunk_3 = {3};
      }
      cp_sbm_concurrent_requests : coverpoint m_sum_axi_to_apb_busy {
          bins total_sbm_ifc_busy[9] = {[1:9]};
      }
    endgroup : cg_axi_to_apb

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_fifo_full_new_read_req
   //
//logic m_sbm_mesh_slave_axi_ARREADY;
   covergroup cg_fifo_full_new_read_req with function sample (logic sbm_mesh_slave_axi_ARREADY);
      option.name = "cg_fifo_full_new_read_req";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_fifo_full_new_read_req : coverpoint sbm_mesh_slave_axi_ARREADY {
          bins fifo_full = {0};
          bins fifo_not_full = {1};
      }
    endgroup : cg_fifo_full_new_read_req

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_fifo_full_new_write_req
   //
//logic m_sbm_mesh_slave_axi_AWREADY;
   covergroup cg_fifo_full_new_write_req with function sample (logic sbm_mesh_slave_axi_AWREADY);
      option.name = "cg_fifo_full_new_write_req";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_fifo_full_new_write_req : coverpoint sbm_mesh_slave_axi_AWREADY {
          bins not_enough_credits = {0};
          bins fifo_not_full = {1};
      }
    endgroup : cg_fifo_full_new_write_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sbm_decode
   //
   function void sample_sbm_decode(logic[39:0] sbm_mesh_slave_axi_AW_ADDR, //sys_slave_axi_AW_channel_t sbm_mesh_slave_axi_AW, 
                                   logic[39:0] sbm_mesh_slave_axi_AR_ADDR, 
                                   logic sbm_mesh_slave_axi_AWVALID,
                                   logic sbm_mesh_slave_axi_ARVALID,
                                   logic sbm_mesh_slave_axi_ARREADY,
                                   logic dec_err,
                                   logic dec_err_msb,
                                   logic dec_err_shire,
                                   logic dec_err_neigh,
                                   logic dec_err_minion,
                                   logic dec_err_sc_bank,
                                   logic dec_err_rbox,
                                   logic[3:0] reset_neigh, //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.reset_neigh
                                   logic reset_sc,
                                   logic reset_rbox,
                                   logic[9:0] sbm_fifo_req_not_empty, //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.sbm_fifo_req_push
                                   logic[9:0] axi_to_apb_ready,
                                   logic[9:0] sbm_fifo_req_push,
                                   logic[9:0][3:0] counter,
                                   logic[4:0] sbm_buffer_count,
                                   logic[$clog2(`SBM_WRITE_CREDITS+1) : 0] reserved_wr_pos,
                                   logic simultaneous_rw_req_same_endpoint);


      
      m_sbm_wr_req_addr  = sbm_mesh_slave_axi_AW_ADDR; //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.sbm_mesh_slave_axi_AW
      m_sbm_rd_req_addr  = sbm_mesh_slave_axi_AR_ADDR;
      m_sbm_wr_req_valid = sbm_mesh_slave_axi_AWVALID;
      m_sbm_rd_req_valid = sbm_mesh_slave_axi_ARVALID;
      m_sbm_rd_req_ready = sbm_mesh_slave_axi_ARREADY;
      m_sbm_rd_req_valid_and_ready = m_sbm_rd_req_valid && m_sbm_rd_req_ready;

      // Decode error
    m_dec_err_msb = dec_err_msb;
    m_dec_err_shire = dec_err_shire;
    m_dec_err_rbox = dec_err_rbox;
    m_dec_err_neigh = dec_err_neigh;
    m_dec_err_minion = dec_err_minion;
    m_dec_err_sc_bank = dec_err_sc_bank;
    m_dec_err = dec_err;

      m_neigh_req  = (m_sbm_wr_req_addr[21:20] == 2'h1) || (m_sbm_rd_req_addr[21:20] == 2'h1);
      m_minion_req = (m_sbm_wr_req_addr[21:20] == 2'h0) || (m_sbm_rd_req_addr[21:20] == 2'h0);
      m_sc_req     = ((m_sbm_wr_req_addr[21:20] == 2'h3) && (m_sbm_wr_req_addr[19:17] == 3'h0)) || ((m_sbm_rd_req_addr[21:20] == 2'h3) && (m_sbm_rd_req_addr[19:17] == 3'h0));
      m_rbox_req   = ((m_sbm_wr_req_addr[21:20] == 2'h3) && (m_sbm_wr_req_addr[19:17] == 3'h1)) || ((m_sbm_rd_req_addr[21:20] == 2'h3) && (m_sbm_rd_req_addr[19:17] == 3'h1));
      m_shire_req  = ((m_sbm_wr_req_addr[21:20] == 2'h3) && (m_sbm_wr_req_addr[19:17] == 3'h2)) || ((m_sbm_rd_req_addr[21:20] == 2'h3) && (m_sbm_rd_req_addr[19:17] == 3'h2));

/*      m_dec_err_msb = ( (m_sbm_wr_req_addr[39:32] == 8'h01) || (m_sbm_rd_req_addr[39:32] == 8'h01) ) ? 0 : 1;
      m_dec_err_neigh =  m_neigh_req ?        // Request targets neigh esrs
                             ( ((m_sbm_wr_req_addr[19:16] > 4'h3) && (m_sbm_wr_req_addr[19:16] != 4'hf)) || (m_sbm_rd_req_addr[19:16] > 4'h3) ) ? 1 : 0 : 0;  // Request targets non-existing neigh and is not bdc
      m_dec_err_minion = m_minion_req ?        // Request targets minion esrs
                             ( (m_sbm_wr_req_addr[19:13] > 31) || (m_sbm_rd_req_addr[19:13] > 31) ) ? 1 : 0 : 0; // Request targets non-existing minion
      m_dec_err_sc_bank = m_sc_req ?        // Request targets sc esrs
                              ( ((m_sbm_wr_req_addr[16:13] > 4'h3) && (m_sbm_wr_req_addr[16:13] != 4'hf)) || (m_sbm_rd_req_addr[16:13] > 4'h3) ) ? 1 : 0 : 0;  // Request targets non-existing sc and is not bdc
      m_dec_err = |{m_dec_err_msb,m_dec_err_neigh,m_dec_err_minion,m_dec_err_sc_bank};
*/
      
      //Simultaneous Read and Write requests
      m_simultaneous_rw_req_same_addr = (m_sbm_wr_req_addr == m_sbm_rd_req_addr) ? 1 : 0;
      m_simultaneous_rw_req = ( (m_sbm_wr_req_addr >= 40'h01_0000_0000) && (m_sbm_rd_req_addr >= 40'h01_0000_0000) );
      m_simultaneous_rw_req_same_endpoint = simultaneous_rw_req_same_endpoint;
      m_counter = counter;
      m_sbm_buffer_count = sbm_buffer_count;
      if (m_simultaneous_rw_req_same_endpoint && (m_dec_err==0)) begin // rw requests targets same esr at the same time and there is no decode error
         if (m_minion_req) begin
            if ((m_sbm_wr_req_addr[19:13] <= 7) && (m_counter[0][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:13] <= 15) && (m_counter[1][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:13] <= 23) && (m_counter[2][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:13] <= 31) && (m_counter[3][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
         end
         else if (m_neigh_req) begin
            if ((m_sbm_wr_req_addr[19:16] == 4'h0) && (m_counter[0][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:16] == 4'h1) && (m_counter[1][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:16] == 4'h2) && (m_counter[2][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[19:16] == 4'h3) && (m_counter[3][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
         end
         else if (m_sc_req) begin
            if ((m_sbm_wr_req_addr[16:13] == 4'h0) && (m_counter[4][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[16:13] == 4'h1) && (m_counter[5][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[16:13] == 4'h2) && (m_counter[6][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
            else if ((m_sbm_wr_req_addr[16:13] == 4'h3) && (m_counter[7][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
         end
         else if ((m_shire_req) && (m_counter[8][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
         else if ((m_rbox_req) && (m_counter[9][3:0] == fifo_almost_full)) m_fifo_almost_full_rw_req_same_endpoint = 1;
      end



      // Write requests sent from the Noc

      m_reserved_wr_pos = reserved_wr_pos;


      //BDC requests

      m_neigh_bdc_req = (m_sbm_wr_req_addr[21:20] == 2'h1) && (m_sbm_wr_req_addr[19:16] == 4'hf);
      m_sc_bdc_req    = (m_sbm_wr_req_addr[21:20] == 2'h3) && (m_sbm_wr_req_addr[19:17] == 3'h0) && (m_sbm_wr_req_addr[16:13] == 4'hf);
      m_bdc_req       = |{m_neigh_bdc_req,m_sc_bdc_req};

      m_neigh_in_reset = |reset_neigh;

      // REQ FIFO

      m_sbm_fifo_req_not_empty = sbm_fifo_req_not_empty;
      m_axi_to_apb_not_ready   = ~axi_to_apb_ready;
      m_sbm_fifo_req_push      = sbm_fifo_req_push;


      for (int i=0; i < sbm_ifc; i++) m_req_fifo_almost_full[i] = (counter[i][3:0] == 14);

      // Endpoint in reset

      m_reset_neigh = reset_neigh;
      m_reset_sc = reset_sc;
      m_reset_rbox = reset_rbox;
      
      if ((m_sbm_wr_req_addr[21:20] <= 2'h1) && (m_sbm_wr_req_addr[19:16]<=3)) m_req_to_neigh_in_reset = m_reset_neigh[m_sbm_wr_req_addr[19:16]];
      else if ((m_sbm_rd_req_addr[21:20] <= 2'h1) && (m_sbm_rd_req_addr[19:16]<=3)) m_req_to_neigh_in_reset = m_reset_neigh[m_sbm_rd_req_addr[19:16]];
      if (m_sc_req && m_reset_sc && (m_sc_bdc_req == 0)) m_req_to_sc_in_reset = 1;
      if (m_rbox_req && m_reset_rbox) m_req_to_rbox_in_reset = 1;
      m_req_to_endpoint_in_reset = |{|m_req_to_neigh_in_reset,m_req_to_sc_in_reset,m_req_to_rbox_in_reset};     

      cg_sbm_decode.sample();
   endfunction : sample_sbm_decode




   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_sbm_rsp
   //
   function void sample_sbm_rsp(logic writeRsp_fifo_not_empty, logic readRsp_fifo_not_empty, logic[1:0] error_return, logic[1:0] error_search, logic sbm_mesh_slave_axi_BREADY, logic sbm_mesh_slave_axi_RREADY, logic [sbm_ifc-1 : 0] mesh_slave_axi_BVALID, logic [sbm_ifc-1 : 0] mesh_slave_axi_RVALID, logic neigh_bdc_valid, logic sc_bdc_valid);

      m_wr_rsp_fifo_not_empty = writeRsp_fifo_not_empty; //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.writeRsp_fifo_not_empty
      m_rd_rsp_fifo_not_empty = readRsp_fifo_not_empty;
      m_decode_error_return   = error_return;

      m_bdc_error_search = error_search; //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.error_search
      m_neigh_bdc_valid = neigh_bdc_valid;
      m_sc_bdc_valid    = sc_bdc_valid;

      m_sbm_mesh_slave_axi_BREADY   = sbm_mesh_slave_axi_BREADY;
      m_sbm_mesh_slave_axi_RREADY   = sbm_mesh_slave_axi_RREADY;
      m_mesh_slave_axi_BVALID   = mesh_slave_axi_BVALID;
      m_mesh_slave_axi_RVALID   = mesh_slave_axi_RVALID;

      cg_sbm_rsp.sample();
   endfunction : sample_sbm_rsp

   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_axi_to_apb
   //
   function void sample_axi_to_apb(logic[sbm_ifc-1:0] axi_to_apb_ready, logic[39:0] axi_aw_addr, logic axi_aw_valid);

      m_sum_axi_to_apb_busy = $countones(~axi_to_apb_ready); //shire_top.shire_control_gen.shire_control_status_inst.sbm_top.sbm_nocs.axi_to_apb_ready[9:0] 
      m_axi_aw_offset = axi_aw_addr[4:3];
      m_axi_aw_valid  = axi_aw_valid;

      cg_axi_to_apb.sample();
   endfunction : sample_axi_to_apb

   //-----------------------------------------------------------------------------------------------
/*
   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_fifo_full_new_read_req
   //
   function void sample_fifo_full_new_read_req(logic sbm_mesh_slave_axi_ARREADY);

      m_sbm_mesh_slave_axi_ARREADY = sbm_mesh_slave_axi_ARREADY;

      cg_fifo_full_new_read_req.sample();
   endfunction : sample_fifo_full_new_read_req

   //-----------------------------------------------------------------------------------------------

   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_fifo_full_new_write_req
   //
   function void sample_fifo_full_new_write_req(logic sbm_mesh_slave_axi_AWREADY);

      m_sbm_mesh_slave_axi_AWREADY = sbm_mesh_slave_axi_AWREADY;

      cg_fifo_full_new_write_req.sample();
   endfunction : sample_fifo_full_new_write_req



   //-----------------------------------------------------------------------------------------------
*/
   //
   // Constructor
   //
   function new();
      super.new();
      cg_sbm_decode = new();
      cg_sbm_rsp    = new();
      cg_axi_to_apb = new();
      cg_fifo_full_new_read_req = new();
      cg_fifo_full_new_write_req = new();
   endfunction : new

endclass : evl_shire_bus_master_cvg
