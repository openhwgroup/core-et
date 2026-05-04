//--------------------------------------------------------------------------------------------------
//
// Class: evl_soc_rtl_port
//
class evl_soc_rtl_port extends evl_rtl_port;

   int   m_cycle_count_ref  = -1;
   int   m_cycle_count_step = -1;
   event m_clock_event_ref;
   event m_clock_event_step;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ping_clock
   //
   virtual function void ping_clock(input int clock_index_in = 0);
      if (clock_index_in == 0) begin
         super.ping_clock(0);
      end
      else if (clock_index_in == 1) begin
         ->m_clock_event_ref;
         m_cycle_count_ref++;
      end
      else if (clock_index_in == 2) begin
         ->m_clock_event_step;
         m_cycle_count_step++;
      end
      else begin
         super.ping_clock(0);
      end
   endfunction : ping_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle_count
   //
   virtual function int get_cycle_count(input int clock_index_in = 0);
      case (clock_index_in)
         1: return m_cycle_count_ref;
         2: return m_cycle_count_step;
      endcase
      return super.get_cycle_count(0);
   endfunction : get_cycle_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cycles
   //
   virtual task wait_cycles(input int cycle_count_in, input int clock_index_in = 0);
      if (cycle_count_in <= 0) begin
         return;
      end
      if (clock_index_in == 0) begin
         super.wait_cycles(cycle_count_in, 0);
      end
      else if (clock_index_in == 1) begin
         int end_cycle_count = m_cycle_count_ref + cycle_count_in - 1;

         if (cycle_count_in > 1) begin
            #0 wait (m_cycle_count_ref >= end_cycle_count);
         end
         #0 @(m_clock_event_ref);
      end
      else if (clock_index_in == 2) begin
         int end_cycle_count = m_cycle_count_step + cycle_count_in - 1;

         if (cycle_count_in > 1) begin
            #0 wait (m_cycle_count_step >= end_cycle_count);
         end
         #0 @(m_clock_event_step);
      end
      else begin
         super.wait_cycles(cycle_count_in, 0);
      end
   endtask : wait_cycles


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_cycles_with_release
   //
   virtual task wait_cycles_with_release(input int cycle_count_in, input int clock_index_in = 0);
      if ((cycle_count_in <= 0) || (m_delays_are_disabled != 0)) begin
         return;
      end
      if (clock_index_in == 0) begin
         super.wait_cycles_with_release(cycle_count_in, 0);
      end
      else if (clock_index_in == 1) begin
         int end_cycle_count = m_cycle_count_ref + cycle_count_in - 1;

         if (cycle_count_in > 1) begin
            #0 wait ((m_cycle_count_ref >= end_cycle_count) || (m_delays_are_disabled != 0));
         end
         #0 @(m_clock_event_ref);
      end
      else if (clock_index_in == 2) begin
         int end_cycle_count = m_cycle_count_step + cycle_count_in - 1;

         if (cycle_count_in > 1) begin
            #0 wait ((m_cycle_count_step >= end_cycle_count) || (m_delays_are_disabled != 0));
         end
         #0 @(m_clock_event_step);
      end
      else begin
         super.wait_cycles_with_release(cycle_count_in, 0);
      end
   endtask : wait_cycles_with_release


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_clock
   //
   virtual task wait_for_clock(input int clock_index_in = 0);
      if (clock_index_in == 0) begin
         super.wait_for_clock(0);
      end
      else if (clock_index_in == 1) begin
         @(m_clock_event_ref);
      end
      else if (clock_index_in == 2) begin
         @(m_clock_event_step);
      end
      else begin
         super.wait_for_clock(0);
      end
   endtask : wait_for_clock


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_interrupt_packet
   //
   task get_interrupt_packet(output evl_message_packet packet_out);
      m_global_attrs.get_blocking_message_packet(packet_out, "interrupt_mailbox", 0);
   endtask


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string full_name_in     = "evl_soc_rtl_port",
                input int    skip_levels_in[] = {});
      super.new(full_name_in, SOC_RTL_AGENT, ~evl_agent_id_t'(0), "evl_soc_env", "", "", "", skip_levels_in);
      m_global_attrs.subscribe("interrupt_mailbox");
   endfunction : new

endclass : evl_soc_rtl_port
