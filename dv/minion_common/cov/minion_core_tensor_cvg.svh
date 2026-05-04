`ifdef EVL_COVERAGE_ON

`ifndef __MINION_CORE_TENSOR_CVG
`define __MINION_CORE_TENSOR_CVG


//
// Note: search for keyword "FUTURE".
//



// ===========================================================
// Collecting coverage related to DCache
// ============================================================

class minion_core_tensor_cvg;

   covergroup cg_dcache_tensor_load with function sample(bit[9:0]    tensor_error_csr,
                                                         bit         tensor_error_en,
                                                         bit         tensor_load_en,
                                                         bit         tensor_load_start,
                                                         bit[3:0]    tensor_load_num_lines,
                                                         bit         tensor_load_coop,
                                                         bit         tensor_load_use_tmask,
                                                         logic[15:0] tensor_load_tmask,
                                                         logic[4:0]  tensor_load_tmask_sum,
                                                         bit         tensor_load_sel_setupB,
                                                         logic[1:0]  tensor_load_mem_err_flags);

      option.name = "cg_dcache_tensor_load";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // ------------- TensorLoad auxiliary cps for valid cfgs  -------------
      cp_dcache_tensor_load_num_lines : coverpoint tensor_load_num_lines iff (tensor_load_start) {
         bins tensor_load_1_line            = {4'h0};
         bins tensor_load_multiple_lines[2] = {[4'h1:4'he]};
         bins tensor_load_16_lines          = {4'hf};
      }

      cp_dcache_tensor_load_coop : coverpoint tensor_load_coop iff (tensor_load_start) {
         bins tensor_load_coop     = {1'h1};
         bins tensor_load_not_coop = {1'h0};
      }

      cp_dcache_tensor_load_use_tmask : coverpoint tensor_load_use_tmask iff (tensor_load_start) {
         bins tensor_load_tmask     = {1'h1};
         bins tensor_load_not_tmask = {1'h0};
      }

      cp_dcache_tensor_load_tmask_bits : coverpoint tensor_load_tmask_sum iff (tensor_load_start && tensor_load_use_tmask) { //there is an assumption that mask bits will be 1 for idxs larger than num lines
         bins tensor_load_tmask_nothing_masked = {0};
         bins tensor_load_tmask_mask_l[1]      = {[1:7]};
         bins tensor_load_tmask_mask_h[1]      = {[8:15]};
         bins tensor_load_tmask_fully_masked   = {16};
      }

      // ------------- FUTURE: TensorLoad valid cfg  -------------
      cp_dcache_tensor_load_configs_0_mask_en : cross cp_dcache_tensor_load_coop, cp_dcache_tensor_load_num_lines, cp_dcache_tensor_load_tmask_bits iff (!tensor_load_sel_setupB && tensor_load_start && tensor_load_use_tmask);

      cp_dcache_tensor_load_configs_0_mask_dis : cross cp_dcache_tensor_load_coop, cp_dcache_tensor_load_num_lines iff (!tensor_load_sel_setupB && tensor_load_start && !tensor_load_use_tmask);

      // --------- FUTURE: TensorLoadSetupB valid cfg ------------
      cp_dcache_tensor_load_configs_1 : cross cp_dcache_tensor_load_coop, cp_dcache_tensor_load_num_lines iff (tensor_load_sel_setupB && tensor_load_start);


      // --------- FUTURE: tensor load incorrect cfg ---------
      cp_dcache_tensor_load_wrong_cfg : coverpoint tensor_error_csr {
         wildcard bins tensor_load_wrong_trans_field = {10'b??_????_??1?} iff (tensor_error_en);
         wildcard bins tensor_load_L1_scp_disabled   = {10'b??_???1_????} iff (tensor_load_en);
      }

      // ------------ tensor load memory error ---------
      cp_dcache_tensor_load_mem_error : coverpoint tensor_load_mem_err_flags {
         wildcard bins tensor_load_mem_error_th0 = {2'b?1};
         wildcard bins tensor_load_mem_error_th1 = {2'b1?};
      }

   endgroup : cg_dcache_tensor_load


   covergroup cg_dcache_tensor_store with function sample(bit        tensor_store_from_vpu,
                                                          bit        tensor_store_from_scp,
                                                          bit        tensor_store_en,
                                                          bit[63:0]  tensor_store_csr_data,
                                                          bit[3:0]   tensor_reduce_fsm_state,
                                                          bit[9:0]   tensor_error_csr,
                                                          bit        tensor_error_en,
                                                          bit        tensor_fma_en,
                                                          logic[1:0] tensor_reduce_mem_err_flag);

      option.name = "cg_dcache_tensor_store";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // ------------- tensor store from vpu -------------
      cp_dcache_tensor_store_vpu : coverpoint tensor_store_from_vpu iff (tensor_reduce_fsm_state == 4'h8) {
         bins dcache_ts_vpu = {1'h1};
      }

      // ------------- tensor store from scp -------------
      cp_dcache_tensor_store_scp : coverpoint tensor_store_from_scp iff (tensor_reduce_fsm_state == 4'h8) {
         bins dcache_ts_scp = {1'h1};
      }

      // ----------- tensor store incorrect cfg ---------
      cp_dcache_tensor_store_wrong_cfg : coverpoint tensor_error_csr {
         wildcard bins tensor_store_vpu_invalid_cfg = {10'b?1_????_????} iff (tensor_error_en);
         wildcard bins tensor_store_L1_scp_disabled = {10'b??_???1_????} iff (tensor_store_en);
      }

      // ------------ FUTURE: tensor store tlb miss ---------
//    cp_dcache_tensor_store_tlb_miss : coverpoint ______ iff ____ {
//       bins _____;
//    }

      // ------------ tensor store memory error ---------
      cp_dcache_tensor_store_mem_error : coverpoint tensor_reduce_mem_err_flag {
         wildcard bins tensor_store_mem_error_th0 = {2'b?1};
         wildcard bins tensor_store_mem_error_th1 = {2'b1?};
      }

   endgroup : cg_dcache_tensor_store


   covergroup cg_dcache_tensor_reduce with function sample(bit[9:0]    tensor_error_csr,
                                                           bit         tensor_error_en,
                                                           bit         tensor_reduce_start,
                                                           logic[4:0]  tensor_reduce_start_reg,
                                                           logic[3:0]  tensor_reduce_op,
                                                           logic[7:0]  tensor_reduce_num_reg,
                                                           logic[1:0]  tensor_reduce_action,
                                                           logic[12:0] tensor_reduce_partner,
                                                           logic       handshake_is_done,
                                                           logic       receiver_is_first);

      option.name = "cg_dcache_tensor_reduce";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // ------------- TensorReduce auxiliary cps for valid cfgs  -------------
      cp_dcache_tensor_reduce_start_reg : coverpoint tensor_reduce_start_reg iff (tensor_reduce_start) {
         bins tensor_reduce_start_reg0   = {0};
         bins tensor_reduce_start_reg[2] = {[5'h1:5'h1e]};
         bins tensor_reduce_start_reg31  = {5'h1f};
      }

      cp_dcache_tensor_reduce_op : coverpoint tensor_reduce_op iff (tensor_reduce_start) {
         bins tensor_reduce_fadd  = {4'h0};
         bins tensor_reduce_fsub  = {4'h1};
         bins tensor_reduce_fmax  = {4'h2};
         bins tensor_reduce_fmin  = {4'h3};
         bins tensor_reduce_iadd  = {4'h4};
         bins tensor_reduce_isub  = {4'h5};
         bins tensor_reduce_imax  = {4'h6};
         bins tensor_reduce_imin  = {4'h7};
         bins tensor_reduce_fget  = {4'h8};
         bins tensor_reduce_undef = default;
      }

      cp_dcache_tensor_reduce_num_regs : coverpoint tensor_reduce_num_reg iff (tensor_reduce_start) {
         ignore_bins tensor_reduce_0_reg_undef = {8'h0};    // 0 regs leads to undefined behavior
                bins tensor_reduce_reg[4] = {[8'h1:8'h20]}; // 1 to 32 regs
      }

      cp_dcache_tensor_reduce_action : coverpoint tensor_reduce_action iff (tensor_reduce_start) {
         bins tensor_reduce_send     = {2'h0};
         bins tensor_reduce_receive  = {2'h1};
         bins tensor_reduce_auto_bdc = {2'h2};
         bins tensor_reduce_auto_rdc = {2'h3};
      }

      cp_dcache_tensor_reduce_partner_shire_id : coverpoint tensor_reduce_partner[12:5] iff (tensor_reduce_start && (tensor_reduce_action <= 1)) {
         // in current implementation there are 34 shires
                bins tensor_reduce_shire_id_to_32[4]  = {[8'h0:8'h1f]};
         ignore_bins tensor_reduce_shire_id_spare     = {[8'h20:8'h21]};
         ignore_bins tensor_reduce_shire_id_above_34  = {[8'h22:$]};
      }

      cp_dcache_tensor_reduce_neigh_id : coverpoint tensor_reduce_partner[4:3] iff (tensor_reduce_start && (tensor_reduce_action <= 1)) {
         bins tensor_reduce_neigh_id[4] = {[2'h0:2'h3]};
      }

      cp_dcache_tensor_reduce_minion_id : coverpoint tensor_reduce_partner[2:0] iff (tensor_reduce_start && (tensor_reduce_action <= 1)) {
         bins tensor_reduce_minion_id[8] = {[3'h0:3'h7]};
      }

      cp_dcache_tensor_reduce_tree_depth : coverpoint tensor_reduce_partner[3:0] iff (tensor_reduce_start && (tensor_reduce_action > 1)) {
         bins tensor_reduce_within_neigh[1]   = {[0:2]};
         bins tensor_reduce_within_shire[1]   = {[3:4]};
         bins tensor_reduce_between_shires[1] = {[5:$]};
      }

      // ------------- FUTURE: TensorReduce valid cfg  -------------
      cp_dcache_tensor_reduce_auto_cfg : cross cp_dcache_tensor_reduce_start_reg, cp_dcache_tensor_reduce_num_regs, cp_dcache_tensor_reduce_op, cp_dcache_tensor_reduce_tree_depth, cp_dcache_tensor_reduce_action {
         ignore_bins tensor_reduce_not_auto = cp_dcache_tensor_reduce_auto_cfg with (cp_dcache_tensor_reduce_action == 2'h0 || cp_dcache_tensor_reduce_action == 2'h1);
      }

      cp_dcache_tensor_reduce_send_rcv_cfg : cross cp_dcache_tensor_reduce_start_reg, cp_dcache_tensor_reduce_num_regs, cp_dcache_tensor_reduce_op, cp_dcache_tensor_reduce_partner_shire_id, cp_dcache_tensor_reduce_neigh_id, cp_dcache_tensor_reduce_minion_id, cp_dcache_tensor_reduce_action {
         ignore_bins tensor_reduce_auto = cp_dcache_tensor_reduce_send_rcv_cfg with (cp_dcache_tensor_reduce_action == 2'h2 || cp_dcache_tensor_reduce_action == 2'h3);
      }

      // ----------- TensorReduce incorrect cfg ------------
      cp_dcache_tensor_reduce_wrong_cfg : coverpoint tensor_error_csr iff (tensor_error_en) {
         wildcard bins tensor_reduce_invalid_cfg = {10'b1?_????_????};
      }

      // -------- TensorReduce partner minion ready ---------
      cp_dcache_tensor_reduce_ready : coverpoint receiver_is_first iff (handshake_is_done) {
         bins receiver_ready_before_sender = {1'b1};
         bins sender_ready_before_receiver = {1'b0};
      }

   endgroup : cg_dcache_tensor_reduce


   covergroup cg_dcache_pma with function sample(logic       valid_addr,
                                                 logic[39:0] paddr,
                                                 logic[6:0]  dcache_mem_region_permitted,
                                                 logic       paddr_io_region,
                                                 logic       paddr_scp_region,
                                                 logic       paddr_esr_region,
                                                 logic       paddr_pcie_region,
                                                 logic       paddr_mbox_region,
                                                 logic       paddr_os_region,
                                                 logic       paddr_dram_region,
                                                 logic[1:0]  dcache_dram_size,
                                                 logic[3:0]  dcache_mprot,
                                                 logic[1:0]  privilege_mode);

      option.name = "cg_dcache_pma";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // ------------- PMA -------------
      cp_mem_region_accesses: coverpoint dcache_mem_region_permitted iff (valid_addr) {
         wildcard bins dcache_req_io_accessed   = {7'b???_???1} iff (paddr_io_region);
         wildcard bins dcache_req_scp_accessed  = {7'b???_??1?} iff (paddr_scp_region);
         wildcard bins dcache_req_esr_accessed  = {7'b???_?1??} iff (paddr_esr_region);
         wildcard bins dcache_req_pcie_accessed = {7'b???_1???} iff (paddr_pcie_region);
         wildcard bins dcache_req_mbox_accessed = {7'b??1_????} iff (paddr_mbox_region);
         wildcard bins dcache_req_os_accessed   = {7'b?1?_????} iff (paddr_os_region);
         wildcard bins dcache_req_dram_accessed = {7'b1??_????} iff (paddr_dram_region);
      }

      cp_mprot_pma: coverpoint dcache_mprot {
         bins dcache_os_pcie_enabled_iou  = {4'b0000};
         bins dcache_os_pcie_enabled_ios  = {4'b0001};
         bins dcache_os_pcie_enabled_iof  = {4'b0010};
         bins dcache_os_pcie_enabled_iom  = {4'b0011};
         bins dcache_os_enabled_iou       = {4'b0100};
         bins dcache_os_enabled_ios       = {4'b0101};
         bins dcache_os_enabled_iof       = {4'b0110};
         bins dcache_os_enabled_iom       = {4'b0111};
         bins dcache_pcie_enabled_iou     = {4'b1000};
         bins dcache_pcie_enabled_ios     = {4'b1001};
         bins dcache_pcie_enabled_iof     = {4'b1010};
         bins dcache_pcie_enabled_iom     = {4'b1011};
         bins dcache_os_pcie_disabled_iou = {4'b1100};
         bins dcache_os_pcie_disabled_ios = {4'b1101};
         bins dcache_os_pcie_disabled_iof = {4'b1110};
         bins dcache_os_pcie_disabled_iom = {4'b1111};
      }

      cp_dram_size_pma: coverpoint dcache_dram_size {
         bins dcache_dram_size_8GB  = {2'h0};
         bins dcache_dram_size_16GB = {2'h1};
         bins dcache_dram_size_24GB = {2'h2};
         bins dcache_dram_size_32GB = {2'h3};
      }

      cp_priv_pma: coverpoint privilege_mode {
                bins u_mode_priv   = {2'h0};
                bins s_mode_priv   = {2'h1};
         ignore_bins reserved_priv = {2'h2};
                bins m_mode_priv   = {2'h3};
      }

      // ------------- mem regions vs. mprot vs. priv -------------
      cross_region_mprot_priv : cross cp_mem_region_accesses, cp_mprot_pma, cp_priv_pma iff (valid_addr) {
         ignore_bins io_when_disabled   = cross_region_mprot_priv with ((cp_mprot_pma == 4'b??10) && (cp_mem_region_accesses == 7'b???_???1));
         ignore_bins io_not_m_mode      = cross_region_mprot_priv with ((cp_priv_pma < 3) && (cp_mprot_pma == 4'b??11) && (cp_mem_region_accesses == 7'b???_???1));
         ignore_bins io_not_s_mode      = cross_region_mprot_priv with ((cp_priv_pma < 1) && (cp_mprot_pma == 4'b??01) && (cp_mem_region_accesses == 7'b???_???1));
         ignore_bins pcie_when_disabled = cross_region_mprot_priv with ((cp_mprot_pma == 4'b?1??) && (cp_mem_region_accesses == 7'b???_1???));
         ignore_bins os_when_disabled   = cross_region_mprot_priv with ((cp_mprot_pma == 4'b1???) && (cp_mem_region_accesses == 7'b?1?_????));
         ignore_bins mbox_not_m_mode    = cross_region_mprot_priv with ((cp_priv_pma < 3) && (cp_mem_region_accesses == 7'b??1_????)) ;
      }

   endgroup : cg_dcache_pma


   covergroup cg_tensor_seq with function sample(bit [5:0] tensor_seq);

      option.name = "cg_tensor_seq";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      tensor_sequences_cov : coverpoint tensor_seq {
             ignore_bins b_ignore = {[0:7]};
             bins b_tensor_seq[] = {[8:62]};
      }

   endgroup : cg_tensor_seq

   // -----------------------------------
   //     Covergroup instances
   // -----------------------------------
   function new();
     cg_dcache_tensor_load   = new();
     cg_dcache_tensor_store  = new();
     cg_dcache_tensor_reduce = new();
     cg_dcache_pma           = new();
     cg_tensor_seq           = new();
   endfunction : new

endclass: minion_core_tensor_cvg

`endif
`endif
