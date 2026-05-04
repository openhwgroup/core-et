//--------------------------------------------------------------------------------------------------
//
// Class: evl_env
//
virtual class evl_env extends uvm_component;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: connect_phase
   //
   virtual function void connect_phase(uvm_phase phase);
      super.connect_phase(phase);
   endfunction : connect_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: end_of_elaboration_phase
   //
   virtual function void end_of_elaboration_phase(uvm_phase phase);
      super.end_of_elaboration_phase(phase);
   endfunction : end_of_elaboration_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      super.run_phase(phase);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      super.pre_reset_phase(phase);
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      super.reset_phase(phase);
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      super.post_reset_phase(phase);
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      super.pre_configure_phase(phase);
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      super.configure_phase(phase);
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      super.post_configure_phase(phase);
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      super.pre_main_phase(phase);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      super.post_main_phase(phase);
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      super.pre_shutdown_phase(phase);
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      super.shutdown_phase(phase);
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      super.post_shutdown_phase(phase);
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_env", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_env
