//--------------------------------------------------------------------------------------------------
//
// Class: evl_tlb
//
class evl_tlb extends evl_cache_base#(evl_vaddr_t, evl_page_table_entry);

   `uvm_component_utils(evl_tlb)

   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tlb", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_tag_mask(const_page_vaddr_mask);
   endfunction : new

endclass : evl_tlb

