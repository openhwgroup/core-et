//==================================================================================================
//
// Class: evl_objection
//
class evl_objection extends evl_object;

   `uvm_object_utils(evl_objection)


   //-----------------------------------------------------------------------------------------------
   //
   // Local Variables
   //
   int       m_count;
   uvm_phase m_phase;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr
   //
   function void incr(input string src_name);
      if (m_phase == null) begin
         m_count++;
         if (src_name != "") begin
            `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: incrementing objection count from %0s (now %0d)", get_abstract_name(), src_name, m_count))
         end
         else begin
            `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: incrementing objection count (now %0d)", get_abstract_name(), m_count))
         end
      end
   endfunction : incr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr
   //
   function void decr(input string src_name);
      if (m_count > 0) begin
         m_count--;
         if ((m_count > 0) || (m_phase == null)) begin
            if (src_name != "") begin
               `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: decrementing objection count from %0s (now %0d)", get_abstract_name(), src_name, m_count))
            end
            else begin
               `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: decrementing objection count (now %0d)", get_abstract_name(), m_count))
            end
         end
         if ((m_count == 0) && (m_phase != null)) begin
            if (src_name != "") begin
               `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: dropping objection from %0s", get_abstract_name(), src_name))
            end
            else begin
               `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: dropping objection", get_abstract_name()))
            end
            m_phase.drop_objection(this);
         end
      end
   endfunction : decr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: raise_objection
   //
   function void raise_objection(input uvm_phase phase_in, input string src_name);
      if ((m_count > 0) && (phase_in != null)) begin
         m_phase = phase_in;
         if (src_name == "") begin
            `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: raising objection (count %0d)", get_abstract_name(), m_count))
         end
         else begin
            `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: raising objection from %0s (count %0d)", get_abstract_name(), src_name, m_count))
         end
         m_phase.raise_objection(this);
      end
   endfunction : raise_objection


   //-----------------------------------------------------------------------------------------------
   //
   // Function: drop_objection
   //
   function void drop_objection();
      if ((m_count > 0) && (m_phase != null)) begin
         m_count = 0;
         `evl_log(UVM_MEDIUM, VMOD_GLOBAL, "OBJECTION", $sformatf("%0s: dropping objection", get_abstract_name()))
         m_phase.drop_objection(this);
      end
   endfunction : drop_objection


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string phase_name_in = "no_phase", input string name_in = "evl_objection");
      super.new(phase_name_in);
      set_abstract_name($sformatf("%0s::%0s", name_in, phase_name_in));
      m_count = 0;
      m_phase = null;
   endfunction

endclass : evl_objection
