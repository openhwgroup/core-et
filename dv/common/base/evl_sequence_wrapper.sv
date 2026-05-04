//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_wrapper_base
//
// This class is used to act as a proxy for typed sequences.  Certain base classes (such as
// evl_verif_comp, evl_verif_comp_map, evl_tcl_handler, evl_tcl_cmd_parser) need to have access to
// some subset of methods within sequences.  To support something other than memory sequences, these
// base classes cannot assume memory sequence usage.  evl_sequence_wrapper_base (the untyped
// sequence wrapper) can act as a proxy for these base classes.
//
// While the base classes will have handles to the untyped sequence wrapper, this wrapper is
// extended to be typed to match the typed sequence (see the evl_sequence_wrapper class definition
// in this file).
//
class evl_sequence_wrapper_base extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object
   //
   // This virtual function is used to get a handle to the actual typed sequence.  It is redefined
   // in evl_sequence_wrapper.
   //
   // Return Value:
   //
   // uvm_object: This is the handle to the typed sequence.  The assumption is that this return
   //             value will need to be cast by the caller.
   //
   virtual function uvm_object get_object();
      return null;
   endfunction : get_object


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   // This virtual function is used to set the agent handle in the actual typed sequence.  It is
   // redefined in evl_sequence_wrapper.
   //
   // Input Variables:
   //
   // agent_in: This is a handle to the agent assigned to the sequence.
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      return;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   // This virtual function is used to get the agent handle from the actual typed sequence.  It is
   // redefined in evl_sequence_wrapper.
   //
   // Return Value:
   //
   // evl_verif_comp: This is the handle to the agent assigned to the typed sequence.
   //
   virtual function evl_verif_comp get_agent();
      return null;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: start_sequence
   //
   // This virtual task is used to start phase evaluation in the actual typed sequence.  It is
   // redefined in evl_sequence_wrapper.
   //
   virtual task start_sequence();
      return;
   endtask : start_sequence


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_terminating_seq
   //
   // This virtual task is used to configure the actual typed sequence as a terminating sequence.
   // It is redefined in evl_sequence_wrapper.
   //
   virtual task convert_to_terminating_seq();
      return;
   endtask : convert_to_terminating_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_background_seq
   //
   // This virtual task is used to configure the actual typed sequence as a background sequence.  It
   // is redefined in evl_sequence_wrapper.
   //
   virtual task convert_to_background_seq();
      return;
   endtask : convert_to_background_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequencer_wrapper
   //
   virtual function void add_sequencer_wrapper(input evl_sequencer_wrapper_base sequencer_wrapper_in);
      return;
   endfunction : add_sequencer_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_weight
   //
   virtual function void set_weight(input int weight_in);
      return;
   endfunction : set_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_weight
   //
   virtual function int get_weight();
      return 0;
   endfunction : get_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_seq_group
   //
   // This virtual function is used to add a sequence group for the sequence.  It is redefined in
   // evl_sequence_wrapper.
   //
   virtual function void add_seq_group(input int seq_group_in);
      return;
   endfunction : add_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_seq_group_list
   //
   // This virtual function is used to get the sequence group list for the sequence.  It is
   // redefined in evl_sequence_wrapper.
   //
   virtual function int get_seq_group_list(output int seq_group_list[]);
      seq_group_list = {};
      return 0;
   endfunction : get_seq_group_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_for_seq_group
   //
   // This virtual function is used to determine if the sequence should be used for the specified
   // sequence group.  It is redefined in evl_sequence_wrapper.
   //
   virtual function bit use_for_seq_group(input int seq_group_in);
      return 1'b1;
   endfunction : use_for_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_wrapper_base");
      super.new(name_in);
   endfunction : new

endclass : evl_sequence_wrapper_base;


//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_wrapper
//
// This class is a typed version of evl_sequence_wrapper_base, which is used to act as a proxy for
// typed sequences.
//
class evl_sequence_wrapper #(type REQ = evl_sequence_item, type RSP = REQ) extends evl_sequence_wrapper_base;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_object: This is handle to the actual sequence.
   //
   evl_sequence #(REQ, RSP) m_object;


   `include "dv/common/base/evl_base_inc_wrap.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object
   //
   // This function is used to get a handle to the actual typed sequence.
   //
   // Return Value:
   //
   // uvm_object: This is the handle to the typed sequence.  The assumption is that this return
   //             value will need to be cast by the caller.
   //
   virtual function uvm_object get_object();
      return m_object;
   endfunction : get_object


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   // This function is used to set the agent handle in the actual typed sequence.
   //
   // Input Variables:
   //
   // agent_in: This is a handle to the agent assigned to the sequence.
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
   // This virtual function is used to get the agent handle from the actual typed sequence.
   //
   // Return Value:
   //
   // evl_verif_comp: This is the handle to the agent assigned to the typed sequence.
   //
   virtual function evl_verif_comp get_agent();
      if (m_object != null) begin
         return m_object.get_agent();
      end
      return null;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: start_sequence
   //
   // This task is used to start phase evaluation in the actual typed sequence.
   //
   virtual task start_sequence();
      if (m_object != null) begin
         m_object.start_sequence();
      end
   endtask : start_sequence


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_terminating_seq
   //
   // This task is used to configure the actual typed sequence as a terminating sequence.
   //
   virtual task convert_to_terminating_seq();
      if (m_object != null) begin
         m_object.convert_to_terminating_seq();
      end
   endtask : convert_to_terminating_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Task: convert_to_background_seq
   //
   // This task is used to configure the actual typed sequence as a background sequence.
   //
   virtual task convert_to_background_seq();
      if (m_object != null) begin
         m_object.convert_to_background_seq();
      end
   endtask : convert_to_background_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_sequencer_wrapper
   //
   virtual function void add_sequencer_wrapper(input evl_sequencer_wrapper_base sequencer_wrapper_in);
      if (m_object != null) begin
         m_object.add_sequencer_wrapper(sequencer_wrapper_in);
      end
   endfunction : add_sequencer_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_weight
   //
   virtual function void set_weight(input int weight_in);
      if (m_object != null) begin
         m_object.set_weight(weight_in);
      end
   endfunction : set_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_weight
   //
   virtual function int get_weight();
      if (m_object != null) begin
         return m_object.get_weight();
      end
      return 0;
   endfunction : get_weight


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_seq_group
   //
   // This virtual function is used to add a sequence group for the sequence.
   //
   virtual function void add_seq_group(input int seq_group_in);
      if (m_object != null) begin
         m_object.add_seq_group(seq_group_in);
      end
   endfunction : add_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_seq_group_list
   //
   // This virtual function is used to get the sequence group list for the sequence.
   //
   virtual function int get_seq_group_list(output int seq_group_list[]);
      if (m_object != null) begin
         return m_object.get_seq_group_list(seq_group_list);
      end
      return 0;
   endfunction : get_seq_group_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: use_for_seq_group
   //
   // This virtual function is used to determine if the sequence should be used for the specified
   // sequence group.
   //
   virtual function bit use_for_seq_group(input int seq_group_in);
      if (m_object != null) begin
         return m_object.use_for_seq_group(seq_group_in);
      end
      return 1'b1;
   endfunction : use_for_seq_group


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_wrapper", input evl_sequence #(REQ, RSP) object_in);
      super.new(name_in);
      m_object = object_in;
   endfunction : new

endclass : evl_sequence_wrapper
