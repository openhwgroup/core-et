//--------------------------------------------------------------------------------------------------
//
// Class: evl_agent
//
virtual class evl_agent extends uvm_agent;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_rtl_port: This is a handle to the RTL port.  RTL ports are used to connect a particular
   //             verification component to one or more blocks of Verilog RTL, allowing access to
   //             clocks, resets, and any integer parameters implemented in the RTL.
   //
   evl_rtl_port m_rtl_port;
   evl_rtl_port m_rtl_ports[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_port
   //
   virtual function void set_rtl_port(input evl_rtl_port rtl_port_in, input int index = 0);
      if ((index == 0) || (m_rtl_port == null)) begin
         m_rtl_port = rtl_port_in;
      end
      m_rtl_ports[index] = rtl_port_in;
   endfunction : set_rtl_port


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_port
   //
   function evl_rtl_port get_rtl_port(input int index = 0);
      if (!m_rtl_ports.exists(index)) begin
         return m_rtl_port;
      end
      return m_rtl_ports[index];
   endfunction : get_rtl_port


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_agent", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_agent
