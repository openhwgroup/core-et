//--------------------------------------------------------------------------------------------------
//
// Class: evl_rtl_instance
//
class evl_rtl_instance extends evl_void;

   `include "dv/common/base/evl_base_inc.svh"


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   string           m_full_name;
   string           m_parent_name;
   string           m_instance_name;
   string           m_sv_comp;
   string           m_param_strings[$];
   evl_agent_type_t m_agent_type;
   evl_agent_id_t   m_agent_pid;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_name
   //
   function string get_full_name();
      return m_full_name;
   endfunction : get_full_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_parent_name
   //
   function string get_parent_name();
      return m_parent_name;
   endfunction : get_parent_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_inst_name
   //
   function string get_inst_name();
      return m_instance_name;
   endfunction : get_inst_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_type
   //
   function evl_agent_type_t get_agent_type();
      return m_agent_type;
   endfunction : get_agent_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_pid
   //
   function evl_agent_id_t get_agent_pid();
      return m_agent_pid;
   endfunction : get_agent_pid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sv_comp
   //
   function string get_sv_comp();
      return m_sv_comp;
   endfunction : get_sv_comp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: skip_leading_space
   //
   function string skip_leading_space(input string param_string_in);
      int index;
      int limit;

      index = 0;
      limit = param_string_in.len();
      while (index < limit) begin
         byte cc = param_string_in.getc(index);

         if (cc > byte'(" ")) begin
            return param_string_in.substr(index, limit - 1);
         end
         else begin
            index++;
         end
      end
      return "";
   endfunction : skip_leading_space


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_string_start
   //
   function string find_string_start(input string param_string_in);
      int index;
      int limit;

      index = 0;
      limit = param_string_in.len();
      while (index < limit) begin
         byte cc = param_string_in.getc(index);

         if ( ((cc >= byte'("A")) && (cc <= byte'("Z"))) ||
              ((cc >= byte'("a")) && (cc <= byte'("z"))) ) begin
            return param_string_in.substr(index, limit - 1);
         end
         else begin
            index++;
         end
      end
      return "";
   endfunction : find_string_start


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_string_end
   //
   function int find_string_end(input string param_string_in);
      int index;
      int limit;

      index = 0;
      limit = param_string_in.len();
      while (index < limit) begin
         byte cc = param_string_in.getc(index);

         if ( ((cc >= byte'("0")) && (cc <= byte'("9"))) ||
              ((cc >= byte'("A")) && (cc <= byte'("Z"))) ||
              ((cc >= byte'("a")) && (cc <= byte'("z"))) || (cc == byte'("_")) ) begin
            index++;
         end
         else begin
            return (index - 1);
         end
      end
      return -1;
   endfunction : find_string_end


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_numeric_string
   //
   function string find_numeric_string(input string param_string_in);
      int    index;
      int    limit;
      int    base;
      string param_string;
      string param_value;

      param_string = skip_leading_space(param_string_in);
      param_value  = "";
      index        = 0;
      base         = 10;
      limit        = param_string.len();
      if (limit >= 2) begin
         if (param_string.getc(0) == byte'("0")) begin
            if (param_string.getc(1) inside { byte'("X"), byte'("x") }) begin
               base = 16;
               param_string = param_string.substr(2, limit - 1);
            end
            else if (param_string.getc(1) inside { byte'("B"), byte'("b") }) begin
               base = 2;
               param_string = param_string.substr(2, limit - 1);
            end
            else begin
               base = 8;
            end
         end
      end

      limit = param_string.len();
      while (index < limit) begin
         byte cc = param_string_in.getc(index);

         if (base == 10) begin
            if ((cc >= byte'("0")) && (cc <= byte'("9"))) begin
               index++;
            end
            else begin
               return ($sformatf("D%s", param_string.substr(0, index - 1)));
            end
         end
         else if (base == 8) begin
            if ((cc >= byte'("0")) && (cc <= byte'("7"))) begin
               index++;
            end
            else begin
               return ($sformatf("O%s", param_string.substr(0, index - 1)));
            end
         end
         else if (base == 2) begin
            if ((cc >= byte'("0")) && (cc <= byte'("1"))) begin
               index++;
            end
            else begin
               return ($sformatf("B%s", param_string.substr(0, index - 1)));
            end
         end
         else if (base == 16) begin
            if ( ((cc >= byte'("0")) && (cc <= byte'("9"))) ||
                 ((cc >= byte'("A")) && (cc <= byte'("F"))) ||
                 ((cc >= byte'("a")) && (cc <= byte'("f"))) ) begin
               index++;
            end
            else begin
               return ($sformatf("X%s", param_string.substr(0, index - 1)));
            end
         end
      end
      return ($sformatf("D%s", param_string.substr(0, limit - 1)));
   endfunction : find_numeric_string


   //-----------------------------------------------------------------------------------------------
   //
   // Function: skip_numeric_string
   //
   function string skip_numeric_string(input string param_string_in);
      int    index;
      int    limit;
      string param_string;

      param_string = skip_leading_space(param_string_in);
      index        = 0;
      limit        = param_string.len();
      while (index < limit) begin
         byte cc = param_string_in.getc(index);

         if ((cc <= byte'(" ")) || (cc == byte'(",")) || (cc == byte'(";"))) begin
            return (param_string.substr(index + 1, limit - 1));
         end
         else begin
            index++;
         end
      end
      return "";
   endfunction : skip_numeric_string


   //-----------------------------------------------------------------------------------------------
   //
   // Function: parse_param_string
   //
   function void parse_param_string(input string param_string_in);
      int    param_end;
      string cur_param_string;
      string param_name;
      string param_value;

      cur_param_string = param_string_in;
      while (cur_param_string != "") begin
         cur_param_string = find_string_start(cur_param_string);
         param_end        = find_string_end(cur_param_string);
         if (param_end < 0) begin
            cur_param_string = "";
         end
         else begin
            param_name       = cur_param_string.substr(0, param_end);
            cur_param_string = skip_leading_space(cur_param_string.substr(param_end + 1, cur_param_string.len() - 1));
            if ((param_name != "") && (cur_param_string != "") && (cur_param_string.getc(0) == byte'("="))) begin
               cur_param_string = cur_param_string.substr(1, cur_param_string.len() - 1);
               param_value      = find_numeric_string(cur_param_string);
               cur_param_string = skip_numeric_string(cur_param_string);
               if (param_value != "") begin
                  int value;

                  if (param_value.getc(0) == byte'("D")) begin
                     param_value = param_value.substr(1, param_value.len() - 1);
                     value = param_value.atoi();
                  end
                  else if (param_value.getc(0) == byte'("X")) begin
                     param_value = param_value.substr(1, param_value.len() - 1);
                     value = param_value.atohex();
                  end
                  else if (param_value.getc(0) == byte'("O")) begin
                     param_value = param_value.substr(1, param_value.len() - 1);
                     value = param_value.atooct();
                  end
                  else begin
                     param_value = param_value.substr(1, param_value.len() - 1);
                     value = param_value.atobin();
                  end
                  set_int_param_by_name(param_name, value);
                  m_param_strings.push_back(param_name);
               end
            end
         end
      end
   endfunction : parse_param_string


   //-----------------------------------------------------------------------------------------------
   //
   // Function: propagate_param_strings
   //
   function void propagate_param_strings(input evl_verif_comp agent_in);
      foreach (m_param_strings[ii]) begin
         agent_in.set_int_param_by_name(m_param_strings[ii], get_int_param_by_name(m_param_strings[ii]));
      end
   endfunction : propagate_param_strings


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           name_in       = "evl_rtl_instance",
                input evl_agent_type_t agent_type_in = NO_AGENT,
                input evl_agent_id_t   agent_pid_in  = ~evl_agent_id_t'(0),
                input string           sv_comp_in    = "unknown",
                input string           param_list_in = "",
                input int              skip_levels_in[] = {});
      int parent_skip_levels[$];
      int skip_levels[$];

      super.new(name_in);
      void'(evl_base_pkg::evl_log_create());
      parent_skip_levels.push_back(1);
      if (skip_levels_in.size() == 0) begin
         skip_levels = {};
      end
      else begin
         foreach (skip_levels_in[ii]) begin
            skip_levels.push_back(skip_levels_in[ii] + 1);
            parent_skip_levels.push_back(skip_levels_in[ii] + 1);
         end
      end
      m_full_name     = name_in;
      m_agent_type    = agent_type_in;
      m_agent_pid     = agent_pid_in;
      m_sv_comp       = sv_comp_in;
      m_parent_name   = evl_base_pkg::parse_hier_levels(name_in, "", "", parent_skip_levels);
      m_instance_name = evl_base_pkg::parse_hier_levels(name_in, "", "", skip_levels, 1'b1);
      if (m_parent_name == m_instance_name) begin
         m_parent_name = "";
      end
      if (param_list_in != "") begin
         parse_param_string(param_list_in);
      end
      uvm_config_db#(evl_rtl_instance)::set(null, m_full_name, "instance", this);
      evl_rtl_instance_map::add_instance(this);

      `evl_log(UVM_MEDIUM, VMOD_CONFIG, "Inst", $sformatf("created instance of %0p (%0s, id %0d, agent_pid %0d)", m_agent_type, get_abstract_name(), get_inst_id(), m_agent_pid))
   endfunction : new

endclass : evl_rtl_instance
