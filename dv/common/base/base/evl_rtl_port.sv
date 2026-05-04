//--------------------------------------------------------------------------------------------------
//
// Class: evl_rtl_port
//
class evl_rtl_port extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   `evl_param_key(pk_ignore_clock_err)

   static int    m_heartbeat_interval = 10000;
   static string m_months[16] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "???", "???", "???", "???" };


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   string            m_rtl_name;
   string            m_full_name;
   evl_shire_id_t    m_shire_id            = ~evl_shire_id_t'(0);
   evl_agent_id_t    m_agent_id            = ~evl_agent_id_t'(0);
   int               m_port_id             = -1;
   int               m_max_port_id         = -1;
   int               m_port_type           = 0;
   evl_delay_wrapper m_delay_wrapper       = null;
   int               m_cycle_count         = -1;
   int               m_delays_are_disabled = 0;
   bit               m_send_final_state    = 1'b0;
   bit               m_final_state_sent    = 1'b0;
   bit               m_waiting             = 1'b0;
   int               m_delay_change        = 0;
   int               m_bus_data_width      = `EVL_LINE_BYTES;
   bit               m_power_on            = 1'b1;
   bit               m_reset_asserted[];
   bit               m_reset_deasserted[];
   event             m_reset_change;
   event             m_clock_event;
   event             m_int_clock_event;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rtl_name
   //
   function void set_rtl_name(input string rtl_name_in);
      m_rtl_name = $sformatf("%s", rtl_name_in);
   endfunction : set_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rtl_name
   //
   function string get_rtl_name();
      return m_rtl_name;
   endfunction : get_rtl_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   function void set_shire_id(input evl_shire_id_t shire_id_in);
      m_shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   function evl_shire_id_t get_shire_id();
      return m_global_attrs.get_current_shire_id(m_shire_id);
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_clock
   //
   virtual function void ping_clock(input int clock_index_in = 0);
      ->m_clock_event;
      m_cycle_count++;
   endfunction : ping_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_int_clock
   //
   virtual function void ping_int_clock(input int clock_index_in = 0);
      ->m_int_clock_event;
      m_cycle_count++;
   endfunction : ping_int_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_on
   //
   virtual function void power_on();
      if (m_power_on == 1'b0) begin
         `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power on (%s)", m_rtl_name))
      end
      m_power_on = 1'b1;
   endfunction : power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_off
   //
   virtual function void power_off();
      if (m_power_on == 1'b1) begin
         `evl_log(UVM_LOW, VMOD_GLOBAL, get_abstract_name(), $sformatf("turning power off (%s)", m_rtl_name))
      end
      m_power_on = 1'b0;
   endfunction : power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_on
   //
   function bit power_is_on();
      return m_power_on;
   endfunction : power_is_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: power_is_off
   //
   function bit power_is_off();
      return ~m_power_on;
   endfunction : power_is_off


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_on
   //
   task wait_for_power_on();
      #0 wait (m_power_on == 1'b1);
   endtask : wait_for_power_on


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_power_off
   //
   task wait_for_power_off();
      #0 wait (m_power_on == 1'b0);
   endtask : wait_for_power_off


   //-----------------------------------------------------------------------------------------------
   //
   // Function: assert_reset
   //
   function void assert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b1;
         m_reset_deasserted[reset_index_in] = 1'b0;
         -> m_reset_change;
      end
   endfunction : assert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: deassert_reset
   //
   function void deassert_reset(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         m_reset_asserted[reset_index_in]   = 1'b0;
         m_reset_deasserted[reset_index_in] = 1'b1;
         -> m_reset_change;
      end
   endfunction : deassert_reset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_deasserted
   //
   function bit reset_is_deasserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b0) && (m_reset_deasserted[reset_index_in] == 1'b1)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_deasserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_is_asserted
   //
   function bit reset_is_asserted(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         return (((m_reset_asserted[reset_index_in] == 1'b1) && (m_reset_deasserted[reset_index_in] == 1'b0)) ? 1'b1 : 1'b0);
      end
      return 1'b0;
   endfunction : reset_is_asserted


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_deassertion
   //
   task wait_for_reset_deassertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_deasserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_deassertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: wait_for_reset_assertion
   //
   task wait_for_reset_assertion(input int unsigned reset_index_in = 0);
      if (reset_index_in < m_reset_asserted.size()) begin
         #0;
         while (reset_is_asserted(reset_index_in) == 1'b0) begin
            @(m_reset_change);
         end
         #0;
      end
      return;
   endtask : wait_for_reset_assertion


   //-----------------------------------------------------------------------------------------------
   //
   // Function: disable_delays
   //
   function void disable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled |= disable_type;
   endfunction : disable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: enable_delays
   //
   function void enable_delays(input int disable_type = `EVL_MASTER_DISABLE);
      m_delays_are_disabled &= ~disable_type;
   endfunction : enable_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_enabled
   //
   function bit delays_are_enabled();
      return ((m_delays_are_disabled == 0) ? 1'b1 : 1'b0);
   endfunction : delays_are_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delays_are_disabled
   //
   function bit delays_are_disabled();
      return ((m_delays_are_disabled == 0) ? 1'b0 : 1'b1);
   endfunction : delays_are_disabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_enable
   //
   task wait_for_delay_enable();
      if (m_delays_are_disabled != 0) begin
         #0 wait (m_delays_are_disabled == 0);
      end
   endtask : wait_for_delay_enable


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay_disable
   //
   task wait_for_delay_disable();
      if (m_delays_are_disabled == 0) begin
         #0 wait (m_delays_are_disabled != 0);
      end
   endtask : wait_for_delay_disable


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle_count
   //
   virtual function int get_cycle_count(input int clock_index_in = 0);
      return m_cycle_count;
   endfunction : get_cycle_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cycles
   //
   virtual task wait_cycles(input int cycle_count_in, input int clock_index_in = 0);
      if (cycle_count_in > 0) begin
         int end_cycle_count = m_cycle_count + cycle_count_in - 1;

         m_waiting = 1'b1;
         if (cycle_count_in > 1) begin
            #0 wait (m_cycle_count >= end_cycle_count);
         end
         #0 @(m_clock_event or m_int_clock_event);
      end
   endtask : wait_cycles


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cycles_with_release
   //
   virtual task wait_cycles_with_release(input int cycle_count_in, input int clock_index_in = 0);
      if ((cycle_count_in > 0) && (m_delays_are_disabled == 0)) begin
         int end_cycle_count = m_cycle_count + cycle_count_in - 1;

         m_waiting = 1'b1;
         if (cycle_count_in > 1) begin
            #0 wait ((m_cycle_count >= end_cycle_count) || (m_delays_are_disabled != 0));
         end
         #0 @(m_clock_event or m_int_clock_event);
      end
   endtask : wait_cycles_with_release


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_clock
   //
   virtual task wait_for_clock(input int clock_index_in = 0);
      m_waiting = 1'b1;
      @(m_clock_event or m_int_clock_event);
   endtask : wait_for_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_delay_wrapper
   //
   virtual function void set_delay_wrapper(input evl_delay_wrapper delay_wrapper_in);
      m_delay_wrapper = delay_wrapper_in;
      m_delay_change++;
   endfunction : set_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay_wrapper
   //
   virtual function evl_delay_wrapper get_delay_wrapper();
      return m_delay_wrapper;
   endfunction : get_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_delay
   //
   virtual function void reset_delay(input int delay_key);
      if (m_delay_wrapper == null) begin
         return;
      end
      m_delay_wrapper.reset_delay(delay_key);
   endfunction : reset_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay
   //
   virtual function void add_delay(input int delay_key, input evl_delay_t delay_in, input bit force_on_delay_list = 1'b0);
      if (m_delay_wrapper == null) begin
         m_delay_wrapper = new($sformatf("%0s(DLY)", get_abstract_name()));
         m_delay_change++;
      end
      m_delay_wrapper.add_delay(delay_key, delay_in, force_on_delay_list);
   endfunction : add_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_delay
   //
   virtual function bit have_delay(input int delay_key, input int index = 0);
      if (m_delay_wrapper == null) begin
         return 1'b0;
      end
      return m_delay_wrapper.have_delay(delay_key, index);
   endfunction : have_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay
   //
   virtual function evl_delay_t get_delay(input int delay_key, input int index = 0, input evl_delay_t default_delay = evl_delay_t'(0));
      evl_delay_t delay;

      if ((delays_are_enabled() == 1'b0) || (m_delay_wrapper == null)) begin
         return default_delay;
      end
      if (m_delay_wrapper.get_delay(delay, delay_key, index) == 1'b0) begin
         return default_delay;
      end
      return delay;
   endfunction : get_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_delay
   //
   virtual function evl_delay_t get_next_delay(input int delay_key, input evl_delay_t default_delay = evl_delay_t'(0));
      evl_delay_t delay;

      if ((delays_are_enabled() == 1'b0) || (m_delay_wrapper == null)) begin
         return default_delay;
      end
      void'(m_delay_wrapper.get_next_delay(delay, delay_key, default_delay));
      return delay;
   endfunction : get_next_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_delay
   //
   virtual task wait_for_delay(input int delay_key);
      if (m_delay_wrapper == null) begin
         int delay_change;

         forever begin
            #0 delay_change = m_delay_change;
            if (m_delay_wrapper != null) begin
               break;
            end
            #0 wait (delay_change != m_delay_change);
         end
      end
      m_delay_wrapper.wait_for_delay(delay_key);
      wait_for_delay_enable();
   endtask : wait_for_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_to_assert_backp
   //
   virtual task wait_to_assert_backp(input int backpressure_key);
      forever begin
         if ((delays_are_enabled() == 1'b0) || (have_delay(backpressure_key) == 1'b0)) begin
            wait_for_delay(backpressure_key);
         end
         wait_cycles_with_release(get_next_delay(backpressure_key, 1));
         if (delays_are_enabled() == 1'b1) begin
            return;
         end
      end
   endtask : wait_to_assert_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_to_deassert_backp
   //
   virtual task wait_to_deassert_backp(input int backpressure_key);
      if ((delays_are_enabled() == 1'b1) && (have_delay(backpressure_key) == 1'b1)) begin
         wait_cycles_with_release(get_next_delay(backpressure_key, 0));
      end
   endtask : wait_to_deassert_backp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_name
   //
   function string get_full_name();
      return m_full_name;
   endfunction : get_full_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_final_state
   //
   virtual function void send_final_state();
      m_send_final_state = 1'b1;
   endfunction : send_final_state


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_to_send_final_state
   //
   virtual task wait_to_send_final_state();
      #0 wait (m_send_final_state == 1'b1);
   endtask : wait_to_send_final_state


   //-----------------------------------------------------------------------------------------------
   //
   // Function: final_state_sent
   //
   virtual function void final_state_sent();
      m_final_state_sent = 1'b1;
   endfunction : final_state_sent


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_final_state_sent
   //
   virtual task wait_for_final_state_sent();
      #0 wait (m_final_state_sent == 1'b1);
   endtask : wait_for_final_state_sent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_data_width
   //
   virtual function int get_bus_data_width();
      return m_bus_data_width;
   endfunction : get_bus_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_data_width
   //
   virtual function void set_bus_data_width(input int bus_data_width);
      m_bus_data_width = bus_data_width;
   endfunction : set_bus_data_width


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_port_id
   //
   virtual function int get_port_id();
      return m_port_id;
   endfunction : get_port_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_max_port_id
   //
   virtual function int get_max_port_id();
      return m_max_port_id;
   endfunction : get_max_port_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_port_type
   //
   virtual function int set_port_type(input int port_type_in);
      m_port_type = port_type_in;
   endfunction : set_port_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_port_type
   //
   virtual function int get_port_type();
      return m_port_type;
   endfunction : get_port_type


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_event
   //
   virtual task get_event(output evl_monitor_packet packet);
      wait (0);
   endtask : get_event


   //-----------------------------------------------------------------------------------------------
   //
   // Task: heartbeat_timer
   //
   virtual task heartbeat_timer(input string abstract_name);
      int     count;
      longint cur_time;

      #0;
      `ifdef evl_log
         cur_time = `evl_tcl_get_cur_time;
         if (cur_time != 0) begin
            #0 `evl_log(UVM_LOW, VMOD_TIMER, abstract_name, $sformatf("clock cycle counter: %0d (%s %02d %02d:%02d:%02d)", 0, m_months[((cur_time >> 32) & 'h0f)], ((cur_time >> 24) & 'h1f), ((cur_time >> 16) & 'h1f), ((cur_time >> 8) & 'h3f), (cur_time & 'h3f)))
         end
      `endif
      #1 m_cycle_count = 0;
      count = 1;
      forever begin
         wait_cycles(m_heartbeat_interval);
         `ifdef evl_log
            if (count == 10) begin
               cur_time = `evl_tcl_get_cur_time;
               if (cur_time == 0) begin
                  #0 `evl_log(UVM_LOW, VMOD_TIMER, abstract_name, $sformatf("clock cycle counter: %0d", m_cycle_count))
               end
               else begin
                  #0 `evl_log(UVM_LOW, VMOD_TIMER, abstract_name, $sformatf("clock cycle counter: %0d (%s %02d %02d:%02d:%02d)", m_cycle_count, m_months[((cur_time >> 32) & 'h0f)], ((cur_time >> 24) & 'h1f), ((cur_time >> 16) & 'h1f), ((cur_time >> 8) & 'h3f), (cur_time & 'h3f)))
               end
               count = 1;
            end
            else begin
               #0 `evl_log(UVM_LOW, VMOD_TIMER, abstract_name, $sformatf("clock cycle counter: %0d", m_cycle_count))
               count++;
            end
         `else
            $display("%t (%m) clock_cycle_counter @ %0d cycles", $time, m_cycle_count);
         `endif
      end
   endtask : heartbeat_timer


   //-----------------------------------------------------------------------------------------------
   //
   // Task: watch_for_clock_problem
   //
   task watch_for_clock_problem();
      wait (m_waiting == 1'b1);
      fork
         begin
            #0 @(m_clock_event);
            #0 @(m_clock_event);
         end
         begin
            #10000ns;
            `ifdef UPF
               //
               // When running in UPF mode, it is possible that this particular instance will never
               // get powered on.  If that is the case, issue a warning and generate fake clocks
               // just to make sure things proceed.
               //
               if (get_int_param(pk_ignore_clock_err, 0) == 0) begin
                  `dut_warning(get_abstract_name(), $sformatf("timed out wating for clocks (%0s)", get_full_name()))
                  forever begin
                     #10ns ping_int_clock();
                  end
               end
            `else
               //
               // When running in normal mode, it is a potential problem if clocks do not start.  If
               // that is the case, issue a warning and generate fake clocks just to make sure
               // things proceed.
               //
               if (get_int_param(pk_ignore_clock_err, 0) == 0) begin
                  `dut_error(get_abstract_name(), $sformatf("timed out wating for clocks (%0s)", get_full_name()))
                  forever begin
                     #10ns ping_int_clock();
                  end
               end
            `endif
         end
      join_any
      disable fork;
   endtask : watch_for_clock_problem


   //-----------------------------------------------------------------------------------------------
   //
   // Function: coverage_enabled
   //
   function bit coverage_enabled();
      return m_global_attrs.coverage_enabled();
   endfunction : coverage_enabled


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_coverage_enabled
   //
   task wait_for_coverage_enabled();
      m_global_attrs.wait_for_coverage_enabled();
   endtask : wait_for_coverage_enabled


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
               end
            end
         end
      end
   endfunction : parse_param_string


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_zero_time
   //
   // This task is used to force zero-time waits, but it requires support in the RTL interface
   // module.  Within the interface module, an extended version of evl_rtl_port must be created and
   // the module must have a loop that performs non-blocking assigns across 2 variables
   // (non-blocking assigns occur at the end of a simulation tick according to the Verilog spec)
   // repeatedly waits for mismatches and then matches for the specified count.
   //
   virtual task wait_zero_time(input int unsigned count);
      int value = 0;

      if (count == 0) begin
         #0 value = value + 1;
      end
      else if (count > 10) begin
         repeat (10) #0 value = value + 1;
      end
      else begin
         repeat (count) #0 value = value + 1;
      end
      return;
   endtask : wait_zero_time


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set
   //
   virtual function int get_cache_set(input evl_paddr_t caddr,
                                      input int cway  = 0,
                                      input int ctype = 0);
      return -1;
   endfunction : get_cache_set


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_bank
   //
   virtual function int get_cache_bank(input evl_paddr_t caddr,
                                       input int cway  = 0,
                                       input int ctype = 0);
      return -1;
   endfunction : get_cache_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_subbank
   //
   virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                          input int cway  = 0,
                                          input int ctype = 0);
      return -1;
   endfunction : get_cache_subbank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_tag
   //
   virtual function evl_paddr_t get_cache_tag(input int cindex,
                                              input int cway  = 0,
                                              input int ctype = 0);
      return ~evl_paddr_t'(0);
   endfunction : get_cache_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_data
   //
   virtual function int get_cache_data(output evl_dword_t cdata[],
                                       input  int         cindex,
                                       input  int         cway  = 0,
                                       input  int         ctype = 0);
      cdata = new[`EVL_LINE_DWORDS];
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         cdata[ii] = ~evl_dword_t'(0);
      end
      return -1;
   endfunction : get_cache_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_bit_err
   //
   virtual function int set_cache_bit_err(input int cindex,
                                          input int cbit0,
                                          input int cbit1 = -1,
                                          input int cway  = 0,
                                          input int ctype = 0,
                                          input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
      return -1;
   endfunction : set_cache_bit_err


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string           full_name_in     = "evl_rtl_port",
                input evl_agent_type_t agent_type_in    = NO_AGENT,
                input evl_agent_id_t   agent_pid_in     = ~evl_agent_id_t'(0),
                input string           sv_comp_in       = "",
                input string           param_list_in    = "",
                input string           name_prefix_in   = "",
                input string           name_postfix_in  = "",
                input int              skip_levels_in[] = {},
                input int              port_id_in       = 0,
                input int              max_port_id_in   = 0,
                input int              additional_hier  = 0);
      void'(evl_base_pkg::evl_log_create());
      m_rtl_name  = $sformatf("%s", full_name_in);
      m_full_name = evl_base_pkg::parse_hier_levels(full_name_in, name_prefix_in, name_postfix_in, skip_levels_in, 1'b0, additional_hier);
      uvm_config_db#(evl_rtl_port)::set(null, m_full_name, "rtl_port", this);
      m_reset_asserted      = new[1];
      m_reset_deasserted    = new[1];
      m_reset_asserted[0]   = 1'b0;
      m_reset_deasserted[0] = 1'b0;
      m_port_id             = port_id_in;
      m_agent_id            = agent_pid_in;
      m_max_port_id         = max_port_id_in;
      if (param_list_in != "") begin
         parse_param_string(param_list_in);
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, "RTLPort", $sformatf("created RTL port (%0s, id %0d)", m_full_name, get_inst_id()))
      fork
         watch_for_clock_problem();
      join_none
   endfunction : new

endclass : evl_rtl_port
