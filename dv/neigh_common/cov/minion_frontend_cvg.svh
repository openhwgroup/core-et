`ifdef EVL_COVERAGE_ON

`ifndef __MINION_FRONTEND_CVG
`define __MINION_FRONTEND_CVG

package minion_frontend_cov_pkg;

   covergroup minion_frontend_cg (   
   // FE coverage signals
   // fixed helper signals
   ref bit chicken_bit, 
   ref bit f1_icache_req_ready, 
   ref bit f1_icache_req_valid,
   ref bit f5_icache_resp_valid, 
   ref bit f5_icache_resp_miss, 
   ref bit f6_icache_fill_done, 
   ref bit f5_icache_resp_cacheable, 
   ref bit f5_icache_resp_access_fault, 
   ref bit f5_icache_resp_page_fault, 
   ref bit [1:0] t0_vm_status_prv, 
   ref bit [1:0] t0_vm_status_mpp, 
   ref bit t0_vm_status_mprv, 
   ref bit t0_vm_status_sum, 
   ref bit t0_vm_status_mxr, 
   ref bit t0_vm_status_debug, 
   ref bit [1:0] t1_vm_status_prv,
   ref bit [1:0] t1_vm_status_mpp, 
   ref bit t1_vm_status_mprv, 
   ref bit t1_vm_status_sum, 
   ref bit t1_vm_status_mxr, 
   ref bit t1_vm_status_debug, 
   ref bit [`CORE_NR_THREADS-1:0] f0_icache_req_valid_arb,
   ref bit t0_f0_icache_req_valid, 
   ref bit t1_f0_icache_req_valid, 
   ref bit [`FE_FETCH_BUFFERS-1:0] t0_buffer_cacheable, 
   ref bit [`FE_FETCH_BUFFERS-1:0] t1_buffer_cacheable, 
   ref bit t0_f7_buffer_id,
   ref bit t1_f7_buffer_id,
   ref bit t0_f6_valid,
   ref bit t0_f6_buffer_wr, 
   ref bit t1_f6_valid, 
   ref bit t1_f6_buffer_wr, 
   ref bit t0_f6_icache_resp_cacheable,
   ref bit t1_f6_icache_resp_cacheable,
   ref bit t0_icache_inflight,
   ref bit t1_icache_inflight,
   ref bit t0_miss_pending,
   ref bit t1_miss_pending,

   // FE helper signals
   ref bit f5_icache_resp_miss_seen, 
   ref bit prev_icache_resp_was_uncacheable,
   ref bit prev_icache_resp_was_cacheable,
   ref bit first_t0_arb_req = 0,
   ref bit first_t1_arb_req = 0,
   ref bit [2:0] consecutive_t0_arb_req = 0,
   ref bit [2:0] consecutive_t1_arb_req = 0,
   ref bit first_t0_inst = 0,
   ref bit first_t1_inst = 0,
   ref bit [2:0] consecutive_t0_inst = 0,
   ref bit [2:0] consecutive_t1_inst = 0,

   ref bit [4:0] icache_req_valid_hi_cycles = 0,
   ref bit [4:0] icache_req_valid_to_resp_cycles = 0,
   ref bit [4:0] icache_req_to_redir_cycles = 0,
   ref bit uncacheable_inst_t0 = 0,
   ref bit uncacheable_inst_t1 = 0,
   ref bit t0_f6_bypass,
   ref bit t1_f6_bypass,
   ref bit t0_f7_bypass,
   ref bit t1_f7_bypass,
   ref bit t0_inst_from_bypass,
   ref bit t1_inst_from_bypass,
   ref bit icache_request_made = 0,
   ref bit icache_ready_seen = 0,

   ref bit replay_inst_t0 = 0,
   ref bit replay_inst_t1 = 0,

   ref bit t0_inst_uc_from_bypass,
   ref bit t1_inst_uc_from_bypass,

   ref bit [4:0] inst_ready_lo_cycles = 0,
   ref bit [4:0] redir0_to_redir1_cycles = 0,
   ref bit redir0_seen,

   ref bit [1:0] stall_mon_buffer_empty_t0,     
   ref bit [1:0] stall_mon_buffer_empty_t1,      
   ref bit fe_mon_id_inst_ready,
   ref bit fe_mon_id_inst_valid,
   ref bit fe_mon_id_inst_thread_id,
   ref bit [1:0] fe_mon_f0_core_req_valid,
   ref bit fe_mon_id_inst_data_replay, 
   ref bit fe_mon_ipi_with_pc,  
   ref bit fe_mon_wb_xcpt,
   ref bit fe_mon_wb_csr_eret,  
   ref bit fe_mon_wb_csr_replay,
   ref bit fe_mon_wb_reg_replay

);
         option.name = "minion_frontend_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;


      chicken_bit_cvg:  coverpoint chicken_bit;
      icache_hit_miss_signalled_chicken_cvg:  coverpoint f5_icache_resp_miss iff ( f5_icache_resp_valid === 1'b1 && chicken_bit === 1);
      icache_fill_done_signalled_chicken_cvg: coverpoint f6_icache_fill_done iff ( f5_icache_resp_miss_seen === 1'b1 && chicken_bit === 1);

      icache_hit_miss_signalled_cvg:  coverpoint f5_icache_resp_miss iff ( f5_icache_resp_valid === 1'b1);
      icache_fill_done_signalled_cvg: coverpoint f6_icache_fill_done iff ( f5_icache_resp_miss_seen === 1'b1);

      icache_resp_page_fault_signalled_cvg:  coverpoint f5_icache_resp_page_fault iff ( f5_icache_resp_valid === 1'b1);
      icache_access_fault_signalled_cvg:  coverpoint f5_icache_resp_access_fault iff ( f5_icache_resp_valid === 1'b1);

      //icache_uncacheable_signalled_cvg:  coverpoint f5_icache_resp_cacheable iff ( f5_icache_resp_valid === 1'b1); //removed as per Sebastia
      //icache_uncacheable_then_cacheable_cvg:  coverpoint f5_icache_resp_cacheable iff ( prev_icache_resp_was_uncacheable === 1'b1);
      //icache_cacheable_then_uncacheable_cvg:  coverpoint f5_icache_resp_cacheable iff ( prev_icache_resp_was_cacheable === 1'b1);

      vm_status_t0_prv_cvg:  coverpoint t0_vm_status_prv  {
             ignore_bins prv_10 = {2'b10};
         }
      vm_status_t0_mpp_cvg:  coverpoint t0_vm_status_mpp  {
             ignore_bins mpp_10 = {2'b10};
         }
      vm_status_t0_mprv_cvg:  coverpoint t0_vm_status_mprv;
      vm_status_t0_sum_cvg:   coverpoint t0_vm_status_sum;
      vm_status_t0_debug_cvg: coverpoint t0_vm_status_debug;

      vm_status_t1_prv_cvg:  coverpoint t1_vm_status_prv  {
             ignore_bins prv_10 = {2'b10};
         }
      vm_status_t1_mpp_cvg:  coverpoint t1_vm_status_mpp  {
             ignore_bins mpp_10 = {2'b10};
         }
      vm_status_t1_mprv_cvg:  coverpoint t1_vm_status_mprv;
      vm_status_t1_sum_cvg:   coverpoint t1_vm_status_sum;
      vm_status_t1_debug_cvg: coverpoint t1_vm_status_debug;


////////////////////////////////////////////////
      inst_buffers_bothreq2arb_t1_0_t0_01_chicken_cvg : coverpoint stall_mon_buffer_empty_t0  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t1 === 2'b11 && chicken_bit === 1) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }


      inst_buffers_bothreq2arb_t1_1_t0_01_chicken_cvg : coverpoint stall_mon_buffer_empty_t0  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && (stall_mon_buffer_empty_t1 === 2'b10 ||stall_mon_buffer_empty_t1 === 2'b01) && chicken_bit === 1) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }

      inst_buffers_bothreq2arb_t0_0_t1_01_chicken_cvg : coverpoint stall_mon_buffer_empty_t1  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t0 === 2'b11 && chicken_bit === 1) {
             bins inst_buffer_t1_0  = {2'b11};
             bins inst_buffer_t1_1a = {2'b10};
             bins inst_buffer_t1_1b = {2'b01};
         }


      inst_buffers_bothreq2arb_t0_1_t1_01_chicken_cvg : coverpoint stall_mon_buffer_empty_t1  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && (stall_mon_buffer_empty_t0 === 2'b10 ||stall_mon_buffer_empty_t0 === 2'b01) && chicken_bit === 1) {
             bins inst_buffer_t1_0  = {2'b11};
             bins inst_buffer_t1_1a = {2'b10};
             bins inst_buffer_t1_1b = {2'b01};
         }
////////////////////////////////////////////////



      inst_buffers_bothreq2arb_t1_0_t0_01_cvg : coverpoint stall_mon_buffer_empty_t0  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t1 === 2'b11) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }


      inst_buffers_bothreq2arb_t1_1_t0_01_cvg : coverpoint stall_mon_buffer_empty_t0  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && (stall_mon_buffer_empty_t1 === 2'b10 ||stall_mon_buffer_empty_t1 === 2'b01)) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }

      inst_buffers_bothreq2arb_t0_0_t1_01_cvg : coverpoint stall_mon_buffer_empty_t1  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t0 === 2'b11) {
             bins inst_buffer_t1_0  = {2'b11};
             bins inst_buffer_t1_1a = {2'b10};
             bins inst_buffer_t1_1b = {2'b01};
         }


      inst_buffers_bothreq2arb_t0_1_t1_01_cvg : coverpoint stall_mon_buffer_empty_t1  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1 && (stall_mon_buffer_empty_t0 === 2'b10 ||stall_mon_buffer_empty_t0 === 2'b01)) {
             bins inst_buffer_t1_0  = {2'b11};
             bins inst_buffer_t1_1a = {2'b10};
             bins inst_buffer_t1_1b = {2'b01};
         }

      inst_buffers_t0req2arb_t1_2_t0_01_cvg : coverpoint stall_mon_buffer_empty_t0  iff (t0_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t1 === 2'b00) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }

      inst_buffers_t0req2arb_t0_2_t1_01_cvg : coverpoint stall_mon_buffer_empty_t1  iff (t0_f0_icache_req_valid === 1'b1 && stall_mon_buffer_empty_t0 === 2'b00) {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
         }

      inst_buffers_bothreq2arb_t0_0to7_cvg : coverpoint consecutive_t0_arb_req  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1) {
             bins cons_0 = {3'h0};
             bins cons_1 = {3'h1};
             bins cons_2 = {3'h2};
             bins cons_3 = {3'h3};
             bins cons_4 = {3'h4};
             bins cons_5 = {3'h5};
             bins cons_6 = {3'h6};
             bins cons_7 = {3'h7}; 
         }

      inst_buffers_bothreq2arb_t1_0to7_cvg : coverpoint consecutive_t1_arb_req  iff (t0_f0_icache_req_valid === 1'b1 && t1_f0_icache_req_valid === 1'b1) {
             bins cons_0 = {3'h0};
             bins cons_1 = {3'h1};
             bins cons_2 = {3'h2};
             bins cons_3 = {3'h3};
             bins cons_4 = {3'h4};
             bins cons_5 = {3'h5};
             bins cons_6 = {3'h6};
             bins cons_7 = {3'h7}; 
	 }


      inst_t1_t0_0to7_cvg : coverpoint consecutive_t0_inst  iff (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1) {
             bins cons_0 = {3'h0};
             bins cons_1 = {3'h1};
             bins cons_2 = {3'h2};
             bins cons_3 = {3'h3};
             bins cons_4 = {3'h4};
             bins cons_5 = {3'h5};
             bins cons_6 = {3'h6};
             bins cons_7 = {3'h7}; 
         }


      inst_t0_t1_0to7_cvg : coverpoint consecutive_t1_inst  iff (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0) {
             bins cons_0 = {3'h0};
             bins cons_1 = {3'h1};
             bins cons_2 = {3'h2};
             bins cons_3 = {3'h3};
             bins cons_4 = {3'h4};
             bins cons_5 = {3'h5};
             bins cons_6 = {3'h6};
             bins cons_7 = {3'h7}; 
         }


      inst_marked_replay_t0_cvg:  coverpoint fe_mon_id_inst_data_replay iff (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0);
      inst_marked_replay_t1_cvg:  coverpoint fe_mon_id_inst_data_replay iff (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1);

      icache_ready_valid_0_to_16_cvg : coverpoint icache_req_valid_hi_cycles   {
             bins cons_0 = {4'h0};
             bins cons_1 = {4'h1};
             bins cons_2 = {4'h2};
             bins cons_3 = {4'h3};
             bins cons_4 = {4'h4};
             bins cons_5 = {4'h5};
             bins cons_6 = {4'h6};
             bins cons_7 = {4'h7}; 
             bins cons_8 = {4'h8};
             bins cons_9 = {4'h9};
             bins cons_10 = {4'ha};
             bins cons_11 = {4'hb};
             bins cons_12 = {4'hc};
             bins cons_13 = {4'hd};
          }

       //uncacheable_not_marked_replay_t0_cvg: coverpoint fe_mon_id_inst_data_replay  iff (t0_buffer_cacheable[t0_f7_buffer_id]===1'b0 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0);
       //uncacheable_not_marked_replay_t1_cvg: coverpoint fe_mon_id_inst_data_replay  iff (t1_buffer_cacheable[t1_f7_buffer_id]===1'b0 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1);

       //uncacheable_inst_t0_cvg: coverpoint uncacheable_inst_t0;	
       //uncacheable_inst_t1_cvg: coverpoint uncacheable_inst_t1;	

       //cross_uncacheable_inst_t0t1_cvg : cross uncacheable_inst_t0_cvg , uncacheable_inst_t1_cvg; 

       inst_from_bypass_t0_cvg: coverpoint t0_inst_from_bypass;	
       inst_from_bypass_t1_cvg: coverpoint t1_inst_from_bypass;	

       cross_inst_from_bypass_t0t1_cvg : cross inst_from_bypass_t0_cvg , inst_from_bypass_t1_cvg; 

       //cons_uncacheable_from_bypass_t0_cvg: coverpoint t0_inst_uc_from_bypass  iff (fe_mon_id_inst_data_replay === 1'b1 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0);
       //cons_uncacheable_from_bypass_t1_cvg: coverpoint t1_inst_uc_from_bypass  iff (fe_mon_id_inst_data_replay === 1'b1 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1);

       //uncacheable_then_cacheable_from_bypass_t0_cvg: coverpoint t0_inst_uc_from_bypass  iff (fe_mon_id_inst_data_replay === 1'b0 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0);
       //uncacheable_then_cacheable_from_bypass_t1_cvg: coverpoint t1_inst_uc_from_bypass  iff (fe_mon_id_inst_data_replay === 1'b0 && fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1);


      inst_ready_lo_0_to_31_cvg : coverpoint inst_ready_lo_cycles   {
             bins lo  = {0,10};
             bins med = {11,20};
             bins hi  = {21,31};
          }

      inst_buffers_full_t0_cvg : coverpoint stall_mon_buffer_empty_t0 {
             bins inst_buffer_t0_0  = {2'b11};
             bins inst_buffer_t0_1a = {2'b10};
             bins inst_buffer_t0_1b = {2'b01};
             bins inst_buffer_t0_2  = {2'b00};
         }

      inst_buffers_full_t1_cvg : coverpoint stall_mon_buffer_empty_t1 {
             bins inst_buffer_t1_0  = {2'b11};
             bins inst_buffer_t1_1a = {2'b10};
             bins inst_buffer_t1_1b = {2'b01};
             bins inst_buffer_t1_2  = {2'b00};
         }


      cross_readylo_buffers_t0t1_cvg : cross inst_ready_lo_0_to_31_cvg , inst_buffers_full_t0_cvg, inst_buffers_full_t1_cvg; 

      redirects_t01_cvg : coverpoint fe_mon_f0_core_req_valid {
             bins t1 = {2'b10};
             bins t0 = {2'b01};
         }

      cross_redirects_buffers_t0t1_cvg : cross redirects_t01_cvg , inst_buffers_full_t0_cvg, inst_buffers_full_t1_cvg; 

      redir0_to_redir1_cvg : coverpoint redir0_to_redir1_cycles   {
             bins lo  = {0,10};
             bins med = {11,20};
             bins hi  = {21,31};
          }

      icache_req_to_redir_t0_cycles_cvg : coverpoint  icache_req_to_redir_cycles iff (fe_mon_f0_core_req_valid[0] === 1)  {
             bins zero  = {0};
             bins  one  = {1};
             bins  two  = {2};
             bins three = {3};
             bins more_than_3  = {4,31};
          }

      icache_req_to_redir_t1_cycles_cvg : coverpoint  icache_req_to_redir_cycles iff (fe_mon_f0_core_req_valid[1] === 1)  {
             bins zero  = {0};
             bins  one  = {1};
             bins  two  = {2};
             bins three = {3};
             bins more_than_3  = {4,31};
          }


      redirects_t01_t0_inflight : coverpoint fe_mon_f0_core_req_valid iff (t0_icache_inflight === 1) {
             bins t1 = {2'b10};
             bins t0 = {2'b01};
         }

      redirects_t01_t1_inflight : coverpoint fe_mon_f0_core_req_valid iff (t1_icache_inflight === 1) {
             bins t1 = {2'b10};
             bins t0 = {2'b01};
         }

      redirects_t01_t0__miss_pending : coverpoint fe_mon_f0_core_req_valid iff (t0_miss_pending === 1) {
             bins t1 = {2'b10};
             bins t0 = {2'b01};
         }

      redirects_t01_t1__miss_pending : coverpoint fe_mon_f0_core_req_valid iff (t1_miss_pending === 1) {
             bins t1 = {2'b10};
             bins t0 = {2'b01};
         }



   endgroup
      

endpackage;

module minion_frontend_cvg (input clock);
      import minion_frontend_cov_pkg::*;

      // FE coverage signals
      bit chicken_bit; 
      bit f1_icache_req_ready; 
      bit f1_icache_req_valid;
      bit f5_icache_resp_valid; 
      bit f5_icache_resp_miss; 
      bit f6_icache_fill_done; 
      bit f5_icache_resp_cacheable; 
      bit f5_icache_resp_access_fault; 
      bit f5_icache_resp_page_fault; 
      bit [1:0] t0_vm_status_prv;
      bit [1:0] t0_vm_status_mpp; 
      bit t0_vm_status_mprv;
      bit t0_vm_status_sum;
      bit t0_vm_status_mxr; 
      bit t0_vm_status_debug; 
      bit [1:0] t1_vm_status_prv;
      bit [1:0] t1_vm_status_mpp; 
      bit t1_vm_status_mprv;
      bit t1_vm_status_sum;
      bit t1_vm_status_mxr; 
      bit t1_vm_status_debug;
      bit [`CORE_NR_THREADS-1:0] f0_icache_req_valid_arb;
      bit t0_f0_icache_req_valid; 
      bit t1_f0_icache_req_valid; 
      bit [`FE_FETCH_BUFFERS-1:0] t0_buffer_cacheable; 
      bit [`FE_FETCH_BUFFERS-1:0] t1_buffer_cacheable; 
      bit t0_f7_buffer_id;
      bit t1_f7_buffer_id;
      bit t0_f6_valid;
      bit t0_f6_buffer_wr; 
      bit t1_f6_valid; 
      bit t1_f6_buffer_wr; 
      bit t0_f6_icache_resp_cacheable;
      bit t1_f6_icache_resp_cacheable;
      bit t0_icache_inflight;
      bit t1_icache_inflight;
      bit t0_miss_pending;
      bit t1_miss_pending;
      //FE helper signals
      bit f5_icache_resp_miss_seen; 
      bit prev_icache_resp_was_uncacheable;
      bit prev_icache_resp_was_cacheable;
      bit first_t0_arb_req = 0;
      bit first_t1_arb_req = 0;
      bit [2:0] consecutive_t0_arb_req = 0;
      bit [2:0] consecutive_t1_arb_req = 0;
      bit first_t0_inst = 0;
      bit first_t1_inst = 0;
      bit [2:0] consecutive_t0_inst = 0;
      bit [2:0] consecutive_t1_inst = 0;

      bit [4:0] icache_req_valid_hi_cycles = 0;
      bit [4:0] icache_req_valid_to_resp_cycles = 0;
      bit [4:0] icache_req_to_redir_cycles = 0;
      bit uncacheable_inst_t0 = 0;
      bit uncacheable_inst_t1 = 0;
      bit t0_f6_bypass;
      bit t1_f6_bypass;
      bit t0_f7_bypass;
      bit t1_f7_bypass;
      bit t0_inst_from_bypass;
      bit t1_inst_from_bypass;
      bit icache_request_made = 0;
      bit icache_ready_seen = 0;

      bit replay_inst_t0 = 0;
      bit replay_inst_t1 = 0;

      bit t0_inst_uc_from_bypass;
      bit t1_inst_uc_from_bypass;

      bit [4:0] inst_ready_lo_cycles = 0;
      bit [4:0] redir0_to_redir1_cycles = 0;
      bit redir0_seen;
      bit [1:0] stall_mon_buffer_empty_t0;      //= `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.buffer_empty;
      bit [1:0] stall_mon_buffer_empty_t1;      //= `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.buffer_empty;
      bit fe_mon_id_inst_ready;
      bit fe_mon_id_inst_valid;
      bit fe_mon_id_inst_thread_id;
      bit [1:0] fe_mon_f0_core_req_valid;
      bit fe_mon_id_inst_data_replay;
      bit fe_mon_ipi_with_pc;
      bit fe_mon_wb_xcpt;
      bit fe_mon_wb_csr_eret;  
      bit fe_mon_wb_csr_replay;
      bit fe_mon_wb_reg_replay;


	minion_frontend_cg	minion_frontend_cg_inst = new(        
       // FE coverage signals
       chicken_bit, 
       f1_icache_req_ready, 
       f1_icache_req_valid,
       f5_icache_resp_valid, 
       f5_icache_resp_miss, 
       f6_icache_fill_done, 
       f5_icache_resp_cacheable, 
       f5_icache_resp_access_fault, 
       f5_icache_resp_page_fault, 
       t0_vm_status_prv, 
       t0_vm_status_mpp, 
       t0_vm_status_mprv, 
       t0_vm_status_sum, 
       t0_vm_status_mxr, 
       t0_vm_status_debug, 
       t1_vm_status_prv,
       t1_vm_status_mpp, 
       t1_vm_status_mprv, 
       t1_vm_status_sum, 
       t1_vm_status_mxr, 
       t1_vm_status_debug, 
       f0_icache_req_valid_arb,
       t0_f0_icache_req_valid, 
       t1_f0_icache_req_valid, 
       t0_buffer_cacheable, 
       t1_buffer_cacheable, 
       t0_f7_buffer_id,
       t1_f7_buffer_id,
       t0_f6_valid,
       t0_f6_buffer_wr, 
       t1_f6_valid, 
       t1_f6_buffer_wr, 
       t0_f6_icache_resp_cacheable,
       t1_f6_icache_resp_cacheable,
       t0_icache_inflight,
       t1_icache_inflight,
       t0_miss_pending,
       t1_miss_pending,
       //FE helper signals
       f5_icache_resp_miss_seen, 
       prev_icache_resp_was_uncacheable,
       prev_icache_resp_was_cacheable,
       first_t0_arb_req,
       first_t1_arb_req,
       consecutive_t0_arb_req,
       consecutive_t1_arb_req,
       first_t0_inst,
       first_t1_inst,
       consecutive_t0_inst,
       consecutive_t1_inst,
       icache_req_valid_hi_cycles,
       icache_req_valid_to_resp_cycles,
       icache_req_to_redir_cycles,
       uncacheable_inst_t0,
       uncacheable_inst_t1,
       t0_f6_bypass,
       t1_f6_bypass,
       t0_f7_bypass,
       t1_f7_bypass,
       t0_inst_from_bypass,
       t1_inst_from_bypass,
       icache_request_made,
       icache_ready_seen,
       replay_inst_t0,
       replay_inst_t1,
       t0_inst_uc_from_bypass,
       t1_inst_uc_from_bypass,
       inst_ready_lo_cycles,
       redir0_to_redir1_cycles,
       redir0_seen,
       stall_mon_buffer_empty_t0,
       stall_mon_buffer_empty_t1,
       fe_mon_id_inst_ready,
       fe_mon_id_inst_valid,
       fe_mon_id_inst_thread_id,
       fe_mon_f0_core_req_valid,
       fe_mon_id_inst_data_replay,
       fe_mon_ipi_with_pc,
       fe_mon_wb_xcpt,
       fe_mon_wb_csr_eret,  
       fe_mon_wb_csr_replay,
       fe_mon_wb_reg_replay
       
);

        assign t0_f6_bypass                  = t0_f6_valid & t0_f6_buffer_wr;
        assign t1_f6_bypass                  = t1_f6_valid & t1_f6_buffer_wr;

        assign    stall_mon_buffer_empty_t0      = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.buffer_empty;
        assign    stall_mon_buffer_empty_t1      = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.buffer_empty;

        assign    chicken_bit                   = minion_top.core.frontend.chicken_bit; 
        assign    f1_icache_req_ready           = minion_top.core.frontend.f1_icache_req_ready; 
        assign    f1_icache_req_valid           = minion_top.core.frontend.f1_icache_req_valid;
        assign    f5_icache_resp_valid          = minion_top.core.frontend.f5_icache_resp_valid; 
        assign    f5_icache_resp_miss           = minion_top.core.frontend.f5_icache_resp_miss; 
        assign    f6_icache_fill_done           = minion_top.core.frontend.f6_icache_fill_done; 
        assign    f5_icache_resp_cacheable      = minion_top.core.frontend.f5_icache_resp.cacheable; 
        assign    f5_icache_resp_access_fault   = minion_top.core.frontend.f5_icache_resp.access_fault; 
        assign    f5_icache_resp_page_fault     = minion_top.core.frontend.f5_icache_resp.page_fault; 


        assign    t0_vm_status_prv                  = minion_top.core.frontend.vm_status[0].prv; 
        assign    t0_vm_status_mpp                  = minion_top.core.frontend.vm_status[0].mpp; 
        assign    t0_vm_status_mprv                 = minion_top.core.frontend.vm_status[0].mprv; 
        assign    t0_vm_status_sum                  = minion_top.core.frontend.vm_status[0].sum; 
        assign    t0_vm_status_mxr                  = minion_top.core.frontend.vm_status[0].mxr; 
        assign    t0_vm_status_debug                = minion_top.core.frontend.vm_status[0].debug; 

        assign    t1_vm_status_prv                  = minion_top.core.frontend.vm_status[1].prv; 
        assign    t1_vm_status_mpp                  = minion_top.core.frontend.vm_status[1].mpp; 
        assign    t1_vm_status_mprv                 = minion_top.core.frontend.vm_status[1].mprv; 
        assign    t1_vm_status_sum                  = minion_top.core.frontend.vm_status[1].sum; 
        assign    t1_vm_status_mxr                  = minion_top.core.frontend.vm_status[1].mxr; 
        assign    t1_vm_status_debug                = minion_top.core.frontend.vm_status[1].debug; 


        assign    f0_icache_req_valid_arb       = minion_top.core.frontend.f0_icache_req_valid_arb;
        assign    t0_f0_icache_req_valid        = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f0_icache_req_valid; 
        assign    t1_f0_icache_req_valid        = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f0_icache_req_valid; 
        assign    t0_buffer_cacheable           = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.buffer_cacheable; 
        assign    t1_buffer_cacheable           = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.buffer_cacheable; 
        assign    t0_f7_buffer_id               = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f7_buffer_id;
        assign    t1_f7_buffer_id               = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f7_buffer_id;
        assign    t0_f6_valid                   = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f6_valid; 
        assign    t0_f6_buffer_wr               = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr; 
        assign    t1_f6_valid                   = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f6_valid; 
        assign    t1_f6_buffer_wr               = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f6_buffer_wr; 
        assign    t0_f6_icache_resp_cacheable   = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f6_icache_resp.cacheable;
        assign    t1_f6_icache_resp_cacheable   = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f6_icache_resp.cacheable;

        assign    t0_icache_inflight            = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.icache_inflight; 
        assign    t1_icache_inflight            = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.icache_inflight; 

        assign    t0_miss_pending               = minion_top.core.frontend.FRONTEND_THREAD[0].thread_buffer.f0_miss_pending; 
        assign    t1_miss_pending               = minion_top.core.frontend.FRONTEND_THREAD[1].thread_buffer.f0_miss_pending; 

        assign    fe_mon_id_inst_ready           = minion_top.core.frontend.id_inst_ready;
        assign    fe_mon_id_inst_valid           = minion_top.core.frontend.id_inst_valid;
        assign    fe_mon_id_inst_thread_id       = minion_top.core.frontend.id_inst_thread_id;
        assign    fe_mon_id_inst_data_replay     = minion_top.core.frontend.id_inst_data.replay;
        assign    fe_mon_f0_core_req_valid       = minion_top.core.frontend.f0_core_req_valid;
        assign    fe_mon_ipi_with_pc             = minion_top.core.intpipe.ipi_with_pc;
        assign    fe_mon_wb_xcpt                 = minion_top.core.intpipe.wb_xcpt;
        assign    fe_mon_wb_csr_eret             = minion_top.core.intpipe.wb_csr_eret;
        assign    fe_mon_wb_csr_replay           = minion_top.core.intpipe.wb_csr_replay;
        assign    fe_mon_wb_reg_replay           = minion_top.core.intpipe.wb_reg_replay;


        always @ (posedge clock) begin

	    if (redir0_seen === 1)
		redir0_to_redir1_cycles <= redir0_to_redir1_cycles + 1 ;

            if (fe_mon_f0_core_req_valid[0] === 1'b1) begin
	       redir0_seen <= 1;
	       redir0_to_redir1_cycles <= 0;
	    end

            if (fe_mon_f0_core_req_valid[1] === 1'b1) begin
	       redir0_seen <= 0;
	       redir0_to_redir1_cycles <= 0;
	    end

	    if (replay_inst_t0 === 1'b1 && fe_mon_f0_core_req_valid[0] === 1'b1 && fe_mon_wb_reg_replay === 1'b1) 
		replay_inst_t0 <= 0;

	    if (replay_inst_t1 === 1'b1 && fe_mon_f0_core_req_valid[1] === 1'b1 && fe_mon_wb_reg_replay === 1'b1) 
		replay_inst_t1 <= 0;

	    if (fe_mon_id_inst_ready === 0)
		inst_ready_lo_cycles <= inst_ready_lo_cycles + 1;	

	    if (fe_mon_id_inst_ready === 1)
		inst_ready_lo_cycles <= 0;	

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && fe_mon_id_inst_data_replay === 0) begin
		uncacheable_inst_t0 <= 0;
		uncacheable_inst_t1 <= 0;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && fe_mon_id_inst_data_replay === 1) begin
		uncacheable_inst_t0 <= 1;
		replay_inst_t0 <= 1;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && fe_mon_id_inst_data_replay === 0) begin
		uncacheable_inst_t1 <= 0;
		uncacheable_inst_t0 <= 0;
            end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && fe_mon_id_inst_data_replay === 1) begin
		uncacheable_inst_t1 <= 1;
		replay_inst_t1 <= 1;
            end


	    if (icache_request_made === 1)
		icache_req_to_redir_cycles <= icache_req_to_redir_cycles + 1 ;

            if (fe_mon_f0_core_req_valid[0] === 1'b1 || fe_mon_f0_core_req_valid[1] === 1'b1 ) begin
	        icache_request_made <= 0;	
		icache_req_to_redir_cycles <= 0;
 	    end 		
	    	
	    if (f1_icache_req_valid === 1 && f1_icache_req_ready === 0) begin
	        icache_request_made <= 0;	
		icache_req_to_redir_cycles <= 0;
 	    end 		
	    if (f1_icache_req_valid === 1 && f1_icache_req_ready === 1) begin
	        icache_request_made <= 1;	
		icache_req_to_redir_cycles <= 0;
 	    end 		

	    t0_f7_bypass <= t0_f6_bypass;	
	    t1_f7_bypass <= t1_f6_bypass;	

            if (f5_icache_resp_valid === 1'b0 && icache_ready_seen === 1'b1)
		icache_req_valid_to_resp_cycles <= icache_req_valid_to_resp_cycles + 1 ;

            if (f5_icache_resp_valid === 1'b1 && icache_ready_seen === 1'b1) begin
	        icache_ready_seen <= 0;	
                icache_req_valid_to_resp_cycles <= 0;
            end

	    if (f1_icache_req_valid === 0)
		icache_req_valid_hi_cycles <= 0;

	    if (f1_icache_req_valid === 1)
		icache_req_valid_hi_cycles <= icache_req_valid_hi_cycles + 1;	

	    if (f1_icache_req_ready === 1) begin
		icache_req_valid_hi_cycles <= 0;
	        icache_ready_seen <= 1;	
                icache_req_valid_to_resp_cycles <= 0;
            end
		
            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp_miss === 1'b1)
               f5_icache_resp_miss_seen <= 1'b1;
            if (f6_icache_fill_done === 1'b1) 
               f5_icache_resp_miss_seen <= 1'b0;
            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp_cacheable === 1'b1)begin
               prev_icache_resp_was_cacheable <= 1'b1;
               prev_icache_resp_was_uncacheable <= 1'b0;
            end
            if (f5_icache_resp_valid === 1'b1 && f5_icache_resp_cacheable === 1'b0)begin
               prev_icache_resp_was_cacheable <= 1'b0;
               prev_icache_resp_was_uncacheable <= 1'b1;
           end

            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 0 && first_t0_arb_req === 1'b1) begin
		consecutive_t0_arb_req <= consecutive_t0_arb_req + 1;
	    end
            if (t0_f0_icache_req_valid === 0 &&  t1_f0_icache_req_valid === 1 && first_t1_arb_req === 1'b1) begin
		consecutive_t1_arb_req <= consecutive_t1_arb_req + 1;
	    end
	
            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 0 && first_t0_arb_req === 1'b0) begin
		first_t0_arb_req <= 1'b1;
		first_t1_arb_req <= 1'b0;
		consecutive_t0_arb_req <= 0;
		consecutive_t1_arb_req <= 0;
	    end
            if (t0_f0_icache_req_valid === 0 &&  t1_f0_icache_req_valid === 1 && first_t1_arb_req === 1'b0) begin
		first_t1_arb_req <= 1'b1;
		first_t0_arb_req <= 1'b0;
		consecutive_t1_arb_req <= 0;
		consecutive_t0_arb_req <= 0;
	    end
            if (t0_f0_icache_req_valid === 1 &&  t1_f0_icache_req_valid === 1) begin
		first_t1_arb_req <= 1'b0;
		first_t0_arb_req <= 1'b0;
		consecutive_t0_arb_req <= 0;
		consecutive_t1_arb_req <= 0;
	    end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && first_t0_inst === 1'b1) begin
		consecutive_t0_inst = consecutive_t0_inst + 1;
	    end
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && first_t1_inst === 1'b1) begin
		consecutive_t1_inst <= consecutive_t1_inst + 1;
	    end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && first_t0_inst === 1'b0) begin
		first_t0_inst <= 1'b1;
		first_t1_inst <= 1'b0;
		consecutive_t0_inst <= 0;
		consecutive_t1_inst <= 0;
	    end	
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && first_t1_inst == 1'b0) begin
		first_t1_inst <= 1'b1;
		first_t0_inst <= 1'b0;
		consecutive_t0_inst <= 0;
		consecutive_t1_inst <= 0;
	    end	

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 0) begin
		t0_inst_from_bypass <= 0;
		t1_inst_from_bypass <= 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 1) begin
		t0_inst_from_bypass <= 1;
            end
		
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 0) begin
		t0_inst_from_bypass <= 0;
		t1_inst_from_bypass <= 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 1) begin
		t1_inst_from_bypass <= 1;
            end


            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 0 && fe_mon_id_inst_data_replay === 0) begin
		t0_inst_uc_from_bypass <= 0;
		t1_inst_uc_from_bypass <= 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 0 && t0_f7_bypass === 1 && fe_mon_id_inst_data_replay === 1) begin
		t0_inst_uc_from_bypass <= 1;
            end
		
            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 0 && fe_mon_id_inst_data_replay === 0) begin
		t0_inst_uc_from_bypass <= 0;
		t1_inst_uc_from_bypass <= 0;
            end

            if (fe_mon_id_inst_ready === 1'b1 && fe_mon_id_inst_valid === 1'b1 && fe_mon_id_inst_thread_id === 1 && t1_f7_bypass === 1 && fe_mon_id_inst_data_replay === 1) begin
		t1_inst_uc_from_bypass <= 1;
            end
	
    	    minion_frontend_cg_inst.sample();

	end
endmodule: minion_frontend_cvg

	bind minion_top minion_frontend_cvg minion_frontend_cov(.clock(clock));

`endif
`endif
