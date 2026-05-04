//`ifdef EVL_COVERAGE_ON

`ifndef __NEIGH_CVG
`define __NEIGH_CVG

`define walking_one_6  \
   wildcard bins bit_0_as_0 = {6'b?????0};\
   wildcard bins bit_0_as_1 = {6'b?????1};\
   wildcard bins bit_1_as_0 = {6'b????0?};\
   wildcard bins bit_1_as_1 = {6'b????1?};\
   wildcard bins bit_2_as_0 = {6'b???0??};\
   wildcard bins bit_2_as_1 = {6'b???1??};\
   wildcard bins bit_3_as_0 = {6'b??0???};\
   wildcard bins bit_3_as_1 = {6'b??1???};\
   wildcard bins bit_4_as_0 = {6'b?0????};\
   wildcard bins bit_4_as_1 = {6'b?1????};\
   wildcard bins bit_5_as_0 = {6'b0?????};\
   wildcard bins bit_5_as_1 = {6'b1?????};

`define walking_one_8  \
   wildcard bins bit_0_as_0 = {8'b???????0};\
   wildcard bins bit_0_as_1 = {8'b???????1};\
   wildcard bins bit_1_as_0 = {8'b??????0?};\
   wildcard bins bit_1_as_1 = {8'b??????1?};\
   wildcard bins bit_2_as_0 = {8'b?????0??};\
   wildcard bins bit_2_as_1 = {8'b?????1??};\
   wildcard bins bit_3_as_0 = {8'b????0???};\
   wildcard bins bit_3_as_1 = {8'b????1???};\
   wildcard bins bit_4_as_0 = {8'b???0????};\
   wildcard bins bit_4_as_1 = {8'b???1????};\
   wildcard bins bit_5_as_0 = {8'b??0?????};\
   wildcard bins bit_5_as_1 = {8'b??1?????};\
   wildcard bins bit_6_as_0 = {8'b?0??????};\
   wildcard bins bit_6_as_1 = {8'b?1??????};\
   wildcard bins bit_7_as_0 = {8'b0???????};\
   wildcard bins bit_7_as_1 = {8'b1???????};

`define walking_one_9  \
   wildcard bins bit_0_as_0 = {9'b????????0};\
   wildcard bins bit_0_as_1 = {9'b????????1};\
   wildcard bins bit_1_as_0 = {9'b???????0?};\
   wildcard bins bit_1_as_1 = {9'b???????1?};\
   wildcard bins bit_2_as_0 = {9'b??????0??};\
   wildcard bins bit_2_as_1 = {9'b??????1??};\
   wildcard bins bit_3_as_0 = {9'b?????0???};\
   wildcard bins bit_3_as_1 = {9'b?????1???};\
   wildcard bins bit_4_as_0 = {9'b????0????};\
   wildcard bins bit_4_as_1 = {9'b????1????};\
   wildcard bins bit_5_as_0 = {9'b???0?????};\
   wildcard bins bit_5_as_1 = {9'b???1?????};\
   wildcard bins bit_6_as_0 = {9'b??0??????};\
   wildcard bins bit_6_as_1 = {9'b??1??????};\
   wildcard bins bit_7_as_0 = {9'b?0???????};\
   wildcard bins bit_7_as_1 = {9'b?1???????};\
   wildcard bins bit_8_as_0 = {9'b0????????};\
   wildcard bins bit_8_as_1 = {9'b1????????};

`define walking_one_16  \
   wildcard bins bit_0_as_0 = {16'b???????????????0};\
   wildcard bins bit_0_as_1 = {16'b???????????????1};\
   wildcard bins bit_1_as_0 = {16'b??????????????0?};\
   wildcard bins bit_1_as_1 = {16'b??????????????1?};\
   wildcard bins bit_2_as_0 = {16'b?????????????0??};\
   wildcard bins bit_2_as_1 = {16'b?????????????1??};\
   wildcard bins bit_3_as_0 = {16'b????????????0???};\
   wildcard bins bit_3_as_1 = {16'b????????????1???};\
   wildcard bins bit_4_as_0 = {16'b???????????0????};\
   wildcard bins bit_4_as_1 = {16'b???????????1????};\
   wildcard bins bit_5_as_0 = {16'b??????????0?????};\
   wildcard bins bit_5_as_1 = {16'b??????????1?????};\
   wildcard bins bit_6_as_0 = {16'b?????????0??????};\
   wildcard bins bit_6_as_1 = {16'b?????????1??????};\
   wildcard bins bit_7_as_0 = {16'b????????0???????};\
   wildcard bins bit_7_as_1 = {16'b????????1???????};\
   wildcard bins bit_8_as_0 = {16'b???????0????????};\
   wildcard bins bit_8_as_1 = {16'b???????1????????};\
   wildcard bins bit_9_as_0 = {16'b??????0?????????};\
   wildcard bins bit_9_as_1 = {16'b??????1?????????};\
   wildcard bins bit_10_as_0 = {16'b?????0??????????};\
   wildcard bins bit_10_as_1 = {16'b?????1??????????};\
   wildcard bins bit_11_as_0 = {16'b????0???????????};\
   wildcard bins bit_11_as_1 = {16'b????1???????????};\
   wildcard bins bit_12_as_0 = {16'b???0????????????};\
   wildcard bins bit_12_as_1 = {16'b???1????????????};\
   wildcard bins bit_13_as_0 = {16'b??0?????????????};\
   wildcard bins bit_13_as_1 = {16'b??1?????????????};\
   wildcard bins bit_14_as_0 = {16'b?0??????????????};\
   wildcard bins bit_14_as_1 = {16'b?1??????????????};\
   wildcard bins bit_15_as_0 = {16'b0???????????????};\
   wildcard bins bit_15_as_1 = {16'b1???????????????};

typedef struct packed {
   logic err_rsp_enable_field;
   logic [2:0] err_interrupt_enable_field;
} esr_icache_err_log_ctl_struct;

// Icache prefetch service configuration
typedef struct packed
{
   logic [`CSR_PRV_SZ-1:0] prv_field;
   logic [`VA_RANGE_CA]    start_addr_field;
   logic [5:0]             num_lines_field;
} icache_prefetch_conf_struct;

typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_CORE_MISS_ID_SIZE, 1, `SHIRE_COOP_DATA_SIZE   ) neigh_et_link_minion_miss_req_info_t;
typedef `ET_LINK_REQ_INFO_TYPE_DEF( `ET_CORE_MISS_ID_SIZE, 1, `CORE_L2_BLOCK_EXT_SIZE ) neigh_et_link_minion_evict_req_info_t;
typedef `ET_LINK_RSP_INFO_TYPE_DEF( `ET_LINK_ID_SIZE,      1, `CORE_L2_BLOCK_EXT_SIZE ) neigh_et_link_minion_rsp_info_t;

class neigh_cvg;
// Shire_id
   covergroup shire_id_cg with function sample (logic [7:0] neigh_shire_id);

      option.name = "shire_id_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_shire_id: coverpoint neigh_shire_id {
         //bins shire_id[] = {[0:34]};
         bins shire_id_0 = {0};
         bins shire_id_1 = {1};
         bins shire_id_2 = {2};
         bins shire_id_3 = {3};
         bins shire_id_4 = {4};
         bins shire_id_5 = {5};
         bins shire_id_6 = {6};
         bins shire_id_7 = {7};
         bins shire_id_8 = {8};
         bins shire_id_9 = {9};
         bins shire_id_10 = {10};
         bins shire_id_11 = {11};
         bins shire_id_12 = {12};
         bins shire_id_13 = {13};
         bins shire_id_14 = {14};
         bins shire_id_15 = {15};
         bins shire_id_16 = {16};
         bins shire_id_17 = {17};
         bins shire_id_18 = {18};
         bins shire_id_19 = {19};
         bins shire_id_20 = {20};
         bins shire_id_21 = {21};
         bins shire_id_22 = {22};
         bins shire_id_23 = {23};
         bins shire_id_24 = {24};
         bins shire_id_25 = {25};
         bins shire_id_26 = {26};
         bins shire_id_27 = {27};
         bins shire_id_28 = {28};
         bins shire_id_29 = {29};
         bins shire_id_30 = {30};
         bins shire_id_31 = {31};
         bins shire_id_32 = {32};
         bins shire_id_33 = {33};
         bins shire_id_34 = {34};
      }

   endgroup


// Neigh_id
   covergroup neigh_id_cg with function sample (logic [1:0] neigh_neigh_id);

      option.name = "neigh_id_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_neigh_id: coverpoint neigh_neigh_id {
         bins neigh_id[] = {[0:3]};
      }
   endgroup


// ESR threads enable
   covergroup neigh_threads_enable_cg with function sample (logic [7:0] neigh_neigh_thread0_enable, logic[7:0] neigh_neigh_thread1_enable);

      option.name = "neigh_threads_enable_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_thread0_enable : coverpoint neigh_neigh_thread0_enable {`walking_one_8}
      cp_neigh_thread1_enable : coverpoint neigh_neigh_thread1_enable {`walking_one_8}

   endgroup


// ESR Minion features
   covergroup neigh_minion_features_cg with function sample (logic [5:0] neigh_neigh_minion_features);

      option.name = "neigh_minion_features_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      //cp_neigh_minion_features : coverpoint neigh_neigh_minion_features {`walking_one_6}
      cp_neigh_minion_features : coverpoint neigh_neigh_minion_features {
         wildcard bins disable_lock_unlock_as_0 = {6'b?????0}; //enable_lock_unlock
         wildcard bins disable_lock_unlock_as_1 = {6'b?????1}; //disable_lock_unlock
         wildcard bins disable_multithreading_as_0 = {6'b????0?}; //enable_multithreading
         wildcard bins disable_multithreading_as_1 = {6'b????1?}; //disable_multithreading
         wildcard bins trap_on_u_scp_as_0 = {6'b???0??}; //not_trap_on_u_scp
         wildcard bins trap_on_u_scp_as_1 = {6'b???1??}; //trap_on_u_scp
         wildcard bins trap_on_u_cacheops_as_0 = {6'b??0???}; //not_trap_on_u_cacheops
         wildcard bins trap_on_u_cacheops_as_1 = {6'b??1???}; //trap_on_u_cacheops
         wildcard bins trap_on_ml_as_0 = {6'b?0????}; //not_trap_on_ml
         wildcard bins trap_on_ml_as_1 = {6'b?1????}; //trap_on_ml
         wildcard bins trap_on_gfx_as_0 = {6'b0?????}; //not_trap_on_gfx
         wildcard bins trap_on_gfx_as_1 = {6'b1?????}; //trap_on_gfx
      }

   endgroup


// ESR Shire coop mode
   covergroup neigh_shire_coop_mode_cg with function sample (logic neigh_neigh_shire_coop_mode);

      option.name = "neigh_shire_coop_mode_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_shire_coop_mode: coverpoint neigh_neigh_shire_coop_mode {
         bins shire_coop_mode_0 = {0};
         bins shire_coop_mode_1 = {1};
      }

   endgroup


// Machine timer interrupt
   covergroup neigh_int_mtip_cg with function sample (logic [7:0] neigh_neigh_int_mtip);

      option.name = "neigh_int_mtip_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_int_mtip : coverpoint neigh_neigh_int_mtip {`walking_one_8}

   endgroup


// Machine external interrupt
   covergroup neigh_int_meip_cg with function sample (logic neigh_neigh_int_meip);

      option.name = "neigh_int_meip_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_int_meip: coverpoint neigh_neigh_int_meip {
         bins int_meip_0 = {0};
         bins int_meip_1 = {1};
      }

   endgroup


// Supervisor external interrupt
   covergroup neigh_int_seip_cg with function sample (logic neigh_neigh_int_seip);

      option.name = "neigh_int_seip_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_int_seip: coverpoint neigh_neigh_int_seip {
         bins int_seip_0 = {0};
         bins int_seip_1 = {1};
      }

   endgroup


// Machine software interrupt
   covergroup neigh_ipi_msip_cg with function sample (logic [15:0] neigh_neigh_ipi_msip);

      option.name = "neigh_ipi_msip_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_ipi_msip : coverpoint neigh_neigh_ipi_msip {`walking_one_16}

   endgroup


// Ipi redirect trigger
   covergroup neigh_ipi_redirect_trigger_cg with function sample (logic [15:0] neigh_neigh_ipi_redirect_trigger);

      option.name = "neigh_ipi_redirect_trigger_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_ipi_redirect_trigger : coverpoint neigh_neigh_ipi_redirect_trigger {`walking_one_16}

   endgroup


// Uc to neigh fcc
   covergroup neigh_uc_to_neigh_fcc_cg with function sample (logic [7:0] neigh_neigh_uc_to_neigh_fcc);

      option.name = "neigh_uc_to_neigh_fcc_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_uc_to_neigh_fcc : coverpoint neigh_neigh_uc_to_neigh_fcc {`walking_one_8}

   endgroup


// Uc to neigh fcc target
   covergroup neigh_uc_to_neigh_fcc_target_cg with function sample (logic [1:0] neigh_neigh_uc_to_neigh_fcc_target);

      option.name = "neigh_uc_to_neigh_fcc_target_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_uc_to_neigh_fcc_target: coverpoint neigh_neigh_uc_to_neigh_fcc_target {
         bins neigh_uc_to_neigh_fcc_target[] = {[0:3]};
      }

   endgroup


// Icache f2 sram req
   covergroup neigh_icache_sram_req_cg with function sample (logic neigh_icache_f2_sram_req_valid, logic neigh_icache_f2_sram_req_ready, logic neigh_icache_f2_sram_req_write, logic [8:0] neigh_icache_f2_sram_req_addr);

      option.name = "neigh_icache_sram_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_icache_sram_req_valid_ready: coverpoint neigh_icache_f2_sram_req_valid iff (neigh_icache_f2_sram_req_ready) {
         bins valid_1 = {1};
      }

      cp_neigh_icache_sram_req_write: coverpoint neigh_icache_f2_sram_req_write iff (neigh_icache_f2_sram_req_valid && neigh_icache_f2_sram_req_ready){
         bins write_0 = {0};
         bins write_1 = {1};
      }

        //address range
      cp_neigh_icache_sram_req_addr: coverpoint neigh_icache_f2_sram_req_addr iff (neigh_icache_f2_sram_req_valid && neigh_icache_f2_sram_req_ready){
         wildcard bins bits_0_and_1_as_0 = {9'b???????00};
         wildcard bins bits_0_and_1_as_1 = {9'b???????01};
         wildcard bins bits_0_and_1_as_2 = {9'b???????10};
         wildcard bins bits_0_and_1_as_3 = {9'b???????11};
         `walking_one_9
      }

   endgroup


// Min etl miss req
   covergroup neigh_min_etl_miss_req_cg with function sample (logic [7:0][2:0] neigh_l2_dcache_miss_req_valid, logic [7:0][2:0] neigh_l2_dcache_miss_req_ready);

      option.name = "neigh_min_etl_miss_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_etl_miss_req_0: coverpoint neigh_l2_dcache_miss_req_valid[0][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[0][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[0][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[0][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[0][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[0][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[0][2]);
      }

      cp_neigh_min_etl_miss_req_1: coverpoint neigh_l2_dcache_miss_req_valid[1][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[1][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[1][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[1][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[1][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[1][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[1][2]);
      }

      cp_neigh_min_etl_miss_req_2: coverpoint neigh_l2_dcache_miss_req_valid[2][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[2][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[2][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[2][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[2][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[0][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[2][2]);
      }

      cp_neigh_min_etl_miss_req_3: coverpoint neigh_l2_dcache_miss_req_valid[3][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[3][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[3][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[3][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[3][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[3][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[3][2]);
      }

      cp_neigh_min_etl_miss_req_4: coverpoint neigh_l2_dcache_miss_req_valid[4][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[4][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[4][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[4][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[4][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[4][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[4][2]);
      }

      cp_neigh_min_etl_miss_req_5: coverpoint neigh_l2_dcache_miss_req_valid[5][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[5][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[5][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[5][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[5][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[5][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[5][2]);
      }

      cp_neigh_min_etl_miss_req_6: coverpoint neigh_l2_dcache_miss_req_valid[6][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[6][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[6][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[6][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[6][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[6][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[6][2]);
      }

      cp_neigh_min_etl_miss_req_7: coverpoint neigh_l2_dcache_miss_req_valid[7][2:0] {
         //wildcard bins valid_0_as_0 = {3'b??0} iff (neigh_l2_dcache_miss_req_ready[7][0]);
         wildcard bins valid_0_as_1 = {3'b??1} iff (neigh_l2_dcache_miss_req_ready[7][0]);
         //wildcard bins valid_1_as_0 = {3'b?0?} iff (neigh_l2_dcache_miss_req_ready[7][1]);
         wildcard bins valid_1_as_1 = {3'b?1?} iff (neigh_l2_dcache_miss_req_ready[7][1]);
         //wildcard bins valid_2_as_0 = {3'b0??} iff (neigh_l2_dcache_miss_req_ready[7][2]);
         wildcard bins valid_2_as_1 = {3'b1??} iff (neigh_l2_dcache_miss_req_ready[7][2]);
      }

   endgroup


// Neigh min etl evict req
   covergroup neigh_min_etl_evict_req_cg with function sample (logic [7:0][1:0] neigh_l2_dcache_evict_req_valid, logic [7:0][1:0] neigh_l2_dcache_evict_req_ready);

      option.name = "neigh_min_etl_evict_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_etl_evict_req_0: coverpoint neigh_l2_dcache_evict_req_valid[0][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[0][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[0][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[0][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[0][1]);
      }

      cp_neigh_min_etl_evict_req_1: coverpoint neigh_l2_dcache_evict_req_valid[1][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[1][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[1][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[1][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[1][1]);
      }

      cp_neigh_min_etl_evict_req_2: coverpoint neigh_l2_dcache_evict_req_valid[2][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[2][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[2][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[2][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[2][1]);
      }

      cp_neigh_min_etl_evict_req_3: coverpoint neigh_l2_dcache_evict_req_valid[3][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[3][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[3][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[3][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[3][1]);
      }

      cp_neigh_min_etl_evict_req_4: coverpoint neigh_l2_dcache_evict_req_valid[4][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[4][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[4][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[4][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[4][1]);
      }

      cp_neigh_min_etl_evict_req_5: coverpoint neigh_l2_dcache_evict_req_valid[5][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[5][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[5][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[5][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[5][1]);
      }

      cp_neigh_min_etl_evict_req_6: coverpoint neigh_l2_dcache_evict_req_valid[6][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[6][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[6][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[6][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[6][1]);
      }

      cp_neigh_min_etl_evict_req_7: coverpoint neigh_l2_dcache_evict_req_valid[7][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_l2_dcache_evict_req_ready[7][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_l2_dcache_evict_req_ready[7][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_l2_dcache_evict_req_ready[7][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_l2_dcache_evict_req_ready[7][1]);
      }

   endgroup


// Icache etl req
   covergroup neigh_icache_etl_req_cg with function sample (logic neigh_icache_etl_req_valid, logic neigh_icache_etl_req_ready);

      option.name = "neigh_icache_etl_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_icache_etl_req: coverpoint neigh_icache_etl_req_valid iff (neigh_icache_etl_req_ready) {
         bins valid_and_ready = {1};
      }

   endgroup


// Ptw etl req
   covergroup neigh_ptw_etl_req_cg with function sample (logic [1:0] neigh_ptw_l2_miss_req_valid, logic [1:0] neigh_ptw_l2_miss_req_ready);

      option.name = "neigh_ptw_etl_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_ptw_etl_req: coverpoint neigh_ptw_l2_miss_req_valid {
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_ptw_l2_miss_req_ready[0]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_ptw_l2_miss_req_ready[1]);
      }

   endgroup


// TBOX etl req
   covergroup neigh_tbox_etl_req_cg with function sample (logic neigh_tbox_etl_req_valid, logic neigh_tbox_etl_req_ready);

      option.name = "neigh_tbox_etl_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_tbox_etl_req: coverpoint neigh_tbox_etl_req_valid iff (neigh_tbox_etl_req_ready) {
         bins valid_0 = {0};
         bins valid_1 = {1};
      }

   endgroup


// Coop tload etl req
   covergroup neigh_coop_tload_etl_req_cg with function sample (logic neigh_coop_tload_miss_req_valid, logic neigh_coop_tload_miss_req_ready);

      option.name = "neigh_coop_tload_etl_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_etl_req: coverpoint neigh_coop_tload_miss_req_valid iff (neigh_coop_tload_miss_req_ready) {
         bins valid_and_ready = {1};
      }

   endgroup


// Coop tstore etl req
   covergroup neigh_coop_tstore_etl_req_cg with function sample (logic neigh_coop_tstore_etl_req_valid, logic neigh_coop_tstore_etl_req_ready);

      option.name = "neigh_coop_tstore_etl_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tstore_etl_req: coverpoint neigh_coop_tstore_etl_req_valid iff (neigh_coop_tstore_etl_req_ready) {
         bins valid_and_ready = {1};
      }

   endgroup


// Etl req goes to int fifo
   covergroup neigh_etl_req_goes_to_int_fifo_cg with function sample (logic neigh_miss_req_goes_to_int_fifo, logic neigh_miss_req_valid_ff, logic neigh_miss_req_ready, logic neigh_miss_req_goes_to_tbox);

      option.name = "neigh_etl_req_goes_to_int_fifo_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_goes_to_int_fifo: coverpoint neigh_miss_req_goes_to_int_fifo iff (neigh_miss_req_valid_ff && neigh_miss_req_ready && !neigh_miss_req_goes_to_tbox) {
         bins req_0 = {0};
         bins req_1 = {1};
      }

   endgroup


// Etl req goes to TBOX
   covergroup neigh_etl_req_goes_to_tbox_fifo_cg with function sample (logic neigh_min_tbox_req_valid, logic neigh_min_tbox_req_ready);

      option.name = "neigh_etl_req_goes_to_int_fifo_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_goes_to_tbox_fifo: coverpoint neigh_min_tbox_req_valid iff (neigh_min_tbox_req_valid) {
         bins valid_0 = {0};
         bins valid_1 = {1};
      }

   endgroup


// Etl req goes to bank fifo
   covergroup neigh_etl_req_goes_to_bank_fifo_cg with function sample (logic [3:0] neigh_bank_req_info_push);

      option.name = "neigh_etl_req_goes_to_bank_fifo_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_goes_to_bank_fifo: coverpoint neigh_bank_req_info_push {
         wildcard bins bank_fifo_0 = {4'b???1};
         wildcard bins bank_fifo_1 = {4'b??1?};
         wildcard bins bank_fifo_2 = {4'b?1??};
         wildcard bins bank_fifo_3 = {4'b1???};
      }

   endgroup


// Etl req goes to uc fifo
   covergroup neigh_etl_req_goes_to_uc_fifo_cg with function sample (logic neigh_uc_req_info_push);

      option.name = "neigh_etl_req_goes_to_uc_fifo_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_goes_to_uc_fifo: coverpoint neigh_uc_req_info_push {
         bins req_0 = {0};
         bins req_1 = {1};
      }

   endgroup


// Etl req goes to sc
   covergroup neigh_etl_req_to_sc_cg with function sample (logic [4:0] neigh_neigh_sc_req_valid,logic [4:0] neigh_neigh_sc_req_ready);

      option.name = "neigh_etl_req_to_sc_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_to_sc: coverpoint neigh_neigh_sc_req_valid {
         wildcard bins valid_0_as_1 = {5'b????1} iff (neigh_neigh_sc_req_ready[0]);
         wildcard bins valid_1_as_1 = {5'b???1?} iff (neigh_neigh_sc_req_ready[1]);
         wildcard bins valid_2_as_1 = {5'b??1??} iff (neigh_neigh_sc_req_ready[2]);
         wildcard bins valid_3_as_1 = {5'b?1???} iff (neigh_neigh_sc_req_ready[3]);
         wildcard bins valid_4_as_1 = {5'b1????} iff (neigh_neigh_sc_req_ready[4]);
      }

   endgroup


// Min etl rsp
   covergroup neigh_min_etl_rsp_cg with function sample (logic neigh_fill_fifo_min_valid, logic neigh_fill_fifo_min_ready);

      option.name = "neigh_min_etl_rsp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_etl_rsp: coverpoint neigh_fill_fifo_min_valid iff (neigh_fill_fifo_min_ready) {
         bins valid_0 = {0};
         bins valid_1 = {1};
      }

   endgroup


// Icache etl rsp
   covergroup neigh_icache_etl_rsp_cg with function sample (logic neigh_icache_l2_miss_resp_valid, logic neigh_icache_l2_miss_resp_ready);

      option.name = "neigh_icache_etl_rsp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_icache_etl_rsp: coverpoint neigh_icache_l2_miss_resp_valid iff (neigh_icache_l2_miss_resp_ready) {
         bins valid_0 = {0};
         bins valid_1 = {1};
      }

   endgroup


// PTW etl rsp
   covergroup neigh_ptw_etl_rsp_cg with function sample (logic [1:0] neigh_ptw_l2_miss_resp_valid, logic [1:0] neigh_ptw_l2_miss_resp_ready);

      option.name = "neigh_ptw_etl_rsp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_ptw_0_etl_rsp: coverpoint neigh_ptw_l2_miss_resp_valid[0] iff (neigh_ptw_l2_miss_resp_ready[0]) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

      cp_neigh_ptw_1_etl_rsp: coverpoint neigh_ptw_l2_miss_resp_valid[1] iff (neigh_ptw_l2_miss_resp_ready[1]) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// TBOX etl rsp
   covergroup neigh_tbox_etl_rsp_cg with function sample (logic neigh_sc_tbox_rsp_valid, logic neigh_sc_tbox_rsp_ready);

      option.name = "neigh_tbox_etl_rsp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_tbox_etl_rsp: coverpoint neigh_sc_tbox_rsp_valid iff (neigh_sc_tbox_rsp_ready) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// Coop tstore etl rsp
   covergroup neigh_coop_tstore_etl_rsp_cg with function sample (logic [7:0] neigh_tstore_min_resp_valid, logic [7:0] neigh_tstore_min_resp_ready);

      option.name = "neigh_coop_tstore_etl_rsp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tstore_etl_rsp: coverpoint neigh_tstore_min_resp_valid {
//       wildcard bins valid_0_as_1 = {8'b???????1} iff (neigh_tstore_min_resp_ready[0]);
         wildcard bins valid_1_as_1 = {8'b??????1?} iff (neigh_tstore_min_resp_ready[1]);
         wildcard bins valid_2_as_1 = {8'b?????1??} iff (neigh_tstore_min_resp_ready[2]);
         wildcard bins valid_3_as_1 = {8'b????1???} iff (neigh_tstore_min_resp_ready[3]);
//       wildcard bins valid_4_as_1 = {8'b???1????} iff (neigh_tstore_min_resp_ready[4]);
         wildcard bins valid_5_as_1 = {8'b??1?????} iff (neigh_tstore_min_resp_ready[5]);
         wildcard bins valid_6_as_1 = {8'b?1??????} iff (neigh_tstore_min_resp_ready[6]);
         wildcard bins valid_7_as_1 = {8'b1???????} iff (neigh_tstore_min_resp_ready[7]);
      }

   endgroup


// Etl rsp from sc
   covergroup neigh_etl_rsp_from_sc_cg with function sample (logic neigh_neigh_sc_rsp_valid, logic neigh_neigh_sc_rsp_ready);

      option.name = "neigh_etl_rsp_from_sc_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_rsp_from_sc: coverpoint neigh_neigh_sc_rsp_valid iff (neigh_neigh_sc_rsp_ready) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// Etl msg req to tbox
   covergroup neigh_etl_msg_req_to_tbox_cg with function sample (logic neigh_min_tbox_rsp_valid, logic neigh_min_tbox_rsp_ready);

      option.name = "neigh_etl_msg_req_to_tbox_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_msg_req_to_tbox: coverpoint neigh_min_tbox_rsp_valid iff (neigh_min_tbox_rsp_ready) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// Etl msg rsp from tbox
   covergroup neigh_etl_msg_rsp_from_tbox_cg with function sample (logic [`MIN_PER_N-1:0] neigh_tbox_min_rsp_valid_in, logic [`MIN_PER_N-1:0] neigh_tbox_min_rsp_ready_in);

      option.name = "neigh_etl_msg_rsp_from_tbox_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_msg_rsp_from_tbox: coverpoint neigh_tbox_min_rsp_valid_in {
         wildcard bins valid_0_as_1 = {8'b???????1} iff (neigh_tbox_min_rsp_ready_in[0]);
         wildcard bins valid_1_as_1 = {8'b??????1?} iff (neigh_tbox_min_rsp_ready_in[1]);
         wildcard bins valid_2_as_1 = {8'b?????1??} iff (neigh_tbox_min_rsp_ready_in[2]);
         wildcard bins valid_3_as_1 = {8'b????1???} iff (neigh_tbox_min_rsp_ready_in[3]);
         wildcard bins valid_4_as_1 = {8'b???1????} iff (neigh_tbox_min_rsp_ready_in[4]);
         wildcard bins valid_5_as_1 = {8'b??1?????} iff (neigh_tbox_min_rsp_ready_in[5]);
         wildcard bins valid_6_as_1 = {8'b?1??????} iff (neigh_tbox_min_rsp_ready_in[6]);
         wildcard bins valid_7_as_1 = {8'b1???????} iff (neigh_tbox_min_rsp_ready_in[7]);
      }

   endgroup


// Etl req from tbox
   covergroup neigh_etl_req_from_tbox_cg with function sample (logic neigh_tbox_neigh_req_valid, logic neigh_tbox_neigh_req_ready);

      option.name = "neigh_etl_req_from_tbox_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_req_from_tbox: coverpoint neigh_tbox_neigh_req_valid iff (neigh_tbox_neigh_req_ready) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// Etl rsp from tbox
   covergroup neigh_etl_rsp_to_tbox_cg with function sample (logic neigh_neigh_tbox_rsp_valid, logic neigh_neigh_tbox_rsp_ready);

      option.name = "neigh_etl_rsp_to_tbox_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_etl_rsp_to_tbox: coverpoint neigh_neigh_tbox_rsp_valid iff (neigh_neigh_tbox_rsp_ready) {
         bins rsp_0 = {0};
         bins rsp_1 = {1};
      }

   endgroup


// Min flb req
   covergroup neigh_min_flb_req_cg with function sample (logic [7:0] neigh_flb_core_neigh_req_valid);

      option.name = "neigh_min_flb_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_flb_req : coverpoint neigh_flb_core_neigh_req_valid {
         wildcard bins valid_0 = {8'b???????1};
         wildcard bins valid_1 = {8'b??????1?};
         wildcard bins valid_2 = {8'b?????1??};
         wildcard bins valid_3 = {8'b????1???};
         wildcard bins valid_4 = {8'b???1????};
         wildcard bins valid_5 = {8'b??1?????};
         wildcard bins valid_6 = {8'b?1??????};
         wildcard bins valid_7 = {8'b1???????};
      }

   endgroup


// Flb req
   covergroup neigh_flb_req_cg with function sample (logic neigh_flb_neigh_l2_req_valid);

      option.name = "neigh_flb_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_flb_req: coverpoint neigh_flb_neigh_l2_req_valid {
         bins flb_neigh_l2_req_valid = {1};
      }

   endgroup


// Min flb resp
   covergroup neigh_min_flb_resp_cg with function sample (logic [7:0] neigh_flb_neigh_core_resp_valid);

      option.name = "neigh_min_flb_resp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_flb_req : coverpoint neigh_flb_neigh_core_resp_valid {
         wildcard bins valid_0 = {8'b???????1};
         wildcard bins valid_1 = {8'b??????1?};
         wildcard bins valid_2 = {8'b?????1??};
         wildcard bins valid_3 = {8'b????1???};
         wildcard bins valid_4 = {8'b???1????};
         wildcard bins valid_5 = {8'b??1?????};
         wildcard bins valid_6 = {8'b?1??????};
         wildcard bins valid_7 = {8'b1???????};
      }

   endgroup


// Flb resp
   covergroup neigh_flb_resp_cg with function sample (logic neigh_flb_l2_neigh_resp_valid);

      option.name = "neigh_flb_resp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_flb_resp: coverpoint neigh_flb_l2_neigh_resp_valid {
         bins flb_l2_neigh_resp_valid = {1};
      }

   endgroup


// ET Link cooperative read requests going to coop_tload module
   covergroup neigh_min_coop_miss_req_cg with function sample (logic [7:0][1:0] neigh_min_coop_miss_req_valid, logic [7:0][1:0] neigh_min_coop_miss_req_ready);

      option.name = "neigh_min_coop_miss_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_coop_miss_req_0: coverpoint neigh_min_coop_miss_req_valid[0][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[0][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[0][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[0][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[0][1]);
      }

      cp_neigh_min_coop_miss_req_1: coverpoint neigh_min_coop_miss_req_valid[1][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[1][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[1][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[1][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[1][1]);
      }

      cp_neigh_min_coop_miss_req_2: coverpoint neigh_min_coop_miss_req_valid[2][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[2][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[2][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[2][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[2][1]);
      }

      cp_neigh_min_coop_miss_req_3: coverpoint neigh_min_coop_miss_req_valid[3][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[3][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[3][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[3][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[3][1]);
      }

      cp_neigh_min_coop_miss_req_4: coverpoint neigh_min_coop_miss_req_valid[4][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[4][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[4][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[4][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[4][1]);
      }

      cp_neigh_min_coop_miss_req_5: coverpoint neigh_min_coop_miss_req_valid[5][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[5][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[5][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[5][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[5][1]);
      }

      cp_neigh_min_coop_miss_req_6: coverpoint neigh_min_coop_miss_req_valid[6][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[6][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[6][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[6][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[6][1]);
      }

      cp_neigh_min_coop_miss_req_7: coverpoint neigh_min_coop_miss_req_valid[7][1:0] {
         //wildcard bins valid_0_as_0 = {2'b?0} iff (neigh_min_coop_miss_req_ready[7][0]);
         wildcard bins valid_0_as_1 = {2'b?1} iff (neigh_min_coop_miss_req_ready[7][0]);
         //wildcard bins valid_1_as_0 = {2'b0?} iff (neigh_min_coop_miss_req_ready[7][1]);
         wildcard bins valid_1_as_1 = {2'b1?} iff (neigh_min_coop_miss_req_ready[7][1]);
      }

   endgroup


// Cooperative TLoad slave ready output bus
   covergroup neigh_coop_tload_slv_rdy_out_cg with function sample (logic neigh_coop_tload_slv_rdy_out_valid);

      option.name = "neigh_coop_tload_slv_rdy_out_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_slv_rdy_out: coverpoint neigh_coop_tload_slv_rdy_out_valid {
         bins coop_tload_slv_rdy_out_valid_0 = {0};
         bins coop_tload_slv_rdy_out_valid_1 = {1};
      }

   endgroup


// Cooperative TLoad slave ready input buses
   covergroup neigh_coop_tload_slv_rdy_in_cg with function sample (logic [2:0] neigh_coop_tload_slv_rdy_in_valid);

      option.name = "neigh_coop_tload_slv_rdy_in_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_slv_rdy_in: coverpoint neigh_coop_tload_slv_rdy_in_valid {
         wildcard bins coop_tload_slv_rdy_in_valid_1 = {3'b??1};
         wildcard bins coop_tload_slv_rdy_in_valid_2 = {3'b?1?};
         wildcard bins coop_tload_slv_rdy_in_valid_3 = {3'b1??};
      }

   endgroup


// Cooperative TLoad master done output bus
   covergroup neigh_coop_tload_mst_done_out_cg with function sample (logic [2:0] neigh_coop_tload_mst_done_out_valid);

      option.name = "neigh_coop_tload_mst_done_out_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_mst_done_out: coverpoint neigh_coop_tload_mst_done_out_valid {
         wildcard bins coop_tload_mst_done_out_valid_1 = {3'b??1};
         wildcard bins coop_tload_mst_done_out_valid_2 = {3'b?1?};
         wildcard bins coop_tload_mst_done_out_valid_3 = {3'b1??};
      }

   endgroup


// Cooperative TLoad master done input buses
   covergroup neigh_coop_tload_mst_done_in_cg with function sample (logic [2:0] neigh_coop_tload_mst_done_in_valid);

      option.name = "neigh_coop_tload_mst_done_in_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_mst_done_in: coverpoint neigh_coop_tload_mst_done_in_valid {
         wildcard bins coop_tload_mst_done_in_valid_1 = {3'b??1};
         wildcard bins coop_tload_mst_done_in_valid_2 = {3'b?1?};
         wildcard bins coop_tload_mst_done_in_valid_3 = {3'b1??};
      }

   endgroup


// Cooperative TLoad responses
   covergroup neigh_coop_tload_etl_resp_cg with function sample (logic neigh_coop_tload_tag_table_cam_pop);

      option.name = "neigh_coop_tload_etl_resp_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_tload_etl_resp: coverpoint neigh_coop_tload_tag_table_cam_pop {
         bins coop_tload_tag_table_cam_pop_0 = {0};
         bins coop_tload_tag_table_cam_pop_1 = {1};
      }

   endgroup


// ET Link cooperative store requests going to tensor_store_buffer module
   covergroup neigh_min_coop_evict_req_cg with function sample (logic [`MIN_PER_N-1:0] neigh_coop_evict_valid, logic [`MIN_PER_N-1:0] neigh_coop_evict_ready);

      option.name = "neigh_min_coop_evict_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_min_coop_evict_req: coverpoint neigh_coop_evict_valid {
         wildcard bins valid_0_as_1 = {8'b???????1} iff (neigh_coop_evict_ready[0]);
         wildcard bins valid_1_as_1 = {8'b??????1?} iff (neigh_coop_evict_ready[1]);
         wildcard bins valid_2_as_1 = {8'b?????1??} iff (neigh_coop_evict_ready[2]);
         wildcard bins valid_3_as_1 = {8'b????1???} iff (neigh_coop_evict_ready[3]);
         wildcard bins valid_4_as_1 = {8'b???1????} iff (neigh_coop_evict_ready[4]);
         wildcard bins valid_5_as_1 = {8'b??1?????} iff (neigh_coop_evict_ready[5]);
         wildcard bins valid_6_as_1 = {8'b?1??????} iff (neigh_coop_evict_ready[6]);
         wildcard bins valid_7_as_1 = {8'b1???????} iff (neigh_coop_evict_ready[7]);
      }

   endgroup


// Cooperative store response ACK
   covergroup neigh_coop_store_resp_ack_cg with function sample (logic [`MIN_PER_N-1:0] neigh_l2_dcache_resp_tstore_ack);

      option.name = "neigh_coop_store_resp_ack_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_neigh_coop_store_resp_ack: coverpoint neigh_l2_dcache_resp_tstore_ack {
         wildcard bins resp_ack_0 = {8'b???????1};
         wildcard bins resp_ack_1 = {8'b??????1?};
         wildcard bins resp_ack_2 = {8'b?????1??};
         wildcard bins resp_ack_3 = {8'b????1???};
         wildcard bins resp_ack_4 = {8'b???1????};
         wildcard bins resp_ack_5 = {8'b??1?????};
         wildcard bins resp_ack_6 = {8'b?1??????};
         wildcard bins resp_ack_7 = {8'b1???????};
      }

   endgroup


//Sizes of ET Link evict requests generated by minions
// covergroup neigh_min_evict_req_size_cg with function sample (`toVectorExt(et_link_minion_evict_req_info_t,[`MIN_PER_N-1:0]) neigh_l2_dcache_evict_req, logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] neigh_l2_dcache_evict_req_valid, logic [`MIN_PER_N-1:0][`DCACHE_L2_EVICT_REQ_PORTS-1:0] neigh_l2_dcache_evict_req_ready);

//    option.name = "neigh_min_evict_req_size_cg";
//    option.per_instance         = 0;
//    type_option.merge_instances = 1;

//    cp_neigh_min_evict_req_size_0: coverpoint neigh_l2_dcache_evict_req[0].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[0][0] && neigh_l2_dcache_evict_req_ready[0][0]) || (neigh_l2_dcache_evict_req_valid[0][1] && neigh_l2_dcache_evict_req_ready[0][1])) {
//       bins et_link_Byte_min_0            = {3'b000}; // 8b
//       bins et_link_HWord_min_0           = {3'b001}; // 16b
//       bins et_link_Word_min_0            = {3'b010}; // 32b
//       bins et_link_DWord_min_0           = {3'b011}; // 64b
//       bins et_link_QWord_min_0           = {3'b100}; // 128b
//       bins et_link_HLine_min_0           = {3'b101}; // 256b
//       bins et_link_Line_min_0            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_0     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_1: coverpoint neigh_l2_dcache_evict_req[1].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[1][0] && neigh_l2_dcache_evict_req_ready[1][0]) || (neigh_l2_dcache_evict_req_valid[1][1] && neigh_l2_dcache_evict_req_ready[1][1])) {
//       bins et_link_Byte_min_1            = {3'b000}; // 8b
//       bins et_link_HWord_min_1           = {3'b001}; // 16b
//       bins et_link_Word_min_1            = {3'b010}; // 32b
//       bins et_link_DWord_min_1           = {3'b011}; // 64b
//       bins et_link_QWord_min_1           = {3'b100}; // 128b
//       bins et_link_HLine_min_1           = {3'b101}; // 256b
//       bins et_link_Line_min_1            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_1     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_2: coverpoint neigh_l2_dcache_evict_req[2].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[2][0] && neigh_l2_dcache_evict_req_ready[2][0]) || (neigh_l2_dcache_evict_req_valid[2][1] && neigh_l2_dcache_evict_req_ready[2][1])) {
//       bins et_link_Byte_min_2            = {3'b000}; // 8b
//       bins et_link_HWord_min_2           = {3'b001}; // 16b
//       bins et_link_Word_min_2            = {3'b010}; // 32b
//       bins et_link_DWord_min_2           = {3'b011}; // 64b
//       bins et_link_QWord_min_2           = {3'b100}; // 128b
//       bins et_link_HLine_min_2           = {3'b101}; // 256b
//       bins et_link_Line_min_2            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_2     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_3: coverpoint neigh_l2_dcache_evict_req[3].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[3][0] && neigh_l2_dcache_evict_req_ready[3][0]) || (neigh_l2_dcache_evict_req_valid[3][1] && neigh_l2_dcache_evict_req_ready[3][1])) {
//       bins et_link_Byte_min_3            = {3'b000}; // 8b
//       bins et_link_HWord_min_3           = {3'b001}; // 16b
//       bins et_link_Word_min_3            = {3'b010}; // 32b
//       bins et_link_DWord_min_3           = {3'b011}; // 64b
//       bins et_link_QWord_min_3           = {3'b100}; // 128b
//       bins et_link_HLine_min_3           = {3'b101}; // 256b
//       bins et_link_Line_min_3            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_3     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_4: coverpoint neigh_l2_dcache_evict_req[4].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[4][0] && neigh_l2_dcache_evict_req_ready[4][0]) || (neigh_l2_dcache_evict_req_valid[4][1] && neigh_l2_dcache_evict_req_ready[4][1])) {
//       bins et_link_Byte_min_4            = {3'b000}; // 8b
//       bins et_link_HWord_min_4           = {3'b001}; // 16b
//       bins et_link_Word_min_4            = {3'b010}; // 32b
//       bins et_link_DWord_min_4           = {3'b011}; // 64b
//       bins et_link_QWord_min_4           = {3'b100}; // 128b
//       bins et_link_HLine_min_4           = {3'b101}; // 256b
//       bins et_link_Line_min_4            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_4     = {3'b111}; // Unused
//    }
//    cp_neigh_min_evict_req_size_5: coverpoint neigh_l2_dcache_evict_req[5].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[5][0] && neigh_l2_dcache_evict_req_ready[5][0]) || (neigh_l2_dcache_evict_req_valid[5][1] && neigh_l2_dcache_evict_req_ready[5][1])) {
//       bins et_link_Byte_min_5            = {3'b000}; // 8b
//       bins et_link_HWord_min_5           = {3'b001}; // 16b
//       bins et_link_Word_min_5            = {3'b010}; // 32b
//       bins et_link_DWord_min_5           = {3'b011}; // 64b
//       bins et_link_QWord_min_5           = {3'b100}; // 128b
//       bins et_link_HLine_min_5           = {3'b101}; // 256b
//       bins et_link_Line_min_5            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_5     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_6: coverpoint neigh_l2_dcache_evict_req[6].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[6][0] && neigh_l2_dcache_evict_req_ready[6][0]) || (neigh_l2_dcache_evict_req_valid[6][1] && neigh_l2_dcache_evict_req_ready[6][1])) {
//       bins et_link_Byte_min_6            = {3'b000}; // 8b
//       bins et_link_HWord_min_6           = {3'b001}; // 16b
//       bins et_link_Word_min_6            = {3'b010}; // 32b
//       bins et_link_DWord_min_6           = {3'b011}; // 64b
//       bins et_link_QWord_min_6           = {3'b100}; // 128b
//       bins et_link_HLine_min_6           = {3'b101}; // 256b
//       bins et_link_Line_min_6            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_6     = {3'b111}; // Unused
//    }

//    cp_neigh_min_evict_req_size_7: coverpoint neigh_l2_dcache_evict_req[7].size[2:0] iff ((neigh_l2_dcache_evict_req_valid[7][0] && neigh_l2_dcache_evict_req_ready[7][0]) || (neigh_l2_dcache_evict_req_valid[7][1] && neigh_l2_dcache_evict_req_ready[7][1])) {
//       bins et_link_Byte_min_7            = {3'b000}; // 8b
//       bins et_link_HWord_min_7           = {3'b001}; // 16b
//       bins et_link_Word_min_7            = {3'b010}; // 32b
//       bins et_link_DWord_min_7           = {3'b011}; // 64b
//       bins et_link_QWord_min_7           = {3'b100}; // 128b
//       bins et_link_HLine_min_7           = {3'b101}; // 256b
//       bins et_link_Line_min_7            = {3'b110}; // 512b
//       ignore_bins et_link_IllegalSize_min_7     = {3'b111}; // Unused
//    }

// endgroup


// After a valid response that notified a miss, a fill_done pulse is always sent
   covergroup icache_resp_fill_done_cg with function sample (int fill_done);

      option.name = "icache_resp_fill_done_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

//    cp_icache_resp_fill_done: coverpoint icache_f4_resp_valid {
//       wildcard bins valid_0_as_1 = {2'b?1} iff (icache_f4_resp_miss[0]);
//       wildcard bins valid_1_as_1 = {2'b1?} iff (icache_f4_resp_miss[1]);
//    }

      cp_icache_resp_fill_done: coverpoint fill_done {
         bins latency_less_than_10 = {[0:10]};
         bins latency_less_than_100 = {[11:100]};
         bins latency_greater_than_100 = {[101:$]};

      }

   endgroup


// Icache flush
   covergroup icache_flush_cg with function sample (logic icache_f0_flush_data);

      option.name = "icache_flush_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_flush: coverpoint icache_f0_flush_data {
         bins flush_0 = {1'b0};
         bins flush_1 = {1'b1};
      }

   endgroup


// Icache l1 sram req
   covergroup icache_l1_sram_req_cg with function sample (logic icache_f2_sram_req_valid, logic icache_f2_sram_req_ready, logic [8:0] icache_f2_sram_req_addr);

      option.name = "icache_l1_sram_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      //address range
      cp_icache_l1_sram_req_consecutive_set: coverpoint icache_f2_sram_req_addr[`ICACHE_WAY_ADDR_WIDTH] iff (icache_f2_sram_req_valid && icache_f2_sram_req_ready){
         bins low = {0};
         bins high = {1};
      }

      cp_icache_l1_sram_req_set_diff_row: coverpoint icache_f2_sram_req_addr[`ICACHE_SRAM_ADD_WIDTH-1] iff (icache_f2_sram_req_valid && icache_f2_sram_req_ready){
         bins low = {0};
         bins high = {1};
      }

      cp_icache_l1_sram_req_way: coverpoint icache_f2_sram_req_addr[`ICACHE_WAY_RANGE] iff (icache_f2_sram_req_valid && icache_f2_sram_req_ready){
         bins way_0 = {0};
         bins way_1 = {1};
         bins way_2 = {2};
         bins way_3 = {3};
      }

      cp_icache_l1_sram_req: coverpoint icache_f2_sram_req_addr iff (icache_f2_sram_req_valid && icache_f2_sram_req_ready){
         bins addr_way_0 = {9'b101010100};
         bins addr_way_1 = {9'b101010101};
         bins addr_way_2 = {9'b101010110};
         bins addr_way_3 = {9'b101010111};
      }

   endgroup


// Icache disable l2 miss req
   covergroup icache_disable_l2_miss_req_cg with function sample (logic icache_f0_l2_miss_req_valid, logic icache_signal_f0_l2_miss_req_disable_next);

      option.name = "icache_disable_l2_miss_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_disable_l2_miss_req: coverpoint icache_f0_l2_miss_req_valid iff (icache_signal_f0_l2_miss_req_disable_next) {
         bins valid_0 = {1'b0};
      }

   endgroup


// Icache l1 sram resp latency
   covergroup icache_l1_sram_resp_latency_cg with function sample (int l1_sram_latency);

      option.name = "icache_l1_sram_resp_latency_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_l1_sram_resp_latency: coverpoint l1_sram_latency {
         bins latency_short = {[0:10]};
         bins latency_long = {[11:20]};
      }

   endgroup


// Icache l2 miss resp latency
   covergroup icache_l2_miss_resp_latency_cg with function sample (int l2_miss_latency);

      option.name = "icache_l2_miss_resp_latency_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_l2_miss_resp_latency: coverpoint l2_miss_latency {
         bins latency_less_than_30 = {[0:10]};
         bins latency_less_than_100 = {[11:100]};
         bins latency_greater_than_100 = {[101:$]};
      }

   endgroup


// ESR error control fields
   covergroup icache_err_log_ctl_cg with function sample (esr_icache_err_log_ctl_struct icache_esr_err_log_ctl);

      option.name = "icache_err_log_ctl_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;


      cp_icache_err_log_ctl_interrupt: coverpoint icache_esr_err_log_ctl.err_interrupt_enable_field {
         wildcard bins err_interrupt_enable_bit0_as_0 = {3'b??0};
         wildcard bins err_interrupt_enable_bit0_as_1 = {3'b??1};
         wildcard bins err_interrupt_enable_bit1_as_0 = {3'b?0?};
         wildcard bins err_interrupt_enable_bit1_as_1 = {3'b?1?};
      }

      cp_icache_err_log_ctl_rsp: coverpoint icache_esr_err_log_ctl.err_rsp_enable_field {
         bins err_rsp_enable_0 = {1'b0};
         bins err_rsp_enable_1 = {1'b1};
      }

   endgroup


// Prefetch request
   covergroup icache_prefetch_req_cg with function sample (icache_prefetch_conf_struct icache_esr_prefetch_conf, logic icache_esr_prefetch_start, logic icache_esr_prefetch_done);

      option.name = "icache_prefetch_req_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_prefetch_req_prv: coverpoint icache_esr_prefetch_conf.prv_field iff (icache_esr_prefetch_start) {
         bins CSR_PRV_U = {2'b00};
         bins CSR_PRV_S = {2'b01};
//       bins invalid_CSR_PRV_H = {2'b10};
         bins CSR_PRV_M = {2'b11};
      }

      cp_icache_prefetch_req_num_lines: coverpoint icache_esr_prefetch_conf.num_lines_field iff (icache_esr_prefetch_start) {
         bins all_ones = {6'b111111};
//       bins pw_zero = {6'b000001};
//       bins pw_one = {6'b000010};
//       bins pw_two = {6'b000100};
//       bins pw_three = {6'b001000};
//       bins pw_four = {6'b010000};
//       bins pw_five = {6'b100000};
         wildcard bins bit_one = {6'b?????1};
         wildcard bins bit_two = {6'b????1?};
         wildcard bins bit_three = {6'b???1??};
         wildcard bins bit_four = {6'b??1???};
         wildcard bins bit_five = {6'b?1????};
         wildcard bins bit_six = {6'b1?????};
      }

      cp_icache_prefetch_req_done: coverpoint icache_esr_prefetch_done {
         bins done_0 = {1'b0};
         bins done_1 = {1'b1};
      }

   endgroup


// Icache bypass
   covergroup icache_bypass_cg with function sample (logic icache_esr_bypass_icache, logic [1:0] icache_f2_hit, logic [1:0] icache_bypass_en);

      option.name = "icache_bypass_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_icache_bypass: coverpoint icache_esr_bypass_icache {
         bins bypass_enabled = {1'b1};
      }

      cp_icache_bypass_hit: coverpoint icache_f2_hit iff (icache_esr_bypass_icache) {
         wildcard bins f2_hit_ucache_0 = {2'b?1};
         wildcard bins f2_hit_ucache_1 = {2'b1?};
      }

      cp_icache_bypass_en: coverpoint icache_bypass_en {
         wildcard bins bypass_en_ucache_0 = {2'b?1};
         wildcard bins bypass_en_ucache_1 = {2'b1?};
      }

   endgroup


// fifo_gen RTLMIN-5554
   covergroup int_req_info_fifo_cg with function sample (logic fifo_gen_full, 
                                                         logic fifo_gen_valid, 
                                                         logic fifo_gen_push,
                                                         logic fifo_gen_pop);

      option.name = "int_req_info_fifo_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_int_req_info_fifo : coverpoint {fifo_gen_full, fifo_gen_valid, fifo_gen_push, fifo_gen_pop} {
         bins valid_pop           = {4'b0101};
         bins valid_push          = {4'b0110};
         bins valid_push_pop      = {4'b0111};
         bins empty_push          = {4'b0010};
         bins full_valid_pop      = {4'b1101};
         bins full_valid_push_pop = {4'b1111}; 
      }
   endgroup   


   function new ();
      shire_id_cg = new();
      neigh_id_cg = new();
      neigh_threads_enable_cg = new();
      neigh_minion_features_cg = new();
      neigh_shire_coop_mode_cg = new();
      neigh_int_mtip_cg = new();
      neigh_int_meip_cg = new();
      neigh_int_seip_cg = new();
      neigh_ipi_msip_cg = new();
      neigh_ipi_redirect_trigger_cg = new();
      neigh_uc_to_neigh_fcc_cg = new();
      neigh_uc_to_neigh_fcc_target_cg = new();
      neigh_icache_sram_req_cg = new();
      neigh_min_etl_miss_req_cg = new();
      neigh_min_etl_evict_req_cg = new();
      neigh_icache_etl_req_cg = new();
      neigh_ptw_etl_req_cg = new();
      neigh_tbox_etl_req_cg = new();
      neigh_coop_tload_etl_req_cg = new();
      neigh_coop_tstore_etl_req_cg = new();
      neigh_etl_req_goes_to_int_fifo_cg = new();
      neigh_etl_req_goes_to_tbox_fifo_cg = new();
      neigh_etl_req_goes_to_bank_fifo_cg = new();
      neigh_etl_req_goes_to_uc_fifo_cg = new();
      neigh_etl_req_to_sc_cg = new();
      neigh_min_etl_rsp_cg = new();
      neigh_icache_etl_rsp_cg = new();
      neigh_ptw_etl_rsp_cg = new();
      neigh_tbox_etl_rsp_cg = new();
      neigh_coop_tstore_etl_rsp_cg = new();
      neigh_etl_rsp_from_sc_cg = new();
      neigh_etl_msg_req_to_tbox_cg = new();
      neigh_etl_msg_rsp_from_tbox_cg = new();
      neigh_etl_req_from_tbox_cg = new();
      neigh_etl_rsp_to_tbox_cg = new();
      neigh_min_flb_req_cg = new();
      neigh_flb_req_cg = new();
      neigh_min_flb_resp_cg = new();
      neigh_flb_resp_cg = new();
      neigh_min_coop_miss_req_cg = new();
      neigh_coop_tload_slv_rdy_out_cg = new();
      neigh_coop_tload_slv_rdy_in_cg = new();
      neigh_coop_tload_mst_done_out_cg = new();
      neigh_coop_tload_mst_done_in_cg = new();
      neigh_coop_tload_etl_resp_cg = new();
      neigh_min_coop_evict_req_cg = new();
      neigh_coop_store_resp_ack_cg = new();
      icache_resp_fill_done_cg = new();
      icache_flush_cg = new();
      icache_l1_sram_req_cg = new();
      icache_disable_l2_miss_req_cg = new();
      icache_l1_sram_resp_latency_cg = new();
      icache_l2_miss_resp_latency_cg = new();
      icache_err_log_ctl_cg = new();
      icache_prefetch_req_cg = new();
      icache_bypass_cg = new();
      int_req_info_fifo_cg = new();
   endfunction

endclass

`endif
//`endif
