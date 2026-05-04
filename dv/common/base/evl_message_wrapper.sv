//--------------------------------------------------------------------------------------------------
//
// Class: evl_message_wrapper
//
class evl_message_wrapper extends evl_object;

   `uvm_object_utils(evl_message_wrapper)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_message_queue m_message_queues[string];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_message_queue
   //
   function evl_message_queue get_message_queue(input string name_in);
      if (!m_message_queues.exists(name_in)) begin
         m_message_queues[name_in] = new();
      end
      return m_message_queues[name_in];
   endfunction : get_message_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message
   //
   function void send_message(input string name_in, input int source_in, input int type_in, input evl_dword_t value_in);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      message_queue.send_message(source_in, type_in, value_in);
   endfunction : send_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message_packet
   //
   function void send_message_packet(input string name_in, input evl_message_packet packet_in);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      if (packet_in != null) begin
         message_queue.send_message_packet(packet_in);
      end
   endfunction : send_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_message_packet
   //
   function bit have_message_packet(input string name_in, input int subscriber_id);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      return message_queue.have_message_packet(subscriber_id);
   endfunction : have_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_message_packet
   //
   task get_message_packet(output evl_message_packet packet_out, input string name_in, input int subscriber_id);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      message_queue.get_message_packet(packet_out, subscriber_id);
   endtask : get_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_blocking_message_packet
   //
   task get_blocking_message_packet(output evl_message_packet packet_out, input string name_in, input int subscriber_id);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      message_queue.get_blocking_message_packet(packet_out, subscriber_id);
   endtask : get_blocking_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: subscribe
   //
   function int subscribe(input string name_in);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      return message_queue.subscribe();
   endfunction : subscribe


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_subscribers
   //
   function bit have_subscribers(input string name_in);
      evl_message_queue message_queue;

      message_queue = get_message_queue(name_in);
      return message_queue.have_subscribers();
   endfunction : have_subscribers


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_message_wrapper");
      super.new(name_in);
   endfunction : new

endclass : evl_message_wrapper
