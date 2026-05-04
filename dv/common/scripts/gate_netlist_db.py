# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


gatesim_db = {
    "PSHIRE_ET" 	: "/project/pd/release/A0/pshire_et_block/2020_04_04/2020_12_19_19:49:38_opportunistic_DEC19/pnr/outputs_icc2/write_data_Dec18/pshire_et_block.pt.v.gz",
    "PSHIRE_SS" 	: "/eng/cad/foundry/snps_tsmc/7nm/IP/v1.1/iip/DWC_ET_hard_pcie/latest/subsystem/dwc_pcie4_subsystem_x8_ns_gate_level.v /eng/cad/foundry/snps_tsmc/7nm/IP/v1.1/synopsys/dwc_pcie4phy_tsmc7ff_x4ns/latest/pma/behavior/15M_1X_h_1Xa_v_1Ya_h_5Y_vhvhv_2Yy2Yx2R/dwc_pcie4_pma_x4_ns_gate_level.v +define+DWC_PCIE4_X4NS_SIM_GATES+DWC_PCIE4_X4NS_SIM_MODE+DWC_PCIE4_X4NS_SHORT_RESET+PHY0_X4+PHY1_X4 +define+PSHIRE_SS_GATESIM_ENABLE +define+DWC_PCIE4_X4NS_RANDOM_SEED=1 +define+PRINTF_COND=0 +define+STOP_COND=0 +notimingchecks +no_notifier",
    "DCU"               : "/project/pd/release/A0/dft_control_unit/latest/syn/results/dft_control_unit.mapped.v",
    "DDR"       	: "",
    "MINSHIRE"  	: "/project/jenkins/gate_netlists/shire_top/etcust_128x64_1r1w_lram.v /project/jenkins/gate_netlists/shire_top/etcust_64x32_3r2w_vpurf.v /project/jenkins/gate_netlists/shire_top/minion_top.v /project/jenkins/gate_netlists/shire_top/neigh_top.v /project/jenkins/gate_netlists/shire_top/shire_cache_bank.v /project/jenkins/gate_netlists/shire_top/shire_channel_wrap.v /project/jenkins/gate_netlists/shire_top/shire_top_logic_wrapper.v /project/jenkins/gate_netlists/shire_top/shire_top.v",

    "IOSHIRE_MAXION"    : "/project/pd/release/A0/maxshire_top/2020_03_26/2020_12_03_06:00:00_TAPEOUT_release/pnr/outputs_icc2/eco62/maxshire_top.pt.v.gz /project/pd/release/A0/maxion_shire_channel/2020_03_24/2020_11_21_19:00:22_A0_final02_ColoredFillExtraction/pnr/outputs_icc2/final02_pdqa/maxion_shire_channel.pt.v.gz /project/pd/release/A0/MaxionTile/2020_03_08/2020_10_12_17:01:43_release_topeco8p5p6/pnr/outputs_icc2/topeco8p5p6/MaxionTile.pt.v.gz",
    "IOSHIRE_SPIO"   	: "/project/pd/release/A0/spio/2020_04_13/2020_12_05_17:22:40_efuse/pnr/outputs_icc2/write_data/spio.pt.v.gz",
    "IOSHIRE_PU" 	: "/project/pd/release/A0/pu/2020_03_22/2020_12_02_20:59:14_holdFix/pnr/outputs_icc2/write_data/pu.pt.v.gz",
    "IOSHIRE_MISC"   	: "/project/pd/release/A0/ioshire_misc/2020_04_09/2020_11_21_19:07:19_A0_final01_ColoredFillExtraction/pnr/outputs_icc2/final01_pdqa/ioshire_misc.pt.v.gz",
    "SPIO_NOC_WRAPPER" 	: "/project/pd/release/A0/spio_noc_wrapper/2020_03_24/2020_11_24_22:22:58_ColoredFillExtraction_All_block_level_check_Clean_with_PDQA/pnr/outputs_icc2/iter96_swap_vt_CM/spio_noc_wrapper.pt.v.gz",
    "PU_NOC_WRAPPER" 	: "/project/pd/release/A0/pu_noc_wrapper/2020_03_08/2020_12_04_01:38:35_fixing_5ps_ColoredFillExtraction/pnr/outputs_icc2/setup_fix_gr_paths_eco21/pu_noc_wrapper.pt.v.gz",
    "IOSHIRE_MESHSTOP_WRAPPER" : "/project/pd/release/A0/ioshire_meshstop_wrapper/2020_03_22/2020_12_05_13:44:02_pdqa_reviewed_timing_clean_ColoredFillExtraction/pnr/outputs_icc2/from_p_setup_eco_pd453_eco29/ioshire_meshstop_wrapper.pt.v.gz", 
    #"IOSHIRE_WRAPPER"   : "/project/pd/release/A0/ioshire_wrapper/2020_03_30/2020_12_20_15:10:20_opportunistic_Dec20/pnr/outputs_icc2/write_data/ioshire_wrapper.pt.v.gz",
    "IOSHIRE_WRAPPER"   : "/project/jenkins/gate_netlists/ioshire/A0/ioshire_wrapper/2020_03_30/2020_12_20_15:10:20_opportunistic_Dec20/pnr/outputs_icc2/write_data/ioshire_wrapper.pt.v.gz",
    "SOCTOP"     : "/project/jenkins/gate_netlists/soc_top/soc_top.mapped.v"
}

# Pointers to netlists to be used by atpg
atpg_db = {
    "spio"              : "/project/pd/release/A0/spio/2020_03_18/2020_04_07_00:16_syn_release/syn/results/spio.mapped.v"
}
