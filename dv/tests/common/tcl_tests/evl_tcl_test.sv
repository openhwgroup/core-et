//--------------------------------------------------------------------------------------------------
//
// Class: evl_tcl_test
//
class evl_tcl_test extends evl_stimulus;

   `uvm_object_utils(evl_tcl_test)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_tcl_test m_tcl_test       = new();
   static bit          m_crash_and_burn = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   string                m_root_file_by_type[evl_agent_type_t];
   evl_sequence_mdir_tcl m_seq_list[$];
   evl_tcl_handler       m_tcl_handler_list[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   // This function is used to parse the available stimulus agents and, if specific matches are
   // found, prepare TCL tests for them.
   //
   // The m_root_file_by_type[] array defines the default +arg name to use when assigning a TCL
   // file for use and the default TCL file name.  For example, SYSTEM_AGENT will, by default, use
   // the file "systemStim.tcl" for stimulus, but this can be changed with the "+systemStim=<file>"
   // run-time command-line argument.
   //
   // The agent_types[] queue defines all of the stimulus agent types to be searched.
   //
   // The local_seq_list[] queue contains all of the TCL sequences generated for use.
   //
   virtual function void prepare_test_for_use();
      bit                   have_target = 1'b0;
      evl_agent_type_t      agent_types[$];
      evl_sequence_mdir_tcl local_seq_list[$];

      m_root_file_by_type[SYSTEM_AGENT]                     = "systemStim";
      m_root_file_by_type[NEIGH_STIM_AGENT]                 = "neighStim";
      m_root_file_by_type[CORE_STIM_AGENT]                  = "minionStim";
      m_root_file_by_type[L2_MASTER_STIM_AGENT]             = "l2Stim";
      m_root_file_by_type[L3_MASTER_STIM_AGENT]             = "l3Stim";
      m_root_file_by_type[TOSYS_MASTER_STIM_AGENT]          = "tosysStim";
      m_root_file_by_type[ATOMIC_RSP_MASTER_STIM_AGENT]     = "amorspStim";
      m_root_file_by_type[IOTOSYS_MASTER_STIM_AGENT]        = "iotosysStim";
      m_root_file_by_type[SPIO_VAULT_MASTER_STIM_AGENT]     = "spiovaultStim";
      m_root_file_by_type[SPIO_AM_MASTER_STIM_AGENT]        = "spioamStim";
      m_root_file_by_type[SPIO_DMA_MASTER_STIM_AGENT]       = "spiodmaStim";
      m_root_file_by_type[PU_DMA_AM_MASTER_STIM_AGENT]      = "pudmaamStim";
      m_root_file_by_type[PU_DMA_MASTER_STIM_AGENT]         = "pudmaStim";
      m_root_file_by_type[PU_EMMC_MASTER_STIM_AGENT]        = "puemmcStim";
      m_root_file_by_type[PU_USB_MASTER_STIM_AGENT]         = "puusbStim";
      m_root_file_by_type[PSHIRE_TOSYS_MASTER_STIM_AGENT]   = "pshiretosysStim";
      m_root_file_by_type[PSHIRE_PCIE_MASTER_STIM_AGENT]    = "pshiretosysStim";
      m_root_file_by_type[PSHIRE_ESR_MASTER_STIM_AGENT]     = "pshiretosysStim";
      m_root_file_by_type[PSHIRE_DBG_MASTER_STIM_AGENT]     = "pshiretosysStim";
      m_root_file_by_type[IO_PM_MASTER_STIM_AGENT]          = "iopmStim";
      m_root_file_by_type[UCTOSYS_MASTER_STIM_AGENT]        = "uctosysStim";
      m_root_file_by_type[UCTOL3_MASTER_STIM_AGENT]         = "uctol3Stim";
      m_root_file_by_type[L2TOL3_MASTER_STIM_AGENT]         = "l2tol3Stim";
      m_root_file_by_type[SBM_MASTER_STIM_AGENT]            = "sbmStim";
      m_root_file_by_type[FROMSYS_MASTER_STIM_AGENT]        = "fromsysStim";
      m_root_file_by_type[MEM_MASTER_STIM_AGENT]            = "memStim";
      m_root_file_by_type[UST_MASTER_STIM_AGENT]            = "ustStim";
      m_root_file_by_type[SPIO_E2A_MASTER_STIM_AGENT]       = "spioE2aStim";
      m_root_file_by_type[SPIO_ROM_MASTER_STIM_AGENT]       = "spioRomStim";
      m_root_file_by_type[SPIO_RAM_MASTER_STIM_AGENT]       = "spioRamStim";
      m_root_file_by_type[RBM_MASTER_STIM_AGENT]            = "rbmStim";
      m_root_file_by_type[DBG_NOC_MASTER_STIM_AGENT]        = "dbgnocStim";
      m_root_file_by_type[JTAG_STIM_AGENT]                  = "jtagStim";
      m_root_file_by_type[GENERIC_AXI_MASTER_STIM_AGENT]    = "axiStim";
      m_root_file_by_type[BPAM_MASTER_STIM_AGENT]           = "bpamStim";

      agent_types.push_back(SYSTEM_AGENT);
      agent_types.push_back(NEIGH_STIM_AGENT);
      agent_types.push_back(CORE_STIM_AGENT);
      agent_types.push_back(L2_MASTER_STIM_AGENT);
      agent_types.push_back(L3_MASTER_STIM_AGENT);
      agent_types.push_back(TOSYS_MASTER_STIM_AGENT);
      agent_types.push_back(ATOMIC_RSP_MASTER_STIM_AGENT);
      agent_types.push_back(IOTOSYS_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_VAULT_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_AM_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_DMA_MASTER_STIM_AGENT);
      agent_types.push_back(PU_DMA_AM_MASTER_STIM_AGENT);
      agent_types.push_back(PU_DMA_MASTER_STIM_AGENT);
      agent_types.push_back(PU_EMMC_MASTER_STIM_AGENT);
      agent_types.push_back(PU_USB_MASTER_STIM_AGENT);
      agent_types.push_back(PSHIRE_TOSYS_MASTER_STIM_AGENT);
      agent_types.push_back(PSHIRE_PCIE_MASTER_STIM_AGENT);
      agent_types.push_back(PSHIRE_ESR_MASTER_STIM_AGENT);
      agent_types.push_back(PSHIRE_DBG_MASTER_STIM_AGENT);
      agent_types.push_back(IO_PM_MASTER_STIM_AGENT);
      agent_types.push_back(UCTOSYS_MASTER_STIM_AGENT);
      agent_types.push_back(UCTOL3_MASTER_STIM_AGENT);
      agent_types.push_back(L2TOL3_MASTER_STIM_AGENT);
      agent_types.push_back(SBM_MASTER_STIM_AGENT);
      agent_types.push_back(FROMSYS_MASTER_STIM_AGENT);
      agent_types.push_back(MEM_MASTER_STIM_AGENT);
      agent_types.push_back(UST_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_E2A_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_ROM_MASTER_STIM_AGENT);
      agent_types.push_back(SPIO_RAM_MASTER_STIM_AGENT);
      agent_types.push_back(RBM_MASTER_STIM_AGENT);
      agent_types.push_back(DBG_NOC_MASTER_STIM_AGENT);
      agent_types.push_back(JTAG_STIM_AGENT);
      agent_types.push_back(GENERIC_AXI_MASTER_STIM_AGENT);
      agent_types.push_back(BPAM_MASTER_STIM_AGENT);

      foreach (agent_types[ii]) begin
         evl_verif_comp agent_list_per_type[];

         evl_verif_comp_map::get_verif_comp_list(agent_list_per_type, agent_types[ii]);
         foreach (agent_list_per_type[jj]) begin
            string file_name_list[];

            if (find_file_name_list(file_name_list, agent_list_per_type[jj], 1'b1) > 0) begin
               have_target = 1'b1;
            end
         end
      end

      if (have_target == 1'b0) begin
         return;
      end

      //
      // We have found at least 1 target.  Given that, let's start a directed sequence for every
      // agent (so we can use the "select_agent" feature in the TCL script) and then assign TCL
      // scripts as necessary.
      //

      //
      // Start by creating directed sequences for each available agent's sequencers.  These get
      // added to a list which can then be used by the TCL "select_agent" command.
      //
      foreach (agent_types[ii]) begin
         evl_verif_comp agent_list_per_type[];

         evl_verif_comp_map::get_verif_comp_list(agent_list_per_type, agent_types[ii]);
         foreach (agent_list_per_type[jj]) begin
            evl_sequence_mdir_tcl test_seq;

            for (int kk = 0; kk < agent_list_per_type[jj].get_sequencer_count(); kk++) begin
               evl_sequence_wrapper_base sequence_wrapper;

               test_seq = new();
               test_seq.m_sequencer_number = kk;
               sequence_wrapper = test_seq.get_wrapper();
               test_seq.set_abstract_name($sformatf("TCL Sequence #1 for %0s, sequencer %0d", agent_list_per_type[jj].get_abstract_name(), test_seq.m_sequencer_number));
               sequence_wrapper.set_abstract_name($sformatf("TCL Sequence #1 for %0s, sequencer %0d", agent_list_per_type[jj].get_abstract_name(), test_seq.m_sequencer_number));
               m_seq_list.push_back(test_seq);
               agent_list_per_type[jj].add_sequence_wrapper(test_seq.get_wrapper(), kk);
            end
         end
      end

      //
      // Now, for each created sequence, create a TCL handler as necessary.  Start by selecting only
      // the first one.
      //
      foreach (m_seq_list[ii]) begin
         int    file_name_count;
         string file_name_list[];

         if (m_seq_list[ii].get_agent() != null) begin
            file_name_count = find_file_name_list(file_name_list, m_seq_list[ii].get_agent());
            if (file_name_count > 0) begin
               prepare_tcl_for_agent(m_seq_list[ii], file_name_list[0]);
            end
         end
      end

      //
      // Now, for each created sequence, create a TCL handler as necessary.  Start by selecting only
      // the first one.
      //
      foreach (m_seq_list[ii]) begin
         int    file_name_count;
         string file_name_list[];

         if (m_seq_list[ii].get_agent() != null) begin
            evl_verif_comp agent = m_seq_list[ii].get_agent();

            if (agent != null) begin
               file_name_count = find_file_name_list(file_name_list, agent);
               for (int jj = 1; jj < file_name_count; jj++) begin
                  evl_sequence_mdir_tcl test_seq;

                  for (int kk = 0; kk < agent.get_sequencer_count(); kk++) begin
                     evl_sequence_wrapper_base sequence_wrapper;

                     test_seq = new();
                     test_seq.m_sequencer_number = kk;
                     sequence_wrapper = test_seq.get_wrapper();
                     test_seq.set_abstract_name($sformatf("TCL Sequence #%0d for %0s, sequencer %0d", jj + 1, agent.get_abstract_name(), test_seq.m_sequencer_number));
                     sequence_wrapper.set_abstract_name($sformatf("TCL Sequence #%0d for %0s, sequencer %0d", jj + 1, agent.get_abstract_name(), test_seq.m_sequencer_number));
                     local_seq_list.push_back(test_seq);
                     agent.add_sequence_wrapper(test_seq.get_wrapper(), kk);
                     prepare_tcl_for_agent(test_seq, file_name_list[jj]);
                  end
               end
            end
         end
      end

      //
      // Now, for each created TCL handler, make sure we have access to all agents.
      //
      foreach (m_tcl_handler_list[ii]) begin
         foreach (m_seq_list[jj]) begin
            evl_sequence_mdir_tcl test_seq;
            evl_verif_comp        agent;

            test_seq = m_seq_list[jj];
            agent    = test_seq.get_agent();
            if (agent != null) begin
               m_tcl_handler_list[ii].add_sequence_wrapper_for_agent(agent, test_seq.get_wrapper(), test_seq.m_sequencer_number);
            end
         end
      end

      foreach (local_seq_list[ii]) begin
         m_seq_list.push_back(local_seq_list[ii]);
      end
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_file_name_list
   //
   function int find_file_name_list(output string file_name_list_out[], input evl_verif_comp agent_in, input bit allow_error = 1'b0);
      string                root_file_name = "";
      string                file_name      = "";
      string                file_name_list[];
      evl_shire_id_t        shire_id;
      evl_neighborhood_id_t neighborhood_id;
      evl_core_id_t         core_id;
      int                   file_name_count;

      if (agent_in == null) begin
         file_name_list_out    = new[1];
         file_name_list_out[0] = "";
         return 0;
      end

      if (m_root_file_by_type.exists(agent_in.get_agent_type())) begin
         root_file_name = m_root_file_by_type[agent_in.get_agent_type()];
      end

      if (root_file_name != "") begin
         bit have_match;

         //
         // Look for a +arg match.
         //
         have_match = agent_in.plus_arg_match(root_file_name);

         //
         // First search for the agent-specific format (e.g., +neighStim,s0,n1=blah).  This will
         // also find the generic form (e.g., +neighStim=blah).
         //
         file_name_count = get_full_file_path_list(file_name_list, "", root_file_name, agent_in, 1, 1'b1);
         if (file_name_count < 0) begin
            file_name_count = 1 - file_name_count;
            if (allow_error == 1'b1) begin
               if (m_crash_and_burn == 1'b1) begin
                  `dut_error(get_abstract_name(), $sformatf("Couldn't find tcl script for \"+%s\" (%s)", agent_in.sprint_plus_arg(root_file_name), agent_in.get_abstract_name()))
               end
               else begin
                  `evl_log(UVM_LOW, VMOD_TCL, get_abstract_name(), $sformatf("Couldn't find tcl script \"+%s\" (%0s)", agent_in.sprint_plus_arg(root_file_name), agent_in.get_abstract_name()))
               end
            end
         end
         if (file_name_count > 0) begin
            file_name_list_out = new[file_name_list.size()];
            foreach (file_name_list_out[ii]) begin
               file_name_list_out[ii] = file_name_list[ii];
            end
            return file_name_list_out.size();
         end

         //
         // If it wasn't found, search for the specific file name as long as there is no +arg
         // restricting it.
         //
         if (have_match == 1'b0) begin
            file_name_count = get_full_file_path_list(file_name_list, $sformatf("%s.tcl", root_file_name), "", agent_in, 1);
            if (file_name_count > 0) begin
               file_name_list_out = new[file_name_list.size()];
               foreach (file_name_list_out[ii]) begin
                  file_name_list_out[ii] = file_name_list[ii];
               end
               return file_name_list_out.size();
            end
         end
      end
      return 0;
   endfunction : find_file_name_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_tcl_for_agent
   //
   function void prepare_tcl_for_agent(input evl_sequence_mdir_tcl test_seq, input string file_name);
      evl_verif_comp         agent;
      evl_tcl_mem_seq_parser tcl_mem_seq_parser;
      evl_tcl_handler        tcl_handler;

      if ((test_seq == null) || (file_name == "") || (test_seq.get_agent() == null)) begin
         return;
      end

      agent = test_seq.get_agent();
      `evl_log(UVM_DEBUG, VMOD_TCL, get_abstract_name(), $sformatf("TCL file found: %s for agent %s", file_name, agent.get_abstract_name()))

      //
      // We have found a TCL file to use, so create a TCL handler for it.
      //
      tcl_handler        = new();
      tcl_mem_seq_parser = new();
      test_seq.m_tcl_handler = tcl_handler;
      test_seq.m_file_name   = file_name;
      tcl_handler.set_agent(agent, test_seq.m_sequencer_number);
      tcl_handler.set_semaphore_wrapper(m_semaphore_wrapper);
      tcl_handler.set_message_wrapper(m_message_wrapper);
      tcl_handler.set_sequencer_id(test_seq.m_sequencer_number);
      tcl_handler.add_sequence_wrapper_for_agent(agent, test_seq.get_wrapper(), test_seq.m_sequencer_number);
      tcl_handler.add_tcl_cmd_parser(tcl_mem_seq_parser);
      tcl_handler.set_file_name(file_name);
      tcl_handler.set_starting_phase(MainPhase);
      tcl_handler.set_abstract_name($sformatf("TCL Handler Sequence for %0s, sequencer %0d (%0s)", agent.get_abstract_name(), test_seq.m_sequencer_number, file_name));
      m_tcl_handler_list.push_back(tcl_handler);
   endfunction : prepare_tcl_for_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_tcl_test");
      super.new(name_in);
      set_abstract_name("evl_tcl_test");
      if (`et_test_plusargs("crash_and_burn") != 0) begin
         m_crash_and_burn = 1'b1;
      end
   endfunction

endclass : evl_tcl_test
