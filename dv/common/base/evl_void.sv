//--------------------------------------------------------------------------------------------------
//
// Class: evl_void
//
// This class is provided solely to include some debug hooks for EVL objects that would normally be
// extended from uvm_void.
//
class evl_void extends uvm_void;

   `include "dv/common/base/evl_base_inc_name.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_global_inst_id: This is the global instance ID used to generate a unique instance ID for
   //                   this object.
   //
   static protected int m_global_inst_id = -1;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_inst_id: This is the instance ID for this object.  Instance IDs for evl_void objects are
   //            decrementing negative numbers.
   //
   int m_inst_id = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_inst_id
   //
   // This function is used to get the instance ID of this object.
   //
   // Return Value:
   //
   // int: This is the instance ID of the object.
   //
   function int get_inst_id();
      return m_inst_id;
   endfunction : get_inst_id


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "");
      m_inst_id = m_global_inst_id--;
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_void
