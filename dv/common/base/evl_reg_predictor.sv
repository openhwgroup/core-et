//--------------------------------------------------------------------------------------------------
//
// Class: evl_reg_predictor
//
class evl_reg_predictor #(type BUSTYPE=int) extends uvm_reg_predictor;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_component_param_utils(evl_reg_predictor#(BUSTYPE))


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_reg_predictor", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_reg_predictor
