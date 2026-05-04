// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_decoder (
  input  logic [`OPCODE_WIDTH-1:0] id_inst,
  output vpu_ctrl_sigs_t           id_sigs,
  output logic                     id_vpu_insn
);

  // combinational process
  // decode instruction
  always_comb begin
    id_vpu_insn = 1'b1;


    //                                cmd
    //                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub
    //                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul
    //                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round
    //                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags
    //                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |
    //                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |
    casex (id_inst)                                                   
      `FLW:          id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSW:          id_sigs = '{`VPU_FCMD_MVS_FX,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`N,`N,`N,`N,    `N,`Y,`Y,`N,`N,   `Y,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMV_S_X:      id_sigs = '{`VPU_FCMD_MV_XF,           `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `Y,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_S_W:     id_sigs = '{`VPU_FCMD_CVT_F32INT,      `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`Y,    `N,`VPU_DTYPE_INT,   `Y,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_S_WU:    id_sigs = '{`VPU_FCMD_CVT_F32UINT,     `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`Y,    `N,`VPU_DTYPE_INT,   `Y,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FMV_X_S:      id_sigs = '{`VPU_FCMD_MVS_FX,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`Y,`N};
      `FCLASS_S:     id_sigs = '{`VPU_FCMD_CLASS,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_W_S:     id_sigs = '{`VPU_FCMD_CVT_INTF32,      `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_WU_S:    id_sigs = '{`VPU_FCMD_CVT_UINTF32,     `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`Y,`Y};
      `FEQ_S:        id_sigs = '{`VPU_FCMD_FEQ,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`Y};
      `FLT_S:        id_sigs = '{`VPU_FCMD_FLT,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`Y};
      `FLE_S:        id_sigs = '{`VPU_FCMD_FLE,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`Y};
      `FSGNJ_S:      id_sigs = '{`VPU_FCMD_FSGNJ,           `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSGNJN_S:     id_sigs = '{`VPU_FCMD_FSGNJN,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSGNJX_S:     id_sigs = '{`VPU_FCMD_FSGNJX,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMIN_S:       id_sigs = '{`VPU_FCMD_MIN,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FMAX_S:       id_sigs = '{`VPU_FCMD_MAX,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FADD_S:       id_sigs = '{`VPU_FCMD_ADD,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`Y,   `N,`N,`Y,`Y};
      `FSUB_S:       id_sigs = '{`VPU_FCMD_SUB,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `Y,`N,`Y,`Y};
      `FMUL_S:       id_sigs = '{`VPU_FCMD_MUL,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`Y,`Y,`Y};
      `FMADD_S:      id_sigs = '{`VPU_FCMD_MADD,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};
      `FMSUB_S:      id_sigs = '{`VPU_FCMD_MSUB,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`Y,`N,   `N,`N,`Y,`Y};
      `FNMADD_S:     id_sigs = '{`VPU_FCMD_NMADD,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};
      `FNMSUB_S:     id_sigs = '{`VPU_FCMD_NMSUB,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`Y,`N,   `N,`N,`Y,`Y};


      //                                cmd
      //                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub
      //                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul
      //                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round
      //                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags
      //                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |
      //                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |
      // Moscow
      `FLQ2:        id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSQ2:        id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
                                                                                                                                                   
      //Packed-Single                                                                                                                              
        //Load & store                                                                                                                             
      `FLW_PS:      id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSW_PS:      id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`Y,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Broadcast                                                                                                                                
      `FBC_PS:      id_sigs = '{`VPU_FCMD_FBC,             `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FBCX_PS:     id_sigs = '{`VPU_FCMD_FBC,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `Y,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FBCI_PS:     id_sigs = '{`VPU_FCMD_FBCI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
        //Gather and Scatter                                                                                                                       
      `FGW_PS:      id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGH_PS:      id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGB_PS:      id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCW_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCH_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCB_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FG32W_PS:    id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FG32H_PS:    id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FG32B_PS:    id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSC32W_PS:   id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`N,`N,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSC32H_PS:   id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`N,`N,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSC32B_PS:   id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`N,`N,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Computational                                                                                                                            
      `FADD_PS:    id_sigs = '{`VPU_FCMD_ADD,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`Y,   `N,`N,`Y,`Y};
      `FSUB_PS:    id_sigs = '{`VPU_FCMD_SUB,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `Y,`N,`Y,`Y};
      `FMUL_PS:    id_sigs = '{`VPU_FCMD_MUL,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`Y,`Y,`Y};
      `FMIN_PS:    id_sigs = '{`VPU_FCMD_MIN,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FMAX_PS:    id_sigs = '{`VPU_FCMD_MAX,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FMADD_PS:   id_sigs = '{`VPU_FCMD_MADD,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};
      `FMSUB_PS:   id_sigs = '{`VPU_FCMD_MSUB,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`Y,`N,   `N,`N,`Y,`Y};
      `FNMADD_PS:  id_sigs = '{`VPU_FCMD_NMADD,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};
      `FNMSUB_PS:  id_sigs = '{`VPU_FCMD_NMSUB,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`Y,`N,   `N,`N,`Y,`Y};
                       
//                                                                                                                                                                                                              
//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |        
        //Compare                                                                                                                           
      `FEQ_PS:     id_sigs = '{`VPU_FCMD_FEQ,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FEQM_PS:    id_sigs = '{`VPU_FCMD_FEQ,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`Y,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FLT_PS:     id_sigs = '{`VPU_FCMD_FLT,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FLTM_PS:    id_sigs = '{`VPU_FCMD_FLT,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`Y,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FLE_PS:     id_sigs = '{`VPU_FCMD_FLE,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FLEM_PS:    id_sigs = '{`VPU_FCMD_FLE,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`Y,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FCLASS_PS:  id_sigs = '{`VPU_FCMD_CLASS,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Conversion                                                                                                                               
      `FCVT_PW_PS:  id_sigs = '{`VPU_FCMD_CVT_INTF32,      `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_PWU_PS: id_sigs = '{`VPU_FCMD_CVT_UINTF32,     `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_PS_PW:  id_sigs = '{`VPU_FCMD_CVT_F32INT,      `Y,`N,`N,    `N,`Y,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_PS_PWU: id_sigs = '{`VPU_FCMD_CVT_F32UINT,     `Y,`N,`N,    `N,`Y,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_PS_RAST: id_sigs = '{`VPU_FCMD_CVT_F32RAST,     `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_RAST_PS: id_sigs = '{`VPU_FCMD_CVT_RASTF32,     `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSGNJ_PS:     id_sigs = '{`VPU_FCMD_FSGNJ,           `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSGNJN_PS:    id_sigs = '{`VPU_FCMD_FSGNJN,          `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSGNJX_PS:    id_sigs = '{`VPU_FCMD_FSGNJX,          `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMVZ_X_PS:    id_sigs = '{`VPU_FCMD_MVZ_FX,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `FMVS_X_PS:    id_sigs = '{`VPU_FCMD_MVS_FX,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `FCMOV_PS:     id_sigs = '{`VPU_FCMD_FCMOV,           `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCMOVM_PS:    id_sigs = '{`VPU_FCMD_FCMOVM,          `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSWIZZ_PS:    id_sigs = '{`VPU_FCMD_FSWIZZ,          `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FFRC_PS:      id_sigs = '{`VPU_FCMD_FFRC,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};     
      `FROUND_PS:    id_sigs = '{`VPU_FCMD_FROUND,          `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
        // Transcedental                                                                                                                     
      `FRCP_PS:      id_sigs = '{`VPU_TRANS_RCP_RR,        `N,`N,`Y,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
`ifdef ENABLE_EXTRA_TRANS
      `FRSQ_PS:      id_sigs = '{`VPU_TRANS_RSQRT_RR,      `N,`N,`Y,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
`endif
      `FLOG_PS:      id_sigs = '{`VPU_TRANS_LOG_RR,        `Y,`N,`Y,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FEXP_PS:      id_sigs = '{`VPU_TRANS_EXP_FRAC,      `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
`ifdef ENABLE_EXTRA_TRANS
      `FSIN_PS:      id_sigs = '{`VPU_FCMD_FFRC_SIN,       `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};     
`endif
      `FRCP_FIX_RAST: id_sigs = '{`VPU_FCMD_NR1_FRCPFXP,    `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`Y,`N,`N};     

//                                                                                                                                                                                                              
//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |     
                                                                         
        //Graphics upconvert
      `FCVT_PS_F16:  id_sigs = '{`VPU_FCMD_CVT_F32F16,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_F16,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FCVT_PS_F11:  id_sigs = '{`VPU_FCMD_CVT_F32F11,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_F11,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_PS_F10:  id_sigs = '{`VPU_FCMD_CVT_F32F10,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_F10,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_PS_UN24: id_sigs = '{`VPU_FCMD_CVT_F32UN24,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_UN24,  `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_UN16: id_sigs = '{`VPU_FCMD_CVT_F32UN16,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_UN16,  `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_UN10: id_sigs = '{`VPU_FCMD_CVT_F32UN10,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_UN10,  `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_UN8:  id_sigs = '{`VPU_FCMD_CVT_F32UN8,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_UN8,   `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_UN2:  id_sigs = '{`VPU_FCMD_CVT_F32UN2,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_UN2,   `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_SN16: id_sigs = '{`VPU_FCMD_CVT_F32SN16,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_SN16,  `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `FCVT_PS_SN8:  id_sigs = '{`VPU_FCMD_CVT_F32SN8,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`Y,    `Y,`VPU_DTYPE_SN8,   `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
        //Graphics downconvert                                       
      `FCVT_F16_PS:  id_sigs = '{`VPU_FCMD_CVT_F16F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_F11_PS:  id_sigs = '{`VPU_FCMD_CVT_F11F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_F10_PS:  id_sigs = '{`VPU_FCMD_CVT_F10F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_UN24_PS: id_sigs = '{`VPU_FCMD_CVT_UN24F32,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_UN16_PS: id_sigs = '{`VPU_FCMD_CVT_UN16F32,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_UN10_PS: id_sigs = '{`VPU_FCMD_CVT_UN10F32,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_UN8_PS:  id_sigs = '{`VPU_FCMD_CVT_UN8F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_UN2_PS:  id_sigs = '{`VPU_FCMD_CVT_UN2F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_SN16_PS: id_sigs = '{`VPU_FCMD_CVT_SN16F32,    `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FCVT_SN8_PS:  id_sigs = '{`VPU_FCMD_CVT_SN8F32,     `Y,`N,`N,    `N,`N,`N,`Y,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,    `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
                                                                                                                                                      
                                                                                                                                                
      //Packed-Integer (TXFMA)                                                                                                                             
        //Arithmetic                                                                                                                                 
      `FMUL_PI:     id_sigs = '{`VPU_FCMD_MULL,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`Y,`N,`N};
      `FMULH_PI:     id_sigs = '{`VPU_FCMD_MULH,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`Y,`N,`N};
      `FMULHU_PI:    id_sigs = '{`VPU_FCMD_MULHU,           `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`Y,`N,`N};
                                                                         
 
//                                                                                                                                                                                                              
//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |                                                                  
      //Packed-Integer (Short)                                                                                                                                   
        //Arithemtic 
      `FADD_PI:      id_sigs = '{`VPU_FCMD_ADD,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`Y,   `N,`N,`N,`N};
      `FADDI_PI:     id_sigs = '{`VPU_FCMD_ADDI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`Y,   `N,`N,`N,`N};      
      `FSUB_PI:      id_sigs = '{`VPU_FCMD_SUB,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `Y,`N,`N,`N};                                                                                                                                           
      `FMIN_PI:      id_sigs = '{`VPU_FCMD_MIN,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMAX_PI:      id_sigs = '{`VPU_FCMD_MAX,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMINU_PI:     id_sigs = '{`VPU_FCMD_MINU,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMAXU_PI:     id_sigs = '{`VPU_FCMD_MAXU,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Logic                                                                                                                            
      `FAND_PI:     id_sigs = '{`VPU_FCMD_AND,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FOR_PI:      id_sigs = '{`VPU_FCMD_OR,              `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FNOT_PI:     id_sigs = '{`VPU_FCMD_NOT,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FXOR_PI:     id_sigs = '{`VPU_FCMD_XOR,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSLL_PI:     id_sigs = '{`VPU_FCMD_SLL,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSRL_PI:     id_sigs = '{`VPU_FCMD_SRL,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSRA_PI:     id_sigs = '{`VPU_FCMD_SRA,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Compare                                                                                                                                    
      `FEQ_PI:      id_sigs = '{`VPU_FCMD_FEQ,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FLT_PI:      id_sigs = '{`VPU_FCMD_FLT,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FLTU_PI:     id_sigs = '{`VPU_FCMD_FLTU,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FLE_PI:      id_sigs = '{`VPU_FCMD_FLE,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FLTM_PI:     id_sigs = '{`VPU_FCMD_FLT,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`Y,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSETM_PI:    id_sigs = '{`VPU_FCMD_FSETM,           `N,`Y,`N,    `N,`N,`N,`N,    `N,`Y,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSAT8_PI:    id_sigs = '{`VPU_FCMD_FSAT8,           `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSATU8_PI:   id_sigs = '{`VPU_FCMD_FSATU8,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};      
        //Immediate                                                                                                                                     
      `FANDI_PI:    id_sigs = '{`VPU_FCMD_ANDI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSLLI_PI:    id_sigs = '{`VPU_FCMD_SLLI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FBCI_PI:     id_sigs = '{`VPU_FCMD_FBCI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSRLI_PI:    id_sigs = '{`VPU_FCMD_SRLI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSRAI_PI:    id_sigs = '{`VPU_FCMD_SRAI,            `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        // Pack and Replicate
      `FPACKREPB_PI: id_sigs = '{`VPU_FCMD_PACKREPB,        `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FPACKREPH_PI: id_sigs = '{`VPU_FCMD_PACKREPH,        `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      //Cubeface
      `CUBEF_PS:    id_sigs = '{`VPU_FCMD_CUBEFACE,        `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `CUBEFIDX_PS: id_sigs = '{`VPU_FCMD_CUBEFACE_IDX,    `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `CUBESGNSC_PS: id_sigs = '{`VPU_FCMD_CUBESGNSC,       `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `CUBESGNTC_PS: id_sigs = '{`VPU_FCMD_CUBESGNTC,       `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      //Mask Extension                                                                                                                             
      `MAND:        id_sigs = '{`VPU_FCMD_MAND,            `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`Y,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MOR:         id_sigs = '{`VPU_FCMD_MOR,             `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`Y,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MXOR:        id_sigs = '{`VPU_FCMD_MXOR,            `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`Y,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MNOT:        id_sigs = '{`VPU_FCMD_MNOT,            `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MOVA_M_X:    id_sigs = '{`VPU_FCMD_MOVA_M_X,        `N,`N,`N,    `N,`N,`N,`N,    `N,`Y,`N,`Y,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `Y,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MOVA_X_M:    id_sigs = '{`VPU_FCMD_MOVA_X_M,        `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`Y,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `MOV_M_X:     id_sigs = '{`VPU_FCMD_MOV_M_X,         `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `Y,`N,`N,`N,`N,   `N,`N,`N,`N};
      `MASKPOPC:    id_sigs = '{`VPU_FCMD_MPOPC,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `MASKPOPCZ:   id_sigs = '{`VPU_FCMD_MPOPCZ,          `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};
      `MASKPOPCR:   id_sigs = '{`VPU_FCMD_MPOPC_RAST,      `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`Y,     `N,`N,`N,`Y,`Y,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `N,`VPU_DTYPE_F32,   `N,`Y,`N,`N,`N,   `N,`N,`N,`N};                                                               


//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |
      // atomics
      `FAMOADDL_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOSWAPL_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOANDL_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOORL_PI:   id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOXORL_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMINL_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXL_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMINUL_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXUL_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXL_PS:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMINL_PS:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOADDG_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOSWAPG_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOANDG_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOORG_PI:   id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOXORG_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMING_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXG_PI:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMINUG_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXUG_PI: id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMAXG_PS:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     
      `FAMOMING_PS:  id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};                                                     

//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |
      // instructions for CUDA support
      `FLWL_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSWL_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`N,`N,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGWL_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};     
      `FSCWL_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FLWG_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSWG_PS:     id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`N,`N,`Y,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGWG_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCWG_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGBG_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCBG_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGHL_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};     
      `FSCHL_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGHG_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSCHG_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FGBL_PS:     id_sigs = '{`VPU_FCMD_X,               `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};    
      `FSCBL_PS:    id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `Y,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`Y,`N,`N,`Y,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};


      default      : begin
         id_sigs = '{default:0};
         id_vpu_insn = 1'b0;
      end
      
    endcase
  end

endmodule
