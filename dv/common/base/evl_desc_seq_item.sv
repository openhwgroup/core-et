//--------------------------------------------------------------------------------------------------
//
// Class: evl_desc_seq_item
//
virtual class evl_desc_seq_item extends evl_sequence_item;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_desc_type_t m_desc_type    = EVL_DESC_UNKNOWN;
   evl_verif_comp  m_master_agent = null;
   evl_verif_comp  m_slave_agent  = null;
   evl_core_type_t m_core_type    = EVL_MinionCore;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_desc_seq_item source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_master_agent = source_obj.m_master_agent;
      m_slave_agent  = source_obj.m_slave_agent;
      m_core_type    = source_obj.m_core_type;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_desc_type
   //
   function evl_desc_type_t get_desc_type();
      return m_desc_type;
   endfunction : get_desc_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_type
   //
   virtual function void set_core_type(input evl_core_type_t core_type_in);
      m_core_type = core_type_in;
   endfunction : set_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_type
   //
   virtual function evl_core_type_t get_core_type();
      return m_core_type;
   endfunction : get_core_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_master_agent
   //
   function void set_master_agent(input evl_verif_comp agent_in);
      m_master_agent = agent_in;
   endfunction : set_master_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_master_agent
   //
   function evl_verif_comp get_master_agent();
      return m_master_agent;
   endfunction : get_master_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_slave_agent
   //
   function void set_slave_agent(input evl_verif_comp agent_in);
      m_slave_agent = agent_in;
   endfunction : set_slave_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_slave_agent
   //
   function evl_verif_comp get_slave_agent();
      return m_slave_agent;
   endfunction : get_slave_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_desc_seq_item");
      super.new(name_in);
   endfunction : new

endclass : evl_desc_seq_item
