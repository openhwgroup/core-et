//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbgnoc_scoreboard
//
//
class evl_dbgnoc_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_dbgnoc_scoreboard)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_dbgnoc_sb_error_intensity)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   uvm_tlm_analysis_fifo #(evl_req_desc) m_dn_evt_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_dn_evt_rx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_dn_msg_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_dn_msg_rx_req_fifo[evl_shire_id_t];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_lf_evt_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_lf_evt_rx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_lf_msg_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_lf_msg_rx_req_fifo[evl_shire_id_t];

   uvm_tlm_analysis_fifo #(evl_req_desc) m_rt_evt_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rt_evt_rx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rt_msg_tx_req_fifo[evl_shire_id_t];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_rt_msg_rx_req_fifo[evl_shire_id_t];

   uvm_analysis_export #(evl_req_desc) m_dn_evt_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_dn_evt_rx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_dn_msg_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_dn_msg_rx_req_export[evl_shire_id_t];

   uvm_analysis_export #(evl_req_desc) m_lf_evt_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_lf_evt_rx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_lf_msg_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_lf_msg_rx_req_export[evl_shire_id_t];

   uvm_analysis_export #(evl_req_desc) m_rt_evt_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_rt_evt_rx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_rt_msg_tx_req_export[evl_shire_id_t];
   uvm_analysis_export #(evl_req_desc) m_rt_msg_rx_req_export[evl_shire_id_t];

   // REQUEST Qs
   evl_req_desc  m_active_dn_evt_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_dn_msg_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_lf_evt_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_lf_msg_req_descs[evl_shire_id_t][evl_shire_id_t][$];

   evl_req_desc  m_active_rt_evt_req_descs[evl_shire_id_t][evl_shire_id_t][$];
   evl_req_desc  m_active_rt_msg_req_descs[evl_shire_id_t][evl_shire_id_t][$];
/*
   `ifdef EVL_COVERAGE_ON
      evl_noc_sb_cvg    m_noc_cov;
   `endif
*/
   int memshire_master_ids [7:0]       = '{`MEMSHIRE_MASTER_IDS};
   int tol3_sib_master_ids [33:0]      = '{`TOL3_SIB_MASTER_IDS};
   int tosys_sib_master_ids [33:0]     = '{`TOSYS_SIB_MASTER_IDS};
   int l2tol3_master_ids [3:0] [33:0]  = '{'{`L2TOL3D_MASTER_IDS}, '{`L2TOL3C_MASTER_IDS}, '{`L2TOL3B_MASTER_IDS}, '{`TOL3_SIB_MASTER_IDS}};

   int io_rt_id = 0;
   int io_lf_id = 154;
   int ps_dn_id = 153;
   int memshire_lf_ids [7:0] = '{162, 161, 160, 159, 158, 157, 156, 155};
   int memshire_dn_ids [7:0] = '{151, 149, 147, 145, 143, 141, 139, 137};
   int minshire_dn_ids [33:0]    = '{133, 129, 125, 121, 117, 113, 109, 105, 101, 97, 93, 89, 85, 81, 77, 73, 69, 65, 61, 57, 53, 49, 45, 41, 37, 33, 29, 25, 21, 17, 13, 9, 5, 1};

   bit m_dbg_sanity = 1'b0;
   evl_xword_t m_sanity_test_data [2:0] = '{'h2aaaaaaa81c0403300109, 'h443300103, 'h26c00600004};
   bit [2:0] m_sanity_test_data_seen = 3'b0;

   bit m_dbg_evt_test = 1'b0;
   bit [33:0] m_dbg_min_evts      = 34'b0;
   bit [239:232] m_dbg_mc_lf_evts = 8'b0;
   bit [239:232] m_dbg_mc_dn_evts = 8'b0;
   bit m_dbg_ios_lf_evt           = 1'b0;
   bit m_dbg_ios_rt_evt           = 1'b0;
   bit m_dbg_ps_dn_evt            = 1'b0;

   //int m_current_shire_ids [33:0];

   `include "get_shire_id_functions.vh"
   //`include "evl_noc_regmap.svh"
   //------------------------------------------------------------------------------------------------
   //
   // build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      int i;
      super.build_phase(phase);
/*
      `ifdef EVL_COVERAGE_ON
         `evl_log(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("Building NoC Coverage Object..."))
         m_noc_cov = new();
         `evl_log(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("Done Building NoC Coverage Object..."))
      `endif
*/
      `evl_log(UVM_LOW, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("Building Debug NoC Scoreboard"))

      for (i = 0; i <= 33; i = i + 1) begin
         m_dn_evt_tx_req_fifo[i]   = new($sformatf("m_dn_evt_tx_req_fifo_%0d", i), this);
         m_dn_evt_rx_req_fifo[i]   = new($sformatf("m_dn_evt_rx_req_fifo_%0d", i), this);
         m_dn_msg_tx_req_fifo[i]   = new($sformatf("m_dn_msg_tx_req_fifo_%0d", i), this);
         m_dn_msg_rx_req_fifo[i]   = new($sformatf("m_dn_msg_rx_req_fifo_%0d", i), this);
         m_dn_evt_tx_req_export[i] = new($sformatf("m_dn_evt_tx_req_export_%0d", i), this);
         m_dn_evt_rx_req_export[i] = new($sformatf("m_dn_evt_rx_req_export_%0d", i), this);
         m_dn_msg_tx_req_export[i] = new($sformatf("m_dn_msg_tx_req_export_%0d", i), this);
         m_dn_msg_rx_req_export[i] = new($sformatf("m_dn_msg_rx_req_export_%0d", i), this);
      end

      for (i = 232; i <= 239; i = i + 1) begin
         m_dn_evt_tx_req_fifo[i]   = new($sformatf("m_dn_evt_tx_req_fifo_%0d", i), this);
         m_dn_evt_rx_req_fifo[i]   = new($sformatf("m_dn_evt_rx_req_fifo_%0d", i), this);
         m_dn_msg_tx_req_fifo[i]   = new($sformatf("m_dn_msg_tx_req_fifo_%0d", i), this);
         m_dn_msg_rx_req_fifo[i]   = new($sformatf("m_dn_msg_rx_req_fifo_%0d", i), this);
         m_lf_evt_tx_req_fifo[i]   = new($sformatf("m_lf_evt_tx_req_fifo_%0d", i), this);
         m_lf_evt_rx_req_fifo[i]   = new($sformatf("m_lf_evt_rx_req_fifo_%0d", i), this);
         m_lf_msg_tx_req_fifo[i]   = new($sformatf("m_lf_msg_tx_req_fifo_%0d", i), this);
         m_lf_msg_rx_req_fifo[i]   = new($sformatf("m_lf_msg_rx_req_fifo_%0d", i), this);
         m_dn_evt_tx_req_export[i] = new($sformatf("m_dn_evt_tx_req_export_%0d", i), this);
         m_dn_evt_rx_req_export[i] = new($sformatf("m_dn_evt_rx_req_export_%0d", i), this);
         m_dn_msg_tx_req_export[i] = new($sformatf("m_dn_msg_tx_req_export_%0d", i), this);
         m_dn_msg_rx_req_export[i] = new($sformatf("m_dn_msg_rx_req_export_%0d", i), this);
         m_lf_evt_tx_req_export[i] = new($sformatf("m_lf_evt_tx_req_export_%0d", i), this);
         m_lf_evt_rx_req_export[i] = new($sformatf("m_lf_evt_rx_req_export_%0d", i), this);
         m_lf_msg_tx_req_export[i] = new($sformatf("m_lf_msg_tx_req_export_%0d", i), this);
         m_lf_msg_rx_req_export[i] = new($sformatf("m_lf_msg_rx_req_export_%0d", i), this);
      end

      m_dn_evt_tx_req_fifo[253]   = new($sformatf("m_dn_evt_tx_req_fifo_253"), this);
      m_dn_evt_rx_req_fifo[253]   = new($sformatf("m_dn_evt_rx_req_fifo_253"), this);
      m_dn_msg_tx_req_fifo[253]   = new($sformatf("m_dn_msg_tx_req_fifo_253"), this);
      m_dn_msg_rx_req_fifo[253]   = new($sformatf("m_dn_msg_rx_req_fifo_253"), this);
      m_dn_evt_tx_req_export[253] = new($sformatf("m_dn_evt_tx_req_export_253"), this);
      m_dn_evt_rx_req_export[253] = new($sformatf("m_dn_evt_rx_req_export_253"), this);
      m_dn_msg_tx_req_export[253] = new($sformatf("m_dn_msg_tx_req_export_253"), this);
      m_dn_msg_rx_req_export[253] = new($sformatf("m_dn_msg_rx_req_export_253"), this);

      m_lf_evt_tx_req_fifo[254]   = new($sformatf("m_lf_evt_tx_req_fifo_254"), this);
      m_lf_evt_rx_req_fifo[254]   = new($sformatf("m_lf_evt_rx_req_fifo_254"), this);
      m_lf_msg_tx_req_fifo[254]   = new($sformatf("m_lf_msg_tx_req_fifo_254"), this);
      m_lf_msg_rx_req_fifo[254]   = new($sformatf("m_lf_msg_rx_req_fifo_254"), this);
      m_rt_evt_tx_req_fifo[254]   = new($sformatf("m_rt_evt_tx_req_fifo_254"), this);
      m_rt_evt_rx_req_fifo[254]   = new($sformatf("m_rt_evt_rx_req_fifo_254"), this);
      m_rt_msg_tx_req_fifo[254]   = new($sformatf("m_rt_msg_tx_req_fifo_254"), this);
      m_rt_msg_rx_req_fifo[254]   = new($sformatf("m_rt_msg_rx_req_fifo_254"), this);
      m_lf_evt_tx_req_export[254] = new($sformatf("m_lf_evt_tx_req_export_254"), this);
      m_lf_evt_rx_req_export[254] = new($sformatf("m_lf_evt_rx_req_export_254"), this);
      m_lf_msg_tx_req_export[254] = new($sformatf("m_lf_msg_tx_req_export_254"), this);
      m_lf_msg_rx_req_export[254] = new($sformatf("m_lf_msg_rx_req_export_254"), this);
      m_rt_evt_tx_req_export[254] = new($sformatf("m_rt_evt_tx_req_export_254"), this);
      m_rt_evt_rx_req_export[254] = new($sformatf("m_rt_evt_rx_req_export_254"), this);
      m_rt_msg_tx_req_export[254] = new($sformatf("m_rt_msg_tx_req_export_254"), this);
      m_rt_msg_rx_req_export[254] = new($sformatf("m_rt_msg_rx_req_export_254"), this);

   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
      foreach(m_dn_evt_tx_req_export[ii]) begin
         m_dn_evt_tx_req_export[ii].connect(m_dn_evt_tx_req_fifo[ii].analysis_export);
         m_dn_evt_rx_req_export[ii].connect(m_dn_evt_rx_req_fifo[ii].analysis_export);
         m_dn_msg_tx_req_export[ii].connect(m_dn_msg_tx_req_fifo[ii].analysis_export);
         m_dn_msg_rx_req_export[ii].connect(m_dn_msg_rx_req_fifo[ii].analysis_export);
      end
      foreach(m_lf_evt_tx_req_export[ii]) begin
         m_lf_evt_tx_req_export[ii].connect(m_lf_evt_tx_req_fifo[ii].analysis_export);
         m_lf_evt_rx_req_export[ii].connect(m_lf_evt_rx_req_fifo[ii].analysis_export);
         m_lf_msg_tx_req_export[ii].connect(m_lf_msg_tx_req_fifo[ii].analysis_export);
         m_lf_msg_rx_req_export[ii].connect(m_lf_msg_rx_req_fifo[ii].analysis_export);
      end
      foreach(m_rt_evt_tx_req_export[ii]) begin
         m_rt_evt_tx_req_export[ii].connect(m_rt_evt_tx_req_fifo[ii].analysis_export);
         m_rt_evt_rx_req_export[ii].connect(m_rt_evt_rx_req_fifo[ii].analysis_export);
         m_rt_msg_tx_req_export[ii].connect(m_rt_msg_tx_req_fifo[ii].analysis_export);
         m_rt_msg_rx_req_export[ii].connect(m_rt_msg_rx_req_fifo[ii].analysis_export);
      end
   endfunction : connect_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         super.run_phase(phase);
      join_none;
   endtask: run_phase

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_shire_dbgnoc_operations
   //
   virtual task watch_shire_dbgnoc_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_dn_evt_tx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_sent_requests(m_dn_msg_tx_req_fifo[shire], shire, DBG_DN_MSG);

      watch_for_received_requests(m_dn_evt_rx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_received_requests(m_dn_msg_rx_req_fifo[shire], shire, DBG_DN_MSG);

   endtask : watch_shire_dbgnoc_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_memshire_dbgnoc_operations
   //
   virtual task watch_memshire_dbgnoc_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_dn_evt_tx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_sent_requests(m_dn_msg_tx_req_fifo[shire], shire, DBG_DN_MSG);
      watch_for_sent_requests(m_lf_evt_tx_req_fifo[shire], shire, DBG_LF_EVT);
      watch_for_sent_requests(m_lf_msg_tx_req_fifo[shire], shire, DBG_LF_MSG);

      watch_for_received_requests(m_dn_evt_rx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_received_requests(m_dn_msg_rx_req_fifo[shire], shire, DBG_DN_MSG);
      watch_for_received_requests(m_lf_evt_rx_req_fifo[shire], shire, DBG_LF_EVT);
      watch_for_received_requests(m_lf_msg_rx_req_fifo[shire], shire, DBG_LF_MSG);

   endtask : watch_memshire_dbgnoc_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_ioshire_dbgnoc_operations
   //
   virtual task watch_ioshire_dbgnoc_operations(input evl_shire_id_t shire);
      watch_for_sent_requests(m_rt_evt_tx_req_fifo[shire], shire, DBG_RT_EVT);
      watch_for_sent_requests(m_rt_msg_tx_req_fifo[shire], shire, DBG_RT_MSG);
      watch_for_sent_requests(m_lf_evt_tx_req_fifo[shire], shire, DBG_LF_EVT);
      watch_for_sent_requests(m_lf_msg_tx_req_fifo[shire], shire, DBG_LF_MSG);

      watch_for_received_requests(m_rt_evt_rx_req_fifo[shire], shire, DBG_RT_EVT);
      watch_for_received_requests(m_rt_msg_rx_req_fifo[shire], shire, DBG_RT_MSG);
      watch_for_received_requests(m_lf_evt_rx_req_fifo[shire], shire, DBG_LF_EVT);
      watch_for_received_requests(m_lf_msg_rx_req_fifo[shire], shire, DBG_LF_MSG);

   endtask : watch_ioshire_dbgnoc_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_pshire_dbgnoc_operations
   //
   virtual task watch_pshire_dbgnoc_operations(input evl_shire_id_t shire);

      watch_for_sent_requests(m_dn_evt_tx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_sent_requests(m_dn_msg_tx_req_fifo[shire], shire, DBG_DN_MSG);

      watch_for_received_requests(m_dn_evt_rx_req_fifo[shire], shire, DBG_DN_EVT);
      watch_for_received_requests(m_dn_msg_rx_req_fifo[shire], shire, DBG_DN_MSG);

   endtask : watch_pshire_dbgnoc_operations

   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_sent_requests
   //
   task watch_for_sent_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_shire_id_t shire, input evl_dbgnoc_ifc_type_t ifc_type);
      evl_req_desc            rcv_req_desc;
      evl_req_desc            tgt_req_desc;
      evl_shire_id_t          target_shire;
      evl_paddr_t             rcv_req_addr;
      evl_bus_qos_t           rcv_req_qos;
      evl_bus_req_vc_t        rcv_req_dest_int;
      bit                     qos_ok;
      bit                     dest_int_ok;
      evl_dbgnoc_ifc_type_t   tgt_ifc_type;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               rcv_req_addr      = rcv_req_desc.get_paddr();
               rcv_req_qos       = rcv_req_desc.get_bus_qos();
               rcv_req_dest_int  = rcv_req_desc.get_vc();
               qos_ok            = (((ifc_type inside {`EVL_DBGNOC_EVT_IFC}) && (rcv_req_qos == 4'h1)) || ((ifc_type inside {`EVL_DBGNOC_MSG_IFC}) && (rcv_req_qos == 4'h0)));
               dest_int_ok       = (((ifc_type inside {`EVL_DBGNOC_EVT_IFC}) && (rcv_req_dest_int == 2'b01)) || ((ifc_type inside {`EVL_DBGNOC_MSG_IFC}) && (rcv_req_dest_int == 2'b00)));
               if (qos_ok && dest_int_ok) begin
                  target_shire = get_shire_id_from_addr(rcv_req_addr, ifc_type, tgt_ifc_type, shire);
               end
               //`ifdef EVL_COVERAGE_ON
               //   m_noc_cov.sample_master(shire, target_shire, rcv_req_addr, ifc_type, noc_layer);
               //`endif
               if (!qos_ok) begin
                  `dut_warning(get_abstract_name(), $sformatf("0x%0x is illegal QoS value for S%0d %p (%0s)", rcv_req_qos, shire, ifc_type, rcv_req_desc.sprint_obj()))
               end
               else if (!dest_int_ok) begin
                  `dut_warning(get_abstract_name(), $sformatf("0x%0x is illegal dest_int value for S%0d %p (%0s)", rcv_req_dest_int, shire, ifc_type, rcv_req_desc.sprint_obj()))
               end
               else if (target_shire == ~evl_shire_id_t'(0)) begin
                  `dut_warning(get_abstract_name(), $sformatf("0x%0x is illegal address for S%0d %p (%0s)", rcv_req_addr, shire, ifc_type, rcv_req_desc.sprint_obj()))
               end
               //else if (target_shire == shire) begin
               //   `dut_warning(get_abstract_name(), $sformatf("S%0d: %p can't send to itself (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
               //end
               //else if ((ifc_type == IO_PM) && (rcv_req_desc.bus_cmd_is_write() == 1'b1) && (tgt_slave_type == TOL3_S)) begin
               //   `dut_warning(get_abstract_name(), $sformatf("S%0d %p can only read from S%0d %p (%0s)", shire, ifc_type, target_shire, tgt_slave_type, rcv_req_desc.sprint_obj()))
               //end
               else begin
                  `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p DBGNOC REQ, %0s sent to S%0d %p...", shire, ifc_type, rcv_req_desc.sprint_obj(), target_shire, tgt_ifc_type))
                  rcv_req_desc.set_observed();
                  $cast(tgt_req_desc, rcv_req_desc.clone());
                  rcv_req_desc.add_child_desc(tgt_req_desc);
                  if (rcv_req_desc.bus_cmd_is_write() == 1'b1) begin
                     void'(rcv_req_desc.cascade_req_data_to_dst(tgt_req_desc));
                  end
                  `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), tgt_req_desc, $sformatf("S%0d: Putting %0s in S%0d queue... (S%0d %p 0x%0x)", shire, tgt_req_desc.sprint_obj(), target_shire, shire, ifc_type, rcv_req_addr))
                  case (ifc_type)
                     DBG_RT_EVT  : m_active_rt_evt_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     DBG_RT_MSG  : m_active_rt_msg_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     DBG_LF_EVT  : m_active_lf_evt_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     DBG_LF_MSG  : m_active_lf_msg_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     DBG_DN_EVT  : m_active_dn_evt_req_descs[shire][target_shire].push_back(tgt_req_desc);
                     DBG_DN_MSG  : m_active_dn_msg_req_descs[shire][target_shire].push_back(tgt_req_desc);
                  endcase
               end
            end
            else begin
               $display("S%0d %p has received a null req_desc. Not great!!!", shire, ifc_type);
            end
         end
      join_none

   endtask : watch_for_sent_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_received_requests
   //
   task watch_for_received_requests(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo, input evl_shire_id_t shire, input evl_dbgnoc_ifc_type_t ifc_type);
      evl_req_desc            rcv_req_desc;
      evl_bus_req_id_t        rcv_bus_req_id;
      evl_paddr_t             rcv_req_addr;
      evl_bus_cmd_t           rcv_bus_cmd;
      evl_trans_id_t          rcv_req_tid;
      evl_bus_qos_t           rcv_req_qos;
      //evl_line_data           rcv_req_data;
      evl_xword_t              rcv_req_data;
      evl_req_desc            active_req_desc;
      evl_bus_req_id_t        active_bus_req_id;
      bit                     req_match_found;
      evl_dbgnoc_ifc_type_t   tgt_slave_type;

      if (req_fifo == null) begin
         return;
      end
      fork
         forever begin
            req_fifo.get(rcv_req_desc);
            if (rcv_req_desc != null) begin
               req_match_found = 1'b0;
               `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p DBGNOC REQ with data 0x%0x seen!", shire, ifc_type, rcv_req_desc.get_req_xword(0)))
               rcv_req_data = rcv_req_desc.get_req_xword(0);
               `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p rcv_req_data 0x%0x", shire, ifc_type, rcv_req_data))
               rcv_req_tid  = rcv_req_desc.get_trans_id();

               if ((ifc_type == DBG_RT_MSG) && (m_dbg_sanity == 1'b1)) begin
                  check_for_sanity_test_data(rcv_req_data);
               end

               if ((m_dbg_evt_test == 1'b1) && (rcv_req_data inside {'h03, 'h103, 'h203})) begin
                  if (ifc_type == DBG_RT_EVT) begin
                     if (m_dbg_ios_rt_evt == 1'b1) begin
                        `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                     end
                     else begin
                        m_dbg_ios_rt_evt = 1'b1;
                     end
                  end
                  else if (ifc_type == DBG_LF_EVT) begin
                     if (shire == 'd254) begin
                        if (m_dbg_ios_lf_evt == 1'b1) begin
                           `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                        end
                        else begin
                           m_dbg_ios_lf_evt = 1'b1;
                        end
                     end
                     else begin
                        if (m_dbg_mc_lf_evts[shire] == 1'b1) begin
                           `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                        end
                        else begin
                           m_dbg_mc_lf_evts[shire] = 1'b1;
                        end
                     end
                  end
                  else if (ifc_type == DBG_DN_EVT) begin
                     if (shire == 'd253) begin
                        if (m_dbg_ps_dn_evt == 1'b1) begin
                           `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                        end
                        else begin
                           m_dbg_ps_dn_evt = 1'b1;
                        end
                     end
                     else if (shire > 'd34) begin
                        if (m_dbg_mc_dn_evts[shire] == 1'b1) begin
                           `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                        end
                        else begin
                           m_dbg_mc_dn_evts[shire] = 1'b1;
                        end
                     end
                     else begin
                        if (m_dbg_min_evts[shire] == 1'b1) begin
                           `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected Debug Event! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
                        end
                        else begin
                           m_dbg_min_evts[shire] = 1'b1;
                        end
                     end
                  end
               end

               case (ifc_type) inside
                  `EVL_DBGNOC_EVT_IFC : begin
                                          foreach (m_active_rt_evt_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_rt_evt_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_rt_evt_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_rt_evt_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                          foreach (m_active_lf_evt_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_lf_evt_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_lf_evt_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_lf_evt_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                          foreach (m_active_dn_evt_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_dn_evt_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_dn_evt_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_dn_evt_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                        end
                  `EVL_DBGNOC_MSG_IFC : begin
                                          foreach (m_active_rt_msg_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_rt_msg_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_rt_msg_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_rt_msg_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                          foreach (m_active_lf_msg_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_lf_msg_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_lf_msg_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_lf_msg_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                          foreach (m_active_dn_msg_req_descs[ii]) begin
                                             if (req_match_found == 1'b0) begin
                                                foreach (m_active_dn_msg_req_descs[ii][shire][jj]) begin
                                                   active_req_desc = m_active_dn_msg_req_descs[ii][shire][jj];
                                                   if (rcv_req_data == active_req_desc.get_req_xword(0)) begin
                                                      `evl_log_mdesc(UVM_DEBUG, VMOD_DBG_NOC_SB, get_abstract_name(), rcv_req_desc, $sformatf("S%0d: %p received DBGNOC REQ matches req sent from S%0d %p", shire, ifc_type, ii, ifc_type))
                                                      m_active_dn_msg_req_descs[ii][shire].delete(jj);
                                                      req_match_found = 1'b1;
                                                      break;
                                                   end
                                                end
                                             end
                                          end
                                        end
               endcase
               if (req_match_found != 1'b1) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d: %p received an unexpected transaction! (%0s)", shire, ifc_type, rcv_req_desc.sprint_obj()))
               end
            end
         end
      join_none
   endtask : watch_for_received_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: match_observed_request
   //
   function evl_req_desc match_observed_request(input evl_req_desc obs_req_desc, input evl_shire_id_t rst_shire_id, input evl_noc_slave_type_t slave_type);

   endfunction : match_observed_request

   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_port_to_my_export
   //
   virtual function void connect_port_to_my_export(input uvm_analysis_port#(evl_req_desc) port_in,
                                                   input string                           port_name      = "unknown",
                                                   input int                              port_number    = 0,
                                                   input int                              subport_number = 0);

      uvm_analysis_export #(evl_req_desc) tlm_port;
      if (port_in == null) begin
         return;
      end
      case (port_name)
         "m_dn_evt_tx_req_export"   : tlm_port = m_dn_evt_tx_req_export[port_number];
         "m_dn_evt_rx_req_export"   : tlm_port = m_dn_evt_rx_req_export[port_number];
         "m_dn_msg_tx_req_export"   : tlm_port = m_dn_msg_tx_req_export[port_number];
         "m_dn_msg_rx_req_export"   : tlm_port = m_dn_msg_rx_req_export[port_number];
         "m_lf_evt_tx_req_export"   : tlm_port = m_lf_evt_tx_req_export[port_number];
         "m_lf_evt_rx_req_export"   : tlm_port = m_lf_evt_rx_req_export[port_number];
         "m_lf_msg_tx_req_export"   : tlm_port = m_lf_msg_tx_req_export[port_number];
         "m_lf_msg_rx_req_export"   : tlm_port = m_lf_msg_rx_req_export[port_number];
         "m_rt_evt_tx_req_export"   : tlm_port = m_rt_evt_tx_req_export[port_number];
         "m_rt_evt_rx_req_export"   : tlm_port = m_rt_evt_rx_req_export[port_number];
         "m_rt_msg_tx_req_export"   : tlm_port = m_rt_msg_tx_req_export[port_number];
         "m_rt_msg_rx_req_export"   : tlm_port = m_rt_msg_rx_req_export[port_number];
         default                    : return;
      endcase

      if (tlm_port != null) begin
         `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_abstract_name(), $sformatf("connecting %0s -> %0s", port_in.get_full_name(), tlm_port.get_full_name()))
         port_in.connect(tlm_port);
      end
      return;
   endfunction : connect_port_to_my_export


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id_from_addr
   //
   function evl_shire_id_t get_shire_id_from_addr (input evl_paddr_t bus_addr, input evl_dbgnoc_ifc_type_t ifc_type, output evl_dbgnoc_ifc_type_t tgt_ifc_type, input evl_shire_id_t shire);
      int            addr_id;

      addr_id = bus_addr[7:0];

      if (addr_id == io_rt_id) begin
         if (!(((ifc_type inside {DBG_RT_EVT, DBG_RT_MSG}) && (shire == 'd254)) || ((ifc_type inside {DBG_LF_EVT, DBG_LF_MSG}) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_RT_EVT : DBG_RT_MSG;
            return 'd254;
         end
      end
      if (addr_id == io_lf_id) begin
         if (!(((ifc_type == DBG_DN_MSG) && ((shire >= 'd0) && (shire <= 'd33))) || ((ifc_type inside {DBG_LF_EVT, DBG_LF_MSG}) && (shire == 'd254)) || ((ifc_type == DBG_DN_MSG) && (shire == 'd253)) || ((ifc_type == DBG_DN_MSG) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_LF_EVT : DBG_LF_MSG;
            return 'd254;
         end
      end
      if (addr_id == ps_dn_id) begin
         if (!(((ifc_type == DBG_DN_MSG) && ((shire >= 'd0) && (shire <= 'd33))) || (shire == 'd253) || ((ifc_type == DBG_LF_MSG) && (shire == 'd254)) || ((ifc_type == DBG_DN_MSG) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_DN_EVT : DBG_DN_MSG;
            return 'd253;
         end
      end

      if ((addr_id >= 'd155) && (addr_id <= 'd162)) begin
         if (!(((ifc_type inside {DBG_RT_EVT, DBG_RT_MSG}) && (shire == 'd254)) || ((ifc_type inside {DBG_LF_EVT, DBG_LF_MSG}) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            foreach (memshire_lf_ids[ii]) begin
               if (memshire_lf_ids[ii] == addr_id) begin
                  if (!((ifc_type inside {DBG_LF_EVT, DBG_LF_MSG}) && (ii == shire))) begin
                     tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_LF_EVT : DBG_LF_MSG;
                     return get_memshire_virt_id_from_index(ii);
                  end
               end
            end
         end
      end
      if ((addr_id >= 'd137) && (addr_id <= 'd151)) begin
         if (!(((ifc_type == DBG_DN_MSG) && ((shire >= 'd0) && (shire <= 'd33))) || ((ifc_type == DBG_LF_MSG) && (shire == 'd254)) || ((ifc_type == DBG_DN_MSG) && (shire == 'd253)) || ((ifc_type == DBG_DN_MSG) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            foreach (memshire_dn_ids[ii]) begin
               if (memshire_dn_ids[ii] == addr_id) begin
                  if (!((ifc_type inside {DBG_DN_EVT, DBG_DN_MSG}) && (get_memshire_virt_id_from_index(ii) == shire))) begin
                     tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_DN_EVT : DBG_DN_MSG;
                     return get_memshire_virt_id_from_index(ii);
                  end
               end
            end
         end
      end
      if ((addr_id >= 'd1) && (addr_id <= 'd133)) begin
         if (!(((ifc_type == DBG_DN_MSG) && ((shire >= 'd0) && (shire <= 'd33))) || ((ifc_type == DBG_LF_MSG) && (shire == 'd254)) || ((ifc_type == DBG_DN_MSG) && (shire == 'd253)) || ((ifc_type == DBG_DN_MSG) && ((shire >= 'd232) && (shire <= 'd239))))) begin
            foreach (minshire_dn_ids[ii]) begin
               if (minshire_dn_ids[ii] == addr_id) begin
                  if (get_minion_shire_virt_id(ii) != shire) begin
                     tgt_ifc_type = (ifc_type inside {`EVL_DBGNOC_EVT_IFC}) ? DBG_DN_EVT : DBG_DN_MSG;
                     return get_minion_shire_virt_id(ii);
                  end
               end
            end
         end
      end

      return ~evl_shire_id_t'(0);
   endfunction : get_shire_id_from_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decode_master_id
   //
   function void decode_master_id (input evl_bus_req_id_t req_id, output evl_shire_id_t shire, output evl_noc_master_type_t master_type, output int noc_layer);

   endfunction : decode_master_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_for_sanity_test_data
   //
   function void check_for_sanity_test_data (input evl_xword_t data);

      foreach(m_sanity_test_data[ii]) begin
         `evl_log(UVM_HIGH, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("check_for_sanity_test_data: Checking data 0x%0x against m_sanity_test_data[%0d] 0x%0x...", data, ii, m_sanity_test_data[ii]))
         if (data == m_sanity_test_data[ii]) begin
            m_sanity_test_data_seen[ii] = 1'b1;
         end
      end

   endfunction : check_for_sanity_test_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.post_shutdown_phase(phase);
         end
      join
      phase.drop_objection(this);
   endtask : post_shutdown_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: check_phase
   //
   virtual function void check_phase(uvm_phase phase);
      bit err_found;
      bit evt_err;

      super.check_phase(phase);

      if (m_dbg_sanity == 1'b1) begin
         if (|m_sanity_test_data_seen != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Sanity Test: S254 RT_EVT did not see expected test data: m_sanity_test_data_seen = 0x%0x", m_sanity_test_data_seen))
         end
         else begin
            `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), "DBG_NOC Sanity Test: S254 RT_EVT saw the expected test data!")
         end
      end

      if (m_dbg_evt_test == 1'b1) begin
         if (|m_dbg_min_evts != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: Not all Minion Shires received the expected Debug event"))
            evt_err = 1'b1;
            foreach (m_dbg_min_evts[ii]) begin
               if (m_dbg_min_evts[ii] == 1'b0) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  S%0d: No event received", ii))
               end
            end
         end
         if (|m_dbg_mc_lf_evts != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: Not all Memshire LF ports received the expected Debug event"))
            evt_err = 1'b1;
            foreach (m_dbg_mc_lf_evts[ii]) begin
               if (m_dbg_mc_lf_evts[ii] == 1'b0) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  S%0d LF: No event received", ii))
               end
            end
         end
         if (|m_dbg_mc_dn_evts != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: Not all Memshire DN ports received the expected Debug event"))
            evt_err = 1'b1;
            foreach (m_dbg_mc_dn_evts[ii]) begin
               if (m_dbg_mc_dn_evts[ii] == 1'b0) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  S%0d DN: No event received", ii))
               end
            end
         end
         if (m_dbg_ios_lf_evt != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: IOShire LF port didn't receive the expected Debug event"))
            evt_err = 1'b1;
         end
         if (m_dbg_ios_rt_evt != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: IOShire RT port didn't receive the expected Debug event"))
            evt_err = 1'b1;
         end
         if (m_dbg_ps_dn_evt != 1'b1) begin
            `dut_error(get_abstract_name(), $sformatf("DBG_NOC Event Test: PShire DN port didn't receive the expected Debug event"))
            evt_err = 1'b1;
         end

         if (evt_err != 1'b1) begin
            `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("DBG_NOC Event Test: All Debug events seen!"))
         end
      end


      `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), "Checking for active Mesh transactions")
      foreach (m_active_dn_evt_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_dn_evt_req_descs[ii][jj]) begin
            if (m_active_dn_evt_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d DN_EVT interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_dn_evt_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_dn_evt_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_dn_msg_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_dn_msg_req_descs[ii][jj]) begin
            if (m_active_dn_msg_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d DN_MSG interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_dn_msg_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_dn_msg_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_lf_evt_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_lf_evt_req_descs[ii][jj]) begin
            if (m_active_lf_evt_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d LF_EVT interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_lf_evt_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_lf_evt_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_lf_msg_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_lf_msg_req_descs[ii][jj]) begin
            if (m_active_lf_msg_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d LF_MSG interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_lf_msg_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_lf_msg_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_rt_evt_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_rt_evt_req_descs[ii][jj]) begin
            if (m_active_rt_evt_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d RT_EVT interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_rt_evt_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_rt_evt_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end
      foreach (m_active_rt_msg_req_descs[ii]) begin
         err_found = 1'b0;
         foreach (m_active_rt_msg_req_descs[ii][jj]) begin
            if (m_active_rt_msg_req_descs[ii][jj].size() != 0) begin
               if (err_found == 1'b0) begin
                  `dut_error(get_abstract_name(), $sformatf("S%0d RT_MSG interface has active REQs unnacounted for:", ii))
                  err_found = 1'b1;
               end
               `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("  Sent to S%0d:", jj))
               foreach (m_active_rt_msg_req_descs[ii][jj][kk]) begin
                  `evl_log(UVM_NONE, VMOD_DBG_NOC_SB, get_abstract_name(), $sformatf("     %0s", m_active_rt_msg_req_descs[ii][jj][kk].sprint_obj()))
               end
            end
         end
      end


   endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_dbgnoc_scoreboard", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("DBG_NOC(SB)");

      if (`et_test_plusargs("disabledbgNOCChecks") != 0) begin
         `evl_set_error_intensity(pk_dbgnoc_sb_error_intensity, UVM_HIGH)
      end
      else begin
         `evl_set_error_intensity(pk_dbgnoc_sb_error_intensity, -1)
      end

      if (`et_test_plusargs("dbgnoc_sanity_test") != 0) begin
         m_dbg_sanity = 1'b1;
      end
      else begin
         m_dbg_sanity = 1'b0;
      end

      if (`et_test_plusargs("dbgnoc_ios_evt_test") != 0) begin
         m_dbg_evt_test = 1'b1;
         m_dbg_ios_lf_evt = 1'b1;
         //m_dbg_ios_rt_evt = 1'b1;
      end
      else if (`et_test_plusargs("dbgnoc_min_evt_test") != 0) begin
         m_dbg_evt_test = 1'b1;
         m_dbg_min_evts = 34'h01;
      end
      else begin
         m_dbg_evt_test = 1'b0;
      end

   endfunction : new

endclass : evl_dbgnoc_scoreboard
