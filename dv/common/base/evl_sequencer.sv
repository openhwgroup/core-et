//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequencer
//
class evl_sequencer #(type REQ = evl_sequence_item, type RSP = REQ) extends uvm_sequencer#(REQ, RSP);

   `uvm_component_param_utils(evl_sequencer #(REQ, RSP))


   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_sequencer_wrapper #(REQ, RSP) m_sequencer_wrapper = null;
   evl_verif_comp                    m_agent;
   evl_rand_gen                      m_rand_gen;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_wrapper
   //
   virtual function evl_sequencer_wrapper_base get_wrapper();
      return m_sequencer_wrapper;
   endfunction : get_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      m_agent = agent_in;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   virtual function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   function void set_rand_gen(input evl_rand_gen rand_gen_in);
      m_rand_gen = rand_gen_in;
   endfunction : set_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequence_wrapper
   //
   virtual function void add_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in);
      return;
   endfunction : add_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_sequence_wrapper_on_ice
   //
   virtual function void put_sequence_wrapper_on_ice(input evl_sequence_wrapper_base sequence_wrapper_in);
      return;
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
      return;
   endfunction : change_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_seq_group_change
   //
   virtual task wait_for_seq_group_change();
      return;
   endtask : wait_for_seq_group_change


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequencer", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      m_sequencer_wrapper = new(name_in, this);
   endfunction : new

endclass : evl_sequencer
