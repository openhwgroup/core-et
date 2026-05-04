//--------------------------------------------------------------------------------------------------
//
// Class: evl_object
//
class evl_object extends uvm_object;

   `uvm_object_utils(evl_object)


   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_object source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      set_abstract_name(source_obj.get_abstract_name());
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_object");
      super.new(name_in);
   endfunction : new

endclass : evl_object
