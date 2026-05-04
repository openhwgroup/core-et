//--------------------------------------------------------------------------------------------------
//
// Class: evl_message_queue
//
class evl_message_queue extends evl_object;

   `uvm_object_utils(evl_message_queue)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int                           m_subscribed = 0;
   mailbox #(evl_message_packet) m_message_mbs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_new_message_mb
   //
   function int create_new_message_mb();
      mailbox #(evl_message_packet) message_mb;

      message_mb = new();
      m_message_mbs.push_back(message_mb);
      return (m_message_mbs.size() - 1);
   endfunction : create_new_message_mb


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message
   //
   function void send_message(input int source_in, input int type_in, input evl_dword_t value_in);
      evl_message_packet packet;

      packet = new("evl_message_packet", source_in, type_in, value_in);
      send_message_packet(packet);
   endfunction : send_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message_packet
   //
   function void send_message_packet(input evl_message_packet packet_in);
      if (packet_in != null) begin
         if (m_message_mbs.size() == 0) begin
            void'(create_new_message_mb());
         end
         foreach (m_message_mbs[ii]) begin
            void'(m_message_mbs[ii].try_put(packet_in));
         end
      end
   endfunction : send_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_message_packet
   //
   function bit have_message_packet(input int subscriber_id);
      if (subscriber_id >= m_message_mbs.size()) begin
         return 1'b0;
      end
      return ((m_message_mbs[subscriber_id].num() > 0) ? 1'b1 : 1'b0);
   endfunction : have_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_message_packet
   //
   task get_message_packet(output evl_message_packet packet_out, input int subscriber_id);
      if (subscriber_id >= m_message_mbs.size()) begin
         packet_out = null;
      end
      else if (m_message_mbs[subscriber_id].try_get(packet_out) == 0) begin
         packet_out = null;
      end
   endtask : get_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_blocking_message_packet
   //
   task get_blocking_message_packet(output evl_message_packet packet_out, input int subscriber_id);
      if (subscriber_id >= m_message_mbs.size()) begin
         packet_out = null;
      end
      else begin
         m_message_mbs[subscriber_id].get(packet_out);
      end
   endtask : get_blocking_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: subscribe
   //
   function int subscribe();
      if (m_subscribed++ == 0) begin
         if (m_message_mbs.size() == 0) begin
            return create_new_message_mb();
         end
         return 0;
      end
      return create_new_message_mb();
   endfunction : subscribe


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_subscribers
   //
   function bit have_subscribers();
      return ((m_subscribed > 0) ? 1'b1 : 1'b0);
   endfunction : have_subscribers


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_message_queue");
      super.new(name_in);
   endfunction : new

endclass : evl_message_queue
