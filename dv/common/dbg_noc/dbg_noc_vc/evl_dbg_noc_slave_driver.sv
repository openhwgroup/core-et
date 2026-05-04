//--------------------------------------------------------------------------------------------------
//
// Class: evl_dbg_noc_slave_driver
//
class evl_dbg_noc_slave_driver extends evl_push_driver #(evl_req_desc);

  `uvm_component_utils(evl_dbg_noc_slave_driver)


   `evl_param_key(pk_mem_bus_rsp_delay)


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   evl_dbg_noc_rtl_port_base  m_dbg_noc_rtl_port;
   evl_paddr_t                m_data_addr_mask;
   int                        m_data_width;
   int                        m_data_width_clog2;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: put
   //
   virtual task put(input evl_req_desc item);
      drive_response(item);
   endtask : put


   //-----------------------------------------------------------------------------------------------
   //
   // Function: drive_response
   //
   // This function splits up the transaction into bus cycles and sends those to the interface
   // module.
   //
   function void drive_response(input evl_req_desc req_desc);

   endfunction : drive_response


   //-----------------------------------------------------------------------------------------------
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      if (m_rtl_port == null) begin
         m_data_width = 64;
      end
      else begin
         m_data_width = m_rtl_port.get_int_param_by_name("pk_data_width", 64);
      end
      m_data_width_clog2 = $clog2(m_data_width);
      m_data_addr_mask   = ~((evl_paddr_t'(1) << m_data_width_clog2) - 1) & (`EVL_LINE_BYTES - 1);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_dbg_noc_rtl_port, m_rtl_port);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
      if (m_dbg_noc_rtl_port == null) begin
         return;
      end
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   virtual task post_reset_phase(uvm_phase phase);
      super.post_reset_phase(phase);
      if (m_dbg_noc_rtl_port == null) begin
         return;
      end
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_dbg_noc_slave_driver", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_dbg_noc_slave_driver
