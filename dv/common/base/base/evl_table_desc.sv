//--------------------------------------------------------------------------------------------------
//
// Class: evl_table_desc
//
class evl_table_desc extends evl_desc;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   longint              m_index       = 0;
   bit                  m_index_set   = 1'b0;
   evl_cache_st_t       m_tag_action  = St_Ignore;
   evl_data_action_t    m_data_action = Data_None;
   evl_bus_cmd_t        m_bus_cmd     = `EVL_Bus_Idle;
   evl_bus_rsp_type_t   m_bus_rsp     = `EVL_Rsp_Null;
   int                  m_err_code    = 0;
   int                  m_cover_index = -1;
   evl_table_cover_base m_table_cover;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_index
   //
   function void set_index(input longint index_in);
      m_index     = index_in;
      m_index_set = 1'b0;
   endfunction : set_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cmd
   //
   function void set_bus_cmd(input evl_bus_cmd_t bus_cmd_in);
      m_bus_cmd = bus_cmd_in;
   endfunction : set_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_tag_action
   //
   function void set_tag_action(input evl_cache_st_t tag_action_in);
      m_tag_action = tag_action_in;
   endfunction : set_tag_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_action
   //
   function void set_data_action(input evl_data_action_t data_action_in);
      m_data_action = data_action_in;
   endfunction : set_data_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp
   //
   function void set_bus_rsp(input evl_bus_rsp_type_t bus_rsp_in);
      m_bus_rsp = bus_rsp_in;
   endfunction : set_bus_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_err_code
   //
   function void set_err_code(input int err_code_in);
      m_err_code = err_code_in;
   endfunction : set_err_code


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index
   //
   function longint get_index();
      return m_index;
   endfunction : get_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd
   //
   function evl_bus_cmd_t get_bus_cmd();
      return m_bus_cmd;
   endfunction : get_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_action
   //
   function evl_cache_st_t get_tag_action();
      return m_tag_action;
   endfunction : get_tag_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_data_action
   //
   function evl_data_action_t get_data_action();
      return m_data_action;
   endfunction : get_data_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: index_is_set
   //
   function bit index_is_set();
      return m_index_set;
   endfunction : index_is_set


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp
   //
   function evl_bus_rsp_type_t get_bus_rsp();
      return m_bus_rsp;
   endfunction : get_bus_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_err_code
   //
   function int get_err_code();
      return m_err_code;
   endfunction : get_err_code


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pending_bus_cmd
   //
   function bit pending_bus_cmd();
      return (m_bus_cmd inside { `EVL_Bus_Idle }) ? 1'b0 : 1'b1;
   endfunction : pending_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pending_tag_action
   //
   function bit pending_tag_action();
      return (m_tag_action inside { St_Ignore, St_Conflict }) ? 1'b0 : 1'b1;
   endfunction : pending_tag_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pending_conflict
   //
   function bit pending_conflict();
      return ((m_tag_action inside { St_Conflict }) ? 1'b1 : 1'b0);
   endfunction : pending_conflict


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pending_data_action
   //
   function bit pending_data_action();
      return (m_data_action inside { Data_None }) ? 1'b0 : 1'b1;
   endfunction : pending_data_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pending_action
   //
   function bit pending_action();
      return (pending_bus_cmd() | pending_tag_action() | pending_data_action());
   endfunction : pending_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_table_cover
   //
   function void set_table_cover(input evl_table_cover_base table_cover_in, input string name_in, input longint index_in = longint'(-1));
      if ((table_cover_in != null) && (m_cover_index < 0)) begin
         longint index = (index_in >= 0) ? index_in : m_index;

         m_table_cover = table_cover_in;
         m_cover_index = m_table_cover.add_table_cover_entry(index, name_in);
      end
   endfunction : set_table_cover


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample
   //
   function void sample();
      if ((m_table_cover != null) && (m_cover_index >= 0)) begin
         m_table_cover.sample(m_cover_index);
      end
   endfunction : sample


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_table_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_tag_action  = source_obj.m_tag_action;
      m_data_action = source_obj.m_data_action;
      m_bus_cmd     = source_obj.m_bus_cmd;
      m_bus_rsp     = source_obj.m_bus_rsp;
      m_err_code    = source_obj.m_err_code;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   function string sprint_obj();
      return $sformatf("tag_action: %0p - data_action: %0p - bus_cmd: %0p - bus_rsp: %0p - err_code: %0p", m_tag_action, m_data_action, m_bus_cmd, m_bus_rsp, m_err_code);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ssprint_obj: try to print "string/name" of enum
   //
   function string ssprint_obj();
      evl_abstract_bus_cmd_t   my_bus_cmd;
      evl_abstract_rsp_type_t  my_bus_rsp;
      evl_error_code           my_err_code;

      my_bus_cmd = evl_abstract_bus_cmd_t'(m_bus_cmd);
      my_bus_rsp = evl_abstract_rsp_type_t'(m_bus_rsp);
      my_err_code = evl_error_code'(m_err_code);

      return $sformatf("tag_action=%0p data_action=%0p bus_cmd=%0s bus_rsp=%0s err_code=%0s", m_tag_action, m_data_action, my_bus_cmd.name(), my_bus_rsp.name(), my_err_code.name());
   endfunction : ssprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_table_desc");
      super.new(name_in);
      m_desc_type = EVL_DESC_TABLE;
   endfunction : new

endclass : evl_table_desc
