// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_uinst_decoder
(
    input  logic [`OPCODE_WIDTH-1:0] id_inst,
    output vpu_ctrl_sigs_t           id_sigs
);

  //combinational process
    //decode instruction
  always_comb begin
//                                                                                                                                                                                                                 
//                                cmd                                                                                                                                                                              
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |       
    casex (id_inst)                                                   
      //Packed-Single                                                                                                                              
        //Load & store                                                                                                                             
      `FSW_PS      : id_sigs = '{`VPU_FCMD_MV_XF,           `N,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `Y,`Y,`N,`N,`N,    `N,`N,`Y,`N,`N,   `Y,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
        //Computational                                                                                                                            
      `FADD_PS     : id_sigs = '{`VPU_FCMD_ADD,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`Y,   `N,`N,`Y,`Y};
      `FSUB_PS     : id_sigs = '{`VPU_FCMD_SUB,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `Y,`N,`Y,`Y};
      `FMUL_PS     : id_sigs = '{`VPU_FCMD_MUL,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`Y,`Y,`Y};
      `FMIN_PS     : id_sigs = '{`VPU_FCMD_MIN,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FMAX_PS     : id_sigs = '{`VPU_FCMD_MAX,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`Y};
      `FMADD_PS    : id_sigs = '{`VPU_FCMD_MADD,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};
        //Conversion                                                                                                                               
      `FCVT_PW_PS  : id_sigs = '{`VPU_FCMD_CVT_INTF32,      `Y,`N,`N,    `N,`Y,`N,`N,    `N,`N,`Y,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCVT_PS_PW  : id_sigs = '{`VPU_FCMD_CVT_F32INT,      `Y,`N,`N,    `N,`Y,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`Y,`Y};
      `FCMOVM_PS   : id_sigs = '{`VPU_FCMD_FCMOVM,          `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`Y,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};

      // Trans
      `TR_FMA1_RCP  : id_sigs = '{`VPU_TRANS_RCP_FMA1,      `Y,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`N};
      `TR_FMA2_RCP  : id_sigs = '{`VPU_TRANS_RCP_FMA2,      `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`Y};
`ifdef ENABLE_EXTRA_TRANS
      `TR_FMA1_RSQRT : id_sigs = '{`VPU_TRANS_RSQRT_FMA1,   `Y,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`N};
      `TR_FMA2_RSQRT : id_sigs = '{`VPU_TRANS_RSQRT_FMA2,   `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`Y};
`endif
      `TR_FMA1_LOG  : id_sigs = '{`VPU_TRANS_LOG_FMA1,      `Y,`Y,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`N};
      `TR_FMA2_LOG  : id_sigs = '{`VPU_TRANS_LOG_FMA2,      `Y,`Y,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`N};
      `TR_MUL_LOG   : id_sigs = '{`VPU_TRANS_LOG_MUL,       `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`Y,`N,`Y};
`ifdef ENABLE_EXTRA_TRANS
      `TR_TR_SIN    : id_sigs = '{`VPU_TRANS_SIN_TRANSFORM, `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`Y,`N};
      `TR_RR_SIN    : id_sigs = '{`VPU_TRANS_SIN_RR,        `N,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `TR_P1_SIN    : id_sigs = '{`VPU_TRANS_SIN_P1,        `Y,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`Y,`Y,`N};
      `TR_P2_SIN    : id_sigs = '{`VPU_TRANS_SIN_P2,        `Y,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`Y,`N,`N,   `N,`N,`Y,`N};
      `TR_P3_SIN    : id_sigs = '{`VPU_TRANS_SIN_P3,        `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`Y,`Y,`Y};
`endif
      `TR_RR_EXP    : id_sigs = '{`VPU_TRANS_EXP_RR,        `N,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `TR_FMA1_EXP  : id_sigs = '{`VPU_TRANS_EXP_FMA1,      `Y,`N,`Y,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `N,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`N};
      `TR_FMA2_EXP  : id_sigs = '{`VPU_TRANS_EXP_FMA2,      `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`Y};
      `TR_NR:         id_sigs = '{`VPU_FCMD_NR2_FRCPFXP,    `Y,`N,`N,    `N,`N,`Y,`N,    `N,`N,`N,`N,     `N,`N,`N,`N,`N,    `Y,`N,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`Y,`N,`N,   `N,`N,`N,`Y};   

//                                                                                                                                                                                                              
//                                cmd                                                                                                                                                                           
//                                |                                      scatter         fromintf         ldst               wen               swap12       vector               fromint           sub              
//                                |                          txfma        |  cvt          |  tointm        |  m0ren           |  ren1           |  swap23    |  dtype             |  toint          |  mul           
//                                |                          |  shsw      |  |  transDiv  |  |  tointf     |  |  mallren      |  |  ren2        |  |  swap13 |  |                 |  |  fma         |  |  round      
//                                |                          |  |  rom    |  |  |  gcvt   |  |  |  maskop  |  |  |  mren1     |  |  |  ren3     |  |  |      |  |                 |  |  |  fms      |  |  |  wflags  
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  mren2  |  |  |  |  rend  |  |  |      |  |                 |  |  |  |  add   |  |  |  |       
//                                |                          |  |  |      |  |  |  |      |  |  |  |       |  |  |  |  |      |  |  |  |  |     |  |  |      |  |                 |  |  |  |  |     |  |  |  |    
                                                                                                                              
      //Packed-Integer                                                                                                                                   
        //Arithemtic  
      `FADD_PI     : id_sigs = '{`VPU_FCMD_ADD,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`Y,   `N,`N,`N,`N};
      `FSUB_PI     : id_sigs = '{`VPU_FCMD_SUB,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`Y,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `Y,`N,`N,`N};                                                                                                                                          
      `FMIN_PI     : id_sigs = '{`VPU_FCMD_MIN,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FMAX_PI     : id_sigs = '{`VPU_FCMD_MAX,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FXOR_PI     : id_sigs = '{`VPU_FCMD_XOR,             `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_INT,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSAT8_PI    : id_sigs = '{`VPU_FCMD_FSAT8,           `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};
      `FSATU8_PI   : id_sigs = '{`VPU_FCMD_FSATU8,          `N,`Y,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};      
        // Pack and Replicate
      `FPACKREPB_PI: id_sigs = '{`VPU_FCMD_PACKREPB,        `N,`Y,`N,    `N,`N,`N,`N,    `Y,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`N,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F32,   `N,`N,`N,`N,`N,   `N,`N,`N,`N};   

      // Tensor   
      `TMUL_PH_PS  : id_sigs = '{`VPU_FCMD_MUL,             `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`N,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F16_F32, `N,`N,`N,`N,`N,   `N,`Y,`Y,`Y};
      `TFMA_PH_PS  : id_sigs = '{`VPU_FCMD_MADD,            `Y,`N,`N,    `N,`N,`N,`N,    `N,`N,`N,`N,     `N,`Y,`N,`N,`N,    `Y,`Y,`Y,`Y,`N,   `N,`N,`N,    `Y,`VPU_DTYPE_F16_F32, `N,`N,`Y,`N,`N,   `N,`N,`Y,`Y};

      default      : begin
         id_sigs = '{default:0};
      end
      
    endcase
  end

endmodule
