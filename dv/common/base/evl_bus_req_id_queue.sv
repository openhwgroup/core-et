//--------------------------------------------------------------------------------------------------
//
// Class: evl_bus_req_id_queue
//
class evl_bus_req_id_queue extends uvm_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   int              m_cur_index  = 0;
   int              m_last_index = 0;
   evl_bus_req_id_t m_queue[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push
   //
   function void push(input evl_bus_req_id_t req_id_in);
      m_queue.push_back(req_id_in);
   endfunction : push


   //-----------------------------------------------------------------------------------------------
   //
   // Function: size
   //
   function int size();
      return m_queue.size();
   endfunction : size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next
   //
   function bit get_next(output evl_bus_req_id_t req_id_out, input bit first_call = 1'b0, input bit random_start = 1'b0);
      if (m_queue.size() == 0) begin
         req_id_out = evl_bus_req_id_t'(0);
         return 1'b0;
      end

      if (m_cur_index >= m_queue.size()) begin
         m_cur_index = 0;
      end
      if (first_call == 1'b0) begin
         bit result;

         req_id_out = m_queue[m_cur_index];
         result = ((m_cur_index == m_last_index) ? 1'b0 : 1'b1);
         m_cur_index++;
         return result;
      end
      if (random_start == 1'b1) begin
         m_cur_index = $urandom % m_queue.size();
      end
      m_last_index = m_cur_index;
      req_id_out = m_queue[m_cur_index];
      m_cur_index++;
      return 1'b1;
   endfunction : get_next

endclass : evl_bus_req_id_queue
