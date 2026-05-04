class evl_core_req_seq extends evl_sequence_mdir;
   evl_trans_desc trans_desc;
   
   // Adress hash table for shared addresses used for cooperative stores
   evl_paddr_t store_addr_per_coop_way[5][$];

   // Adress hash table for shared addresses used for cooperative loads 
   evl_paddr_t coop_load_addr[];
   
   // Adress hash table for pre-randomized addresses for all the transactions 
   evl_paddr_t prerand_addr[];
   
   // Cooperative load tag id
   // Used only when the cmd is a LD_COOP
   evl_bus_coop_id_t coop_load_id[];

   // Cooperative load minion mask
   // Used only when the cmd is a LD_COOP
   bit [7:0] coop_load_mask[];

   // Number of ET-LINK commands to generate
   rand int num_txns;
   
   // Number of ET-LINK commands to generate
   rand int num_uniq_addr;
   
   // Transaction fields for the ET Link Request
   rand evl_dword_t data[8];
   rand evl_paddr_t addr[];
   rand int cmd_size[];
   rand evl_line_qword_en_t qwen[];
   rand evl_cmd_t   cmd_buffer[];
   rand evl_bus_req_source_t bus_req_source[];
   
   // Number of cooperative loads
   rand int num_load_coop;
   
   // Number of cooperative stores per coop way
   rand int num_store_coop;
  
   // Core id used for calculating the address for cooperative stores 
   int core_id;

   // From plusargs
   evl_cmd_t override_cmd = 0; 
  
   constraint num_txns_c
   {
      soft num_txns inside {[1:128]};
   }
   
   constraint num_cmd_c
   {
       qwen.size() == num_txns;
       cmd_size.size() == num_txns;
       addr.size() == num_uniq_addr;
       bus_req_source.size() == num_txns;
       cmd_buffer.size() == num_txns;
   }
   
   constraint coop_id_c
   {
      coop_load_id.size() == num_load_coop;
      foreach(coop_load_id[i])
         coop_load_id[i] inside {[1:255]};
   }
   
   constraint cmd_size_c 
   {
       foreach(cmd_size[i]) {
        cmd_buffer[i] == LD -> cmd_size[i] == 8;// {[1:8]};
        cmd_buffer[i] == ST -> cmd_size[i] inside {[1:8]};
       }
   }
   
   constraint qwen_c
   {
       foreach(qwen[i]) {
          cmd_buffer[i] == ST  &&  (cmd_size[i] inside {1,2}) -> $countones(qwen[i]) == 1;
          cmd_buffer[i] == ST  &&  (cmd_size[i] inside {3,4}) -> $countones(qwen[i]) == 2;
          cmd_buffer[i] == ST  &&  (cmd_size[i] inside {5,6}) -> $countones(qwen[i]) == 3;
          cmd_buffer[i] == ST  &&  (cmd_size[i] inside {7,8}) -> $countones(qwen[i]) == 4;
          cmd_buffer[i] == LD -> $countones(qwen[i]) == 0; 
       }
   }
   
   constraint bus_req_source_c
   {   
      foreach(bus_req_source[i])
         soft bus_req_source[i] == 0;
   } 
   
   constraint cmd_c
   {
   
      foreach(cmd_buffer[i])
      {
         override_cmd != 0 -> cmd_buffer[i] == override_cmd;
         override_cmd == 0 -> cmd_buffer[i] inside {LD, ST};
      }
   }
 
   constraint num_load_coop_c
   {
       soft num_load_coop inside {[1:128]};
   }   
   
   constraint addr_c
   {             
      foreach(addr[i])
      {
         cmd_buffer[i] inside {LD,ST}   -> addr[i] inside {prerand_addr};
         cmd_buffer[i] inside {LD_COOP} -> addr[i] inside {coop_load_addr};
      }
   }

   function void pre_randomize();
       return;
   endfunction
   
   virtual function evl_line_qword_en_t get_coop_store_qwen(evl_paddr_t addr, int coop_mode);
      evl_line_qword_en_t qwen;

       if (coop_mode == 4)
           qwen = evl_line_qword_en_t '(1 << (core_id % 4));
       else if (coop_mode == 2)
           qwen = evl_line_qword_en_t'(1 << ((2*addr[5]) + (core_id % 2)));
       else
           qwen = evl_line_qword_en_t'(4'b1111);

      `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("Core %0d, %0d-way coop stores addr %h, qwen = %b", core_id, coop_mode, addr, qwen ))
       return qwen;
   endfunction
  
   virtual task issue_coop_stores();

     evl_trans_attr_s trans_attr;
      //2-way or 4-way coop only
      for (int i=2; i<=4; i+=2) begin
          // Cooperative way
          trans_attr.subopcode_dest = i/2;
          if (store_addr_per_coop_way[i].size() != 0)

          foreach(store_addr_per_coop_way[i][j]) begin
              `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_name(), $sformatf("Coop store %0d-way address %h",i, store_addr_per_coop_way[i][j]));

              // Randomize write data
              std::randomize(data);

              issue(  .trans_desc(trans_desc),
                  .cmd_in(ST_WA),
                  .paddr_in(store_addr_per_coop_way[i][j]), 
                  .length_in(64),
                  .req_source_in(evl_bus_req_source_t'(0)),
                  .dwords_in(data),
                  .delay_wrapper_in(null), 
                  .qword_en_in(get_coop_store_qwen(store_addr_per_coop_way[i][j], i)),
                  .trans_attr_in(trans_attr), 
                  .cache_attr_in(1'b1),
                  .vc_in()
              );
          end
      end

   endtask: issue_coop_stores


   virtual task issue_coop_loads();
     evl_trans_attr_s trans_attr;

          foreach(coop_load_addr[i]) begin

              if (coop_load_mask[i][core_id] == 1) begin

                 `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_name(), $sformatf("Coop load address %h, Coop mask %b, coop id %h", coop_load_addr[i], coop_load_mask[i], coop_load_id[i]));

                 // Assign coop id and seq_id
                 //FUTURE add seq_id and split 
                 // Minion load coop mask
                 data[0] = evl_dword_t'({coop_load_id[i], coop_load_mask[i]});
                 trans_attr.coop_id = coop_load_id[i];
                 trans_attr.coop_minion_mask = coop_load_mask[i];
                 trans_attr.coop_neigh_mask  = 0;

                 issue(  .trans_desc(trans_desc),
                     .cmd_in(LD_COOP),
                     .paddr_in(coop_load_addr[i]), 
                     .length_in(64),
                     .req_source_in(evl_bus_req_source_t'(0)),
                     .dwords_in(data),
                     .delay_wrapper_in(null), 
                     .qword_en_in(4'b0001),
                     .trans_attr_in(trans_attr), 
                     .cache_attr_in(1'b1),
                     .vc_in()
                   );

             end
         end

   endtask: issue_coop_loads


   //
   // Task : main_phase
   //
    virtual task main_phase(uvm_phase phase);
     evl_trans_attr_s trans_attr;

     phase.raise_objection(phase);
    
     issue_coop_stores();
     issue_coop_loads();

     foreach(cmd_buffer[i]) begin
        

        std::randomize(data);

        `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_name(), $sformatf("Cmd %s Addr %h Size (Dwords) %0d Req Source %0d Qwen %b", cmd_buffer[i].name(), addr[i], cmd_size[i], bus_req_source[i], qwen[i]))
        
        issue(  .trans_desc(trans_desc),
                     .cmd_in(cmd_buffer[i]),
                     .paddr_in(addr[i]), 
                     .length_in(8*cmd_size[i]),
                     .req_source_in(bus_req_source[i]),
                     .dwords_in(data),
                     .delay_wrapper_in(null), 
                     .qword_en_in(qwen[i]),
                     .trans_attr_in(), 
                     .cache_attr_in(1'b1),
                     .vc_in()
              );

     end

     phase.drop_objection(phase);

     endtask : main_phase

     //--------------------------------------------------------------------------------------------
     //
     // Constructor
     //
     function new(input string name_in = "evl_core_req_seq");
        super.new(name_in);
      
        if(`et_value_plusargs("neigh_cmd_override=%d", override_cmd))
            `evl_log(UVM_MEDIUM, VMOD_CONFIG, get_name(), $sformatf("Command line override detected, generating only %s transactions ", override_cmd.name()))

     endfunction


endclass

