//-----------------------------------------------------------------------------------------------
//
// Class: evl_neigh_simple_test
//
class evl_neigh_base_test_seq extends evl_sequence_mdir;
   
   // Minion core sequence for ET_LINK requests  + Icache + 2 PTW
   evl_core_req_seq evl_core_req_seq_h[`MIN_PER_N+1+2];

   // Maximum number of unique addresses
   // Adress hash table for prers/-randomized addresses for all the transactions 
   rand evl_paddr_t prerand_addr[9][];
   rand int num_txns = 2;
   rand int num_coop_loads;
   rand int num_coop_stores;

   // Adress hash table for shared addresses used for cooperative loads  (16 address tags
   rand evl_paddr_t coop_load_addr[];

   // Cooperative load tag id
   // Used only when the cmd is a LD_COOP
   rand bit [7:0] coop_load_id[];

   // Cooperative load minion mask
   // Used only when the cmd is a LD_COOP
   rand bit [7:0] coop_load_mask[];

   // Adress hash table for shared addresses used for cooperative loads  (16 address tags
   rand evl_paddr_t coop_store_addr[];

   int coop_store_addr_q[evl_paddr_t][$];

   // Number of transactions per minion
   constraint num_txns_c
   {
      soft num_txns inside {[1:32]};
   }

   constraint num_coop_loads_c
   {
       soft num_coop_loads == 1;
       coop_load_addr.size() == num_coop_loads;
       coop_load_mask.size() == num_coop_loads;
       coop_load_id.size()   == num_coop_loads;

       foreach(coop_load_mask[i])
           $countones(coop_load_mask[i]) inside {[2:8]};

       foreach(coop_load_id[i])
         coop_load_id[i] inside {[1:255]};

        unique {coop_load_id};

   }

   constraint num_coop_stores_c
   {
       soft num_coop_stores == 16;
       coop_store_addr.size() == num_coop_stores;
   }

   // Uniquify cooperative load/store addresses for minions
   constraint coop_addr_c 
   {
      unique {coop_load_addr,prerand_addr};
      foreach(coop_load_addr[i]) {
          coop_load_addr[i][5:0]  inside {6'b00000, 6'b000010, 6'b000100, 6'b001000, 6'b010000, 6'b100000};
          coop_load_addr[i][39] == 1;
      }
      unique {coop_store_addr,prerand_addr};

      foreach(coop_store_addr[i]) {
          // Cacheable
          coop_store_addr[i][39] == 1;
          // Exclude cache line splits on coop stores (2-way cooperative) 
          coop_store_addr[i][5:4] != 2'b11;
      }

   }

   // Uniquify addresses used by minions 
   constraint prerand_addr_c
   {
      foreach(prerand_addr[i])
      {
         prerand_addr[i].size() == num_txns;
         unique {prerand_addr[i]};
         foreach(prerand_addr[i][j]) {
            prerand_addr[i][j][5:0]  inside {6'b00000, 6'b000010,6'b000100, 6'b001000, 6'b010000, 6'b100000};
            prerand_addr[i][j][39] == 'b1;
         }
      }         
   }

   //--------------------------------------------------------------------------------------------
   //
   // Function: post_randomize
   //
   function void post_randomize();
      evl_trans_desc trans_desc;
      int status;
      int coop_store_q[$];
      evl_paddr_t store_addr_per_coop_way[5][$];
      string coop_minions_str = "";

      evl_paddr_t tmp_addr;

      //`evl_log(UVM_DEBUG, VMOD_TEST, get_name(), $sformatf("================== Test Config ================"))
      //`evl_log(UVM_DEBUG, VMOD_TEST, get_name(), $sformatf("Number of transactions %0d, Coop Loads %0d, Coop Stores %0d", num_txns, , num_coop_loads, num_coop_stores))

      foreach(coop_load_addr[i])
          `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("Coop load Addr %h, Coop Way %0d, Coop Id %0d Coop Minions %b", coop_load_addr[i], $countones(coop_load_mask[i]), coop_load_id[i], coop_load_mask[i]))
      

       // Randomize tensor store cooperation pairs per address
       foreach(coop_store_addr[i]) begin

         coop_store_q = coop_store_addr_q[coop_store_addr[i]];

         // Randomize either 2-way or 4-way cooperative mode
         // FUTURE intra neigh coop modes (8-way etc)
         status = std::randomize(coop_store_q) with {coop_store_q.size() inside {2,4};};

         if (!status)
            `dut_error(get_abstract_name(), $sformatf("Failed to randomized minion_id "))
            
         // Nullify coop minion string for each of the cooperative store addresses
         coop_minions_str = "";
            
         // Randomize minion pairs based on the mode
         // Note that according to the specs in coop mode only even/odd pairs are allowed, hence all the other combinations
         // are invalid
         foreach(coop_store_q[i]) begin
            status = std::randomize(coop_store_q) with { unique {coop_store_q};
                foreach(coop_store_q[i]) {
                                                               ((i%2) == 0) -> coop_store_q[i] inside {0,2,4,6};
                                                               ((i%2) == 1) -> coop_store_q[i] inside {1,3,5,7};
                                                            }
                                                         };
               if (!status)
                  `dut_error(get_abstract_name(), $sformatf("Failed to randomized coop_store minion pairs "))

               coop_minions_str = {coop_minions_str, $sformatf(" %0d", coop_store_q[i])};
            end
             
            // Copy coop queue to the global coop_store_queue
            tmp_addr = coop_store_addr[i];
            coop_store_addr_q[tmp_addr] = coop_store_q;
            `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("Coop store Addr %h, Coop Way %0d, Coop Minions %s", coop_store_addr[i], coop_store_addr_q[coop_store_addr[i]].size(), coop_minions_str ))
      end


      for (int i=0; i<`MIN_PER_N; i++) begin
         evl_core_req_seq_h[i] = new();
    
         foreach(coop_store_addr[j]) begin
            coop_store_q = coop_store_addr_q[coop_store_addr[j]];
            if ( i inside { coop_store_q } ) begin
               `evl_log(UVM_HIGH, VMOD_TEST, get_name(), $sformatf("Generating Coop store Addr %h, Coop queue %0d, Minion %0d", coop_store_addr[j], coop_store_q.size(), i))
                evl_core_req_seq_h[i].store_addr_per_coop_way[coop_store_q.size()].push_back(coop_store_addr[j]);
            end
         end

         // Pass prerand and coop_load ansd coop_store addresses
         evl_core_req_seq_h[i].prerand_addr            = prerand_addr[i];
         evl_core_req_seq_h[i].coop_load_addr          = coop_load_addr;
         evl_core_req_seq_h[i].coop_load_mask          = coop_load_mask;
         evl_core_req_seq_h[i].coop_load_id            = coop_load_id;
         evl_core_req_seq_h[i].core_id                 = i;
         status = evl_core_req_seq_h[i].randomize() with { num_txns == local::num_txns; num_uniq_addr == local::num_txns; };
         
         if (!status)
            `dut_error(get_abstract_name(), $sformatf("Failed to randomized evl_core_req_seq "))

      end

  endfunction : post_randomize

  virtual task main_phase(input uvm_phase phase);
   
   phase.raise_objection(phase);
   super.main_phase(phase);
   phase.drop_objection(phase);

   endtask : main_phase

   //--------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_neigh_base_test_seq");
      super.new(name_in);
   endfunction

endclass : evl_neigh_base_test_seq

