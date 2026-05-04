`ifdef EVL_COVERAGE_ON

`ifndef __NEIGH_VPU_CVG
`define __NEIGH_VPU_CVG

package neigh_vpu_cov_pkg;


   covergroup minion_vpu_cg (   
        // VPU coverage signals
	// changed version 2
        ref bit [4:0] state6_id,
        ref bit [4:0] state5_id,
        ref bit [4:0] state4_id,
        ref bit [4:0] state3_id,
        ref bit [4:0] state2_id,
        ref bit [4:0] state1_id,
        ref bit [4:0] state0_id,
        ref bit [4:0] push_state_id,
        ref bit vpu_ctrl_ex_core_kill,

        ref bit [3:0] id_fp_sboard_bits,
        ref bit [3:0] id_fp_hazard_en,
        ref bit [2:0] id_reg_gsc_cnt,
        ref bit id_ctrl_gsc,
	ref bit [7:0] id_mask_sboard_raddr_bits,
	ref bit [7:0] id_mask_sboard_waddr_bits,
        ref bit id_vpu_ctrl_tfma_wrrf_enabled,
        ref bit vpu_ctrl_tensorfma_start,
        ref bit id_vpu_ctrl_tquant_enabled,
        ref bit vpu_ctrl_tensorquant_start,
        ref bit id_vpu_ctrl_reduce_enabled,

        ref bit id_valid_qual,
        ref bit id_thread_id,
        ref bit id_csr_wr_safe,
        ref bit id_ctrl_to_ex_mem,
        ref bit id_ctrl_to_ex_fp,
        ref bit id_ctrl_to_ex_wfd,
        ref bit id_ctrl_to_ex_gsc,
        ref bit id_ctrl_to_ex_maskop,
        ref bit id_ctrl_to_ex_trans,

	ref bit ex_csr_vpu,
	ref bit tag_csr_vpu,
	ref bit mem_csr_vpu,
	ref bit wb_csr_vpu,

        ref bit vpu_ctrl_f2_core_kill,
        ref bit vpu_ctrl_f3_core_kill,
        ref bit trans_f6_killed,
        ref bit trans_f7_killed,
        ref bit trans_f8_killed,
        ref bit vpu_ctrl_mask_f2_maskop,
        ref bit vpu_ctrl_mask_f3_maskop = 0,
        ref bit vpu_ctrl_f2_shsw,
        ref bit vpu_ctrl_f2_txfma,
        ref bit [2:0] vpu_ctrl_mask_f2_regmask_waddr,
        ref bit [2:0] vpu_ctrl_mask_f4_regmask_waddr,
        ref bit [2:0] vpu_ctrl_mask_f5_regmask_waddr,
        ref bit [2:0] vpu_ctrl_mask_f6_regmask_waddr,
        ref bit [2:0] vpu_ctrl_mask_f7_regmask_waddr,
        ref bit vpu_ctrl_mask_f3_ctrl_valid = 0,
        ref bit vpu_ctrl_f2_thread_id,
        ref bit vpu_ctrl_ex_ctrl_valid_qual,
        ref bit vpu_ctrl_ex_thread_id,
        ref bit vpu_ctrl_ex_req_sigs_maskop,
        ref bit vpu_ctrl_ex_req_sigs_m0ren,
        ref bit vpu_ctrl_ex_req_sigs_mren1,
        ref bit vpu_ctrl_ex_req_sigs_mren2,
        ref bit [4:0] vpu_ctrl_mask_ex_regfile_raddr1,
        ref bit [4:0] vpu_ctrl_mask_ex_regfile_raddr2,
        ref bit vpu_ctrl_ex_req_sigs_vector,
        ref bit vpu_ctrl_ex_req_sigs_shsw,
        ref bit vpu_ctrl_ex_req_sigs_txfma,
        ref bit vpu_ctrl_mask_f2_regmask_store_all,
        ref bit [6:0] vpu_ctrl_mask_f2_cmd,
        ref bit [6:0] vpu_ctrl_mask_f3_cmd = 0,
        ref bit vpu_ctrl_mask_f3_regmask_from_short,
        ref bit [2:0] vpu_ctrl_mask_f3_regmask_waddr,
        ref bit [7:0] vpu_ctrl_mask_f3_regmask_store,
        ref bit [7:0] vpu_ctrl_mask_f2_regmask_store_to_mask,
        ref bit [7:0] vpu_ctrl_mask_f4_regmask_store,
        ref bit [7:0] vpu_ctrl_mask_f5_regmask_store,
        ref bit [7:0] vpu_ctrl_mask_f6_regmask_store,
        ref bit [7:0] vpu_ctrl_mask_f7_regmask_store,
        ref bit [7:0] vpu_ctrl_mask_f8_regmask_store,
        ref bit vpu_ctrl_f3_thread_id,
        ref bit vpu_ctrl_f8_regmask_from_txfma,
        ref bit [2:0] vpu_ctrl_mask_f8_regmask_waddr,
        ref bit [7:0] vpu_ctrl_mask_f8_regmask_store_sel,
        ref bit vpu_ctrl_f8_thread_id,
        ref bit [4:0] vpu_ctrl_ex_regfile_raddr1,
        ref bit [4:0] vpu_ctrl_ex_regfile_raddr2,
        ref bit [4:0] vpu_ctrl_ex_regfile_raddr3,
        ref bit [4:0] vpu_ctrl_ex_regfile_waddr,
        ref bit vpu_ctrl_ex_req_sigs_ren1 ,
        ref bit vpu_ctrl_ex_req_sigs_ren2 ,
        ref bit vpu_ctrl_ex_req_sigs_ren3,
        ref bit vpu_ctrl_f4_ctrl_valid_qual,
        ref bit vpu_ctrl_f4_regfile_thrid_l ,
        ref bit [4:0] vpu_ctrl_f4_regfile_waddr_l,
        ref bit [7:0] vpu_ctrl_f4_regfile_wmask_l,
        ref bit vpu_ctrl_f3_ctrl_valid_qual,
        ref bit [4:0] vpu_ctrl_f3_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f3_regfile_wmask,
        ref bit [4:0] vpu_ctrl_f4_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f4_regfile_wmask,
        ref bit vpu_ctrl_f4_thread_id,
        ref bit vpu_ctrl_f5_ctrl_valid_qual,
        ref bit [4:0] vpu_ctrl_f5_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f5_regfile_wmask,
        ref bit vpu_ctrl_f5_thread_id,
        ref bit vpu_ctrl_f6_ctrl_valid_qual,
        ref bit [4:0] vpu_ctrl_f6_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f6_regfile_wmask,
        ref bit vpu_ctrl_f6_thread_id,
        ref bit vpu_ctrl_f7_ctrl_valid_qual,
        ref bit [4:0] vpu_ctrl_f7_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f7_regfile_wmask,
        ref bit vpu_ctrl_f7_thread_id,
        ref bit [4:0] vpu_ctrl_f8_regfile_waddr,
        ref bit [7:0] vpu_ctrl_f8_regfile_wmask,
        ref bit vpu_ctrl_f8_ctrl_valid_qual ,
        ref bit vpu_ctrl_f8_ctrl_sigs_txfma,

        // tensor reduce
        ref bit vpu_ctrl_reduce_start,
        ref bit [4:0] vpu_ctrl_reduce_start_reg,
        ref bit [6:0] vpu_ctrl_reduce_num_regs,
        ref bit [3:0] vpu_ctrl_reduce_op,        
     
        // VPU helper signals
        ref bit vpu_ctrl_ex_raddr1_matches_f3sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f4sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f4ld_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f5sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f6sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f7sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f8sw_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f8txfma_t0 ,
        ref bit vpu_ctrl_ex_raddr1_matches_f3sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f4sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f4ld_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f5sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f6sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f7sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f8sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff ,
     
        ref bit vpu_ctrl_ex_raddr2_matches_f3sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f4sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f4ld_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f5sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f6sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f7sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f8sw_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f8txfma_t0 ,
        ref bit vpu_ctrl_ex_raddr2_matches_f3sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f4sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f4ld_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f5sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f6sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f7sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f8sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff ,
     
        ref bit vpu_ctrl_ex_raddr3_matches_f3sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f4sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f4ld_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f5sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f6sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f7sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f8sw_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f8txfma_t0 ,
        ref bit vpu_ctrl_ex_raddr3_matches_f3sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f4sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f4ld_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f5sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f6sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f7sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f8sw_tdiff ,
        ref bit vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff ,

        ref bit vpu_ctrl_ex_waddr_matches_f3sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f4sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f4ld_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f5sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f6sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f7sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f8sw_tdiff ,
        ref bit vpu_ctrl_ex_waddr_matches_f8txfma_tdiff ,

	ref bit id_vpu_stall_fp_sb ,
	ref bit id_vpu_stall_rd_scoreboard,
	ref bit id_vpu_stall_wr_scoreboard,
	ref bit id_vpu_stall_tensor_fma,
	ref bit id_vpu_stall_tensor_quant,
	ref bit id_vpu_stall_tensor_reduce,
	ref bit id_vpu_stall_tensor_going_to_vpu,

	ref bit id_vpu_stall_fp_sb_d ,
	ref bit id_vpu_stall_fp_sb_event00 ,
	ref bit id_vpu_stall_fp_sb_event01 ,
	ref bit id_vpu_stall_fp_sb_event10 ,
	ref bit id_vpu_stall_fp_sb_event11 ,

	ref bit id_vpu_stall_rd_scoreboard_d,
	ref bit id_vpu_stall_rd_event00 ,
	ref bit id_vpu_stall_rd_event01 ,
	ref bit id_vpu_stall_rd_event10 ,
	ref bit id_vpu_stall_rd_event11 ,

	ref bit id_vpu_stall_wr_scoreboard_d,
	ref bit id_vpu_stall_wr_event00 ,
	ref bit id_vpu_stall_wr_event01 ,
	ref bit id_vpu_stall_wr_event10 ,
	ref bit id_vpu_stall_wr_event11 ,

	ref bit id_vpu_stall_tensor_fma_d,
	ref bit id_vpu_stall_tensor_fma_event00,
	ref bit id_vpu_stall_tensor_fma_event01,
	ref bit id_vpu_stall_tensor_fma_event10,
	ref bit id_vpu_stall_tensor_fma_event11,

	ref bit id_vpu_stall_tensor_quant_d,
	ref bit id_vpu_stall_tensor_quant_event00,
	ref bit id_vpu_stall_tensor_quant_event01,
	ref bit id_vpu_stall_tensor_quant_event10,
	ref bit id_vpu_stall_tensor_quant_event11,

	ref bit id_vpu_stall_tensor_reduce_d,
	ref bit id_vpu_stall_tensor_reduce_event00,
	ref bit id_vpu_stall_tensor_reduce_event01,
	ref bit id_vpu_stall_tensor_reduce_event10,
	ref bit id_vpu_stall_tensor_reduce_event11,

	ref bit id_vpu_stall_tensor_going_to_vpu_d,
	ref bit id_vpu_stall_tensor_going_to_vpu_event00,
	ref bit id_vpu_stall_tensor_going_to_vpu_event01,
	ref bit id_vpu_stall_tensor_going_to_vpu_event10,
	ref bit id_vpu_stall_tensor_going_to_vpu_event11,

        ref bit id_valid_qual_d,
        ref bit id_thread_id_d,
        ref bit id_csr_wr_safe_d,
        ref bit id_ctrl_to_ex_mem_d,
        ref bit id_ctrl_to_ex_fp_d,
        ref bit id_ctrl_to_ex_wfd_d,
        ref bit id_ctrl_to_ex_gsc_d,
        ref bit id_ctrl_to_ex_maskop_d,
        ref bit id_ctrl_to_ex_trans_d
);
         option.name = "minion_vpu_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;

//-------------------------------------------------------------------------------------------------------------------------------------------


   id_vpu_stall_fp_sb_cp:                    coverpoint id_vpu_stall_fp_sb_d;
   id_vpu_stall_rd_scoreboard_cp:            coverpoint id_vpu_stall_rd_scoreboard_d;
   id_vpu_stall_wr_scoreboard_cp:            coverpoint id_vpu_stall_wr_scoreboard_d;
   id_vpu_stall_tensor_fma_cp:               coverpoint id_vpu_stall_tensor_fma_d;


   id_vpu_stall_tensor_quant_cp:             coverpoint id_vpu_stall_tensor_quant_d;
   id_vpu_stall_tensor_reduce_cp:            coverpoint id_vpu_stall_tensor_reduce_d;
   id_vpu_stall_tensor_going_to_vpu_cp:      coverpoint id_vpu_stall_tensor_going_to_vpu_d;


   id_valid_qual_cp:			     coverpoint   id_valid_qual_d;
   id_csr_wr_safe_cp:                        coverpoint   id_csr_wr_safe_d;
   id_ctrl_to_ex_ld_cp:                      coverpoint   id_ctrl_to_ex_mem_d iff (id_ctrl_to_ex_wfd_d === 1);
   id_ctrl_to_ex_fp_cp:                      coverpoint   id_ctrl_to_ex_fp_d;        
   id_ctrl_to_ex_st_cp:                      coverpoint   id_ctrl_to_ex_mem_d iff (id_ctrl_to_ex_wfd_d === 0);        
   id_ctrl_to_ex_gsc_cp:                     coverpoint   id_ctrl_to_ex_gsc_d;        
   id_ctrl_to_ex_maskop_cp:		     coverpoint   id_ctrl_to_ex_maskop_d;
   id_ctrl_to_ex_trans_cp:		     coverpoint	  id_ctrl_to_ex_trans_d;	

   id_thread_id_cp:                          coverpoint   id_thread_id_d;
	 
	 
   id_vpu_stall_fp_sb_event_00:              coverpoint id_vpu_stall_fp_sb_event00;
   id_vpu_stall_fp_sb_event_01:              coverpoint id_vpu_stall_fp_sb_event01;
   id_vpu_stall_fp_sb_event_10:              coverpoint id_vpu_stall_fp_sb_event10;
   id_vpu_stall_fp_sb_event_11:              coverpoint id_vpu_stall_fp_sb_event11;

   id_vpu_stall_rd_event_00:                 coverpoint id_vpu_stall_rd_event00;
   id_vpu_stall_rd_event_01:                 coverpoint id_vpu_stall_rd_event01;
//   id_vpu_stall_rd_event_10:                 coverpoint id_vpu_stall_rd_event10;
//   id_vpu_stall_rd_event_11:                 coverpoint id_vpu_stall_rd_event11;

   id_vpu_stall_wr_event_00:                 coverpoint id_vpu_stall_wr_event00;
   id_vpu_stall_wr_event_01:                 coverpoint id_vpu_stall_wr_event01;
//   id_vpu_stall_wr_event_10:                 coverpoint id_vpu_stall_wr_event10;
//   id_vpu_stall_wr_event_11:                 coverpoint id_vpu_stall_wr_event11;

   id_vpu_stall_tensor_fma_event_00:         coverpoint id_vpu_stall_tensor_fma_event00;
   id_vpu_stall_tensor_fma_event_01:         coverpoint id_vpu_stall_tensor_fma_event01;
   id_vpu_stall_tensor_fma_event_10:         coverpoint id_vpu_stall_tensor_fma_event10;
   id_vpu_stall_tensor_fma_event_11:         coverpoint id_vpu_stall_tensor_fma_event11;

   id_vpu_stall_tensor_quant_event_00:       coverpoint id_vpu_stall_tensor_quant_event00;
   id_vpu_stall_tensor_quant_event_01:       coverpoint id_vpu_stall_tensor_quant_event01;
   id_vpu_stall_tensor_quant_event_10:       coverpoint id_vpu_stall_tensor_quant_event10;
   id_vpu_stall_tensor_quant_event_11:       coverpoint id_vpu_stall_tensor_quant_event11;

   id_vpu_stall_tensor_reduce_event_00:      coverpoint id_vpu_stall_tensor_reduce_event00;
   id_vpu_stall_tensor_reduce_event_01:      coverpoint id_vpu_stall_tensor_reduce_event01;
   id_vpu_stall_tensor_reduce_event_10:      coverpoint id_vpu_stall_tensor_reduce_event10;
   id_vpu_stall_tensor_reduce_event_11:      coverpoint id_vpu_stall_tensor_reduce_event11;

   id_vpu_stall_tensor_going_to_vpu_event_00: coverpoint id_vpu_stall_tensor_going_to_vpu_event00;
   id_vpu_stall_tensor_going_to_vpu_event_01: coverpoint id_vpu_stall_tensor_going_to_vpu_event01;
   id_vpu_stall_tensor_going_to_vpu_event_10: coverpoint id_vpu_stall_tensor_going_to_vpu_event10;
   id_vpu_stall_tensor_going_to_vpu_event_11: coverpoint id_vpu_stall_tensor_going_to_vpu_event11;

   cross_fpsb00_csr_cvg : cross  id_vpu_stall_fp_sb_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_fpsb01_csr_cvg : cross  id_vpu_stall_fp_sb_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_fpsb10_csr_cvg : cross  id_vpu_stall_fp_sb_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_fpsb11_csr_cvg : cross  id_vpu_stall_fp_sb_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_fpsb00_trans_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_fpsb01_trans_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_fpsb10_trans_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_fpsb11_trans_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_fpsb00_maskop_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_fpsb01_maskop_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_fpsb10_maskop_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_fpsb11_maskop_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_fpsb00_gsc_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_fpsb01_gsc_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_fpsb10_gsc_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_fpsb11_gsc_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_fpsb00_fp_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_fpsb01_fp_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_fpsb10_fp_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_fpsb11_fp_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_fpsb00_ld_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_fpsb01_ld_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_fpsb10_ld_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_fpsb11_ld_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_fpsb00_st_cvg : cross  id_vpu_stall_fp_sb_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_fpsb01_st_cvg : cross  id_vpu_stall_fp_sb_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_fpsb10_st_cvg : cross  id_vpu_stall_fp_sb_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_fpsb11_st_cvg : cross  id_vpu_stall_fp_sb_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_rd00_csr_cvg : cross     id_vpu_stall_rd_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_rd01_csr_cvg : cross     id_vpu_stall_rd_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
//   cross_rd10_csr_cvg : cross     id_vpu_stall_rd_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
//   cross_rd11_csr_cvg : cross     id_vpu_stall_rd_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_rd00_trans_cvg : cross   id_vpu_stall_rd_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_rd01_trans_cvg : cross   id_vpu_stall_rd_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
//   cross_rd10_trans_cvg : cross   id_vpu_stall_rd_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
//   cross_rd11_trans_cvg : cross   id_vpu_stall_rd_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_rd00_maskop_cvg : cross  id_vpu_stall_rd_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_rd01_maskop_cvg : cross  id_vpu_stall_rd_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
//   cross_rd10_maskop_cvg : cross  id_vpu_stall_rd_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
//   cross_rd11_maskop_cvg : cross  id_vpu_stall_rd_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_rd00_gsc_cvg : cross     id_vpu_stall_rd_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_rd01_gsc_cvg : cross     id_vpu_stall_rd_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
//   cross_rd10_gsc_cvg : cross     id_vpu_stall_rd_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
//   cross_rd11_gsc_cvg : cross     id_vpu_stall_rd_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_rd00_fp_cvg : cross      id_vpu_stall_rd_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_rd01_fp_cvg : cross      id_vpu_stall_rd_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
//   cross_rd10_fp_cvg : cross      id_vpu_stall_rd_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
//   cross_rd11_fp_cvg : cross      id_vpu_stall_rd_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_rd00_ld_cvg : cross      id_vpu_stall_rd_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_rd01_ld_cvg : cross      id_vpu_stall_rd_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
//   cross_rd10_ld_cvg : cross      id_vpu_stall_rd_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
//   cross_rd11_ld_cvg : cross      id_vpu_stall_rd_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_rd00_st_cvg : cross      id_vpu_stall_rd_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_rd01_st_cvg : cross      id_vpu_stall_rd_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
//   cross_rd10_st_cvg : cross      id_vpu_stall_rd_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
//   cross_rd11_st_cvg : cross      id_vpu_stall_rd_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_wr00_csr_cvg : cross     id_vpu_stall_wr_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_wr01_csr_cvg : cross     id_vpu_stall_wr_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
//   cross_wr10_csr_cvg : cross     id_vpu_stall_wr_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
//   cross_wr11_csr_cvg : cross     id_vpu_stall_wr_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_wr00_trans_cvg : cross   id_vpu_stall_wr_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_wr01_trans_cvg : cross   id_vpu_stall_wr_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
//   cross_wr10_trans_cvg : cross   id_vpu_stall_wr_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
//   cross_wr11_trans_cvg : cross   id_vpu_stall_wr_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_wr00_maskop_cvg : cross  id_vpu_stall_wr_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_wr01_maskop_cvg : cross  id_vpu_stall_wr_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
//   cross_wr10_maskop_cvg : cross  id_vpu_stall_wr_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
//   cross_wr11_maskop_cvg : cross  id_vpu_stall_wr_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_wr00_gsc_cvg : cross     id_vpu_stall_wr_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_wr01_gsc_cvg : cross     id_vpu_stall_wr_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
//   cross_wr10_gsc_cvg : cross     id_vpu_stall_wr_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
//   cross_wr11_gsc_cvg : cross     id_vpu_stall_wr_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_wr00_fp_cvg : cross      id_vpu_stall_wr_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_wr01_fp_cvg : cross      id_vpu_stall_wr_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
//   cross_wr10_fp_cvg : cross      id_vpu_stall_wr_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
//   cross_wr11_fp_cvg : cross      id_vpu_stall_wr_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_wr00_ld_cvg : cross      id_vpu_stall_wr_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_wr01_ld_cvg : cross      id_vpu_stall_wr_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
//   cross_wr10_ld_cvg : cross      id_vpu_stall_wr_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
//   cross_wr11_ld_cvg : cross      id_vpu_stall_wr_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_wr00_st_cvg : cross      id_vpu_stall_wr_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_wr01_st_cvg : cross      id_vpu_stall_wr_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
//   cross_wr10_st_cvg : cross      id_vpu_stall_wr_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
//   cross_wr11_st_cvg : cross      id_vpu_stall_wr_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_tfma00_csr_cvg : cross     id_vpu_stall_tensor_fma_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tfma01_csr_cvg : cross     id_vpu_stall_tensor_fma_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tfma10_csr_cvg : cross     id_vpu_stall_tensor_fma_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tfma11_csr_cvg : cross     id_vpu_stall_tensor_fma_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_tfma00_trans_cvg : cross   id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tfma01_trans_cvg : cross   id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tfma10_trans_cvg : cross   id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tfma11_trans_cvg : cross   id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_tfma00_maskop_cvg : cross  id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tfma01_maskop_cvg : cross  id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tfma10_maskop_cvg : cross  id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tfma11_maskop_cvg : cross  id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_tfma00_gsc_cvg : cross     id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tfma01_gsc_cvg : cross     id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tfma10_gsc_cvg : cross     id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tfma11_gsc_cvg : cross     id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_tfma00_fp_cvg : cross      id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tfma01_fp_cvg : cross      id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tfma10_fp_cvg : cross      id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tfma11_fp_cvg : cross      id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_tfma00_ld_cvg : cross      id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tfma01_ld_cvg : cross      id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tfma10_ld_cvg : cross      id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tfma11_ld_cvg : cross      id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_tfma00_st_cvg : cross      id_vpu_stall_tensor_fma_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tfma01_st_cvg : cross      id_vpu_stall_tensor_fma_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tfma10_st_cvg : cross      id_vpu_stall_tensor_fma_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tfma11_st_cvg : cross      id_vpu_stall_tensor_fma_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_tquant00_csr_cvg : cross     id_vpu_stall_tensor_quant_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tquant01_csr_cvg : cross     id_vpu_stall_tensor_quant_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tquant10_csr_cvg : cross     id_vpu_stall_tensor_quant_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tquant11_csr_cvg : cross     id_vpu_stall_tensor_quant_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_tquant00_trans_cvg : cross   id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tquant01_trans_cvg : cross   id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tquant10_trans_cvg : cross   id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tquant11_trans_cvg : cross   id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_tquant00_maskop_cvg : cross  id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tquant01_maskop_cvg : cross  id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tquant10_maskop_cvg : cross  id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tquant11_maskop_cvg : cross  id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_tquant00_gsc_cvg : cross     id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tquant01_gsc_cvg : cross     id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tquant10_gsc_cvg : cross     id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tquant11_gsc_cvg : cross     id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_tquant00_fp_cvg : cross      id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tquant01_fp_cvg : cross      id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tquant10_fp_cvg : cross      id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tquant11_fp_cvg : cross      id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_tquant00_ld_cvg : cross      id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tquant01_ld_cvg : cross      id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tquant10_ld_cvg : cross      id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tquant11_ld_cvg : cross      id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_tquant00_st_cvg : cross      id_vpu_stall_tensor_quant_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tquant01_st_cvg : cross      id_vpu_stall_tensor_quant_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tquant10_st_cvg : cross      id_vpu_stall_tensor_quant_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tquant11_st_cvg : cross      id_vpu_stall_tensor_quant_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_treduce00_csr_cvg : cross     id_vpu_stall_tensor_reduce_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_treduce01_csr_cvg : cross     id_vpu_stall_tensor_reduce_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_treduce10_csr_cvg : cross     id_vpu_stall_tensor_reduce_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_treduce11_csr_cvg : cross     id_vpu_stall_tensor_reduce_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_treduce00_trans_cvg : cross   id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_treduce01_trans_cvg : cross   id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_treduce10_trans_cvg : cross   id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_treduce11_trans_cvg : cross   id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_treduce00_maskop_cvg : cross  id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_treduce01_maskop_cvg : cross  id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_treduce10_maskop_cvg : cross  id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_treduce11_maskop_cvg : cross  id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_treduce00_gsc_cvg : cross     id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_treduce01_gsc_cvg : cross     id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_treduce10_gsc_cvg : cross     id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_treduce11_gsc_cvg : cross     id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_treduce00_fp_cvg : cross      id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_treduce01_fp_cvg : cross      id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_treduce10_fp_cvg : cross      id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_treduce11_fp_cvg : cross      id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_treduce00_ld_cvg : cross      id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_treduce01_ld_cvg : cross      id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_treduce10_ld_cvg : cross      id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_treduce11_ld_cvg : cross      id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_treduce00_st_cvg : cross      id_vpu_stall_tensor_reduce_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_treduce01_st_cvg : cross      id_vpu_stall_tensor_reduce_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_treduce10_st_cvg : cross      id_vpu_stall_tensor_reduce_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_treduce11_st_cvg : cross      id_vpu_stall_tensor_reduce_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_csr_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_00, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_csr_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_01, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_csr_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_10, id_csr_wr_safe_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_csr_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_11, id_csr_wr_safe_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_trans_cvg : cross   id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_trans_cvg : cross   id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_trans_cvg : cross   id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_trans_cvg : cross   id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_trans_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_maskop_cvg : cross  id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_maskop_cvg : cross  id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_maskop_cvg : cross  id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_maskop_cvg : cross  id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_maskop_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_gsc_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_gsc_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_gsc_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_gsc_cvg : cross     id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_gsc_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_fp_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_fp_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_fp_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_fp_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_fp_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_ld_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_ld_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_ld_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_ld_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_ld_cp, id_thread_id_cp ;

   cross_tgoing_to_vpu00_st_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_00, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu01_st_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_01, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu10_st_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_10, id_ctrl_to_ex_st_cp, id_thread_id_cp ;
   cross_tgoing_to_vpu11_st_cvg : cross      id_vpu_stall_tensor_going_to_vpu_event_11, id_ctrl_to_ex_st_cp, id_thread_id_cp ;



//-------------------------------------------------------------------------------------------------------------------------------------------

   vpu_ctrl_mask_thread_corruption_f2f3:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f3_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f3_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f3_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f2f4:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f4_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f4_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f4_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f2f5:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f5_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f5_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f5_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f2f6:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f6_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f6_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f6_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f2f7:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f7_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f7_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f7_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f2f8:         coverpoint  vpu_ctrl_f2_thread_id         iff ( (vpu_ctrl_f2_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f2_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f2_regmask_store_to_mask === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

   vpu_ctrl_mask_thread_corruption_f3f4:         coverpoint  vpu_ctrl_f3_thread_id         iff ( (vpu_ctrl_f3_thread_id != vpu_ctrl_f4_thread_id) && (vpu_ctrl_mask_f3_regmask_waddr === vpu_ctrl_mask_f4_regmask_waddr) && (|vpu_ctrl_mask_f3_regmask_store === 1) && (|vpu_ctrl_mask_f4_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f3f5:         coverpoint  vpu_ctrl_f3_thread_id         iff ( (vpu_ctrl_f3_thread_id != vpu_ctrl_f5_thread_id) && (vpu_ctrl_mask_f3_regmask_waddr === vpu_ctrl_mask_f5_regmask_waddr) && (|vpu_ctrl_mask_f3_regmask_store === 1) && (|vpu_ctrl_mask_f5_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f3f6:         coverpoint  vpu_ctrl_f3_thread_id         iff ( (vpu_ctrl_f3_thread_id != vpu_ctrl_f6_thread_id) && (vpu_ctrl_mask_f3_regmask_waddr === vpu_ctrl_mask_f6_regmask_waddr) && (|vpu_ctrl_mask_f3_regmask_store === 1) && (|vpu_ctrl_mask_f6_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f3f7:         coverpoint  vpu_ctrl_f3_thread_id         iff ( (vpu_ctrl_f3_thread_id != vpu_ctrl_f7_thread_id) && (vpu_ctrl_mask_f3_regmask_waddr === vpu_ctrl_mask_f7_regmask_waddr) && (|vpu_ctrl_mask_f3_regmask_store === 1) && (|vpu_ctrl_mask_f7_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f3f8:         coverpoint  vpu_ctrl_f3_thread_id         iff ( (vpu_ctrl_f3_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f3_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f3_regmask_store === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

   vpu_ctrl_mask_thread_corruption_f4f5:         coverpoint  vpu_ctrl_f4_thread_id         iff ( (vpu_ctrl_f4_thread_id != vpu_ctrl_f5_thread_id) && (vpu_ctrl_mask_f4_regmask_waddr === vpu_ctrl_mask_f5_regmask_waddr) && (|vpu_ctrl_mask_f4_regmask_store === 1) && (|vpu_ctrl_mask_f5_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f4f6:         coverpoint  vpu_ctrl_f4_thread_id         iff ( (vpu_ctrl_f4_thread_id != vpu_ctrl_f6_thread_id) && (vpu_ctrl_mask_f4_regmask_waddr === vpu_ctrl_mask_f6_regmask_waddr) && (|vpu_ctrl_mask_f4_regmask_store === 1) && (|vpu_ctrl_mask_f6_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f4f7:         coverpoint  vpu_ctrl_f4_thread_id         iff ( (vpu_ctrl_f4_thread_id != vpu_ctrl_f7_thread_id) && (vpu_ctrl_mask_f4_regmask_waddr === vpu_ctrl_mask_f7_regmask_waddr) && (|vpu_ctrl_mask_f4_regmask_store === 1) && (|vpu_ctrl_mask_f7_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f4f8:         coverpoint  vpu_ctrl_f4_thread_id         iff ( (vpu_ctrl_f4_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f4_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f4_regmask_store === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

   vpu_ctrl_mask_thread_corruption_f5f6:         coverpoint  vpu_ctrl_f5_thread_id         iff ( (vpu_ctrl_f5_thread_id != vpu_ctrl_f6_thread_id) && (vpu_ctrl_mask_f5_regmask_waddr === vpu_ctrl_mask_f6_regmask_waddr) && (|vpu_ctrl_mask_f5_regmask_store === 1) && (|vpu_ctrl_mask_f6_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f5f7:         coverpoint  vpu_ctrl_f5_thread_id         iff ( (vpu_ctrl_f5_thread_id != vpu_ctrl_f7_thread_id) && (vpu_ctrl_mask_f5_regmask_waddr === vpu_ctrl_mask_f7_regmask_waddr) && (|vpu_ctrl_mask_f5_regmask_store === 1) && (|vpu_ctrl_mask_f7_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f5f8:         coverpoint  vpu_ctrl_f5_thread_id         iff ( (vpu_ctrl_f5_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f5_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f5_regmask_store === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

   vpu_ctrl_mask_thread_corruption_f6f7:         coverpoint  vpu_ctrl_f6_thread_id         iff ( (vpu_ctrl_f6_thread_id != vpu_ctrl_f7_thread_id) && (vpu_ctrl_mask_f6_regmask_waddr === vpu_ctrl_mask_f7_regmask_waddr) && (|vpu_ctrl_mask_f6_regmask_store === 1) && (|vpu_ctrl_mask_f7_regmask_store === 1)) ;
   vpu_ctrl_mask_thread_corruption_f6f8:         coverpoint  vpu_ctrl_f6_thread_id         iff ( (vpu_ctrl_f6_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f6_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f6_regmask_store === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

   vpu_ctrl_mask_thread_corruption_f7f8:         coverpoint  vpu_ctrl_f7_thread_id         iff ( (vpu_ctrl_f7_thread_id != vpu_ctrl_f8_thread_id) && (vpu_ctrl_mask_f7_regmask_waddr === vpu_ctrl_mask_f8_regmask_waddr) && (|vpu_ctrl_mask_f7_regmask_store === 1) && (|vpu_ctrl_mask_f8_regmask_store === 1)) ;

//-------------------------------------------------------------------------------------------------------------------------------------------
   vpu_ctrl_f2_kill_maskop_ex_t0:                coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 0 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1) ;
   vpu_ctrl_f2_kill_maskop_ex_t1:                coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1) ;
   vpu_ctrl_f2_kill_shsw_ex_t0:                  coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 0 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1) ;
   vpu_ctrl_f2_kill_shsw_ex_t1:                  coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1) ;
   vpu_ctrl_f2_kill_txfma_ex_t0:                 coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 0 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1) ;
   vpu_ctrl_f2_kill_txfma_ex_t1:                 coverpoint  vpu_ctrl_f2_core_kill          iff (vpu_ctrl_ex_thread_id === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1) ;


   vpu_ctrl_f3_kill_maskop_ex_maskop_f2_tsame:   coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_maskop_ex_shsw_f2_tsame:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_f2_shsw === 1) ;
   vpu_ctrl_f3_kill_maskop_ex_txfma_f2_tsame:    coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_f2_txfma === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_maskop_f2_tsame:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_shsw_f2_tsame:       coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_f2_shsw   === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_txfma_f2_tsame:      coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_f2_txfma  === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_maskop_f2_tsame:    coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_shsw_f2_tsame:      coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_f2_shsw   === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_txfma_f2_tsame:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id === vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_f2_txfma  === 1) ;


   vpu_ctrl_f3_kill_maskop_ex_maskop_f2_tdiff:   coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_maskop_ex_shsw_f2_tdiff:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_f2_shsw === 1) ;
   vpu_ctrl_f3_kill_maskop_ex_txfma_f2_tdiff:    coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_maskop === 1 && vpu_ctrl_f2_txfma === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_maskop_f2_tdiff:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_shsw_f2_tdiff:       coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_f2_shsw   === 1) ;
   vpu_ctrl_f3_kill_shsw_ex_txfma_f2_tdiff:      coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_shsw === 1 && vpu_ctrl_f2_txfma  === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_maskop_f2_tdiff:    coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_mask_f2_maskop === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_shsw_f2_tdiff:      coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_f2_shsw   === 1) ;
   vpu_ctrl_f3_kill_txfma_ex_txfma_f2_tdiff:     coverpoint  vpu_ctrl_f3_core_kill          iff (vpu_ctrl_ex_thread_id != vpu_ctrl_f2_thread_id && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_ex_req_sigs_txfma === 1 && vpu_ctrl_f2_txfma  === 1) ;

//-------------------------------------------------------------------------------------------------------------------------------------------

   vpu_trans_sm_full_f6kill:	   		 coverpoint  trans_f6_killed                      iff (state6_id != 0 && state5_id != 0 && state4_id != 0 && state3_id != 0  && state2_id != 0  && state1_id != 0  && state0_id != 0) ;
   vpu_trans_sm_full_f7kill:	   		 coverpoint  trans_f7_killed                      iff (state6_id != 0 && state5_id != 0 && state4_id != 0 && state3_id != 0  && state2_id != 0  && state1_id != 0  && state0_id != 0) ;
   vpu_trans_sm_full_f8kill:	   		 coverpoint  trans_f8_killed                      iff (state6_id != 0 && state5_id != 0 && state4_id != 0 && state3_id != 0  && state2_id != 0  && state1_id != 0  && state0_id != 0) ;
   
   vpu_trans_sm_full_f8kill_insert_s6:	 	 coverpoint  state6_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s6 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s5:	 	 coverpoint  state5_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s5 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s4:	 	 coverpoint  state4_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s4 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s3:	 	 coverpoint  state3_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s3 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s2:	 	 coverpoint  state2_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s2 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s1:	 	 coverpoint  state1_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s1 = {16,[18:31]};}
   vpu_trans_sm_full_f8kill_insert_s0:	 	 coverpoint  state0_id                            iff (push_state_id != 0 && trans_f8_killed == 1) {ignore_bins vpu_trans_sm_full_f8kill_insert_s0 = {16,[18:31]};}

//-------------------------------------------------------------------------------------------------------------------------------------------

   vpu_ctrl_ex_raddr1_matches_f3sw_t0_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr1_matches_f3sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f4sw_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                             iff ( vpu_ctrl_ex_raddr1_matches_f4sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f4ld_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                           iff ( vpu_ctrl_ex_raddr1_matches_f4ld_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f5sw_t0_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                             iff ( vpu_ctrl_ex_raddr1_matches_f5sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f6sw_t0_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                             iff ( vpu_ctrl_ex_raddr1_matches_f6sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f7sw_t0_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                             iff ( vpu_ctrl_ex_raddr1_matches_f7sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f8sw_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                             iff ( vpu_ctrl_ex_raddr1_matches_f8sw_t0  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f8txfma_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f8txfma_t0  === 1) ;

   vpu_ctrl_ex_raddr2_matches_f3sw_t0_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr2_matches_f3sw_t0  === 1);
   vpu_ctrl_ex_raddr2_matches_f4sw_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                             iff ( vpu_ctrl_ex_raddr2_matches_f4sw_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f4ld_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                           iff ( vpu_ctrl_ex_raddr2_matches_f4ld_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f5sw_t0_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                             iff ( vpu_ctrl_ex_raddr2_matches_f5sw_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f6sw_t0_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                             iff ( vpu_ctrl_ex_raddr2_matches_f6sw_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f7sw_t0_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                             iff ( vpu_ctrl_ex_raddr2_matches_f7sw_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f8sw_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                             iff ( vpu_ctrl_ex_raddr2_matches_f8sw_t0  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f8txfma_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f8txfma_t0  === 1) ;


   vpu_ctrl_ex_raddr3_matches_f3sw_t0_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr3_matches_f3sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f4sw_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                             iff ( vpu_ctrl_ex_raddr3_matches_f4sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f4ld_t0_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                           iff ( vpu_ctrl_ex_raddr3_matches_f4ld_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f5sw_t0_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                             iff ( vpu_ctrl_ex_raddr3_matches_f5sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f6sw_t0_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                             iff ( vpu_ctrl_ex_raddr3_matches_f6sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f7sw_t0_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                             iff ( vpu_ctrl_ex_raddr3_matches_f7sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f8sw_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                             iff ( vpu_ctrl_ex_raddr3_matches_f8sw_t0  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f8txfma_t0_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f8txfma_t0  === 1) ;

   vpu_ctrl_ex_raddr1_matches_f3sw_tdiff_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr1_matches_f3sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f4sw_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f4sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f4ld_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                        iff ( vpu_ctrl_ex_raddr1_matches_f4ld_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f5sw_tdiff_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f5sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f6sw_tdiff_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f6sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f7sw_tdiff_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f7sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f8sw_tdiff_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f8sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff_cvg: coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f3sw_tdiff_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr2_matches_f3sw_tdiff  === 1);
   vpu_ctrl_ex_raddr2_matches_f4sw_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f4sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f4ld_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                        iff ( vpu_ctrl_ex_raddr2_matches_f4ld_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f5sw_tdiff_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f5sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f6sw_tdiff_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f6sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f7sw_tdiff_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f7sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f8sw_tdiff_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f8sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff_cvg: coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f3sw_tdiff_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_raddr3_matches_f3sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f4sw_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f4sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f4ld_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                        iff ( vpu_ctrl_ex_raddr3_matches_f4ld_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f5sw_tdiff_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f5sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f6sw_tdiff_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f6sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f7sw_tdiff_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f7sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f8sw_tdiff_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f8sw_tdiff  === 1) ;
   vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff_cvg: coverpoint vpu_ctrl_f8_regfile_wmask                          iff ( vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff  === 1) ;

   vpu_ctrl_ex_waddr_matches_f3sw_tdiff_cvg:    coverpoint vpu_ctrl_f3_regfile_wmask      		       iff ( vpu_ctrl_ex_waddr_matches_f3sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f4sw_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f4sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f4ld_tdiff_cvg:    coverpoint vpu_ctrl_f4_regfile_wmask_l                         iff ( vpu_ctrl_ex_waddr_matches_f4ld_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f5sw_tdiff_cvg:    coverpoint vpu_ctrl_f5_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f5sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f6sw_tdiff_cvg:    coverpoint vpu_ctrl_f6_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f6sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f7sw_tdiff_cvg:    coverpoint vpu_ctrl_f7_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f7sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f8sw_tdiff_cvg:    coverpoint vpu_ctrl_f8_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f8sw_tdiff  === 1) ;
   vpu_ctrl_ex_waddr_matches_f8txfma_tdiff_cvg: coverpoint vpu_ctrl_f8_regfile_wmask                           iff ( vpu_ctrl_ex_waddr_matches_f8txfma_tdiff  === 1) ;





   // tensor reduce
   vpu_ctrl_reduce_start_reg_cvg: coverpoint vpu_ctrl_reduce_start_reg iff (vpu_ctrl_reduce_start == 1);
   vpu_ctrl_reduce_num_regs_cvg: coverpoint vpu_ctrl_reduce_num_regs iff (vpu_ctrl_reduce_start == 1);
   vpu_ctrl_reduce_op_cvg: coverpoint vpu_ctrl_reduce_op iff (vpu_ctrl_reduce_start == 1);

//-------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------
      
      
   endgroup

endpackage;

module neigh_vpu_cvg (input clock);
	import neigh_vpu_cov_pkg::*;

        // VPU coverage signals
        bit [4:0] state6_id;
        bit [4:0] state5_id;
        bit [4:0] state4_id;
        bit [4:0] state3_id;
        bit [4:0] state2_id;
        bit [4:0] state1_id;
        bit [4:0] state0_id;
        bit [4:0] push_state_id;
        bit vpu_ctrl_ex_core_kill;
        bit [3:0] id_fp_sboard_bits;
        bit [3:0] id_fp_hazard_en;
        bit [2:0] id_reg_gsc_cnt;
        bit id_ctrl_gsc;
	bit [7:0] id_mask_sboard_raddr_bits;
	bit [7:0] id_mask_sboard_waddr_bits;
        bit id_vpu_ctrl_tfma_wrrf_enabled;
        bit vpu_ctrl_tensorfma_start;
        bit id_vpu_ctrl_tquant_enabled;
        bit vpu_ctrl_tensorquant_start;
        bit id_vpu_ctrl_reduce_enabled;
        bit id_valid_qual;
        bit id_thread_id;
        bit id_csr_wr_safe;
        bit id_ctrl_to_ex_mem;
        bit id_ctrl_to_ex_fp;
        bit id_ctrl_to_ex_wfd;
        bit id_ctrl_to_ex_gsc;
        bit id_ctrl_to_ex_maskop;
        bit id_ctrl_to_ex_trans;
	bit ex_csr_vpu;
	bit tag_csr_vpu;
	bit mem_csr_vpu;
	bit wb_csr_vpu;
        bit vpu_ctrl_f2_core_kill;
        bit vpu_ctrl_f3_core_kill;
        bit trans_f6_killed;
        bit trans_f7_killed;
        bit trans_f8_killed;
        bit vpu_ctrl_mask_f2_maskop;
        bit vpu_ctrl_mask_f3_maskop = 0;
        bit vpu_ctrl_f2_shsw;
        bit vpu_ctrl_f2_txfma;
        bit [2:0] vpu_ctrl_mask_f2_regmask_waddr;
        bit [2:0] vpu_ctrl_mask_f4_regmask_waddr;
        bit [2:0] vpu_ctrl_mask_f5_regmask_waddr;
        bit [2:0] vpu_ctrl_mask_f6_regmask_waddr;
        bit [2:0] vpu_ctrl_mask_f7_regmask_waddr;
        bit vpu_ctrl_mask_f3_ctrl_valid = 0;
        bit vpu_ctrl_f2_thread_id;
        bit vpu_ctrl_ex_ctrl_valid_qual;
        bit vpu_ctrl_ex_thread_id;
        bit vpu_ctrl_ex_req_sigs_maskop;
        bit vpu_ctrl_ex_req_sigs_m0ren;
        bit vpu_ctrl_ex_req_sigs_mren1;
        bit vpu_ctrl_ex_req_sigs_mren2;
        bit [4:0] vpu_ctrl_mask_ex_regfile_raddr1;
        bit [4:0] vpu_ctrl_mask_ex_regfile_raddr2;
        bit vpu_ctrl_ex_req_sigs_vector;
        bit vpu_ctrl_ex_req_sigs_shsw;
        bit vpu_ctrl_ex_req_sigs_txfma;
        bit vpu_ctrl_mask_f2_regmask_store_all;
        bit [6:0] vpu_ctrl_mask_f2_cmd;
        bit [6:0] vpu_ctrl_mask_f3_cmd = 0;
        bit vpu_ctrl_mask_f3_regmask_from_short;
        bit [2:0] vpu_ctrl_mask_f3_regmask_waddr;
        bit [7:0] vpu_ctrl_mask_f3_regmask_store;
        bit [7:0] vpu_ctrl_mask_f2_regmask_store_to_mask;
        bit [7:0] vpu_ctrl_mask_f4_regmask_store;
        bit [7:0] vpu_ctrl_mask_f5_regmask_store;
        bit [7:0] vpu_ctrl_mask_f6_regmask_store;
        bit [7:0] vpu_ctrl_mask_f7_regmask_store;
        bit [7:0] vpu_ctrl_mask_f8_regmask_store;
        bit vpu_ctrl_f3_thread_id;
        bit vpu_ctrl_f8_regmask_from_txfma;
        bit [2:0] vpu_ctrl_mask_f8_regmask_waddr;
        bit [7:0] vpu_ctrl_mask_f8_regmask_store_sel;
        bit vpu_ctrl_f8_thread_id;
        bit [4:0] vpu_ctrl_ex_regfile_raddr1;
        bit [4:0] vpu_ctrl_ex_regfile_raddr2;
        bit [4:0] vpu_ctrl_ex_regfile_raddr3;
        bit [4:0] vpu_ctrl_ex_regfile_waddr;
        bit vpu_ctrl_ex_req_sigs_ren1 ;
        bit vpu_ctrl_ex_req_sigs_ren2 ;
        bit vpu_ctrl_ex_req_sigs_ren3;
        bit vpu_ctrl_f4_ctrl_valid_qual;
        bit vpu_ctrl_f4_regfile_thrid_l ;
        bit [4:0] vpu_ctrl_f4_regfile_waddr_l;
        bit [7:0] vpu_ctrl_f4_regfile_wmask_l;
        bit vpu_ctrl_f3_ctrl_valid_qual;
        bit [4:0] vpu_ctrl_f3_regfile_waddr;
        bit [7:0] vpu_ctrl_f3_regfile_wmask;
        bit [4:0] vpu_ctrl_f4_regfile_waddr;
        bit [7:0] vpu_ctrl_f4_regfile_wmask;
        bit vpu_ctrl_f4_thread_id;
        bit vpu_ctrl_f5_ctrl_valid_qual;
        bit [4:0] vpu_ctrl_f5_regfile_waddr;
        bit [7:0] vpu_ctrl_f5_regfile_wmask;
        bit vpu_ctrl_f5_thread_id;
        bit vpu_ctrl_f6_ctrl_valid_qual;
        bit [4:0] vpu_ctrl_f6_regfile_waddr;
        bit [7:0] vpu_ctrl_f6_regfile_wmask;
        bit vpu_ctrl_f6_thread_id;
        bit vpu_ctrl_f7_ctrl_valid_qual;
        bit [4:0] vpu_ctrl_f7_regfile_waddr;
        bit [7:0] vpu_ctrl_f7_regfile_wmask;
        bit vpu_ctrl_f7_thread_id;
        bit [4:0] vpu_ctrl_f8_regfile_waddr;
        bit [7:0] vpu_ctrl_f8_regfile_wmask;
        bit vpu_ctrl_f8_ctrl_valid_qual ;
        bit vpu_ctrl_f8_ctrl_sigs_txfma;
        // tensor reduce
        bit vpu_ctrl_reduce_start;
        bit [4:0] vpu_ctrl_reduce_start_reg;
        bit [6:0] vpu_ctrl_reduce_num_regs;
        bit [3:0] vpu_ctrl_reduce_op;
     
        // VPU helper signals
        bit vpu_ctrl_ex_raddr1_matches_f3sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f4sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f4ld_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f5sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f6sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f7sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f8sw_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f8txfma_t0 ;
        bit vpu_ctrl_ex_raddr1_matches_f3sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f4sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f4ld_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f5sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f6sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f7sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f8sw_tdiff ;
        bit vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff ;
     
        bit vpu_ctrl_ex_raddr2_matches_f3sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f4sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f4ld_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f5sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f6sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f7sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f8sw_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f8txfma_t0 ;
        bit vpu_ctrl_ex_raddr2_matches_f3sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f4sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f4ld_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f5sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f6sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f7sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f8sw_tdiff ;
        bit vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff ;
     
        bit vpu_ctrl_ex_raddr3_matches_f3sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f4sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f4ld_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f5sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f6sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f7sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f8sw_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f8txfma_t0 ;
        bit vpu_ctrl_ex_raddr3_matches_f3sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f4sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f4ld_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f5sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f6sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f7sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f8sw_tdiff ;
        bit vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff ;
        

        bit vpu_ctrl_ex_waddr_matches_f3sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f4sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f4ld_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f5sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f6sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f7sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f8sw_tdiff ;
        bit vpu_ctrl_ex_waddr_matches_f8txfma_tdiff ;

	bit id_vpu_stall_fp_sb ;
	bit id_vpu_stall_rd_scoreboard;
	bit id_vpu_stall_wr_scoreboard;
	bit id_vpu_stall_tensor_fma;
	bit id_vpu_stall_tensor_quant;
	bit id_vpu_stall_tensor_reduce;
	bit id_vpu_stall_tensor_going_to_vpu;

	bit id_vpu_stall_fp_sb_d ;
	bit id_vpu_stall_fp_sb_event00 ;
	bit id_vpu_stall_fp_sb_event01 ;
	bit id_vpu_stall_fp_sb_event10 ;
	bit id_vpu_stall_fp_sb_event11 ;

	bit id_vpu_stall_rd_scoreboard_d;
	bit id_vpu_stall_rd_event00 ;
	bit id_vpu_stall_rd_event01 ;
	bit id_vpu_stall_rd_event10 ;
	bit id_vpu_stall_rd_event11 ;

	bit id_vpu_stall_wr_scoreboard_d;
	bit id_vpu_stall_wr_event00 ;
	bit id_vpu_stall_wr_event01 ;
	bit id_vpu_stall_wr_event10 ;
	bit id_vpu_stall_wr_event11 ;

	bit id_vpu_stall_tensor_fma_d;
	bit id_vpu_stall_tensor_fma_event00;
	bit id_vpu_stall_tensor_fma_event01;
	bit id_vpu_stall_tensor_fma_event10;
	bit id_vpu_stall_tensor_fma_event11;
	bit id_vpu_stall_tensor_quant_d;
	bit id_vpu_stall_tensor_quant_event00;
	bit id_vpu_stall_tensor_quant_event01;
	bit id_vpu_stall_tensor_quant_event10;
	bit id_vpu_stall_tensor_quant_event11;


	bit id_vpu_stall_tensor_reduce_d;
	bit id_vpu_stall_tensor_reduce_event00;
	bit id_vpu_stall_tensor_reduce_event01;
	bit id_vpu_stall_tensor_reduce_event10;
	bit id_vpu_stall_tensor_reduce_event11;

	bit id_vpu_stall_tensor_going_to_vpu_d;
	bit id_vpu_stall_tensor_going_to_vpu_event00;
	bit id_vpu_stall_tensor_going_to_vpu_event01;
	bit id_vpu_stall_tensor_going_to_vpu_event10;
	bit id_vpu_stall_tensor_going_to_vpu_event11;

        bit id_valid_qual_d;

        bit id_thread_id_d;
        bit id_csr_wr_safe_d;
        bit id_ctrl_to_ex_mem_d;
        bit id_ctrl_to_ex_fp_d;
        bit id_ctrl_to_ex_wfd_d;
        bit id_ctrl_to_ex_gsc_d;
        bit id_ctrl_to_ex_maskop_d;
        bit id_ctrl_to_ex_trans_d;



	minion_vpu_cg	minion_vpu_cg_inst = new(        
        state6_id,
        state5_id,
        state4_id,
        state3_id,
        state2_id,
        state1_id,
        state0_id,
        push_state_id,
        vpu_ctrl_ex_core_kill,
        id_fp_sboard_bits,
        id_fp_hazard_en,
        id_reg_gsc_cnt,
        id_ctrl_gsc,
	id_mask_sboard_raddr_bits,
	id_mask_sboard_waddr_bits,
        id_vpu_ctrl_tfma_wrrf_enabled,
        vpu_ctrl_tensorfma_start,
        id_vpu_ctrl_tquant_enabled,
        vpu_ctrl_tensorquant_start,
        id_vpu_ctrl_reduce_enabled,
        id_valid_qual,
        id_thread_id,
        id_csr_wr_safe,
        id_ctrl_to_ex_mem,
        id_ctrl_to_ex_fp,
        id_ctrl_to_ex_wfd,
        id_ctrl_to_ex_gsc,
        id_ctrl_to_ex_maskop,
        id_ctrl_to_ex_trans,
	ex_csr_vpu,
	tag_csr_vpu,
	mem_csr_vpu,
	wb_csr_vpu,
        vpu_ctrl_f2_core_kill,
        vpu_ctrl_f3_core_kill,
        trans_f6_killed,
        trans_f7_killed,
        trans_f8_killed,
        vpu_ctrl_mask_f2_maskop,
        vpu_ctrl_mask_f3_maskop ,
        vpu_ctrl_f2_shsw,
        vpu_ctrl_f2_txfma,
        vpu_ctrl_mask_f2_regmask_waddr,
        vpu_ctrl_mask_f4_regmask_waddr,
        vpu_ctrl_mask_f5_regmask_waddr,
        vpu_ctrl_mask_f6_regmask_waddr,
        vpu_ctrl_mask_f7_regmask_waddr,
        vpu_ctrl_mask_f3_ctrl_valid ,
        vpu_ctrl_f2_thread_id,
        vpu_ctrl_ex_ctrl_valid_qual,
        vpu_ctrl_ex_thread_id,
        vpu_ctrl_ex_req_sigs_maskop,
        vpu_ctrl_ex_req_sigs_m0ren,
        vpu_ctrl_ex_req_sigs_mren1,
        vpu_ctrl_ex_req_sigs_mren2,
        vpu_ctrl_mask_ex_regfile_raddr1,
        vpu_ctrl_mask_ex_regfile_raddr2,
        vpu_ctrl_ex_req_sigs_vector,
        vpu_ctrl_ex_req_sigs_shsw,
        vpu_ctrl_ex_req_sigs_txfma,
        vpu_ctrl_mask_f2_regmask_store_all,
        vpu_ctrl_mask_f2_cmd,
        vpu_ctrl_mask_f3_cmd ,
        vpu_ctrl_mask_f3_regmask_from_short,
        vpu_ctrl_mask_f3_regmask_waddr,
        vpu_ctrl_mask_f3_regmask_store,
        vpu_ctrl_mask_f2_regmask_store_to_mask,
        vpu_ctrl_mask_f4_regmask_store,
        vpu_ctrl_mask_f5_regmask_store,
        vpu_ctrl_mask_f6_regmask_store,
        vpu_ctrl_mask_f7_regmask_store,
        vpu_ctrl_mask_f8_regmask_store,
        vpu_ctrl_f3_thread_id,
        vpu_ctrl_f8_regmask_from_txfma,
        vpu_ctrl_mask_f8_regmask_waddr,
        vpu_ctrl_mask_f8_regmask_store_sel,
        vpu_ctrl_f8_thread_id,
        vpu_ctrl_ex_regfile_raddr1,
        vpu_ctrl_ex_regfile_raddr2,
        vpu_ctrl_ex_regfile_raddr3,
        vpu_ctrl_ex_regfile_waddr,
        vpu_ctrl_ex_req_sigs_ren1 ,
        vpu_ctrl_ex_req_sigs_ren2 ,
        vpu_ctrl_ex_req_sigs_ren3,
        vpu_ctrl_f4_ctrl_valid_qual,
        vpu_ctrl_f4_regfile_thrid_l ,
        vpu_ctrl_f4_regfile_waddr_l,
        vpu_ctrl_f4_regfile_wmask_l,
        vpu_ctrl_f3_ctrl_valid_qual,
        vpu_ctrl_f3_regfile_waddr,
        vpu_ctrl_f3_regfile_wmask,
        vpu_ctrl_f4_regfile_waddr,
        vpu_ctrl_f4_regfile_wmask,
        vpu_ctrl_f4_thread_id,
        vpu_ctrl_f5_ctrl_valid_qual,
        vpu_ctrl_f5_regfile_waddr,
        vpu_ctrl_f5_regfile_wmask,
        vpu_ctrl_f5_thread_id,
        vpu_ctrl_f6_ctrl_valid_qual,
        vpu_ctrl_f6_regfile_waddr,
        vpu_ctrl_f6_regfile_wmask,
        vpu_ctrl_f6_thread_id,
        vpu_ctrl_f7_ctrl_valid_qual,
        vpu_ctrl_f7_regfile_waddr,
        vpu_ctrl_f7_regfile_wmask,
        vpu_ctrl_f7_thread_id,
        vpu_ctrl_f8_regfile_waddr,
        vpu_ctrl_f8_regfile_wmask,
        vpu_ctrl_f8_ctrl_valid_qual ,
        vpu_ctrl_f8_ctrl_sigs_txfma,
        // tensor reduce
        vpu_ctrl_reduce_start,
        vpu_ctrl_reduce_start_reg,
        vpu_ctrl_reduce_num_regs,
        vpu_ctrl_reduce_op, 

        // VPU helper signals
        vpu_ctrl_ex_raddr1_matches_f3sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f4sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f4ld_t0 ,
        vpu_ctrl_ex_raddr1_matches_f5sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f6sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f7sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f8sw_t0 ,
        vpu_ctrl_ex_raddr1_matches_f8txfma_t0 ,
        vpu_ctrl_ex_raddr1_matches_f3sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f4sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f4ld_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f5sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f6sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f7sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f8sw_tdiff ,
        vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff ,
     
        vpu_ctrl_ex_raddr2_matches_f3sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f4sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f4ld_t0 ,
        vpu_ctrl_ex_raddr2_matches_f5sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f6sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f7sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f8sw_t0 ,
        vpu_ctrl_ex_raddr2_matches_f8txfma_t0 ,
        vpu_ctrl_ex_raddr2_matches_f3sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f4sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f4ld_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f5sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f6sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f7sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f8sw_tdiff ,
        vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff ,
     
        vpu_ctrl_ex_raddr3_matches_f3sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f4sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f4ld_t0 ,
        vpu_ctrl_ex_raddr3_matches_f5sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f6sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f7sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f8sw_t0 ,
        vpu_ctrl_ex_raddr3_matches_f8txfma_t0 ,
        vpu_ctrl_ex_raddr3_matches_f3sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f4sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f4ld_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f5sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f6sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f7sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f8sw_tdiff ,
        vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff,
        vpu_ctrl_ex_waddr_matches_f3sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f4sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f4ld_tdiff ,
        vpu_ctrl_ex_waddr_matches_f5sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f6sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f7sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f8sw_tdiff ,
        vpu_ctrl_ex_waddr_matches_f8txfma_tdiff,
	id_vpu_stall_fp_sb,
	id_vpu_stall_rd_scoreboard,
	id_vpu_stall_wr_scoreboard,
	id_vpu_stall_tensor_fma,
	id_vpu_stall_tensor_quant,
	id_vpu_stall_tensor_reduce,
	id_vpu_stall_tensor_going_to_vpu,
	id_vpu_stall_fp_sb_d,
	id_vpu_stall_fp_sb_event00 ,
	id_vpu_stall_fp_sb_event01 ,
	id_vpu_stall_fp_sb_event10 ,
	id_vpu_stall_fp_sb_event11 ,

	id_vpu_stall_rd_scoreboard_d,
	id_vpu_stall_rd_event00 ,
	id_vpu_stall_rd_event01 ,
	id_vpu_stall_rd_event10 ,
	id_vpu_stall_rd_event11 ,

	id_vpu_stall_wr_scoreboard_d,
	id_vpu_stall_wr_event00 ,
	id_vpu_stall_wr_event01 ,
	id_vpu_stall_wr_event10 ,
	id_vpu_stall_wr_event11 ,

	id_vpu_stall_tensor_fma_d,
	id_vpu_stall_tensor_fma_event00,
	id_vpu_stall_tensor_fma_event01,
	id_vpu_stall_tensor_fma_event10,
	id_vpu_stall_tensor_fma_event11,

	id_vpu_stall_tensor_quant_d,
	id_vpu_stall_tensor_quant_event00,
	id_vpu_stall_tensor_quant_event01,
	id_vpu_stall_tensor_quant_event10,
	id_vpu_stall_tensor_quant_event11,

	id_vpu_stall_tensor_reduce_d,
	id_vpu_stall_tensor_reduce_event00,
	id_vpu_stall_tensor_reduce_event01,
	id_vpu_stall_tensor_reduce_event10,
	id_vpu_stall_tensor_reduce_event11,

	id_vpu_stall_tensor_going_to_vpu_d,
	id_vpu_stall_tensor_going_to_vpu_event00,
	id_vpu_stall_tensor_going_to_vpu_event01,
	id_vpu_stall_tensor_going_to_vpu_event10,
	id_vpu_stall_tensor_going_to_vpu_event11,

        id_valid_qual_d,
        id_thread_id_d,
        id_csr_wr_safe_d,
        id_ctrl_to_ex_mem_d,
        id_ctrl_to_ex_fp_d,
        id_ctrl_to_ex_wfd_d,
        id_ctrl_to_ex_gsc_d,
        id_ctrl_to_ex_maskop_d,
        id_ctrl_to_ex_trans_d
);

        assign vpu_ctrl_ex_raddr1_matches_f3sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f4sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f4ld_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f5sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f6sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f7sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f8sw_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr1_matches_f8txfma_t0 = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
    
        assign vpu_ctrl_ex_raddr1_matches_f3sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f4sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f4ld_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f5sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f6sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f7sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr1_matches_f8sw_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id != vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr1_matches_f8txfma_tdiff = vpu_ctrl_ex_req_sigs_ren1 === 1 && (vpu_ctrl_ex_regfile_raddr1 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id !=  vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
    
    
        assign vpu_ctrl_ex_raddr2_matches_f3sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f4sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f4ld_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f5sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f6sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f7sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f8sw_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr2_matches_f8txfma_t0 = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
    
        assign vpu_ctrl_ex_raddr2_matches_f3sw_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f3_regfile_waddr)   &&    vpu_ctrl_f3_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f4sw_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f4_regfile_waddr)   &&    vpu_ctrl_f4_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f4ld_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f4_regfile_waddr_l) &&    vpu_ctrl_f4_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f5sw_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f5_regfile_waddr)   &&    vpu_ctrl_f5_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f6sw_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f6_regfile_waddr)   &&    vpu_ctrl_f6_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f7sw_tdiff =    vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f7_regfile_waddr)   &&    vpu_ctrl_f7_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr2_matches_f8sw_tdiff =       vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr2_matches_f8txfma_tdiff = vpu_ctrl_ex_req_sigs_ren2 === 1 && (vpu_ctrl_ex_regfile_raddr2 === vpu_ctrl_f8_regfile_waddr)   &&    vpu_ctrl_f8_thread_id != vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
    
        assign vpu_ctrl_ex_raddr3_matches_f3sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f4sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f4ld_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f5sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f6sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f7sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f8sw_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr3_matches_f8txfma_t0 = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id === vpu_ctrl_ex_thread_id  && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
    
        assign vpu_ctrl_ex_raddr3_matches_f3sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f4sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f4ld_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f5sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f6sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f7sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_raddr3_matches_f8sw_tdiff =    vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_raddr3_matches_f8txfma_tdiff = vpu_ctrl_ex_req_sigs_ren3 === 1 && (vpu_ctrl_ex_regfile_raddr3 === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;
	
        assign vpu_ctrl_ex_waddr_matches_f3sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f3_regfile_waddr)   && vpu_ctrl_f3_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f3_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f4sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f4_regfile_waddr)   && vpu_ctrl_f4_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f4ld_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f4_regfile_waddr_l) && vpu_ctrl_f4_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f4_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f5sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f5_regfile_waddr)   && vpu_ctrl_f5_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f5_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f6sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f6_regfile_waddr)   && vpu_ctrl_f6_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f6_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f7sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f7_regfile_waddr)   && vpu_ctrl_f7_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f7_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1;
        assign vpu_ctrl_ex_waddr_matches_f8sw_tdiff =     (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 0;
        assign vpu_ctrl_ex_waddr_matches_f8txfma_tdiff =  (vpu_ctrl_ex_regfile_waddr === vpu_ctrl_f8_regfile_waddr)   && vpu_ctrl_f8_thread_id !=  vpu_ctrl_ex_thread_id && vpu_ctrl_f8_ctrl_valid_qual === 1 && vpu_ctrl_ex_ctrl_valid_qual === 1 && vpu_ctrl_f8_ctrl_sigs_txfma === 1;



	assign id_vpu_stall_fp_sb                  = |(id_fp_sboard_bits & id_fp_hazard_en);
        assign id_vpu_stall_rd_scoreboard        = |(id_mask_sboard_raddr_bits);
        assign id_vpu_stall_wr_scoreboard        = |(id_mask_sboard_waddr_bits);
        assign id_vpu_stall_tensor_fma           = id_vpu_ctrl_tfma_wrrf_enabled || vpu_ctrl_tensorfma_start;  // TensorFMA  (only for ones that use the VPU RF)
        assign id_vpu_stall_tensor_quant         = (id_vpu_ctrl_tquant_enabled || vpu_ctrl_tensorquant_start); // TensorQuant
        assign id_vpu_stall_tensor_reduce        = id_vpu_ctrl_reduce_enabled;                                 // TensorReduce/TensorStore
        assign id_vpu_stall_tensor_going_to_vpu  = ex_csr_vpu || tag_csr_vpu || mem_csr_vpu || wb_csr_vpu;     // Tensor* going to VPU

        always @ (posedge clock) begin
                vpu_ctrl_mask_f3_maskop                              = vpu_ctrl_mask_f2_maskop;
                vpu_ctrl_mask_f3_cmd                                 = vpu_ctrl_mask_f2_cmd;
                vpu_ctrl_f2_shsw                                     = vpu_ctrl_ex_ctrl_valid_qual && vpu_ctrl_ex_req_sigs_shsw;
                id_vpu_stall_fp_sb_d                                 = id_vpu_stall_fp_sb                  ;
                id_vpu_stall_rd_scoreboard_d                         = id_vpu_stall_rd_scoreboard        ;
                id_vpu_stall_wr_scoreboard_d                         = id_vpu_stall_wr_scoreboard        ;
                id_vpu_stall_tensor_fma_d                            = id_vpu_stall_tensor_fma           ;
                id_vpu_stall_tensor_quant_d                          = id_vpu_stall_tensor_quant         ;
                id_vpu_stall_tensor_reduce_d                         = id_vpu_stall_tensor_reduce        ;
                id_vpu_stall_tensor_going_to_vpu_d                   = id_vpu_stall_tensor_going_to_vpu  ;
                id_valid_qual_d					     = id_valid_qual;


                id_thread_id_d				            =	id_thread_id;	      
                id_csr_wr_safe_d                                    =   id_csr_wr_safe;
                id_ctrl_to_ex_mem_d                                 =   id_ctrl_to_ex_mem;
                id_ctrl_to_ex_fp_d                                  =   id_ctrl_to_ex_fp;
                id_ctrl_to_ex_wfd_d                                 =   id_ctrl_to_ex_wfd;
                id_ctrl_to_ex_gsc_d                                 =   id_ctrl_to_ex_gsc;
                id_ctrl_to_ex_maskop_d                              =   id_ctrl_to_ex_maskop;
                id_ctrl_to_ex_trans_d                               =   id_ctrl_to_ex_trans;


		minion_vpu_cg_inst.sample();
	end


	        assign id_vpu_stall_fp_sb_event00                     = ~id_vpu_stall_fp_sb_d & ~id_vpu_stall_fp_sb & id_valid_qual_d;
	        assign id_vpu_stall_fp_sb_event01                     = ~id_vpu_stall_fp_sb_d &  id_vpu_stall_fp_sb & id_valid_qual_d;
	        assign id_vpu_stall_fp_sb_event10                      = id_vpu_stall_fp_sb_d & ~id_vpu_stall_fp_sb & id_valid_qual_d;
	        assign id_vpu_stall_fp_sb_event11                      = id_vpu_stall_fp_sb_d &  id_vpu_stall_fp_sb & id_valid_qual_d;


	        assign id_vpu_stall_rd_event00                     = ~id_vpu_stall_rd_scoreboard_d & ~id_vpu_stall_rd_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_rd_event01                     = ~id_vpu_stall_rd_scoreboard_d &  id_vpu_stall_rd_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_rd_event10                      = id_vpu_stall_rd_scoreboard_d & ~id_vpu_stall_rd_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_rd_event11                      = id_vpu_stall_rd_scoreboard_d &  id_vpu_stall_rd_scoreboard & id_valid_qual_d;


	        assign id_vpu_stall_wr_event00                     = ~id_vpu_stall_wr_scoreboard_d & ~id_vpu_stall_wr_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_wr_event01                     = ~id_vpu_stall_wr_scoreboard_d &  id_vpu_stall_wr_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_wr_event10                      = id_vpu_stall_wr_scoreboard_d & ~id_vpu_stall_wr_scoreboard & id_valid_qual_d;
	        assign id_vpu_stall_wr_event11                      = id_vpu_stall_wr_scoreboard_d &  id_vpu_stall_wr_scoreboard & id_valid_qual_d;

	        assign id_vpu_stall_tensor_fma_event00                      = ~id_vpu_stall_tensor_fma_d & ~id_vpu_stall_tensor_fma & id_valid_qual_d;
	        assign id_vpu_stall_tensor_fma_event01                      = ~id_vpu_stall_tensor_fma_d & id_vpu_stall_tensor_fma & id_valid_qual_d;
	        assign id_vpu_stall_tensor_fma_event10                      = id_vpu_stall_tensor_fma_d & ~id_vpu_stall_tensor_fma & id_valid_qual_d;
	        assign id_vpu_stall_tensor_fma_event11                      = id_vpu_stall_tensor_fma_d & id_vpu_stall_tensor_fma & id_valid_qual_d;

	        assign id_vpu_stall_tensor_quant_event00                     = ~id_vpu_stall_tensor_quant_d & ~id_vpu_stall_tensor_quant & id_valid_qual_d;
	        assign id_vpu_stall_tensor_quant_event01                     = ~id_vpu_stall_tensor_quant_d &  id_vpu_stall_tensor_quant & id_valid_qual_d;
	        assign id_vpu_stall_tensor_quant_event10                      = id_vpu_stall_tensor_quant_d & ~id_vpu_stall_tensor_quant & id_valid_qual_d;
	        assign id_vpu_stall_tensor_quant_event11                      = id_vpu_stall_tensor_quant_d &  id_vpu_stall_tensor_quant & id_valid_qual_d;

	        assign id_vpu_stall_tensor_reduce_event00                     = ~id_vpu_stall_tensor_reduce_d & ~id_vpu_stall_tensor_reduce & id_valid_qual_d;
	        assign id_vpu_stall_tensor_reduce_event01                     = ~id_vpu_stall_tensor_reduce_d &  id_vpu_stall_tensor_reduce & id_valid_qual_d;
	        assign id_vpu_stall_tensor_reduce_event10                      = id_vpu_stall_tensor_reduce_d & ~id_vpu_stall_tensor_reduce & id_valid_qual_d;
	        assign id_vpu_stall_tensor_reduce_event11                      = id_vpu_stall_tensor_reduce_d &  id_vpu_stall_tensor_reduce & id_valid_qual_d;

	        assign id_vpu_stall_tensor_going_to_vpu_event00               = ~id_vpu_stall_tensor_going_to_vpu_d & ~id_vpu_stall_tensor_going_to_vpu & id_valid_qual_d;
	        assign id_vpu_stall_tensor_going_to_vpu_event01               = ~id_vpu_stall_tensor_going_to_vpu_d &  id_vpu_stall_tensor_going_to_vpu & id_valid_qual_d;
	        assign id_vpu_stall_tensor_going_to_vpu_event10                = id_vpu_stall_tensor_going_to_vpu_d & ~id_vpu_stall_tensor_going_to_vpu & id_valid_qual_d;
	        assign id_vpu_stall_tensor_going_to_vpu_event11                = id_vpu_stall_tensor_going_to_vpu_d &  id_vpu_stall_tensor_going_to_vpu & id_valid_qual_d;

                assign state6_id                                            = minion_top.vpu.ctrl.trans.states[6].id[4:0];
                assign state5_id                                            = minion_top.vpu.ctrl.trans.states[5].id[4:0];
                assign state4_id                                            = minion_top.vpu.ctrl.trans.states[4].id[4:0];
                assign state3_id                                            = minion_top.vpu.ctrl.trans.states[3].id[4:0];
                assign state2_id                                            = minion_top.vpu.ctrl.trans.states[2].id[4:0];
                assign state1_id                                            = minion_top.vpu.ctrl.trans.states[1].id[4:0];
                assign state0_id                                            = minion_top.vpu.ctrl.trans.states[0].id[4:0];
                assign push_state_id                                        = minion_top.vpu.ctrl.trans.push_ex_state.id[4:0];
                assign vpu_ctrl_f2_txfma                                    = minion_top.vpu.ctrl.f2_txfma_valid_all_en;
                assign vpu_ctrl_ex_core_kill                                = minion_top.vpu.ctrl.ex_core_kill;

                assign id_fp_sboard_bits				    = minion_top.core.intpipe.id_fp_sboard_bits	        ;     
                assign id_fp_hazard_en                                      = minion_top.core.intpipe.id_fp_hazard_en           ; 
                assign id_reg_gsc_cnt                                       = minion_top.core.intpipe.id_reg_gsc_cnt            ; 
                assign id_ctrl_gsc                                          = minion_top.core.intpipe.id_ctrl.gsc               ; 
	        assign id_mask_sboard_raddr_bits                            = minion_top.core.intpipe.id_mask_sboard_raddr_bits ; 
	        assign id_mask_sboard_waddr_bits                            = minion_top.core.intpipe.id_mask_sboard_waddr_bits ; 
                assign id_vpu_ctrl_tfma_wrrf_enabled                        = minion_top.core.intpipe.id_vpu_ctrl.tfma_wrrf_enabled;
                assign vpu_ctrl_tensorfma_start                             = minion_top.core.intpipe.vpu_ctrl.tensorfma_start  ; 
                assign id_vpu_ctrl_tquant_enabled                           = minion_top.core.intpipe.id_vpu_ctrl.tquant_enabled; 
                assign vpu_ctrl_tensorquant_start                           = minion_top.core.intpipe.vpu_ctrl.tensorquant_start; 
                assign id_vpu_ctrl_reduce_enabled                           = minion_top.core.intpipe.id_vpu_ctrl.reduce_enabled;
                assign id_valid_qual					    = minion_top.core.intpipe.id_valid_qual;			
                assign id_thread_id                                         = minion_top.core.intpipe.id_thread_id;           
                assign id_csr_wr_safe                                       = minion_top.core.intpipe.id_csr_wr_safe;          
                assign id_ctrl_to_ex_mem                                    = minion_top.core.intpipe.id_ctrl_to_ex.mem;       
                assign id_ctrl_to_ex_fp                                     = minion_top.core.intpipe.id_ctrl_to_ex.fp;       
                assign id_ctrl_to_ex_wfd                                    = minion_top.core.intpipe.id_ctrl_to_ex.wfd;       
                assign id_ctrl_to_ex_gsc                                    = minion_top.core.intpipe.id_ctrl_to_ex.gsc;       
                assign id_ctrl_to_ex_maskop                                 = minion_top.vpu.ctrl.id_vpu_ctrl.sigs.maskop;    
                assign id_ctrl_to_ex_trans                                  = minion_top.vpu.ctrl.id_vpu_ctrl.sigs.trans;    

                assign ex_csr_vpu         				    = minion_top.core.intpipe.ex_csr_vpu	        ;     
                assign tag_csr_vpu         				    = minion_top.core.intpipe.tag_csr_vpu	        ;     
                assign mem_csr_vpu         				    = minion_top.core.intpipe.mem_csr_vpu	        ;     
                assign wb_csr_vpu         				    = minion_top.core.intpipe.wb_csr_vpu	        ;     

                assign vpu_ctrl_f2_core_kill                                = minion_top.vpu.ctrl.f3_core_kill;
                assign vpu_ctrl_f3_core_kill                                = minion_top.vpu.ctrl.f3_core_kill;
                assign trans_f6_killed                                      = minion_top.vpu.ctrl.trans.f6_killed;
                assign trans_f7_killed                                      = minion_top.vpu.ctrl.trans.f7_killed;
                assign trans_f8_killed                                      = minion_top.vpu.ctrl.trans.f8_killed;
                 
                assign vpu_ctrl_mask_f2_maskop                              = minion_top.vpu.ctrl.mask.f2_maskop;
                assign vpu_ctrl_mask_f2_regmask_waddr                       = minion_top.vpu.ctrl.mask.f2_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f4_regmask_waddr                       = minion_top.vpu.ctrl.mask.f4_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f5_regmask_waddr                       = minion_top.vpu.ctrl.mask.f5_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f6_regmask_waddr                       = minion_top.vpu.ctrl.mask.f6_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f7_regmask_waddr                       = minion_top.vpu.ctrl.mask.f7_regmask_waddr[2:0];
                assign vpu_ctrl_f2_thread_id                                = minion_top.vpu.ctrl.f2_thread_id;
                assign vpu_ctrl_ex_ctrl_valid_qual                          = minion_top.vpu.ctrl.ex_ctrl_valid_qual;
                assign vpu_ctrl_ex_thread_id                                = minion_top.vpu.ctrl.ex_thread_id;
                assign vpu_ctrl_ex_req_sigs_maskop                          = minion_top.vpu.ctrl.ex_req.sigs.maskop;
                assign vpu_ctrl_ex_req_sigs_m0ren                           = minion_top.vpu.ctrl.ex_req.sigs.m0ren; 
                assign vpu_ctrl_ex_req_sigs_mren1                           = minion_top.vpu.ctrl.ex_req.sigs.mren1; 
                assign vpu_ctrl_ex_req_sigs_mren2                           = minion_top.vpu.ctrl.ex_req.sigs.mren2;
                assign vpu_ctrl_mask_ex_regfile_raddr1                      = minion_top.vpu.ctrl.mask.ex_regfile_raddr1[4:0];
                assign vpu_ctrl_mask_ex_regfile_raddr2                      = minion_top.vpu.ctrl.mask.ex_regfile_raddr2[4:0];
                assign vpu_ctrl_ex_req_sigs_vector                          = minion_top.vpu.ctrl.ex_req.sigs.vector;
                assign vpu_ctrl_ex_req_sigs_shsw                            = minion_top.vpu.ctrl.ex_req.sigs.shsw;
                assign vpu_ctrl_ex_req_sigs_txfma                           = minion_top.vpu.ctrl.ex_req.sigs.txfma;
                assign vpu_ctrl_mask_f2_regmask_store_all                   = minion_top.vpu.ctrl.mask.f2_regmask_store_all;
                assign vpu_ctrl_mask_f2_cmd                                 = minion_top.vpu.ctrl.mask.f2_cmd[6:0];
                assign vpu_ctrl_mask_f3_regmask_from_short                  = minion_top.vpu.ctrl.mask.f3_regmask_from_short;
                assign vpu_ctrl_mask_f3_regmask_waddr                       = minion_top.vpu.ctrl.mask.f3_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f3_regmask_store                       = minion_top.vpu.ctrl.mask.f3_regmask_store[7:0];
                assign vpu_ctrl_mask_f2_regmask_store_to_mask               = minion_top.vpu.ctrl.mask.f2_regmask_store_to_mask[7:0];
                assign vpu_ctrl_mask_f4_regmask_store                       = minion_top.vpu.ctrl.mask.f4_regmask_store[7:0];
                assign vpu_ctrl_mask_f5_regmask_store                       = minion_top.vpu.ctrl.mask.f5_regmask_store[7:0];
                assign vpu_ctrl_mask_f6_regmask_store                       = minion_top.vpu.ctrl.mask.f6_regmask_store[7:0];
                assign vpu_ctrl_mask_f7_regmask_store                       = minion_top.vpu.ctrl.mask.f7_regmask_store[7:0];
                assign vpu_ctrl_mask_f8_regmask_store                       = minion_top.vpu.ctrl.mask.f8_regmask_store[7:0];
                assign vpu_ctrl_f3_thread_id                                = minion_top.vpu.ctrl.f3_thread_id;
                assign vpu_ctrl_f8_regmask_from_txfma                       = minion_top.vpu.ctrl.f8_regmask_from_txfma;
                assign vpu_ctrl_mask_f8_regmask_waddr                       = minion_top.vpu.ctrl.mask.f8_regmask_waddr[2:0];
                assign vpu_ctrl_mask_f8_regmask_store_sel                   = minion_top.vpu.ctrl.mask.f8_regmask_store[7:0];
                assign vpu_ctrl_f8_thread_id                                = minion_top.vpu.ctrl.f8_thread_id;
                assign vpu_ctrl_ex_regfile_raddr1                           = minion_top.vpu.ctrl.ex_regfile_raddr1[4:0];
                assign vpu_ctrl_ex_regfile_raddr2                           = minion_top.vpu.ctrl.ex_regfile_raddr2[4:0];
                assign vpu_ctrl_ex_regfile_raddr3                           = minion_top.vpu.ctrl.ex_regfile_raddr3[4:0];
                assign vpu_ctrl_ex_regfile_waddr                            = minion_top.vpu.ctrl.ex_core_inst[11:7];
                assign vpu_ctrl_ex_req_sigs_ren1                            = minion_top.vpu.ctrl.ex_req.sigs.ren1;
                assign vpu_ctrl_ex_req_sigs_ren2                            = minion_top.vpu.ctrl.ex_req.sigs.ren2;
                assign vpu_ctrl_ex_req_sigs_ren3                            = minion_top.vpu.ctrl.ex_req.sigs.ren3;
                assign vpu_ctrl_f4_ctrl_valid_qual                          = minion_top.vpu.ctrl.f4_ctrl_valid_qual;
                assign vpu_ctrl_f4_regfile_thrid_l                          = minion_top.vpu.ctrl.f4_regfile_thrid_l ;
                assign vpu_ctrl_f4_regfile_waddr_l                          = minion_top.vpu.ctrl.f4_regfile_waddr_l[4:0];
                assign vpu_ctrl_f4_regfile_wmask_l                          = {minion_top.vpu.VPU_LANES[7].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[6].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[5].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[4].lane.bypass.f4_regfile_wmask_l_muxed,
                         				                       minion_top.vpu.VPU_LANES[3].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[2].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[1].lane.bypass.f4_regfile_wmask_l_muxed,minion_top.vpu.VPU_LANES[0].lane.bypass.f4_regfile_wmask_l_muxed};
                assign vpu_ctrl_f3_ctrl_valid_qual                          = minion_top.vpu.ctrl.f3_ctrl_valid_qual;
                assign vpu_ctrl_f3_regfile_waddr                            = minion_top.vpu.ctrl.f3_regfile_waddr[4:0];
                assign vpu_ctrl_f3_regfile_wmask                            = minion_top.vpu.ctrl.f3_regfile_wmask[7:0];
                assign vpu_ctrl_f4_regfile_waddr                            = minion_top.vpu.ctrl.f4_regfile_waddr[4:0];
                assign vpu_ctrl_f4_regfile_wmask                            = minion_top.vpu.ctrl.f4_regfile_wmask[7:0];
                assign vpu_ctrl_f4_thread_id                                = minion_top.vpu.ctrl.f4_thread_id;
                assign vpu_ctrl_f5_ctrl_valid_qual                          = minion_top.vpu.ctrl.f5_ctrl_valid_qual;
                assign vpu_ctrl_f5_regfile_waddr                            = minion_top.vpu.ctrl.f5_regfile_waddr[4:0];
                assign vpu_ctrl_f5_regfile_wmask                            = minion_top.vpu.ctrl.f5_regfile_wmask[7:0];
                assign vpu_ctrl_f5_thread_id                                = minion_top.vpu.ctrl.f5_thread_id;
                assign vpu_ctrl_f6_ctrl_valid_qual                          = minion_top.vpu.ctrl.f6_ctrl_valid_qual;
                assign vpu_ctrl_f6_regfile_waddr                            = minion_top.vpu.ctrl.f6_regfile_waddr[4:0];
                assign vpu_ctrl_f6_regfile_wmask                            = minion_top.vpu.ctrl.f6_regfile_wmask[7:0];
                assign vpu_ctrl_f6_thread_id                                = minion_top.vpu.ctrl.f6_thread_id;
                assign vpu_ctrl_f7_ctrl_valid_qual                          = minion_top.vpu.ctrl.f7_ctrl_valid_qual;
                assign vpu_ctrl_f7_regfile_waddr                            = minion_top.vpu.ctrl.f7_regfile_waddr[4:0];
                assign vpu_ctrl_f7_regfile_wmask                            = minion_top.vpu.ctrl.f7_regfile_wmask[7:0];
                assign vpu_ctrl_f7_thread_id                                = minion_top.vpu.ctrl.f7_thread_id;
                assign vpu_ctrl_f8_regfile_waddr                            = minion_top.vpu.ctrl.f8_regfile_waddr[4:0];
                assign vpu_ctrl_f8_regfile_wmask                            = minion_top.vpu.ctrl.f8_regfile_wmask[7:0];
                assign vpu_ctrl_f8_ctrl_valid_qual                          = minion_top.vpu.ctrl.f8_ctrl_valid_qual;
                assign vpu_ctrl_f8_ctrl_sigs_txfma                          = minion_top.vpu.ctrl.f8_ctrl_sigs.txfma;

                // tensor reduce
                assign vpu_ctrl_reduce_start                                = minion_top.vpu.ctrl.ml.tensorreduce.reduce_start;
                assign vpu_ctrl_reduce_start_reg                            = minion_top.vpu.ctrl.ml.tensorreduce.reduce_ctrl.reduce.start_reg[4:0];
                assign vpu_ctrl_reduce_num_regs                             = minion_top.vpu.ctrl.ml.tensorreduce.reduce_ctrl.reduce.num_regs[6:0];
                assign vpu_ctrl_reduce_op                                   = minion_top.vpu.ctrl.ml.tensorreduce.reduce_ctrl.reduce.op[3:0];
	
endmodule: neigh_vpu_cvg

	bind minion_top neigh_vpu_cvg neigh_vpu_cov(.clock(clock));

`endif
`endif
