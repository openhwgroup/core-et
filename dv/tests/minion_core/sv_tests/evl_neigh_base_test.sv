class evl_neigh_base_test extends evl_stimulus;
	   `uvm_object_utils(evl_neigh_base_test)
      
      // Total number of ET-Link agents is 11 (8 cores + 2 PTW + 1 I-Cache)
      parameter NUM_ETLINK_NEIGH_AGENTS = 11;

	   //-----------------------------------------------------------------------------------------------
	   //
	   // Static Member Variables and Objects
	   //

      // Queue that holds all the CORE stimulus agents
	   evl_verif_comp verif_comp_list[$];

      // Queue that holds all the cache stim agents (L2, L3) in the case of
      // neigh TB to control the delays and the preloading 
	   evl_verif_comp cache_stim_agent[$];

      // Number of active core stimulus agents      
      int num_active_agents = 1;

      // Number of transactions per core agent
      // FUTURE fix num_txns to be per core instead
	   int num_txns = 1;

      // Number of coop_stores 
	   int num_coop_stores = 1;
	   int num_coop_loads = 1;

      // L2/L3 preloadig knob
	   bit preload = 1'b1;

      // Verif Components delays knobs
      bit l2_delay = `EVL_GLOBAL_DISABLE;
      bit [31:0] core_delay = `EVL_GLOBAL_DISABLE;

      // Temp queue and string for overriding the active agents from the 
      // command line	   
      int active_agents_q[$];
	   string active_agents_str;

	   static evl_neigh_base_test m_simple_test = new();

	   //-----------------------------------------------------------------------------------------------
	   //
	   // Function: build_phase
	   //

	   virtual function void get_stim_agents();
	      evl_verif_comp neigh_verif_agent[$];

	      // Retrieve cache stim agent (temp hack in order to check whether the stimulus is enabled or not
	      evl_verif_comp_map::get_verif_comp_list(cache_stim_agent, SHIRE_CACHE_STIM_AGENT);

	      if (evl_verif_comp_map::get_verif_comp_list(neigh_verif_agent, NEIGH_RTL_AGENT) != 0) begin

		      // Minion-agents (x2)
		      foreach(neigh_verif_agent[i])
		         neigh_verif_agent[i].get_vc_children_by_type(verif_comp_list, CORE_STIM_AGENT);
		      
            ////I-CACHE agents (x8)
		      //foreach(neigh_verif_agent[i])
		      //    neigh_verif_agent[i].get_vc_children_by_type(verif_comp_list, ICACHE_STIM_AGENT);

		      //// PTW agents (x2)
		      //foreach(neigh_verif_agent[i])
		      //    neigh_verif_agent[i].get_vc_children_by_type(verif_comp_list, PTW_STIM_AGENT);
         end
	      
         if (cache_stim_agent.size()  != 0) begin
		      foreach(cache_stim_agent[i]) begin
                cache_stim_agent[i].m_allow_mem_load = preload;
                cache_stim_agent[i].disable_delays(l2_delay);
            end
         end
       
         foreach (verif_comp_list[i]) begin
             verif_comp_list[i].disable_delays(core_delay[i]);
         end


	   endfunction


	   //-----------------------------------------------------------------------------------------------
	   //
	   // Function: prepare_test_for_use
	   //
      virtual function void prepare_test_for_use();
         evl_neigh_base_test_seq neigh_simple_test_seq_h;
	      int status;
	      // Queue with active agents
	      int num_active_agents_q[$];

	      // Get command line arguments for controlling the constraints and the stimulus
	      get_plusargs();

	      // Retrieve stimulus agents (ET-LINK agents, Core[0-7] + Icache)
	      get_stim_agents();
	      
	      //FUTURE : Should use factory create not new 
	      neigh_simple_test_seq_h = new();
	      
	      // Randomize virtual sequence (pass cmd line knobs) 
         status = neigh_simple_test_seq_h.randomize() with {num_txns == local::num_txns; num_coop_stores == local::num_coop_stores; num_coop_loads == local::num_coop_loads; };

         if (!status)
            `dut_error(get_abstract_name(), $sformatf("Failed to randomize neigh_simple_test_seq_h "))

		   foreach (verif_comp_list[i]) begin
            if (active_agents_q.size() > 0 ) begin
               if (i inside {active_agents_q} && i < NUM_ETLINK_NEIGH_AGENTS)
               begin
                  // Add the sequence to the verif component (Cache model agent in this case) sequencer.
                  verif_comp_list[i].add_sequence_wrapper(neigh_simple_test_seq_h.evl_core_req_seq_h[i].get_wrapper());
   		      end
               else
                   `dut_error(get_abstract_name(), $sformatf("Incorrect command line override (neigh_active_agents=%s), neigh has up to %0d ET-LINK agents", active_agents_str, NUM_ETLINK_NEIGH_AGENTS));
            end
            else begin
               if ( i < num_active_agents) begin
                  verif_comp_list[i].add_sequence_wrapper(neigh_simple_test_seq_h.evl_core_req_seq_h[i].get_wrapper());
              end
            end
        end

	    endfunction : prepare_test_for_use

	   virtual function void get_plusargs;
	      string active_agents_str_out[];

	      if(`et_value_plusargs("neigh_num_txns=%d", num_txns)) begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, generating #%0d transactions ",num_txns))
	      end
	      else begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Generating #%0d transactions ",num_txns))
	      end

         if(`et_value_plusargs("neigh_num_coop_stores=%d", num_coop_stores)) begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("command line override detected, generating #%0d coop_stores ",num_coop_stores))
	      end
	      else begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("generating #%0d transactions ",num_coop_stores))
	      end
         if(`et_value_plusargs("neigh_num_coop_loads=%d", num_coop_loads)) begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, generating #%0d coop_stores ",num_coop_loads))
	      end
	      else begin
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Generating #%0d transactions ",num_coop_loads))
	      end

	      //Parse string for active agents in the format of active_agents_q=I0:M0:M1:M2:M3:M4:M5:M6
	      if(`et_value_plusargs("neigh_active_agents=%s", active_agents_str))
	      begin

		      parse(active_agents_str_out, active_agents_str, ":");
	           
		      //FUTURE replace with function
		      foreach(active_agents_str_out[i])
		      begin
		         string agent_type_str;
		         string agent_id_str;
		         agent_type_str = active_agents_str_out[i].substr(0,2);
		         agent_id_str   = active_agents_str_out[i].substr(1,1);

               // Assumes 8 Miss/Evict agents, 8 I-cache agents + 2 PTW agents
		         if (agent_type_str.toupper() == "M")
		             active_agents_q.push_back(agent_id_str.atoi());
		         else if (agent_type_str.toupper() == "I")
		             active_agents_q.push_back(agent_id_str.atoi()+8);
		         else if (agent_type_str.toupper() == "P")
		             active_agents_q.push_back(agent_id_str.atoi()+16);
		    
               `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, running transactions on agent %s %s",active_agents_str_out[i], active_agents_str_out[i].substr(1,1)))

   		   end
	      end

	      foreach(active_agents_q[i])
	      begin      
		      `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Active agents queue[%0d]",active_agents_q[i]))
	      end
	      
	      //For now just get the number of active agents
	      if(`et_value_plusargs("neigh_num_agents=%d", num_active_agents))
             `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, running transactions on agents [0-%0d]",num_active_agents))
         else
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Running transactions on agents [0-%0d]",num_active_agents))
         if (num_active_agents > NUM_ETLINK_NEIGH_AGENTS || num_active_agents <= 0)
             `dut_error(get_abstract_name(), $sformatf("Incorrect command line override (num_active_agents=%0d), neigh has up to %0d ET-LINK agents", num_active_agents, NUM_ETLINK_NEIGH_AGENTS))

	      if(`et_value_plusargs("neigh_preload=%d", preload))
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, preload %0d",num_active_agents))
	      if(`et_value_plusargs("l2_delay=%d", l2_delay))
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, l2_delay %0d",l2_delay))
	      if(`et_value_plusargs("core_delay=%d", core_delay))
            `evl_log(UVM_HIGH, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, core_delay %0d",core_delay))

	   endfunction


	   //-----------------------------------------------------------------------------------------------
	   //
	   // Constructor
	   //
	   function new(input string name_in = "evl_neigh_base_test");
	      super.new(name_in);
	   endfunction


	   //-----------------------------------------------------------------------------------------------
	   //
	   // Function: parse
	   //           Parses an input string (in) for the active agents in a neigh and returns
	   //           an array with the agent IDs (M[0-7],I0, P[0-1] for Minion, Icache, Page Walk)
	   function automatic void parse( output string out [],
		                          input string in,
		                          input byte separator = "|"
		                        );

	   automatic int index [$];

	   foreach (in[i]) begin
		   if (in[i] == separator) begin
		     index.push_back(i-1);
		     index.push_back(i+1);
		   end
	   end
	   index.push_front(0);
	   index.push_back(in.len()-1);
	      
	   out = new[index.size()/2];

	   foreach (out[i]) begin
		   out[i] = in.substr(index[2*i], index[2*i+1]);
	   end

	   endfunction
	endclass : evl_neigh_base_test
