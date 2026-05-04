//==================================================================================================
//
// Class: evl_plus_arg
//
// This class is used to support special evaluation of +args.  In particular, we want to be able to
// restrict usage of some +arg options to specific shires/neighborhoods/cores.  The syntax for
// this is to include a comma-separated string specifying these elements after the +arg and before
// any value-setting "=" sign.  The comma-separated values take on the following forms:
//
//    s<x>           - <x> is a shire ID
//    n<y>           - <y> is a neighborhood ID
//    c<z>           - <z> is a core ID
//    s<x>,n<y>      - <x> is a shire ID and <y> is a neighborhood ID
//    s<x>,c<z>      - <x> is a shire ID and <z> is a core ID
//    n<y>,c<z>      - <y> is a neighborhood ID and <z> is a core ID
//    s<x>,n<y>,c<z> - <x> is a shire ID, <y> is a neighborhood ID, and <z> is a core ID
//
// So to specify a particular TCL file for neighborhoods with an ID of 5 (regardless of shire), you
// could use the following:
//
//    +neighTCL,n5=<full_path>
//
// To specify a particular TCL file for neighborhood 4 in shire 2, you could use the following:
//
//    +neighTCL,s2,n4=<full_path>
//
class evl_plus_arg extends uvm_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_root_args[]:     This array contains all of the comma-separated arguments in a +arg that
   //                    occur before the first "=" sign.  m_root_args[0] will always be the base
   //                    +arg value (before any commas).  m_root_args[1] will the string after the
   //                    first comma but before the second comma, etc.
   //                    ---------------------------------------------------------------------------
   // m_arg_value:       This string is the value after the first "=" sign.
   //                    ---------------------------------------------------------------------------
   // m_shire_id:        To simplify checking, some predecoding is done while processing the +arg.
   //                    This field is the shire ID (i.e., the integer in the "s<x>"  field).  If
   //                    shire field is found this will be ~evl_shire_id_t'(0).
   //                    ---------------------------------------------------------------------------
   // m_neighborhood_id: Like the shire ID, this field is the neighborhood ID (i.e., the integer in
   //                    the "n<y>" field).  If no neighborhood field is found this will be
   //                    ~evl_neighborhood_id_t'(0).
   //                    ---------------------------------------------------------------------------
   // m_core_id:         Finally, this field is the core ID (i.e., the integer in the "c<z>"
   //                    field).  If no core field is found this will be ~evl_core_id_t'(0).
   //
   string                m_root_args[$];
   string                m_arg_value       = "";
   evl_shire_id_t        m_shire_id        = ~evl_shire_id_t'(0);
   evl_neighborhood_id_t m_neighborhood_id = ~evl_neighborhood_id_t'(0);
   evl_core_id_t         m_core_id         = ~evl_core_id_t'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: arg_match
   //
   // This function is used to determine if a specific match can be found for this +arg.
   //
   // Input Variables:
   //
   // arg_name_in:        This is the root +arg name (before any commas) for which you are trying to
   //                     check.
   // shire_id_in:        This is the shire ID of the object testing for a +arg.  This could be the
   //                     shire ID of the agent on which a particular test is running, for example.
   //                     If there is no shire ID, the default value of ~evl_shire_id_t'(0) should
   //                     be used.
   // neighborhood_id_in: This is the nerighborhood ID of the object testing for a +arg.  This could
   //                     be the nerighborhood ID of the agent on which a particular test is
   //                     running, for example.  If there is no nerighborhood ID, the default value
   //                     of ~evl_nerighborhood_id_t'(0) should be used.
   // core_id_in:         This is the core ID of the object testing for a +arg.  This could be the
   //                     core ID of the agent on which a particular test is running, for example.
   //                     If there is no core ID, the default value of ~evl_core_id_t'(0) should
   //                     be used.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if a match is found.  A match is defined as follows:
   //
   //      - the root +arg name must match m_root_args[0]
   //      - if both the object's shire ID and the +arg's shire ID are not ~evl_shire_id_t'(0), then
   //        they must match.
   //      - if both the object's neighborhood ID and the +arg's neighborhood ID are not
   //        ~evl_neighborhood_id_t'(0), then they must match.
   //      - if both the object's core ID and the +arg's core ID are not ~evl_core_id_t'(0),
   //        then they must match.
   //
   function bit arg_match(input string                arg_name_in,
                          input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                          input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                          input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0),
                          input bit [2:0]             use_specific       = 3'b000);
      if (m_root_args[0] != arg_name_in) begin
         return 1'b0;
      end

      //
      // If we are looking for a specific +arg match (one that has comma-separated values), then
      // make sure this +arg has comma-separated values.
      //
      if ( ((use_specific[2:2] == 1'b1) && (m_shire_id        == ~evl_shire_id_t'(0)))        ||
           ((use_specific[1:1] == 1'b1) && (m_neighborhood_id == ~evl_neighborhood_id_t'(0))) ||
           ((use_specific[0:0] == 1'b1) && (m_core_id         == ~evl_core_id_t'(0))) ) begin
         return 1'b0;
      end
      if ((shire_id_in != m_shire_id) && (shire_id_in != ~evl_shire_id_t'(0)) && (m_shire_id != ~evl_shire_id_t'(0))) begin
         return 1'b0;
      end
      if ((neighborhood_id_in != m_neighborhood_id) && (neighborhood_id_in != ~evl_neighborhood_id_t'(0)) && (m_neighborhood_id != ~evl_neighborhood_id_t'(0))) begin
         return 1'b0;
      end
      if ((core_id_in != m_core_id) && (core_id_in != ~evl_core_id_t'(0)) && (m_core_id != ~evl_core_id_t'(0))) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : arg_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_arg_list
   //
   // This function is used to get a list of all of the +arg parameters before the first "=" sign.
   //
   // Output Values:
   //
   // root_args[]: This is an array of strings containing all of the +arg parameters.
   //
   // Return Value:
   //
   // int: The number of +arg parameters is returned.
   //
   function int get_root_arg_list(output string root_args[]);
      if (m_root_args.size() == 0) begin
         root_args = new[1];
         root_args[0] = "";
         return 0;
      end
      root_args = new[m_root_args.size()];
      foreach (m_root_args[ii]) begin
         root_args[ii] = m_root_args[ii];
      end
      return root_args.size();
   endfunction : get_root_arg_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_arg
   //
   // This function is used to get the base +arg name (before any commas).
   //
   // Return Value:
   //
   // string: This is the base +arg name (m_root_args[0]).
   //
   function string get_root_arg();
      if (m_root_args.size() == 0) begin
         return "";
      end
      return m_root_args[0];
   endfunction : get_root_arg


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_arg_value
   //
   // This function is used to get the string value for the +arg (after the first "=" sign).
   //
   // Return Value:
   //
   // string: This is the string value for this +arg.
   //
   function string get_arg_value();
      return m_arg_value;
   endfunction : get_arg_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_arg_value_as_int
   //
   // This function is used to get the int value for the +arg (after the first "=" sign).
   //
   // Return Value:
   //
   // longint: This is the value for this +arg.
   //
   function longint get_arg_value_as_int();
      longint result = longint'(0);
      int     base   = 10;
      int     limit;
      string  raw_arg;

      limit   = m_arg_value.len();
      raw_arg = m_arg_value.tolower();
      if (limit == 0) begin
         result = longint'(-1);
      end
      else begin
         if (limit > 2) begin
            if (raw_arg.substr(0, 1) == "0b") begin
               base    = 2;
               raw_arg = raw_arg.substr(2, limit - 1);
               limit   = raw_arg.len();
            end
            else if (raw_arg.substr(0, 1) == "0x") begin
               base    = 16;
               raw_arg = raw_arg.substr(2, limit - 1);
               limit   = raw_arg.len();
            end
            else if (raw_arg.substr(0, 0) == "0") begin
               base    = 8;
               raw_arg = raw_arg.substr(1, limit - 1);
               limit   = raw_arg.len();
            end
         end
         else if (limit > 1) begin
            if (raw_arg.substr(0, 0) == "0") begin
               base = 8;
            end
         end
         if (base == 16) begin
            byte cc;

            for (int ii = 0; ii < limit; ii++) begin
               cc = byte'(raw_arg.getc(ii));
               if ((cc >= byte'("a")) && (cc <= byte'("f"))) begin
                  result = (result * 16) + (int'(cc) - (byte'("a") - 10));
               end
               else if ((cc < byte'("0")) || (cc > byte'("9"))) begin
                  result = longint'(-1);
                  break;
               end
               else begin
                  result = (result * 16) + (int'(cc) - byte'("0"));
               end
            end
         end
         else begin
            int cc;

            for (int ii = 0; ii < limit; ii++) begin
               cc = int'(raw_arg.getc(ii)) - byte'("0");
               if ((cc < 0) || (cc >= base)) begin
                  result = longint'(-1);
                  break;
               end
               result = (result * base) + cc;
            end
         end
      end
      return result;
   endfunction : get_arg_value_as_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to get a string of the entire +arg, making it available for debug
   // printing.  The result string is essentially the original +arg will all of its parts.
   //
   // Return Value:
   //
   // string: This is the +arg as capture from the command line.
   //
   function string sprint_obj();
      string message;

      if (m_root_args.size() == 0) begin
         return "";
      end
      message = m_root_args[0];
      for (int ii = 1; ii < m_root_args.size(); ii++) begin
         message = $sformatf("%0s,%0s", message, m_root_args[ii]);
      end
      if (m_arg_value != "") begin
         message = $sformatf("%0s=%0s", message, m_arg_value);
      end
      return message;
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string plus_arg_in = "evl_plus_arg");
      string root_arg;
      int    start;
      int    limit;

      start = 0;
      limit = plus_arg_in.len() - 1;
      for (int ii = 0; ii <= limit; ii++) begin
         if (plus_arg_in.getc(ii) == "=") begin
            root_arg = plus_arg_in.substr(start, ii - 1);
            if (root_arg != "") begin
               m_root_args.push_back(root_arg);
            end
            m_arg_value = plus_arg_in.substr(ii + 1, limit);
            start = limit + 1;
            break;
         end
         else if (plus_arg_in.getc(ii) == ",") begin
            root_arg = plus_arg_in.substr(start, ii - 1);
            if (root_arg != "") begin
               m_root_args.push_back(root_arg);
            end
            start = ii + 1;
         end
      end
      if (start <= limit) begin
         m_root_args.push_back(plus_arg_in.substr(start, limit));
      end
      if (m_root_args.size() == 0) begin
         m_root_args.push_back("");
      end
      for (int ii = m_root_args.size() - 1; ii > 0; ii--) begin
         int value;

         if ($sscanf(m_root_args[ii], "s%d", value) > 0) begin
            m_shire_id = evl_shire_id_t'(value);
         end
         else if ($sscanf(m_root_args[ii], "n%d", value) > 0) begin
            m_neighborhood_id = evl_neighborhood_id_t'(value);
         end
         else if ($sscanf(m_root_args[ii], "c%d", value) > 0) begin
            m_core_id = evl_core_id_t'(value);
         end
      end
   endfunction : new

endclass : evl_plus_arg
