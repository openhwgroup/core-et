//--------------------------------------------------------------------------------------------------
//
// Class: evl_ucblock_scoreboard
//
// The UC SB works as follows:
// 1. Requests can come in from one of the n neighbourhoods in a shire channel
// 2. These requests are captured in the request queues (m_pending_uc_etlink_orig_reqq)
// 3. Based on these requests the UC SB predicts which interface
//    (l3axi or sysaxi or sideband signals) these requests goes out.
// 4. The predicted requests are pushed into a queue (m_ucblock_axi_expected_req)
// 5. Once the predicted request is seen on the corresponding interface the
//    m_ucblock_axi_expected_req is pop'ed. The compare is done by the AXI monitor / SB
// 6. When the response come back on the predicted interface, UC SB creates an expected response
//    output is attached to the original request on the ETLINK
// 7. When the expected response appear on the corresponding interface the m_pending_uc_etlink_orig_reqq
//    is pop'ed. The compare happens at the neighbourhood SB level

class evl_ucblock_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_ucblock_scoreboard)

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   //
   `evl_param_key(pk_uc_etlink_req_port)
   `evl_param_key(pk_etlink_master_count)
   `evl_param_key(pk_uc_sb_error_intensity)
   `evl_param_key(pk_from_axi_stim_agent)
   `evl_param_key(pk_default_cache_attr)

   // analysis port and export for etlink master requests
   uvm_analysis_export   #(evl_req_desc) m_ucblock_etlink_req_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_etlink_req_fifo[];
   // analysis port and export for etlink master responses
   uvm_analysis_export   #(evl_req_desc) m_ucblock_etlink_rsp_export[];
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_etlink_rsp_fifo[];
   // analysis port and export for the AXI L3 Master req and rsp
   uvm_analysis_export   #(evl_req_desc) m_ucblock_l3axi_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_l3axi_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_ucblock_l3axi_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_l3axi_rsp_fifo;
   // analysis port and export for the AXI sys Master req and rsp
   uvm_analysis_export   #(evl_req_desc) m_ucblock_sysaxi_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_sysaxi_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_ucblock_sysaxi_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_sysaxi_rsp_fifo;
   // analysis port and export for the AXI fromsys slave req and rsp
   uvm_analysis_export   #(evl_req_desc) m_ucblock_fromsysaxi_req_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_fromsysaxi_req_fifo;
   uvm_analysis_export   #(evl_req_desc) m_ucblock_fromsysaxi_rsp_export;
   uvm_tlm_analysis_fifo #(evl_req_desc) m_ucblock_fromsysaxi_rsp_fifo;

   // Original ETLink request queue
   evl_req_desc m_pending_uc_etlink_orig_reqq[int][$];
   evl_req_desc m_pending_uc_etlink_req[evl_bus_req_id_t];
   evl_req_desc m_pending_uc_first_bc_req;
   int m_pending_uc_first_bc_portid;
   evl_req_desc m_pending_uc_etlink_msg_rspq[int][$];
   // expected AXI request array for l3 and sys
   evl_req_desc m_ucblock_axi_expected_req[int][evl_bus_req_id_t];
   evl_req_desc m_ucblock_bc_expected_req[evl_bus_req_id_t][$];

   // actual AXI request array for l3 and sys
   evl_req_desc m_ucblock_axi_actual_req[int][evl_bus_req_id_t];
   evl_req_desc m_ucblock_fromsysaxi_req[evl_bus_req_id_t][$];
   evl_ucblock_info_s m_predict_esr_request[$];
   bit [7:0] ucblock_flb_counters[32];
   evl_ucblock_info_s ucblock_flb_reqs[int][$];
   int m_ucblock_axi_bc_req_cnt[$];
   int uc_etlink_req_cnt;
   int bc_has_error;


   evl_line_data             m_template_req_data_sysaxi;
   evl_line_desc             m_template_line_desc_sysaxi;
   evl_trans_desc            m_template_trans_desc_sysaxi;
   evl_req_desc              m_template_req_desc_sysaxi;
   evl_line_data             m_template_req_data_l3axi;
   evl_line_desc             m_template_line_desc_l3axi;
   evl_trans_desc            m_template_trans_desc_l3axi;
   evl_req_desc              m_template_req_desc_l3axi;

   evl_line_data             m_template_req_data_et;
   evl_line_desc             m_template_line_desc_et;
   evl_trans_desc            m_template_trans_desc_et;
   evl_req_desc              m_template_req_desc_et;
   evl_bus_cache_attr_t      m_default_cache_attr;

   extern function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);

   extern virtual function void build_phase(uvm_phase phase);

   extern virtual function void connect_phase(uvm_phase phase);

   extern virtual function void start_of_simulation_phase(uvm_phase phase);

   extern virtual function uvm_analysis_export#(evl_req_desc) get_req_desc_export(input string port_name   = "unknown",
                                                                                  input int port_number    = 0,
                                                                                  input int subport_number = 0);

   extern virtual task run_phase(uvm_phase phase);

   extern task  process_rtl_events();

   extern task  watch_bus_operations();

   extern task  watch_etlink_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo,
                                              input int port_num);

   extern function void process_uc_etlink_reqs(input evl_req_desc req_desc, input int src_id);

   extern function void predict_uc_axi_reqs(evl_ucblock_info_s packet_info);

   extern function void check_uc_esr_reqs(evl_ucblock_info_s packet_info);

   extern function void process_uc_flb_reqs(evl_ucblock_info_s packet_info);

   extern function void check_uc_flb_resp(evl_ucblock_info_s packet_info);

   extern function evl_req_desc generate_axi_desc(input evl_req_desc req_desc, input evl_bus_req_id_t req_id);

   extern function string decode_req_addr(evl_paddr_t req_paddr);

   extern task  watch_sysaxi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   extern task  watch_l3axi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   extern function void process_uc_axi_reqs(input evl_req_desc req_desc, input int axi_port);

   extern task  watch_fromsysaxi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   extern function void process_uc_fromsysaxi_reqs(input evl_req_desc req_desc);

   extern task  watch_sysaxi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   extern task  watch_l3axi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   extern function void process_uc_axi_resp(input evl_req_desc axi_req_desc, input int axi_port);

   extern task  watch_fromsysaxi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   extern function void process_uc_fromsysaxi_resp(input evl_req_desc axi_req_desc);

   extern task  watch_etlink_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo,
                                              input int port_num);

   extern function void process_uc_etlink_resp(input evl_req_desc req_desc, input int src_id);

   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));

      evl_req_desc     match_req_desc;
      evl_bus_cmd_t    bus_cmd;
      evl_paddr_t      address;
      evl_bus_req_id_t req_id;

      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), paired_req_desc,
                     $sformatf("find_observed_request Find matching request for axi_req_id %0d %0s", paired_req_desc.get_bus_req_id(), paired_req_desc.sprint_obj()))


      if (paired_req_desc == null) begin
         bus_cmd  = bus_cmd_in;
         req_id   = req_id_in;
         address  = address_in;
      end
      else begin
         bus_cmd  = (bus_cmd_in  != `EVL_Bus_Idle)         ? bus_cmd_in  : paired_req_desc.get_bus_cmd();
         req_id   = (req_id_in   != ~evl_bus_req_id_t'(0)) ? req_id_in   : paired_req_desc.get_bus_req_id();
         address  = (address_in  != ~evl_paddr_t'(0))      ? address_in  : paired_req_desc.get_paddr();
      end

      if (m_ucblock_axi_expected_req[port_in].exists(paired_req_desc.get_bus_req_id())) begin
         match_req_desc = m_ucblock_axi_expected_req[port_in][paired_req_desc.get_bus_req_id()];
         m_ucblock_axi_expected_req[port_in].delete(paired_req_desc.get_bus_req_id());
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("find_observed_request -  matched axi request, req_id 0x%0x", paired_req_desc.get_bus_req_id()));
      end
      if (match_req_desc == null) begin
         if (port_in == int'(SHIRE_UCTOSYS_MESH_AXI)) begin
            if (m_ucblock_bc_expected_req.exists(paired_req_desc.get_bus_req_id())) begin
               match_req_desc = m_ucblock_bc_expected_req[paired_req_desc.get_bus_req_id()].pop_front();
            end
         end
      end

      if (match_req_desc != null) begin
         if ((paired_req_desc != null) && (paired_req_desc.get_parent_desc() != match_req_desc)) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), match_req_desc,
                     $sformatf("find_observed_request Found matching request for %0s", match_req_desc.sprint_obj()))
         return match_req_desc;
      end
      `evl_log(UVM_LOW, VMOD_UC_SB, get_abstract_name(),
               $sformatf("find_observed_request: cannot find observed request! Returning null, req_id 0x%0x", paired_req_desc.get_bus_req_id()));
      return null;

   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_response
   //
   virtual function evl_req_desc find_observed_response(input int                 port_in,
                                                        input evl_req_desc        paired_req_desc,
                                                        input bit                 compare_fields = 1'b0,
                                                        input byte                qualifier      = 0,
                                                        input evl_bus_rsp_type_t  rsp_type_in    = `EVL_Rsp_Null,
                                                        input evl_bus_req_id_t    req_id_in      = ~evl_bus_req_id_t'(0),
                                                        input evl_bus_rsp_dest_t  dest_id_in     = ~evl_bus_rsp_dest_t'(0),
                                                        input evl_xword_byte_en_t rsp_byte_en_in = evl_xword_byte_en_t'(0),
                                                        input evl_xword_t         rsp_data_in    = evl_xword_t'(0));
      evl_req_desc match_req_desc;

      match_req_desc = m_pending_uc_etlink_msg_rspq[port_in][0];
      if (match_req_desc == null) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                  $sformatf("find_observed_response: cannot find observed response! Returning null"))
         return null;
      end else begin
         if ((match_req_desc.get_bus_rsp_dest() == dest_id_in) & (match_req_desc.get_bus_rsp_id() == req_id_in)) begin
            match_req_desc = m_pending_uc_etlink_msg_rspq[port_in].pop_front();
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), match_req_desc,
                           $sformatf("find_observed_response - found observed response : %0s", match_req_desc.sprint_rsp()))
            `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                     $sformatf("find_observed_response: cache attribute of the request is %d", match_req_desc.get_bus_cache_attr()));
            return match_req_desc;
         end else begin
            `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                     $sformatf("find_observed_response: find observed response does not match! Returning null"))
            return null;
         end // else: !if((match_req_desc.get_bus_rsp_dest() == dest_id_in) & (match_req_desc.get_bus_req_id() == req_id_in))
      end
   endfunction : find_observed_response
endclass : evl_ucblock_scoreboard

//------------------------------------------------------------------------------------------------
//
// Constructor
//
function evl_ucblock_scoreboard::new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
   super.new(name_in, parent_in);
   set_abstract_name("UCBLOCK(SB)");
   if (`et_test_plusargs("disableUCSBChecks") != 0) begin
      `evl_set_error_intensity(pk_uc_sb_error_intensity, -1);
      `dut_warning(get_abstract_name(), "Disabling the UCBLOCK Scoreboard because test plusarg disableUCSBChecks used")
   end else begin
      //`evl_set_error_intensity(pk_uc_sb_error_intensity, UVM_HIGH);
   end
endfunction : new

//------------------------------------------------------------------------------------------------
//
// build_phase
//
function void evl_ucblock_scoreboard::build_phase(uvm_phase phase);

   super.build_phase(phase);

   if (m_rtl_port == null) begin
      `dut_error(get_abstract_name(), "ucblock_scoreboard: m_rtl_port is null!", pk_uc_sb_error_intensity)
   end

   // Get the number of etlink master instances and create the request queue fifo
   if (get_int_param(pk_etlink_master_count) > 0) begin
      m_ucblock_etlink_req_export = new[get_int_param(pk_etlink_master_count)];
      m_ucblock_etlink_req_fifo   = new[get_int_param(pk_etlink_master_count)];

      m_ucblock_etlink_rsp_export = new[get_int_param(pk_etlink_master_count)];
      m_ucblock_etlink_rsp_fifo   = new[get_int_param(pk_etlink_master_count)];
   end
   for (int ii = 0; ii < get_int_param(pk_etlink_master_count); ii++) begin
      m_ucblock_etlink_req_export[ii]  = new($sformatf("m_ucblock_etlink_req_export[%0d]", ii), this);
      m_ucblock_etlink_req_fifo[ii]    = new($sformatf("m_ucblock_etlink_req_fifo[%0d]", ii), this);

      m_ucblock_etlink_rsp_export[ii]  = new($sformatf("m_ucblock_etlink_rsp_export[%0d]", ii), this);
      m_ucblock_etlink_rsp_fifo[ii]    = new($sformatf("m_ucblock_etlink_rsp_fifo[%0d]", ii), this);
   end

   m_ucblock_l3axi_req_export = new($sformatf("m_ucblock_l3axi_req_export"), this);
   m_ucblock_l3axi_req_fifo = new($sformatf("m_ucblock_l3axi_req_fifo"), this);
   m_ucblock_l3axi_rsp_export = new($sformatf("m_ucblock_l3axi_rsp_export"), this);
   m_ucblock_l3axi_rsp_fifo = new($sformatf("m_ucblock_l3axi_rsp_fifo"), this);

   m_ucblock_sysaxi_req_export = new($sformatf("m_ucblock_sysaxi_req_export"), this);
   m_ucblock_sysaxi_req_fifo = new($sformatf("m_ucblock_sysaxi_req_fifo"), this);
   m_ucblock_sysaxi_rsp_export = new($sformatf("m_ucblock_sysaxi_rsp_export"), this);
   m_ucblock_sysaxi_rsp_fifo = new($sformatf("m_ucblock_sysaxi_rsp_fifo"), this);

   m_ucblock_fromsysaxi_req_export = new($sformatf("m_ucblock_fromsysaxi_req_export"), this);
   m_ucblock_fromsysaxi_req_fifo = new($sformatf("m_ucblock_fromsysaxi_req_fifo"), this);
   m_ucblock_fromsysaxi_rsp_export = new($sformatf("m_ucblock_fromsysaxi_rsp_export"), this);
   m_ucblock_fromsysaxi_rsp_fifo = new($sformatf("m_ucblock_fromsysaxi_rsp_fifo"), this);

endfunction : build_phase

//-----------------------------------------------------------------------------------------------
//
// connect_phase
//
function void evl_ucblock_scoreboard::connect_phase(uvm_phase phase);

   super.connect_phase(phase);

   for (int ii = 0; ii < m_ucblock_etlink_req_export.size(); ii++) begin
      m_ucblock_etlink_req_export[ii].connect(m_ucblock_etlink_req_fifo[ii].analysis_export);
      m_ucblock_etlink_rsp_export[ii].connect(m_ucblock_etlink_rsp_fifo[ii].analysis_export);
   end
   // connect the export to the analysis fifo
   m_ucblock_l3axi_req_export.connect(m_ucblock_l3axi_req_fifo.analysis_export);
   m_ucblock_l3axi_rsp_export.connect(m_ucblock_l3axi_rsp_fifo.analysis_export);
   m_ucblock_sysaxi_req_export.connect(m_ucblock_sysaxi_req_fifo.analysis_export);
   m_ucblock_sysaxi_rsp_export.connect(m_ucblock_sysaxi_rsp_fifo.analysis_export);
   m_ucblock_fromsysaxi_req_export.connect(m_ucblock_fromsysaxi_req_fifo.analysis_export);
   m_ucblock_fromsysaxi_rsp_export.connect(m_ucblock_fromsysaxi_rsp_fifo.analysis_export);

endfunction: connect_phase


//-----------------------------------------------------------------------------------------------
//
// start of simulation phase
//
function void evl_ucblock_scoreboard::start_of_simulation_phase(uvm_phase phase);
   int value;

   super.start_of_simulation_phase(phase);

   m_template_req_data_sysaxi   = evl_line_data::type_id::create($sformatf("S%0d:UCTOSYS_MESH(AXI)", get_shire_id()));
   m_template_line_desc_sysaxi  = evl_line_desc::type_id::create($sformatf("S%0d:UCTOSYS_MESH(AXI)", get_shire_id()));
   m_template_trans_desc_sysaxi = evl_trans_desc::type_id::create($sformatf("S%0d:UCTOSYS_MESH(AXI)", get_shire_id()));
   m_template_req_desc_sysaxi = evl_req_desc::type_id::create($sformatf("S%0d:UCTOSYS_MESH(AXI)", get_shire_id()));

   m_template_trans_desc_sysaxi.set_shire_id(get_shire_id());
   m_template_req_desc_sysaxi.set_shire_id(get_shire_id());

   m_template_line_desc_sysaxi.set_data_template(m_template_req_data_sysaxi);
   m_template_trans_desc_sysaxi.set_data_template(m_template_req_data_sysaxi);
   m_template_req_desc_sysaxi.set_data_template(m_template_req_data_sysaxi);
   m_template_req_desc_sysaxi.set_bus_type(EVL_BUS_TYPE_AXI);

   m_template_trans_desc_sysaxi.set_line_template(m_template_line_desc_sysaxi);
   m_template_req_desc_sysaxi.set_line_template(m_template_line_desc_sysaxi);

   m_template_req_data_l3axi   = evl_line_data::type_id::create($sformatf("S%0d:UCTOL3_MESH(AXI)", get_shire_id()));
   m_template_line_desc_l3axi  = evl_line_desc::type_id::create($sformatf("S%0d:UCTOL3_MESH(AXI)", get_shire_id()));
   m_template_trans_desc_l3axi = evl_trans_desc::type_id::create($sformatf("S%0d:UCTOL3_MESH(AXI)", get_shire_id()));
   m_template_req_desc_l3axi = evl_req_desc::type_id::create($sformatf("S%0d:UCTOL3_MESH(AXI)", get_shire_id()));

   m_template_trans_desc_l3axi.set_shire_id(get_shire_id());
   m_template_req_desc_l3axi.set_shire_id(get_shire_id());

   m_template_line_desc_l3axi.set_data_template(m_template_req_data_l3axi);
   m_template_trans_desc_l3axi.set_data_template(m_template_req_data_l3axi);
   m_template_req_desc_l3axi.set_data_template(m_template_req_data_l3axi);
   m_template_req_desc_l3axi.set_bus_type(EVL_BUS_TYPE_AXI);

   m_template_trans_desc_l3axi.set_line_template(m_template_line_desc_l3axi);
   m_template_req_desc_l3axi.set_line_template(m_template_line_desc_l3axi);

   m_template_req_data_et   = evl_line_data::type_id::create($sformatf("S%0d:NEIGH(ETLINK)", get_shire_id()));
   m_template_line_desc_et  = evl_line_desc::type_id::create($sformatf("S%0d:NEIGH(ETLINK)", get_shire_id()));
   m_template_trans_desc_et = evl_trans_desc::type_id::create($sformatf("S%0d:NEIGH(ETLINK)", get_shire_id()));
   m_template_req_desc_et = evl_req_desc::type_id::create($sformatf("S%0d:NEIGH(ETLINK)", get_shire_id()));

   m_template_trans_desc_et.set_shire_id(get_shire_id());
   m_template_req_desc_et.set_shire_id(get_shire_id());

   m_template_line_desc_et.set_data_template(m_template_req_data_et);
   m_template_trans_desc_et.set_data_template(m_template_req_data_et);
   m_template_req_desc_et.set_data_template(m_template_req_data_et);
   m_template_req_desc_et.set_bus_type(EVL_BUS_TYPE_ETLINK);

   m_template_trans_desc_et.set_line_template(m_template_line_desc_et);
   m_template_req_desc_et.set_line_template(m_template_line_desc_et);

   uc_etlink_req_cnt = 0;
   m_pending_uc_first_bc_portid = -1;
   bc_has_error = 0;

   value = get_int_param(pk_default_cache_attr, 15);
   m_default_cache_attr = evl_bus_cache_attr_t'(value & 15);
endfunction : start_of_simulation_phase

function uvm_analysis_export#(evl_req_desc) evl_ucblock_scoreboard::get_req_desc_export(input string port_name = "unknown",
                                                                                        input int port_number = 0,
                                                                                        input int subport_number = 0);
   if (port_name == "m_ucblock_etlink_req_export") begin
      if (port_number < m_ucblock_etlink_req_export.size()) begin
         return m_ucblock_etlink_req_export[port_number];
      end
   end
   if (port_name == "m_ucblock_etlink_rsp_export") begin
      if (port_number < m_ucblock_etlink_rsp_export.size()) begin
         return m_ucblock_etlink_rsp_export[port_number];
      end
   end
   if (port_name == "m_ucblock_l3axi_req_export") begin
      return m_ucblock_l3axi_req_export;
   end
   if (port_name == "m_ucblock_l3axi_rsp_export") begin
      return m_ucblock_l3axi_rsp_export;
   end
   if (port_name == "m_ucblock_sysaxi_req_export") begin
      return m_ucblock_sysaxi_req_export;
   end
   if (port_name == "m_ucblock_sysaxi_rsp_export") begin
      return m_ucblock_sysaxi_rsp_export;
   end
   if (port_name == "m_ucblock_fromsysaxi_req_export") begin
      return m_ucblock_fromsysaxi_req_export;
   end
   if (port_name == "m_ucblock_fromsysaxi_rsp_export") begin
      return m_ucblock_fromsysaxi_rsp_export;
   end
   return null;
endfunction : get_req_desc_export


//----------------------------------------------------------------------------------------------
//
// Task: run_phase
//
task evl_ucblock_scoreboard::run_phase(uvm_phase phase);

   fork
      super.run_phase(phase);
      watch_for_timeouts();
      // peek into RTL
      process_rtl_events();
      watch_bus_operations();
   join_none;

endtask: run_phase

//-----------------------------------------------------------------------------------------------
//
// Tasks: process_rtl_events
//
task automatic evl_ucblock_scoreboard::process_rtl_events();
   evl_ucblock_packet rtl_event;
   evl_ucblock_info_s packet_info;

   if (m_rtl_port == null) begin
      `dut_error(get_abstract_name(), "ucblock_scoreboard: m_rtl_port is null!", pk_uc_sb_error_intensity)
      return;
   end
   forever begin
      evl_monitor_packet rtl_monitor_packet;

      // Get the rtl events
      m_rtl_port.get_event(rtl_monitor_packet);
      if (rtl_monitor_packet.get_packet_type() == UCBLOCK_ETLINK_REQ_INFO) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("found RTL event for etlink req info"))
         $cast(rtl_event, rtl_monitor_packet);
         predict_uc_axi_reqs(rtl_event.get_info());
      end
      else if (rtl_monitor_packet.get_packet_type() == UCBLOCK_ESR_REQ_INFO) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("found RTL event for ESR Request"))
         $cast(rtl_event, rtl_monitor_packet);
         check_uc_esr_reqs(rtl_event.get_info());
      end
      else if (rtl_monitor_packet.get_packet_type() == UCBLOCK_FLB_REQ_INFO) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("found RTL event for FLB Request"))
         $cast(rtl_event, rtl_monitor_packet);
         process_uc_flb_reqs(rtl_event.get_info());
      end
      else if (rtl_event.get_packet_type() == UCBLOCK_FLB_RSP_INFO) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("found RTL event for FLB response"))
         $cast(rtl_event, rtl_monitor_packet);
         check_uc_flb_resp(rtl_event.get_info());
      end
   end // forever begin

endtask : process_rtl_events

//----------------------------------------------------------------------------------------------
//
// Task: Watch for ETLink UC requests
// This task monitors the requests on the ETLinks
task automatic evl_ucblock_scoreboard::watch_bus_operations();
   foreach (m_ucblock_etlink_req_fifo[ii]) begin
      watch_etlink_uc_reqs(m_ucblock_etlink_req_fifo[ii], ii);
   end

   watch_sysaxi_uc_reqs(m_ucblock_sysaxi_req_fifo);

   watch_l3axi_uc_reqs(m_ucblock_l3axi_req_fifo);

   watch_fromsysaxi_uc_reqs(m_ucblock_fromsysaxi_req_fifo);

   watch_sysaxi_uc_resp(m_ucblock_sysaxi_rsp_fifo);

   watch_l3axi_uc_resp(m_ucblock_l3axi_rsp_fifo);

   watch_fromsysaxi_uc_resp(m_ucblock_fromsysaxi_rsp_fifo);

   foreach (m_ucblock_etlink_rsp_fifo[ii]) begin
      watch_etlink_uc_resp(m_ucblock_etlink_rsp_fifo[ii], ii);
   end
endtask : watch_bus_operations



//----------------------------------------------------------------------------------------------
//
// Task: Watch for ETLink UC requests
// This task monitors the requests on the ETLinks
task automatic evl_ucblock_scoreboard::watch_etlink_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo,
                                                            input int port_num);

   evl_req_desc rcv_req_desc;
   if (req_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_etlink_uc_reqs req_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end

   // watch for request from all the neighbourhoods
   fork
      forever begin
         req_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            // process only the UC requests
            if (rcv_req_desc.get_bus_bank() == `SC_BANKS) begin
               `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                              $sformatf("Request from Neighbourhood port num %0d, transid 0x%0x: %0s", port_num, rcv_req_desc.get_trans_id, rcv_req_desc.sprint_obj()))
               process_uc_etlink_reqs(rcv_req_desc, port_num);
            end
         end
      end // forever begin
   join_none

endtask : watch_etlink_uc_reqs


function void evl_ucblock_scoreboard::process_uc_etlink_reqs(input evl_req_desc req_desc, input int src_id);
   evl_req_desc prev_req_desc;

   // Mark the request as observed
   //if (! req_desc.get_bus_cmd() inside {ET_LINK_REQ_Atomic}) begin
   req_desc.set_observed();
   //end
   m_pending_uc_etlink_orig_reqq[src_id].push_back(req_desc);
   uc_etlink_req_cnt += 1;
   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_etlink_reqs(): push to queue m_pending_uc_etlink_orig_reqq of size %0d, req_id 0x%0x, src_id %0d, cache_attr 0x%0x", m_pending_uc_etlink_orig_reqq[src_id].size(), req_desc.get_bus_req_source(), src_id,req_desc.get_bus_cache_attr()))
endfunction : process_uc_etlink_reqs

function void evl_ucblock_scoreboard::predict_uc_axi_reqs(evl_ucblock_info_s packet_info);
   bit to_l3axi;
   bit found = 1'b0;
   evl_req_desc axi_req_desc;
   evl_req_desc req_desc;
   evl_req_desc bc_req_desc;
   evl_paddr_t  req_addr;
   evl_xword_t  axi_data;
   evl_dword_t  req_data;
   int bc_index = 64;

   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs create expected axi request packet info 0x%0x etlinkreq id 0x%0x, axireq id 0x%0x, scr 0x%0x", packet_info.etlink_trans_id, packet_info.etlink_req_id, packet_info.axi_req_id, packet_info.etlink_req_src))
   // get the request in the original etlink queue
   if (m_pending_uc_etlink_orig_reqq[packet_info.etlink_port_id].size() > 0) begin
      foreach (m_pending_uc_etlink_orig_reqq[packet_info.etlink_port_id][ii]) begin
         req_desc = m_pending_uc_etlink_orig_reqq[packet_info.etlink_port_id][ii];
         if ((req_desc.get_bus_req_id() == packet_info.etlink_req_id) && (req_desc.get_bus_req_source() == packet_info.etlink_req_src)) begin
            m_pending_uc_etlink_orig_reqq[packet_info.etlink_port_id].delete(ii);
            break;
         end
      end
   end else begin
      `dut_error(get_abstract_name(), $sformatf("predict_uc_axi_reqs - cannot find a request in m_pending_uc_etlink_orig_reqq queue req_id 0x%0x",
                                                packet_info.axi_req_id), pk_uc_sb_error_intensity)
      return;
   end
   if (req_desc == null) begin
      `dut_error(get_abstract_name(), $sformatf("predict_uc_axi_reqs - cannot find a request in m_pending_uc_etlink_orig_reqq queue req_id 0x%0x rtl req id 0x%0x", req_desc.get_bus_req_id(),packet_info.axi_req_id), pk_uc_sb_error_intensity)
      return;
   end
   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("req_desc data is %s", req_desc.format_req_data()))
   // CHECKER - Check that the size is always less than or equal to 256b(32B)
   if (req_desc.get_bus_req_size() inside { ET_LINK_Line, 3'b111 }) begin
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs the ETLink request size is incorrectly set to 0x%0x", req_desc.get_bus_req_size()))
      req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
      `dut_warning(get_abstract_name(), "process_uc_etlink_reqs the ETLink request size is incorrectly set!", pk_uc_sb_error_intensity)
      // Generate error Response
      req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
      req_desc.set_bus_rsp_dest(req_desc.get_bus_req_source());
      req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Err));
      req_desc.set_bus_rsp_valid(0, get_cycle_count());
      return;
   end

   // Check for broadcast
   if (req_desc.get_paddr() inside { 40'h013FF5FFF0 }) begin
      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                     $sformatf("process_uc_axi_reqs - predict_uc_axi_reqs found first broadcast request: %0s", req_desc.sprint_obj()))
      // first broadcast request received. this does not cause any request. Just returns a response.
      // set the response and destination id here.
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs ET_LINK_RSP_Ack resp for first broadcast request"))
      req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
      req_desc.set_bus_rsp_dest(req_desc.get_bus_req_source());
      req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Ack));
      req_desc.set_bus_rsp_valid(0, get_cycle_count());
      m_pending_uc_first_bc_req = req_desc;
      m_pending_uc_first_bc_portid = packet_info.etlink_port_id;
      return;
   end // if (req_desc.get_paddr() inside { 40'h013FF5FFF0 })
   else if (req_desc.get_paddr() inside { 40'h013FF5FFF8, 40'h017FF5FFF8, 40'h01FFF5FFF8, 40'h01BFF5FFF8 }) begin
      evl_paddr_t  req_paddr;
      // If Broadcast req received after the Broadcast ESR request
      // if the broadcast request source does not match or
      // if the write address not equal to one of the four protection mode addresses generate error response
      // or if broadcast req received without the broadcast ESR
      if (( ( m_pending_uc_first_bc_portid >= 0 ) & ((m_pending_uc_first_bc_req.get_bus_req_source() != req_desc.get_bus_req_source()) |
                                                    (m_pending_uc_first_bc_portid != packet_info.etlink_port_id))) |
          (m_pending_uc_first_bc_portid == -1 )) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("req_desc data is bcesr src = 0x%0x, bc src = 0x%0x, bcesrportid = 0x%0x, bcportid = 0x%0x",
                                                                       m_pending_uc_first_bc_req.get_bus_req_source(), req_desc.get_bus_req_source(),
                                                                       m_pending_uc_first_bc_portid, packet_info.etlink_req_id))
         `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                        $sformatf("predict_uc_axi_reqs Predict ET_LINK_RSP_Err resp for illegal broadcast request: %0s", req_desc.sprint_obj()))
         req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
         req_desc.set_bus_rsp_dest(req_desc.get_bus_req_source());
         req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Err));
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
         return;
      end // if (( ( m_pending_uc_first_bc_portid >= 0 ) & ((m_pending_uc_first_bc_req.get_bus_req_source() != req_desc.get_bus_req_source()) |...

      // clear the port_id
      //m_pending_uc_first_bc_portid = -1;

      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), m_pending_uc_first_bc_req,
                     $sformatf("predict_uc_axi_reqs - process the broadcast esr : %0s", m_pending_uc_first_bc_req.sprint_obj()))
      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                     $sformatf("predict_uc_axi_reqs - process the broadcast request : %0s", req_desc.sprint_obj()))
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("req_desc data is %s", req_desc.format_req_data()))
      // get the broadcast data
      req_data = req_desc.get_req_dword(-1);
      req_paddr = req_desc.get_paddr();
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - broadcast data 0x%0x", m_pending_uc_first_bc_req.get_req_dword(-1)))
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs broadcast fields 0x%0x address 0x%0x", req_data, req_paddr[31:30]))
      req_addr = evl_paddr_t'({8'b00000001, req_paddr[31:30], 8'b00000000, req_data[58:40], 3'b000});
      // create the shire mask
      req_data &= evl_dword_t'(40'hffffffffff);
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs generate multiple AXI requests 0x%0x", req_data))
      // generate multiple expected AXI requests
      for (int i = 0; i <= 39; i++) begin
         if (req_data & (40'h0 | (1'b1 << i))) begin
            axi_req_desc = generate_axi_desc(req_desc, packet_info.axi_req_id);
            axi_req_desc.set_bus_req_id(evl_bus_req_id_t'(bc_index));
            // QOS always 3 for broadcast
            axi_req_desc.set_bus_qos(evl_bus_qos_t'(3));
       if (req_desc.bus_cmd_is_write() == 1'b1) begin
               axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
       end
       else begin
               axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_READ));
       end
            if (i >= 32) begin
               req_addr[29:22] = 232+(i%32);
            end
            else begin
               req_addr[29:22] = i;
            end
            axi_req_desc.set_paddr(req_addr);
            // set the request data from the first broadcast request
            axi_req_desc.set_req_dword(-1, m_pending_uc_first_bc_req.get_req_dword(-1));
            void'(req_desc.add_child_desc(axi_req_desc));
            m_ucblock_bc_expected_req[bc_index].push_back(axi_req_desc);
            m_ucblock_axi_bc_req_cnt.push_back(bc_index++);
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), axi_req_desc, $sformatf("predict_uc_axi_reqs generating expected broadcast AXI request axi id 0x%0x index 0x%0x: %s", axi_req_desc.get_bus_req_id(), bc_index, axi_req_desc.sprint_obj()))
         end // if (req_data & (40'h0 | (1'b1 << i)))
      end // for (int i = 0; i <= 39; i++)
      // marker to end the number of broadcasts
      m_ucblock_axi_bc_req_cnt.push_back(int'(packet_info.axi_req_id));
      return;
   end
   else if (m_pending_uc_first_bc_portid >= 0) begin
      if ((m_pending_uc_first_bc_req.get_bus_req_source() == req_desc.get_bus_req_source()) &
          (m_pending_uc_first_bc_portid == packet_info.etlink_port_id)) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs broadcast received non-broadcast request %h, %h, %h, %h", m_pending_uc_first_bc_req.get_bus_req_source(), req_desc.get_bus_req_source(), m_pending_uc_first_bc_portid, packet_info.etlink_req_id))
         /*
         // if the broadcast esr is received but the next request is not broadcast request expect an error
         `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                        $sformatf("predict_uc_axi_reqs Predict ET_LINK_RSP_Err resp for illegal request after broadcast esr: %0s", req_desc.sprint_obj()))
         req_desc.set_bus_rsp_id(req_desc.get_bus_req_id());
         req_desc.set_bus_rsp_dest(req_desc.get_bus_req_source());
         req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Err));
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
         return;
          */
      end // if ((m_pending_uc_first_bc_req.get_bus_req_source() == req_desc.get_bus_req_source()) &...
   end // if (m_pending_uc_first_bc_portid >= 0)

   if (m_pending_uc_etlink_req.exists(packet_info.axi_req_id)) begin
      req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
      `dut_error(get_abstract_name(), $sformatf("predict_uc_axi_reqs - Request ID 0x%0x already exist. Illegal UC request queue state.", packet_info.axi_req_id), pk_uc_sb_error_intensity)
   end
   m_pending_uc_etlink_req[packet_info.axi_req_id] = req_desc;

   axi_req_desc = generate_axi_desc(req_desc, packet_info.axi_req_id);
   // decode the request operation
   case (req_desc.get_bus_cmd())
     ET_LINK_REQ_Read, ET_LINK_REQ_Write: begin
        axi_req_desc.set_req_data(req_desc.get_req_data());
        if (req_desc.bus_cmd_is_write() == 1'b1) begin
           axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
        end
        else begin
           axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_READ));
        end
        // update the AXI address for ESR request
        req_addr = req_desc.get_paddr();
        // why replace ff to shireid - as per PRM-19 - Section 2.4 The ESR Region
        if ((req_addr[39:32] == 8'b00000001) && (req_addr[29:22] == 8'hff)) begin
           req_addr[29:22] = get_shire_id();
        end
        axi_req_desc.set_paddr(req_addr);
        if ( ((req_addr[39] == 1'b1) && packet_info.uc_l3_enabled) || ((req_addr[39:31] == 'h1) && packet_info.uc_scp_enabled) ) begin
           axi_req_desc.set_bus_qos(evl_bus_qos_t'(packet_info.uc_mem_qos));
           if (req_addr[39]) begin
              axi_req_desc.set_bus_qos(evl_bus_qos_t'(packet_info.uc_mem_qos));
           end else begin
              axi_req_desc.set_bus_qos(evl_bus_qos_t'(3));
           end
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs push to m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)] of size 0x%0x transid 0x%0x: ET_LINK_REQ_Read, ET_LINK_REQ_Write", m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)].size(), axi_req_desc.get_trans_id()))
           m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)][axi_req_desc.get_bus_req_id()] = axi_req_desc;
        end else begin
           if (req_addr[39]) begin
              axi_req_desc.set_bus_qos(evl_bus_qos_t'(packet_info.uc_mem_qos));
           end else if (req_addr[39:32] == 8'b00000001) begin
              axi_req_desc.set_bus_qos(evl_bus_qos_t'(3));
           end else begin
              axi_req_desc.set_bus_qos(evl_bus_qos_t'(3));
           end
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs push to m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)] of size 0x%0x transid 0x%0x: ET_LINK_REQ_Read, ET_LINK_REQ_Write", m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)].size(), axi_req_desc.get_trans_id()))
           m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)][axi_req_desc.get_bus_req_id()] = axi_req_desc;
        end
     end
     // UC messages
     ET_LINK_REQ_MsgSendData: begin
        req_addr = req_desc.get_paddr();
        // why replace ff to shireid - as per PRM-19 - Section 2.4 The ESR Region
        if (req_addr[29:22] == 8'hff) begin
           req_addr[29:22] = get_shire_id();
        end
        axi_req_desc.set_paddr(req_addr);
        axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
        `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("%0s: processing %0s ... size is %0d, addr is 0x%x, 0x%x", get_abstract_name(), req_desc.sprint_obj(), 1 << req_desc.get_bus_req_size(), req_desc.get_paddr(), req_desc.get_req_oword(0)));
        case (1 << req_desc.get_bus_req_size())
          1:       axi_req_desc.set_req_byte (-1, req_desc.get_req_oword(0));
          2:       axi_req_desc.set_req_hword(-1, req_desc.get_req_oword(0));
          4:       axi_req_desc.set_req_word (-1, req_desc.get_req_oword(0));
          8:       axi_req_desc.set_req_dword(-1, req_desc.get_req_oword(0));
          16:      axi_req_desc.set_req_qword(-1, req_desc.get_req_oword(0));
          32:      axi_req_desc.set_req_oword(-1, req_desc.get_req_oword(0));
          default: axi_req_desc.set_req_xword(-1, req_desc.get_req_oword(0));
        endcase
        // Should be always 2 for Message requests
        axi_req_desc.set_bus_qos(evl_bus_qos_t'(2));
        m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)][axi_req_desc.get_bus_req_id()] = axi_req_desc;
        `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs push to m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)] of size 0x%0x transid 0x%0x: ET_LINK_REQ_MsgSendData", m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)].size(), axi_req_desc.get_trans_id()))
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - data - %s", axi_req_desc.format_req_data()))
     end
     // Global Atomics
     ET_LINK_REQ_Atomic: begin
        // CHECKER - if the qwen is not set properly die - shire connection spec - CHECKER
        if (! (req_desc.get_bus_req_qword_en() inside { 4'b0001, 4'b0011})) begin
           req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
           `dut_error(get_abstract_name(), $sformatf("predict_uc_axi_reqs: qwen %0b is incorrectly set on the ETLink request", req_desc.get_bus_req_qword_en()), pk_uc_sb_error_intensity)
        end
        axi_req_desc.set_req_data(req_desc.get_req_data());
        axi_req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
        req_addr = req_desc.get_paddr();
        axi_req_desc.set_paddr(req_desc.get_paddr() & evl_paddr_t'(~(40'h3f)));
        // form the data bus
        axi_data = '0;
        axi_data[281:277] = packet_info.axi_req_id;
        axi_data[276:269] = get_shire_id();
        axi_data[268:263] = req_addr[5:0];
        axi_data[262:256] = req_desc.get_bus_subopcode_dest();
        axi_data[255:0] =  req_desc.get_req_oword(0);
        // hack / fix for VERIF-3520 AMO_Cmp_Swap
        if ( axi_data[259:256] == 4'hb) begin
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("xxxXXXxxx doing this size 0x%0x conf 0x%0x", req_desc.get_bus_req_size(), axi_data[259:256]))
           case ( 1 << req_desc.get_bus_req_size() )
             4:       axi_req_desc.set_req_word ( 2, evl_word_t'(axi_data[64+:32]) );
             8:       axi_req_desc.set_req_dword( 1, evl_dword_t'(axi_data[64+:64]) );
           endcase // case ( 1 << req_desc.get_bus_req_size() )
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("xxxXXXxxx predict_uc_axi_reqs - data - %s", axi_req_desc.format_req_data()))
        end

        axi_req_desc.set_req_word(8, evl_word_t'(axi_data[287:256]));
        axi_req_desc.set_bus_req_size(evl_bus_req_size_t'(4'b0110));
        axi_req_desc.set_bus_qos(evl_bus_qos_t'(packet_info.uc_mem_qos));
        // push the request into the expected queue
        // if l3 enabled or if scp enable push to l3
        if ( (packet_info.uc_l3_enabled) || (packet_info.uc_scp_enabled && (req_addr[39:31] == 'h1)) ) begin
           m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)][axi_req_desc.get_bus_req_id()] = axi_req_desc;
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - push to m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)] of size 0x%0x transid 0x%0x qworden 0x%0x subopcode 0x%0x: ET_LINK_REQ_Atomic", m_ucblock_axi_expected_req[int'(SHIRE_UCTOL3_MESH_AXI)].size(), axi_req_desc.get_trans_id(), axi_req_desc.get_bus_req_qword_en(), req_desc.get_bus_subopcode_dest()))
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - data - %s", axi_req_desc.format_req_data()))
        end
        else begin
           m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)][axi_req_desc.get_bus_req_id()] = axi_req_desc;
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - push to m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)] of size 0x%0x transid 0x%0x qworden 0x%0x subopcode 0x%0x: ET_LINK_REQ_Atomic", m_ucblock_axi_expected_req[int'(SHIRE_UCTOSYS_MESH_AXI)].size(), axi_req_desc.get_trans_id(), axi_req_desc.get_bus_req_qword_en(), req_desc.get_bus_subopcode_dest()))
           `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - data - %s", axi_req_desc.format_req_data()))
        end
     end
     default: begin
        req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
        `dut_error(get_abstract_name(), $sformatf("predict_uc_axi_reqs: Unexpected command %0d on the etlink",req_desc.get_bus_cmd()), pk_uc_sb_error_intensity)
     end
   endcase // case (req_desc.get_bus_cmd())

   void'(req_desc.add_child_desc(axi_req_desc));

   req_desc.set_processed();
   `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), axi_req_desc, $sformatf("predict_uc_axi_reqs - Generated expected AXI request %s%s", decode_req_addr(axi_req_desc.get_paddr()), axi_req_desc.sprint_obj()))

endfunction : predict_uc_axi_reqs

function void evl_ucblock_scoreboard::check_uc_esr_reqs(evl_ucblock_info_s packet_info);
   evl_req_desc fromsysaxi_req_desc;
   evl_ucblock_info_s exp_packet_info;
   exp_packet_info =  m_predict_esr_request.pop_front();
   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("check_uc_esr_reqs - check the fcc and flb side band signals fcc req 0x%0x flb req 0x%0x wdata 0x%0x", packet_info.uc_fcc_enables_in, packet_info.uc_flb_enables_in, packet_info.uc_esr_wdata_in))
   if ((exp_packet_info.uc_fcc_enables_in != packet_info.uc_fcc_enables_in) ||
       (exp_packet_info.uc_flb_enables_in != packet_info.uc_flb_enables_in) ||
       (exp_packet_info.uc_esr_wdata_in != packet_info.uc_esr_wdata_in) ||
       (exp_packet_info.uc_to_neighen_fcc_out != packet_info.uc_to_neighen_fcc_out) ||
       (exp_packet_info.uc_to_neighdata_fcc_out != packet_info.uc_to_neighdata_fcc_out)) begin
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
               $sformatf("check_uc_esr_reqs - exp fccen 0x%0x == act fccen 0x%0x, exp flben 0x%0x == act flben 0x%0x, exp wdata 0x%0x == act wdata 0x%0x, exp fcc_out 0x%0x == act fcc_out 0x%0x, exp fccd_out 0x%0x == act fccd_out 0x%0x",
                         exp_packet_info.uc_fcc_enables_in, packet_info.uc_fcc_enables_in,
                         exp_packet_info.uc_flb_enables_in, packet_info.uc_flb_enables_in,
                         exp_packet_info.uc_esr_wdata_in, packet_info.uc_esr_wdata_in,
                         exp_packet_info.uc_to_neighen_fcc_out, packet_info.uc_to_neighen_fcc_out,
                         exp_packet_info.uc_to_neighdata_fcc_out, packet_info.uc_to_neighdata_fcc_out))
      `dut_error(get_abstract_name(), "check_uc_esr_reqs - Mismatch on the FCC/FLB interface", pk_uc_sb_error_intensity)
   end else begin // if ((exp_packet_info.uc_fcc_enables_in != packet_info.uc_fcc_enables_in) ||...
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
               $sformatf("check_uc_esr_reqs - FCC/FLB request matches!!"))
   end // else: !if((exp_packet_info.uc_fcc_enables_in != packet_info.uc_fcc_enables_in) ||...
   // keep track of the flb writes through ETLINK
   if (exp_packet_info.uc_flb_enables_in) begin
      for (int ii = 0; ii < 32; ii++) begin
         if (exp_packet_info.uc_flb_enables_in[ii]) begin
            ucblock_flb_counters[ii] = packet_info.uc_esr_wdata_in[12:5];
            `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                     $sformatf("check_uc_esr_reqs - SW init FLB counter 0x%0x = 0x%0x",ii, packet_info.uc_esr_wdata_in[12:5]))
         end
      end
   end
endfunction : check_uc_esr_reqs


function void evl_ucblock_scoreboard::process_uc_flb_reqs(evl_ucblock_info_s packet_info);

   for (int ii = 0; ii < 4; ii++) begin
      evl_ucblock_info_s exp_packet_info;
      if (packet_info.uc_flb_req_valid[ii]) begin
         // neigh ii accesses one of the 32 flb reg
         ucblock_flb_reqs[ii].push_back(packet_info);
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                  $sformatf("process_uc_flb_reqs - Received FLB CSR Req for counter 0x%0x, compare value 0x%0x neigh 0x%0x",
                            packet_info.uc_flb_req_data[ii][4:0], packet_info.uc_flb_req_data[ii][12:5], ii))
      end
   end

endfunction : process_uc_flb_reqs


function void evl_ucblock_scoreboard::check_uc_flb_resp(evl_ucblock_info_s packet_info);
   evl_ucblock_info_s exp_packet_info;
   evl_ucblock_info_s req_packet_info;

   for (int ii = 0; ii < 4; ii++) begin
      if (packet_info.uc_flb_resp_valid[ii]) begin
         req_packet_info = ucblock_flb_reqs[ii].pop_front();
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                  $sformatf("check_uc_flb_resp - Received FLB Response neigh 0x%0x counter 0x%0x = (SB value 0x%0x)(rtl compareval 0x%0x)",
                            ii, req_packet_info.uc_flb_req_data[ii][4:0], ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]], req_packet_info.uc_flb_req_data[ii][12:5]))
         if (ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]] == req_packet_info.uc_flb_req_data[ii][12:5]) begin
            if (! packet_info.uc_flb_resp_data[ii]) begin
               `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                        $sformatf("check_uc_flb_resp counter 0x%0x counterval 0x%0x, compare_val 0x%0x, neigh 0x%0x",
                                  req_packet_info.uc_flb_req_data[ii][4:0],
                                  ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]],
                                  req_packet_info.uc_flb_req_data[ii][12:5], ii))
               `dut_error(get_abstract_name(), "check_uc_flb_resp - Mismatch on the FLB response interface", pk_uc_sb_error_intensity)
            end else begin
               `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                        $sformatf("check_uc_flb_resp - FLB request and response matches!!"))
            end
            ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]] = 0;
         end else begin
            ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]] += 1;
               `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(),
                        $sformatf("check_uc_flb_resp - FLB increment counter %h newval %h compareval 0x%0x",
                                  req_packet_info.uc_flb_req_data[ii][4:0],
                                  ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]],
                                  req_packet_info.uc_flb_req_data[ii][12:5]))
         end // else: !if(ucblock_flb_counters[req_packet_info.uc_flb_req_data[ii][4:0]] == req_packet_info.uc_flb_req_data[ii][12:5])
      end
   end // for (int ii = 0; ii < 4; ii++)

endfunction : check_uc_flb_resp

function evl_req_desc evl_ucblock_scoreboard::generate_axi_desc(input evl_req_desc req_desc, input evl_bus_req_id_t req_id);

   evl_req_desc axi_req_desc;

   // create an expected AXI request
   if (req_desc.get_bus_cmd() inside { ET_LINK_REQ_Read, ET_LINK_REQ_Write, ET_LINK_REQ_MsgSendData }) begin
      if (!$cast(axi_req_desc, m_template_req_desc_sysaxi.clone())) begin
         `dut_error(get_abstract_name(), "generate_axi_desc -  axi request cast failed", pk_uc_sb_error_intensity)
      end
   end
   else begin
      if (!$cast(axi_req_desc, m_template_req_desc_l3axi.clone())) begin
         `dut_error(get_abstract_name(), "generate_axi_desc - axi request cast failed", pk_uc_sb_error_intensity)
      end
   end
   axi_req_desc.set_bus_bank(`SC_BANKS);
   req_desc.copy_to_desc(axi_req_desc);
   axi_req_desc.set_abstract_name(get_abstract_name());
   axi_req_desc.set_bus_req_size(req_desc.get_bus_req_size());
   axi_req_desc.set_bus_req_id(req_id);
   axi_req_desc.set_bus_lock(evl_bus_lock_t'(0));
   if (req_desc.bus_cmd_is_write() == 1'b1) begin
      axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(req_desc.get_bus_cmd()));
   end
   else begin
      axi_req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
   end
   axi_req_desc.set_bus_cache_attr(m_default_cache_attr);
   return axi_req_desc;

endfunction : generate_axi_desc

function string evl_ucblock_scoreboard::decode_req_addr(evl_paddr_t req_paddr);
   // ESR request
   if (req_paddr[39:32] == 8'h01) begin
      // Shire ESR
      if (req_paddr[21:17] == 5'h1a) begin
         if (req_paddr[16:3] inside {[14'h18:14'h1b]}) begin
            return "for Shire ESR FCC request ";
         end else if (req_paddr[16:3] inside {[14'h20:14'h3f]}) begin
            return "for Shire ESR FLB request ";
         end
      end
   end
   return "";
endfunction : decode_req_addr

//-----------------------------------------------------------------------------
//
// Task: Watch for UC sysaxi requests
//
task automatic evl_ucblock_scoreboard::watch_sysaxi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   evl_req_desc rcv_req_desc;
   if (req_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_sysaxi_uc_reqs req_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end
   `evl_log(UVM_MEDIUM, VMOD_UC_SB, get_abstract_name(), $sformatf("UCBlock scoreboard watch_sysaxi_uc_reqs"))
   // watch for request from all the neighbourhoods
   fork
      forever begin
         req_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_sysaxi_uc_reqs Observed a sysaxi request : %0s", rcv_req_desc.sprint_obj()))
            process_uc_axi_reqs(rcv_req_desc, int'(SHIRE_UCTOSYS_MESH_AXI));
         end
      end
   join_none

endtask : watch_sysaxi_uc_reqs


//-----------------------------------------------------------------------------
//
// Task: Watch for UC l3axi requests
//
task automatic evl_ucblock_scoreboard::watch_l3axi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   evl_req_desc rcv_req_desc;
   if (req_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_l3axi_uc_reqs req_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end
   `evl_log(UVM_MEDIUM, VMOD_UC_SB, get_abstract_name(), $sformatf("UCBlock scoreboard watch_l3axi_uc_reqs"))
   // watch for request from all the neighbourhoods
   fork
      forever begin
         req_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_l3axi_uc_reqs Observed a l3axi request : %0s", rcv_req_desc.sprint_obj()))
            process_uc_axi_reqs(rcv_req_desc, int'(SHIRE_UCTOL3_MESH_AXI));
         end
      end
   join_none

endtask : watch_l3axi_uc_reqs


function void evl_ucblock_scoreboard::process_uc_axi_reqs(input evl_req_desc req_desc, input int axi_port);
   evl_bus_req_id_t bus_req_id = req_desc.get_bus_req_id();

   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_reqs bus req id is 0x%0x", bus_req_id))
   // CHECKER - if another request is received with and ID same as a request outstanding, then die - CHECKER
   if ((m_ucblock_axi_actual_req[int'(SHIRE_UCTOSYS_MESH_AXI)].exists(bus_req_id)) || m_ucblock_axi_actual_req[int'(SHIRE_UCTOL3_MESH_AXI)].exists(bus_req_id)) begin
      req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
      `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_uc_axi_reqs There is an outstanding AXI request with id 0x%0x: outstanding req: %0s - new request: %0s", bus_req_id, m_ucblock_axi_actual_req[axi_port][bus_req_id].sprint_obj(), req_desc.sprint_obj()), pk_uc_sb_error_intensity)
   end
   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_reqs push to m_ucblock_axi_actual_req[%0d] (%0d = sys, %0d = l3) of size %0d bus req id 0x%0x transid 0x%0x", axi_port, int'(SHIRE_UCTOSYS_MESH_AXI), int'(SHIRE_UCTOL3_MESH_AXI), m_ucblock_axi_actual_req[axi_port].size(), bus_req_id, req_desc.get_trans_id()))
   // push the axi request into the request queue
   m_ucblock_axi_actual_req[axi_port][bus_req_id] = req_desc;
   // set the timeout on the request
   add_active_mem_desc(req_desc);

endfunction : process_uc_axi_reqs


//-----------------------------------------------------------------------------
//
// Task: Watch for UC fromsysaxi requests
//
task automatic evl_ucblock_scoreboard::watch_fromsysaxi_uc_reqs(input uvm_tlm_analysis_fifo #(evl_req_desc) req_fifo);

   evl_req_desc rcv_req_desc;
   if (req_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_fromsysaxi_uc_reqs req_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end
   // watch for request from all the neighbourhoods
   fork
      forever begin
         req_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            if (!rcv_req_desc.bus_cmd_is_read()) begin
               `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("watch_fromsysaxi_uc_reqs - req addr 0x%0x", rcv_req_desc.get_paddr()))
               process_uc_fromsysaxi_reqs(rcv_req_desc);
            end
         end
      end
   join_none

endtask : watch_fromsysaxi_uc_reqs


function void evl_ucblock_scoreboard::process_uc_fromsysaxi_reqs(input evl_req_desc req_desc);
   // UCblock gets the FROMSYS AXI write request for messages or atomics
   evl_trans_desc trans_desc;
   evl_req_desc etlink_desc;
   evl_paddr_t paddr;
   evl_ucblock_info_s  packet_info;

   bit valid_uc = 0;

   trans_desc = req_desc.get_root_trans_desc();
   if ((trans_desc != null) && (trans_desc.get_int_param(pk_from_axi_stim_agent, 0) > 0)) begin
      // If the originating request was from the L3 stimulus agent (which can happen in shire cache
      // standalone testing), ignore this request descriptor.
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("I was here"))
      //return;
   end
   paddr = req_desc.get_paddr();
   // Check for FCC and FLB write requests
   if ((paddr[39:32] == 8'b00000001) && (paddr[29:22] == get_shire_id()) && (paddr[21:17] == 5'h1a) && (paddr[16:3] inside {[14'h18:14'h1b], [14'h20:14'h3f]})) begin
      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                     $sformatf("process_uc_fromsysaxi_reqs Observed a fromsysaxi FCC / FLB request : %0s", req_desc.sprint_obj()))
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_fromsysaxi_reqs - data - %s", req_desc.format_req_data()))

      // clear out the enable bits
      {packet_info.uc_fcc_enables_in, packet_info.uc_flb_enables_in, packet_info.uc_to_neighen_fcc_out,
       packet_info.uc_to_neighdata_fcc_out, packet_info.uc_fcc_enables_in, packet_info.uc_flb_enables_in, packet_info.uc_esr_wdata_in} = 'b0;
      packet_info.uc_flb_enables_in[paddr[7:3]] = (paddr[16:3] inside {[14'h20:14'h3f]});
      if (paddr[16:3] inside {[14'h18:14'h1b]}) begin
         packet_info.uc_to_neighen_fcc_out = paddr[4:3];
         packet_info.uc_to_neighdata_fcc_out = req_desc.get_req_dword(-1);
         packet_info.uc_fcc_enables_in[paddr[4:3]] = 1'b1;
      end
      packet_info.uc_esr_wdata_in = req_desc.get_req_dword(-1);
      m_predict_esr_request.push_back(packet_info);
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_fromsysaxi_reqs - fccen 0x%0x fccdata 0x%0x flben 0x%0x flb data - 0x%0x desc data 0x%0x", packet_info.uc_to_neighen_fcc_out, packet_info.uc_to_neighdata_fcc_out, packet_info.uc_flb_enables_in, packet_info.uc_esr_wdata_in, req_desc.get_req_dword(-1)))
      valid_uc = 1'b1;
   end
   // Check if this is a message
   else if ((paddr[39:32] == 8'b00000001) && (paddr[21:20] == 2'b00) && (paddr[11] == 1'b1)) begin
      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                     $sformatf("process_uc_fromsysaxi_reqs Observed a fromsysaxi ET_LINK_REQ_MsgSendData request id = 0x%0x, dest = 0x%0x, port_id = 0x%0x: %0s", paddr[10:3], paddr[15:12], paddr[19:16], req_desc.sprint_obj()))
      // set the expected ETLink reponse for the message
      if (!$cast(etlink_desc, m_template_req_desc_et.clone())) begin
         `dut_error(get_abstract_name(), "process_uc_fromsysaxi_reqs - etlink request cast failed", pk_uc_sb_error_intensity)
      end
      req_desc.copy_to_desc(etlink_desc);
      req_desc.add_child_desc(etlink_desc);
      etlink_desc.set_abstract_name(get_abstract_name());
      etlink_desc.set_bus_rsp_type(evl_bus_rsp_type_t'(ET_LINK_RSP_MsgRcvData));
      etlink_desc.set_bus_rsp_valid(0, get_cycle_count());
      etlink_desc.set_bus_rsp_size(req_desc.get_bus_req_size());
      etlink_desc.set_bus_rsp_id(evl_bus_rsp_id_t'(paddr[10:3]));
      etlink_desc.set_bus_rsp_dest(evl_bus_rsp_dest_t'(paddr[15:12]));
      case (1 << req_desc.get_bus_req_size())
        1:       etlink_desc.set_rsp_byte (0, req_desc.get_req_oword(paddr[5]));
        2:       etlink_desc.set_rsp_hword(0, req_desc.get_req_oword(paddr[5]));
        4:       etlink_desc.set_rsp_word (0, req_desc.get_req_oword(paddr[5]));
        8:       etlink_desc.set_rsp_dword(0, req_desc.get_req_oword(paddr[5]));
        16:      etlink_desc.set_rsp_qword(0, req_desc.get_req_oword(paddr[5]));
        32:      etlink_desc.set_rsp_oword(0, req_desc.get_req_oword(paddr[5]));
        default: etlink_desc.set_rsp_xword(0, req_desc.get_req_oword(paddr[5]));
      endcase
      //etlink_desc.set_rsp_data(req_desc.get_req_data());
      //      etlink_desc.set_trans_id(req_desc.get_trans_id());
      // save this in the expected etlink rsp queue
      m_pending_uc_etlink_msg_rspq[paddr[19:16]].push_back(etlink_desc);
      valid_uc = 1'b1;
   end // if ((paddr[39:32] == 8'b00000001) && (paddr[21:20] == 2'b00) && (paddr[11] == 1'b1))
   else if ((paddr[39:32] == 8'b00000001) && (paddr[31:30] == 2'b10) && (paddr[21:17] == 5'b11111) && (paddr[16:6] == 11'b0)) begin
      // Atomic operation from table chapter 4.1 of PRM-19
      `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), req_desc,
                     $sformatf("process_uc_fromsysaxi_reqs Observed a fromsysaxi atomic response request : %0s", req_desc.sprint_obj()))
      // AXI user field will have the tol3 request id
      if (! m_pending_uc_etlink_req.exists(req_desc.get_bus_req_user())) begin
         req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_uc_fromsysaxi_reqs - Received unexpected atomic fromsys axi request (response) %0s", req_desc.sprint_obj()), pk_uc_sb_error_intensity)
      end
      etlink_desc = m_pending_uc_etlink_req[req_desc.get_bus_req_user()];
      // delete the atomic req entry
      m_pending_uc_etlink_req.delete(req_desc.get_bus_req_user());
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_fromsysaxi_reqs ET_LINK_RSP_AckData resp REQ_Atomic request id 0x%0x", req_desc.get_bus_req_user()))
      if (etlink_desc != null) begin
         // fill in the response fields
         etlink_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_AckData));
         etlink_desc.set_bus_rsp_valid(0, get_cycle_count());
         // set the response and destination id here.
         etlink_desc.set_bus_rsp_size(req_desc.get_bus_req_size());
         etlink_desc.set_bus_rsp_id(etlink_desc.get_bus_req_id());
         etlink_desc.set_bus_rsp_dest(etlink_desc.get_bus_req_source());
         etlink_desc.set_rsp_data(req_desc.get_req_data());
         valid_uc = 1'b1;
         `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), etlink_desc,
                     $sformatf("process_uc_fromsysaxi_reqs Observed a fromsysaxi atomic response request : %0s", etlink_desc.sprint_obj()))
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_fromsysaxi_reqs observed UC sysaxi response - data - %s", etlink_desc.format_rsp_data()))
      end else begin // if (etlink_desc != null)
         req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("process_uc_fromsysaxi_reqs - cannot find the original etlink request to respond %0s", req_desc.sprint_obj()), pk_uc_sb_error_intensity)
      end // else: !if(etlink_desc != null)
   end else begin // if ((paddr[39:32] == 8'b00000001) && (paddr[31:30] == 2'b10) && (paddr[21:17] == 5'b11111) && (paddr[16:6] == 11'b0))
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_fromsysaxi_reqs request does not belong to UC"))
   end
   if (valid_uc) begin
      req_desc.set_bus_rsp_type(2'b00);
      req_desc.set_bus_rsp_valid(0, get_cycle_count());
      m_ucblock_fromsysaxi_req[req_desc.get_bus_req_id()].push_back(req_desc);
      // set the timeout on the request
      add_active_mem_desc(req_desc);
   end
endfunction : process_uc_fromsysaxi_reqs


//-----------------------------------------------------------------------------------------------
//
// Task: watch_sysaxi_uc_resp
//
task automatic evl_ucblock_scoreboard::watch_sysaxi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   evl_req_desc rcv_req_desc;
   if (rsp_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_sysaxi_uc_resp rsp_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end

   `evl_log(UVM_MEDIUM, VMOD_UC_SB, get_abstract_name(), $sformatf("UCBlock scoreboard watch_sysaxi_uc_resp"))
   // watch for the response
   fork
      forever begin
         rsp_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_sysaxi_uc_resp observed UC sysaxi response : %0s", rcv_req_desc.sprint_rsp()))
            process_uc_axi_resp(rcv_req_desc, int'(SHIRE_UCTOSYS_MESH_AXI));
         end
      end // forever begin
   join_none

endtask: watch_sysaxi_uc_resp


//-----------------------------------------------------------------------------------------------
//
// Task: watch_l3axi_uc_resp
//
task automatic evl_ucblock_scoreboard::watch_l3axi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   evl_req_desc rcv_req_desc;
   if (rsp_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_l3axi_uc_resp rsp_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end

   `evl_log(UVM_MEDIUM, VMOD_UC_SB, get_abstract_name(), $sformatf("UCBlock scoreboard watch_l3axi_uc_resp"))
   // watch for the response
   fork
      forever begin
         rsp_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_l3axi_uc_resp observed UC l3axi response : %0s", rcv_req_desc.sprint_rsp()))
            process_uc_axi_resp(rcv_req_desc, int'(SHIRE_UCTOL3_MESH_AXI));
         end
      end // forever begin
   join_none

endtask: watch_l3axi_uc_resp

function void evl_ucblock_scoreboard::process_uc_axi_resp(input evl_req_desc axi_req_desc, input int axi_port);

   evl_req_desc etlink_req_desc;
   evl_req_desc orig_req_desc;
   evl_bus_req_id_t     axi_req_id;
   evl_paddr_t req_paddr;
   evl_req_desc bus_req_desc;
   evl_bus_req_id_t bus_req_id;


   bit broadcast_done;

   `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp process the axi response req id 0x%0x", axi_req_desc.get_bus_req_id()));

   // populate the etlink original request with the expected response.
   // Check if this is a broadcast response
   bus_req_id = axi_req_desc.get_bus_req_id();
   if (bus_req_id >= 64) begin
      m_ucblock_axi_bc_req_cnt.pop_front();
      bc_has_error += axi_req_desc.get_bus_rsp_type() > (evl_bus_rsp_type_t'(0));
      // Check if this is the last request in the broadcast set. the req queue id should be less than 64
      if (m_ucblock_axi_bc_req_cnt[0] < 64) begin
         axi_req_id = m_ucblock_axi_bc_req_cnt.pop_front();
      end else begin
         if (m_ucblock_axi_actual_req.exists(axi_port) && m_ucblock_axi_actual_req[axi_port].exists(bus_req_id)) begin
            bus_req_desc = m_ucblock_axi_actual_req[axi_port][bus_req_id];
            if (bus_req_desc != null) begin
               bus_req_desc.set_done();
               delete_active_mem_desc(bus_req_desc);
            end
            m_ucblock_axi_actual_req[axi_port].delete(bus_req_id);
         end
         broadcast_done = 0;
         return;
      end
      broadcast_done = 1;
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp broadcast requests done id 0x%0x", axi_req_id))
   end
   // get the original ETLINK req_desc
   etlink_req_desc = axi_req_desc.get_parent_req_desc();
   if (etlink_req_desc == null) begin
      axi_req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
      `dut_error_mdesc(get_abstract_name(), axi_req_desc, $sformatf("process_uc_axi_resp no source request could be found for %0s", axi_req_desc.sprint_obj()), pk_uc_sb_error_intensity)
      return;
   end

   if (etlink_req_desc.get_bus_cmd() inside { ET_LINK_REQ_MsgSendData }) begin
      // If the original ETLink was a ET_LINK_REQ_MsgSendData request, there is no response generated
      etlink_req_desc.set_done();
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - This is a message request. set the original etlink request as done"))
   end // if (etlink_req_desc.get_bus_cmd() inside { ET_LINK_REQ_MsgSendData })
   else if ((! (etlink_req_desc.get_bus_cmd() inside { ET_LINK_REQ_Atomic })) | ( axi_req_desc.get_bus_rsp_type() > (evl_bus_rsp_type_t'(0)) )) begin
      // fill in the response fields
      etlink_req_desc.replicate_rsp_info(axi_req_desc, 0, 1);
      etlink_req_desc.set_bus_rsp_valid(0, get_cycle_count());
      // set the response and destination id here.
      etlink_req_desc.set_bus_rsp_id(etlink_req_desc.get_bus_req_id());
      etlink_req_desc.set_bus_rsp_dest(etlink_req_desc.get_bus_req_source());

      if (((broadcast_done) & (bc_has_error > 0)) || (axi_req_desc.get_bus_rsp_type() > (evl_bus_rsp_type_t'(0)))) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp ET_LINK_RSP_Err response for the req id 0x%0x", bus_req_id));
         etlink_req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Err));
         etlink_req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end else if (axi_req_desc.bus_cmd_is_read()) begin
         //if (axi_req_desc.get_bus_cmd() inside { ET_LINK_REQ_Read }) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp ET_LINK_RSP_AckData response for the read req id 0x%0x", bus_req_id));
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("predict_uc_axi_reqs - data - %s", axi_req_desc.format_rsp_data()))
         etlink_req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_AckData));
         etlink_req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end else begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp ET_LINK_RSP_Ack for the writes req id 0x%0x", bus_req_id));
         etlink_req_desc.set_bus_rsp_type(evl_bus_cmd_t'(ET_LINK_RSP_Ack));
         etlink_req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end

   end // if (! etlink_req_desc.get_bus_cmd() inside { ET_LINK_REQ_Atomic })

   if (broadcast_done) begin
      `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp remove broadcast request timeout and pop m_ucblock_axi_actual_req[%0d] (%0d = sys, %0d = l3) size 0x%0x transid 0x%0x req id 0x%0x", axi_port, int'(SHIRE_UCTOSYS_MESH_AXI), int'(SHIRE_UCTOL3_MESH_AXI), m_ucblock_axi_actual_req[axi_port].size(), axi_req_desc.get_trans_id(), axi_req_id))
      m_pending_uc_etlink_req.delete(axi_req_id);
      broadcast_done = 0;
      bc_has_error = 0;
   end else begin
      // remove the AXI request from the timeout
      if ((! (etlink_req_desc.get_bus_cmd() inside { ET_LINK_REQ_Atomic })) | ( axi_req_desc.get_bus_rsp_type() > (evl_bus_rsp_type_t'(0)) )) begin
         `evl_log(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), $sformatf("process_uc_axi_resp remove request timeout and pop m_ucblock_axi_actual_req[%0d] (%0d = sys, %0d = l3) size 0x%0x transid 0x%0x req id 0x%0x", axi_port, int'(SHIRE_UCTOSYS_MESH_AXI), int'(SHIRE_UCTOL3_MESH_AXI), m_ucblock_axi_actual_req[axi_port].size(), axi_req_desc.get_trans_id(), bus_req_id))
         m_pending_uc_etlink_req.delete(bus_req_id);
         axi_req_desc.set_done();
      end
   end

   bus_req_desc = null;
   if (m_ucblock_axi_actual_req.exists(axi_port)) begin
      if (m_ucblock_axi_actual_req[axi_port].exists(bus_req_id)) begin
         bus_req_desc = m_ucblock_axi_actual_req[axi_port][bus_req_id];
         m_ucblock_axi_actual_req[axi_port].delete(bus_req_id);
      end
   end
   if (bus_req_desc != null) begin
      //
      // This AXI request was targeted to the UC block.  To cleanly finish transactions, mark the
      // request descriptor created by the AXI bus agent as done.
      //
      bus_req_desc.set_done();
      delete_active_mem_desc(bus_req_desc);
   end
   else begin
      //
      // This AXI request was not targeted to the UC block (a read/write to ESRs?).  To cleanly
      // finish transactions, mark the request descriptor created by the AXI bus agent as done.
      //
      bus_req_desc = axi_req_desc.get_bus_req_desc();
      if (bus_req_desc != null) begin
         bus_req_desc.set_done();
      end
   end
endfunction : process_uc_axi_resp

//-----------------------------------------------------------------------------------------------
//
// Task: watch_fromsysaxi_uc_resp
//
task automatic evl_ucblock_scoreboard::watch_fromsysaxi_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo);

   evl_req_desc rcv_req_desc;
   if (rsp_fifo == null) begin
      `dut_error(get_abstract_name(), $sformatf("watch_fromsysaxi_uc_resp rsp_fifo is null"), pk_uc_sb_error_intensity)
      return;
   end

   // watch for the response
   fork
      forever begin
         rsp_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_fromsysaxi_uc_resp observed UC fromsysaxi response for request : %0s", rcv_req_desc.sprint_obj()))
            process_uc_fromsysaxi_resp(rcv_req_desc);
         end
      end // forever begin
   join_none

endtask: watch_fromsysaxi_uc_resp


function void evl_ucblock_scoreboard::process_uc_fromsysaxi_resp(input evl_req_desc axi_req_desc);
   evl_bus_req_id_t bus_req_id   = axi_req_desc.get_bus_req_id();
   evl_req_desc     bus_req_desc = null;

   if (m_ucblock_fromsysaxi_req.exists(bus_req_id)) begin
      bus_req_desc = m_ucblock_fromsysaxi_req[bus_req_id].pop_front();
   end
   if (bus_req_desc != null) begin
      //
      // This AXI request was targeted to the UC block.  To cleanly finish transactions, mark the
      // request descriptor created by the AXI bus agent as done and remove it from the list of
      // pending requests.
      //
      if ((axi_req_desc.is_bus_child() == 1'b0) && (bus_req_desc.get_parent_desc() != axi_req_desc)) begin
         `dut_error(get_abstract_name(), $sformatf("potential testbench issue ... expected %0s to be a child of %0s", bus_req_desc.sprint_obj(), axi_req_desc.sprint_obj()))
      end
      bus_req_desc.set_done();
      delete_active_mem_desc(bus_req_desc);
   end
   else begin
      //
      // This AXI request was not targeted to the UC block (a read/write to ESRs?).  To cleanly
      // finish transactions, mark the request descriptor created by the AXI bus agent as done.
      //
      bus_req_desc = axi_req_desc.get_bus_req_desc();
      if (bus_req_desc != null) begin
         bus_req_desc.set_done();
      end
   end
endfunction : process_uc_fromsysaxi_resp

//-----------------------------------------------------------------------------------------------
//
// Task: watch_etlink_uc_resp
//
task automatic evl_ucblock_scoreboard::watch_etlink_uc_resp(input uvm_tlm_analysis_fifo #(evl_req_desc) rsp_fifo,
                                                            input int port_num);

   evl_req_desc rcv_req_desc;
   if (rsp_fifo == null) begin
      return;
   end
   fork
      forever begin
         // get the rsp
         rsp_fifo.get(rcv_req_desc);
         if (rcv_req_desc != null) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_etlink_uc_resp ETLink response for neighbourhood %0d: %0s", port_num, rcv_req_desc.sprint_rsp()))
            `evl_log_mdesc(UVM_HIGH, VMOD_UC_SB, get_abstract_name(), rcv_req_desc,
                           $sformatf("watch_etlink_uc_resp ETLink response for neighbourhood %0d: %0s", port_num, rcv_req_desc.sprint_obj()))
            //rcv_req_desc.print_from_root(UVM_NONE, VMOD_UC_SB);
            process_uc_etlink_resp(rcv_req_desc, port_num);
         end
      end // forever begin

   join_none

endtask: watch_etlink_uc_resp

function void evl_ucblock_scoreboard::process_uc_etlink_resp(input evl_req_desc req_desc, input int src_id);
   uc_etlink_req_cnt -= 1;
endfunction : process_uc_etlink_resp

