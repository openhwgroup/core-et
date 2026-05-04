// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

    typedef enum logic [4:0]{
        TRANS_NULL,                 //0
        TRANS_RCP_FMA1_STATE,       //1
        TRANS_RCP_FMA2_STATE,       //2
        TRANS_RSQRT_FMA1_STATE,     //3
        TRANS_RSQRT_FMA2_STATE,     //4
        TRANS_LOG_FMA1_STATE,       //5
        TRANS_LOG_FMA2_STATE,       //6
        TRANS_LOG_MUL_STATE,        //7
        TRANS_EXP_RR_STATE,         //8
        TRANS_EXP_FMA1_STATE,       //9
        TRANS_EXP_FMA2_STATE,       //10
        TRANS_SIN_TR_STATE,         //11
        TRANS_SIN_RR_STATE,         //12
        TRANS_SIN_P1_STATE,         //13
        TRANS_SIN_P2_STATE,         //14
        TRANS_SIN_P3_STATE,         //15
        TRANS_NR_STATE,             //16
        TRANS_DIRTY_STATE           //17
    } trans_state_id;


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

    typedef struct packed{
        trans_state_id              id; //Indicates operation to be performed
        logic                       thread_id;
        logic[4:0]                  rs; //Register from which we read
        logic[4:0]                  rd; //Destination register
        logic                       mask_valid;
        logic[`VPU_LANE_NUM-1:0]    mask; 
    } trans_state;

    //SEQUENCER CONTROL SIGNALS

    typedef struct packed{
        logic                       en;
        logic[`VPU_LANE_NUM-1:0]    mask;
        logic                       thread_id;
        logic                       fcsr_en;
    } trans_write_ctrl;


    //ROM INTERNALS

    typedef enum logic[3:0]{
        TRANS_ROM_NONE_ID,
        TRANS_ROM_RCP1_ID,
        TRANS_ROM_RCP2_ID,
        TRANS_ROM_RSQRT1_ID,
        TRANS_ROM_RSQRT2_ID,
        TRANS_ROM_LOG1_ID,
        TRANS_ROM_LOG2_ID,
        TRANS_ROM_EXP1_ID,
        TRANS_ROM_EXP2_ID,
        TRANS_ROM_SIN1_ID,
        TRANS_ROM_SIN2_ID,
        TRANS_ROM_SIN3_ID
    } trans_rom_id;

    typedef logic[`TRANS_ROM_MAX_INDEX_SIZE-1:0] trans_rom_index;

    typedef struct packed{
        trans_rom_id         cmd;
        trans_rom_index      index;
    } trans_rom_ctrl;

    //ROM OUTPUT
    
    typedef struct packed{
        logic                         exc;        
        logic[`TRANS_MASK_SIZE-1:0]   mask;   //7
        logic                         taylor;        
        logic[`TRANS_C2_MAX_SIZE-1:0] c2;     //17
        logic[`TRANS_C0_MAX_SIZE-1:0] c0;
    } trans_fma_coefficients;


    typedef struct packed{
        logic           rs_valid;
        minion_reg_dest rs;
        logic           rd_valid;
        minion_reg_dest rd;
    } trans_scoreboard_slot;

