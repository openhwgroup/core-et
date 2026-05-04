//--------------------------------------------------------------------------------------------------
//
// Class: evl_delay_list
//
// This class is used to contain a list of delays of a specific type.  It is possible to create a
// random list of delays, or to have a fixed list of delays.
//
// The function "get_next_delay()" should be used to get delays from the list *unless* you know
// which specific delay to get.  As an example, assume a transaction from a stimulus agent is
// broadcast to multiple coherent agents (such as Maxion stimulus agents).  It may be that you want
// different intervention response delays for each of the coherent agents.  Because each Maxion
// stimulus agent would know what instance it is, the 0th Maxion stimulus agent would get its delay
// with a "get_delay(delay, 0)" call, the 1st Maxion stimulus agent would get its delay with a
// "get_delay(delay, 1)" call, etc.
//
// For a random delay list, the "get_next_delay()" function would be used to get the next available
// delay value.  The random list behaves as a wrap-around list ... once we reach the end of the
// list, the next available delay is the 0th entry.
//
// Because delays are "typed" in a manner similar to integer and object parameters, the usage model
// is that each evl_delay_list instance is for one type of delay.  The following sample code
// illustrates how one might generate 2 lists of random delays, one list for "pk_req_delay" and one
// list for "pk_rsp_delay":
//
//    evl_delay_list m_delays[int];
//
//    class my_delays extends evl_delay_list;
//       constraint c_num_delays { r_delays.size() inside { [100:200] }; }
//
//       function new(input string name_in = "my_delays");
//          super.new(name_in);
//       endfunction : new
//    endclass : my_delays
//
//    function void gen_delays();
//       int       delay_type;
//       my_delays delay_list;
//
//       delay_type = evl_param_xref::get_param_key("pk_req_delay");
//       delay_list = new("pk_req_delay");
//       delay_list.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { 1, 2, 8, 16 }); };
//       m_delays[delay_type] = delay_list;
//
//       delay_type = evl_param_xref::get_param_key("pk_rsp_delay");
//       delay_list = new("pk_rsp_delay");
//       delay_list.randomize() with { foreach (r_delays[ii]) (r_delays[ii] inside { 2, 4, 8 }); };
//       m_delays[delay_type] = delay_list;
//    endfunction : gen_delays
//
class evl_delay_list extends uvm_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_global_attrs m_global_attrs = evl_global_attrs::get_inst();


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   string m_abstract_name;


   //-----------------------------------------------------------------------------------------------
   //
   // Randomizing Variables
   //
   rand evl_delay_t r_delays[$];
   rand int         r_cur_index = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Default Constraints
   //
   constraint c_delay { foreach (r_delays[ii]) (r_delays[ii] >= 0); }


   //-----------------------------------------------------------------------------------------------
   //
   // Unmodifiable Constraints
   //
   constraint c_num_delays_local  { ((r_delays.size() > 0) && (r_delays.size() < 1000)); }
   constraint c_cur_index_local   { (r_cur_index >= 0) && (r_cur_index <= r_delays.size()); }
   constraint c_eval_order1_local { solve r_delays.size() before r_delays;    }
   constraint c_eval_order2_local { solve r_delays.size() before r_cur_index; }
   constraint c_eval_order3_local { solve r_delays        before r_cur_index; }


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   virtual function void set_abstract_name(input string name_in);
      m_abstract_name = name_in;
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_abstract_name
   //
   virtual function string get_abstract_name();
      return m_abstract_name;
   endfunction : get_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset
   //
   function void reset();
      r_delays = {};
   endfunction : reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay
   //
   function void add_delay(input evl_delay_t delay_in);
      r_delays.push_back(delay_in);
   endfunction : add_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_delay
   //
   function bit have_delay(input int index = 0);
      if ((index < 0) || (index >= r_delays.size())) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : have_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay
   //
   function bit get_delay(output evl_delay_t delay_out, input int index = 0);
      if ((index < 0) || (index >= r_delays.size())) begin
         delay_out = evl_delay_t'(0);
         return 1'b0;
      end
      delay_out = r_delays[index];
      return 1'b1;
   endfunction : get_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_delay
   //
   function bit get_next_delay(output evl_delay_t delay_out, input evl_delay_t default_delay = evl_delay_t'(0));
      if (r_delays.size() == 0) begin
         delay_out = default_delay;
         return 1'b0;
      end
      if (r_cur_index >= r_delays.size()) begin
         r_cur_index = 0;
      end
      delay_out = r_delays[r_cur_index++];
      return 1'b1;
   endfunction : get_next_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay_list
   //
   function int get_delay_list(output evl_delay_t delays_out[]);
      if (r_delays.size() == 0) begin
         delays_out = new[0];
         return 0;
      end
      delays_out = new[r_delays.size()](r_delays);
      return r_delays.size();
   endfunction : get_delay_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_delay_list
   //
   function void set_delay_list(input evl_delay_t delays_in[]);
      r_delays = {};
      foreach (delays_in[ii]) begin
         r_delays.push_back(delays_in[ii]);
      end
   endfunction : set_delay_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: concat_delay_list
   //
   function void concat_delay_list(input evl_delay_t delays_in[]);
      foreach (delays_in[ii]) begin
         r_delays.push_back(delays_in[ii]);
      end
   endfunction : concat_delay_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   virtual function void print_obj(input string name_in = "");
      if (r_delays.size() > 0) begin
         string message_info = $sformatf("m_delays[%0s]:", get_abstract_name());
         string space        = " ";

         foreach (r_delays[ii]) begin
            if (message_info.len() > 100) begin
               `evl_log(UVM_NONE, VMOD_GLOBAL, name_in, $sformatf("%0s,", message_info))
               message_info = $sformatf("%0d", r_delays[ii]);
            end
            else begin
               message_info = $sformatf("%0s%0s%0d", message_info, space, r_delays[ii]);
               space        = ", ";
            end
         end
         if (message_info != "") begin
            `evl_log(UVM_NONE, VMOD_GLOBAL, name_in, message_info)
         end
         `evl_log_no_time(UVM_NONE, VMOD_GLOBAL, "")
      end
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj(input string space_in = "");
      string message = "";
      string space   = space_in;

      foreach (r_delays[ii]) begin
         message = $sformatf("%0s%0s%0s[%0d]=%0d", message, space, get_abstract_name(), ii, r_delays[ii]);
         space   = " ";
      end
      return message;
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "delay_list");
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_delay_list
