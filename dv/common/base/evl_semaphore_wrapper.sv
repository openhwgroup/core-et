//--------------------------------------------------------------------------------------------------
//
// Class: evl_semaphore_wrapper
//
class evl_semaphore_wrapper extends evl_object;

   `uvm_object_utils(evl_semaphore_wrapper)

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   semaphore m_semaphores[string];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_semaphore
   //
   function void create_semaphore(input string name_in);
      if (!m_semaphores.exists(name_in)) begin
         m_semaphores[name_in] = new(1);
      end
      return;
   endfunction : create_semaphore


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_semaphore
   //
   task get_semaphore(input string name_in);
      if (!m_semaphores.exists(name_in)) begin
         m_semaphores[name_in] = new();
      end
      m_semaphores[name_in].get(1);
   endtask : get_semaphore


   //-----------------------------------------------------------------------------------------------
   //
   // Function: put_semaphore
   //
   function void put_semaphore(input string name_in);
      if (!m_semaphores.exists(name_in)) begin
         m_semaphores[name_in] = new(1);
         return;
      end
      m_semaphores[name_in].put(1);
   endfunction : put_semaphore


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_sync_count
   //
   function void create_sync_count(input string name_in, input int value_in);
      if (have_int_param_by_name(name_in) == 1'b0) begin
         set_int_param_by_name(name_in, value_in);
      end
   endfunction : create_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_sync_count
   //
   function void incr_sync_count(input string name_in);
      void'(incr_int_param_by_name(name_in));
   endfunction : incr_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_sync_count
   //
   function void decr_sync_count(input string name_in);
      void'(decr_int_param_by_name(name_in));
   endfunction : decr_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: decr_sync_count_and_wait
   //
   task decr_sync_count_and_wait(input string name_in);
      if (decr_int_param_by_name(name_in, 0) == 0) begin
         return;
      end
      wait_for_int_param_by_name(name_in, 0, 0);
   endtask : decr_sync_count_and_wait


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_sync_count
   //
   function int get_sync_count(input string name_in);
      return get_int_param_by_name(name_in, 0);
   endfunction : get_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_semaphore_wrapper");
      super.new(name_in);
      set_abstract_name(name_in);
   endfunction : new

endclass : evl_semaphore_wrapper
