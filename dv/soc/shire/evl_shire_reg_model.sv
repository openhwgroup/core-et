//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_reg_model
//
class evl_shire_reg_model extends evl_reg_block;

   `uvm_object_utils(evl_shire_reg_model)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_map
   //
   // This function adds the each of the shire register maps as subblock maps of the root map.
   //
   virtual function void add_evl_map(input uvm_reg_map reg_map);
      foreach (m_evl_reg_blocks[ii]) begin
         add_evl_subblock_map(reg_map, ii, evl_paddr_t'(0));
      end
   endfunction : add_evl_map


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_reg_model", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

endclass : evl_shire_reg_model
