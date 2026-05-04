//--------------------------------------------------------------------------------------------------
//
// Class: evl_soc_noc_env
//
class evl_soc_noc_env extends evl_verif_comp;

   `uvm_component_utils(evl_soc_noc_env)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Local Variables
   //
   int                m_noc_sb_enable;
   evl_noc_scoreboard m_noc_sb;

   int                     m_dbgnoc_sb_enable;
   evl_dbgnoc_scoreboard   m_dbgnoc_sb;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      if (m_noc_sb_enable == 1) begin
         m_noc_sb = evl_noc_scoreboard::type_id::create("m_noc_sb", this);
         m_noc_sb.set_agent(this);
         m_noc_sb.set_rtl_port(m_rtl_port);
      end
      else begin
         m_noc_sb = null;
      end
      if (m_dbgnoc_sb_enable == 1) begin
         m_dbgnoc_sb = evl_dbgnoc_scoreboard::type_id::create("m_dbgnoc_sb", this);
         m_dbgnoc_sb.set_agent(this);
         m_dbgnoc_sb.set_rtl_port(m_rtl_port);
      end
      else begin
         m_dbgnoc_sb = null;
      end
   endfunction : build_phase


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

      evl_shire_id_t   obs_req_shire_id;
      evl_req_desc     match_req_desc;
      evl_bus_req_id_t obs_bus_req_id;

      obs_req_shire_id  = paired_req_desc.get_shire_id();
      obs_bus_req_id    = paired_req_desc.get_bus_req_id();

      `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Req %0s observed on port %0d of shire %0d", paired_req_desc.sprint_obj(), port_in, obs_req_shire_id))

      if (port_type_in == AXI_BUS_AGENT) begin
         if (obs_req_shire_id == 254) begin
            `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on PS Slave port of IOShire", paired_req_desc.get_bus_req_id()))
            match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, IO_PS);
         end
         else if (obs_req_shire_id == 253) begin
            `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on PS Slave port of PShire", paired_req_desc.get_bus_req_id()))
            match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, PSHIRE_PS);
         end
         else if (obs_req_shire_id inside {[232:239]}) begin
            if (port_in == 0) begin
               `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on TO MEM Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOMEM_S);
            end
            else if (port_in == 1) begin
               `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on SYS Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, MC_SB_S);
            end
         end
         else if (obs_req_shire_id inside {[0:34]}) begin
            if (port_in == 2) begin
               `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on L3 Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOL3_S);
            end
            else if (port_in == 3) begin
               `evl_log(UVM_DEBUG, VMOD_NOC_SB, get_abstract_name(), $sformatf("NOC_ENV: Request with ID 0x%0x observed on SYS Slave port of shire %0d", paired_req_desc.get_bus_req_id(), obs_req_shire_id))
               match_req_desc = m_noc_sb.match_observed_request(paired_req_desc, obs_req_shire_id, TOSYS_S);
            end
         end
         if (match_req_desc != null) begin
            match_req_desc.add_child_desc(paired_req_desc);
         end
         return match_req_desc;
      end
      return null;
   endfunction : find_observed_request

   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_req_checked
   //
   virtual function bit is_req_checked(input evl_req_desc req_desc_in);
      return m_noc_sb.is_req_checked(req_desc_in);
   endfunction : is_req_checked


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_noc_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in, NOC_RTL_AGENT);
      set_abstract_name("NOC");

      if (`et_test_plusargs("disable_NOC_SB") != 0) begin
         m_noc_sb_enable = 0;
      end
      else begin
         m_noc_sb_enable = 1;
      end
      //if (`et_test_plusargs("disable_DBGNOC_SB") != 0) begin
      //   m_dbgnoc_sb_enable = 0;
      //end
      //else begin
      //   m_dbgnoc_sb_enable = 1;
      //end
      if (`et_test_plusargs("enable_DBGNOC_SB") != 0) begin
         m_dbgnoc_sb_enable = 1;
      end
      else begin
         m_dbgnoc_sb_enable = 0;
      end
   endfunction : new

endclass : evl_soc_noc_env
