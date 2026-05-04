//--------------------------------------------------------------------------------------------------
//
// Class: evl_unique_element_queue
//
class evl_unique_element_queue #(type t_item = evl_paddr_t) extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   semaphore m_lock = new(1);
   t_item    m_item_queue[$];


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   task add_item(output bit success, input t_item item_in);
      m_lock.get(1);
      if (item_in inside { m_item_queue }) begin
         success = 1'b0;
      end
      else begin
         m_item_queue.push_back(item_in);
         success = 1'b1;
      end
      m_lock.put(1);
   endtask : add_item


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   task delete_item(input t_item item_in);
      m_lock.get(1);
      foreach (m_item_queue[ii]) begin
         if (m_item_queue[ii] == item_in) begin
            m_item_queue.delete(ii);
            m_lock.put(1);
            return;
         end
      end
      m_lock.put(1);
   endtask : delete_item


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_unique_element_queue");
      super.new(name_in);
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_unique_element_queue
