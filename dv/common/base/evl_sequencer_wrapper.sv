//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequencer_wrapper_base
//
class evl_sequencer_wrapper_base extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object
   //
   virtual function uvm_object get_object();
      return null;
   endfunction : get_object


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      return;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   virtual function evl_verif_comp get_agent();
      return null;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   virtual function void set_rand_gen(input evl_rand_gen rand_gen_in);
      return;
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
   // Task: wait_for_seq_group_change
   //
   virtual task wait_for_seq_group_change();
      return;
   endtask : wait_for_seq_group_change


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequencer_wrapper_base");
      super.new(name_in);
   endfunction : new

endclass : evl_sequencer_wrapper_base;


//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequencer_wrapper
//
class evl_sequencer_wrapper #(type REQ = evl_sequence_item, type RSP = REQ) extends evl_sequencer_wrapper_base;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_sequencer #(REQ, RSP) m_object;


   `include "dv/common/base/evl_base_inc_wrap.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name for this object.
   //
   // Input Variables:
   //
   // name_in: This is the string to which to set the abstract name.
   //
   virtual function void set_abstract_name(input string name_in);
      super.set_abstract_name(name_in);
      if (m_object != null) begin
         m_object.set_abstract_name(name_in);
      end
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object
   //
   virtual function uvm_object get_object();
      return m_object;
   endfunction : get_object


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      if (m_object != null) begin
         m_object.set_agent(agent_in);
      end
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   virtual function evl_verif_comp get_agent();
      if (m_object != null) begin
         return m_object.get_agent();
      end
      return null;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   virtual function void set_rand_gen(input evl_rand_gen rand_gen_in);
      if (m_object != null) begin
         m_object.set_rand_gen(rand_gen_in);
      end
   endfunction : set_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequence_wrapper
   //
   virtual function void add_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (m_object != null) begin
         m_object.add_sequence_wrapper(sequence_wrapper_in);
      end
   endfunction : add_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_sequence_wrapper_on_ice
   //
   virtual function void put_sequence_wrapper_on_ice(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (m_object != null) begin
         m_object.put_sequence_wrapper_on_ice(sequence_wrapper_in);
      end
   endfunction : put_sequence_wrapper_on_ice


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reheat_sequence_wrapper
   //
   virtual function void reheat_sequence_wrapper(input evl_sequence_wrapper_base sequence_wrapper_in);
      if (m_object != null) begin
         m_object.reheat_sequence_wrapper(sequence_wrapper_in);
      end
   endfunction : reheat_sequence_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: change_seq_group
   //
   virtual function void change_seq_group(input int seq_group_in);
      if (m_object != null) begin
         m_object.change_seq_group(seq_group_in);
      end
   endfunction : change_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_seq_group_change
   //
   virtual task wait_for_seq_group_change();
      if (m_object != null) begin
         m_object.wait_for_seq_group_change();
      end
   endtask : wait_for_seq_group_change


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequencer_wrapper", input evl_sequencer #(REQ, RSP) object_in);
      super.new(name_in);
      m_object = object_in;
   endfunction : new

endclass : evl_sequencer_wrapper
