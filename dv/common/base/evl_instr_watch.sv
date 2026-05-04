//==================================================================================================
//
// Class: evl_instr_watch
//
class evl_instr_watch extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit         m_pc_valid;
   bit         m_instr_valid;
   bit         m_source_valid;
   evl_vaddr_t m_pc;
   evl_instr_t m_instr;
   int         m_source;
   event       m_match;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pc
   //
   function evl_vaddr_t get_pc();
      return m_pc;
   endfunction : get_pc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_instr
   //
   function evl_instr_t get_instr();
      return m_instr;
   endfunction : get_instr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_source
   //
   function int get_source();
      return m_source;
   endfunction : get_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_watch_params
   //
   function void set_watch_params(input bit         use_pc,
                                  input evl_vaddr_t pc_in,
                                  input bit         use_instr,
                                  input evl_instr_t instr_in,
                                  input bit         use_source,
                                  input int         source_in);
      m_pc_valid     = use_pc;
      m_pc           = pc_in;
      m_instr_valid  = use_instr;
      m_instr        = instr_in;
      m_source_valid = use_source;
      m_source       = source_in;
   endfunction : set_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_watch_params
   //
   function bit have_watch_params(input bit         use_pc,
                                  input evl_vaddr_t pc_in,
                                  input bit         use_instr,
                                  input evl_instr_t instr_in,
                                  input bit         use_source,
                                  input int         source_in);
      if ( (m_pc_valid     == use_pc)     && (m_pc     == pc_in)    &&
           (m_instr_valid  == use_instr)  && (m_instr  == instr_in) &&
           (m_source_valid == use_source) && (m_source == source_in) ) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : have_watch_params


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_match
   //
   task wait_for_match();
      #0 @(m_match);
   endtask : wait_for_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_for_match
   //
   function void check_for_match(input evl_vaddr_t pc_in,
                                 input evl_instr_t instr_in,
                                 input int         source_in);
      if ( ((m_pc_valid     == 1'b0) || (m_pc     == pc_in))    &&
           ((m_instr_valid  == 1'b0) || (m_instr  == instr_in)) &&
           ((m_source_valid == 1'b0) || (m_source == source_in)) ) begin
         if (m_pc_valid == 1'b0) begin
            m_pc = pc_in;
         end
         if (m_instr_valid == 1'b0) begin
            m_instr = instr_in;
         end
         if (m_source_valid == 1'b0) begin
            m_source = source_in;
         end
         -> m_match;
      end
   endfunction : check_for_match


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      m_pc_valid     = 1'b0;
      m_instr_valid  = 1'b0;
      m_source_valid = 1'b0;
      m_pc           = evl_vaddr_t'(0);
      m_instr        = evl_instr_t'(0);
      m_source       = -1;
   endfunction : new

endclass : evl_instr_watch
