//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_monitor
//
class evl_dbg_noc_monitor extends evl_monitor;

   `uvm_component_utils(evl_dbg_noc_monitor)

   `evl_param_key(pk_allow_duplicate_outstanding_ids)
   `evl_param_key(pk_data_bytes)
   `evl_param_key(pk_sc_dbg_noc_error_intensity)

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a handle to the RTL port (which is instantiated in the
   // evl_dbg_noc_interface module).
   //
   evl_dbg_noc_rtl_port_base m_dbg_noc_rtl_port;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are intended to uniquely identify this interface.
   //
   int                   m_port_type  = 0;
   int                   m_subport_id = 0;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are handles back to the bus verification component and the master and
   // slave components for this interface.
   //
   evl_verif_comp        m_verif_comp        = null;
   evl_verif_comp        m_master_port_agent = null;
   evl_verif_comp        m_slave_port_agent  = null;

   //-----------------------------------------------------------------------------------------------
   //
   // This following variable is a request descriptor queue to hold pending requests that can then
   // be matched with incoming responses.
   //

   // Using normal queue for now instead of evl_req_desc_queue because we don't need the extra 
   // functionality
   //evl_req_desc_queue    m_request_queue;
   evl_req_desc          m_request_queue[$];

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables are templates from which to create new requests.  Each request
   // observed on the bus spawns a new request descriptor ... the following templates are used to
   // create the spawned descriptor.
   //
   evl_line_data         m_template_req_data;
   evl_line_desc         m_template_line_desc;
   evl_trans_desc        m_template_trans_desc;
   evl_req_desc          m_template_req_desc;

   //-----------------------------------------------------------------------------------------------
   //
   // The following variables define the data width of the bus for this inteface.
   int                   m_data_bytes;
   int                   m_data_bytes_log2;

   //-----------------------------------------------------------------------------------------------
   //
   // The following analysis ports are used to send observed requests to a downstream object and
   // observed responses to an upstream object.
   //
   uvm_analysis_port #(evl_req_desc) m_obs_tx_req_port;
   uvm_analysis_port #(evl_req_desc) m_obs_rx_req_port;
   uvm_analysis_port #(evl_req_desc) m_exp_rsp_port;

   int   m_rx_trans_count  = 0;
   bit   m_count_rx_txns   = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_port_agent
   //
   virtual function void set_master_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_master_port_agent = agent_in;
   endfunction : set_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_port_agent
   //
   virtual function evl_verif_comp get_master_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_master_port_agent;
   endfunction : get_master_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_port_agent
   //
   virtual function void set_slave_port_agent(input evl_verif_comp agent_in, input string port_name = "", input int port_id = 0);
      m_slave_port_agent = agent_in;
   endfunction : set_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_port_agent
   //
   virtual function evl_verif_comp get_slave_port_agent(input string port_name = "", input int port_id = 0, input int subport_id = 0);
      return m_slave_port_agent;
   endfunction : get_slave_port_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_tx_requests
   //
   // Get request objects from the Tx interface, transfer the information into a request descriptor,
   // then send it on the m_obs_tx_req_port.
   //
   task process_tx_requests();
      evl_req_desc        req_desc;
      evl_req_desc        exp_req_desc;
      evl_line_data       line_data;
      evl_xword_t         xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_dbg_noc_bus_req bus_req;

      if (m_dbg_noc_rtl_port == null) begin
         return;
      end
      forever begin
         //
         // Get the next observed bus cycle.
         //
         m_dbg_noc_rtl_port.get_obs_tx_req(bus_req);

         //
         // Create an observed request
         // descriptor and fill in the fields.
         //
         $cast(req_desc, m_template_req_desc.clone());
         req_desc.set_paddr(bus_req.get_req_addr());
         req_desc.set_bus_qos(bus_req.get_req_qos());
         req_desc.set_obs_req_data(evl_paddr_t'(0), m_data_bytes, bus_req.get_req_data(), ~evl_xword_byte_en_t'(0));

         req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(0));
         req_desc.set_bus_req_size(evl_bus_req_size_t'(0));
         req_desc.set_bus_req_len(evl_bus_req_len_t'(0));
         req_desc.set_bus_req_id(evl_bus_req_id_t'(0));
         req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
         req_desc.set_bus_prot(evl_bus_prot_t'(0));
         if (bus_req.get_req_dest_int() != ~evl_bus_req_vc_t'(0)) begin
            req_desc.set_vc(bus_req.get_req_dest_int());
         end
         if (!(bus_req.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
            req_desc.set_trans_id(bus_req.get_trans_id());
         end


         //
         // The following code fragment attempts to link the observed request to a predicted
         // request.  If the handle to upstream verification component (m_master_port_agent) is
         // null, no connection can be made.
         //
         if (m_master_port_agent != null) begin
            exp_req_desc = m_master_port_agent.find_observed_request(DBG_NOC_BUS_AGENT, m_port_type, m_subport_id, req_desc);
            req_desc.set_parent_check();
            if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
               if (!(req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
                  req_desc.set_trans_id();
               end
            end
         end

         //
         // If no match for the observed request descriptor was made and it has no parent, we
         // need to create a fake transaction descriptor for it and set the transaction
         // descriptor as the parent.  This way, the downstream object (which will get a handle
         // to the observed request descriptor) can assume there is a valid transaction
         // descriptor associated with the observed request descriptor.
         //
         if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
            if (req_desc.get_root_trans_desc() == null) begin
               evl_trans_desc trans_desc;

               $cast(trans_desc, m_template_trans_desc.clone());
               trans_desc.set_cmd(ST_DBG_NOC);
               trans_desc.set_paddr(req_desc.get_paddr());
               if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  trans_desc.set_trans_id();
               end
               else begin
                  trans_desc.set_trans_id(req_desc.get_trans_id());
               end
               trans_desc.add_child_desc(req_desc);
            end
         end

         //
         // One final check to make sure there is a unique transaction ID for the observed
         // request descriptor.
         //
         if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            req_desc.set_trans_id();
         end

         `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), req_desc, $sformatf("saw a request with addr 0x%x, data 0x%x", bus_req.get_req_addr(), bus_req.get_req_data()))

         //
         // Put the observed request descriptor onto the request queue so we can match its
         // response later.
         //
         m_request_queue.push_back(req_desc);

         //
         // Add the observed request descriptor to the list that is checked for timeouts.
         //
         m_verif_comp.add_active_mem_desc(req_desc);

         //
         // Send the observed request descriptor off to any downstream component.
         //
         m_obs_tx_req_port.write(req_desc);
      end
   endtask : process_tx_requests

   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_rx_requests
   //
   // Get request objects from the interface, transfer the information into a request descriptor,
   // then send it on the m_obs_rx_req_port.
   //
   task process_rx_requests();
      evl_req_desc        req_desc;
      evl_req_desc        exp_req_desc;
      evl_line_data       line_data;
      evl_xword_t         xword_data;
      evl_xword_byte_en_t xword_byte_en;
      evl_dbg_noc_bus_req bus_req;

      if (m_dbg_noc_rtl_port == null) begin
         return;
      end
      forever begin
         //
         // Get the next observed bus cycle.
         //
         m_dbg_noc_rtl_port.get_obs_rx_req(bus_req);

         //
         // Create an observed request
         // descriptor and fill in the fields.
         //
         $cast(req_desc, m_template_req_desc.clone());
         req_desc.set_paddr(bus_req.get_req_addr());
         req_desc.set_bus_qos(bus_req.get_req_qos());
         req_desc.set_obs_req_data(evl_paddr_t'(0), m_data_bytes, bus_req.get_req_data(), ~evl_xword_byte_en_t'(0));

         req_desc.set_bus_cmd(evl_bus_cmd_t'(`EVL_AXI_WRITE));
         req_desc.set_bus_cache_attr(evl_bus_cache_attr_t'(0));
         req_desc.set_bus_req_size(evl_bus_req_size_t'(0));
         req_desc.set_bus_req_len(evl_bus_req_len_t'(0));
         req_desc.set_bus_req_id(evl_bus_req_id_t'(0));
         req_desc.set_bus_req_user(evl_bus_req_user_t'(0));
         req_desc.set_bus_prot(evl_bus_prot_t'(0));
         req_desc.set_vc(evl_bus_req_vc_t'(0));
         if (!(bus_req.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
            req_desc.set_trans_id(bus_req.get_trans_id());
         end


         //
         // The following code fragment attempts to link the observed request to a predicted
         // request.  If the handle to upstream verification component (m_master_port_agent) is
         // null, no connection can be made.
         //
         /* // Temporarily disable 
         if (m_master_port_agent != null) begin
            exp_req_desc = m_master_port_agent.find_observed_request(DBG_NOC_BUS_AGENT, m_port_type, m_subport_id, req_desc);
            req_desc.set_parent_check();
            if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
               if (!(req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) })) begin
                  req_desc.set_trans_id();
               end
            end
         end
         */

         //
         // If no match for the observed request descriptor was made and it has no parent, we
         // need to create a fake transaction descriptor for it and set the transaction
         // descriptor as the parent.  This way, the downstream object (which will get a handle
         // to the observed request descriptor) can assume there is a valid transaction
         // descriptor associated with the observed request descriptor.
         //
         if ((exp_req_desc == null) && (req_desc.get_parent_req_desc() == null)) begin
            if (req_desc.get_root_trans_desc() == null) begin
               evl_trans_desc trans_desc;

               $cast(trans_desc, m_template_trans_desc.clone());
               trans_desc.set_cmd(ST_DBG_NOC);
               trans_desc.set_paddr(req_desc.get_paddr());
               if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
                  trans_desc.set_trans_id();
               end
               else begin
                  trans_desc.set_trans_id(req_desc.get_trans_id());
               end
               trans_desc.add_child_desc(req_desc);
            end
         end

         //
         // One final check to make sure there is a unique transaction ID for the observed
         // request descriptor.
         //
         if (req_desc.get_trans_id() inside { evl_trans_id_t'(0), ~evl_trans_id_t'(0) }) begin
            req_desc.set_trans_id();
         end

         `evl_log_mdesc(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), req_desc, $sformatf("saw a request with data 0x%x", bus_req.get_req_data()))

         if (m_count_rx_txns == 1'b1) begin
            m_rx_trans_count++;
         end

         //
         // Put the observed request descriptor onto the request queue so we can match its
         // response later.
         //
         //m_request_queue.push_back(req_desc);

         //
         // Add the observed request descriptor to the list that is checked for timeouts.
         //
         //m_verif_comp.add_active_mem_desc(req_desc);

         //
         // Send the observed request descriptor off to any downstream component.
         //
         m_obs_rx_req_port.write(req_desc);
      end
   endtask : process_rx_requests


   //-----------------------------------------------------------------------------------------------
   //
   // Task: process_responses
   //
   // Get response objects from the interface, transfer the information into a request descriptor,
   // then send it on the m_exp_rsp_port.
   //
   task process_responses();
      evl_req_desc        exp_req_desc;
      bit                 bus_rsp;

      if (m_dbg_noc_rtl_port == null) begin
         return;
      end
      forever begin
         m_dbg_noc_rtl_port.get_obs_rsp(bus_rsp);

         `evl_log(UVM_HIGH, VMOD_DBG_NOC_MON, get_abstract_name(), $sformatf("received a response"))

         if (bus_rsp) begin
            exp_req_desc = m_request_queue.pop_front();
         end

         `evl_log_mdesc(UVM_HIGH, VMOD_AXI_MON, get_abstract_name(), exp_req_desc, $sformatf("Deleting request descriptor from queue: %s", exp_req_desc.sprint_obj()))

         m_exp_rsp_port.write(exp_req_desc);
      end
   endtask : process_responses


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_dbg_noc_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data   = evl_line_data::type_id::create($sformatf("%s(DBG_NOC)", get_root_abstract_name()));
      m_template_line_desc  = evl_line_desc::type_id::create($sformatf("%s(DBG_NOC)", get_root_abstract_name()));
      m_template_trans_desc = evl_trans_desc::type_id::create($sformatf("%s(DBG_NOC)", get_root_abstract_name()));
      m_template_req_desc   = evl_req_desc::type_id::create($sformatf("%s(DBG_NOC)", get_root_abstract_name()));

      m_template_trans_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_trans_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_trans_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_req_desc.set_shire_id(m_verif_comp.get_shire_id());
      m_template_req_desc.set_neighborhood_id(m_verif_comp.get_neighborhood_id());
      m_template_req_desc.set_core_id(m_verif_comp.get_core_id());

      m_template_line_desc.set_data_template(m_template_req_data);
      m_template_trans_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_data_template(m_template_req_data);
      m_template_req_desc.set_bus_type(EVL_BUS_TYPE_DBG_NOC);

      m_template_trans_desc.set_line_template(m_template_line_desc);
      m_template_req_desc.set_line_template(m_template_line_desc);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      m_count_rx_txns = 1'b1;
      super.run_phase(phase);
      //m_request_queue.set_abstract_name($sformatf("%s(REQQ)", get_root_abstract_name(get_abstract_name())));
      fork
         process_tx_requests();
         process_rx_requests();
         process_responses();
      join_none;

      m_data_bytes      = m_rtl_port.get_int_param(pk_data_bytes);
      m_data_bytes_log2 = $clog2(m_data_bytes);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: report_phase
   //
   virtual function void report_phase(uvm_phase phase);
      super.report_phase(phase);
      if (m_rx_trans_count > 0) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, get_abstract_name(), $sformatf("Number of transactions received during test: %0d", m_rx_trans_count))
      end
   endfunction : report_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_dbg_noc_monitor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_obs_tx_req_port = new("m_obs_tx_req_port", this);
      m_obs_rx_req_port = new("m_obs_rx_req_port", this);
      m_exp_rsp_port    = new("m_exp_rsp_port", this);
      //m_request_queue   = new("dbg_noc_mon_request_queue");
   endfunction : new

endclass: evl_dbg_noc_monitor
