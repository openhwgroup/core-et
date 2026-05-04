//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_reg_model
//
class evl_memshire_reg_model extends evl_reg_block;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_memshire_reg_model)

   int m_built = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_map
   //
   virtual function void add_evl_map(input uvm_reg_map reg_map);
      foreach (m_evl_reg_blocks[ii]) begin
         if (m_evl_reg_blocks[ii] != null) begin
            if (reg_map.get_name() == "AXI") begin
               add_evl_subblock_map(reg_map, ii, 0);
            end
         end
      end
   endfunction : add_evl_map


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      if (m_built++ != 0) begin
         return;
      end
      void'(create_map("AXI", uvm_reg_addr_t'(base_addr), 8, UVM_LITTLE_ENDIAN, 0));
      foreach (m_evl_reg_blocks[ii]) begin
         m_evl_reg_blocks[ii].build();
      end
   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_memshire_reg_model", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

endclass
