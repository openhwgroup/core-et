// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _TXFMA_DEFINES_
`define _TXFMA_DEFINES_

//////////////////////////////////////////////////////////////////////////////
// TXFMA specific defines
//////////////////////////////////////////////////////////////////////////////

// Datpath bit naming
`define TXFMA_A3  63
`define TXFMA_A2  62
`define TXFMA_A1  61
`define TXFMA_A0  60
`define TXFMA_B0  59
`define TXFMA_B1  58
`define TXFMA_B2  57
`define TXFMA_B3  56
`define TXFMA_B4  55
`define TXFMA_B5  54
`define TXFMA_B6  53
`define TXFMA_B7  52
`define TXFMA_B8  51
`define TXFMA_B9  50
`define TXFMA_B10 49
`define TXFMA_B11 48
`define TXFMA_B12 47
`define TXFMA_B13 46
`define TXFMA_B14 45
`define TXFMA_B15 44
`define TXFMA_B16 43
`define TXFMA_B17 42
`define TXFMA_B18 41
`define TXFMA_B19 40
`define TXFMA_B20 39
`define TXFMA_B21 38
`define TXFMA_B22 37
`define TXFMA_B23 36
`define TXFMA_B24 35
`define TXFMA_B25 34
`define TXFMA_B26 33
`define TXFMA_B27 32
`define TXFMA_B28 31
`define TXFMA_B29 30
`define TXFMA_B30 29
`define TXFMA_B31 28
`define TXFMA_B32 27
`define TXFMA_B33 26
`define TXFMA_B34 25
`define TXFMA_B35 24
`define TXFMA_B36 23
`define TXFMA_B37 22
`define TXFMA_B38 21
`define TXFMA_B39 20
`define TXFMA_B40 19
`define TXFMA_B41 18
`define TXFMA_B42 17
`define TXFMA_B43 16
`define TXFMA_B44 15
`define TXFMA_B45 14
`define TXFMA_B46 13
`define TXFMA_B47 12
`define TXFMA_B48 11
`define TXFMA_B49 10
`define TXFMA_B50 9
`define TXFMA_B51 8
`define TXFMA_B52 7
`define TXFMA_B53 6
`define TXFMA_B54 5
`define TXFMA_B55 4
`define TXFMA_B56 3
`define TXFMA_B57 2
`define TXFMA_B58 1
`define TXFMA_B59 0

//TXFMA datapath definitions
`define TXFMA_BOOTH_PP_SZ            34
`define TXFMA_BOOTH_PP16_SZ          32
`define TXFMA_BOOTH_PP_CORR_SZ       16
`define TXFMA_EXP_PS_SZ              `FLOAT32_E
`define TXFMA_EXP_PH_SZ              `FLOAT16_E
`define TXFMA_MANT_PS_SZ             `FLOAT32_M
`define TXFMA_MANT_PH_SZ             `FLOAT16_M
`define TXFMA_EXP_SEXT_PS_SZ         10
`define TXFMA_EXP_SEXT_PH_SZ         7
`define TXFMA_PS_DPATH_SZ            (`TXFMA_A3 - `TXFMA_B47 + 1)
`define TXFMA_PH_DPATH_SZ            (`TXFMA_A3 - `TXFMA_B19 + 1)
`define TXFMA_NORM_SHF_SZ            33
`define TXFMA_NORM_SHF_AMNT_SZ       7
`define TXFMA_LXD_PS_SZ              7
`define TXFMA_LXD_PH_SZ              6
`define TXFMA_ETRZ_PS_SZ             5
`define TXFMA_ETRZ_PROD_PS_SZ        6
`define TXFMA_ETRZ_PROD_PH_SZ        5
`define TXFMA_ETRZ_PH_SZ             4
`define TXFMA_WSPS_ETRZ_SZ           6
`define TXFMA_WSPH_ETRZ_SZ           5

`define TXFMA_LXD_HI_MAX_VALUE      (`TXFMA_A0 - `TXFMA_B47)

`define TXFMA_F32_A_C_STICKY_DISTANCE        (`TXFMA_B22-`TXFMA_B45)        // distance between end of C mantissa and end of datapath
`define TXFMA_F32_PROD_STICKY_DISTANCE       (`TXFMA_B45-`TXFMA_B47)        // distance between end of prod mantissa and end of datapath
`define TXFMA_INT_STICKY_DISTANCE            (`TXFMA_B22-`TXFMA_B31)        // distance between end of Integer Datatype and end of integer datapath
`define TXFMA_F16_F32_C_STICKY_DISTANCE      (`TXFMA_B22-`TXFMA_B24)        // distance between end of C mantissa and end of f32 f16 datapath (r bit)
`define TXFMA_F16_F32_PROD_STICKY_DISTANCE   (`TXFMA_B19-`TXFMA_B24)        // distance between end of ph product and end of f32 f16 datapath (r bit)
`define TXFMA_TRANS_EXP_STICKY_DISTANCE      (`FLOAT32_E_BIAS_HEX+`TXFMA_EXP_FRAC_OFFSET) // trans exponent sticky distance
`define TXFMA_F16_F32_MAN_DIFF               -(`TXFMA_EXP_SEXT_PS_SZ'd12)   // F16 mantissa - F32 mantissa number of bits 
`define TXFMA_F11_F32_MAN_DIFF               -(`TXFMA_EXP_SEXT_PS_SZ'd17)   // F11 mantissa - F32 mantissa number of bits 
`define TXFMA_F10_F32_MAN_DIFF               -(`TXFMA_EXP_SEXT_PS_SZ'd18)   // F10 mantissa - F32 mantissa number of bits 

`define TXFMA_RND_CTL_SZ             6
`define TXFMA_RND_CTL_HI_SZ          6
`define TXFMA_RND_CTL_LO_SZ          2
`define TXFMA_RM_SZ                  `FP_RM_BITS
`define TXFMA_MERGE_MUX_SEL_SZ       3
`define TXFMA_EDIFF_OPD_SEL_SZ       5
`define TXFMA_MINMAX_OPD_SEL_SZ      4

`define TXFMA_FTZ_SZ                 2
`define TXFMA_FTZ_HI_SZ              3

`define TXFMA_CVT_F32INT_OFFSET          `TXFMA_EXP_SEXT_PS_SZ'd31 // distance between A0 and B30
`define TXFMA_CVT_INTF32_OFFSET          `TXFMA_EXP_SEXT_PS_SZ'd31 // distance between A0 and B30
`define TXFMA_GCVT_F32_NORM_OFFSET       `TXFMA_EXP_SEXT_PS_SZ'd2  // distance between B0 and A1
`define TXFMA_EXP_FRAC_OFFSET            `TXFMA_EXP_SEXT_PS_SZ'd7  // distance between A0 and B7
`define TXFMA_CVT_F32_FXP1615_OFFSET     `TXFMA_EXP_SEXT_PS_SZ'd15 // distance between A0 and B14
`define TXFMA_CVT_FXP1714_F32_OFFSET     `TXFMA_EXP_SEXT_PS_SZ'd17 // distance between A0 and B16
`define TXFMA_TRANS_RCP_OFFSET           `TXFMA_EXP_SEXT_PS_SZ'd254 

`define TXFMA_NORM_F32_BIAS              `TXFMA_EXP_SEXT_PS_SZ'd158  // 127 (bias) + 31 (shift)  

`define UNORM_MAX_OUT(N)  {(`FLOAT32_W - N)'(0), N'(((N+1)'(1)<<N) - 1)}   // maximum number for unorm (1)
`define SNORM_MAX_OUT(N)  {(`FLOAT32_W - N)'(0), N'((N'(1)<<(N-1)) - 1)}   // maximum number for snorm (1)
`define UNORM_MIN_OUT     `FLOAT32_W'(0);                                   // minimal number for unorm (0) 
`define SNORM_MIN_OUT(N)  {(`FLOAT32_W - N)'(0), N'((N'(1)<<(N-1)) + 1)};  // minimal number for unorm (-1) 

`define TXFMA_INT_RES_MUX_SZ         2

`define TXFMA_POS_INT_NAN            32'h7fffffff                 
`define TXFMA_NEG_INT_NAN            32'h80000000
`define TXFMA_POS_UINT_NAN           32'hffffffff

`define TXFMA_BIAS_DIFF              10'd112

`define TXFMA_SEL_PH                 `TXFMA_EDIFF_OPD_SEL_SZ'd0
`define TXFMA_SEL_PL                 `TXFMA_EDIFF_OPD_SEL_SZ'd1
`define TXFMA_SEL_C                  `TXFMA_EDIFF_OPD_SEL_SZ'd2
`define TXFMA_SEL_A                  `TXFMA_EDIFF_OPD_SEL_SZ'd3
`define TXFMA_SEL_P                  `TXFMA_EDIFF_OPD_SEL_SZ'd4
`define TXFMA_SEL_C_INT              `TXFMA_EDIFF_OPD_SEL_SZ'd5
`define TXFMA_SEL_CVT_F32INT         `TXFMA_EDIFF_OPD_SEL_SZ'd6
`define TXFMA_SEL_ZERO               `TXFMA_EDIFF_OPD_SEL_SZ'd7
`define TXFMA_SEL_PH_PL              `TXFMA_EDIFF_OPD_SEL_SZ'd8
`define TXFMA_SEL_P_INT              `TXFMA_EDIFF_OPD_SEL_SZ'd9
`define TXFMA_SEL_ROUND              `TXFMA_EDIFF_OPD_SEL_SZ'd10
`define TXFMA_SEL_GCVT_F32_NORM      `TXFMA_EDIFF_OPD_SEL_SZ'd11
`define TXFMA_SEL_EXP_RES            `TXFMA_EDIFF_OPD_SEL_SZ'd12
`define TXFMA_SEL_EXP_FRAC           `TXFMA_EDIFF_OPD_SEL_SZ'd13
`define TXFMA_SEL_CVT_F32_FXP1615    `TXFMA_EDIFF_OPD_SEL_SZ'd14
`define TXFMA_SEL_B                  `TXFMA_EDIFF_OPD_SEL_SZ'd15

`define TXFMA_INT_MINMAX_SEL_PH       `TXFMA_MINMAX_OPD_SEL_SZ'd0
`define TXFMA_INT_MINMAX_SEL_PL       `TXFMA_MINMAX_OPD_SEL_SZ'd1
`define TXFMA_INT_MINMAX_SEL_C        `TXFMA_MINMAX_OPD_SEL_SZ'd2
`define TXFMA_INT_MINMAX_SEL_CH       `TXFMA_MINMAX_OPD_SEL_SZ'd3
`define TXFMA_INT_MINMAX_SEL_CL       `TXFMA_MINMAX_OPD_SEL_SZ'd4
`define TXFMA_INT_MINMAX_SEL_A        `TXFMA_MINMAX_OPD_SEL_SZ'd5
`define TXFMA_INT_MINMAX_SEL_P        `TXFMA_MINMAX_OPD_SEL_SZ'd6
`define TXFMA_INT_MINMAX_SEL_S        `TXFMA_MINMAX_OPD_SEL_SZ'd7  // INT SUM

`define TXFMA_MERGE_MUX_SEL_INTL        `TXFMA_MERGE_MUX_SEL_SZ'd0
`define TXFMA_MERGE_MUX_SEL_INTH        `TXFMA_MERGE_MUX_SEL_SZ'd1
`define TXFMA_MERGE_MUX_SEL_NORM        `TXFMA_MERGE_MUX_SEL_SZ'd2
`define TXFMA_MERGE_MUX_SEL_NORM_NSHC   `TXFMA_MERGE_MUX_SEL_SZ'd3
`define TXFMA_MERGE_MUX_SEL_COMP_TRUE   `TXFMA_MERGE_MUX_SEL_SZ'd4
`define TXFMA_MERGE_MUX_SEL_COMP_FALSE  `TXFMA_MERGE_MUX_SEL_SZ'd5
`define TXFMA_MERGE_MUX_SEL_NORM_TRANS  `TXFMA_MERGE_MUX_SEL_SZ'd6
`define TXFMA_MERGE_MUX_SEL_NORM_LOG_RR `TXFMA_MERGE_MUX_SEL_SZ'd7

`define TXFMA_SEL_EDIFF_EC_EPH       `TXFMA_EDIFF_OPD_SEL_SZ'd0
`define TXFMA_SEL_EDIFF_EC_EPL       `TXFMA_EDIFF_OPD_SEL_SZ'd1
`define TXFMA_SEL_EDIFF_N_EC_EPH     `TXFMA_EDIFF_OPD_SEL_SZ'd2
`define TXFMA_SEL_EDIFF_N_EC_EPL     `TXFMA_EDIFF_OPD_SEL_SZ'd3
`define TXFMA_SEL_EDIFF_EPH_EPL      `TXFMA_EDIFF_OPD_SEL_SZ'd4
`define TXFMA_SEL_EDIFF_N_EPH_EPL    `TXFMA_EDIFF_OPD_SEL_SZ'd5
`define TXFMA_SEL_EDIFF_ECH_EPH      `TXFMA_EDIFF_OPD_SEL_SZ'd6
`define TXFMA_SEL_EDIFF_N_ECH_EPH    `TXFMA_EDIFF_OPD_SEL_SZ'd7
`define TXFMA_SEL_EDIFF_ZERO         `TXFMA_EDIFF_OPD_SEL_SZ'd8
`define TXFMA_SEL_EDIFF_LOG_MUL      `TXFMA_EDIFF_OPD_SEL_SZ'd9
`define TXFMA_SEL_EDIFF_LOG_MUL2     `TXFMA_EDIFF_OPD_SEL_SZ'd10
`define TXFMA_SEL_EDIFF_NR2_FRCPFXP  `TXFMA_EDIFF_OPD_SEL_SZ'd11

`define TXFMA_SEL_STKY_TP_EDIFF      `TXFMA_EDIFF_OPD_SEL_SZ'd0
`define TXFMA_SEL_STKY_TC_EDIFF      `TXFMA_EDIFF_OPD_SEL_SZ'd1
`define TXFMA_SEL_STKY_TA_EDIFF      `TXFMA_EDIFF_OPD_SEL_SZ'd2


`define TXFMA_INT_RES_LOW            `TXFMA_INT_RES_MUX_SZ'd0
`define TXFMA_INT_RES_HIGH           `TXFMA_INT_RES_MUX_SZ'd1
`define TXFMA_INT_RES_FP             `TXFMA_INT_RES_MUX_SZ'd2


`define TXFMA_PS_L    `TXFMA_B22
`define TXFMA_PS_R    `TXFMA_B23
`define TXFMA_PS_G    `TXFMA_B24
`define TXFMA_INT_L   29
`define TXFMA_INT_R   28
`define TXFMA_PH_HI_L `TXFMA_B9
`define TXFMA_PH_HI_R `TXFMA_B10
`define TXFMA_PH_HI_G `TXFMA_B11
`define TXFMA_PH_LO_L `TXFMA_B35
`define TXFMA_PH_LO_R `TXFMA_B36
`define TXFMA_PH_LO_G `TXFMA_B37
`define TXFMA_F11_L   `TXFMA_B5
`define TXFMA_F11_R   `TXFMA_B6
`define TXFMA_F11_G   `TXFMA_B7
`define TXFMA_F10_L   `TXFMA_B4
`define TXFMA_F10_R   `TXFMA_B5
`define TXFMA_F10_G   `TXFMA_B6

// Low product mapping
`define TXFMA_LO_A3  `TXFMA_B34
`define TXFMA_LO_A2  `TXFMA_B35
`define TXFMA_LO_A1  `TXFMA_B36
`define TXFMA_LO_A0  `TXFMA_B37
`define TXFMA_LO_B0  `TXFMA_B38
`define TXFMA_LO_B1  `TXFMA_B39 
`define TXFMA_LO_B2  `TXFMA_B40 
`define TXFMA_LO_B3  `TXFMA_B41 
`define TXFMA_LO_B4  `TXFMA_B42 
`define TXFMA_LO_B5  `TXFMA_B43 
`define TXFMA_LO_B6  `TXFMA_B44 
`define TXFMA_LO_B7  `TXFMA_B45 
`define TXFMA_LO_B8  `TXFMA_B46 
`define TXFMA_LO_B9  `TXFMA_B47 
`define TXFMA_LO_B10 `TXFMA_B48 
`define TXFMA_LO_B11 `TXFMA_B49 
`define TXFMA_LO_B12 `TXFMA_B50 
`define TXFMA_LO_B13 `TXFMA_B51 
`define TXFMA_LO_B14 `TXFMA_B52 
`define TXFMA_LO_B15 `TXFMA_B53 
`define TXFMA_LO_B16 `TXFMA_B54 
`define TXFMA_LO_B17 `TXFMA_B55 
`define TXFMA_LO_B18 `TXFMA_B56 
`define TXFMA_LO_B19 `TXFMA_B57 
`define TXFMA_LO_B20 `TXFMA_B58 
`define TXFMA_LO_B21 `TXFMA_B59 
 

//out of range
`define TXFMA_F32_EDIFF_OUT_OF_RANGE        49
`define TXFMA_PH_PS_P_EDIFF_OUT_OF_RANGE    26
`define TXFMA_PH_PS_C_EDIFF_OUT_OF_RANGE    26
`define TXFMA_PH_EDIFF_OUT_OF_RANGE         26
`define TXFMA_EDIFF_OUT_OF_RANGE_CVT_F32INT 32

//sticky adder constants (changed for +2 bits dp)
`define TXFMA_STICKY_K_F32        10'b1111101000  // -24
`define TXFMA_STICKY_K_FI         10'b1111110000  // -16
`define TXFMA_STICKY_TO_G_K_F16_L 10'b1111101000  // -24
`define TXFMA_STICKY_TO_R_K_IF    10'b1111101000  // -24

//control sigs
`define TXFMA_SIGS_SZ        19
`define TXFMA_REN1            0
`define TXFMA_REN2            1
`define TXFMA_REN3            2
`define TXFMA_FMA             3
`define TXFMA_FMS             4
`define TXFMA_ADD             5
`define TXFMA_SUB             6
`define TXFMA_MUL             7  
`define TXFMA_CVT             8
`define TXFMA_GCVT            9
`define TXFMA_UGCVT          10
`define TXFMA_SGCVT          11
`define TXFMA_F32_FX         12
`define TXFMA_FX_F32         13
`define TXFMA_F32_NORM       14
`define TXFMA_NORM_F32       15
`define TXFMA_TRANS1         16
`define TXFMA_TRANS2         17
`define TXFMA_FP32_ADD_SUB   18

`endif // _TXFMA_DEFINES_

