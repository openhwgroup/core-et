`ifdef EVL_COVERAGE_ON

`ifndef __MINION_CORE_DCACHE_CVG
`define __MINION_CORE_DCACHE_CVG

`include "dv/minion_common/cov/minion_core_tensor_cvg.svh"


//
// Note: search for keyword "FUTURE".
//



// (8/27/19) ? already defined in rtl/inc/dcache_types.vh and imported as evl_minion_rtl_pkg in evl_minion_vc_pkg.sv;
//           ? temporary commented out; Rob will review this; 
//typedef struct packed
//{
//   logic pf_ld; // Page fault load
//   logic pf_st; // Page fault store
//   logic mf_ld; // Misaligned fault load
//   logic mf_st; // Misaligned fault store
//   logic af_ld; // Access fault load
//   logic af_st; // Access fault store
//   logic ps_ld; // Page-split fault load
//   logic ps_st; // Page-split fault store
//} dcache_tag_xcpt;


class minion_core_dcache_cvg extends minion_core_tensor_cvg;

   // ============================================================
   // Collecting coverage related to DCache
   // ============================================================

//`ifndef USE_PSHIRE_ENV

   covergroup cg_dcache_general with function sample(evl_dcache_tag_xcpt dcache_xcpt,
                                                     logic[3:0]      bypass,
                                                     bit[4:0]        dcache_cmd,
                                                     bit[3:0]        dcache_dtype,
                                                     bit             core_req_valid,
                                                     bit             s2_valid_qual,
                                                     bit             mh_req_ready,
                                                     bit[1:0]        mh_tag_match,
                                                     bit             s2_nack_miss,
                                                     bit[1:0]        s2_mh_req_assigned,
                                                     bit             s2_mh_req_cacheable,
                                                     bit             s2_mh_req_thread,
                                                     bit             s2_hit,
                                                     bit             s2_to_s3_en_store_byp,
                                                     bit[3:0]        s3_store_bypass,
                                                     bit             id_rq_full,
                                                     bit             id_ba_full,
                                                     bit[4:0]        s2_cmd,
                                                     bit[3:0]        s2_dtype,
                                                     bit             s2_rq_renack,
                                                     bit             s1_has_xcpt,
                                                     logic[4:0]      s1_req_cmd,
                                                     logic[3:0]      s1_req_typ,
                                                     bit             s1_valid,
                                                     bit[2:0]        gsc_progress,
                                                     bit[1:0]        gsc_progress_save,
                                                     bit             s2_misaligned_2nd,
                                                     bit             s2_valid_masked,
                                                     logic[7:0]      dcache_ps_mask,
                                                     logic           min_frontend_clock_gate_disable,
                                                     logic           min_dcache_clock_gate_disable,
                                                     logic           min_vputrans_clock_gate_disable,
                                                     logic           min_vputima_clock_gate_disable,
                                                     logic           min_vpulane_clock_gate_disable,
                                                     logic           min_intpipe_clock_gate_disable);

      option.name = "cg_dcache_general";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      //
      // -------------- replay queue full ------------------
      //
      cp_replay_queue_full : coverpoint id_rq_full {
         bins dcache_rq_full = {1'h1};
      }


      //
      // --------------- buffer array full ------------------
      //
      cp_buffer_array_full : coverpoint id_ba_full {
         bins dcache_ba_full = {1'h1};
      }


      //
      // -------- core request coverage - commands ----------
      //
      cp_dcache_s0_cmd : coverpoint dcache_cmd iff (core_req_valid) {
                // from minion_types.vh
                bins dcache_cmd_XRD       = {5'h0};
                bins dcache_cmd_XWR       = {5'h1};
         ignore_bins dcache_cmd_PFR       = {5'h2};
         ignore_bins dcache_cmd_PFW       = {5'h3};
                bins dcache_cmd_XA_SWAP   = {5'h4};
         ignore_bins dcache_cmd_FLUSH_ALL = {5'h5};
         ignore_bins dcache_cmd_XLR       = {5'h6};
         ignore_bins dcache_cmd_XSC       = {5'h7};
                bins dcache_cmd_XA_ADD    = {5'h8};
                bins dcache_cmd_XA_XOR    = {5'h9};
                bins dcache_cmd_XA_OR     = {5'ha};
                bins dcache_cmd_XA_AND    = {5'hb};
                bins dcache_cmd_XA_MIN    = {5'hc};
                bins dcache_cmd_XA_MAX    = {5'hd};
                bins dcache_cmd_XA_MINU   = {5'he};
                bins dcache_cmd_XA_MAXU   = {5'hf};
         ignore_bins dcache_cmd_FLUSH     = {5'h10};
         ignore_bins dcache_cmd_PRODUCE   = {5'h11};
         ignore_bins dcache_cmd_CLEAN     = {5'h13};
                bins dcache_cmd_G32       = {5'h19};
                bins dcache_cmd_SC32      = {5'h1a};
                bins dcache_cmd_XWRB      = {5'h1b};
                bins dcache_cmd_XRDB      = {5'h1c};
                bins dcache_cmd_XA_MIN_PS = {5'h1d};
                bins dcache_cmd_XA_MAX_PS = {5'h1f};
      }


      //
      // --------- core request coverage - data types ----
      //
      cp_dcache_s0_dtype : coverpoint dcache_dtype iff (core_req_valid) {
         // from minion_types.vh
         bins dcache_type_B       = {4'h0};
         bins dcache_type_H       = {4'h1};
         bins dcache_type_W       = {4'h2};
         bins dcache_type_D       = {4'h3};
         bins dcache_type_BU      = {4'h4};
         bins dcache_type_HU      = {4'h5};
         bins dcache_type_WU      = {4'h6};
         bins dcache_type_PS      = {4'hb};
         bins dcache_type_PS_BR   = {4'ha};
         bins dcache_type_PS_GS8  = {4'hc};
         bins dcache_type_PS_GS16 = {4'he};
         bins dcache_type_PS_GS32 = {4'hd};
      }


      //
      // --------- core request coverage - mask_bits ----
      //
      cp_dcache_s0_mask : coverpoint dcache_ps_mask iff (core_req_valid && (dcache_dtype >= 4'hb)) {
                  bins dcache_ps_mask_all0 = {8'b0};
         wildcard bins dcache_ps_mask_bit0 = {8'b????_???1} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit1 = {8'b????_??1?} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit2 = {8'b????_?1??} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit3 = {8'b????_1???} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit4 = {8'b???1_????} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit5 = {8'b??1?_????} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit6 = {8'b?1??_????} iff (dcache_ps_mask != 8'hff);
         wildcard bins dcache_ps_mask_bit7 = {8'b1???_????} iff (dcache_ps_mask != 8'hff);
                  bins dcache_ps_mask_all1 = {8'hff};
      }


      //
      // --------- core request coverage - cross of vector types vs mask_bits ----
      //
      cross_dcache_s0_ps_type_vs_mask : cross cp_dcache_s0_dtype, cp_dcache_s0_mask {
         ignore_bins not_vector_type = cross_dcache_s0_ps_type_vs_mask with (cp_dcache_s0_dtype < 4'hb);
      }


      //
      // ------------------ exceptions -------------
      //
      cp_dcache_xcpt_pf_ld : coverpoint dcache_xcpt.pf_ld { bins dcache_xcpt_pf_ld = {1'h1}; }
      cp_dcache_xcpt_pf_st : coverpoint dcache_xcpt.pf_st { bins dcache_xcpt_pf_st = {1'h1}; }
//    cp_dcache_xcpt_mf_ld : coverpoint dcache_xcpt.mf_ld { bins dcache_xcpt_mf_ld = {1'h1}; } // misaligned fault not longer generated, RTLMIN-3211 & RTLMIN-3405
//    cp_dcache_xcpt_mf_st : coverpoint dcache_xcpt.mf_st { bins dcache_xcpt_mf_st = {1'h1}; }
      cp_dcache_xcpt_af_ld : coverpoint dcache_xcpt.af_ld { bins dcache_xcpt_af_ld = {1'h1}; }
      cp_dcache_xcpt_af_st : coverpoint dcache_xcpt.af_st { bins dcache_xcpt_af_st = {1'h1}; }
      cp_dcache_xcpt_ps_ld : coverpoint dcache_xcpt.ps_ld { bins dcache_xcpt_ps_ld = {1'h1}; }
      cp_dcache_xcpt_ps_st : coverpoint dcache_xcpt.ps_st { bins dcache_xcpt_ps_st = {1'h1}; }


      //
      // ------------- store bypass data -------------
      //
      cp_dcache_store_bypass : coverpoint bypass iff (s2_to_s3_en_store_byp && s3_store_bypass) {
         bins s3_store_bypass       = {3'h1};
         bins s4_store_bypass       = {3'h2};
         bins s2_s4_store_bypass    = {3'h3};
         bins s5_store_bypass       = {3'h4};
         bins s3_s5_store_bypass    = {3'h5};
         bins s4_s5_store_bypass    = {3'h6};
         bins s3_s4_s5_store_bypass = {3'h7};
      }


      //
      // -------------------- misses ---------------
      //
      cp_dcache_miss_in_mh : coverpoint mh_req_ready iff (mh_tag_match && s2_valid_qual) {
         bins dcache_sec_miss = {1'h1};
      }

      cp_dcache_miss_with_mh_busy : coverpoint s2_nack_miss iff (s2_valid_qual) {
         bins dcache_mh_busy = {1'h1};
      }

      cp_dcache_mh_assigned_to_thread : coverpoint s2_mh_req_assigned iff (~s2_mh_req_cacheable && s2_valid_qual) {
         bins mh0_thread0 = {2'h1} iff (s2_mh_req_thread == 1'h0);
         bins mh1_thread1 = {2'h2} iff (s2_mh_req_thread == 1'h1);
      }


      //
      // ------ cross coverage gsc inst vs. dtype (s1) -----
      //
      cp_dcache_s1_cmd : coverpoint s1_req_cmd iff (s1_valid) {
         bins dcache_cmd_XRD       = {5'h0};
         bins dcache_cmd_XWR       = {5'h1};
         bins dcache_cmd_XA_SWAP   = {5'h4};
         bins dcache_cmd_XA_ADD    = {5'h8};
         bins dcache_cmd_XA_XOR    = {5'h9};
         bins dcache_cmd_XA_OR     = {5'ha};
         bins dcache_cmd_XA_AND    = {5'hb};
         bins dcache_cmd_XA_MIN    = {5'hc};
         bins dcache_cmd_XA_MAX    = {5'hd};
         bins dcache_cmd_XA_MINU   = {5'he};
         bins dcache_cmd_XA_MAXU   = {5'hf};
         bins dcache_cmd_G32       = {5'h19};
         bins dcache_cmd_SC32      = {5'h1a};
         bins dcache_cmd_XWRB      = {5'h1b};
         bins dcache_cmd_XRDB      = {5'h1c};
         bins dcache_cmd_XA_MIN_PS = {5'h1d};
         bins dcache_cmd_XA_MAX_PS = {5'h1f};
      }

      cp_dcache_s1_dtype : coverpoint s1_req_typ iff (s1_valid) {
         bins dcache_type_B       = {4'h0};
         bins dcache_type_H       = {4'h1};
         bins dcache_type_W       = {4'h2};
         bins dcache_type_D       = {4'h3};
         bins dcache_type_BU      = {4'h4};
         bins dcache_type_HU      = {4'h5};
         bins dcache_type_WU      = {4'h6};
         bins dcache_type_PS      = {4'hb};
         bins dcache_type_PS_BR   = {4'ha};
         bins dcache_type_PS_GS8  = {4'hc};
         bins dcache_type_PS_GS16 = {4'he};
         bins dcache_type_PS_GS32 = {4'hd};
      }

      cp_dcache_min_frontend_clock_gate_disable : coverpoint min_frontend_clock_gate_disable {
         bins min_frontend_clock_gate_disable_ON  = {1'b1};
         bins min_frontend_clock_gate_disable_OFF = {1'b0};
      }

      cp_dcache_min_dcache_clock_gate_disable : coverpoint min_dcache_clock_gate_disable {
         bins min_dcache_clock_gate_disable_ON  = {1'b1};
         bins min_dcache_clock_gate_disable_OFF = {1'b0};
      }

      cp_dcache_min_vputrans_clock_gate_disable : coverpoint min_vputrans_clock_gate_disable {
         bins min_vputrans_clock_gate_disable_ON  = {1'b1};
         bins min_vputrans_clock_gate_disable_OFF = {1'b0};
      }

      cp_dcache_min_vputima_clock_gate_disable : coverpoint min_vputima_clock_gate_disable {
         bins min_vputima_clock_gate_disable_ON  = {1'b1};
         bins min_vputima_clock_gate_disable_OFF = {1'b0};
      }

      cp_dcache_min_vpulane_clock_gate_disable : coverpoint min_vpulane_clock_gate_disable {
         bins min_vpulane_clock_gate_disable_ON  = {1'b1};
         bins min_vpulane_clock_gate_disable_OFF = {1'b0};
      }

      cp_dcache_min_intpipe_clock_gate_disable : coverpoint min_intpipe_clock_gate_disable {
         bins min_intpipe_clock_gate_disable_ON  = {1'b1};
         bins min_intpipe_clock_gate_disable_OFF = {1'b0};
      }


      cross_dcache_cmd_vs_min_dcache_clock_gate_disable : cross cp_dcache_s1_cmd, cp_dcache_min_dcache_clock_gate_disable iff (min_dcache_clock_gate_disable == 1'b1);

      cross_dcache_cmd_vs_min_vputrans_clock_gate_disable : cross cp_dcache_s1_cmd, cp_dcache_min_vputrans_clock_gate_disable iff (min_vputrans_clock_gate_disable == 1'b1);

      cross_dcache_cmd_vs_min_vputima_clock_gate_disable : cross cp_dcache_s1_cmd, cp_dcache_min_vputima_clock_gate_disable iff (min_vputima_clock_gate_disable == 1'b1);

      cross_dcache_cmd_vs_min_vpulane_clock_gate_disable : cross cp_dcache_s1_cmd, cp_dcache_min_vpulane_clock_gate_disable iff (min_vpulane_clock_gate_disable == 1'b1);

      cross_dcache_cmd_vs_min_intpipe_clock_gate_disable : cross cp_dcache_s1_cmd, cp_dcache_min_intpipe_clock_gate_disable iff (min_intpipe_clock_gate_disable == 1'b1);


      //
      // ----------------  cross of commands and exceptions ----------
      //
      cross_dcache_cmd_vs_xcpt_pf_ld : cross cp_dcache_s1_cmd, cp_dcache_xcpt_pf_ld iff (s1_has_xcpt) {
         ignore_bins not_ld_inst = cross_dcache_cmd_vs_xcpt_pf_ld with ( ( (cp_dcache_s1_cmd == 5'h0) || (cp_dcache_s1_cmd == 5'h19) || (cp_dcache_s1_cmd == 5'h1c) )!=1 ); // ld xcpt could happen for XRD, G32, XRDB
      }

      cross_dcache_cmd_vs_xcpt_pf_st : cross cp_dcache_s1_cmd, cp_dcache_xcpt_pf_st iff (s1_has_xcpt) {
         ignore_bins not_st_inst = cross_dcache_cmd_vs_xcpt_pf_st with ( ( (cp_dcache_s1_cmd == 5'h1) || (cp_dcache_s1_cmd == 5'h1a) || (cp_dcache_s1_cmd == 5'h1b) ||
                                                                          (((cp_dcache_s1_cmd >= 5'h4) && (cp_dcache_s1_cmd <= 5'hf)) || (cp_dcache_s1_cmd >= 5'h1d)) )!=1 ); // st xcpt could happen for XWR, SC32, XWRB, AMO
      }

//    cross_dcache_cmd_vs_xcpt_mf_ld : cross cp_dcache_s1_cmd, cp_dcache_xcpt_mf_ld iff (s1_has_xcpt) {
//       ignore_bins not_ld_inst = cross_dcache_cmd_vs_xcpt_mf_ld with ( ( (cp_dcache_s1_cmd == 5'h0) || (cp_dcache_s1_cmd == 5'h19) || (cp_dcache_s1_cmd == 5'h1c) )!=1 );
//    }

//    cross_dcache_cmd_vs_xcpt_mf_st : cross cp_dcache_s1_cmd, cp_dcache_xcpt_mf_st iff (s1_has_xcpt) {
//       ignore_bins not_st_inst = cross_dcache_cmd_vs_xcpt_mf_st with ( ( (cp_dcache_s1_cmd == 5'h1) || (cp_dcache_s1_cmd == 5'h1a) || (cp_dcache_s1_cmd == 5'h1b) )!=1 ); // mf_st cannot happen for AMO
//    }

      cross_dcache_cmd_vs_xcpt_af_ld : cross cp_dcache_s1_cmd, cp_dcache_xcpt_af_ld iff (s1_has_xcpt) {
         ignore_bins not_ld_inst = cross_dcache_cmd_vs_xcpt_af_ld with ( ( (cp_dcache_s1_cmd == 5'h0) || (cp_dcache_s1_cmd == 5'h19) || (cp_dcache_s1_cmd == 5'h1c) )!=1 );
      }

      cross_dcache_cmd_vs_xcpt_af_st : cross cp_dcache_s1_cmd, cp_dcache_xcpt_af_st iff (s1_has_xcpt) {
         ignore_bins not_st_inst = cross_dcache_cmd_vs_xcpt_af_st with ( ( (cp_dcache_s1_cmd == 5'h1) || (cp_dcache_s1_cmd == 5'h1a) || (cp_dcache_s1_cmd == 5'h1b) ||
                                                                          (((cp_dcache_s1_cmd >= 5'h4) && (cp_dcache_s1_cmd <= 5'hf)) || (cp_dcache_s1_cmd >= 5'h1d)) )!=1 );
      }

      cross_dcache_cmd_vs_xcpt_ps_ld : cross cp_dcache_s1_cmd, cp_dcache_xcpt_ps_ld iff (s1_has_xcpt) {
         ignore_bins not_ld_inst = cross_dcache_cmd_vs_xcpt_ps_ld with ( ( (cp_dcache_s1_cmd == 5'h0) || (cp_dcache_s1_cmd == 5'h19) || (cp_dcache_s1_cmd == 5'h1c) )!=1 );
      }

      cross_dcache_cmd_vs_xcpt_ps_st : cross cp_dcache_s1_cmd, cp_dcache_xcpt_ps_st iff (s1_has_xcpt) {
         ignore_bins not_st_inst = cross_dcache_cmd_vs_xcpt_ps_st with ( ( (cp_dcache_s1_cmd == 5'h1) || (cp_dcache_s1_cmd == 5'h1a) || (cp_dcache_s1_cmd == 5'h1b) ||
                                                                          (((cp_dcache_s1_cmd >= 5'h4) && (cp_dcache_s1_cmd <= 5'hf)) || (cp_dcache_s1_cmd >= 5'h1d)) )!=1 );
      }


      //
      // -----------------  all possible regular gsc and gsc32 instructions vs dtypes
      //
      cp_gather_vs_dtype_s1 : cross cp_dcache_s1_cmd, cp_dcache_s1_dtype {
         ignore_bins not_gather   = cp_gather_vs_dtype_s1 with (cp_dcache_s1_dtype < 4'hc || (cp_dcache_s1_cmd == 5'h0 || cp_dcache_s1_cmd == 5'h19 || cp_dcache_s1_cmd == 5'h1c)!=1);
      }

      cp_scatter_vs_dtype_s1 : cross cp_dcache_s1_cmd, cp_dcache_s1_dtype {
         ignore_bins not_scatter = cp_scatter_vs_dtype_s1 with (cp_dcache_s1_dtype < 4'hc || (cp_dcache_s1_cmd == 5'h1 || cp_dcache_s1_cmd == 5'h1a || cp_dcache_s1_cmd == 5'h1b)!=1);
      }


      //
      // cross coverage gsc_vs_dtype vs. xcpt (s1)
      //
      cp_gsc_xcpt_pf_ld : cross cp_gather_vs_dtype_s1,  cp_dcache_xcpt_pf_ld iff (s1_has_xcpt);
      cp_gsc_xcpt_pf_st : cross cp_scatter_vs_dtype_s1, cp_dcache_xcpt_pf_st iff (s1_has_xcpt);
//    cp_gsc_xcpt_mf_ld : cross cp_gather_vs_dtype_s1,  cp_dcache_xcpt_mf_ld iff (s1_has_xcpt);
//    cp_gsc_xcpt_mf_st : cross cp_scatter_vs_dtype_s1, cp_dcache_xcpt_mf_st iff (s1_has_xcpt);
      cp_gsc_xcpt_af_ld : cross cp_gather_vs_dtype_s1,  cp_dcache_xcpt_af_ld iff (s1_has_xcpt);
      cp_gsc_xcpt_af_st : cross cp_scatter_vs_dtype_s1, cp_dcache_xcpt_af_st iff (s1_has_xcpt);
      cp_gsc_xcpt_ps_ld : cross cp_gather_vs_dtype_s1,  cp_dcache_xcpt_ps_ld iff (s1_has_xcpt);
      cp_gsc_xcpt_ps_st : cross cp_scatter_vs_dtype_s1, cp_dcache_xcpt_ps_st iff (s1_has_xcpt);


      //
      // ------ cross coverage regular gsc inst vs. dtype (s2) -----
      //
      cp_dcache_s2_cmd : coverpoint s2_cmd iff (s2_valid_qual) {
         bins dcache_cmd_XRD       = {5'h0};
         bins dcache_cmd_XWR       = {5'h1};
         bins dcache_cmd_XA_SWAP   = {5'h4};
         bins dcache_cmd_XA_ADD    = {5'h8};
         bins dcache_cmd_XA_XOR    = {5'h9};
         bins dcache_cmd_XA_OR     = {5'ha};
         bins dcache_cmd_XA_AND    = {5'hb};
         bins dcache_cmd_XA_MIN    = {5'hc};
         bins dcache_cmd_XA_MAX    = {5'hd};
         bins dcache_cmd_XA_MINU   = {5'he};
         bins dcache_cmd_XA_MAXU   = {5'hf};
         bins dcache_cmd_G32       = {5'h19};
         bins dcache_cmd_SC32      = {5'h1a};
         bins dcache_cmd_XWRB      = {5'h1b};
         bins dcache_cmd_XRDB      = {5'h1c};
         bins dcache_cmd_XA_MIN_PS = {5'h1d};
         bins dcache_cmd_XA_MAX_PS = {5'h1f};
      }

      cp_dcache_s2_dtype : coverpoint s2_dtype iff (s2_valid_qual) {
         bins dcache_type_B       = {4'h0};
         bins dcache_type_H       = {4'h1};
         bins dcache_type_W       = {4'h2};
         bins dcache_type_D       = {4'h3};
         bins dcache_type_BU      = {4'h4};
         bins dcache_type_HU      = {4'h5};
         bins dcache_type_WU      = {4'h6};
         bins dcache_type_PS      = {4'hb};
         bins dcache_type_PS_BR   = {4'ha};
         bins dcache_type_PS_GS8  = {4'hc};
         bins dcache_type_PS_GS16 = {4'he};
         bins dcache_type_PS_GS32 = {4'hd};
      }

      cp_gsc_vs_dtype_s2 : cross cp_dcache_s2_cmd, cp_dcache_s2_dtype {
         ignore_bins not_regular_gsc = cp_gsc_vs_dtype_s2 with (cp_dcache_s2_dtype < 4'hc || (cp_dcache_s2_cmd == 5'h0 || cp_dcache_s2_cmd == 5'h1 || cp_dcache_s2_cmd == 5'h1b || cp_dcache_s2_cmd == 5'h1c)!=1);
      }


      //
      // cross coverage gsc_vs_dtype vs. miss (s2)
      //
      cp_gsc_miss : cross cp_gsc_vs_dtype_s2, s2_hit {
         ignore_bins ignore_hit = cp_gsc_miss with (s2_hit == 1);
      }


      //
      // cross coverage gsc_vs_dtype vs. gsc_progress (s2)
      //
      cp_dcache_gsc_progress : coverpoint gsc_progress iff (gsc_progress_save) {
         bins gsc_progress_0 = {3'h0};
         bins gsc_progress_1 = {3'h1};
         bins gsc_progress_2 = {3'h2};
         bins gsc_progress_3 = {3'h3};
         bins gsc_progress_4 = {3'h4};
         bins gsc_progress_5 = {3'h5};
         bins gsc_progress_6 = {3'h6};
         bins gsc_progress_7 = {3'h7};
      }

      cp_gsc_resume : cross cp_gsc_vs_dtype_s2, cp_dcache_gsc_progress iff (gsc_progress_save);


      //
      // cross coverage gsc32_vs_dtype vs. store_bypass (s2)
      //
      cp_gsc32_vs_dtype_s2 : cross cp_dcache_s2_cmd, cp_dcache_s2_dtype {
         ignore_bins not_gsc32 = cp_gsc32_vs_dtype_s2 with (cp_dcache_s2_dtype < 4'hc || (cp_dcache_s2_cmd == 5'h19 || cp_dcache_s2_cmd == 5'h1a)!=1);
      }

      cp_gsc32_store_bypass : cross cp_gsc32_vs_dtype_s2, cp_dcache_store_bypass iff (s2_to_s3_en_store_byp && s3_store_bypass);


      //
      // instructions rearmed to replay queue (s2)
      //
      cp_dcache_replay_rearm : coverpoint s2_cmd iff (s2_rq_renack) {
         bins rearmed_cmd_XRD       = {5'h0};
         bins rearmed_cmd_XWR       = {5'h1};
         bins rearmed_cmd_XA_SWAP   = {5'h4};
         bins rearmed_cmd_XA_ADD    = {5'h8};
         bins rearmed_cmd_XA_XOR    = {5'h9};
         bins rearmed_cmd_XA_OR     = {5'ha};
         bins rearmed_cmd_XA_AND    = {5'hb};
         bins rearmed_cmd_XA_MIN    = {5'hc};
         bins rearmed_cmd_XA_MAX    = {5'hd};
         bins rearmed_cmd_XA_MINU   = {5'he};
         bins rearmed_cmd_XA_MAXU   = {5'hf};
         bins rearmed_cmd_G32       = {5'h19};
         bins rearmed_cmd_SC32      = {5'h1a};
         bins rearmed_cmd_XWRB      = {5'h1b};
         bins rearmed_cmd_XRDB      = {5'h1c};
         bins rearmed_cmd_XA_MIN_PS = {5'h1d};
         bins rearmed_cmd_XA_MAX_PS = {5'h1f};
      }


      //
      // 2nd pass instructions for misaligned (s2)
      //
      cp_dcache_2pass_misaligned : coverpoint s2_cmd iff (s2_misaligned_2nd && s2_hit && s2_valid_masked) {
         bins dcache_cmd_XRD_2nd_pass       = {5'h0};
         bins dcache_cmd_XWR_2nd_pass       = {5'h1};
         ignore_bins dcache_cmd_XA_SWAP_2nd_pass   = {5'h4};
         ignore_bins dcache_cmd_XA_ADD_2nd_pass    = {5'h8};
         ignore_bins dcache_cmd_XA_XOR_2nd_pass    = {5'h9};
         ignore_bins dcache_cmd_XA_OR_2nd_pass     = {5'ha};
         ignore_bins dcache_cmd_XA_AND_2nd_pass    = {5'hb};
         ignore_bins dcache_cmd_XA_MIN_2nd_pass    = {5'hc};
         ignore_bins dcache_cmd_XA_MAX_2nd_pass    = {5'hd};
         ignore_bins dcache_cmd_XA_MINU_2nd_pass   = {5'he};
         ignore_bins dcache_cmd_XA_MAXU_2nd_pass   = {5'hf};
         ignore_bins dcache_cmd_G32_2nd_pass       = {5'h19};
         ignore_bins dcache_cmd_SC32_2nd_pass      = {5'h1a};
         ignore_bins dcache_cmd_XWRB_2nd_pass      = {5'h1b};
         ignore_bins dcache_cmd_XRDB_2nd_pass      = {5'h1c};
         ignore_bins dcache_cmd_XA_MIN_PS_2nd_pass = {5'h1d};
         ignore_bins dcache_cmd_XA_MAX_PS_2nd_pass = {5'h1f};
      }

   endgroup : cg_dcache_general

   // ------------- arbiter grants -------------
   covergroup cg_dcache_id_arbiter with function sample(bit id_co_mr_grant,
                                                        bit id_dbg_grant,
                                                        bit id_wb_da_grant,
                                                        bit id_rq_grant,
                                                        bit id_co_pf_grant);

      option.name = "cg_dcache_id_arbiter";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      cp_dcache_id_co_mr_grant : coverpoint id_co_mr_grant { bins id_co_mr = {1'h1}; }
      cp_dcache_id_dbg_grant   : coverpoint id_dbg_grant   { bins id_dbg   = {1'h1}; }
      cp_dcache_id_wb_da_grant : coverpoint id_wb_da_grant { bins id_wb_da = {1'h1}; }
      cp_dcache_id_rq_grant    : coverpoint id_rq_grant    { bins id_rq    = {1'h1}; }
      cp_dcache_id_co_pf_grant : coverpoint id_co_pf_grant { bins id_co_pf = {1'h1}; }
//    cp_dcache_id_core_pipe_available : coverpoint id_core_pipe_available { bins id_core_pipe_available = {1'h1}; }

   endgroup : cg_dcache_id_arbiter


   covergroup cg_dcache_va_pa_translate with function sample(evl_dcache_tag_xcpt dcache_xcpt,
                                                             bit[2:0] s1_vm_sel,
                                                             bit s1_tlb_req_valid,
                                                             bit s1_tlb_miss,
                                                             bit s1_has_xcpt,
                                                             logic[4:0] s1_req_cmd);

      option.name = "cg_dcache_va_pa_translate";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      // ------------- VA to PA translation -------------

      cp_dcache_addr_translate : coverpoint s1_vm_sel iff (s1_tlb_req_valid) {
         bins dcache_va_pa_sel_ip  = {3'h0};
         bins dcache_va_pa_sel_co  = {3'h1};
         bins dcache_va_pa_sel_tl0 = {3'h2};
         bins dcache_va_pa_sel_tl1 = {3'h3};
         bins dcache_va_pa_sel_ts  = {3'h4};
         bins dcache_va_pa_sel_l2  = {3'h5};
      }

      // --------- VA to PA translation miss -----------

      cp_dcache_addr_translate_miss : coverpoint s1_vm_sel iff (s1_tlb_req_valid && s1_tlb_miss) {
         bins dcache_va_pa_ip_miss  = {3'h0};
         bins dcache_va_pa_co_miss  = {3'h1};
         bins dcache_va_pa_tl0_miss = {3'h2};
         bins dcache_va_pa_tl1_miss = {3'h3};
         bins dcache_va_pa_ts_miss  = {3'h4};
         bins dcache_va_pa_l2_miss  = {3'h5};
      }

       // ---------  VA to PA translation error from all agents ----------
//    cp_dcache_addr_translate_err :
      cp_dcache_addr_translate_agent : coverpoint s1_vm_sel iff (s1_has_xcpt) {
         bins dcache_va_pa_ip_xcpt  = {3'h0};
         bins dcache_va_pa_co_xcpt  = {3'h1};
         bins dcache_va_pa_tl0_xcpt = {3'h2};
         bins dcache_va_pa_tl1_xcpt = {3'h3};
         bins dcache_va_pa_ts_xcpt  = {3'h4};
         bins dcache_va_pa_l2_xcpt  = {3'h5};
      }

      cp_va_to_pa_agent_vs_pf_ld : cross cp_dcache_addr_translate_agent, dcache_xcpt.pf_ld iff (s1_has_xcpt);
      cp_va_to_pa_agent_vs_pf_st : cross cp_dcache_addr_translate_agent, dcache_xcpt.pf_st iff (s1_has_xcpt);
//    cp_va_to_pa_agent_vs_mf_ld : cross cp_dcache_addr_translate_agent, dcache_xcpt.mf_ld iff (s1_has_xcpt);
//    cp_va_to_pa_agent_vs_mf_st : cross cp_dcache_addr_translate_agent, dcache_xcpt.mf_st iff (s1_has_xcpt);
      cp_va_to_pa_agent_vs_af_ld : cross cp_dcache_addr_translate_agent, dcache_xcpt.af_ld iff (s1_has_xcpt);
      cp_va_to_pa_agent_vs_af_st : cross cp_dcache_addr_translate_agent, dcache_xcpt.af_st iff (s1_has_xcpt);
      cp_va_to_pa_agent_vs_ps_ld : cross cp_dcache_addr_translate_agent, dcache_xcpt.ps_ld iff (s1_has_xcpt);
      cp_va_to_pa_agent_vs_ps_st : cross cp_dcache_addr_translate_agent, dcache_xcpt.ps_st iff (s1_has_xcpt);

      // ------ VA to PA translation error from all core commands ------
//    cp_dcache_addr_translate_err_cmd :
      cp_dcache_s1_req_cmd : coverpoint s1_req_cmd {
         bins dcache_cmd_XRD       = {5'h0};
         bins dcache_cmd_XWR       = {5'h1};
         bins dcache_cmd_XA_SWAP   = {5'h4};
         bins dcache_cmd_XA_ADD    = {5'h8};
         bins dcache_cmd_XA_XOR    = {5'h9};
         bins dcache_cmd_XA_OR     = {5'ha};
         bins dcache_cmd_XA_AND    = {5'hb};
         bins dcache_cmd_XA_MIN    = {5'hc};
         bins dcache_cmd_XA_MAX    = {5'hd};
         bins dcache_cmd_XA_MINU   = {5'he};
         bins dcache_cmd_XA_MAXU   = {5'hf};
         bins dcache_cmd_G32       = {5'h19};
         bins dcache_cmd_SC32      = {5'h1a};
         bins dcache_cmd_XWRB      = {5'h1b};
         bins dcache_cmd_XRDB      = {5'h1c};
         bins dcache_cmd_XA_MIN_PS = {5'h1d};
         bins dcache_cmd_XA_MAX_PS = {5'h1f};
      }

      cp_va_to_pa_core_cmd_vs_pf_ld : cross cp_dcache_s1_req_cmd, dcache_xcpt.pf_ld iff (s1_has_xcpt);
      cp_va_to_pa_core_cmd_vs_pf_st : cross cp_dcache_s1_req_cmd, dcache_xcpt.pf_st iff (s1_has_xcpt);
//    cp_va_to_pa_core_cmd_vs_mf_ld : cross cp_dcache_s1_req_cmd, dcache_xcpt.mf_ld iff (s1_has_xcpt);
//    cp_va_to_pa_core_cmd_vs_mf_st : cross cp_dcache_s1_req_cmd, dcache_xcpt.mf_st iff (s1_has_xcpt);
      cp_va_to_pa_core_cmd_vs_af_ld : cross cp_dcache_s1_req_cmd, dcache_xcpt.af_ld iff (s1_has_xcpt);
      cp_va_to_pa_core_cmd_vs_af_st : cross cp_dcache_s1_req_cmd, dcache_xcpt.af_st iff (s1_has_xcpt);
      cp_va_to_pa_core_cmd_vs_ps_ld : cross cp_dcache_s1_req_cmd, dcache_xcpt.ps_ld iff (s1_has_xcpt);
      cp_va_to_pa_core_cmd_vs_ps_st : cross cp_dcache_s1_req_cmd, dcache_xcpt.ps_st iff (s1_has_xcpt);

   endgroup : cg_dcache_va_pa_translate


   covergroup cg_dcache_cache_ops with function sample(bit[3:0]    cache_op_req_cmd,
                                                       bit         cache_op_valid,
                                                       bit[1:0]    dcache_cfg_mode,
                                                       bit[3:0]    cop_req_cmd,
                                                       bit[1:0]    req_way,
                                                       bit[3:0]    req_set,
                                                       bit         lock_en,
                                                       bit         l2_scp_fill,
                                                       bit[3:0]    tensor_load_l2_scp_num_lines,
                                                       bit         tensor_load_l2_scp_use_tmask,
                                                       logic[15:0] tensor_load_l2_scp_tmask,
                                                       logic[4:0]  tensor_load_l2_scp_tmask_sum,
                                                       bit         cacheop_busy,
                                                       bit         cacheop_l2_busy,
                                                       bit         cache_op_req_is_l2,
                                                       bit         cache_op_valid_byp);

      option.name = "cg_dcache_cache_ops";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // ------------- cache_ops -------------
      cp_dcache_scp_cops : coverpoint cache_op_req_cmd iff (cache_op_valid && (dcache_cfg_mode == 2'b11)) {
                bins dcache_cop_lock_va   = {4'h0};
                bins dcache_cop_unlock_va = {4'h1};
                bins dcache_cop_flush_sw  = {4'h2};
                bins dcache_cop_evict_sw  = {4'h3};
                bins dcache_cop_prefetch  = {4'h4};
         ignore_bins dcache_cop_snoop     = {4'h5};
                bins dcache_cop_flush_va  = {4'h6};
                bins dcache_cop_evict_va  = {4'h7};
                bins dcache_cop_lock_sw   = {4'h8};
                bins dcache_cop_unlock_sw = {4'h9};
      }

      // ------------- locked set/ways -------------
      cp_dcache_locked_way : coverpoint req_way iff (cop_req_cmd == 4'h8) {
         bins dcache_way0 = {2'h0};
         bins dcache_way1 = {2'h1};
         bins dcache_way2 = {2'h2};
         bins dcache_way3 = {2'h3};
      }

      cp_dcache_locked_set : coverpoint req_set iff (cop_req_cmd == 4'h8) {
         bins dcache_set0  = {4'h0};
         bins dcache_set1  = {4'h1};
         bins dcache_set2  = {4'h2};
         bins dcache_set3  = {4'h3};
         bins dcache_set4  = {4'h4};
         bins dcache_set5  = {4'h5};
         bins dcache_set6  = {4'h6};
         bins dcache_set7  = {4'h7};
         bins dcache_set8  = {4'h8};
         bins dcache_set9  = {4'h9};
         bins dcache_set10 = {4'ha};
         bins dcache_set11 = {4'hb};
         bins dcache_set12 = {4'hc};
         bins dcache_set13 = {4'hd};
         bins dcache_set14 = {4'he};
         bins dcache_set15 = {4'hf};
      }

      cp_dcache_locked_setways : cross cp_dcache_locked_way, cp_dcache_locked_set iff (cop_req_cmd == 4'h8);

      // ------------- FUTURE: cop causing page cross -------------
//    cp_dcache_cop_page_cross : coverpoint ____ iff (______) { bins ________; }

      // ------------- TensorLoad L2 Scp -------------
      cp_dcache_tensor_load_l2_scp_num_lines : coverpoint tensor_load_l2_scp_num_lines iff ((cache_op_req_cmd == 4'h4) && cache_op_valid) {
         bins tensor_load_1_line             = {4'h0};
         bins tensor_load_multiple_lines[2]  = {[4'h1:4'he]};
         bins tensor_load_16_lines           = {4'hf};
      }

      cp_dcache_tensor_load_l2_scp_use_tmask : coverpoint tensor_load_l2_scp_use_tmask iff ((cache_op_req_cmd == 4'h4) && cache_op_valid) {
         bins tensor_load_l2scp_tmask     = {1'h1};
         bins tensor_load_l2scp_not_tmask = {1'h0};
      }

      cp_dcache_tensor_load_l2_scp_tmask_bits : coverpoint tensor_load_l2_scp_tmask_sum iff ((cache_op_req_cmd == 4'h4) && cache_op_valid && tensor_load_l2_scp_use_tmask) {
         bins tensor_load_tmask_nothing_masked = {0}      iff (tensor_load_l2_scp_num_lines >= 4'h0);
         bins tensor_load_tmask_mask_l[1]      = {[1:7]}  iff ((tensor_load_l2_scp_num_lines >= 4'h1)&&(tensor_load_l2_scp_num_lines <= 4'h6));
         bins tensor_load_tmask_mask_h[1]      = {[8:15]} iff ((tensor_load_l2_scp_num_lines >= 4'h7)&&(tensor_load_l2_scp_num_lines <= 4'he));
         bins tensor_load_tmask_fully_masked   = {16}     iff (tensor_load_l2_scp_num_lines == 4'hf);
      }

      cp_dcache_tensor_l2_scp : coverpoint l2_scp_fill iff ((cache_op_req_cmd == 4'h4) && cache_op_valid) {
        bins dcache_tensor_load_l2_scp = {1'h1};
      }

      cp_dcache_tensor_load_l2_scp_configs_0_mask_en : cross cp_dcache_tensor_l2_scp, cp_dcache_tensor_load_l2_scp_num_lines, cp_dcache_tensor_load_l2_scp_tmask_bits iff ((cache_op_req_cmd == 4'h4) && cache_op_valid && l2_scp_fill);

      cp_dcache_tensor_load_l2_scp_configs_0_mask_dis : cross cp_dcache_tensor_l2_scp, cp_dcache_tensor_load_l2_scp_num_lines iff ((cache_op_req_cmd == 4'h4) && cache_op_valid && l2_scp_fill);

      // ------------- FUTURE: simultaneous cop and copL2 -------------
      cp_dcache_cop_unit : coverpoint cacheop_busy {
         bins cache_op_unit_busy  = {1'b1};
         bins cache_op_unit_ready = {1'b0};
      }

      cp_dcache_cops_unit_l2 : coverpoint cacheop_l2_busy {
         bins cache_op_l2_unit_busy  = {1'b1};
         bins cache_op_l2_unit_ready = {1'b0};
      }

      cp_dcache_cops_simultaneous : cross cp_dcache_cop_unit, cp_dcache_cops_unit_l2;

      // ------------- FUTURE: cops and core req to copsL2 -------------
      cp_dcache_cops_l2_with_bypass : coverpoint cache_op_req_is_l2 iff (cache_op_valid_byp) {
         bins core_req_to_l2_while_bypass = {1'b1};
      }

   endgroup : cg_dcache_cache_ops


   covergroup cg_dcache_uncacheable with function sample(logic req_is_cacheable,
                                                         logic req_is_amo,
                                                         logic req_is_bypass,
                                                         logic[1:0] dcache_access_fault_xcpt,
                                                         logic req_is_read,
                                                         logic req_is_write,
                                                         logic[1:0] dcache_page_split_xcpt,
                                                         logic s1_access_is_page_split,
                                                         logic s1_pma_access_fault,
                                                         logic s1_mf_xcpt,
                                                         logic[1:0] dcache_page_fault_xcpt,
                                                         logic req_is_read_no_amo,
                                                         logic req_is_store);

      option.name = "cg_dcache_uncacheable";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      // --------- UC access -> access fault exception -----------
      cp_dcache_uc_af_xcpt : coverpoint dcache_access_fault_xcpt iff (req_is_cacheable == 0) {
         wildcard bins dcache_uc_pma_af_ld       = {2'b?1} iff (req_is_read_no_amo && s1_pma_access_fault);
         wildcard bins dcache_uc_pma_af_st       = {2'b1?} iff (req_is_store && s1_pma_access_fault);
         wildcard bins dcache_uc_misalign_af_ld  = {2'b?1} iff (req_is_read_no_amo && s1_mf_xcpt);
         wildcard bins dcache_uc_misalign_af_st  = {2'b1?} iff (req_is_write && s1_mf_xcpt);
      }

      // --------- UC -> page fault exception -----------
      cp_dcache_uc_pf_xcpt : coverpoint dcache_page_fault_xcpt iff (req_is_cacheable == 0) {
         wildcard bins dcache_uc_pf_ld = {2'b?1} iff (req_is_read);
         wildcard bins dcache_uc_pf_st = {2'b1?} iff (req_is_write);
      }

      // --------- UC -> page split exception -----------
      cp_dcache_uc_ps_xcpt : coverpoint dcache_page_split_xcpt iff ((req_is_cacheable == 0) && s1_access_is_page_split) {
         wildcard bins dcache_uc_ps_ld = {2'b?1} iff (req_is_read);
         wildcard bins dcache_uc_ps_st = {2'b1?} iff (req_is_write);
      }

   endgroup : cg_dcache_uncacheable


   covergroup cg_dcache_misaligned with function sample(logic req_is_cacheable,
                                                        logic req_is_amo,
                                                        logic req_is_bypass,
                                                        logic[1:0] dcache_access_fault_xcpt,
                                                        logic req_is_read,
                                                        logic req_is_write,
                                                        logic[1:0] dcache_page_split_xcpt,
                                                        logic s1_access_is_page_split,
                                                        logic[4:0] s1_req_cmd,
                                                        logic[3:0] s1_req_typ,
                                                        logic vm_enabled,
                                                        logic[6:0] s1_access_size,
                                                        logic[4:0] s1_req_addr,
                                                        bit s1_valid);

      option.name = "cg_dcache_misaligned";
      option.per_instance = 0;
      type_option.merge_instances = 1;


      cp_dcache_access_size : coverpoint s1_access_size iff (s1_valid) {
         bins s1_access_size_8b   = {7'h0};
         bins s1_access_size_16b  = {7'h1};
         bins s1_access_size_32b  = {7'h3};
         bins s1_access_size_64b  = {7'h7};
         bins s1_access_size_96b  = {7'hb};
         bins s1_access_size_128b = {7'hf};
         bins s1_access_size_160b = {7'h13};
         bins s1_access_size_192b = {7'h17};
         bins s1_access_size_224b = {7'h1b};
         bins s1_access_size_256b = {7'h1f};
      }

      // ------------- Access size vs offset -------------
      cp_dcache_misaligned_access_size_16b_vs_offset : coverpoint (2 - s1_req_addr[0]) iff (s1_valid && (s1_access_size == 7'h1)) {
         bins access_misaligned_0  = {2}; // aligned access
         bins access_misaligned_1B = {1}; // misaligned access
      }

      cp_dcache_misaligned_access_size_32b_vs_offset : coverpoint (4 - s1_req_addr[1:0]) iff (s1_valid && (s1_access_size == 7'h3)) {
         bins access_misaligned_0  = {4}; // aligned access
         bins access_misaligned_1B = {3}; // misaligned access
         bins access_misaligned_2B = {2}; // misaligned access
         bins access_misaligned_3B = {1}; // misaligned access
      }

      cp_dcache_misaligned_access_size_64b_vs_offset : coverpoint (8 - s1_req_addr[2:0]) iff (s1_valid && (s1_access_size == 7'h7)) {
         bins access_misaligned_0    = {8};    // aligned access
         bins access_misaligned_lo   = {5, 7}; // misaligned access
         bins access_misaligned_2_6B = {2, 6}; // misaligned access
         bins access_misaligned_4B   = {4};    // misaligned access
         bins access_misaligned_hi   = {1, 3}; // misaligned access
      }

      cp_dcache_misaligned_access_size_96b_vs_offset : coverpoint (12 - s1_req_addr[3:0]) iff (s1_valid && (s1_access_size == 7'hb)) {
         bins access_misaligned_0             = {12}; // aligned access
         bins access_misaligned_lo[1]         = cp_dcache_misaligned_access_size_96b_vs_offset with ( (item inside {[7:11]}) && (item%2==1) );
         bins access_misaligned_2_6_10_14B[1] = cp_dcache_misaligned_access_size_96b_vs_offset with ( (item inside {[2:10]}) && (item%4==2) );
         bins access_misaligned_4_8_12B[1]    = cp_dcache_misaligned_access_size_96b_vs_offset with ( (item inside {[4:8]}) && (item%4==0) );
         bins access_misaligned_hi[1]         = cp_dcache_misaligned_access_size_96b_vs_offset with ( (item inside {[1:5]}) && (item%2==1) );
      }

      cp_dcache_misaligned_access_size_128b_vs_offset : coverpoint (16 - s1_req_addr[3:0]) iff (s1_valid && (s1_access_size == 7'hf)) {
         bins access_misaligned_0             = {16}; // aligned access
         bins access_misaligned_lo[1]         = cp_dcache_misaligned_access_size_128b_vs_offset with ( (item inside {[9:15]}) && (item%2==1) );
         bins access_misaligned_2_6_10_14B[1] = cp_dcache_misaligned_access_size_128b_vs_offset with ( (item inside {[2:14]}) && (item%4==2) );
         bins access_misaligned_4_8_12B[1]    = cp_dcache_misaligned_access_size_128b_vs_offset with ( (item inside {[4:12]}) && (item%4==0) );
         bins access_misaligned_hi[1]         = cp_dcache_misaligned_access_size_128b_vs_offset with ( (item inside {[1:7]}) && (item%2==1) );
      }

      cp_dcache_misaligned_access_size_160b_vs_offset : coverpoint (20 - s1_req_addr[4:0]) iff (s1_valid && (s1_access_size == 7'h13)) {
         bins access_misaligned_0        = {20}; // aligned access
         bins access_misaligned_lo[1]    = cp_dcache_misaligned_access_size_160b_vs_offset with ( (item inside {[11:19]}) && (item%2==1) );
         bins access_misaligned_2_18B[1] = cp_dcache_misaligned_access_size_160b_vs_offset with ( (item inside {[2:18]}) && (item%4==2) );
         bins access_misaligned_4_16B[1] = cp_dcache_misaligned_access_size_160b_vs_offset with ( (item inside {[4:16]}) && (item%4==0) );
         bins access_misaligned_hi[1]    = cp_dcache_misaligned_access_size_160b_vs_offset with ( (item inside {[1:9]}) && (item%2==1) );
      }

      cp_dcache_misaligned_access_size_192b_vs_offset : coverpoint (24 - s1_req_addr[4:0]) iff (s1_valid && (s1_access_size == 7'h17)) {
         bins access_misaligned_0        = {24}; // aligned access
         bins access_misaligned_lo[1]    = cp_dcache_misaligned_access_size_192b_vs_offset with ( (item inside {[13:23]}) && (item%2==1) );
         bins access_misaligned_2_22B[1] = cp_dcache_misaligned_access_size_192b_vs_offset with ( (item inside {[2:22]}) && (item%4==2) );
         bins access_misaligned_4_20B[1] = cp_dcache_misaligned_access_size_192b_vs_offset with ( (item inside {[4:20]}) && (item%4==0) );
         bins access_misaligned_hi[1]    = cp_dcache_misaligned_access_size_192b_vs_offset with ( (item inside {[1:11]}) && (item%2==1) );
      }

      cp_dcache_misaligned_access_size_224b_vs_offset : coverpoint (28 - s1_req_addr[4:0]) iff (s1_valid && (s1_access_size == 7'h1b)) {
         bins access_misaligned_0        = {28}; // aligned access
         bins access_misaligned_lo[1]    = cp_dcache_misaligned_access_size_224b_vs_offset with ( (item inside {[15:27]}) && (item%2==1) );
         bins access_misaligned_2_26B[1] = cp_dcache_misaligned_access_size_224b_vs_offset with ( (item inside {[2:26]}) && (item%4==2) );
         bins access_misaligned_4_24B[1] = cp_dcache_misaligned_access_size_224b_vs_offset with ( (item inside {[4:24]}) && (item%4==0) );
         bins access_misaligned_hi[1]    = cp_dcache_misaligned_access_size_224b_vs_offset with ( (item inside {[1:13]}) && (item%2==1) );
      }

      cp_dcache_misaligned_access_size_256b_vs_offset : coverpoint (32 - s1_req_addr[4:0]) iff (s1_valid && (s1_access_size == 7'h1f)) {
         bins access_misaligned_0        = {32}; // aligned access
         bins access_misaligned_lo[1]    = cp_dcache_misaligned_access_size_256b_vs_offset with ( (item inside {[17:31]}) && (item%2==1) );
         bins access_misaligned_2_30B[1] = cp_dcache_misaligned_access_size_256b_vs_offset with ( (item inside {[2:30]}) && (item%4==2) );
         bins access_misaligned_4_28B[1] = cp_dcache_misaligned_access_size_256b_vs_offset with ( (item inside {[4:28]}) && (item%4==0) );
         bins access_misaligned_hi[1]    = cp_dcache_misaligned_access_size_256b_vs_offset with ( (item inside {[1:15]}) && (item%2==1) );
      }

      // --------- Missaligned access -> page split exception -----------
      cp_dcache_misaligned_page_split : coverpoint dcache_page_split_xcpt iff (s1_access_is_page_split && vm_enabled) {
         wildcard bins dcache_ps_ld  = {2'b?1} iff (req_is_read);
         wildcard bins dcache_ps_st  = {2'b1?} iff (req_is_write);
      }

      // -------- cuda commands ----------
      cp_dcache_cuda_cmd : coverpoint s1_req_cmd iff (req_is_bypass && (dcache_access_fault_xcpt> 0)) {
         bins dcache_cmd_XWRB      = {5'h1b}; //cuda
         bins dcache_cmd_XRDB      = {5'h1c}; //cuda
      }

      cp_dcache_cuda_dtype : coverpoint s1_req_typ iff (req_is_bypass && (dcache_access_fault_xcpt > 0)) {
         bins dcache_type_B       = {4'h0};
         bins dcache_type_H       = {4'h1};
         bins dcache_type_PS      = {4'hb};
         bins dcache_type_PS_GS8  = {4'hc};
         bins dcache_type_PS_GS16 = {4'he};
         bins dcache_type_PS_GS32 = {4'hd};
      }

      // --------- Missaligned accesses that bypass L1 (cuda) -----------
      cp_dcache_misaligned_l1bypass : cross cp_dcache_cuda_cmd, cp_dcache_cuda_dtype iff ((dcache_access_fault_xcpt>0) && req_is_bypass) {
         ignore_bins dcache_XRDB_B_or_H = cp_dcache_misaligned_l1bypass with ((cp_dcache_cuda_cmd == 5'h1c) && ((cp_dcache_cuda_dtype==4'h0) || (cp_dcache_cuda_dtype==4'h1)));
      }

      // -------- amo commands ----------
      cp_dcache_amo_cmd : coverpoint s1_req_cmd iff (req_is_amo && (dcache_access_fault_xcpt > 0)) {
         bins dcache_cmd_XA_ADD    = {5'h8}; //amo
         bins dcache_cmd_XA_XOR    = {5'h9};
         bins dcache_cmd_XA_OR     = {5'ha};
         bins dcache_cmd_XA_AND    = {5'hb};
         bins dcache_cmd_XA_MIN    = {5'hc};
         bins dcache_cmd_XA_MAX    = {5'hd};
         bins dcache_cmd_XA_MINU   = {5'he};
         bins dcache_cmd_XA_MAXU   = {5'hf};
         bins dcache_cmd_XA_MIN_PS = {5'h1d};
         bins dcache_cmd_XA_MAX_PS = {5'h1f};
      }

      // --------- amo data types ----
      cp_dcache_amo_typ : coverpoint s1_req_typ iff (req_is_amo && (dcache_access_fault_xcpt > 0)) {
         bins dcache_type_W       = {4'h2};
         bins dcache_type_D       = {4'h3};
         bins dcache_type_PS_GS32 = {4'hd};
      }

      // --------- Missaligned AMO access  -----------
      cp_dcache_misaligned_amo : cross cp_dcache_amo_cmd, cp_dcache_amo_typ iff ((dcache_access_fault_xcpt>0) && req_is_amo) {
         ignore_bins dcache_XA_PS_vs_W_or_D = cp_dcache_misaligned_amo with ((cp_dcache_amo_typ != 4'hd) && ((cp_dcache_amo_cmd==5'h1d) || (cp_dcache_amo_cmd==5'h1f)));
      }

   endgroup : cg_dcache_misaligned



    //
    // ------------------- FSM STATES ----------------------------
    //
    covergroup cg_dcache_fsm_states with function sample(logic[3:0] cache_op_state,
                                                         bit cacheop_busy,
                                                         logic[2:0] cache_op_l2_state,
                                                         bit cacheop_l2_busy,
                                                         logic[3:0] mh0_state,
                                                         logic[3:0] mh1_state,
                                                         logic[2:0] tl0_ctrl_state,
                                                         logic[2:0] tl1_ctrl_state,
                                                         logic[3:0] reduce_op_state,
                                                         logic[2:0] atrans_state,
                                                         logic wb_active,
                                                         logic[5:0] dcache_pipeline_state,
                                                         logic[1:0] texsend_req_state,
                                                         logic[1:0] texsend_pull_state);

      option.name = "cg_dcache_fsm_states";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      // --------- CacheOp unit fsm states -----------
      cp_dcache_cache_op_state : coverpoint cache_op_state iff (cacheop_busy) {
         ignore_bins dcache_cache_op_invalid         = {4'h0};
                bins dcache_cache_op_meta_read       = {4'h1};
                bins dcache_cache_op_meta_resp_wait  = {4'h2};
                bins dcache_cache_op_meta_resp       = {4'h3};
                bins dcache_cache_op_release         = {4'h4};
                bins dcache_cache_op_writeback_req   = {4'h5};
                bins dcache_cache_op_writeback_resp  = {4'h6};
                bins dcache_cache_op_meta_write      = {4'h7};
                bins dcache_cache_op_meta_write_lock = {4'h8};
                bins dcache_cache_op_next_operation  = {4'h9};
                bins dcache_cache_op_wait_tmask      = {4'ha};
                bins dcache_cache_op_l1_prefetch     = {4'hb};
                bins dcache_cache_op_wait_co_l2      = {4'hc};
                bins dcache_cache_op_wait_tlb        = {4'hd};
      }

      // --------- CacheOpL2 unit fsm states -----------
      cp_dcache_cache_op_l2_state : coverpoint cache_op_l2_state iff (cacheop_l2_busy) {
         ignore_bins dcache_cache_op_l2_invalid        = {3'h0};
                bins dcache_cache_op_l2_translate      = {3'h1};
                bins dcache_cache_op_l2_translate_wait = {3'h2};
                bins dcache_cache_op_l2_request        = {3'h3};
                bins dcache_cache_op_l2_next_operation = {3'h4};
                bins dcache_cache_op_l2_wait_tmask     = {3'h5};
      }

      // --------- TexSend unit req states -----------
      cp_dcache_texsend_req_state : coverpoint texsend_req_state {
         ignore_bins dcache_tex_ctrl_req_idle           = {2'h0};
                bins dcache_tex_ctrl_req_send_req_ready = {2'h1};
                bins dcache_tex_ctrl_req_send_req_stall = {2'h2};
      }

      // --------- TexSend unit pull states -----------
      cp_dcache_texsend_pull_state : coverpoint texsend_pull_state {
         ignore_bins dcache_tex_ctrl_pull_idle           = {2'h0};
                bins dcache_tex_ctrl_pull_wait_pull      = {2'h1};
                bins dcache_tex_ctrl_pull_send_data      = {2'h2};
                bins dcache_tex_ctrl_pull_send_next_data = {2'h3};
      }

      // --------- MH fsm states -----------
      cp_dcache_mh0_state : coverpoint mh0_state {
         ignore_bins dcache_mh_invalid        = {4'h0}; // MH is available
                bins dcache_mh_fill_req       = {4'h1}; // Mem acquire
                bins dcache_mh_fill_resp      = {4'h2}; // Waiting for the fill to finish
                bins dcache_mh_meta_write_req = {4'h3}; // Updates the meta state to the fill one
                bins dcache_mh_meta_hazard    = {4'h4}; // Hazard after meta write
                bins dcache_mh_uc_wait_idle   = {4'h5}; // Waits until other UCs are empty before starting a UC load
                bins dcache_mh_uc_load_req    = {4'h6}; // Requests the UC load data
                bins dcache_mh_uc_load_resp   = {4'h7}; // Waits for UC load data simply to keep the UC as outstanding
                bins dcache_mh_uc_store_wait  = {4'h8}; //
                bins dcache_mh_uc_store_req   = {4'h9}; // Sends the store UC request to the L2
                bins dcache_mh_uc_store_ack   = {4'ha}; // Waits for the L2 ack
      }

      cp_dcache_mh1_state : coverpoint mh1_state {
         ignore_bins dcache_mh_invalid        = {4'h0};
                bins dcache_mh_fill_req       = {4'h1};
                bins dcache_mh_fill_resp      = {4'h2};
                bins dcache_mh_meta_write_req = {4'h3};
                bins dcache_mh_meta_hazard    = {4'h4};
                bins dcache_mh_uc_wait_idle   = {4'h5};
                bins dcache_mh_uc_load_req    = {4'h6};
                bins dcache_mh_uc_load_resp   = {4'h7};
                bins dcache_mh_uc_store_wait  = {4'h8};
                bins dcache_mh_uc_store_req   = {4'h9};
                bins dcache_mh_uc_store_ack   = {4'ha};
      }

      // --------- TL unit fsm states -----------
      cp_dcache_tl0_ctrl_state : coverpoint tl0_ctrl_state {
         ignore_bins dcache_ml_ctrl_idle       = {3'h0}; // Default idle state
                bins dcache_ml_ctrl_wait_tmask = {3'h1}; // Wait for convolutional bits to be ready
                bins dcache_ml_ctrl_not_req    = {3'h2}; // State for no transformation, plain, writes
                bins dcache_ml_ctrl_int_trans  = {3'h3}; // Base state to implement interleaves
                bins dcache_ml_ctrl_int_req    = {3'h4}; // Interleave makes request to download lines
                bins dcache_ml_ctrl_int_int    = {3'h5}; // Interleave makes data interleaving and writes to scratchpad
                bins dcache_ml_ctrl_tra_req    = {3'h6}; // Transpose makes request to download lines
                bins dcache_ml_ctrl_tra_tra    = {3'h7}; // Transpose makes data transposition and writes to scratchpad
      }

      cp_dcache_tl1_ctrl_state : coverpoint tl1_ctrl_state {
         ignore_bins dcache_ml_ctrl_idle       = {3'h0};
                bins dcache_ml_ctrl_wait_tmask = {3'h1};
                bins dcache_ml_ctrl_not_req    = {3'h2};
                bins dcache_ml_ctrl_int_trans  = {3'h3};
                bins dcache_ml_ctrl_int_req    = {3'h4};
                bins dcache_ml_ctrl_int_int    = {3'h5};
                bins dcache_ml_ctrl_tra_req    = {3'h6};
                bins dcache_ml_ctrl_tra_tra    = {3'h7};
      }

      // --------- Reduce unit request fsm states -----------
      cp_dcache_reduce_op_state : coverpoint reduce_op_state {
         ignore_bins dcache_reduce_invalid     = {4'h0}; // Default idle state
                bins dcache_reduce_wait_tensor = {4'h1}; // Waits for TensorOp dependencies to be cleared
                bins dcache_reduce_new_req     = {4'h2}; // Decoding a new req
                bins dcache_reduce_send_ready  = {4'h3}; // Send that unit is ready to receive data (receiver only)
                bins dcache_reduce_wait_ready  = {4'h4}; // Waiting for the ready msg from receiver (sender only)
                bins dcache_reduce_send_data   = {4'h5}; // Send data to receiver (sender only)
                bins dcache_reduce_wait_data   = {4'h6}; // Waits for data from sender (receiver only)
                bins dcache_reduce_drain       = {4'h7}; // Waits for instruction to drain
                bins dcache_reduce_store_data  = {4'h8}; // Sends TensorStore data
      }

      // --------- Reduce unit address translation states -----------
      cp_dcache_reduce_atrans_state : coverpoint atrans_state {
         ignore_bins dcache_atrans_idle   = {3'h0}; // Address translation engine is idle
                bins dcache_atrans_loop   = {3'h1}; // Loop base state to do multiple address translations
                bins dcache_atrans_s0_req = {3'h2}; // Do request to s0 stage
                bins dcache_atrans_s1_req = {3'h3}; // Do request to s1 stage
                bins dcache_atrans_wait   = {3'h4}; // Wait for TLB
                bins dcache_atrans_flush  = {3'h5}; // Flush available addresses before going to idle state
                bins dcache_atrans_fail   = {3'h6}; // Failure or exception in address translation
                bins dcache_atrans_save   = {3'h7}; // Save translated address
      }

      // --------- WB unit req state -----------
      cp_dcache_wb_state : coverpoint wb_active {
         ignore_bins dcache_wb_idle = {1'h0};
                bins dcache_wb_busy = {1'h1};
      }

      // --------- Pipeline state -----------
      cp_dcache_pipeline_state : coverpoint dcache_pipeline_state {
         wildcard bins dcache_s0_busy = {6'b?????1};
         wildcard bins dcache_s1_busy = {6'b????1?};
         wildcard bins dcache_s2_busy = {6'b???1??};
         wildcard bins dcache_s3_busy = {6'b??1???};
         wildcard bins dcache_s4_busy = {6'b?1????};
         wildcard bins dcache_s5_busy = {6'b1?????};
      }

      // --------- FSM states cross -----------
      cross_cop_mh_wb_states : cross cp_dcache_mh0_state, cp_dcache_mh1_state, cp_dcache_cache_op_state, cp_dcache_cache_op_l2_state, cp_dcache_wb_state;

      cross_tload_mh_wb_states : cross cp_dcache_mh0_state, cp_dcache_mh1_state, cp_dcache_tl0_ctrl_state, cp_dcache_tl1_ctrl_state, cp_dcache_wb_state;

      cross_treduce_mh_wb_states : cross cp_dcache_mh0_state, cp_dcache_mh1_state, cp_dcache_reduce_op_state, cp_dcache_reduce_atrans_state, cp_dcache_wb_state;

   endgroup : cg_dcache_fsm_states


   //
   // ------------------- DCACHE AND VPU INTERACTION ----------------------
   //
   covergroup cg_dcache_vpu_interaction with function sample(logic scp_read_req_en,
                                                             logic s1_valid_masked,
                                                             logic s1_wb_da_valid,
                                                             logic s1_da_read_en,
                                                             logic dbg_da_read_en,
                                                             logic s1_translate_only_addr_load,
                                                             logic s1_valid_to_s2,
                                                             logic s1_is_msg,
                                                             logic s1_is_amo,
                                                             logic s1_is_bypass,
                                                             logic s1_nack_co_collide,
                                                             logic tfma_enabled,
                                                             logic[2:0] tenb_credits,
                                                             logic tenb_busy,
                                                             logic[1:0] tfma_status);

      option.name = "cg_dcache_vpu_interaction";
      option.per_instance = 0;
      type_option.merge_instances = 1;

      // --------- VPU asynchronously gets into DCache pipeline -----------
      cp_vpu_async_access : coverpoint scp_read_req_en {
         bins s1_valid_masked = {1} iff (s1_valid_masked);
         bins s1_wb_da_valid  = {1} iff (s1_wb_da_valid);
         bins s1_da_read_en   = {1} iff (s1_da_read_en);
         bins dbg_da_read_en  = {1} iff (dbg_da_read_en);
         bins s1_translate_only_addr_load    = {1} iff (s1_translate_only_addr_load);
         bins s1_valid_to_s2                 = {1} iff (s1_valid_to_s2);
         bins s1_valid_to_s2_not_msg_amo_byp = {1} iff (s1_valid_to_s2 && !s1_is_msg && !s1_is_amo && !s1_is_bypass);
         bins s1_nack_co_collide             = {1} iff (s1_nack_co_collide);
      }

      // --------- TENB credit consumption -----------
      cp_tenb_credits : coverpoint tenb_credits {
         bins credits[7] = {[0:6]};
      }

      cross_tenb_credits_tfma_enabled : cross cp_tenb_credits, tfma_enabled {
         ignore_bins credits_tfma_dis = cross_tenb_credits_tfma_enabled with ((cp_tenb_credits<2) && (tfma_enabled==0));
      }

      // ------ TL1 busy cross with FMA status -------
      cp_dcache_tfma_status : coverpoint tfma_status {
                bins tfma_tenb_inactive     = {0};
                bins tfma_busy_tenb_starts  = {1};
                bins tfma_starts_after_tenb = {2};
         ignore_bins tfma_starts_for_current_tenb = {3};
      }

      cross_tenb_tfma_status : cross tenb_busy, cp_dcache_tfma_status;

   endgroup : cg_dcache_vpu_interaction


// covergroup cg_dcache_chicken_bits with function sample(logic min_frontend_clock_gate_disable,
//                                                        logic min_dcache_clock_gate_disable,
//                                                        logic min_vputrans_clock_gate_disable,
//                                                        logic min_vputima_clock_gate_disable,
//                                                        logic min_vpulane_clock_gate_disable,
//                                                        logic min_intpipe_clock_gate_disable);

//    option.name = "cg_dcache_chicken_bits";
//    option.per_instance = 0;
//    type_option.merge_instances = 1;


//    cp_dcache_min_frontend_clock_gate_disable : coverpoint min_frontend_clock_gate_disable {
//       bins min_frontend_clock_gate_disable_ON  = {1'b1};
//       bins min_frontend_clock_gate_disable_OFF = {1'b0};
//    }

//    cp_dcache_min_dcache_clock_gate_disable : coverpoint min_dcache_clock_gate_disable {
//       bins min_dcache_clock_gate_disable_ON  = {1'b1};
//       bins min_dcache_clock_gate_disable_OFF = {1'b0};
//    }

//    cp_dcache_min_vputrans_clock_gate_disable : coverpoint min_vputrans_clock_gate_disable {
//       bins min_vputrans_clock_gate_disable_ON  = {1'b1};
//       bins min_vputrans_clock_gate_disable_OFF = {1'b0};
//    }

//    cp_dcache_min_vputima_clock_gate_disable : coverpoint min_vputima_clock_gate_disable {
//       bins min_vputima_clock_gate_disable_ON  = {1'b1};
//       bins min_vputima_clock_gate_disable_OFF = {1'b0};
//    }

//    cp_dcache_min_vpulane_clock_gate_disable : coverpoint min_vpulane_clock_gate_disable {
//       bins min_vpulane_clock_gate_disable_ON  = {1'b1};
//       bins min_vpulane_clock_gate_disable_OFF = {1'b0};
//    }

//    cp_dcache_min_intpipe_clock_gate_disable : coverpoint min_intpipe_clock_gate_disable {
//       bins min_intpipe_clock_gate_disable_ON  = {1'b1};
//       bins min_intpipe_clock_gate_disable_OFF = {1'b0};
//    }

// endgroup : cg_dcache_chicken_bits


   // -----------------------------------
   //     Covergroup instances
   // -----------------------------------
   function new();
      super.new();
      cg_dcache_general         = new();
      cg_dcache_id_arbiter      = new();
      cg_dcache_va_pa_translate = new();
      cg_dcache_cache_ops       = new();
      cg_dcache_uncacheable     = new();
      cg_dcache_misaligned      = new();
      cg_dcache_fsm_states      = new();
      cg_dcache_vpu_interaction = new();
//    cg_dcache_chicken_bits    = new();
   endfunction : new

endclass : minion_core_dcache_cvg

`endif
`endif
