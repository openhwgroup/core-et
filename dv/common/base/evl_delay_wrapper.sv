//--------------------------------------------------------------------------------------------------
//
// Class: evl_delay_wrapper
//
class evl_delay_wrapper extends uvm_void;

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
   // Local Variables
   //
   // m_delay_key:    This is the delay key for this wrapper.  Normally, a wrapper can contain
   //                 multiple delay keys, but if this wrapper was created for a specific address
   //                 range, then all of the delays will be for this specific key.
   //                 ------------------------------------------------------------------------------
   // m_delay_change:
   //
   int            m_delay_key    = 0;
   evl_paddr_t    m_start_addr   = ~evl_paddr_t'(0);
   evl_paddr_t    m_end_addr     = evl_paddr_t'(0);
   int            m_delay_change = 0;
   evl_delay_t    m_delays[int];
   evl_delay_list m_delay_lists[int];


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
      m_delays.delete();
      m_delay_lists.delete();
   endfunction : reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_delay
   //
   function void reset_delay(input int delay_key);
      m_delays.delete(delay_key);
      m_delay_lists.delete(delay_key);
   endfunction : reset_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay_list
   //
   function void add_delay_list(input int delay_key, input evl_delay_list delay_list_in);
      m_delay_lists[delay_key] = delay_list_in;
      m_delay_change++;
   endfunction : add_delay_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay
   //
   function void add_delay(input int delay_key, input evl_delay_t delay_in, input bit force_on_delay_list = 1'b0);
      if (!(m_delay_lists.exists(delay_key))) begin
         if (force_on_delay_list == 1'b0) begin
            m_delays[delay_key] = delay_in;
            m_delay_change++;
            return;
         end
         m_delay_lists[delay_key] = new(evl_param_xref::get_param_name(delay_key));
         if (m_delays.exists(delay_key)) begin
            m_delay_lists[delay_key].add_delay(m_delays[delay_key]);
            m_delays.delete(delay_key);
         end
      end
      m_delay_lists[delay_key].add_delay(delay_in);
      m_delay_change++;
   endfunction : add_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: absorb_delay_wrapper
   //
   virtual function void absorb_delay_wrapper(input evl_delay_wrapper delay_wrapper);
      delay_wrapper.add_my_delays_to_wrapper(this);
   endfunction : absorb_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_my_delays_to_wrapper
   //
   virtual function void add_my_delays_to_wrapper(input evl_delay_wrapper delay_wrapper);
      foreach (m_delays[ii]) begin
         delay_wrapper.add_delay(ii, m_delays[ii]);
      end
      foreach (m_delay_lists[ii]) begin
         int         limit;
         evl_delay_t delays[];

         limit = m_delay_lists[ii].get_delay_list(delays);
         if (limit > 0) begin
            foreach (delays[jj]) begin
               delay_wrapper.add_delay(ii, delays[jj], 1'b1);
            end
         end
      end
   endfunction : add_my_delays_to_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_delay
   //
   function bit have_delay(input int delay_key, input int index = 0);
      if (m_delay_lists.exists(delay_key)) begin
         return m_delay_lists[delay_key].have_delay(index);
      end
      if (m_delays.exists(delay_key)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : have_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay
   //
   function bit get_delay(output evl_delay_t delay_out, input int delay_key, input int index = 0);
      if (m_delay_lists.exists(delay_key)) begin
         return m_delay_lists[delay_key].get_delay(delay_out, index);
      end
      if (m_delays.exists(delay_key)) begin
         delay_out = m_delays[delay_key];
         return 1'b1;
      end
      delay_out = evl_delay_t'(0);
      return 1'b0;
   endfunction : get_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_delay
   //
   function bit get_next_delay(output evl_delay_t delay_out, input int delay_key, input evl_delay_t default_delay = evl_delay_t'(0));
      if (m_delay_lists.exists(delay_key)) begin
         return m_delay_lists[delay_key].get_next_delay(delay_out, default_delay);
      end
      if (m_delays.exists(delay_key)) begin
         delay_out = m_delays[delay_key];
         return 1'b1;
      end
      delay_out = default_delay;
      return 1'b0;
   endfunction : get_next_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay
   //
   task wait_for_delay(input int delay_key);
      int delay_change;

      if (have_delay(delay_key) == 1'b1) begin
         return;
      end
      forever begin
         #0 delay_change = m_delay_change;
         if (have_delay(delay_key) == 1'b1) begin
            return;
         end
         #0 wait (delay_change != m_delay_change);
      end
   endtask : wait_for_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_range
   //
   function void set_addr_range(input int delay_key, input evl_paddr_t start_addr, input evl_paddr_t end_addr);
      m_delay_key = delay_key;
      if (start_addr <= end_addr) begin
         m_start_addr =  const_line_paddr_mask & start_addr;
         m_end_addr   = ~const_line_paddr_mask | end_addr;
      end
      else begin
         m_start_addr =  const_line_paddr_mask & end_addr;
         m_end_addr   = ~const_line_paddr_mask | start_addr;
      end
      reset();
   endfunction : set_addr_range


   //-----------------------------------------------------------------------------------------------
   //
   // Function: range_and_key_match
   //
   function bit range_and_key_match(input  int         delay_key,
                                    input  evl_paddr_t start_addr,
                                    input  evl_paddr_t end_addr);
      if ((m_delay_key == 0) || (m_delay_key != delay_key) || (m_start_addr > m_end_addr)) begin
         return 1'b0;
      end
      if (start_addr <= end_addr) begin
         if ((m_start_addr == (const_line_paddr_mask & start_addr)) && (m_end_addr == (~const_line_paddr_mask | end_addr))) begin
            return 1'b1;
         end
      end
      else begin
         if ((m_start_addr == (const_line_paddr_mask & end_addr)) && (m_end_addr == (~const_line_paddr_mask | start_addr))) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : range_and_key_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_addr_and_key
   //
   function bit has_addr_and_key(output evl_delay_t delay_out,
                                 input  int         delay_key,
                                 input  evl_paddr_t paddr,
                                 input  evl_delay_t default_delay = evl_delay_t'(0));
      if ((m_delay_key == 0) || (m_start_addr > m_end_addr)) begin
         delay_out = 0;
         return 1'b0;
      end
      if ((m_delay_key == delay_key) && (paddr >= m_start_addr) && (paddr <= m_end_addr)) begin
         return get_next_delay(delay_out, delay_key, default_delay);
      end
      delay_out = 0;
      return 1'b0;
   endfunction : has_addr_and_key


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clone
   //
   virtual function evl_delay_wrapper clone();
      evl_delay_wrapper delay_wrapper;

      delay_wrapper = new(get_abstract_name());
      delay_wrapper.m_delay_key  = m_delay_key;
      delay_wrapper.m_start_addr = m_start_addr;
      delay_wrapper.m_end_addr   = m_end_addr;
      foreach (m_delays[ii]) begin
         delay_wrapper.add_delay(ii, m_delays[ii]);
      end
      foreach (m_delay_lists[ii]) begin
         evl_delay_t delays[];

         if (m_delay_lists[ii].get_delay_list(delays) > 0) begin
            foreach (delays[jj]) begin
               delay_wrapper.add_delay(ii, delays[jj], 1'b1);
            end
         end
      end
      return delay_wrapper;
   endfunction : clone


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   virtual function string sprint_obj(input string space_in = "");
      string message = "";
      string space   = space_in;

      foreach (m_delays[ii]) begin
         message = $sformatf("%0s%0s%0s=%0d", message, space, evl_param_xref::get_param_name(ii), m_delays[ii]);
         space   = " ";
      end
      foreach (m_delay_lists[ii]) begin
         message = $sformatf("%0s%0s %0s", message, space, m_delay_lists[ii].sprint_obj());
         space   = " ";
      end
      return message;
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   virtual function void print_obj(input string name_in = "");
      string name = (name_in != "") ? name_in : get_abstract_name();

      if ((m_delay_key != 0) && (m_start_addr <= m_end_addr)) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, name, $sformatf("Delays for 0x%x to 0x%x:", m_start_addr, m_end_addr))
      end
      foreach (m_delays[ii]) begin
         `evl_log(UVM_NONE, VMOD_GLOBAL, name, $sformatf("m_delays[%0s]: %0d", evl_param_xref::get_param_name(ii), m_delays[ii]))
      end
      foreach (m_delay_lists[ii]) begin
         m_delay_lists[ii].print_obj(name);
      end
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "delay_wrapper");
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_delay_wrapper
