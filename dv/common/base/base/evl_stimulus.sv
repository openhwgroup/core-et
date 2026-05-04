//--------------------------------------------------------------------------------------------------
//
// Class: evl_stimulus
//
virtual class evl_stimulus extends evl_object;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_verif_comp_map m_verif_comp_map;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_semaphore_wrapper m_semaphore_wrapper;
   evl_message_wrapper   m_message_wrapper;


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_semaphore
   //
   task get_semaphore(input string name_in);
      m_semaphore_wrapper.get_semaphore(name_in);
   endtask : get_semaphore


   //-----------------------------------------------------------------------------------------------
   //
   // Task: put_semaphore
   //
   task put_semaphore(input string name_in);
      m_semaphore_wrapper.put_semaphore(name_in);
   endtask : put_semaphore


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_sync_count
   //
   function void incr_sync_count(input string name_in);
      m_semaphore_wrapper.incr_sync_count(name_in);
   endfunction : incr_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_sync_count
   //
   function void decr_sync_count(input string name_in);
      m_semaphore_wrapper.decr_sync_count(name_in);
   endfunction : decr_sync_count


   //-----------------------------------------------------------------------------------------------
   //
   // Task: decr_sync_count_and_wait
   //
   task decr_sync_count_and_wait(input string name_in);
      m_semaphore_wrapper.decr_sync_count_and_wait(name_in);
   endtask : decr_sync_count_and_wait


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_message_queue
   //
   function evl_message_queue get_message_queue(input string name_in);
      return m_message_wrapper.get_message_queue(name_in);
   endfunction : get_message_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message
   //
   function void send_message(input string name_in, input int source_in, input int type_in, input evl_dword_t value_in);
      m_message_wrapper.send_message(name_in, source_in, type_in, value_in);
   endfunction : send_message


   //-----------------------------------------------------------------------------------------------
   //
   // Function: send_message_packet
   //
   function void send_message_packet(input string name_in, input evl_message_packet packet_in);
      m_message_wrapper.send_message_packet(name_in, packet_in);
   endfunction : send_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_message_packet
   //
   function bit have_message_packet(input string name_in, input int subscriber_id);
      return m_message_wrapper.have_message_packet(name_in, subscriber_id);
   endfunction : have_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_message_packet
   //
   task get_message_packet(output evl_message_packet packet_out, input string name_in, input int subscriber_id);
      m_message_wrapper.get_message_packet(packet_out, name_in, subscriber_id);
   endtask : get_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Task: get_blocking_message_packet
   //
   task get_blocking_message_packet(output evl_message_packet packet_out, input string name_in, input int subscriber_id);
      m_message_wrapper.get_blocking_message_packet(packet_out, name_in, subscriber_id);
   endtask : get_blocking_message_packet


   //-----------------------------------------------------------------------------------------------
   //
   // Function: subscribe
   //
   function int subscribe(input string name_in);
      return m_message_wrapper.subscribe(name_in);
   endfunction : subscribe


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_subscribers
   //
   function bit have_subscribers(input string name_in);
      return m_message_wrapper.have_subscribers(name_in);
   endfunction : have_subscribers


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      return;
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_stimulus source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_file_path_list
   //
   function int get_full_file_path_list(output string         file_name_list_out[],
                                        input  string         default_file_name,
                                        input  string         plus_arg          = "",
                                        input  evl_verif_comp agent_in          = null,
                                        input  int            use_include_paths = 1,
                                        input  bit            notify_on_fail    = 1'b0);
      string file_name_list[$];
      string file_name = "";
      int    have_fail = 0;

      file_name_list = {};
      if (plus_arg != "") begin
         int    start_idx = 0;
         int    limit;
         string all_file_names;

         if (agent_in != null) begin
            all_file_names = agent_in.get_plus_arg_value(plus_arg);
         end
         else begin
            all_file_names = m_global_attrs.get_plus_arg_value(plus_arg);
         end
         if (all_file_names != "") begin
            limit = all_file_names.len();
            for (int ii = 0; ii < limit; ii++) begin
               if (all_file_names[ii] == "+") begin
                  if (ii > start_idx) begin
                     file_name = evl_tcl_find_file(all_file_names.substr(start_idx, ii - 1), use_include_paths);
                     if (file_name != "") begin
                        file_name_list.push_back(file_name);
                     end
                     else if (notify_on_fail == 1'b1) begin
                        have_fail = -1;
                     end
                  end
                  start_idx = ii + 1;
               end
            end
            if (limit > start_idx) begin
               file_name = evl_tcl_find_file(all_file_names.substr(start_idx, limit - 1), use_include_paths);
               if (file_name != "") begin
                  file_name_list.push_back(file_name);
               end
               else if (notify_on_fail == 1'b1) begin
                  have_fail = -1;
               end
            end
         end
      end
      if ((file_name_list.size() == 0) && (default_file_name != "")) begin
         file_name = evl_tcl_find_file(default_file_name, use_include_paths);
         if (file_name != "") begin
            file_name_list.push_back(file_name);
         end
      end
      if (file_name_list.size() == 0) begin
         file_name_list_out    = new[1];
         file_name_list_out[0] = "";
         return have_fail;
      end
      file_name_list_out = new[file_name_list.size()];
      for (int ii = 0; ii < file_name_list.size(); ii++) begin
         file_name_list_out[ii] = file_name_list[ii];
      end
      if (have_fail != 0) begin
         return (-1 - file_name_list_out.size());
      end
      return file_name_list_out.size();
   endfunction : get_full_file_path_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_full_file_path
   //
   function string get_full_file_path(input string         default_file_name,
                                      input string         plus_arg          = "",
                                      input evl_verif_comp agent_in          = null,
                                      input int            use_include_paths = 1);
      string file_name = "";

      if (plus_arg != "") begin
         if (agent_in != null) begin
            file_name = evl_tcl_find_file(agent_in.get_plus_arg_value(plus_arg), use_include_paths);
         end
         else begin
            file_name = evl_tcl_find_file(m_global_attrs.get_plus_arg_value(plus_arg), use_include_paths);
         end
         if (file_name != "") begin
            return file_name;
         end
      end
      if (default_file_name != "") begin
         file_name = evl_tcl_find_file(default_file_name, use_include_paths);
      end
      return file_name;
   endfunction : get_full_file_path


   //-----------------------------------------------------------------------------------------------
   //
   // Function: plus_arg_match
   //
   function bit plus_arg_match(input evl_shire_id_t        shire_id_in        = ~evl_shire_id_t'(0),
                               input evl_neighborhood_id_t neighborhood_id_in = ~evl_neighborhood_id_t'(0),
                               input evl_core_id_t         core_id_in         = ~evl_core_id_t'(0),
                               input string                plus_arg_name_in   = "",
                               input bit                   use_specific       = 1'b0);
      string plus_arg_name;

      if (plus_arg_name_in != "") begin
         plus_arg_name = plus_arg_name_in;
      end
      else begin
         plus_arg_name = get_type_name();
         if (plus_arg_name.substr(0, 3) == "evl_") begin
            plus_arg_name = plus_arg_name.substr(4, plus_arg_name.len() - 1);
         end
      end
      return m_global_attrs.plus_arg_match(plus_arg_name, shire_id_in, neighborhood_id_in, core_id_in, use_specific);
   endfunction : plus_arg_match


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_stimulus");
      super.new(name_in);
      set_abstract_name(name_in);
      if (m_verif_comp_map == null) begin
         m_verif_comp_map = evl_verif_comp_map::get_inst();
      end
      m_semaphore_wrapper = new(name_in);
      m_message_wrapper   = new(name_in);
      evl_stimulus_map::add_stimulus_to_map(this);
   endfunction : new

endclass : evl_stimulus
