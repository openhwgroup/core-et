class evl_axi_cov #(parameter axi_cov_type_t AXI_COV_TYPE = EVL_COV_AXI_NONE, parameter string AXI_COV_NAME = "None") extends evl_axi_cov_base;

   localparam int DATA_WIDTH         = (AXI_COV_TYPE == EVL_COV_AXI_MEM_SBM_8)  ? 8  :
                                       (AXI_COV_TYPE == EVL_COV_AXI_SYS_ESR_32) ? 32 :
                                       (AXI_COV_TYPE == EVL_COV_AXI_UC_32)      ? 32 : 64;
   localparam bit USE_AW_USER_ET     = (AXI_COV_TYPE == EVL_COV_AXI_SC_64)      ? 1'b1 : 1'b0;
   localparam bit USE_AW_USER_MEM    = (AXI_COV_TYPE == EVL_COV_AXI_SYS_MEM_64) ? 1'b1 : 1'b0;
   localparam bit USE_AW_USER_ATOMIC = (AXI_COV_TYPE == EVL_COV_AXI_SYS_ESR_32) ? 1'b1 : 1'b0;
   localparam bit USE_QOS_0_1        = (AXI_COV_TYPE == EVL_COV_AXI_SYS_MEM_64) ? 1'b1 : 1'b0;
   localparam bit USE_QOS_2_3        = (AXI_COV_TYPE == EVL_COV_AXI_SYS_ESR_32) ? 1'b1 : 1'b0;

   bit                m_allow_req;
   bit                m_request_master;
   bit                m_response_master;
   bit                m_cover_data;
   bit                m_use_qos_0_1;
   bit                m_use_qos_2_3;

   int                ar_idle_count;
   int                ar_busy_count;
   int                ar_req_delay;
   bit [15:0]         ar_active_count;

   bit [2:0]          ar_size;
   evl_bus_req_id_t   ar_id;
   bit [3:0]          ar_qos;
   bit [3:0]          ar_addr_6;
   bit [3:0]          ar_addr_5;
   bit [3:0]          ar_addr_4;
   bit [3:0]          ar_addr_3;
   bit [3:0]          ar_addr_2;
   bit [3:0]          ar_addr_1;
   bit [1:0]          ar_addr_0;
   bit [1:0]          ar_addr_edge_6;
   bit [1:0]          ar_addr_edge_5;
   bit [1:0]          ar_addr_edge_4;
   bit [1:0]          ar_addr_edge_3;
   bit [1:0]          ar_addr_edge_2;
   bit [1:0]          ar_addr_edge_1;

   int                aw_idle_count;
   int                aw_busy_count;
   int                aw_req_delay;
   bit [15:0]         aw_active_count;

   bit [2:0]          aw_size;
   evl_bus_req_id_t   aw_id;
   evl_bus_req_user_t aw_user;
   bit [3:0]          aw_qos;
   bit [3:0]          aw_addr_6;
   bit [3:0]          aw_addr_5;
   bit [3:0]          aw_addr_4;
   bit [3:0]          aw_addr_3;
   bit [3:0]          aw_addr_2;
   bit [3:0]          aw_addr_1;
   bit [1:0]          aw_addr_0;
   bit [1:0]          aw_addr_edge_6;
   bit [1:0]          aw_addr_edge_5;
   bit [1:0]          aw_addr_edge_4;
   bit [1:0]          aw_addr_edge_3;
   bit [1:0]          aw_addr_edge_2;
   bit [1:0]          aw_addr_edge_1;

   int                w_idle_count;
   int                w_busy_count;
   int                w_req_delay;

   bit                w_strb_none;
   bit [4:0]          w_strb_byte;
   bit [3:0]          w_strb_hword;
   bit [3:0]          w_strb_word;
   bit [3:0]          w_strb_dword;
   bit [2:0]          w_strb_qword;
   bit [1:0]          w_strb_oword;
   bit [1:0]          w_strb_full;

   bit [3:0]          w_data_000;
   bit [3:0]          w_data_001;
   bit [3:0]          w_data_002;
   bit [3:0]          w_data_003;
   bit [3:0]          w_data_004;
   bit [3:0]          w_data_005;
   bit [3:0]          w_data_006;
   bit [3:0]          w_data_007;
   bit [3:0]          w_data_008;
   bit [3:0]          w_data_009;
   bit [3:0]          w_data_010;
   bit [3:0]          w_data_011;
   bit [3:0]          w_data_012;
   bit [3:0]          w_data_013;
   bit [3:0]          w_data_014;
   bit [3:0]          w_data_015;
   bit [3:0]          w_data_016;
   bit [3:0]          w_data_017;
   bit [3:0]          w_data_018;
   bit [3:0]          w_data_019;
   bit [3:0]          w_data_020;
   bit [3:0]          w_data_021;
   bit [3:0]          w_data_022;
   bit [3:0]          w_data_023;
   bit [3:0]          w_data_024;
   bit [3:0]          w_data_025;
   bit [3:0]          w_data_026;
   bit [3:0]          w_data_027;
   bit [3:0]          w_data_028;
   bit [3:0]          w_data_029;
   bit [3:0]          w_data_030;
   bit [3:0]          w_data_031;
   bit [3:0]          w_data_032;
   bit [3:0]          w_data_033;
   bit [3:0]          w_data_034;
   bit [3:0]          w_data_035;
   bit [3:0]          w_data_036;
   bit [3:0]          w_data_037;
   bit [3:0]          w_data_038;
   bit [3:0]          w_data_039;
   bit [3:0]          w_data_040;
   bit [3:0]          w_data_041;
   bit [3:0]          w_data_042;
   bit [3:0]          w_data_043;
   bit [3:0]          w_data_044;
   bit [3:0]          w_data_045;
   bit [3:0]          w_data_046;
   bit [3:0]          w_data_047;
   bit [3:0]          w_data_048;
   bit [3:0]          w_data_049;
   bit [3:0]          w_data_050;
   bit [3:0]          w_data_051;
   bit [3:0]          w_data_052;
   bit [3:0]          w_data_053;
   bit [3:0]          w_data_054;
   bit [3:0]          w_data_055;
   bit [3:0]          w_data_056;
   bit [3:0]          w_data_057;
   bit [3:0]          w_data_058;
   bit [3:0]          w_data_059;
   bit [3:0]          w_data_060;
   bit [3:0]          w_data_061;
   bit [3:0]          w_data_062;
   bit [3:0]          w_data_063;
   bit [3:0]          w_data_064;
   bit [3:0]          w_data_065;
   bit [3:0]          w_data_066;
   bit [3:0]          w_data_067;
   bit [3:0]          w_data_068;
   bit [3:0]          w_data_069;
   bit [3:0]          w_data_070;
   bit [3:0]          w_data_071;
   bit [3:0]          w_data_072;
   bit [3:0]          w_data_073;
   bit [3:0]          w_data_074;
   bit [3:0]          w_data_075;
   bit [3:0]          w_data_076;
   bit [3:0]          w_data_077;
   bit [3:0]          w_data_078;
   bit [3:0]          w_data_079;
   bit [3:0]          w_data_080;
   bit [3:0]          w_data_081;
   bit [3:0]          w_data_082;
   bit [3:0]          w_data_083;
   bit [3:0]          w_data_084;
   bit [3:0]          w_data_085;
   bit [3:0]          w_data_086;
   bit [3:0]          w_data_087;
   bit [3:0]          w_data_088;
   bit [3:0]          w_data_089;
   bit [3:0]          w_data_090;
   bit [3:0]          w_data_091;
   bit [3:0]          w_data_092;
   bit [3:0]          w_data_093;
   bit [3:0]          w_data_094;
   bit [3:0]          w_data_095;
   bit [3:0]          w_data_096;
   bit [3:0]          w_data_097;
   bit [3:0]          w_data_098;
   bit [3:0]          w_data_099;
   bit [3:0]          w_data_100;
   bit [3:0]          w_data_101;
   bit [3:0]          w_data_102;
   bit [3:0]          w_data_103;
   bit [3:0]          w_data_104;
   bit [3:0]          w_data_105;
   bit [3:0]          w_data_106;
   bit [3:0]          w_data_107;
   bit [3:0]          w_data_108;
   bit [3:0]          w_data_109;
   bit [3:0]          w_data_110;
   bit [3:0]          w_data_111;
   bit [3:0]          w_data_112;
   bit [3:0]          w_data_113;
   bit [3:0]          w_data_114;
   bit [3:0]          w_data_115;
   bit [3:0]          w_data_116;
   bit [3:0]          w_data_117;
   bit [3:0]          w_data_118;
   bit [3:0]          w_data_119;
   bit [3:0]          w_data_120;
   bit [3:0]          w_data_121;
   bit [3:0]          w_data_122;
   bit [3:0]          w_data_123;
   bit [3:0]          w_data_124;
   bit [3:0]          w_data_125;
   bit [3:0]          w_data_126;
   bit [3:0]          w_data_127;
   bit [1:0]          w_data_edge_001;
   bit [1:0]          w_data_edge_002;
   bit [1:0]          w_data_edge_003;
   bit [1:0]          w_data_edge_004;
   bit [1:0]          w_data_edge_005;
   bit [1:0]          w_data_edge_006;
   bit [1:0]          w_data_edge_007;
   bit [1:0]          w_data_edge_008;
   bit [1:0]          w_data_edge_009;
   bit [1:0]          w_data_edge_010;
   bit [1:0]          w_data_edge_011;
   bit [1:0]          w_data_edge_012;
   bit [1:0]          w_data_edge_013;
   bit [1:0]          w_data_edge_014;
   bit [1:0]          w_data_edge_015;
   bit [1:0]          w_data_edge_016;
   bit [1:0]          w_data_edge_017;
   bit [1:0]          w_data_edge_018;
   bit [1:0]          w_data_edge_019;
   bit [1:0]          w_data_edge_020;
   bit [1:0]          w_data_edge_021;
   bit [1:0]          w_data_edge_022;
   bit [1:0]          w_data_edge_023;
   bit [1:0]          w_data_edge_024;
   bit [1:0]          w_data_edge_025;
   bit [1:0]          w_data_edge_026;
   bit [1:0]          w_data_edge_027;
   bit [1:0]          w_data_edge_028;
   bit [1:0]          w_data_edge_029;
   bit [1:0]          w_data_edge_030;
   bit [1:0]          w_data_edge_031;
   bit [1:0]          w_data_edge_032;
   bit [1:0]          w_data_edge_033;
   bit [1:0]          w_data_edge_034;
   bit [1:0]          w_data_edge_035;
   bit [1:0]          w_data_edge_036;
   bit [1:0]          w_data_edge_037;
   bit [1:0]          w_data_edge_038;
   bit [1:0]          w_data_edge_039;
   bit [1:0]          w_data_edge_040;
   bit [1:0]          w_data_edge_041;
   bit [1:0]          w_data_edge_042;
   bit [1:0]          w_data_edge_043;
   bit [1:0]          w_data_edge_044;
   bit [1:0]          w_data_edge_045;
   bit [1:0]          w_data_edge_046;
   bit [1:0]          w_data_edge_047;
   bit [1:0]          w_data_edge_048;
   bit [1:0]          w_data_edge_049;
   bit [1:0]          w_data_edge_050;
   bit [1:0]          w_data_edge_051;
   bit [1:0]          w_data_edge_052;
   bit [1:0]          w_data_edge_053;
   bit [1:0]          w_data_edge_054;
   bit [1:0]          w_data_edge_055;
   bit [1:0]          w_data_edge_056;
   bit [1:0]          w_data_edge_057;
   bit [1:0]          w_data_edge_058;
   bit [1:0]          w_data_edge_059;
   bit [1:0]          w_data_edge_060;
   bit [1:0]          w_data_edge_061;
   bit [1:0]          w_data_edge_062;
   bit [1:0]          w_data_edge_063;
   bit [1:0]          w_data_edge_064;
   bit [1:0]          w_data_edge_065;
   bit [1:0]          w_data_edge_066;
   bit [1:0]          w_data_edge_067;
   bit [1:0]          w_data_edge_068;
   bit [1:0]          w_data_edge_069;
   bit [1:0]          w_data_edge_070;
   bit [1:0]          w_data_edge_071;
   bit [1:0]          w_data_edge_072;
   bit [1:0]          w_data_edge_073;
   bit [1:0]          w_data_edge_074;
   bit [1:0]          w_data_edge_075;
   bit [1:0]          w_data_edge_076;
   bit [1:0]          w_data_edge_077;
   bit [1:0]          w_data_edge_078;
   bit [1:0]          w_data_edge_079;
   bit [1:0]          w_data_edge_080;
   bit [1:0]          w_data_edge_081;
   bit [1:0]          w_data_edge_082;
   bit [1:0]          w_data_edge_083;
   bit [1:0]          w_data_edge_084;
   bit [1:0]          w_data_edge_085;
   bit [1:0]          w_data_edge_086;
   bit [1:0]          w_data_edge_087;
   bit [1:0]          w_data_edge_088;
   bit [1:0]          w_data_edge_089;
   bit [1:0]          w_data_edge_090;
   bit [1:0]          w_data_edge_091;
   bit [1:0]          w_data_edge_092;
   bit [1:0]          w_data_edge_093;
   bit [1:0]          w_data_edge_094;
   bit [1:0]          w_data_edge_095;
   bit [1:0]          w_data_edge_096;
   bit [1:0]          w_data_edge_097;
   bit [1:0]          w_data_edge_098;
   bit [1:0]          w_data_edge_099;
   bit [1:0]          w_data_edge_100;
   bit [1:0]          w_data_edge_101;
   bit [1:0]          w_data_edge_102;
   bit [1:0]          w_data_edge_103;
   bit [1:0]          w_data_edge_104;
   bit [1:0]          w_data_edge_105;
   bit [1:0]          w_data_edge_106;
   bit [1:0]          w_data_edge_107;
   bit [1:0]          w_data_edge_108;
   bit [1:0]          w_data_edge_109;
   bit [1:0]          w_data_edge_110;
   bit [1:0]          w_data_edge_111;
   bit [1:0]          w_data_edge_112;
   bit [1:0]          w_data_edge_113;
   bit [1:0]          w_data_edge_114;
   bit [1:0]          w_data_edge_115;
   bit [1:0]          w_data_edge_116;
   bit [1:0]          w_data_edge_117;
   bit [1:0]          w_data_edge_118;
   bit [1:0]          w_data_edge_119;
   bit [1:0]          w_data_edge_120;
   bit [1:0]          w_data_edge_121;
   bit [1:0]          w_data_edge_122;
   bit [1:0]          w_data_edge_123;
   bit [1:0]          w_data_edge_124;
   bit [1:0]          w_data_edge_125;
   bit [1:0]          w_data_edge_126;
   bit [1:0]          w_data_edge_127;

   int                r_idle_count;
   int                r_busy_count;
   int                r_rsp_delay;
   bit                r_rsp_ooo;

   bit [1:0]          r_resp;
   bit [3:0]          r_data_000;
   bit [3:0]          r_data_001;
   bit [3:0]          r_data_002;
   bit [3:0]          r_data_003;
   bit [3:0]          r_data_004;
   bit [3:0]          r_data_005;
   bit [3:0]          r_data_006;
   bit [3:0]          r_data_007;
   bit [3:0]          r_data_008;
   bit [3:0]          r_data_009;
   bit [3:0]          r_data_010;
   bit [3:0]          r_data_011;
   bit [3:0]          r_data_012;
   bit [3:0]          r_data_013;
   bit [3:0]          r_data_014;
   bit [3:0]          r_data_015;
   bit [3:0]          r_data_016;
   bit [3:0]          r_data_017;
   bit [3:0]          r_data_018;
   bit [3:0]          r_data_019;
   bit [3:0]          r_data_020;
   bit [3:0]          r_data_021;
   bit [3:0]          r_data_022;
   bit [3:0]          r_data_023;
   bit [3:0]          r_data_024;
   bit [3:0]          r_data_025;
   bit [3:0]          r_data_026;
   bit [3:0]          r_data_027;
   bit [3:0]          r_data_028;
   bit [3:0]          r_data_029;
   bit [3:0]          r_data_030;
   bit [3:0]          r_data_031;
   bit [3:0]          r_data_032;
   bit [3:0]          r_data_033;
   bit [3:0]          r_data_034;
   bit [3:0]          r_data_035;
   bit [3:0]          r_data_036;
   bit [3:0]          r_data_037;
   bit [3:0]          r_data_038;
   bit [3:0]          r_data_039;
   bit [3:0]          r_data_040;
   bit [3:0]          r_data_041;
   bit [3:0]          r_data_042;
   bit [3:0]          r_data_043;
   bit [3:0]          r_data_044;
   bit [3:0]          r_data_045;
   bit [3:0]          r_data_046;
   bit [3:0]          r_data_047;
   bit [3:0]          r_data_048;
   bit [3:0]          r_data_049;
   bit [3:0]          r_data_050;
   bit [3:0]          r_data_051;
   bit [3:0]          r_data_052;
   bit [3:0]          r_data_053;
   bit [3:0]          r_data_054;
   bit [3:0]          r_data_055;
   bit [3:0]          r_data_056;
   bit [3:0]          r_data_057;
   bit [3:0]          r_data_058;
   bit [3:0]          r_data_059;
   bit [3:0]          r_data_060;
   bit [3:0]          r_data_061;
   bit [3:0]          r_data_062;
   bit [3:0]          r_data_063;
   bit [3:0]          r_data_064;
   bit [3:0]          r_data_065;
   bit [3:0]          r_data_066;
   bit [3:0]          r_data_067;
   bit [3:0]          r_data_068;
   bit [3:0]          r_data_069;
   bit [3:0]          r_data_070;
   bit [3:0]          r_data_071;
   bit [3:0]          r_data_072;
   bit [3:0]          r_data_073;
   bit [3:0]          r_data_074;
   bit [3:0]          r_data_075;
   bit [3:0]          r_data_076;
   bit [3:0]          r_data_077;
   bit [3:0]          r_data_078;
   bit [3:0]          r_data_079;
   bit [3:0]          r_data_080;
   bit [3:0]          r_data_081;
   bit [3:0]          r_data_082;
   bit [3:0]          r_data_083;
   bit [3:0]          r_data_084;
   bit [3:0]          r_data_085;
   bit [3:0]          r_data_086;
   bit [3:0]          r_data_087;
   bit [3:0]          r_data_088;
   bit [3:0]          r_data_089;
   bit [3:0]          r_data_090;
   bit [3:0]          r_data_091;
   bit [3:0]          r_data_092;
   bit [3:0]          r_data_093;
   bit [3:0]          r_data_094;
   bit [3:0]          r_data_095;
   bit [3:0]          r_data_096;
   bit [3:0]          r_data_097;
   bit [3:0]          r_data_098;
   bit [3:0]          r_data_099;
   bit [3:0]          r_data_100;
   bit [3:0]          r_data_101;
   bit [3:0]          r_data_102;
   bit [3:0]          r_data_103;
   bit [3:0]          r_data_104;
   bit [3:0]          r_data_105;
   bit [3:0]          r_data_106;
   bit [3:0]          r_data_107;
   bit [3:0]          r_data_108;
   bit [3:0]          r_data_109;
   bit [3:0]          r_data_110;
   bit [3:0]          r_data_111;
   bit [3:0]          r_data_112;
   bit [3:0]          r_data_113;
   bit [3:0]          r_data_114;
   bit [3:0]          r_data_115;
   bit [3:0]          r_data_116;
   bit [3:0]          r_data_117;
   bit [3:0]          r_data_118;
   bit [3:0]          r_data_119;
   bit [3:0]          r_data_120;
   bit [3:0]          r_data_121;
   bit [3:0]          r_data_122;
   bit [3:0]          r_data_123;
   bit [3:0]          r_data_124;
   bit [3:0]          r_data_125;
   bit [3:0]          r_data_126;
   bit [3:0]          r_data_127;
   bit [1:0]          r_data_edge_001;
   bit [1:0]          r_data_edge_002;
   bit [1:0]          r_data_edge_003;
   bit [1:0]          r_data_edge_004;
   bit [1:0]          r_data_edge_005;
   bit [1:0]          r_data_edge_006;
   bit [1:0]          r_data_edge_007;
   bit [1:0]          r_data_edge_008;
   bit [1:0]          r_data_edge_009;
   bit [1:0]          r_data_edge_010;
   bit [1:0]          r_data_edge_011;
   bit [1:0]          r_data_edge_012;
   bit [1:0]          r_data_edge_013;
   bit [1:0]          r_data_edge_014;
   bit [1:0]          r_data_edge_015;
   bit [1:0]          r_data_edge_016;
   bit [1:0]          r_data_edge_017;
   bit [1:0]          r_data_edge_018;
   bit [1:0]          r_data_edge_019;
   bit [1:0]          r_data_edge_020;
   bit [1:0]          r_data_edge_021;
   bit [1:0]          r_data_edge_022;
   bit [1:0]          r_data_edge_023;
   bit [1:0]          r_data_edge_024;
   bit [1:0]          r_data_edge_025;
   bit [1:0]          r_data_edge_026;
   bit [1:0]          r_data_edge_027;
   bit [1:0]          r_data_edge_028;
   bit [1:0]          r_data_edge_029;
   bit [1:0]          r_data_edge_030;
   bit [1:0]          r_data_edge_031;
   bit [1:0]          r_data_edge_032;
   bit [1:0]          r_data_edge_033;
   bit [1:0]          r_data_edge_034;
   bit [1:0]          r_data_edge_035;
   bit [1:0]          r_data_edge_036;
   bit [1:0]          r_data_edge_037;
   bit [1:0]          r_data_edge_038;
   bit [1:0]          r_data_edge_039;
   bit [1:0]          r_data_edge_040;
   bit [1:0]          r_data_edge_041;
   bit [1:0]          r_data_edge_042;
   bit [1:0]          r_data_edge_043;
   bit [1:0]          r_data_edge_044;
   bit [1:0]          r_data_edge_045;
   bit [1:0]          r_data_edge_046;
   bit [1:0]          r_data_edge_047;
   bit [1:0]          r_data_edge_048;
   bit [1:0]          r_data_edge_049;
   bit [1:0]          r_data_edge_050;
   bit [1:0]          r_data_edge_051;
   bit [1:0]          r_data_edge_052;
   bit [1:0]          r_data_edge_053;
   bit [1:0]          r_data_edge_054;
   bit [1:0]          r_data_edge_055;
   bit [1:0]          r_data_edge_056;
   bit [1:0]          r_data_edge_057;
   bit [1:0]          r_data_edge_058;
   bit [1:0]          r_data_edge_059;
   bit [1:0]          r_data_edge_060;
   bit [1:0]          r_data_edge_061;
   bit [1:0]          r_data_edge_062;
   bit [1:0]          r_data_edge_063;
   bit [1:0]          r_data_edge_064;
   bit [1:0]          r_data_edge_065;
   bit [1:0]          r_data_edge_066;
   bit [1:0]          r_data_edge_067;
   bit [1:0]          r_data_edge_068;
   bit [1:0]          r_data_edge_069;
   bit [1:0]          r_data_edge_070;
   bit [1:0]          r_data_edge_071;
   bit [1:0]          r_data_edge_072;
   bit [1:0]          r_data_edge_073;
   bit [1:0]          r_data_edge_074;
   bit [1:0]          r_data_edge_075;
   bit [1:0]          r_data_edge_076;
   bit [1:0]          r_data_edge_077;
   bit [1:0]          r_data_edge_078;
   bit [1:0]          r_data_edge_079;
   bit [1:0]          r_data_edge_080;
   bit [1:0]          r_data_edge_081;
   bit [1:0]          r_data_edge_082;
   bit [1:0]          r_data_edge_083;
   bit [1:0]          r_data_edge_084;
   bit [1:0]          r_data_edge_085;
   bit [1:0]          r_data_edge_086;
   bit [1:0]          r_data_edge_087;
   bit [1:0]          r_data_edge_088;
   bit [1:0]          r_data_edge_089;
   bit [1:0]          r_data_edge_090;
   bit [1:0]          r_data_edge_091;
   bit [1:0]          r_data_edge_092;
   bit [1:0]          r_data_edge_093;
   bit [1:0]          r_data_edge_094;
   bit [1:0]          r_data_edge_095;
   bit [1:0]          r_data_edge_096;
   bit [1:0]          r_data_edge_097;
   bit [1:0]          r_data_edge_098;
   bit [1:0]          r_data_edge_099;
   bit [1:0]          r_data_edge_100;
   bit [1:0]          r_data_edge_101;
   bit [1:0]          r_data_edge_102;
   bit [1:0]          r_data_edge_103;
   bit [1:0]          r_data_edge_104;
   bit [1:0]          r_data_edge_105;
   bit [1:0]          r_data_edge_106;
   bit [1:0]          r_data_edge_107;
   bit [1:0]          r_data_edge_108;
   bit [1:0]          r_data_edge_109;
   bit [1:0]          r_data_edge_110;
   bit [1:0]          r_data_edge_111;
   bit [1:0]          r_data_edge_112;
   bit [1:0]          r_data_edge_113;
   bit [1:0]          r_data_edge_114;
   bit [1:0]          r_data_edge_115;
   bit [1:0]          r_data_edge_116;
   bit [1:0]          r_data_edge_117;
   bit [1:0]          r_data_edge_118;
   bit [1:0]          r_data_edge_119;
   bit [1:0]          r_data_edge_120;
   bit [1:0]          r_data_edge_121;
   bit [1:0]          r_data_edge_122;
   bit [1:0]          r_data_edge_123;
   bit [1:0]          r_data_edge_124;
   bit [1:0]          r_data_edge_125;
   bit [1:0]          r_data_edge_126;
   bit [1:0]          r_data_edge_127;

   int                b_idle_count;
   int                b_busy_count;
   int                b_rsp_delay;
   bit                b_rsp_ooo;

   bit [1:0]          b_resp;

   bit [2:0]          concurrent_req;
   bit [1:0]          concurrent_rsp;

   bit                count_aw_w;
   int                num_aw_waiting_for_w;
   int                num_w_waiting_for_aw;

   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AR Requests
   //
   // This cover group is used to provide coverage for requests on the AR channel.  It consists of
   // cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back requests (as
   //    well as some limited number of cycles between requests).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    AXI ready has been deasserted for some number of cycles.
   //
   // size:
   //    There is a cover point for all of the valid AXI request sizes for this interface.
   //
   // addr:
   //    There are cover points for address bits.  Rather than getting coverage for all addresses,
   //    we group address bits and look for all combinations within a group (addr[7:6], addr[11:8],
   //    addr[15:12], addr[19:16], addr[23:20], addr[27:24], addr[31:28] ... addr[39:32] are not
   //    captured).  In addition, the address "edges" between these groups are checked.
   //
   // qos:
   //    There is a cover point for valid AXI QOS for this interface.
   //
   covergroup cg_axi_ar_req;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      ar_valid_to_valid: coverpoint ar_idle_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      ar_not_ready: coverpoint ar_busy_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      ar_size_mem: coverpoint ar_size {
         option.weight      = (DATA_WIDTH == 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 64) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         bins        B16_128b      = { 3'b100 };
         bins        B32_256b      = { 3'b101 };
         bins        B64_512b      = { 3'b110 };
         ignore_bins ignore_values = { 7 };
      }

      ar_size_esr: coverpoint ar_size {
         option.weight      = (DATA_WIDTH == 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 32) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         bins        B16_128b      = { 3'b100 };
         bins        B32_256b      = { 3'b101 };
         ignore_bins ignore_values = { [6:7] };
      }

      ar_size_sbm: coverpoint ar_size {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         ignore_bins ignore_values = { [4:7] };
      }

      ar_addr_6: coverpoint ar_addr_6 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_5: coverpoint ar_addr_5 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_4: coverpoint ar_addr_4 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_3: coverpoint ar_addr_3 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_2: coverpoint ar_addr_2 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_1: coverpoint ar_addr_1 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      ar_addr_0: coverpoint ar_addr_0 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_6: coverpoint ar_addr_edge_6 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_5: coverpoint ar_addr_edge_5 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_4: coverpoint ar_addr_edge_4 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_3: coverpoint ar_addr_edge_3 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_2: coverpoint ar_addr_edge_2 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      ar_addr_edge_1: coverpoint ar_addr_edge_1 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
   endgroup : cg_axi_ar_req


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AR QoS (values of 0 and 1 only)
   //
   // This covergroup is used to capture coverage for high/low priority requests to the memory
   // shire.
   //
   covergroup cg_axi_ar_qos_0_1;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      ar_qos: coverpoint ar_qos {
         bins        values[]      = { 0, 1 };
         ignore_bins ignore_values = { 2, 3 };
      }
   endgroup : cg_axi_ar_qos_0_1


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AR QoS (values of 2 and 3 only)
   //
   // This covergroup is used to capture coverage for message/UC request differentiation for the
   // Minion shire.
   //
   covergroup cg_axi_ar_qos_2_3;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      ar_qos: coverpoint ar_qos {
         bins        values[]      = { 2, 3 };
         ignore_bins ignore_values = { 0, 1 };
      }
   endgroup : cg_axi_ar_qos_2_3


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AR Request Delay
   //
   covergroup cg_axi_ar_req_delay;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      ar_req_delay: coverpoint ar_req_delay {
         bins        values_1_to_4[]       = { [1:4] };
         bins        values_5_to_10        = { [5:10] };
         bins        values_11_to_20       = { [11:20] };
         bins        values_21_to_infinity = { [21:$] };
         ignore_bins ignore_values         = { 0 };
      }
   endgroup : cg_axi_ar_req_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AW Requests
   //
   // This cover group is used to provide coverage for requests on the AW channel.  It consists of
   // cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back requests (as
   //    well as some limited number of cycles between requests).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    AXI ready has been deasserted for some number of cycles.
   //
   // size:
   //    There is a cover point for all of the valid AXI request sizes for this interface.
   //
   // addr:
   //    There are cover points for address bits.  Rather than getting coverage for all addresses,
   //    we group address bits and look for all combinations within a group (addr[7:6], addr[11:8],
   //    addr[15:12], addr[19:16], addr[23:20], addr[27:24], addr[31:28] ... addr[39:32] are not
   //    captured).  In addition, the address "edges" between these groups are checked.
   //
   // qos:
   //    There is a cover point for valid AXI QOS for this interface.
   //
   // user:
   //    There is a cover point for all of the valid AWUSER values for this interface.
   //
   covergroup cg_axi_aw_req;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      aw_valid_to_valid: coverpoint aw_idle_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      aw_not_ready: coverpoint aw_busy_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      aw_size_mem: coverpoint aw_size {
         option.weight      = (DATA_WIDTH == 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 64) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         bins        B16_128b      = { 3'b100 };
         bins        B32_256b      = { 3'b101 };
         bins        B64_512b      = { 3'b110 };
         ignore_bins ignore_values = { 7 };
      }

      aw_size_esr: coverpoint aw_size {
         option.weight      = (DATA_WIDTH == 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 32) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         bins        B16_128b      = { 3'b100 };
         bins        B32_256b      = { 3'b101 };
         ignore_bins ignore_values = { [6:7] };
      }

      aw_size_sbm: coverpoint aw_size {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        B1_8b         = { 3'b000 };
         bins        B2_16b        = { 3'b001 };
         bins        B4_32b        = { 3'b010 };
         bins        B8_64b        = { 3'b011 };
         ignore_bins ignore_values = { [4:7] };
      }

      aw_addr_6: coverpoint aw_addr_6 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_5: coverpoint aw_addr_5 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_4: coverpoint aw_addr_4 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_3: coverpoint aw_addr_3 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_2: coverpoint aw_addr_2 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_1: coverpoint aw_addr_1 iff (m_allow_req == 1) {
         bins values[] = { [0:15] };
      }
      aw_addr_0: coverpoint aw_addr_0 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_6: coverpoint aw_addr_edge_6 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_5: coverpoint aw_addr_edge_5 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_4: coverpoint aw_addr_edge_4 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_3: coverpoint aw_addr_edge_3 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_2: coverpoint aw_addr_edge_2 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }
      aw_addr_edge_1: coverpoint aw_addr_edge_1 iff (m_allow_req == 1) {
         bins values[] = { [0:3] };
      }

      aw_user_et: coverpoint aw_user iff (m_allow_req == 1) {
         option.weight      = (USE_AW_USER_ET == 1'b1) ? 1   : 0;
         option.goal        = (USE_AW_USER_ET == 1'b1) ? 100 : 0;
         type_option.weight = (USE_AW_USER_ET == 1'b1) ? 1   : 0;
         type_option.goal   = (USE_AW_USER_ET == 1'b1) ? 100 : 0;

         bins        values[]      = { 0, 5, 8, 9, 10, 11, 15 };
         ignore_bins ignore_values = { 1, 2, 3, 4, 6, 7, 12, 13, 14,[16:$] };
      }

      aw_user_mem: coverpoint aw_user iff (m_allow_req == 1) {
         option.weight      = (USE_AW_USER_MEM == 1'b1) ? 1   : 0;
         option.goal        = (USE_AW_USER_MEM == 1'b1) ? 100 : 0;
         type_option.weight = (USE_AW_USER_MEM == 1'b1) ? 1   : 0;
         type_option.goal   = (USE_AW_USER_MEM == 1'b1) ? 100 : 0;

         bins        values[]      = { 1, 5 };
         ignore_bins ignore_values = { 0, [2:4], [6:$] };
      }

      aw_user_atomic: coverpoint aw_user iff (m_allow_req == 1) {
         option.weight      = (USE_AW_USER_ATOMIC == 1'b1) ? 1   : 0;
         option.goal        = (USE_AW_USER_ATOMIC == 1'b1) ? 100 : 0;
         type_option.weight = (USE_AW_USER_ATOMIC == 1'b1) ? 1   : 0;
         type_option.goal   = (USE_AW_USER_ATOMIC == 1'b1) ? 100 : 0;

         bins        values[]      = { [0:31] };
         ignore_bins ignore_values = { [32:$] };
      }
   endgroup : cg_axi_aw_req


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AW QoS (values of 0 and 1 only)
   //
   // This covergroup is used to capture coverage for high/low priority requests to the memory
   // shire.
   //
   covergroup cg_axi_aw_qos_0_1;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      aw_qos: coverpoint aw_qos {
         bins        values[]      = { 0, 1 };
         ignore_bins ignore_values = { 2, 3 };
      }
   endgroup : cg_axi_aw_qos_0_1


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AW QoS (values of 2 and 3 only)
   //
   // This covergroup is used to capture coverage for message/UC request differentiation for the
   // Minion shire.
   //
   covergroup cg_axi_aw_qos_2_3;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      aw_qos: coverpoint aw_qos {
         bins        values[]      = { 2, 3 };
         ignore_bins ignore_values = { 0, 1 };
      }
   endgroup : cg_axi_aw_qos_2_3


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AW Request Delay
   //
   covergroup cg_axi_aw_req_delay;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      aw_req_delay: coverpoint aw_req_delay {
         bins        values_1_to_4[]       = { [1:4] };
         bins        values_5_to_10        = { [5:10] };
         bins        values_11_to_20       = { [11:20] };
         bins        values_21_to_infinity = { [21:$] };
         ignore_bins ignore_values         = { 0 };
      }
   endgroup : cg_axi_aw_req_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: W Requests
   //
   // This cover group is used to provide coverage for the WSTRB signals.  There are some coverage
   // shortcuts for byte-write operations, halfword-write operations, and word-write operations.  In
   // particular, we can get coverage on these operations if there are write operations that fall
   // into these categories for upper or lower byte lanes.  For example, to get coverage for a
   // w_strb_byte value of 0, it could be a byte write in lane 0, or a byte write in lane 16, or a
   // byte write in lane 32, or a byte write in lane 48.
   //
   covergroup cg_axi_w_req;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      w_valid_to_valid: coverpoint w_idle_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      w_not_ready: coverpoint w_busy_count {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      w_strb_none: coverpoint w_strb_none iff (m_allow_req == 1) {
         option.weight      = (AXI_COV_TYPE == EVL_COV_AXI_SC_64) ? 1   : 0;
         option.goal        = (AXI_COV_TYPE == EVL_COV_AXI_SC_64) ? 100 : 0;
         type_option.weight = (AXI_COV_TYPE == EVL_COV_AXI_SC_64) ? 1   : 0;
         type_option.goal   = (AXI_COV_TYPE == EVL_COV_AXI_SC_64) ? 100 : 0;

         bins        values[]      = { 1 };
         ignore_bins ignore_values = { 0 };
      }
      w_strb_byte_64 : coverpoint w_strb_byte iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        values[]      = { [0:7] };
         ignore_bins ignore_values = { [8:$] };
      }
      w_strb_byte_128 : coverpoint w_strb_byte iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins        values[]      = { [0:15] };
         ignore_bins ignore_values = { [16:$] };
      }
      w_strb_hword_64 : coverpoint w_strb_hword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        values[]      = { [0:3] };
         ignore_bins ignore_values = { [4:$] };
      }
      w_strb_hword_128 : coverpoint w_strb_hword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins        values[]      = { [0:7] };
         ignore_bins ignore_values = { [8:$] };
      }
      w_strb_word_64 : coverpoint w_strb_word iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        values[]      = { [0:1] };
         ignore_bins ignore_values = { [2:$] };
      }
      w_strb_word_128 : coverpoint w_strb_word iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 16) ? 100 : 0;

         bins        values[]      = { [0:3] };
         ignore_bins ignore_values = { [4:$] };
      }
      w_strb_word_256 : coverpoint w_strb_word iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins        values[]      = { [0:7] };
         ignore_bins ignore_values = { [8:$] };
      }
      w_strb_dword_64 : coverpoint w_strb_dword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 8) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 8) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 8) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 8) ? 100 : 0;

         bins        values[]      = { 0 };
         ignore_bins ignore_values = { [1:$] };
      }
      w_strb_dword_128 : coverpoint w_strb_dword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 16) ? 100 : 0;

         bins        values[]      = { [0:1] };
         ignore_bins ignore_values = { [2:$] };
      }
      w_strb_dword_256 : coverpoint w_strb_dword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 32) ? 100 : 0;

         bins        values[]      = { [0:3] };
         ignore_bins ignore_values = { [4:$] };
      }
      w_strb_dword_512 : coverpoint w_strb_dword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 64) ? 100 : 0;

         bins        values[]      = { [0:7] };
         ignore_bins ignore_values = { [8:$] };
      }
      w_strb_qword_128 : coverpoint w_strb_qword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 16) ? 100 : 0;

         bins        values[]      = { 0 };
         ignore_bins ignore_values = { [1:$] };
      }
      w_strb_qword_256 : coverpoint w_strb_qword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 32) ? 100 : 0;

         bins        values[]      = { [0:1] };
         ignore_bins ignore_values = { [2:$] };
      }
      w_strb_qword_512 : coverpoint w_strb_qword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 64) ? 100 : 0;

         bins        values[]      = { [0:3] };
         ignore_bins ignore_values = { [4:$] };
      }
      w_strb_oword_256 : coverpoint w_strb_oword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 32) ? 100 : 0;

         bins        values[]      = { 0 };
         ignore_bins ignore_values = { [1:$] };
      }
      w_strb_oword_512 : coverpoint w_strb_oword iff (m_allow_req == 1) {
         option.weight      = (DATA_WIDTH == 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH == 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH == 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH == 64) ? 100 : 0;

         bins        values[]      = { [0:1] };
         ignore_bins ignore_values = { [2:$] };
      }
      w_strb_all: coverpoint w_strb_full iff (m_allow_req == 1) {
         bins        values[]      = { 1 };
         ignore_bins ignore_values = { 0, 2, 3 };
      }
   endgroup : cg_axi_w_req


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: W Request Delay
   //
   covergroup cg_axi_w_req_delay;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      w_req_delay: coverpoint w_req_delay {
         bins        values_1_to_4[]       = { [1:4] };
         bins        values_5_to_10        = { [5:10] };
         bins        values_11_to_20       = { [11:20] };
         bins        values_21_to_infinity = { [21:$] };
         ignore_bins ignore_values         = { 0 };
      }
   endgroup : cg_axi_w_req_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Write Data
   //
   // This cover group is used to provide coverage for the write data signals.
   //
   covergroup cg_axi_w_data;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      w_data_127: coverpoint w_data_127 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_126: coverpoint w_data_126 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_125: coverpoint w_data_125 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_124: coverpoint w_data_124 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_123: coverpoint w_data_123 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_122: coverpoint w_data_122 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_121: coverpoint w_data_121 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_120: coverpoint w_data_120 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_119: coverpoint w_data_119 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_118: coverpoint w_data_118 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_117: coverpoint w_data_117 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_116: coverpoint w_data_116 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_115: coverpoint w_data_115 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_114: coverpoint w_data_114 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_113: coverpoint w_data_113 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_112: coverpoint w_data_112 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_111: coverpoint w_data_111 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_110: coverpoint w_data_110 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_109: coverpoint w_data_109 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_108: coverpoint w_data_108 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_107: coverpoint w_data_107 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_106: coverpoint w_data_106 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_105: coverpoint w_data_105 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_104: coverpoint w_data_104 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_103: coverpoint w_data_103 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_102: coverpoint w_data_102 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_101: coverpoint w_data_101 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_100: coverpoint w_data_100 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_099: coverpoint w_data_099 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_098: coverpoint w_data_098 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_097: coverpoint w_data_097 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_096: coverpoint w_data_096 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_095: coverpoint w_data_095 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_094: coverpoint w_data_094 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_093: coverpoint w_data_093 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_092: coverpoint w_data_092 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_091: coverpoint w_data_091 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_090: coverpoint w_data_090 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_089: coverpoint w_data_089 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_088: coverpoint w_data_088 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_087: coverpoint w_data_087 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_086: coverpoint w_data_086 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_085: coverpoint w_data_085 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_084: coverpoint w_data_084 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_083: coverpoint w_data_083 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_082: coverpoint w_data_082 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_081: coverpoint w_data_081 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_080: coverpoint w_data_080 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_079: coverpoint w_data_079 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_078: coverpoint w_data_078 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_077: coverpoint w_data_077 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_076: coverpoint w_data_076 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_075: coverpoint w_data_075 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_074: coverpoint w_data_074 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_073: coverpoint w_data_073 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_072: coverpoint w_data_072 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_071: coverpoint w_data_071 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_070: coverpoint w_data_070 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_069: coverpoint w_data_069 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_068: coverpoint w_data_068 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_067: coverpoint w_data_067 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_066: coverpoint w_data_066 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_065: coverpoint w_data_065 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_064: coverpoint w_data_064 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_063: coverpoint w_data_063 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_062: coverpoint w_data_062 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_061: coverpoint w_data_061 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_060: coverpoint w_data_060 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_059: coverpoint w_data_059 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_058: coverpoint w_data_058 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_057: coverpoint w_data_057 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_056: coverpoint w_data_056 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_055: coverpoint w_data_055 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_054: coverpoint w_data_054 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_053: coverpoint w_data_053 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_052: coverpoint w_data_052 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_051: coverpoint w_data_051 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_050: coverpoint w_data_050 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_049: coverpoint w_data_049 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_048: coverpoint w_data_048 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_047: coverpoint w_data_047 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_046: coverpoint w_data_046 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_045: coverpoint w_data_045 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_044: coverpoint w_data_044 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_043: coverpoint w_data_043 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_042: coverpoint w_data_042 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_041: coverpoint w_data_041 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_040: coverpoint w_data_040 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_039: coverpoint w_data_039 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_038: coverpoint w_data_038 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_037: coverpoint w_data_037 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_036: coverpoint w_data_036 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_035: coverpoint w_data_035 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_034: coverpoint w_data_034 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_033: coverpoint w_data_033 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_032: coverpoint w_data_032 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_031: coverpoint w_data_031 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_030: coverpoint w_data_030 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_029: coverpoint w_data_029 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_028: coverpoint w_data_028 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_027: coverpoint w_data_027 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_026: coverpoint w_data_026 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_025: coverpoint w_data_025 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_024: coverpoint w_data_024 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_023: coverpoint w_data_023 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_022: coverpoint w_data_022 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_021: coverpoint w_data_021 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_020: coverpoint w_data_020 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_019: coverpoint w_data_019 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_018: coverpoint w_data_018 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_017: coverpoint w_data_017 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_016: coverpoint w_data_016 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      w_data_015: coverpoint w_data_015 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_014: coverpoint w_data_014 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_013: coverpoint w_data_013 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_012: coverpoint w_data_012 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_011: coverpoint w_data_011 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_010: coverpoint w_data_010 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_009: coverpoint w_data_009 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_008: coverpoint w_data_008 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_007: coverpoint w_data_007 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_006: coverpoint w_data_006 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_005: coverpoint w_data_005 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_004: coverpoint w_data_004 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_003: coverpoint w_data_003 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_002: coverpoint w_data_002 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_001: coverpoint w_data_001 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_000: coverpoint w_data_000 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      w_data_edge_127: coverpoint w_data_edge_127 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_126: coverpoint w_data_edge_126 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_125: coverpoint w_data_edge_125 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_124: coverpoint w_data_edge_124 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_123: coverpoint w_data_edge_123 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_122: coverpoint w_data_edge_122 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_121: coverpoint w_data_edge_121 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_120: coverpoint w_data_edge_120 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_119: coverpoint w_data_edge_119 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_118: coverpoint w_data_edge_118 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_117: coverpoint w_data_edge_117 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_116: coverpoint w_data_edge_116 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_115: coverpoint w_data_edge_115 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_114: coverpoint w_data_edge_114 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_113: coverpoint w_data_edge_113 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_112: coverpoint w_data_edge_112 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_111: coverpoint w_data_edge_111 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_110: coverpoint w_data_edge_110 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_109: coverpoint w_data_edge_109 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_108: coverpoint w_data_edge_108 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_107: coverpoint w_data_edge_107 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_106: coverpoint w_data_edge_106 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_105: coverpoint w_data_edge_105 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_104: coverpoint w_data_edge_104 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_103: coverpoint w_data_edge_103 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_102: coverpoint w_data_edge_102 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_101: coverpoint w_data_edge_101 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_100: coverpoint w_data_edge_100 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_099: coverpoint w_data_edge_099 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_098: coverpoint w_data_edge_098 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_097: coverpoint w_data_edge_097 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_096: coverpoint w_data_edge_096 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_095: coverpoint w_data_edge_095 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_094: coverpoint w_data_edge_094 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_093: coverpoint w_data_edge_093 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_092: coverpoint w_data_edge_092 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_091: coverpoint w_data_edge_091 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_090: coverpoint w_data_edge_090 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_089: coverpoint w_data_edge_089 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_088: coverpoint w_data_edge_088 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_087: coverpoint w_data_edge_087 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_086: coverpoint w_data_edge_086 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_085: coverpoint w_data_edge_085 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_084: coverpoint w_data_edge_084 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_083: coverpoint w_data_edge_083 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_082: coverpoint w_data_edge_082 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_081: coverpoint w_data_edge_081 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_080: coverpoint w_data_edge_080 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_079: coverpoint w_data_edge_079 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_078: coverpoint w_data_edge_078 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_077: coverpoint w_data_edge_077 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_076: coverpoint w_data_edge_076 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_075: coverpoint w_data_edge_075 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_074: coverpoint w_data_edge_074 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_073: coverpoint w_data_edge_073 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_072: coverpoint w_data_edge_072 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_071: coverpoint w_data_edge_071 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_070: coverpoint w_data_edge_070 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_069: coverpoint w_data_edge_069 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_068: coverpoint w_data_edge_068 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_067: coverpoint w_data_edge_067 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_066: coverpoint w_data_edge_066 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_065: coverpoint w_data_edge_065 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_064: coverpoint w_data_edge_064 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_063: coverpoint w_data_edge_063 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_062: coverpoint w_data_edge_062 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_061: coverpoint w_data_edge_061 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_060: coverpoint w_data_edge_060 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_059: coverpoint w_data_edge_059 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_058: coverpoint w_data_edge_058 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_057: coverpoint w_data_edge_057 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_056: coverpoint w_data_edge_056 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_055: coverpoint w_data_edge_055 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_054: coverpoint w_data_edge_054 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_053: coverpoint w_data_edge_053 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_052: coverpoint w_data_edge_052 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_051: coverpoint w_data_edge_051 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_050: coverpoint w_data_edge_050 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_049: coverpoint w_data_edge_049 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_048: coverpoint w_data_edge_048 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_047: coverpoint w_data_edge_047 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_046: coverpoint w_data_edge_046 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_045: coverpoint w_data_edge_045 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_044: coverpoint w_data_edge_044 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_043: coverpoint w_data_edge_043 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_042: coverpoint w_data_edge_042 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_041: coverpoint w_data_edge_041 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_040: coverpoint w_data_edge_040 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_039: coverpoint w_data_edge_039 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_038: coverpoint w_data_edge_038 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_037: coverpoint w_data_edge_037 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_036: coverpoint w_data_edge_036 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_035: coverpoint w_data_edge_035 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_034: coverpoint w_data_edge_034 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_033: coverpoint w_data_edge_033 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_032: coverpoint w_data_edge_032 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_031: coverpoint w_data_edge_031 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_030: coverpoint w_data_edge_030 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_029: coverpoint w_data_edge_029 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_028: coverpoint w_data_edge_028 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_027: coverpoint w_data_edge_027 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_026: coverpoint w_data_edge_026 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_025: coverpoint w_data_edge_025 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_024: coverpoint w_data_edge_024 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_023: coverpoint w_data_edge_023 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_022: coverpoint w_data_edge_022 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_021: coverpoint w_data_edge_021 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_020: coverpoint w_data_edge_020 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_019: coverpoint w_data_edge_019 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_018: coverpoint w_data_edge_018 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_017: coverpoint w_data_edge_017 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_016: coverpoint w_data_edge_016 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      w_data_edge_015: coverpoint w_data_edge_015 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_014: coverpoint w_data_edge_014 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_013: coverpoint w_data_edge_013 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_012: coverpoint w_data_edge_012 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_011: coverpoint w_data_edge_011 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_010: coverpoint w_data_edge_010 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_009: coverpoint w_data_edge_009 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_008: coverpoint w_data_edge_008 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_007: coverpoint w_data_edge_007 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_006: coverpoint w_data_edge_006 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_005: coverpoint w_data_edge_005 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_004: coverpoint w_data_edge_004 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_003: coverpoint w_data_edge_003 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_002: coverpoint w_data_edge_002 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      w_data_edge_001: coverpoint w_data_edge_001 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
   endgroup : cg_axi_w_data


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: R Responses
   //
   // This cover group is used to provide coverage for requests on the R channel.  It consists of
   // cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back requests (as
   //    well as some limited number of cycles between requests).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    AXI ready has been deasserted for some number of cycles.
   //
   // resp:
   //    There is a cover point for all of the valid AXI responses for this interface.
   //
   covergroup cg_axi_r_rsp;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      r_valid_to_valid: coverpoint r_idle_count iff (m_allow_req == 1'b1) {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      r_not_ready: coverpoint r_busy_count iff (m_allow_req == 1'b1) {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      r_resp: coverpoint r_resp iff (m_allow_req == 1'b1) {
         bins        OKAY    = { 2'b00 };
         bins        SLVERRR = { 2'b10 };
         bins        DECERR  = { 2'b11 };
         ignore_bins ignore_values = { 2'b01 };
      }
   endgroup : cg_axi_r_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: R Response Delay
   //
   covergroup cg_axi_r_rsp_delay;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      r_rsp_delay: coverpoint r_rsp_delay iff (m_allow_req == 1'b1) {
         bins        values_1_to_4[]       = { [1:4] };
         bins        values_5_to_10        = { [5:10] };
         bins        values_11_to_20       = { [11:20] };
         bins        values_21_to_infinity = { [21:$] };
         ignore_bins ignore_values         = { 0 };
      }

      r_rsp_ooo: coverpoint r_rsp_ooo iff (m_allow_req == 1'b1) {
         bins values[] = { [0:1] };
      }
   endgroup : cg_axi_r_rsp_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: B Responses
   //
   // This cover group is used to provide coverage for requests on the B channel.  It consists of
   // cover points for the following fields:
   //
   // valid-to-valid delays:
   //    There is a cover point so we can determine if there have been back-to-back requests (as
   //    well as some limited number of cycles between requests).
   //
   // not-ready delays:
   //    There is a cover point so we can determine if there have been valid assertions while the
   //    AXI ready has been deasserted for some number of cycles.
   //
   // resp:
   //    There is a cover point for all of the valid AXI responses for this interface.
   //
   covergroup cg_axi_b_rsp;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      b_valid_to_valid: coverpoint b_idle_count iff (m_allow_req == 1'b1) {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      b_not_ready: coverpoint b_busy_count iff (m_allow_req == 1'b1) {
         bins values_0         = { 0 };
         bins values_1         = { 1 };
         bins values_2         = { 2 };
         bins values_3         = { 3 };
         bins values_4         = { 4 };
         bins values_5_or_more = { [5:$] };
      }

      b_resp: coverpoint b_resp iff (m_allow_req == 1'b1) {
         bins        OKAY    = { 2'b00 };
         bins        SLVERRR = { 2'b10 };
         bins        DECERR  = { 2'b11 };
         ignore_bins ignore_values = { 2'b01 };
      }
   endgroup : cg_axi_b_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: B Response Delay
   //
   covergroup cg_axi_b_rsp_delay;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      b_rsp_delay: coverpoint b_rsp_delay iff (m_allow_req == 1'b1) {
         bins        values_1_to_4[]       = { [1:4] };
         bins        values_5_to_10        = { [5:10] };
         bins        values_11_to_20       = { [11:20] };
         bins        values_21_to_infinity = { [21:$] };
         ignore_bins ignore_values         = { 0 };
      }

      b_rsp_ooo: coverpoint b_rsp_ooo iff (m_allow_req == 1'b1) {
         bins values[] = { [0:1] };
      }
   endgroup : cg_axi_b_rsp_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Read Data
   //
   // This cover group is used to provide coverage for the read data signals.
   //
   covergroup cg_axi_r_data;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      r_data_127: coverpoint r_data_127 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_126: coverpoint r_data_126 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_125: coverpoint r_data_125 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_124: coverpoint r_data_124 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_123: coverpoint r_data_123 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_122: coverpoint r_data_122 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_121: coverpoint r_data_121 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_120: coverpoint r_data_120 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_119: coverpoint r_data_119 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_118: coverpoint r_data_118 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_117: coverpoint r_data_117 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_116: coverpoint r_data_116 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_115: coverpoint r_data_115 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_114: coverpoint r_data_114 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_113: coverpoint r_data_113 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_112: coverpoint r_data_112 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_111: coverpoint r_data_111 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_110: coverpoint r_data_110 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_109: coverpoint r_data_109 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_108: coverpoint r_data_108 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_107: coverpoint r_data_107 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_106: coverpoint r_data_106 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_105: coverpoint r_data_105 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_104: coverpoint r_data_104 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_103: coverpoint r_data_103 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_102: coverpoint r_data_102 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_101: coverpoint r_data_101 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_100: coverpoint r_data_100 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_099: coverpoint r_data_099 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_098: coverpoint r_data_098 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_097: coverpoint r_data_097 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_096: coverpoint r_data_096 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_095: coverpoint r_data_095 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_094: coverpoint r_data_094 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_093: coverpoint r_data_093 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_092: coverpoint r_data_092 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_091: coverpoint r_data_091 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_090: coverpoint r_data_090 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_089: coverpoint r_data_089 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_088: coverpoint r_data_088 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_087: coverpoint r_data_087 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_086: coverpoint r_data_086 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_085: coverpoint r_data_085 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_084: coverpoint r_data_084 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_083: coverpoint r_data_083 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_082: coverpoint r_data_082 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_081: coverpoint r_data_081 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_080: coverpoint r_data_080 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_079: coverpoint r_data_079 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_078: coverpoint r_data_078 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_077: coverpoint r_data_077 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_076: coverpoint r_data_076 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_075: coverpoint r_data_075 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_074: coverpoint r_data_074 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_073: coverpoint r_data_073 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_072: coverpoint r_data_072 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_071: coverpoint r_data_071 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_070: coverpoint r_data_070 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_069: coverpoint r_data_069 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_068: coverpoint r_data_068 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_067: coverpoint r_data_067 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_066: coverpoint r_data_066 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_065: coverpoint r_data_065 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_064: coverpoint r_data_064 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_063: coverpoint r_data_063 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_062: coverpoint r_data_062 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_061: coverpoint r_data_061 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_060: coverpoint r_data_060 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_059: coverpoint r_data_059 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_058: coverpoint r_data_058 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_057: coverpoint r_data_057 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_056: coverpoint r_data_056 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_055: coverpoint r_data_055 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_054: coverpoint r_data_054 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_053: coverpoint r_data_053 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_052: coverpoint r_data_052 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_051: coverpoint r_data_051 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_050: coverpoint r_data_050 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_049: coverpoint r_data_049 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_048: coverpoint r_data_048 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_047: coverpoint r_data_047 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_046: coverpoint r_data_046 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_045: coverpoint r_data_045 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_044: coverpoint r_data_044 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_043: coverpoint r_data_043 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_042: coverpoint r_data_042 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_041: coverpoint r_data_041 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_040: coverpoint r_data_040 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_039: coverpoint r_data_039 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_038: coverpoint r_data_038 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_037: coverpoint r_data_037 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_036: coverpoint r_data_036 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_035: coverpoint r_data_035 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_034: coverpoint r_data_034 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_033: coverpoint r_data_033 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_032: coverpoint r_data_032 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_031: coverpoint r_data_031 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_030: coverpoint r_data_030 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_029: coverpoint r_data_029 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_028: coverpoint r_data_028 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_027: coverpoint r_data_027 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_026: coverpoint r_data_026 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_025: coverpoint r_data_025 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_024: coverpoint r_data_024 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_023: coverpoint r_data_023 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_022: coverpoint r_data_022 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_021: coverpoint r_data_021 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_020: coverpoint r_data_020 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_019: coverpoint r_data_019 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_018: coverpoint r_data_018 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_017: coverpoint r_data_017 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_016: coverpoint r_data_016 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:15] };
      }
      r_data_015: coverpoint r_data_015 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_014: coverpoint r_data_014 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_013: coverpoint r_data_013 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_012: coverpoint r_data_012 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_011: coverpoint r_data_011 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_010: coverpoint r_data_010 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_009: coverpoint r_data_009 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_008: coverpoint r_data_008 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_007: coverpoint r_data_007 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_006: coverpoint r_data_006 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_005: coverpoint r_data_005 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_004: coverpoint r_data_004 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_003: coverpoint r_data_003 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_002: coverpoint r_data_002 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_001: coverpoint r_data_001 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_000: coverpoint r_data_000 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:15] };
      }
      r_data_edge_127: coverpoint r_data_edge_127 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_126: coverpoint r_data_edge_126 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_125: coverpoint r_data_edge_125 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_124: coverpoint r_data_edge_124 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_123: coverpoint r_data_edge_123 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_122: coverpoint r_data_edge_122 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_121: coverpoint r_data_edge_121 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_120: coverpoint r_data_edge_120 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_119: coverpoint r_data_edge_119 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_118: coverpoint r_data_edge_118 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_117: coverpoint r_data_edge_117 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_116: coverpoint r_data_edge_116 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_115: coverpoint r_data_edge_115 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_114: coverpoint r_data_edge_114 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_113: coverpoint r_data_edge_113 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_112: coverpoint r_data_edge_112 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_111: coverpoint r_data_edge_111 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_110: coverpoint r_data_edge_110 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_109: coverpoint r_data_edge_109 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_108: coverpoint r_data_edge_108 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_107: coverpoint r_data_edge_107 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_106: coverpoint r_data_edge_106 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_105: coverpoint r_data_edge_105 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_104: coverpoint r_data_edge_104 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_103: coverpoint r_data_edge_103 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_102: coverpoint r_data_edge_102 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_101: coverpoint r_data_edge_101 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_100: coverpoint r_data_edge_100 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_099: coverpoint r_data_edge_099 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_098: coverpoint r_data_edge_098 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_097: coverpoint r_data_edge_097 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_096: coverpoint r_data_edge_096 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_095: coverpoint r_data_edge_095 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_094: coverpoint r_data_edge_094 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_093: coverpoint r_data_edge_093 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_092: coverpoint r_data_edge_092 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_091: coverpoint r_data_edge_091 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_090: coverpoint r_data_edge_090 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_089: coverpoint r_data_edge_089 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_088: coverpoint r_data_edge_088 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_087: coverpoint r_data_edge_087 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_086: coverpoint r_data_edge_086 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_085: coverpoint r_data_edge_085 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_084: coverpoint r_data_edge_084 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_083: coverpoint r_data_edge_083 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_082: coverpoint r_data_edge_082 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_081: coverpoint r_data_edge_081 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_080: coverpoint r_data_edge_080 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_079: coverpoint r_data_edge_079 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_078: coverpoint r_data_edge_078 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_077: coverpoint r_data_edge_077 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_076: coverpoint r_data_edge_076 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_075: coverpoint r_data_edge_075 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_074: coverpoint r_data_edge_074 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_073: coverpoint r_data_edge_073 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_072: coverpoint r_data_edge_072 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_071: coverpoint r_data_edge_071 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_070: coverpoint r_data_edge_070 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_069: coverpoint r_data_edge_069 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_068: coverpoint r_data_edge_068 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_067: coverpoint r_data_edge_067 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_066: coverpoint r_data_edge_066 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_065: coverpoint r_data_edge_065 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_064: coverpoint r_data_edge_064 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 64) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 64) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 64) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 64) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_063: coverpoint r_data_edge_063 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_062: coverpoint r_data_edge_062 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_061: coverpoint r_data_edge_061 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_060: coverpoint r_data_edge_060 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_059: coverpoint r_data_edge_059 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_058: coverpoint r_data_edge_058 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_057: coverpoint r_data_edge_057 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_056: coverpoint r_data_edge_056 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_055: coverpoint r_data_edge_055 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_054: coverpoint r_data_edge_054 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_053: coverpoint r_data_edge_053 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_052: coverpoint r_data_edge_052 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_051: coverpoint r_data_edge_051 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_050: coverpoint r_data_edge_050 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_049: coverpoint r_data_edge_049 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_048: coverpoint r_data_edge_048 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_047: coverpoint r_data_edge_047 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_046: coverpoint r_data_edge_046 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_045: coverpoint r_data_edge_045 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_044: coverpoint r_data_edge_044 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_043: coverpoint r_data_edge_043 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_042: coverpoint r_data_edge_042 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_041: coverpoint r_data_edge_041 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_040: coverpoint r_data_edge_040 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_039: coverpoint r_data_edge_039 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_038: coverpoint r_data_edge_038 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_037: coverpoint r_data_edge_037 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_036: coverpoint r_data_edge_036 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_035: coverpoint r_data_edge_035 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_034: coverpoint r_data_edge_034 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_033: coverpoint r_data_edge_033 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_032: coverpoint r_data_edge_032 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 32) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 32) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 32) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 32) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_031: coverpoint r_data_edge_031 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_030: coverpoint r_data_edge_030 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_029: coverpoint r_data_edge_029 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_028: coverpoint r_data_edge_028 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_027: coverpoint r_data_edge_027 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_026: coverpoint r_data_edge_026 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_025: coverpoint r_data_edge_025 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_024: coverpoint r_data_edge_024 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_023: coverpoint r_data_edge_023 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_022: coverpoint r_data_edge_022 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_021: coverpoint r_data_edge_021 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_020: coverpoint r_data_edge_020 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_019: coverpoint r_data_edge_019 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_018: coverpoint r_data_edge_018 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_017: coverpoint r_data_edge_017 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_016: coverpoint r_data_edge_016 iff (m_allow_req == 1'b1) {
         option.weight      = (DATA_WIDTH >= 16) ? 1   : 0;
         option.goal        = (DATA_WIDTH >= 16) ? 100 : 0;
         type_option.weight = (DATA_WIDTH >= 16) ? 1   : 0;
         type_option.goal   = (DATA_WIDTH >= 16) ? 100 : 0;

         bins values[] = { [0:3] };
      }
      r_data_edge_015: coverpoint r_data_edge_015 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_014: coverpoint r_data_edge_014 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_013: coverpoint r_data_edge_013 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_012: coverpoint r_data_edge_012 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_011: coverpoint r_data_edge_011 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_010: coverpoint r_data_edge_010 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_009: coverpoint r_data_edge_009 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_008: coverpoint r_data_edge_008 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_007: coverpoint r_data_edge_007 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_006: coverpoint r_data_edge_006 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_005: coverpoint r_data_edge_005 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_004: coverpoint r_data_edge_004 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_003: coverpoint r_data_edge_003 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_002: coverpoint r_data_edge_002 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
      r_data_edge_001: coverpoint r_data_edge_001 iff (m_allow_req == 1'b1) {
         bins values[] = { [0:3] };
      }
   endgroup : cg_axi_r_data


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Concurrent Requests
   //
   covergroup cg_axi_concurrent_req;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      concurrent_req: coverpoint concurrent_req {
         bins        ar_req        = { 1 };
         bins        aw_req        = { 2 };
         bins        w_req         = { 4 };
         bins        ar_aw_req     = { 3 };
         bins        ar_w_req      = { 5 };
         bins        aw_w_req      = { 6 };
         bins        ar_aw_w_req   = { 7 };
         ignore_bins ignore_values = { 0 };
      }
   endgroup : cg_axi_concurrent_req


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Concurrent Responses
   //
   covergroup cg_axi_concurrent_rsp;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      concurrent_rsp: coverpoint concurrent_rsp {
         bins        r_rsp         = { 1 };
         bins        b_rsp         = { 2 };
         bins        r_b_rsp       = { 3 };
         ignore_bins ignore_values = { 0 };
      }
   endgroup : cg_axi_concurrent_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Active AR Requests
   //
   covergroup cg_axi_ar_active_count(input int limit = 8);
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      ar_active_count: coverpoint ar_active_count {
         bins        values[]      = { [1:limit] };
         ignore_bins ignore_values = { 0, [limit+1:$] };
      }
   endgroup : cg_axi_ar_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Active AW Requests
   //
   covergroup cg_axi_aw_active_count(input int limit = 8);
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      aw_active_count: coverpoint aw_active_count {
         bins        values[]      = { [1:limit] };
         ignore_bins ignore_values = { 0, [limit+1:$] };
      }
   endgroup : cg_axi_aw_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: AW vs W
   //
   // This covergroup looks at how far ahead the AW and W channels get ahead of each other.  If you
   // have zero coverage on W ahead of AW and are using an axi stim agent make sure parameter
   // ALLOW_DRV_W_BEFORE_AW is set to 1 in your evl_axi_interface instantiation.
   //
   covergroup cg_axi_aw_vs_w;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      // "1" will happen when AW and W are on the same cycle simply because of
      // the order things are sent to the monitor.
      num_aw_waiting_for_w: coverpoint num_aw_waiting_for_w iff (count_aw_w == 1) {
         bins valid[] = {[0:10]};
         ignore_bins ignore_values = { [11:$] };
      }
      num_w_waiting_for_aw: coverpoint num_w_waiting_for_aw iff (count_aw_w == 0) {
         bins valid[] = {[0:10]};
         ignore_bins ignore_values = { [11:$] };
      }
   endgroup : cg_axi_aw_vs_w


   //===============================================================================================
   //
   // Local Function Definitions
   //
   // These functions is provided because of System Verilog idiosyncrasies.  It is not really valid
   // to call virtual functions from new(), so we have the virtual version of these functions call
   // the local versions of these functions ... this allows us to also call the local versions from
   // new()..
   //
   //===============================================================================================


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_local
   //
   function void sample_ar_local(input evl_paddr_t      ar_addr_in,
                                 input evl_bus_req_id_t ar_id_in,
                                 input bit [2:0]        ar_size_in,
                                 input bit [3:0]        ar_qos_in,
                                 input int              ar_idle_count_in,
                                 input int              ar_busy_count_in);
      ar_id          = ar_id_in;
      ar_size        = ar_size_in;
      ar_qos         = ar_qos_in;
      ar_addr_6      = ar_addr_in[31:28];
      ar_addr_5      = ar_addr_in[27:24];
      ar_addr_4      = ar_addr_in[23:20];
      ar_addr_3      = ar_addr_in[19:16];
      ar_addr_2      = ar_addr_in[15:12];
      ar_addr_1      = ar_addr_in[11: 8];
      ar_addr_0      = ar_addr_in[ 7: 6];
      ar_addr_edge_6 = ar_addr_in[28:27];
      ar_addr_edge_5 = ar_addr_in[24:23];
      ar_addr_edge_4 = ar_addr_in[20:19];
      ar_addr_edge_3 = ar_addr_in[16:15];
      ar_addr_edge_2 = ar_addr_in[12:11];
      ar_addr_edge_1 = ar_addr_in[ 8: 7];
      ar_idle_count  = ar_idle_count_in;
      ar_busy_count  = ar_busy_count_in;
      cg_axi_ar_req.sample();
      if ((m_use_qos_0_1 == 1'b1) && (m_allow_req == 1'b1)) begin
         cg_axi_ar_qos_0_1.sample();
      end
      if ((m_use_qos_2_3 == 1'b1) && (m_allow_req == 1'b1)) begin
         cg_axi_ar_qos_2_3.sample();
      end
   endfunction : sample_ar_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_local
   //
   function void sample_aw_local(input evl_paddr_t        aw_addr_in,
                                 input evl_bus_req_id_t   aw_id_in,
                                 input bit [2:0]          aw_size_in,
                                 input bit [3:0]          aw_qos_in,
                                 input evl_bus_req_user_t aw_user_in,
                                 input int                aw_idle_count_in,
                                 input int                aw_busy_count_in);
      aw_id          = aw_id_in;
      aw_size        = aw_size_in;
      aw_qos         = aw_qos_in;
      aw_user        = aw_user_in;
      aw_addr_6      = aw_addr_in[31:28];
      aw_addr_5      = aw_addr_in[27:24];
      aw_addr_4      = aw_addr_in[23:20];
      aw_addr_3      = aw_addr_in[19:16];
      aw_addr_2      = aw_addr_in[15:12];
      aw_addr_1      = aw_addr_in[11: 8];
      aw_addr_0      = aw_addr_in[ 7: 6];
      aw_addr_edge_6 = aw_addr_in[28:27];
      aw_addr_edge_5 = aw_addr_in[24:23];
      aw_addr_edge_4 = aw_addr_in[20:19];
      aw_addr_edge_3 = aw_addr_in[16:15];
      aw_addr_edge_2 = aw_addr_in[12:11];
      aw_addr_edge_1 = aw_addr_in[ 8: 7];
      aw_idle_count  = aw_idle_count_in;
      aw_busy_count  = aw_busy_count_in;
      cg_axi_aw_req.sample();
      if ((m_use_qos_0_1 == 1'b1) && (m_allow_req == 1'b1)) begin
         cg_axi_aw_qos_0_1.sample();
      end
      if ((m_use_qos_2_3 == 1'b1) && (m_allow_req == 1'b1)) begin
         cg_axi_aw_qos_2_3.sample();
      end
   endfunction : sample_aw_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w_local
   //
   function void sample_w_local(input bit [63:0]  w_strb_in,
                                input bit [511:0] w_data_in,
                                input int         w_idle_count_in,
                                input int         w_busy_count_in);
      bit [511:0] w_data;

      w_idle_count = w_idle_count_in;
      w_busy_count = w_busy_count_in;
      if (DATA_WIDTH >= 64) begin
         if (m_cover_data == 1'b1) begin
            w_data_127      = w_data_in[511:508] & {4{w_strb_in[63]}};
            w_data_126      = w_data_in[507:504] & {4{w_strb_in[63]}};
            w_data_125      = w_data_in[503:500] & {4{w_strb_in[62]}};
            w_data_124      = w_data_in[499:496] & {4{w_strb_in[62]}};
            w_data_123      = w_data_in[495:492] & {4{w_strb_in[61]}};
            w_data_122      = w_data_in[491:488] & {4{w_strb_in[61]}};
            w_data_121      = w_data_in[487:484] & {4{w_strb_in[60]}};
            w_data_120      = w_data_in[483:480] & {4{w_strb_in[60]}};
            w_data_119      = w_data_in[479:476] & {4{w_strb_in[59]}};
            w_data_118      = w_data_in[475:472] & {4{w_strb_in[59]}};
            w_data_117      = w_data_in[471:468] & {4{w_strb_in[58]}};
            w_data_116      = w_data_in[467:464] & {4{w_strb_in[58]}};
            w_data_115      = w_data_in[463:460] & {4{w_strb_in[57]}};
            w_data_114      = w_data_in[459:456] & {4{w_strb_in[57]}};
            w_data_113      = w_data_in[455:452] & {4{w_strb_in[56]}};
            w_data_112      = w_data_in[451:448] & {4{w_strb_in[56]}};
            w_data_111      = w_data_in[447:444] & {4{w_strb_in[55]}};
            w_data_110      = w_data_in[443:440] & {4{w_strb_in[55]}};
            w_data_109      = w_data_in[439:436] & {4{w_strb_in[54]}};
            w_data_108      = w_data_in[435:432] & {4{w_strb_in[54]}};
            w_data_107      = w_data_in[431:428] & {4{w_strb_in[53]}};
            w_data_106      = w_data_in[427:424] & {4{w_strb_in[53]}};
            w_data_105      = w_data_in[423:420] & {4{w_strb_in[52]}};
            w_data_104      = w_data_in[419:416] & {4{w_strb_in[52]}};
            w_data_103      = w_data_in[415:412] & {4{w_strb_in[51]}};
            w_data_102      = w_data_in[411:408] & {4{w_strb_in[51]}};
            w_data_101      = w_data_in[407:404] & {4{w_strb_in[50]}};
            w_data_100      = w_data_in[403:400] & {4{w_strb_in[50]}};
            w_data_099      = w_data_in[399:396] & {4{w_strb_in[49]}};
            w_data_098      = w_data_in[395:392] & {4{w_strb_in[49]}};
            w_data_097      = w_data_in[391:388] & {4{w_strb_in[48]}};
            w_data_096      = w_data_in[387:384] & {4{w_strb_in[48]}};
            w_data_095      = w_data_in[383:380] & {4{w_strb_in[47]}};
            w_data_094      = w_data_in[379:376] & {4{w_strb_in[47]}};
            w_data_093      = w_data_in[375:372] & {4{w_strb_in[46]}};
            w_data_092      = w_data_in[371:368] & {4{w_strb_in[46]}};
            w_data_091      = w_data_in[367:364] & {4{w_strb_in[45]}};
            w_data_090      = w_data_in[363:360] & {4{w_strb_in[45]}};
            w_data_089      = w_data_in[359:356] & {4{w_strb_in[44]}};
            w_data_088      = w_data_in[355:352] & {4{w_strb_in[44]}};
            w_data_087      = w_data_in[351:348] & {4{w_strb_in[43]}};
            w_data_086      = w_data_in[347:344] & {4{w_strb_in[43]}};
            w_data_085      = w_data_in[343:340] & {4{w_strb_in[42]}};
            w_data_084      = w_data_in[339:336] & {4{w_strb_in[42]}};
            w_data_083      = w_data_in[335:332] & {4{w_strb_in[41]}};
            w_data_082      = w_data_in[331:328] & {4{w_strb_in[41]}};
            w_data_081      = w_data_in[327:324] & {4{w_strb_in[40]}};
            w_data_080      = w_data_in[323:320] & {4{w_strb_in[40]}};
            w_data_079      = w_data_in[319:316] & {4{w_strb_in[39]}};
            w_data_078      = w_data_in[315:312] & {4{w_strb_in[39]}};
            w_data_077      = w_data_in[311:308] & {4{w_strb_in[38]}};
            w_data_076      = w_data_in[307:304] & {4{w_strb_in[38]}};
            w_data_075      = w_data_in[303:300] & {4{w_strb_in[37]}};
            w_data_074      = w_data_in[299:296] & {4{w_strb_in[37]}};
            w_data_073      = w_data_in[295:292] & {4{w_strb_in[36]}};
            w_data_072      = w_data_in[291:288] & {4{w_strb_in[36]}};
            w_data_071      = w_data_in[287:284] & {4{w_strb_in[35]}};
            w_data_070      = w_data_in[283:280] & {4{w_strb_in[35]}};
            w_data_069      = w_data_in[279:276] & {4{w_strb_in[34]}};
            w_data_068      = w_data_in[275:272] & {4{w_strb_in[34]}};
            w_data_067      = w_data_in[271:268] & {4{w_strb_in[33]}};
            w_data_066      = w_data_in[267:264] & {4{w_strb_in[33]}};
            w_data_065      = w_data_in[263:260] & {4{w_strb_in[32]}};
            w_data_064      = w_data_in[259:256] & {4{w_strb_in[32]}};
            w_data_063      = w_data_in[255:252] & {4{w_strb_in[31]}};
            w_data_062      = w_data_in[251:248] & {4{w_strb_in[31]}};
            w_data_061      = w_data_in[247:244] & {4{w_strb_in[30]}};
            w_data_060      = w_data_in[243:240] & {4{w_strb_in[30]}};
            w_data_059      = w_data_in[239:236] & {4{w_strb_in[29]}};
            w_data_058      = w_data_in[235:232] & {4{w_strb_in[29]}};
            w_data_057      = w_data_in[231:228] & {4{w_strb_in[28]}};
            w_data_056      = w_data_in[227:224] & {4{w_strb_in[28]}};
            w_data_055      = w_data_in[223:220] & {4{w_strb_in[27]}};
            w_data_054      = w_data_in[219:216] & {4{w_strb_in[27]}};
            w_data_053      = w_data_in[215:212] & {4{w_strb_in[26]}};
            w_data_052      = w_data_in[211:208] & {4{w_strb_in[26]}};
            w_data_051      = w_data_in[207:204] & {4{w_strb_in[25]}};
            w_data_050      = w_data_in[203:200] & {4{w_strb_in[25]}};
            w_data_049      = w_data_in[199:196] & {4{w_strb_in[24]}};
            w_data_048      = w_data_in[195:192] & {4{w_strb_in[24]}};
            w_data_047      = w_data_in[191:188] & {4{w_strb_in[23]}};
            w_data_046      = w_data_in[187:184] & {4{w_strb_in[23]}};
            w_data_045      = w_data_in[183:180] & {4{w_strb_in[22]}};
            w_data_044      = w_data_in[179:176] & {4{w_strb_in[22]}};
            w_data_043      = w_data_in[175:172] & {4{w_strb_in[21]}};
            w_data_042      = w_data_in[171:168] & {4{w_strb_in[21]}};
            w_data_041      = w_data_in[167:164] & {4{w_strb_in[20]}};
            w_data_040      = w_data_in[163:160] & {4{w_strb_in[20]}};
            w_data_039      = w_data_in[159:156] & {4{w_strb_in[19]}};
            w_data_038      = w_data_in[155:152] & {4{w_strb_in[19]}};
            w_data_037      = w_data_in[151:148] & {4{w_strb_in[18]}};
            w_data_036      = w_data_in[147:144] & {4{w_strb_in[18]}};
            w_data_035      = w_data_in[143:140] & {4{w_strb_in[17]}};
            w_data_034      = w_data_in[139:136] & {4{w_strb_in[17]}};
            w_data_033      = w_data_in[135:132] & {4{w_strb_in[16]}};
            w_data_032      = w_data_in[131:128] & {4{w_strb_in[16]}};
            w_data_031      = w_data_in[127:124] & {4{w_strb_in[15]}};
            w_data_030      = w_data_in[123:120] & {4{w_strb_in[15]}};
            w_data_029      = w_data_in[119:116] & {4{w_strb_in[14]}};
            w_data_028      = w_data_in[115:112] & {4{w_strb_in[14]}};
            w_data_027      = w_data_in[111:108] & {4{w_strb_in[13]}};
            w_data_026      = w_data_in[107:104] & {4{w_strb_in[13]}};
            w_data_025      = w_data_in[103:100] & {4{w_strb_in[12]}};
            w_data_024      = w_data_in[ 99: 96] & {4{w_strb_in[12]}};
            w_data_023      = w_data_in[ 95: 92] & {4{w_strb_in[11]}};
            w_data_022      = w_data_in[ 91: 88] & {4{w_strb_in[11]}};
            w_data_021      = w_data_in[ 87: 84] & {4{w_strb_in[10]}};
            w_data_020      = w_data_in[ 83: 80] & {4{w_strb_in[10]}};
            w_data_019      = w_data_in[ 79: 76] & {4{w_strb_in[ 9]}};
            w_data_018      = w_data_in[ 75: 72] & {4{w_strb_in[ 9]}};
            w_data_017      = w_data_in[ 71: 68] & {4{w_strb_in[ 8]}};
            w_data_016      = w_data_in[ 67: 64] & {4{w_strb_in[ 8]}};
            w_data_015      = w_data_in[ 63: 60] & {4{w_strb_in[ 7]}};
            w_data_014      = w_data_in[ 59: 56] & {4{w_strb_in[ 7]}};
            w_data_013      = w_data_in[ 55: 52] & {4{w_strb_in[ 6]}};
            w_data_012      = w_data_in[ 51: 48] & {4{w_strb_in[ 6]}};
            w_data_011      = w_data_in[ 47: 44] & {4{w_strb_in[ 5]}};
            w_data_010      = w_data_in[ 43: 40] & {4{w_strb_in[ 5]}};
            w_data_009      = w_data_in[ 39: 36] & {4{w_strb_in[ 4]}};
            w_data_008      = w_data_in[ 35: 32] & {4{w_strb_in[ 4]}};
            w_data_007      = w_data_in[ 31: 28] & {4{w_strb_in[ 3]}};
            w_data_006      = w_data_in[ 27: 24] & {4{w_strb_in[ 3]}};
            w_data_005      = w_data_in[ 23: 20] & {4{w_strb_in[ 2]}};
            w_data_004      = w_data_in[ 19: 16] & {4{w_strb_in[ 2]}};
            w_data_003      = w_data_in[ 15: 12] & {4{w_strb_in[ 1]}};
            w_data_002      = w_data_in[ 11:  8] & {4{w_strb_in[ 1]}};
            w_data_001      = w_data_in[  7:  4] & {4{w_strb_in[ 0]}};
            w_data_000      = w_data_in[  3:  0] & {4{w_strb_in[ 0]}};
            w_data_edge_127 = { w_data_127[0:0], w_data_126[3:3] };
            w_data_edge_126 = { w_data_126[0:0], w_data_125[3:3] };
            w_data_edge_125 = { w_data_125[0:0], w_data_124[3:3] };
            w_data_edge_124 = { w_data_124[0:0], w_data_123[3:3] };
            w_data_edge_123 = { w_data_123[0:0], w_data_122[3:3] };
            w_data_edge_122 = { w_data_122[0:0], w_data_121[3:3] };
            w_data_edge_121 = { w_data_121[0:0], w_data_120[3:3] };
            w_data_edge_120 = { w_data_120[0:0], w_data_119[3:3] };
            w_data_edge_119 = { w_data_119[0:0], w_data_118[3:3] };
            w_data_edge_118 = { w_data_118[0:0], w_data_117[3:3] };
            w_data_edge_117 = { w_data_117[0:0], w_data_116[3:3] };
            w_data_edge_116 = { w_data_116[0:0], w_data_115[3:3] };
            w_data_edge_115 = { w_data_115[0:0], w_data_114[3:3] };
            w_data_edge_114 = { w_data_114[0:0], w_data_113[3:3] };
            w_data_edge_113 = { w_data_113[0:0], w_data_112[3:3] };
            w_data_edge_112 = { w_data_112[0:0], w_data_111[3:3] };
            w_data_edge_111 = { w_data_111[0:0], w_data_110[3:3] };
            w_data_edge_110 = { w_data_110[0:0], w_data_109[3:3] };
            w_data_edge_109 = { w_data_109[0:0], w_data_108[3:3] };
            w_data_edge_108 = { w_data_108[0:0], w_data_107[3:3] };
            w_data_edge_107 = { w_data_107[0:0], w_data_106[3:3] };
            w_data_edge_106 = { w_data_106[0:0], w_data_105[3:3] };
            w_data_edge_105 = { w_data_105[0:0], w_data_104[3:3] };
            w_data_edge_104 = { w_data_104[0:0], w_data_103[3:3] };
            w_data_edge_103 = { w_data_103[0:0], w_data_102[3:3] };
            w_data_edge_102 = { w_data_102[0:0], w_data_101[3:3] };
            w_data_edge_101 = { w_data_101[0:0], w_data_100[3:3] };
            w_data_edge_100 = { w_data_100[0:0], w_data_099[3:3] };
            w_data_edge_099 = { w_data_099[0:0], w_data_098[3:3] };
            w_data_edge_098 = { w_data_098[0:0], w_data_097[3:3] };
            w_data_edge_097 = { w_data_097[0:0], w_data_096[3:3] };
            w_data_edge_096 = { w_data_096[0:0], w_data_095[3:3] };
            w_data_edge_095 = { w_data_095[0:0], w_data_094[3:3] };
            w_data_edge_094 = { w_data_094[0:0], w_data_093[3:3] };
            w_data_edge_093 = { w_data_093[0:0], w_data_092[3:3] };
            w_data_edge_092 = { w_data_092[0:0], w_data_091[3:3] };
            w_data_edge_091 = { w_data_091[0:0], w_data_090[3:3] };
            w_data_edge_090 = { w_data_090[0:0], w_data_089[3:3] };
            w_data_edge_089 = { w_data_089[0:0], w_data_088[3:3] };
            w_data_edge_088 = { w_data_088[0:0], w_data_087[3:3] };
            w_data_edge_087 = { w_data_087[0:0], w_data_086[3:3] };
            w_data_edge_086 = { w_data_086[0:0], w_data_085[3:3] };
            w_data_edge_085 = { w_data_085[0:0], w_data_084[3:3] };
            w_data_edge_084 = { w_data_084[0:0], w_data_083[3:3] };
            w_data_edge_083 = { w_data_083[0:0], w_data_082[3:3] };
            w_data_edge_082 = { w_data_082[0:0], w_data_081[3:3] };
            w_data_edge_081 = { w_data_081[0:0], w_data_080[3:3] };
            w_data_edge_080 = { w_data_080[0:0], w_data_079[3:3] };
            w_data_edge_079 = { w_data_079[0:0], w_data_078[3:3] };
            w_data_edge_078 = { w_data_078[0:0], w_data_077[3:3] };
            w_data_edge_077 = { w_data_077[0:0], w_data_076[3:3] };
            w_data_edge_076 = { w_data_076[0:0], w_data_075[3:3] };
            w_data_edge_075 = { w_data_075[0:0], w_data_074[3:3] };
            w_data_edge_074 = { w_data_074[0:0], w_data_073[3:3] };
            w_data_edge_073 = { w_data_073[0:0], w_data_072[3:3] };
            w_data_edge_072 = { w_data_072[0:0], w_data_071[3:3] };
            w_data_edge_071 = { w_data_071[0:0], w_data_070[3:3] };
            w_data_edge_070 = { w_data_070[0:0], w_data_069[3:3] };
            w_data_edge_069 = { w_data_069[0:0], w_data_068[3:3] };
            w_data_edge_068 = { w_data_068[0:0], w_data_067[3:3] };
            w_data_edge_067 = { w_data_067[0:0], w_data_066[3:3] };
            w_data_edge_066 = { w_data_066[0:0], w_data_065[3:3] };
            w_data_edge_065 = { w_data_065[0:0], w_data_064[3:3] };
            w_data_edge_064 = { w_data_064[0:0], w_data_063[3:3] };
            w_data_edge_063 = { w_data_063[0:0], w_data_062[3:3] };
            w_data_edge_062 = { w_data_062[0:0], w_data_061[3:3] };
            w_data_edge_061 = { w_data_061[0:0], w_data_060[3:3] };
            w_data_edge_060 = { w_data_060[0:0], w_data_059[3:3] };
            w_data_edge_059 = { w_data_059[0:0], w_data_058[3:3] };
            w_data_edge_058 = { w_data_058[0:0], w_data_057[3:3] };
            w_data_edge_057 = { w_data_057[0:0], w_data_056[3:3] };
            w_data_edge_056 = { w_data_056[0:0], w_data_055[3:3] };
            w_data_edge_055 = { w_data_055[0:0], w_data_054[3:3] };
            w_data_edge_054 = { w_data_054[0:0], w_data_053[3:3] };
            w_data_edge_053 = { w_data_053[0:0], w_data_052[3:3] };
            w_data_edge_052 = { w_data_052[0:0], w_data_051[3:3] };
            w_data_edge_051 = { w_data_051[0:0], w_data_050[3:3] };
            w_data_edge_050 = { w_data_050[0:0], w_data_049[3:3] };
            w_data_edge_049 = { w_data_049[0:0], w_data_048[3:3] };
            w_data_edge_048 = { w_data_048[0:0], w_data_047[3:3] };
            w_data_edge_047 = { w_data_047[0:0], w_data_046[3:3] };
            w_data_edge_046 = { w_data_046[0:0], w_data_045[3:3] };
            w_data_edge_045 = { w_data_045[0:0], w_data_044[3:3] };
            w_data_edge_044 = { w_data_044[0:0], w_data_043[3:3] };
            w_data_edge_043 = { w_data_043[0:0], w_data_042[3:3] };
            w_data_edge_042 = { w_data_042[0:0], w_data_041[3:3] };
            w_data_edge_041 = { w_data_041[0:0], w_data_040[3:3] };
            w_data_edge_040 = { w_data_040[0:0], w_data_039[3:3] };
            w_data_edge_039 = { w_data_039[0:0], w_data_038[3:3] };
            w_data_edge_038 = { w_data_038[0:0], w_data_037[3:3] };
            w_data_edge_037 = { w_data_037[0:0], w_data_036[3:3] };
            w_data_edge_036 = { w_data_036[0:0], w_data_035[3:3] };
            w_data_edge_035 = { w_data_035[0:0], w_data_034[3:3] };
            w_data_edge_034 = { w_data_034[0:0], w_data_033[3:3] };
            w_data_edge_033 = { w_data_033[0:0], w_data_032[3:3] };
            w_data_edge_032 = { w_data_032[0:0], w_data_031[3:3] };
            w_data_edge_031 = { w_data_031[0:0], w_data_030[3:3] };
            w_data_edge_030 = { w_data_030[0:0], w_data_029[3:3] };
            w_data_edge_029 = { w_data_029[0:0], w_data_028[3:3] };
            w_data_edge_028 = { w_data_028[0:0], w_data_027[3:3] };
            w_data_edge_027 = { w_data_027[0:0], w_data_026[3:3] };
            w_data_edge_026 = { w_data_026[0:0], w_data_025[3:3] };
            w_data_edge_025 = { w_data_025[0:0], w_data_024[3:3] };
            w_data_edge_024 = { w_data_024[0:0], w_data_023[3:3] };
            w_data_edge_023 = { w_data_023[0:0], w_data_022[3:3] };
            w_data_edge_022 = { w_data_022[0:0], w_data_021[3:3] };
            w_data_edge_021 = { w_data_021[0:0], w_data_020[3:3] };
            w_data_edge_020 = { w_data_020[0:0], w_data_019[3:3] };
            w_data_edge_019 = { w_data_019[0:0], w_data_018[3:3] };
            w_data_edge_018 = { w_data_018[0:0], w_data_017[3:3] };
            w_data_edge_017 = { w_data_017[0:0], w_data_016[3:3] };
            w_data_edge_016 = { w_data_016[0:0], w_data_015[3:3] };
            w_data_edge_015 = { w_data_015[0:0], w_data_014[3:3] };
            w_data_edge_014 = { w_data_014[0:0], w_data_013[3:3] };
            w_data_edge_013 = { w_data_013[0:0], w_data_012[3:3] };
            w_data_edge_012 = { w_data_012[0:0], w_data_011[3:3] };
            w_data_edge_011 = { w_data_011[0:0], w_data_010[3:3] };
            w_data_edge_010 = { w_data_010[0:0], w_data_009[3:3] };
            w_data_edge_009 = { w_data_009[0:0], w_data_008[3:3] };
            w_data_edge_008 = { w_data_008[0:0], w_data_007[3:3] };
            w_data_edge_007 = { w_data_007[0:0], w_data_006[3:3] };
            w_data_edge_006 = { w_data_006[0:0], w_data_005[3:3] };
            w_data_edge_005 = { w_data_005[0:0], w_data_004[3:3] };
            w_data_edge_004 = { w_data_004[0:0], w_data_003[3:3] };
            w_data_edge_003 = { w_data_003[0:0], w_data_002[3:3] };
            w_data_edge_002 = { w_data_002[0:0], w_data_001[3:3] };
            w_data_edge_001 = { w_data_001[0:0], w_data_000[3:3] };
         end
         case (w_strb_in)
            64'h0000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b1 };

            64'h0000000000000001,
            64'h0000000000010000,
            64'h0000000100000000,
            64'h0001000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h00,       1'b0 };

            64'h0000000000000002,
            64'h0000000000020000,
            64'h0000000200000000,
            64'h0002000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h01,       1'b0 };

            64'h0000000000000004,
            64'h0000000000040000,
            64'h0000000400000000,
            64'h0004000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h02,       1'b0 };

            64'h0000000000000008,
            64'h0000000000080000,
            64'h0000000800000000,
            64'h0008000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h03,       1'b0 };

            64'h0000000000000010,
            64'h0000000000100000,
            64'h0000001000000000,
            64'h0010000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h04,       1'b0 };

            64'h0000000000000020,
            64'h0000000000200000,
            64'h0000002000000000,
            64'h0020000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h05,       1'b0 };

            64'h0000000000000040,
            64'h0000000000400000,
            64'h0000004000000000,
            64'h0040000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h06,       1'b0 };

            64'h0000000000000080,
            64'h0000000000800000,
            64'h0000008000000000,
            64'h0080000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h07,       1'b0 };

            64'h0000000000000100,
            64'h0000000001000000,
            64'h0000010000000000,
            64'h0100000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h08,       1'b0 };

            64'h0000000000000200,
            64'h0000000002000000,
            64'h0000020000000000,
            64'h0200000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h09,       1'b0 };

            64'h0000000000000400,
            64'h0000000004000000,
            64'h0000040000000000,
            64'h0400000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0a,       1'b0 };

            64'h0000000000000800,
            64'h0000000008000000,
            64'h0000080000000000,
            64'h0800000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0b,       1'b0 };

            64'h0000000000001000,
            64'h0000000010000000,
            64'h0000100000000000,
            64'h1000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0c,       1'b0 };

            64'h0000000000002000,
            64'h0000000020000000,
            64'h0000200000000000,
            64'h2000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0d,       1'b0 };

            64'h0000000000004000,
            64'h0000000040000000,
            64'h0000400000000000,
            64'h4000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0e,       1'b0 };

            64'h0000000000008000,
            64'h0000000080000000,
            64'h0000800000000000,
            64'h8000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0f,       1'b0 };

            64'h0000000000000003,
            64'h0000000000030000,
            64'h0000000300000000,
            64'h0003000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h0,         5'h1f,       1'b0 };

            64'h000000000000000c,
            64'h00000000000c0000,
            64'h0000000c00000000,
            64'h000c000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h1,         5'h1f,       1'b0 };

            64'h0000000000000030,
            64'h0000000000300000,
            64'h0000003000000000,
            64'h0030000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h2,         5'h1f,       1'b0 };

            64'h00000000000000c0,
            64'h0000000000c00000,
            64'h000000c000000000,
            64'h00c0000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h3,         5'h1f,       1'b0 };

            64'h0000000000000300,
            64'h0000000003000000,
            64'h0000030000000000,
            64'h0300000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h4,         5'h1f,       1'b0 };

            64'h0000000000000c00,
            64'h000000000c000000,
            64'h00000c0000000000,
            64'h0c00000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h5,         5'h1f,       1'b0 };

            64'h0000000000003000,
            64'h0000000030000000,
            64'h0000300000000000,
            64'h3000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h6,         5'h1f,       1'b0 };

            64'h000000000000c000,
            64'h00000000c0000000,
            64'h0000c00000000000,
            64'hc000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h7,         5'h1f,       1'b0 };

            64'h000000000000000f,
            64'h0000000f00000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h0,        4'hf,         5'h1f,       1'b0 };

            64'h00000000000000f0,
            64'h000000f000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h1,        4'hf,         5'h1f,       1'b0 };

            64'h0000000000000f00,
            64'h00000f0000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h2,        4'hf,         5'h1f,       1'b0 };

            64'h000000000000f000,
            64'h0000f00000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h3,        4'hf,         5'h1f,       1'b0 };

            64'h00000000000f0000,
            64'h000f000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h4,        4'hf,         5'h1f,       1'b0 };

            64'h0000000000f00000,
            64'h00f0000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h5,        4'hf,         5'h1f,       1'b0 };

            64'h000000000f000000,
            64'hf000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h6,        4'hf,         5'h1f,       1'b0 };

            64'h00000000f0000000,
            64'hf000000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'hf,         4'h7,        4'hf,         5'h1f,       1'b0 };

            64'h00000000000000ff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h0,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h000000000000ff00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h1,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h0000000000ff0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h2,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h00000000ff000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h3,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h000000ff00000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h4,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h0000ff0000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h5,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h00ff000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h6,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'hff00000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b111,       4'h7,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h000000000000ffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b000,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h00000000ffff0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b001,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h0000ffff00000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b010,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'hffff000000000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b11,        3'b011,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'h00000000ffffffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b00,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'hffffffff00000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b00,       2'b01,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            64'hffffffffffffffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b01,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            default:              { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                                  { 2'b10,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };
         endcase
      end
      else if (DATA_WIDTH >= 32) begin
         if (m_cover_data == 1'b1) begin
            w_data_063      = w_data_in[255:252] & {4{w_strb_in[31]}};
            w_data_062      = w_data_in[251:248] & {4{w_strb_in[31]}};
            w_data_061      = w_data_in[247:244] & {4{w_strb_in[30]}};
            w_data_060      = w_data_in[243:240] & {4{w_strb_in[30]}};
            w_data_059      = w_data_in[239:236] & {4{w_strb_in[29]}};
            w_data_058      = w_data_in[235:232] & {4{w_strb_in[29]}};
            w_data_057      = w_data_in[231:228] & {4{w_strb_in[28]}};
            w_data_056      = w_data_in[227:224] & {4{w_strb_in[28]}};
            w_data_055      = w_data_in[223:220] & {4{w_strb_in[27]}};
            w_data_054      = w_data_in[219:216] & {4{w_strb_in[27]}};
            w_data_053      = w_data_in[215:212] & {4{w_strb_in[26]}};
            w_data_052      = w_data_in[211:208] & {4{w_strb_in[26]}};
            w_data_051      = w_data_in[207:204] & {4{w_strb_in[25]}};
            w_data_050      = w_data_in[203:200] & {4{w_strb_in[25]}};
            w_data_049      = w_data_in[199:196] & {4{w_strb_in[24]}};
            w_data_048      = w_data_in[195:192] & {4{w_strb_in[24]}};
            w_data_047      = w_data_in[191:188] & {4{w_strb_in[23]}};
            w_data_046      = w_data_in[187:184] & {4{w_strb_in[23]}};
            w_data_045      = w_data_in[183:180] & {4{w_strb_in[22]}};
            w_data_044      = w_data_in[179:176] & {4{w_strb_in[22]}};
            w_data_043      = w_data_in[175:172] & {4{w_strb_in[21]}};
            w_data_042      = w_data_in[171:168] & {4{w_strb_in[21]}};
            w_data_041      = w_data_in[167:164] & {4{w_strb_in[20]}};
            w_data_040      = w_data_in[163:160] & {4{w_strb_in[20]}};
            w_data_039      = w_data_in[159:156] & {4{w_strb_in[19]}};
            w_data_038      = w_data_in[155:152] & {4{w_strb_in[19]}};
            w_data_037      = w_data_in[151:148] & {4{w_strb_in[18]}};
            w_data_036      = w_data_in[147:144] & {4{w_strb_in[18]}};
            w_data_035      = w_data_in[143:140] & {4{w_strb_in[17]}};
            w_data_034      = w_data_in[139:136] & {4{w_strb_in[17]}};
            w_data_033      = w_data_in[135:132] & {4{w_strb_in[16]}};
            w_data_032      = w_data_in[131:128] & {4{w_strb_in[16]}};
            w_data_031      = w_data_in[127:124] & {4{w_strb_in[15]}};
            w_data_030      = w_data_in[123:120] & {4{w_strb_in[15]}};
            w_data_029      = w_data_in[119:116] & {4{w_strb_in[14]}};
            w_data_028      = w_data_in[115:112] & {4{w_strb_in[14]}};
            w_data_027      = w_data_in[111:108] & {4{w_strb_in[13]}};
            w_data_026      = w_data_in[107:104] & {4{w_strb_in[13]}};
            w_data_025      = w_data_in[103:100] & {4{w_strb_in[12]}};
            w_data_024      = w_data_in[ 99: 96] & {4{w_strb_in[12]}};
            w_data_023      = w_data_in[ 95: 92] & {4{w_strb_in[11]}};
            w_data_022      = w_data_in[ 91: 88] & {4{w_strb_in[11]}};
            w_data_021      = w_data_in[ 87: 84] & {4{w_strb_in[10]}};
            w_data_020      = w_data_in[ 83: 80] & {4{w_strb_in[10]}};
            w_data_019      = w_data_in[ 79: 76] & {4{w_strb_in[ 9]}};
            w_data_018      = w_data_in[ 75: 72] & {4{w_strb_in[ 9]}};
            w_data_017      = w_data_in[ 71: 68] & {4{w_strb_in[ 8]}};
            w_data_016      = w_data_in[ 67: 64] & {4{w_strb_in[ 8]}};
            w_data_015      = w_data_in[ 63: 60] & {4{w_strb_in[ 7]}};
            w_data_014      = w_data_in[ 59: 56] & {4{w_strb_in[ 7]}};
            w_data_013      = w_data_in[ 55: 52] & {4{w_strb_in[ 6]}};
            w_data_012      = w_data_in[ 51: 48] & {4{w_strb_in[ 6]}};
            w_data_011      = w_data_in[ 47: 44] & {4{w_strb_in[ 5]}};
            w_data_010      = w_data_in[ 43: 40] & {4{w_strb_in[ 5]}};
            w_data_009      = w_data_in[ 39: 36] & {4{w_strb_in[ 4]}};
            w_data_008      = w_data_in[ 35: 32] & {4{w_strb_in[ 4]}};
            w_data_007      = w_data_in[ 31: 28] & {4{w_strb_in[ 3]}};
            w_data_006      = w_data_in[ 27: 24] & {4{w_strb_in[ 3]}};
            w_data_005      = w_data_in[ 23: 20] & {4{w_strb_in[ 2]}};
            w_data_004      = w_data_in[ 19: 16] & {4{w_strb_in[ 2]}};
            w_data_003      = w_data_in[ 15: 12] & {4{w_strb_in[ 1]}};
            w_data_002      = w_data_in[ 11:  8] & {4{w_strb_in[ 1]}};
            w_data_001      = w_data_in[  7:  4] & {4{w_strb_in[ 0]}};
            w_data_000      = w_data_in[  3:  0] & {4{w_strb_in[ 0]}};
            w_data_edge_063 = { w_data_063[0:0], w_data_062[3:3] };
            w_data_edge_062 = { w_data_062[0:0], w_data_061[3:3] };
            w_data_edge_061 = { w_data_061[0:0], w_data_060[3:3] };
            w_data_edge_060 = { w_data_060[0:0], w_data_059[3:3] };
            w_data_edge_059 = { w_data_059[0:0], w_data_058[3:3] };
            w_data_edge_058 = { w_data_058[0:0], w_data_057[3:3] };
            w_data_edge_057 = { w_data_057[0:0], w_data_056[3:3] };
            w_data_edge_056 = { w_data_056[0:0], w_data_055[3:3] };
            w_data_edge_055 = { w_data_055[0:0], w_data_054[3:3] };
            w_data_edge_054 = { w_data_054[0:0], w_data_053[3:3] };
            w_data_edge_053 = { w_data_053[0:0], w_data_052[3:3] };
            w_data_edge_052 = { w_data_052[0:0], w_data_051[3:3] };
            w_data_edge_051 = { w_data_051[0:0], w_data_050[3:3] };
            w_data_edge_050 = { w_data_050[0:0], w_data_049[3:3] };
            w_data_edge_049 = { w_data_049[0:0], w_data_048[3:3] };
            w_data_edge_048 = { w_data_048[0:0], w_data_047[3:3] };
            w_data_edge_047 = { w_data_047[0:0], w_data_046[3:3] };
            w_data_edge_046 = { w_data_046[0:0], w_data_045[3:3] };
            w_data_edge_045 = { w_data_045[0:0], w_data_044[3:3] };
            w_data_edge_044 = { w_data_044[0:0], w_data_043[3:3] };
            w_data_edge_043 = { w_data_043[0:0], w_data_042[3:3] };
            w_data_edge_042 = { w_data_042[0:0], w_data_041[3:3] };
            w_data_edge_041 = { w_data_041[0:0], w_data_040[3:3] };
            w_data_edge_040 = { w_data_040[0:0], w_data_039[3:3] };
            w_data_edge_039 = { w_data_039[0:0], w_data_038[3:3] };
            w_data_edge_038 = { w_data_038[0:0], w_data_037[3:3] };
            w_data_edge_037 = { w_data_037[0:0], w_data_036[3:3] };
            w_data_edge_036 = { w_data_036[0:0], w_data_035[3:3] };
            w_data_edge_035 = { w_data_035[0:0], w_data_034[3:3] };
            w_data_edge_034 = { w_data_034[0:0], w_data_033[3:3] };
            w_data_edge_033 = { w_data_033[0:0], w_data_032[3:3] };
            w_data_edge_032 = { w_data_032[0:0], w_data_031[3:3] };
            w_data_edge_031 = { w_data_031[0:0], w_data_030[3:3] };
            w_data_edge_030 = { w_data_030[0:0], w_data_029[3:3] };
            w_data_edge_029 = { w_data_029[0:0], w_data_028[3:3] };
            w_data_edge_028 = { w_data_028[0:0], w_data_027[3:3] };
            w_data_edge_027 = { w_data_027[0:0], w_data_026[3:3] };
            w_data_edge_026 = { w_data_026[0:0], w_data_025[3:3] };
            w_data_edge_025 = { w_data_025[0:0], w_data_024[3:3] };
            w_data_edge_024 = { w_data_024[0:0], w_data_023[3:3] };
            w_data_edge_023 = { w_data_023[0:0], w_data_022[3:3] };
            w_data_edge_022 = { w_data_022[0:0], w_data_021[3:3] };
            w_data_edge_021 = { w_data_021[0:0], w_data_020[3:3] };
            w_data_edge_020 = { w_data_020[0:0], w_data_019[3:3] };
            w_data_edge_019 = { w_data_019[0:0], w_data_018[3:3] };
            w_data_edge_018 = { w_data_018[0:0], w_data_017[3:3] };
            w_data_edge_017 = { w_data_017[0:0], w_data_016[3:3] };
            w_data_edge_016 = { w_data_016[0:0], w_data_015[3:3] };
            w_data_edge_015 = { w_data_015[0:0], w_data_014[3:3] };
            w_data_edge_014 = { w_data_014[0:0], w_data_013[3:3] };
            w_data_edge_013 = { w_data_013[0:0], w_data_012[3:3] };
            w_data_edge_012 = { w_data_012[0:0], w_data_011[3:3] };
            w_data_edge_011 = { w_data_011[0:0], w_data_010[3:3] };
            w_data_edge_010 = { w_data_010[0:0], w_data_009[3:3] };
            w_data_edge_009 = { w_data_009[0:0], w_data_008[3:3] };
            w_data_edge_008 = { w_data_008[0:0], w_data_007[3:3] };
            w_data_edge_007 = { w_data_007[0:0], w_data_006[3:3] };
            w_data_edge_006 = { w_data_006[0:0], w_data_005[3:3] };
            w_data_edge_005 = { w_data_005[0:0], w_data_004[3:3] };
            w_data_edge_004 = { w_data_004[0:0], w_data_003[3:3] };
            w_data_edge_003 = { w_data_003[0:0], w_data_002[3:3] };
            w_data_edge_002 = { w_data_002[0:0], w_data_001[3:3] };
            w_data_edge_001 = { w_data_001[0:0], w_data_000[3:3] };
         end
         case (w_strb_in)
            32'h00000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b1 };

            32'h00000001,
            32'h00010000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h00,       1'b0 };

            32'h00000002,
            32'h00020000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h01,       1'b0 };

            32'h00000004,
            32'h00040000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h02,       1'b0 };

            32'h00000008,
            32'h00080000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h03,       1'b0 };

            32'h00000010,
            32'h00100000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h04,       1'b0 };

            32'h00000020,
            32'h00200000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h05,       1'b0 };

            32'h00000040,
            32'h00400000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h06,       1'b0 };

            32'h00000080,
            32'h00800000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h07,       1'b0 };

            32'h00000100,
            32'h01000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h08,       1'b0 };

            32'h00000200,
            32'h02000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h09,       1'b0 };

            32'h00000400,
            32'h00400000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0a,       1'b0 };

            32'h00000800,
            32'h08000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0b,       1'b0 };

            32'h00001000,
            32'h10000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0c,       1'b0 };

            32'h00002000,
            32'h20000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0d,       1'b0 };

            32'h00004000,
            32'h40000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0e,       1'b0 };

            32'h00008000,
            32'h80000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0f,       1'b0 };

            32'h00000003,
            32'h00030000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h0,         5'h1f,       1'b0 };

            32'h0000000c,
            32'h000c0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h1,         5'h1f,       1'b0 };

            32'h00000030,
            32'h00300000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h2,         5'h1f,       1'b0 };

            32'h000000c0,
            32'h00c00000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h3,         5'h1f,       1'b0 };

            32'h00000300,
            32'h03000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h4,         5'h1f,       1'b0 };

            32'h00000c00,
            32'h0c000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h5,         5'h1f,       1'b0 };

            32'h00003000,
            32'h30000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h6,         5'h1f,       1'b0 };

            32'h0000c000,
            32'hc0000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h7,         5'h1f,       1'b0 };

            32'h0000000f: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h0,        4'hf,         5'h1f,       1'b0 };

            32'h000000f0: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h1,        4'hf,         5'h1f,       1'b0 };

            32'h00000f00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h2,        4'hf,         5'h1f,       1'b0 };

            32'h0000f000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h3,        4'hf,         5'h1f,       1'b0 };

            32'h000f0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h4,        4'hf,         5'h1f,       1'b0 };

            32'h00f00000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h5,        4'hf,         5'h1f,       1'b0 };

            32'h0f000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h6,        4'hf,         5'h1f,       1'b0 };

            32'hf0000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'hf,         4'h7,        4'hf,         5'h1f,       1'b0 };

            32'h000000ff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'h0,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'h0000ff00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'h1,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'h00ff0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'h2,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'hff000000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b111,       4'h3,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'h0000ffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b000,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'hffff0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b00,       2'b11,        3'b001,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            32'hffffffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b01,       2'b00,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            default:      { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                          { 2'b10,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };
         endcase
      end
      else if (DATA_WIDTH >= 16) begin
         if (m_cover_data == 1'b1) begin
            w_data_031      = w_data_in[127:124] & {4{w_strb_in[15]}};
            w_data_030      = w_data_in[123:120] & {4{w_strb_in[15]}};
            w_data_029      = w_data_in[119:116] & {4{w_strb_in[14]}};
            w_data_028      = w_data_in[115:112] & {4{w_strb_in[14]}};
            w_data_027      = w_data_in[111:108] & {4{w_strb_in[13]}};
            w_data_026      = w_data_in[107:104] & {4{w_strb_in[13]}};
            w_data_025      = w_data_in[103:100] & {4{w_strb_in[12]}};
            w_data_024      = w_data_in[ 99: 96] & {4{w_strb_in[12]}};
            w_data_023      = w_data_in[ 95: 92] & {4{w_strb_in[11]}};
            w_data_022      = w_data_in[ 91: 88] & {4{w_strb_in[11]}};
            w_data_021      = w_data_in[ 87: 84] & {4{w_strb_in[10]}};
            w_data_020      = w_data_in[ 83: 80] & {4{w_strb_in[10]}};
            w_data_019      = w_data_in[ 79: 76] & {4{w_strb_in[ 9]}};
            w_data_018      = w_data_in[ 75: 72] & {4{w_strb_in[ 9]}};
            w_data_017      = w_data_in[ 71: 68] & {4{w_strb_in[ 8]}};
            w_data_016      = w_data_in[ 67: 64] & {4{w_strb_in[ 8]}};
            w_data_015      = w_data_in[ 63: 60] & {4{w_strb_in[ 7]}};
            w_data_014      = w_data_in[ 59: 56] & {4{w_strb_in[ 7]}};
            w_data_013      = w_data_in[ 55: 52] & {4{w_strb_in[ 6]}};
            w_data_012      = w_data_in[ 51: 48] & {4{w_strb_in[ 6]}};
            w_data_011      = w_data_in[ 47: 44] & {4{w_strb_in[ 5]}};
            w_data_010      = w_data_in[ 43: 40] & {4{w_strb_in[ 5]}};
            w_data_009      = w_data_in[ 39: 36] & {4{w_strb_in[ 4]}};
            w_data_008      = w_data_in[ 35: 32] & {4{w_strb_in[ 4]}};
            w_data_007      = w_data_in[ 31: 28] & {4{w_strb_in[ 3]}};
            w_data_006      = w_data_in[ 27: 24] & {4{w_strb_in[ 3]}};
            w_data_005      = w_data_in[ 23: 20] & {4{w_strb_in[ 2]}};
            w_data_004      = w_data_in[ 19: 16] & {4{w_strb_in[ 2]}};
            w_data_003      = w_data_in[ 15: 12] & {4{w_strb_in[ 1]}};
            w_data_002      = w_data_in[ 11:  8] & {4{w_strb_in[ 1]}};
            w_data_001      = w_data_in[  7:  4] & {4{w_strb_in[ 0]}};
            w_data_000      = w_data_in[  3:  0] & {4{w_strb_in[ 0]}};
            w_data_edge_031 = { w_data_031[0:0], w_data_030[3:3] };
            w_data_edge_030 = { w_data_030[0:0], w_data_029[3:3] };
            w_data_edge_029 = { w_data_029[0:0], w_data_028[3:3] };
            w_data_edge_028 = { w_data_028[0:0], w_data_027[3:3] };
            w_data_edge_027 = { w_data_027[0:0], w_data_026[3:3] };
            w_data_edge_026 = { w_data_026[0:0], w_data_025[3:3] };
            w_data_edge_025 = { w_data_025[0:0], w_data_024[3:3] };
            w_data_edge_024 = { w_data_024[0:0], w_data_023[3:3] };
            w_data_edge_023 = { w_data_023[0:0], w_data_022[3:3] };
            w_data_edge_022 = { w_data_022[0:0], w_data_021[3:3] };
            w_data_edge_021 = { w_data_021[0:0], w_data_020[3:3] };
            w_data_edge_020 = { w_data_020[0:0], w_data_019[3:3] };
            w_data_edge_019 = { w_data_019[0:0], w_data_018[3:3] };
            w_data_edge_018 = { w_data_018[0:0], w_data_017[3:3] };
            w_data_edge_017 = { w_data_017[0:0], w_data_016[3:3] };
            w_data_edge_016 = { w_data_016[0:0], w_data_015[3:3] };
            w_data_edge_015 = { w_data_015[0:0], w_data_014[3:3] };
            w_data_edge_014 = { w_data_014[0:0], w_data_013[3:3] };
            w_data_edge_013 = { w_data_013[0:0], w_data_012[3:3] };
            w_data_edge_012 = { w_data_012[0:0], w_data_011[3:3] };
            w_data_edge_011 = { w_data_011[0:0], w_data_010[3:3] };
            w_data_edge_010 = { w_data_010[0:0], w_data_009[3:3] };
            w_data_edge_009 = { w_data_009[0:0], w_data_008[3:3] };
            w_data_edge_008 = { w_data_008[0:0], w_data_007[3:3] };
            w_data_edge_007 = { w_data_007[0:0], w_data_006[3:3] };
            w_data_edge_006 = { w_data_006[0:0], w_data_005[3:3] };
            w_data_edge_005 = { w_data_005[0:0], w_data_004[3:3] };
            w_data_edge_004 = { w_data_004[0:0], w_data_003[3:3] };
            w_data_edge_003 = { w_data_003[0:0], w_data_002[3:3] };
            w_data_edge_002 = { w_data_002[0:0], w_data_001[3:3] };
            w_data_edge_001 = { w_data_001[0:0], w_data_000[3:3] };
         end
         case (w_strb_in)
            16'h0000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b1 };

            16'h0001: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h00,       1'b0 };

            16'h0002: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h01,       1'b0 };

            16'h0004: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h02,       1'b0 };

            16'h0008: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h03,       1'b0 };

            16'h0010: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h04,       1'b0 };

            16'h0020: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h05,       1'b0 };

            16'h0040: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h06,       1'b0 };

            16'h0080: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h07,       1'b0 };

            16'h0100: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h08,       1'b0 };

            16'h0200: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h09,       1'b0 };

            16'h0400: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0a,       1'b0 };

            16'h0800: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0b,       1'b0 };

            16'h1000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0c,       1'b0 };

            16'h2000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0d,       1'b0 };

            16'h4000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0e,       1'b0 };

            16'h8000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h0f,       1'b0 };

            16'h0003: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h0,         5'h1f,       1'b0 };

            16'h000c: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h1,         5'h1f,       1'b0 };

            16'h0030: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h2,         5'h1f,       1'b0 };

            16'h00c0: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h3,         5'h1f,       1'b0 };

            16'h0300: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h4,         5'h1f,       1'b0 };

            16'h0c00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h5,         5'h1f,       1'b0 };

            16'h3000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h6,         5'h1f,       1'b0 };

            16'hc000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h7,         5'h1f,       1'b0 };

            16'h000f: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'h0,        4'hf,         5'h1f,       1'b0 };

            16'h00f0: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'h1,        4'hf,         5'h1f,       1'b0 };

            16'h0f00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'h2,        4'hf,         5'h1f,       1'b0 };

            16'hf000: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'hf,         4'h3,        4'hf,         5'h1f,       1'b0 };

            16'h00ff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'h0,         4'hf,        4'hf,         5'h1f,       1'b0 };

            16'hff00: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b00,       2'b11,        3'b111,       4'h1,         4'hf,        4'hf,         5'h1f,       1'b0 };

            16'hffff: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b01,       2'b11,        3'b000,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };

            default:  { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                      { 2'b10,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };
         endcase
      end
      else if (DATA_WIDTH >= 8) begin
         if (m_cover_data == 1'b1) begin
            w_data_015      = w_data_in[63:60] & {4{w_strb_in[ 7]}};
            w_data_014      = w_data_in[59:56] & {4{w_strb_in[ 7]}};
            w_data_013      = w_data_in[55:52] & {4{w_strb_in[ 6]}};
            w_data_012      = w_data_in[51:48] & {4{w_strb_in[ 6]}};
            w_data_011      = w_data_in[47:44] & {4{w_strb_in[ 5]}};
            w_data_010      = w_data_in[43:40] & {4{w_strb_in[ 5]}};
            w_data_009      = w_data_in[39:36] & {4{w_strb_in[ 4]}};
            w_data_008      = w_data_in[35:32] & {4{w_strb_in[ 4]}};
            w_data_007      = w_data_in[31:28] & {4{w_strb_in[ 3]}};
            w_data_006      = w_data_in[27:24] & {4{w_strb_in[ 3]}};
            w_data_005      = w_data_in[23:20] & {4{w_strb_in[ 2]}};
            w_data_004      = w_data_in[19:16] & {4{w_strb_in[ 2]}};
            w_data_003      = w_data_in[15:12] & {4{w_strb_in[ 1]}};
            w_data_002      = w_data_in[11: 8] & {4{w_strb_in[ 1]}};
            w_data_001      = w_data_in[ 7: 4] & {4{w_strb_in[ 0]}};
            w_data_000      = w_data_in[ 3: 0] & {4{w_strb_in[ 0]}};
            w_data_edge_015 = { w_data_015[0:0], w_data_014[3:3] };
            w_data_edge_014 = { w_data_014[0:0], w_data_013[3:3] };
            w_data_edge_013 = { w_data_013[0:0], w_data_012[3:3] };
            w_data_edge_012 = { w_data_012[0:0], w_data_011[3:3] };
            w_data_edge_011 = { w_data_011[0:0], w_data_010[3:3] };
            w_data_edge_010 = { w_data_010[0:0], w_data_009[3:3] };
            w_data_edge_009 = { w_data_009[0:0], w_data_008[3:3] };
            w_data_edge_008 = { w_data_008[0:0], w_data_007[3:3] };
            w_data_edge_007 = { w_data_007[0:0], w_data_006[3:3] };
            w_data_edge_006 = { w_data_006[0:0], w_data_005[3:3] };
            w_data_edge_005 = { w_data_005[0:0], w_data_004[3:3] };
            w_data_edge_004 = { w_data_004[0:0], w_data_003[3:3] };
            w_data_edge_003 = { w_data_003[0:0], w_data_002[3:3] };
            w_data_edge_002 = { w_data_002[0:0], w_data_001[3:3] };
            w_data_edge_001 = { w_data_001[0:0], w_data_000[3:3] };
         end
         case (w_strb_in)
            8'h00:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b1 };

            8'h01:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h00,       1'b0 };

            8'h02:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h01,       1'b0 };

            8'h04:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h02,       1'b0 };

            8'h08:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h03,       1'b0 };

            8'h10:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h04,       1'b0 };

            8'h20:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h05,       1'b0 };

            8'h40:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h06,       1'b0 };

            8'h80:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h07,       1'b0 };

            8'h03:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h0,         5'h1f,       1'b0 };

            8'h0c:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h1,         5'h1f,       1'b0 };

            8'h30:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h2,         5'h1f,       1'b0 };

            8'hc0:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'hf,        4'h3,         5'h1f,       1'b0 };

            8'h0f:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'h0,        4'hf,         5'h1f,       1'b0 };

            8'hf0:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b00,       2'b11,        3'b111,       4'hf,         4'h1,        4'hf,         5'h1f,       1'b0 };

            8'hff:   { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b01,       2'b11,        3'b111,       4'h0,         4'hf,        4'hf,         5'h1f,       1'b0 };

            default: { w_strb_full, w_strb_oword, w_strb_qword, w_strb_dword, w_strb_word, w_strb_hword, w_strb_byte, w_strb_none } =
                     { 2'b10,       2'b11,        3'b111,       4'hf,         4'hf,        4'hf,         5'h1f,       1'b0 };
         endcase
      end
      cg_axi_w_req.sample();
      if (m_cover_data == 1'b1) begin
         cg_axi_w_data.sample();
      end
   endfunction : sample_w_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r_local
   //
   function void sample_r_local(input bit [1:0]   r_resp_in,
                                input bit [511:0] r_data_in,
                                input int         r_idle_count_in,
                                input int         r_busy_count_in);
      r_resp       = r_resp_in;
      r_idle_count = r_idle_count_in;
      r_busy_count = r_busy_count_in;
      if ((m_cover_data == 1'b1) && (r_resp == 2'b00)) begin
         if (DATA_WIDTH >= 64) begin
            r_data_127      = r_data_in[511:508];
            r_data_126      = r_data_in[507:504];
            r_data_125      = r_data_in[503:500];
            r_data_124      = r_data_in[499:496];
            r_data_123      = r_data_in[495:492];
            r_data_122      = r_data_in[491:488];
            r_data_121      = r_data_in[487:484];
            r_data_120      = r_data_in[483:480];
            r_data_119      = r_data_in[479:476];
            r_data_118      = r_data_in[475:472];
            r_data_117      = r_data_in[471:468];
            r_data_116      = r_data_in[467:464];
            r_data_115      = r_data_in[463:460];
            r_data_114      = r_data_in[459:456];
            r_data_113      = r_data_in[455:452];
            r_data_112      = r_data_in[451:448];
            r_data_111      = r_data_in[447:444];
            r_data_110      = r_data_in[443:440];
            r_data_109      = r_data_in[439:436];
            r_data_108      = r_data_in[435:432];
            r_data_107      = r_data_in[431:428];
            r_data_106      = r_data_in[427:424];
            r_data_105      = r_data_in[423:420];
            r_data_104      = r_data_in[419:416];
            r_data_103      = r_data_in[415:412];
            r_data_102      = r_data_in[411:408];
            r_data_101      = r_data_in[407:404];
            r_data_100      = r_data_in[403:400];
            r_data_099      = r_data_in[399:396];
            r_data_098      = r_data_in[395:392];
            r_data_097      = r_data_in[391:388];
            r_data_096      = r_data_in[387:384];
            r_data_095      = r_data_in[383:380];
            r_data_094      = r_data_in[379:376];
            r_data_093      = r_data_in[375:372];
            r_data_092      = r_data_in[371:368];
            r_data_091      = r_data_in[367:364];
            r_data_090      = r_data_in[363:360];
            r_data_089      = r_data_in[359:356];
            r_data_088      = r_data_in[355:352];
            r_data_087      = r_data_in[351:348];
            r_data_086      = r_data_in[347:344];
            r_data_085      = r_data_in[343:340];
            r_data_084      = r_data_in[339:336];
            r_data_083      = r_data_in[335:332];
            r_data_082      = r_data_in[331:328];
            r_data_081      = r_data_in[327:324];
            r_data_080      = r_data_in[323:320];
            r_data_079      = r_data_in[319:316];
            r_data_078      = r_data_in[315:312];
            r_data_077      = r_data_in[311:308];
            r_data_076      = r_data_in[307:304];
            r_data_075      = r_data_in[303:300];
            r_data_074      = r_data_in[299:296];
            r_data_073      = r_data_in[295:292];
            r_data_072      = r_data_in[291:288];
            r_data_071      = r_data_in[287:284];
            r_data_070      = r_data_in[283:280];
            r_data_069      = r_data_in[279:276];
            r_data_068      = r_data_in[275:272];
            r_data_067      = r_data_in[271:268];
            r_data_066      = r_data_in[267:264];
            r_data_065      = r_data_in[263:260];
            r_data_064      = r_data_in[259:256];
            r_data_063      = r_data_in[255:252];
            r_data_062      = r_data_in[251:248];
            r_data_061      = r_data_in[247:244];
            r_data_060      = r_data_in[243:240];
            r_data_059      = r_data_in[239:236];
            r_data_058      = r_data_in[235:232];
            r_data_057      = r_data_in[231:228];
            r_data_056      = r_data_in[227:224];
            r_data_055      = r_data_in[223:220];
            r_data_054      = r_data_in[219:216];
            r_data_053      = r_data_in[215:212];
            r_data_052      = r_data_in[211:208];
            r_data_051      = r_data_in[207:204];
            r_data_050      = r_data_in[203:200];
            r_data_049      = r_data_in[199:196];
            r_data_048      = r_data_in[195:192];
            r_data_047      = r_data_in[191:188];
            r_data_046      = r_data_in[187:184];
            r_data_045      = r_data_in[183:180];
            r_data_044      = r_data_in[179:176];
            r_data_043      = r_data_in[175:172];
            r_data_042      = r_data_in[171:168];
            r_data_041      = r_data_in[167:164];
            r_data_040      = r_data_in[163:160];
            r_data_039      = r_data_in[159:156];
            r_data_038      = r_data_in[155:152];
            r_data_037      = r_data_in[151:148];
            r_data_036      = r_data_in[147:144];
            r_data_035      = r_data_in[143:140];
            r_data_034      = r_data_in[139:136];
            r_data_033      = r_data_in[135:132];
            r_data_032      = r_data_in[131:128];
            r_data_031      = r_data_in[127:124];
            r_data_030      = r_data_in[123:120];
            r_data_029      = r_data_in[119:116];
            r_data_028      = r_data_in[115:112];
            r_data_027      = r_data_in[111:108];
            r_data_026      = r_data_in[107:104];
            r_data_025      = r_data_in[103:100];
            r_data_024      = r_data_in[ 99: 96];
            r_data_023      = r_data_in[ 95: 92];
            r_data_022      = r_data_in[ 91: 88];
            r_data_021      = r_data_in[ 87: 84];
            r_data_020      = r_data_in[ 83: 80];
            r_data_019      = r_data_in[ 79: 76];
            r_data_018      = r_data_in[ 75: 72];
            r_data_017      = r_data_in[ 71: 68];
            r_data_016      = r_data_in[ 67: 64];
            r_data_015      = r_data_in[ 63: 60];
            r_data_014      = r_data_in[ 59: 56];
            r_data_013      = r_data_in[ 55: 52];
            r_data_012      = r_data_in[ 51: 48];
            r_data_011      = r_data_in[ 47: 44];
            r_data_010      = r_data_in[ 43: 40];
            r_data_009      = r_data_in[ 39: 36];
            r_data_008      = r_data_in[ 35: 32];
            r_data_007      = r_data_in[ 31: 28];
            r_data_006      = r_data_in[ 27: 24];
            r_data_005      = r_data_in[ 23: 20];
            r_data_004      = r_data_in[ 19: 16];
            r_data_003      = r_data_in[ 15: 12];
            r_data_002      = r_data_in[ 11:  8];
            r_data_001      = r_data_in[  7:  4];
            r_data_000      = r_data_in[  3:  0];
            r_data_edge_127 = { r_data_127[0:0], r_data_126[3:3] };
            r_data_edge_126 = { r_data_126[0:0], r_data_125[3:3] };
            r_data_edge_125 = { r_data_125[0:0], r_data_124[3:3] };
            r_data_edge_124 = { r_data_124[0:0], r_data_123[3:3] };
            r_data_edge_123 = { r_data_123[0:0], r_data_122[3:3] };
            r_data_edge_122 = { r_data_122[0:0], r_data_121[3:3] };
            r_data_edge_121 = { r_data_121[0:0], r_data_120[3:3] };
            r_data_edge_120 = { r_data_120[0:0], r_data_119[3:3] };
            r_data_edge_119 = { r_data_119[0:0], r_data_118[3:3] };
            r_data_edge_118 = { r_data_118[0:0], r_data_117[3:3] };
            r_data_edge_117 = { r_data_117[0:0], r_data_116[3:3] };
            r_data_edge_116 = { r_data_116[0:0], r_data_115[3:3] };
            r_data_edge_115 = { r_data_115[0:0], r_data_114[3:3] };
            r_data_edge_114 = { r_data_114[0:0], r_data_113[3:3] };
            r_data_edge_113 = { r_data_113[0:0], r_data_112[3:3] };
            r_data_edge_112 = { r_data_112[0:0], r_data_111[3:3] };
            r_data_edge_111 = { r_data_111[0:0], r_data_110[3:3] };
            r_data_edge_110 = { r_data_110[0:0], r_data_109[3:3] };
            r_data_edge_109 = { r_data_109[0:0], r_data_108[3:3] };
            r_data_edge_108 = { r_data_108[0:0], r_data_107[3:3] };
            r_data_edge_107 = { r_data_107[0:0], r_data_106[3:3] };
            r_data_edge_106 = { r_data_106[0:0], r_data_105[3:3] };
            r_data_edge_105 = { r_data_105[0:0], r_data_104[3:3] };
            r_data_edge_104 = { r_data_104[0:0], r_data_103[3:3] };
            r_data_edge_103 = { r_data_103[0:0], r_data_102[3:3] };
            r_data_edge_102 = { r_data_102[0:0], r_data_101[3:3] };
            r_data_edge_101 = { r_data_101[0:0], r_data_100[3:3] };
            r_data_edge_100 = { r_data_100[0:0], r_data_099[3:3] };
            r_data_edge_099 = { r_data_099[0:0], r_data_098[3:3] };
            r_data_edge_098 = { r_data_098[0:0], r_data_097[3:3] };
            r_data_edge_097 = { r_data_097[0:0], r_data_096[3:3] };
            r_data_edge_096 = { r_data_096[0:0], r_data_095[3:3] };
            r_data_edge_095 = { r_data_095[0:0], r_data_094[3:3] };
            r_data_edge_094 = { r_data_094[0:0], r_data_093[3:3] };
            r_data_edge_093 = { r_data_093[0:0], r_data_092[3:3] };
            r_data_edge_092 = { r_data_092[0:0], r_data_091[3:3] };
            r_data_edge_091 = { r_data_091[0:0], r_data_090[3:3] };
            r_data_edge_090 = { r_data_090[0:0], r_data_089[3:3] };
            r_data_edge_089 = { r_data_089[0:0], r_data_088[3:3] };
            r_data_edge_088 = { r_data_088[0:0], r_data_087[3:3] };
            r_data_edge_087 = { r_data_087[0:0], r_data_086[3:3] };
            r_data_edge_086 = { r_data_086[0:0], r_data_085[3:3] };
            r_data_edge_085 = { r_data_085[0:0], r_data_084[3:3] };
            r_data_edge_084 = { r_data_084[0:0], r_data_083[3:3] };
            r_data_edge_083 = { r_data_083[0:0], r_data_082[3:3] };
            r_data_edge_082 = { r_data_082[0:0], r_data_081[3:3] };
            r_data_edge_081 = { r_data_081[0:0], r_data_080[3:3] };
            r_data_edge_080 = { r_data_080[0:0], r_data_079[3:3] };
            r_data_edge_079 = { r_data_079[0:0], r_data_078[3:3] };
            r_data_edge_078 = { r_data_078[0:0], r_data_077[3:3] };
            r_data_edge_077 = { r_data_077[0:0], r_data_076[3:3] };
            r_data_edge_076 = { r_data_076[0:0], r_data_075[3:3] };
            r_data_edge_075 = { r_data_075[0:0], r_data_074[3:3] };
            r_data_edge_074 = { r_data_074[0:0], r_data_073[3:3] };
            r_data_edge_073 = { r_data_073[0:0], r_data_072[3:3] };
            r_data_edge_072 = { r_data_072[0:0], r_data_071[3:3] };
            r_data_edge_071 = { r_data_071[0:0], r_data_070[3:3] };
            r_data_edge_070 = { r_data_070[0:0], r_data_069[3:3] };
            r_data_edge_069 = { r_data_069[0:0], r_data_068[3:3] };
            r_data_edge_068 = { r_data_068[0:0], r_data_067[3:3] };
            r_data_edge_067 = { r_data_067[0:0], r_data_066[3:3] };
            r_data_edge_066 = { r_data_066[0:0], r_data_065[3:3] };
            r_data_edge_065 = { r_data_065[0:0], r_data_064[3:3] };
            r_data_edge_064 = { r_data_064[0:0], r_data_063[3:3] };
            r_data_edge_063 = { r_data_063[0:0], r_data_062[3:3] };
            r_data_edge_062 = { r_data_062[0:0], r_data_061[3:3] };
            r_data_edge_061 = { r_data_061[0:0], r_data_060[3:3] };
            r_data_edge_060 = { r_data_060[0:0], r_data_059[3:3] };
            r_data_edge_059 = { r_data_059[0:0], r_data_058[3:3] };
            r_data_edge_058 = { r_data_058[0:0], r_data_057[3:3] };
            r_data_edge_057 = { r_data_057[0:0], r_data_056[3:3] };
            r_data_edge_056 = { r_data_056[0:0], r_data_055[3:3] };
            r_data_edge_055 = { r_data_055[0:0], r_data_054[3:3] };
            r_data_edge_054 = { r_data_054[0:0], r_data_053[3:3] };
            r_data_edge_053 = { r_data_053[0:0], r_data_052[3:3] };
            r_data_edge_052 = { r_data_052[0:0], r_data_051[3:3] };
            r_data_edge_051 = { r_data_051[0:0], r_data_050[3:3] };
            r_data_edge_050 = { r_data_050[0:0], r_data_049[3:3] };
            r_data_edge_049 = { r_data_049[0:0], r_data_048[3:3] };
            r_data_edge_048 = { r_data_048[0:0], r_data_047[3:3] };
            r_data_edge_047 = { r_data_047[0:0], r_data_046[3:3] };
            r_data_edge_046 = { r_data_046[0:0], r_data_045[3:3] };
            r_data_edge_045 = { r_data_045[0:0], r_data_044[3:3] };
            r_data_edge_044 = { r_data_044[0:0], r_data_043[3:3] };
            r_data_edge_043 = { r_data_043[0:0], r_data_042[3:3] };
            r_data_edge_042 = { r_data_042[0:0], r_data_041[3:3] };
            r_data_edge_041 = { r_data_041[0:0], r_data_040[3:3] };
            r_data_edge_040 = { r_data_040[0:0], r_data_039[3:3] };
            r_data_edge_039 = { r_data_039[0:0], r_data_038[3:3] };
            r_data_edge_038 = { r_data_038[0:0], r_data_037[3:3] };
            r_data_edge_037 = { r_data_037[0:0], r_data_036[3:3] };
            r_data_edge_036 = { r_data_036[0:0], r_data_035[3:3] };
            r_data_edge_035 = { r_data_035[0:0], r_data_034[3:3] };
            r_data_edge_034 = { r_data_034[0:0], r_data_033[3:3] };
            r_data_edge_033 = { r_data_033[0:0], r_data_032[3:3] };
            r_data_edge_032 = { r_data_032[0:0], r_data_031[3:3] };
            r_data_edge_031 = { r_data_031[0:0], r_data_030[3:3] };
            r_data_edge_030 = { r_data_030[0:0], r_data_029[3:3] };
            r_data_edge_029 = { r_data_029[0:0], r_data_028[3:3] };
            r_data_edge_028 = { r_data_028[0:0], r_data_027[3:3] };
            r_data_edge_027 = { r_data_027[0:0], r_data_026[3:3] };
            r_data_edge_026 = { r_data_026[0:0], r_data_025[3:3] };
            r_data_edge_025 = { r_data_025[0:0], r_data_024[3:3] };
            r_data_edge_024 = { r_data_024[0:0], r_data_023[3:3] };
            r_data_edge_023 = { r_data_023[0:0], r_data_022[3:3] };
            r_data_edge_022 = { r_data_022[0:0], r_data_021[3:3] };
            r_data_edge_021 = { r_data_021[0:0], r_data_020[3:3] };
            r_data_edge_020 = { r_data_020[0:0], r_data_019[3:3] };
            r_data_edge_019 = { r_data_019[0:0], r_data_018[3:3] };
            r_data_edge_018 = { r_data_018[0:0], r_data_017[3:3] };
            r_data_edge_017 = { r_data_017[0:0], r_data_016[3:3] };
            r_data_edge_016 = { r_data_016[0:0], r_data_015[3:3] };
            r_data_edge_015 = { r_data_015[0:0], r_data_014[3:3] };
            r_data_edge_014 = { r_data_014[0:0], r_data_013[3:3] };
            r_data_edge_013 = { r_data_013[0:0], r_data_012[3:3] };
            r_data_edge_012 = { r_data_012[0:0], r_data_011[3:3] };
            r_data_edge_011 = { r_data_011[0:0], r_data_010[3:3] };
            r_data_edge_010 = { r_data_010[0:0], r_data_009[3:3] };
            r_data_edge_009 = { r_data_009[0:0], r_data_008[3:3] };
            r_data_edge_008 = { r_data_008[0:0], r_data_007[3:3] };
            r_data_edge_007 = { r_data_007[0:0], r_data_006[3:3] };
            r_data_edge_006 = { r_data_006[0:0], r_data_005[3:3] };
            r_data_edge_005 = { r_data_005[0:0], r_data_004[3:3] };
            r_data_edge_004 = { r_data_004[0:0], r_data_003[3:3] };
            r_data_edge_003 = { r_data_003[0:0], r_data_002[3:3] };
            r_data_edge_002 = { r_data_002[0:0], r_data_001[3:3] };
            r_data_edge_001 = { r_data_001[0:0], r_data_000[3:3] };
         end
         else if (DATA_WIDTH >= 32) begin
            r_data_063      = r_data_in[255:252];
            r_data_062      = r_data_in[251:248];
            r_data_061      = r_data_in[247:244];
            r_data_060      = r_data_in[243:240];
            r_data_059      = r_data_in[239:236];
            r_data_058      = r_data_in[235:232];
            r_data_057      = r_data_in[231:228];
            r_data_056      = r_data_in[227:224];
            r_data_055      = r_data_in[223:220];
            r_data_054      = r_data_in[219:216];
            r_data_053      = r_data_in[215:212];
            r_data_052      = r_data_in[211:208];
            r_data_051      = r_data_in[207:204];
            r_data_050      = r_data_in[203:200];
            r_data_049      = r_data_in[199:196];
            r_data_048      = r_data_in[195:192];
            r_data_047      = r_data_in[191:188];
            r_data_046      = r_data_in[187:184];
            r_data_045      = r_data_in[183:180];
            r_data_044      = r_data_in[179:176];
            r_data_043      = r_data_in[175:172];
            r_data_042      = r_data_in[171:168];
            r_data_041      = r_data_in[167:164];
            r_data_040      = r_data_in[163:160];
            r_data_039      = r_data_in[159:156];
            r_data_038      = r_data_in[155:152];
            r_data_037      = r_data_in[151:148];
            r_data_036      = r_data_in[147:144];
            r_data_035      = r_data_in[143:140];
            r_data_034      = r_data_in[139:136];
            r_data_033      = r_data_in[135:132];
            r_data_032      = r_data_in[131:128];
            r_data_031      = r_data_in[127:124];
            r_data_030      = r_data_in[123:120];
            r_data_029      = r_data_in[119:116];
            r_data_028      = r_data_in[115:112];
            r_data_027      = r_data_in[111:108];
            r_data_026      = r_data_in[107:104];
            r_data_025      = r_data_in[103:100];
            r_data_024      = r_data_in[ 99: 96];
            r_data_023      = r_data_in[ 95: 92];
            r_data_022      = r_data_in[ 91: 88];
            r_data_021      = r_data_in[ 87: 84];
            r_data_020      = r_data_in[ 83: 80];
            r_data_019      = r_data_in[ 79: 76];
            r_data_018      = r_data_in[ 75: 72];
            r_data_017      = r_data_in[ 71: 68];
            r_data_016      = r_data_in[ 67: 64];
            r_data_015      = r_data_in[ 63: 60];
            r_data_014      = r_data_in[ 59: 56];
            r_data_013      = r_data_in[ 55: 52];
            r_data_012      = r_data_in[ 51: 48];
            r_data_011      = r_data_in[ 47: 44];
            r_data_010      = r_data_in[ 43: 40];
            r_data_009      = r_data_in[ 39: 36];
            r_data_008      = r_data_in[ 35: 32];
            r_data_007      = r_data_in[ 31: 28];
            r_data_006      = r_data_in[ 27: 24];
            r_data_005      = r_data_in[ 23: 20];
            r_data_004      = r_data_in[ 19: 16];
            r_data_003      = r_data_in[ 15: 12];
            r_data_002      = r_data_in[ 11:  8];
            r_data_001      = r_data_in[  7:  4];
            r_data_000      = r_data_in[  3:  0];
            r_data_edge_063 = { r_data_063[0:0], r_data_062[3:3] };
            r_data_edge_062 = { r_data_062[0:0], r_data_061[3:3] };
            r_data_edge_061 = { r_data_061[0:0], r_data_060[3:3] };
            r_data_edge_060 = { r_data_060[0:0], r_data_059[3:3] };
            r_data_edge_059 = { r_data_059[0:0], r_data_058[3:3] };
            r_data_edge_058 = { r_data_058[0:0], r_data_057[3:3] };
            r_data_edge_057 = { r_data_057[0:0], r_data_056[3:3] };
            r_data_edge_056 = { r_data_056[0:0], r_data_055[3:3] };
            r_data_edge_055 = { r_data_055[0:0], r_data_054[3:3] };
            r_data_edge_054 = { r_data_054[0:0], r_data_053[3:3] };
            r_data_edge_053 = { r_data_053[0:0], r_data_052[3:3] };
            r_data_edge_052 = { r_data_052[0:0], r_data_051[3:3] };
            r_data_edge_051 = { r_data_051[0:0], r_data_050[3:3] };
            r_data_edge_050 = { r_data_050[0:0], r_data_049[3:3] };
            r_data_edge_049 = { r_data_049[0:0], r_data_048[3:3] };
            r_data_edge_048 = { r_data_048[0:0], r_data_047[3:3] };
            r_data_edge_047 = { r_data_047[0:0], r_data_046[3:3] };
            r_data_edge_046 = { r_data_046[0:0], r_data_045[3:3] };
            r_data_edge_045 = { r_data_045[0:0], r_data_044[3:3] };
            r_data_edge_044 = { r_data_044[0:0], r_data_043[3:3] };
            r_data_edge_043 = { r_data_043[0:0], r_data_042[3:3] };
            r_data_edge_042 = { r_data_042[0:0], r_data_041[3:3] };
            r_data_edge_041 = { r_data_041[0:0], r_data_040[3:3] };
            r_data_edge_040 = { r_data_040[0:0], r_data_039[3:3] };
            r_data_edge_039 = { r_data_039[0:0], r_data_038[3:3] };
            r_data_edge_038 = { r_data_038[0:0], r_data_037[3:3] };
            r_data_edge_037 = { r_data_037[0:0], r_data_036[3:3] };
            r_data_edge_036 = { r_data_036[0:0], r_data_035[3:3] };
            r_data_edge_035 = { r_data_035[0:0], r_data_034[3:3] };
            r_data_edge_034 = { r_data_034[0:0], r_data_033[3:3] };
            r_data_edge_033 = { r_data_033[0:0], r_data_032[3:3] };
            r_data_edge_032 = { r_data_032[0:0], r_data_031[3:3] };
            r_data_edge_031 = { r_data_031[0:0], r_data_030[3:3] };
            r_data_edge_030 = { r_data_030[0:0], r_data_029[3:3] };
            r_data_edge_029 = { r_data_029[0:0], r_data_028[3:3] };
            r_data_edge_028 = { r_data_028[0:0], r_data_027[3:3] };
            r_data_edge_027 = { r_data_027[0:0], r_data_026[3:3] };
            r_data_edge_026 = { r_data_026[0:0], r_data_025[3:3] };
            r_data_edge_025 = { r_data_025[0:0], r_data_024[3:3] };
            r_data_edge_024 = { r_data_024[0:0], r_data_023[3:3] };
            r_data_edge_023 = { r_data_023[0:0], r_data_022[3:3] };
            r_data_edge_022 = { r_data_022[0:0], r_data_021[3:3] };
            r_data_edge_021 = { r_data_021[0:0], r_data_020[3:3] };
            r_data_edge_020 = { r_data_020[0:0], r_data_019[3:3] };
            r_data_edge_019 = { r_data_019[0:0], r_data_018[3:3] };
            r_data_edge_018 = { r_data_018[0:0], r_data_017[3:3] };
            r_data_edge_017 = { r_data_017[0:0], r_data_016[3:3] };
            r_data_edge_016 = { r_data_016[0:0], r_data_015[3:3] };
            r_data_edge_015 = { r_data_015[0:0], r_data_014[3:3] };
            r_data_edge_014 = { r_data_014[0:0], r_data_013[3:3] };
            r_data_edge_013 = { r_data_013[0:0], r_data_012[3:3] };
            r_data_edge_012 = { r_data_012[0:0], r_data_011[3:3] };
            r_data_edge_011 = { r_data_011[0:0], r_data_010[3:3] };
            r_data_edge_010 = { r_data_010[0:0], r_data_009[3:3] };
            r_data_edge_009 = { r_data_009[0:0], r_data_008[3:3] };
            r_data_edge_008 = { r_data_008[0:0], r_data_007[3:3] };
            r_data_edge_007 = { r_data_007[0:0], r_data_006[3:3] };
            r_data_edge_006 = { r_data_006[0:0], r_data_005[3:3] };
            r_data_edge_005 = { r_data_005[0:0], r_data_004[3:3] };
            r_data_edge_004 = { r_data_004[0:0], r_data_003[3:3] };
            r_data_edge_003 = { r_data_003[0:0], r_data_002[3:3] };
            r_data_edge_002 = { r_data_002[0:0], r_data_001[3:3] };
            r_data_edge_001 = { r_data_001[0:0], r_data_000[3:3] };
         end
         else if (DATA_WIDTH >= 16) begin
            r_data_031      = r_data_in[127:124];
            r_data_030      = r_data_in[123:120];
            r_data_029      = r_data_in[119:116];
            r_data_028      = r_data_in[115:112];
            r_data_027      = r_data_in[111:108];
            r_data_026      = r_data_in[107:104];
            r_data_025      = r_data_in[103:100];
            r_data_024      = r_data_in[ 99: 96];
            r_data_023      = r_data_in[ 95: 92];
            r_data_022      = r_data_in[ 91: 88];
            r_data_021      = r_data_in[ 87: 84];
            r_data_020      = r_data_in[ 83: 80];
            r_data_019      = r_data_in[ 79: 76];
            r_data_018      = r_data_in[ 75: 72];
            r_data_017      = r_data_in[ 71: 68];
            r_data_016      = r_data_in[ 67: 64];
            r_data_015      = r_data_in[ 63: 60];
            r_data_014      = r_data_in[ 59: 56];
            r_data_013      = r_data_in[ 55: 52];
            r_data_012      = r_data_in[ 51: 48];
            r_data_011      = r_data_in[ 47: 44];
            r_data_010      = r_data_in[ 43: 40];
            r_data_009      = r_data_in[ 39: 36];
            r_data_008      = r_data_in[ 35: 32];
            r_data_007      = r_data_in[ 31: 28];
            r_data_006      = r_data_in[ 27: 24];
            r_data_005      = r_data_in[ 23: 20];
            r_data_004      = r_data_in[ 19: 16];
            r_data_003      = r_data_in[ 15: 12];
            r_data_002      = r_data_in[ 11:  8];
            r_data_001      = r_data_in[  7:  4];
            r_data_000      = r_data_in[  3:  0];
            r_data_edge_031 = { r_data_031[0:0], r_data_030[3:3] };
            r_data_edge_030 = { r_data_030[0:0], r_data_029[3:3] };
            r_data_edge_029 = { r_data_029[0:0], r_data_028[3:3] };
            r_data_edge_028 = { r_data_028[0:0], r_data_027[3:3] };
            r_data_edge_027 = { r_data_027[0:0], r_data_026[3:3] };
            r_data_edge_026 = { r_data_026[0:0], r_data_025[3:3] };
            r_data_edge_025 = { r_data_025[0:0], r_data_024[3:3] };
            r_data_edge_024 = { r_data_024[0:0], r_data_023[3:3] };
            r_data_edge_023 = { r_data_023[0:0], r_data_022[3:3] };
            r_data_edge_022 = { r_data_022[0:0], r_data_021[3:3] };
            r_data_edge_021 = { r_data_021[0:0], r_data_020[3:3] };
            r_data_edge_020 = { r_data_020[0:0], r_data_019[3:3] };
            r_data_edge_019 = { r_data_019[0:0], r_data_018[3:3] };
            r_data_edge_018 = { r_data_018[0:0], r_data_017[3:3] };
            r_data_edge_017 = { r_data_017[0:0], r_data_016[3:3] };
            r_data_edge_016 = { r_data_016[0:0], r_data_015[3:3] };
            r_data_edge_015 = { r_data_015[0:0], r_data_014[3:3] };
            r_data_edge_014 = { r_data_014[0:0], r_data_013[3:3] };
            r_data_edge_013 = { r_data_013[0:0], r_data_012[3:3] };
            r_data_edge_012 = { r_data_012[0:0], r_data_011[3:3] };
            r_data_edge_011 = { r_data_011[0:0], r_data_010[3:3] };
            r_data_edge_010 = { r_data_010[0:0], r_data_009[3:3] };
            r_data_edge_009 = { r_data_009[0:0], r_data_008[3:3] };
            r_data_edge_008 = { r_data_008[0:0], r_data_007[3:3] };
            r_data_edge_007 = { r_data_007[0:0], r_data_006[3:3] };
            r_data_edge_006 = { r_data_006[0:0], r_data_005[3:3] };
            r_data_edge_005 = { r_data_005[0:0], r_data_004[3:3] };
            r_data_edge_004 = { r_data_004[0:0], r_data_003[3:3] };
            r_data_edge_003 = { r_data_003[0:0], r_data_002[3:3] };
            r_data_edge_002 = { r_data_002[0:0], r_data_001[3:3] };
            r_data_edge_001 = { r_data_001[0:0], r_data_000[3:3] };
         end
         else if (DATA_WIDTH >= 8) begin
            r_data_015      = r_data_in[63:60];
            r_data_014      = r_data_in[59:56];
            r_data_013      = r_data_in[55:52];
            r_data_012      = r_data_in[51:48];
            r_data_011      = r_data_in[47:44];
            r_data_010      = r_data_in[43:40];
            r_data_009      = r_data_in[39:36];
            r_data_008      = r_data_in[35:32];
            r_data_007      = r_data_in[31:28];
            r_data_006      = r_data_in[27:24];
            r_data_005      = r_data_in[23:20];
            r_data_004      = r_data_in[19:16];
            r_data_003      = r_data_in[15:12];
            r_data_002      = r_data_in[11: 8];
            r_data_001      = r_data_in[ 7: 4];
            r_data_000      = r_data_in[ 3: 0];
            r_data_edge_015 = { r_data_015[0:0], r_data_014[3:3] };
            r_data_edge_014 = { r_data_014[0:0], r_data_013[3:3] };
            r_data_edge_013 = { r_data_013[0:0], r_data_012[3:3] };
            r_data_edge_012 = { r_data_012[0:0], r_data_011[3:3] };
            r_data_edge_011 = { r_data_011[0:0], r_data_010[3:3] };
            r_data_edge_010 = { r_data_010[0:0], r_data_009[3:3] };
            r_data_edge_009 = { r_data_009[0:0], r_data_008[3:3] };
            r_data_edge_008 = { r_data_008[0:0], r_data_007[3:3] };
            r_data_edge_007 = { r_data_007[0:0], r_data_006[3:3] };
            r_data_edge_006 = { r_data_006[0:0], r_data_005[3:3] };
            r_data_edge_005 = { r_data_005[0:0], r_data_004[3:3] };
            r_data_edge_004 = { r_data_004[0:0], r_data_003[3:3] };
            r_data_edge_003 = { r_data_003[0:0], r_data_002[3:3] };
            r_data_edge_002 = { r_data_002[0:0], r_data_001[3:3] };
            r_data_edge_001 = { r_data_001[0:0], r_data_000[3:3] };
         end
      end
      cg_axi_r_rsp.sample();
      if ((m_cover_data == 1'b1) && (r_resp == 2'b00)) begin
         cg_axi_r_data.sample();
      end
   endfunction : sample_r_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b_local
   //
   function void sample_b_local(input bit [1:0] b_resp_in,
                                input int       b_idle_count_in,
                                input int       b_busy_count_in);
      b_resp       = b_resp_in;
      b_idle_count = b_idle_count_in;
      b_busy_count = b_busy_count_in;
      cg_axi_b_rsp.sample();
   endfunction : sample_b_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_delay_local
   //
   function void sample_ar_delay_local(input bit [31:0] delay_in);
      if (m_request_master == 1'b1) begin
         ar_req_delay = delay_in;
         cg_axi_ar_req_delay.sample();
      end
   endfunction : sample_ar_delay_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_delay_local
   //
   function void sample_aw_delay_local(input bit [31:0] delay_in);
      if (m_request_master == 1'b1) begin
         aw_req_delay = delay_in;
         cg_axi_aw_req_delay.sample();
      end
   endfunction : sample_aw_delay_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w_delay_local
   //
   function void sample_w_delay_local(input bit [31:0] delay_in);
      if (m_request_master == 1'b1) begin
         w_req_delay = delay_in;
         cg_axi_w_req_delay.sample();
      end
   endfunction : sample_w_delay_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r_delay_local
   //
   function void sample_r_delay_local(input bit [31:0] delay_in, input bit out_of_order_in);
      if (m_response_master == 1'b1) begin
         r_rsp_delay = delay_in;
         r_rsp_ooo   = out_of_order_in;
         cg_axi_r_rsp_delay.sample();
      end
   endfunction : sample_r_delay_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b_delay_local
   //
   function void sample_b_delay_local(input bit [31:0] delay_in, input bit out_of_order_in);
      if (m_response_master == 1'b1) begin
         b_rsp_delay = delay_in;
         b_rsp_ooo   = out_of_order_in;
         cg_axi_b_rsp_delay.sample();
      end
   endfunction : sample_b_delay_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_req_local
   //
   function void sample_concurrent_req_local(input bit [2:0] value_in);
      concurrent_req = value_in;
      cg_axi_concurrent_req.sample();
   endfunction : sample_concurrent_req_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_rsp_local
   //
   function void sample_concurrent_rsp_local(input bit [1:0] value_in);
      concurrent_rsp = value_in;
      cg_axi_concurrent_rsp.sample();
   endfunction : sample_concurrent_rsp_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_active_count_local
   //
   function void sample_ar_active_count_local(input int value_in);
      if ((value_in >= 0) && (value_in < 32'h0000ffff)) begin
         ar_active_count = value_in[15:0];
         cg_axi_ar_active_count.sample();
      end
   endfunction : sample_ar_active_count_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_active_count_local
   //
   function void sample_aw_active_count_local(input int value_in);
      if ((value_in >= 0) && (value_in < 32'h0000ffff)) begin
         aw_active_count = value_in[15:0];
         cg_axi_aw_active_count.sample();
      end
   endfunction : sample_aw_active_count_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_vs_w_local
   //
   function void sample_aw_vs_w_local(input bit count_aw_w_in, input int count_in);
      // These need to be sampled at different times sadly, but I didn't
      // wanted to add only 1 more covergroup so there's a bit to say
      // which coverpoint to count.
      count_aw_w = count_aw_w_in;
      if (count_aw_w) begin
         num_aw_waiting_for_w = count_in;
      end
      else begin
         num_w_waiting_for_aw = count_in;
      end
      cg_axi_aw_vs_w.sample();
   endfunction : sample_aw_vs_w_local


   //===============================================================================================
   //
   // Virtual Function Redefinitions
   //
   //===============================================================================================


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar
   //
   virtual function void sample_ar(input evl_paddr_t      ar_addr_in,
                                   input evl_bus_req_id_t ar_id_in,
                                   input bit [2:0]        ar_size_in,
                                   input bit [3:0]        ar_qos_in,
                                   input int              ar_idle_count_in,
                                   input int              ar_busy_count_in);
      sample_ar_local(ar_addr_in, ar_id_in, ar_size_in, ar_qos_in, ar_idle_count_in, ar_busy_count_in);
   endfunction : sample_ar


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw
   //
   virtual function void sample_aw(input evl_paddr_t        aw_addr_in,
                                   input evl_bus_req_id_t   aw_id_in,
                                   input bit [2:0]          aw_size_in,
                                   input bit [3:0]          aw_qos_in,
                                   input evl_bus_req_user_t aw_user_in,
                                   input int                aw_idle_count_in,
                                   input int                aw_busy_count_in);
      sample_aw_local(aw_addr_in, aw_id_in, aw_size_in, aw_qos_in, aw_user_in, aw_idle_count_in, aw_busy_count_in);
   endfunction : sample_aw


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w
   //
   virtual function void sample_w(input bit [63:0]  w_strb_in,
                                  input bit [511:0] w_data_in,
                                  input int         w_idle_count_in,
                                  input int         w_busy_count_in);
      sample_w_local(w_strb_in, w_data_in, w_idle_count_in, w_busy_count_in);
   endfunction : sample_w


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r
   //
   virtual function void sample_r(input bit [1:0]   r_resp_in,
                                  input bit [511:0] r_data_in,
                                  input int         r_idle_count_in,
                                  input int         r_busy_count_in);
      sample_r_local(r_resp_in, r_data_in, r_idle_count_in, r_busy_count_in);
   endfunction : sample_r


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b
   //
   virtual function void sample_b(input bit [1:0] b_resp_in,
                                  input int       b_idle_count_in,
                                  input int       b_busy_count_in);
      sample_b_local(b_resp_in, b_idle_count_in, b_busy_count_in);
   endfunction : sample_b


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_delay
   //
   virtual function void sample_ar_delay(input bit [31:0] delay_in);
      sample_ar_delay_local(delay_in);
   endfunction : sample_ar_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_delay
   //
   virtual function void sample_aw_delay(input bit [31:0] delay_in);
      sample_aw_delay_local(delay_in);
   endfunction : sample_aw_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_w_delay
   //
   virtual function void sample_w_delay(input bit [31:0] delay_in);
      sample_w_delay_local(delay_in);
   endfunction : sample_w_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_r_delay
   //
   virtual function void sample_r_delay(input bit [31:0] delay_in, input bit out_of_order_in);
      sample_r_delay_local(delay_in, out_of_order_in);
   endfunction : sample_r_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_b_delay
   //
   virtual function void sample_b_delay(input bit [31:0] delay_in, input bit out_of_order_in);
      sample_b_delay_local(delay_in, out_of_order_in);
   endfunction : sample_b_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_req
   //
   virtual function void sample_concurrent_req(input bit [2:0] value_in);
      sample_concurrent_req_local(value_in);
   endfunction : sample_concurrent_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_concurrent_rsp
   //
   virtual function void sample_concurrent_rsp(input bit [1:0] value_in);
      sample_concurrent_rsp_local(value_in);
   endfunction : sample_concurrent_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_ar_active_count
   //
   virtual function void sample_ar_active_count(input int value_in);
      sample_ar_active_count_local(value_in);
   endfunction : sample_ar_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_active_count
   //
   virtual function void sample_aw_active_count(input int value_in);
      sample_aw_active_count_local(value_in);
   endfunction : sample_aw_active_count


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_aw_vs_w
   //
   virtual function void sample_aw_vs_w(input bit count_aw_w_in, input int count_in);
      sample_aw_vs_w_local(count_aw_w_in, count_in);
   endfunction : sample_aw_vs_w


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input bit request_master, input bit response_master, input int ar_limit, input int aw_limit);
      int value;

      m_request_master  = request_master;
      m_response_master = response_master;
      m_use_qos_0_1 = (USE_QOS_0_1 == 0) ? 1'b0 : 1'b1;
      m_use_qos_2_3 = (USE_QOS_2_3 == 0) ? 1'b0 : 1'b1;
      if (`et_value_plusargs("data_cover_on=%d", value) == 0) begin
         m_cover_data = 1'b0;
      end
      else begin
         m_cover_data = (value == 0) ? 1'b0 : 1'b1;
      end
      cg_axi_ar_req = new();
      cg_axi_aw_req = new();
      cg_axi_w_req  = new();
      cg_axi_r_rsp  = new();
      cg_axi_b_rsp  = new();
      if (m_cover_data == 1'b1) begin
         cg_axi_w_data = new();
         cg_axi_r_data = new();
      end
      if (m_request_master == 1'b1) begin
         cg_axi_ar_req_delay = new();
         cg_axi_aw_req_delay = new();
         cg_axi_w_req_delay  = new();
      end
      if (m_response_master == 1'b1) begin
         cg_axi_r_rsp_delay = new();
         cg_axi_b_rsp_delay = new();
      end
      cg_axi_concurrent_req  = new();
      cg_axi_concurrent_rsp  = new();
      cg_axi_aw_vs_w         = new();
      cg_axi_ar_active_count = new(ar_limit);
      cg_axi_aw_active_count = new(aw_limit);

      if (m_use_qos_0_1 == 1'b1) begin
         cg_axi_ar_qos_0_1 = new();
         cg_axi_aw_qos_0_1 = new();
      end
      if (m_use_qos_2_3 == 1'b1) begin
         cg_axi_ar_qos_2_3 = new();
         cg_axi_aw_qos_2_3 = new();
      end

      //
      // Sample invalid bins just to get appropriate coverage handling.
      //
      m_allow_req = 1'b0;
      sample_ar_local(evl_paddr_t'(0), ~evl_bus_req_id_t'(0), 7, 0, 32'h7fffffff, 32'h7fffffff);
      sample_r_local(2'b01, {512{1'b0}}, 32'h7fffffff, 32'h7fffffff);
      sample_b_local(2'b01, 32'h7fffffff, 32'h7fffffff);
      if (m_request_master == 1'b1) begin
         sample_ar_delay_local(0);
         sample_aw_delay_local(0);
         sample_w_delay_local(0);
      end
      if (m_response_master == 1'b1) begin
         sample_r_delay_local(0, 1'b0);
         sample_b_delay_local(0, 1'b0);
      end
      sample_concurrent_req_local(0);
      sample_concurrent_rsp_local(0);
      sample_ar_active_count_local(0);
      sample_aw_active_count_local(0);
      sample_aw_vs_w_local(1'b0, 32'h7fffffff);
      sample_aw_vs_w_local(1'b1, 32'h7fffffff);
      if (m_use_qos_0_1 == 1'b1) begin
         ar_qos = 3;
         aw_qos = 3;
         cg_axi_ar_qos_0_1.sample();
         cg_axi_aw_qos_0_1.sample();
      end
      if (m_use_qos_2_3 == 1'b1) begin
         ar_qos = 0;
         aw_qos = 0;
         cg_axi_ar_qos_2_3.sample();
         cg_axi_aw_qos_2_3.sample();
      end
      m_allow_req = 1'b1;
   endfunction

endclass : evl_axi_cov
