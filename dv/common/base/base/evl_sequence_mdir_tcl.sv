//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_mdir_tcl
//
class evl_sequence_mdir_tcl extends evl_sequence_mdir;

   `uvm_object_utils(evl_sequence_mdir_tcl)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   string          m_file_name;
   evl_tcl_handler m_tcl_handler;
   int             m_sequencer_number = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: run_script
   //
   task run_script(input uvm_phase phase);
      if (m_tcl_handler != null) begin
         phase.raise_objection(this);
         enable_stimulus(`EVL_PHASE_DISABLE);
         #1ns;
         if (m_background_seq == 1'b0) begin
            m_tcl_handler.wait_for_script_done();
         end
         phase.drop_objection(this);
      end
   endtask : run_script


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pre_reset_phase
   //
   virtual task pre_reset_phase(uvm_phase phase);
      if (m_tcl_handler == null) begin
         super.pre_reset_phase(phase);
         enable_stimulus(`EVL_PHASE_DISABLE);
      end
      else begin
         m_tcl_handler.start_script();
         if (m_tcl_handler.get_starting_phase() == PreResetPhase) begin
            fork
               super.pre_reset_phase(phase);
               run_script(phase);
            join
            return;
         end
         else begin
            super.pre_reset_phase(phase);
         end
      end
   endtask : pre_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_phase
   //
   virtual task reset_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == ResetPhase) begin
            fork
               super.reset_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.reset_phase(phase);
   endtask : reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_reset_phase
   //
   virtual task post_reset_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PostResetPhase) begin
            fork
               super.post_reset_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.post_reset_phase(phase);
   endtask : post_reset_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pre_configure_phase
   //
   virtual task pre_configure_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PreConfigurePhase) begin
            fork
               super.pre_configure_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.pre_configure_phase(phase);
   endtask : pre_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: configure_phase
   //
   virtual task configure_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == ConfigurePhase) begin
            fork
               super.configure_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.configure_phase(phase);
   endtask : configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_configure_phase
   //
   virtual task post_configure_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PostConfigurePhase) begin
            fork
               super.post_configure_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.post_configure_phase(phase);
   endtask : post_configure_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pre_main_phase
   //
   virtual task pre_main_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PreMainPhase) begin
            fork
               super.pre_main_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.pre_main_phase(phase);
   endtask : pre_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == MainPhase) begin
            fork
               super.main_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_main_phase
   //
   virtual task post_main_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PostMainPhase) begin
            fork
               super.post_main_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.post_main_phase(phase);
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pre_shutdown_phase
   //
   virtual task pre_shutdown_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PreShutdownPhase) begin
            fork
               super.pre_shutdown_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.pre_shutdown_phase(phase);
   endtask : pre_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: shutdown_phase
   //
   virtual task shutdown_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == ShutdownPhase) begin
            fork
               super.shutdown_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.shutdown_phase(phase);
   endtask : shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_shutdown_phase
   //
   virtual task post_shutdown_phase(uvm_phase phase);
      if (m_tcl_handler != null) begin
         if (m_tcl_handler.get_starting_phase() == PostShutdownPhase) begin
            fork
               super.post_shutdown_phase(phase);
               run_script(phase);
            join
            return;
         end
      end
      super.post_shutdown_phase(phase);
   endtask : post_shutdown_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_mdir_tcl");
      super.new(name_in);
      disable_stimulus(`EVL_PHASE_DISABLE);
   endfunction

endclass : evl_sequence_mdir_tcl
