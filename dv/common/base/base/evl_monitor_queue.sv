//--------------------------------------------------------------------------------------------------
//
// Class: evl_monitor_queue
//
class evl_monitor_queue extends evl_object;

   `uvm_object_utils(evl_monitor_queue)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_delayed_eval:    Blah.
   //                    ---------------------------------------------------------------------------
   // m_delayed_packets: Blah.
   //                    ---------------------------------------------------------------------------
   // m_main_mb:         Blah.
   //                    ---------------------------------------------------------------------------
   // m_delay_mb:        Blah.
   //                    ---------------------------------------------------------------------------
   // m_delay_queue:     Blah.
   //
   int                           m_delayed_eval    = 0;
   int                           m_delayed_packets = 0;
   mailbox #(evl_monitor_packet) m_main_mb;
   mailbox #(evl_monitor_packet) m_delay_mb;
   evl_monitor_packet            m_delay_queue[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push_packet
   //
   // This function is used to push an incoming RTL packet onto either the main packet mailbox or
   // the delayed packet queue.
   //
   // Input Variables:
   //
   // rtl_packet_in: This is the incoming monitor packet.
   //
   function void push_packet(input evl_monitor_packet rtl_packet_in);
      if (rtl_packet_in != null) begin
         if (rtl_packet_in.get_packet_priority() > 0) begin
            push_delayed_packet(rtl_packet_in);
            return;
         end
      end
      void'(m_main_mb.try_put(rtl_packet_in));
   endfunction : push_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push_delayed_packet
   //
   // This function is used to push a delay monitor packet onto the delayed packet queue.
   //
   // Input Variables:
   //
   // rtl_packet_in: This is the delayed monitor packet to push onto the delayed packet queue.
   //
   function void push_delayed_packet(input evl_monitor_packet rtl_packet_in);
      m_delay_queue.push_back(rtl_packet_in);
      m_delayed_packets++;
   endfunction : push_delayed_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: parse_delayed_packets
   //
   // This function is used to transfer delayed packets to the delayed packet mailbox.
   //
   function void parse_delayed_packets();
      int                cur_priority;
      evl_monitor_packet packet_queue[$];
      evl_monitor_packet new_packet_queue[$];

      while (m_delay_queue.size() > 0) begin
         packet_queue.push_back(m_delay_queue.pop_front());
         m_delayed_packets--;
      end
      while (packet_queue.size() > 0) begin
         cur_priority = 32'h7fffffff;

         foreach (packet_queue[ii]) begin
            if (packet_queue[ii].get_packet_priority() < cur_priority) begin
               cur_priority = packet_queue[ii].get_packet_priority();
            end
         end
         for (int ii = packet_queue.size() - 1; ii >= 0; ii--) begin
            if (packet_queue[ii].get_packet_priority() == cur_priority) begin
               new_packet_queue.push_front(packet_queue[ii]);
               packet_queue.delete(ii);
            end
         end
      end
      foreach (new_packet_queue[ii]) begin
         void'(m_delay_mb.try_put(new_packet_queue[ii]));
      end
   endfunction : parse_delayed_packets


   //-----------------------------------------------------------------------------------------------
   //
   // Task: delayed_eval_task
   //
   // This task is used to transfer delayed packets to the main packet mailbox.
   //
   task delayed_eval_task();
      fork
         forever begin
            #0 wait (m_delayed_packets > 0)
            #0 parse_delayed_packets();
         end
         forever begin
            evl_monitor_packet rtl_packet;

            m_delay_mb.get(rtl_packet);
            m_main_mb.put(rtl_packet);
         end
      join
   endtask : delayed_eval_task


   //-----------------------------------------------------------------------------------------------
   //
   // Task: pop_packet
   //
   // This task is used to get the next available RTL packet from the main packet mailbox.
   //
   // Output Variables:
   //
   // rtl_packet_out: This is the next available RTL packet.
   //
   task pop_packet(output evl_monitor_packet rtl_packet_out);
      evl_monitor_packet rtl_packet;

      rtl_packet = null;
      while (rtl_packet == null) begin
         m_main_mb.get(rtl_packet);
      end
      rtl_packet_out = rtl_packet;
   endtask : pop_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_monitor_queue");
      super.new(name_in);
      m_main_mb  = new();
      m_delay_mb = new();
      fork
         delayed_eval_task();
      join_none
   endfunction : new

endclass : evl_monitor_queue
