//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_item
//
class evl_sequence_item extends uvm_sequence_item;

   `uvm_object_utils(evl_sequence_item)


   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_sequence_item source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      set_abstract_name(source_obj.get_abstract_name());
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_item");
      super.new(name_in);
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_sequence_item
