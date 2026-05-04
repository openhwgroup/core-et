//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequencer_mem
//
class evl_sequencer_mem extends evl_sequencer#(evl_sequence_mem, evl_sequence_mem);

   `uvm_component_utils(evl_sequencer_mem)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_seq_group_change_delay)
   `evl_param_key(pk_incr_seq_group_changes)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int unsigned     m_start_index               = 0;
   bit              m_force_seq_group_zero      = 1'b1;
   bit              m_start_seq_group_changes   = 1'b0;
   bit              m_enqueue_seq_group_shuffle = 1'b0;
   bit              m_seq_group_shuffle         = 1'b0;
   int              m_sequence_limit            = -1;
   int              m_seq_group                 = -2;
   int              m_new_seq_group             = -1;
   bit              m_seq_group_change          = 1'b1;
   int              m_seq_group_count           = 0;
   int              m_seq_group_index           = 0;
   int              m_seq_group_list[$];
   evl_trans_desc   m_template_trans_desc;
   evl_sequence_mem m_all_sequences[$];
   evl_sequence_mem m_iced_sequences[$];
   evl_sequence_mem m_sequences[$];
   evl_sequence_mem m_last_sequence;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_weighted_list
   //
   virtual function void create_weighted_list();
      int min_weight = 32'h7fffffff;
      int max_weight = 32'h00000000;

      m_seq_group_count = 0;
      m_seq_group_list  = {};
      m_sequences       = {};
      foreach (m_all_sequences[ii]) begin
         int weight = m_all_sequences[ii].get_weight();

         if (weight > 0) begin
            int seq_group_list[];

            //
            // Extract all of the valid sequence groups from the available sequences.
            //
            if (m_all_sequences[ii].get_seq_group_list(seq_group_list) > 0) begin
               foreach (seq_group_list[jj]) begin
                  if ((seq_group_list[jj] > 0) && (!(seq_group_list[jj] inside { m_seq_group_list }))) begin
                     m_seq_group_list.push_back(seq_group_list[jj]);
                  end
               end
            end
            if ((m_new_seq_group <= 0) || (m_all_sequences[ii].use_for_seq_group(m_new_seq_group) == 1'b1)) begin
               if (weight > min_weight) begin
                  min_weight = weight;
               end
               if (weight < max_weight) begin
                  max_weight = weight;
               end
            end
         end
      end
      if (min_weight == max_weight) begin
         foreach (m_all_sequences[ii]) begin
            if ((m_new_seq_group <= 0) || (m_all_sequences[ii].use_for_seq_group(m_new_seq_group) == 1'b1)) begin
               m_sequences.push_back(m_all_sequences[ii]);
            end
         end
      end
      else begin
         foreach (m_all_sequences[ii]) begin
            if ((m_new_seq_group <= 0) || (m_all_sequences[ii].use_for_seq_group(m_new_seq_group) == 1'b1)) begin
               repeat (m_all_sequences[ii].get_weight()) begin
                  m_sequences.push_back(m_all_sequences[ii]);
               end
            end
         end
      end

      if (m_seq_group_shuffle == 1'b1) begin
         //
         // Randomize the sequence group list (if there is anything to randomize).
         //
         if (m_seq_group_list.size() > 1) begin
            `evl_log(UVM_LOW, VMOD_TEST, get_abstract_name(), $sformatf("shuffling the sequence groups"))
            m_seq_group_list.shuffle();
         end
      end

      if (m_sequences.size() > 15) begin
         repeat (7) begin
            m_sequences.shuffle();
         end
      end
      else if (m_sequences.size() > 1) begin
         repeat (3) begin
            m_sequences.shuffle();
         end
      end

      m_seq_group_count   = m_seq_group_list.size();
      m_seq_group_shuffle = m_enqueue_seq_group_shuffle;
   endfunction : create_weighted_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequence_wrapper
   //
   virtual function void add_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (sequence_wrapper_in != null) begin
         uvm_object sequence_obj;

         sequence_obj = sequence_wrapper_in.get_object();
         if (sequence_obj != null) begin
            evl_sequence_mem my_real_sequence;

            if ($cast(my_real_sequence, sequence_obj) == 0) begin
               `dut_error(get_abstract_name, $sformatf("could not cast an incoming sequence (id %0d) to the proper type", sequence_obj.get_inst_id()))
               return;
            end
            my_real_sequence.add_sequencer_wrapper(get_wrapper());
            m_all_sequences.push_back(my_real_sequence);
            m_seq_group_change = 1'b1;
         end
      end
   endfunction : add_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_sequence_wrapper_on_ice
   //
   virtual function void put_sequence_wrapper_on_ice(input evl_sequence_wrapper_base sequence_wrapper_in);
      bit seq_group_change = 1'b0;

      for (int ii = m_all_sequences.size() - 1; ii >= 0; ii--) begin
         if (m_all_sequences[ii].get_wrapper() == sequence_wrapper_in) begin
            m_iced_sequences.push_back(m_all_sequences[ii]);
            m_all_sequences.delete(ii);
            seq_group_change = 1'b1;
         end
      end
      if (seq_group_change == 1'b1) begin
         m_seq_group_change = 1'b1;
      end
   endfunction : put_sequence_wrapper_on_ice


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reheat_sequence_wrapper
   //
   virtual function void reheat_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in);
      return;
   endfunction : reheat_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: change_seq_group
   //
   virtual function void change_seq_group(input int seq_group_in);
      if (m_force_seq_group_zero == 1'b1) begin
         m_new_seq_group = 0;
      end
      else begin
         m_new_seq_group = seq_group_in;
      end
      if (seq_group_in != m_seq_group) begin
         `evl_log(UVM_LOW, VMOD_TEST, get_abstract_name(), $sformatf("changing sequence group %0d -> %0d", m_seq_group, seq_group_in))
         m_seq_group_change = 1'b1;
         if (m_agent.get_int_param(pk_incr_seq_group_changes, 0) == 0) begin
            m_enqueue_seq_group_shuffle = 1'b1;
         end
      end
   endfunction : change_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_seq_group_change
   //
   virtual task wait_for_seq_group_change();
      #0 wait (m_new_seq_group == m_seq_group);
   endtask : wait_for_seq_group_change


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_desc_template
   //
   virtual function void set_trans_desc_template(input evl_trans_desc trans_desc_template_in);
      m_template_trans_desc = trans_desc_template_in;
   endfunction : set_trans_desc_template


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_trans_desc
   //
   virtual task get_trans_desc(output evl_trans_desc trans_desc, output evl_sequence_mem sequence_mem);
      int unsigned cur_index;

      trans_desc = null;
      if ((m_seq_group != m_new_seq_group) || (m_seq_group_change == 1'b1)) begin
         create_weighted_list();
         m_sequence_limit   = m_sequences.size() - 1;
         m_seq_group        = m_new_seq_group;
         m_seq_group_change = 1'b0;
      end
      if (m_sequence_limit < 0) begin
         return;
      end
      if (m_sequence_limit == 0) begin
         m_start_index = 0;
      end
      else begin
         m_start_index = m_rand_gen.get_int(m_sequence_limit);
      end
      cur_index = m_start_index;
      do begin
         trans_desc = m_sequences[cur_index].get_trans_desc(this, m_agent, m_template_trans_desc);
         if (trans_desc != null) begin
            sequence_mem    = m_sequences[cur_index];
            m_last_sequence = sequence_mem;
            return;
         end
         else begin
            cur_index++;
            if (cur_index > m_sequence_limit) begin
               cur_index = 0;
            end
         end
      end while (m_start_index != cur_index);
      return;
   endtask : get_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Task: perform_random_seq_group_changes
   //
   virtual task perform_random_seq_group_changes(input int delay_key);
      evl_delay_t delay;

      wait (m_start_seq_group_changes == 1'b1);
      forever begin
         #0 wait (m_seq_group_count > 0);
         if (m_agent.get_next_delay(delay, delay_key, null, 1) == 1'b0) begin
            m_agent.wait_for_delay(delay_key);
            m_agent.wait_for_delay_enable();
         end
         else begin
            if (m_seq_group_index >= m_seq_group_count) begin
               m_seq_group_index = 0;
            end
            change_seq_group(m_seq_group_list[m_seq_group_index]);
            wait_for_seq_group_change();
            m_seq_group_index++;
         end
         m_agent.wait_cycles(delay);
      end
   endtask : perform_random_seq_group_changes


   //-----------------------------------------------------------------------------------------------
   //
   // Task: main_phase
   //
   virtual task main_phase(input uvm_phase phase);
      if (m_agent != null) begin
         m_force_seq_group_zero    = 1'b0;
         m_start_seq_group_changes = 1'b1;
      end
      super.main_phase(phase);
   endtask : main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: post_main_phase
   //
   virtual task post_main_phase(input uvm_phase phase);
      m_force_seq_group_zero = 1'b1;
      if (m_seq_group_count > 0) begin
         foreach (m_seq_group_list[ii]) begin
            m_seq_group_list[ii] = 0;
         end
      end
      if (m_agent != null) begin
         change_seq_group(0);
         m_seq_group = 0;
      end
      super.post_main_phase(phase);
   endtask : post_main_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(input uvm_phase phase);
      fork
         perform_random_seq_group_changes(pk_seq_group_change_delay);
      join_none
      super.run_phase(phase);
   endtask : run_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequencer_mem", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
   endfunction : new

endclass : evl_sequencer_mem
