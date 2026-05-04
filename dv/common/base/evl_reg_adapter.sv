//--------------------------------------------------------------------------------------------------
//
// Class: evl_reg_adapter
//
class evl_reg_adapter extends uvm_reg_adapter;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_reg_adapter)


   int         m_addr_shift    = 0;
   evl_paddr_t m_addr_and_mask = ~evl_paddr_t'(0);
   evl_paddr_t m_addr_or_mask  = evl_paddr_t'(0);


   virtual function void set_addr_shift(input int shift_in);
      m_addr_shift = shift_in;
   endfunction : set_addr_shift


   virtual function void set_addr_and_mask(input evl_paddr_t and_mask_in);
      m_addr_and_mask = and_mask_in;
   endfunction : set_addr_and_mask


   virtual function void set_addr_or_mask(input evl_paddr_t or_mask_in);
      m_addr_or_mask = or_mask_in;
   endfunction : set_addr_or_mask


   virtual function uvm_sequence_item reg2bus(const ref uvm_reg_bus_op rw);
      evl_req_desc bus_req = evl_req_desc::type_id::create("bus_req");

      if (rw.kind == UVM_READ) begin
         bus_req.set_bus_cmd(BusRead);
         bus_req.set_paddr(rw.addr);
      end
      else begin
         bus_req.set_bus_cmd(BusWrite);
         bus_req.set_paddr(rw.addr);
         copy_bus_req_data_to_desc(rw, bus_req);
      end
      return bus_req;
   endfunction


   virtual function void bus2reg(input uvm_sequence_item bus_item,
                                 ref   uvm_reg_bus_op    rw);
      evl_req_desc bus_req;

      if (!$cast(bus_req, bus_item)) begin
         `uvm_fatal("NOT_REQ_DESC_TYPE","Provided bus_item is not of the correct type")
         return;
      end

      if (bus_req.bus_cmd_is_write() == 1'b1) begin
         rw.kind   = UVM_WRITE;
         rw.addr   = ((bus_req.get_paddr() << m_addr_shift) & m_addr_and_mask) | m_addr_or_mask;
         rw.status = UVM_IS_OK;
         rw.data   = bus_req.get_req_dword(bus_req.get_paddr() >> `EVL_DWORD_ADDR_LSB);
      end
      else begin
         rw.kind   = UVM_READ;
         rw.addr   = ((bus_req.get_paddr() << m_addr_shift) & m_addr_and_mask) | m_addr_or_mask;
         rw.status = UVM_IS_OK;
         rw.data   = bus_req.get_rsp_dword(bus_req.get_paddr() >> `EVL_DWORD_ADDR_LSB);
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   function void copy_bus_req_data_to_desc(input uvm_reg_bus_op rw, input evl_req_desc req_desc);
      evl_line_data line_data;
      evl_dword_t   dword_data;

      dword_data = evl_dword_t'(rw.data);
      line_data  = new();
      line_data.invalidate_line();
      line_data.set_dword(req_desc.get_paddr() >> `EVL_DWORD_ADDR_LSB, dword_data);
      req_desc.set_req_data(line_data);
   endfunction : copy_bus_req_data_to_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_reg_adapter", input uvm_component parent_in = null);
      super.new(name_in);
   endfunction : new

endclass : evl_reg_adapter
