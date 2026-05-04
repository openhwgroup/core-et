// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

  typedef struct packed {
   logic anyselected;
   logic anyunavailable;
   logic allhavereset;
   logic anyhavereset;
   logic allresumeack;
   logic anyresumeack;
   logic allrunning;
   logic anyrunning;
   logic allhalted;
   logic anyhalted1;
   logic anyhalted0;
} esr_and_or_tree_L1_t;

typedef struct packed {
   logic [5:0] sel_taps_feedback_shire;
   logic [5:0] sel_taps_feedback_neigh;
   logic [23:0] sel_taps_neigh;
} esr_clk_dly_ctl_t;

typedef struct packed {
   logic dll_muxsel;
   logic [2:0] muxsel;
} esr_clkmux_ctl_t;

typedef struct packed {
   logic dll_enable;
   logic [1:0] pclk_sel;
   logic lock_reset_disable;
   logic [2:0] reg_num;
   logic [2:0] reg_first;
   logic write;
   logic run;
   logic enable;
   logic reset;
} esr_dll_auto_config_t;

typedef struct packed {
   logic [1:0] pclk_sel;
   logic lock_reset_disable;
   logic [3:0] reg_num;
   logic [5:0] reg_first;
   logic write;
   logic run;
   logic enable;
   logic reset;
} esr_pll_auto_config_t;

   typedef struct packed {
   logic [2:0] sc_mbt_bc;
   logic sc_mbt_test_rnm;
   logic sc_mbt_test1;
   logic [2:0] sc_mbt_wpulse;
   logic [2:0] sc_mbt_wa;
   logic [1:0] sc_mbt_ra;
   logic [3:0] sc_mbt_rm;
   logic sc_mbt_rme;
   logic [2:0] sc_mbs_bc;
   logic sc_mbs_test_rnm;
   logic sc_mbs_test1;
   logic [2:0] sc_mbs_wpulse;
   logic [2:0] sc_mbs_wa;
   logic [1:0] sc_mbs_ra;
   logic [3:0] sc_mbs_rm;
   logic sc_mbs_rme;
} esr_shire_cache_ram_cfg1_t;
typedef struct packed {
   logic [2:0] sc_mbd_bc;
   logic sc_mbd_test_rnm;
   logic sc_mbd_test1;
   logic [2:0] sc_mbd_wpulse;
   logic [2:0] sc_mbd_wa;
   logic [1:0] sc_mbd_ra;
   logic [3:0] sc_mbd_rm;
   logic sc_mbd_rme;
} esr_shire_cache_ram_cfg2_t;
typedef struct packed {
   logic [2:0] sc_mbb_bc;
   logic sc_mbb_test_rnm;
   logic sc_mbb_test1;
   logic [2:0] sc_mbb_wpulse;
   logic [2:0] sc_mbb_wa;
   logic [1:0] sc_mbb_ra;
   logic [3:0] sc_mbb_rm;
   logic sc_mbb_rme;
   logic [2:0] sc_mbq_bc;
   logic sc_mbq_test_rnm;
   logic sc_mbq_test1;
   logic [2:0] sc_mbq_wpulse;
   logic [2:0] sc_mbq_wa;
   logic [1:0] sc_mbq_ra;
   logic [3:0] sc_mbq_rm;
   logic sc_mbq_rme;
} esr_shire_cache_ram_cfg3_t;
typedef struct packed {
   logic [2:0] sc_mbr_1p_bc;
   logic sc_mbr_1p_test_rnm;
   logic sc_mbr_1p_test1;
   logic [2:0] sc_mbr_1p_wpulse;
   logic [2:0] sc_mbr_1p_wa;
   logic [1:0] sc_mbr_1p_ra;
   logic [3:0] sc_mbr_1p_rm;
   logic sc_mbr_1p_rme;
   logic [2:0] sc_mbr_2p_bc;
   logic sc_mbr_2p_test_rnm;
   logic sc_mbr_2p_test1;
   logic [2:0] sc_mbr_2p_wpulse;
   logic [2:0] sc_mbr_2p_wa;
   logic [1:0] sc_mbr_2p_ra;
   logic [3:0] sc_mbr_2p_rm;
   logic sc_mbr_2p_rme;
   logic [2:0] sc_mbi_bc;
   logic sc_mbi_test_rnm;
   logic sc_mbi_test1;
   logic [2:0] sc_mbi_wpulse;
   logic [2:0] sc_mbi_wa;
   logic [1:0] sc_mbi_ra;
   logic [3:0] sc_mbi_rm;
   logic sc_mbi_rme;
} esr_shire_cache_ram_cfg4_t;

 typedef struct packed {
   logic [1:0] tbox3_id;
   logic [1:0] tbox2_id;
   logic [1:0] tbox1_id;
   logic [1:0] tbox0_id;
   logic rbox_en;
   logic [3:0] tbox_en;
   logic [3:0] neigh_en;
   logic cache_en;
   logic [7:0] shire_id;
} esr_shire_config_t;

typedef struct packed {
   logic qos_dram;
} esr_uc_config_t;