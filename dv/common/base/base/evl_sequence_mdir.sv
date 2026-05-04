//--------------------------------------------------------------------------------------------------
//
// Class: evl_sequence_mdir
//
class evl_sequence_mdir extends evl_sequence_mem;

   `uvm_object_utils(evl_sequence_mdir)


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sequence_mdir");
      super.new(name_in);
      m_cmds        = {};
      m_paddrs      = {};
      m_scp_addrs   = {};
      m_fill_addrs  = {};
      m_cache_attrs = {};
      m_cmd_lengths = {};
      m_addr_spaces = {};
      m_dwords      = {};
      m_qword_ens   = {};
      m_req_sources = {};
      m_trans_attrs = {};
      m_qoses       = {};
      m_vcs         = {};
   endfunction : new

endclass : evl_sequence_mdir
