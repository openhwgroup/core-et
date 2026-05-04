//-----------------------------------------------------------------------------------------------------
//
// Class: evl_etlink_bus_req_queue
//
class evl_etlink_bus_req_queue;

   evl_etlink_bus_req m_bus_req_queue[$];


   //--------------------------------------------------------------------------------------------------
   function int queue_size();
      return m_bus_req_queue.size();
   endfunction : queue_size


   //--------------------------------------------------------------------------------------------------
   function void push_back(input evl_etlink_bus_req bus_req_in);
      m_bus_req_queue.push_back(bus_req_in);
   endfunction : push_back


   //--------------------------------------------------------------------------------------------------
   function evl_etlink_bus_req pop_front();
      return m_bus_req_queue.pop_front();
   endfunction : pop_front


   //--------------------------------------------------------------------------------------------------
   function int get_req_cycle();
      if (m_bus_req_queue.size() == 0) begin
         return 0;
      end
      return m_bus_req_queue[0].get_req_cycle();
   endfunction : get_req_cycle

endclass : evl_etlink_bus_req_queue
