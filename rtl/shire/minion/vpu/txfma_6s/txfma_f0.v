// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_f0 
(
  input  [`VPU_DATA_S_SZ-1:0]          	opa_f0a_h,
  input  [`VPU_DATA_S_SZ-1:0]          	opb_f0a_h,
  input  [`VPU_DATA_S_SZ-1:0]          	opc_f0a_h, 
  input  [`TXFMA_SIGS_SZ-1:0]          	sigs_f0a_h,
  input  [`VPU_DTYPE_SZ-1:0]           	op_dtype_f0a_h,
  input  [`VPU_FCMD_SZ-1:0]            	cmd_f0a_h,
  input  [`TRANS_C0_MAX_SIZE-1:0]      	trans_c0_c1_f0a_h,
  input  [`TRANS_C2_MAX_SIZE-1:0]      	trans_c2_f0a_h,
  input  [`TRANS_MASK_SIZE-1:0]        	trans_mask_f0a_h,
  input                                	trans_taylor_f0a_h,
  input                                	trans_exp_fma2_exc_f0a_h,
  output logic                         	sa_hi_f0a_h,
  output logic                         	sa_lo_f0a_h,
  output logic                         	sb_hi_f0a_h,
  output logic                         	sb_lo_f0a_h,
  output logic                         	sc_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	ea_hi_f0a_h,
  output logic [`TXFMA_EXP_PH_SZ-1:0]  	ea_lo_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	eb_hi_f0a_h,
  output logic [`TXFMA_EXP_PH_SZ-1:0]  	eb_lo_f0a_h,
  output logic [`TXFMA_EXP_PS_SZ-1:0]  	ec_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fa_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fb_f0a_h,
  output logic [`VPU_DATA_S_SZ-1:0]    	fc_f0a_h,
  output logic [`FLOAT32_M-1:0]        	fb_trans_f0a_h,  
  output logic                         	res_inf_f0a_h,
  output logic                         	res_snan_f0a_h,
  output logic                         	res_qnan_f0a_h,
  output logic [`FP_CLASS_BITS-1:0]    	classify_f0a_h,
  output logic                         	prod_inf_hi_f0a_h,
  output logic                         	c_inf_f0a_h,
  output logic                         	a_inf_f0a_h,
  output logic [`VPU_FLAGS_SZ-1:0]     	flags_f0a_h,
  output logic                         	a_is_qnan_hi_f0a_h,
  output logic                         	c_is_qnan_f0a_h,
  output logic                         	a_is_snan_hi_f0a_h,
  output logic                         	c_is_snan_f0a_h,
  output logic                         	res_pos_int_nan_f0a_h,
  output logic                         	res_neg_int_nan_f0a_h,
  output logic                         	fc_zero30_8_f0a_h,
  output logic                         	res_uint_nan_f0a_h,
  output logic                         	res_zero_f0a_h,
  output logic                         	res_one_f0a_h,
  output logic                         	prod_inf_lo_f0a_h,
  output logic                         	fa_hi_zero_f0a_h,
  output logic                         	fb_hi_zero_f0a_h,
  output logic                         	res_unorm_one_f0a_h,
  output logic                         	res_snorm_one_f0a_h,
  output logic                         	flags_en_f0a_h,
  output logic                         	spec_res_en_f0a_h,
  output logic                          ea_hi_zero_clr_f0a_h,
  output logic                          eb_hi_zero_clr_f0a_h,
  output logic                          ec_zero_clr_f0a_h,
  output logic                          ea_lo_zero_clr_f0a_h,
  output logic                          eb_lo_zero_clr_f0a_h
);


//internal logic
logic                           snorm_output_f0a_h;
logic                           trans1_op_f0a_h;
logic                           trans2_op_f0a_h;
logic                           trans_sin_taylor_p2_op_f0a_h;
logic                           trans_sin_rom_p3_op_f0a_h;
logic                           trans_sin_transform_op_f0a_h;
logic                           neg_fma_f0a_h;
logic                         	ea_hi_all_one_f0a_h;
logic                         	eb_hi_all_one_f0a_h;
logic                         	ec_all_one_f0a_h;
logic                           ea_hi_all_one_clr_f0a_h;
logic                           eb_hi_all_one_clr_f0a_h; 
logic                           ec_all_one_clr_f0a_h;
logic                         	ea_lo_all_one_f0a_h;
logic                         	eb_lo_all_one_f0a_h;
logic                           ea_lo_all_one_clr_f0a_h;
logic                           eb_lo_all_one_clr_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fb_corrected_f0a_h;
logic                         	eb_hi_eq_125_f0a_h;
logic                         	eb_hi_eq_253_f0a_h;
logic                         	eb_hi_eq_254_f0a_h;
logic                         	ec_gt_133_f0a_h;
logic                         	ec_eq_133_f0a_h;


// Sign/Exponent/Mantissa O/P from the InMUX
logic                         	sa_hi_inmux_f0a_h;
logic                         	sa_lo_inmux_f0a_h;
logic                         	sb_hi_inmux_f0a_h;
logic                         	sb_lo_inmux_f0a_h;
logic                         	sc_inmux_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]  	ea_hi_inmux_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]  	ea_lo_inmux_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]  	eb_hi_inmux_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]  	eb_lo_inmux_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]  	ec_inmux_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fa_inmux_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fb_inmux_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fc_inmux_f0a_h;


// Mantissa O/P from the Operand Clearing logic (qualified locally)
logic [`VPU_DATA_S_SZ-1:0]    	fa_clr_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fb_clr_f0a_h;
logic [`VPU_DATA_S_SZ-1:0]    	fc_clr_f0a_h;

logic 				ea_hi_zero_f0a_h;
logic				eb_hi_zero_f0a_h;
logic				ec_zero_f0a_h;
logic				ea_lo_zero_f0a_h;
logic				eb_lo_zero_f0a_h;

//======================================================================================
// Control logic, move to txfma_c0
//====================================================================================== 

assign neg_fma_f0a_h = (cmd_f0a_h == `VPU_FCMD_NMADD) | (cmd_f0a_h == `VPU_FCMD_NMSUB);

assign snorm_output_f0a_h = (cmd_f0a_h == `VPU_FCMD_CVT_SN16F32) | (cmd_f0a_h == `VPU_FCMD_CVT_SN8F32);
assign trans1_op_f0a_h    = (cmd_f0a_h==`VPU_TRANS_RCP_FMA1) | (cmd_f0a_h==`VPU_TRANS_RSQRT_FMA1) | (cmd_f0a_h==`VPU_TRANS_LOG_FMA1) | (cmd_f0a_h==`VPU_TRANS_EXP_FMA1) |
                            ((cmd_f0a_h==`VPU_TRANS_SIN_P1) & !trans_taylor_f0a_h);
assign trans2_op_f0a_h =    (cmd_f0a_h==`VPU_TRANS_RCP_FMA2) | (cmd_f0a_h==`VPU_TRANS_RSQRT_FMA2) | (cmd_f0a_h==`VPU_TRANS_LOG_FMA2) | (cmd_f0a_h==`VPU_TRANS_EXP_FMA2) |
                            ((cmd_f0a_h==`VPU_TRANS_SIN_P2) & !trans_taylor_f0a_h);

assign trans_sin_taylor_p2_op_f0a_h = ((cmd_f0a_h==`VPU_TRANS_SIN_P2) & trans_taylor_f0a_h);
assign trans_sin_rom_p3_op_f0a_h    = ((cmd_f0a_h==`VPU_TRANS_SIN_P3) & !trans_taylor_f0a_h);
assign trans_sin_transform_op_f0a_h = (cmd_f0a_h==`VPU_TRANS_SIN_TRANSFORM);


//===================================================================================================================
// mantissa datapath
//===================================================================================================================

always_comb begin
    //default assignation for INT and GCVT 
  fa_inmux_f0a_h = opa_f0a_h;
  fb_inmux_f0a_h = opb_f0a_h;
  fc_inmux_f0a_h = opc_f0a_h;
  fb_trans_f0a_h = opb_f0a_h[`FLOAT32_M-1:0];

    //trans1
  if (trans1_op_f0a_h) begin
    fa_inmux_f0a_h = {trans_c2_f0a_h,{(32-`TRANS_C2_MAX_SIZE){1'b0}}};    
    fb_inmux_f0a_h = {3'b0,opb_f0a_h[22:16]&trans_mask_f0a_h,opb_f0a_h[15:0],6'b0};
    fc_inmux_f0a_h = trans_c0_c1_f0a_h; //,{(32-`TRANS_C0_MAX_SIZE){1'b0}}};
  end
    //trans2
  else if (trans2_op_f0a_h) begin
    fa_inmux_f0a_h = opa_f0a_h;
    fb_inmux_f0a_h = {2'b0,opb_f0a_h[22:16]&trans_mask_f0a_h,opb_f0a_h[15:0],6'b0};
    fc_inmux_f0a_h = {4'b0,trans_c0_c1_f0a_h};
  end
    //rast nr2
  else if (cmd_f0a_h==`VPU_FCMD_NR2_FRCPFXP) begin
    fa_inmux_f0a_h = {2'b0,opa_f0a_h[31:2]};
    fb_inmux_f0a_h = {2'b0,opb_f0a_h[29:0]};
    fc_inmux_f0a_h = {opc_f0a_h[30:0],1'b0};
  end

    //float
  else begin
    case (op_dtype_f0a_h)

      `VPU_DTYPE_F32 : begin
        fa_inmux_f0a_h = {1'b1, opa_f0a_h[`FLOAT32_M-1:0], {31-`FLOAT32_M{1'b0}}};
        fb_inmux_f0a_h = {2'b0, 1'b1, opb_f0a_h[`FLOAT32_M-1:0], {31-2-`FLOAT32_M{1'b0}}};
        fc_inmux_f0a_h = {1'b1, opc_f0a_h[`FLOAT32_M-1:0], {31-`FLOAT32_M{1'b0}}};
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN24F32) begin
          fb_inmux_f0a_h = {3'b0, {24{1'b1}}, 5'b0};
        end
        if ((cmd_f0a_h == `VPU_FCMD_CVT_UN16F32) | (cmd_f0a_h == `VPU_FCMD_CVT_SN16F32)) begin
          fb_inmux_f0a_h = {3'b0, ~snorm_output_f0a_h, {15{1'b1}}, 13'b0};      
        end
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN10F32) begin
          fb_inmux_f0a_h = {3'b0, {10{1'b1}}, 19'b0};      
        end
        if ((cmd_f0a_h == `VPU_FCMD_CVT_UN8F32) | (cmd_f0a_h == `VPU_FCMD_CVT_SN8F32)) begin
          fb_inmux_f0a_h = {3'b0, ~snorm_output_f0a_h, {7{1'b1}}, 21'b0};      
        end
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN2F32) begin
          fb_inmux_f0a_h = {3'b0, {2{1'b1}}, 27'b0};
        end

          //trans
            //log prod
        if (cmd_f0a_h==`VPU_TRANS_LOG_MUL) begin
          fa_inmux_f0a_h= {2'b1,opa_f0a_h[24:0],5'b0};
          fb_inmux_f0a_h= {2'b0,opb_f0a_h[`FLOAT32_M-1:0],{30-`FLOAT32_M{1'b0}}};
        end    
            //exp frac
        if (cmd_f0a_h==`VPU_TRANS_EXP_FRAC) begin
          fa_inmux_f0a_h= {{31-1-`FLOAT32_M{1'b0}},`FLOAT32_M'h7c0000,1'b0};
        end
            //sin transform
        if (trans_sin_transform_op_f0a_h) begin
          fa_inmux_f0a_h = {1'b1, `FLOAT32_M'b0, {31-`FLOAT32_M{1'b0}}};
        end
            //sin taylor
        if (trans_sin_taylor_p2_op_f0a_h) begin
          fb_inmux_f0a_h = {2'b0, 1'b1, `FLOAT32_M'h255de7, {31-2-`FLOAT32_M{1'b0}}}; //-(M_PI*M_PI*M_PI*2*2*2/(2*3))
          fc_inmux_f0a_h = {1'b1, `FLOAT32_M'h490fdb, {31-`FLOAT32_M{1'b0}}}; //2*M
        end
            //rom sin p3
        if (trans_sin_rom_p3_op_f0a_h) begin
          fa_inmux_f0a_h = {2'b0, 1'b1, opa_f0a_h[`FLOAT32_M:0], {31-(`FLOAT32_M+1){1'b0}}};
        end
      end    

      `VPU_DTYPE_F16 : begin
        fc_inmux_f0a_h = {1'b1, opc_f0a_h[9:0], {31-`FLOAT16_M{1'b0}}};        
      end

      `VPU_DTYPE_F16_F32 : begin
        fa_inmux_f0a_h = {1'b1, opa_f0a_h[25:16], 9'b0, 1'b1, opa_f0a_h[9:0], 1'b0};
        fb_inmux_f0a_h = {2'b0, 1'b1, opb_f0a_h[25:16], 7'b0, 1'b1, opb_f0a_h[9:0], 1'b0};
        fc_inmux_f0a_h = {1'b1, opc_f0a_h[`FLOAT32_M-1:0], {31-`FLOAT32_M{1'b0}}};     
      end

      `VPU_DTYPE_F11 : begin
        fc_inmux_f0a_h = {1'b1, opc_f0a_h[5:0], {31-`FLOAT11_M{1'b0}}};
      end
      `VPU_DTYPE_F10 : begin
        fc_inmux_f0a_h = {1'b1, opc_f0a_h[4:0], {31-`FLOAT10_M{1'b0}}};
      end

      `VPU_DTYPE_UN24 : begin
        fc_inmux_f0a_h = {opc_f0a_h[23:0], 8'h0};
      end

      `VPU_DTYPE_UN16, `VPU_DTYPE_SN16 : begin
        fc_inmux_f0a_h = {opc_f0a_h[15:0], 16'h0};       
      end

      `VPU_DTYPE_UN10 : begin
        fc_inmux_f0a_h = {opc_f0a_h[9:0], 22'h0};              
      end

      `VPU_DTYPE_UN8, `VPU_DTYPE_SN8 : begin
        fc_inmux_f0a_h = {opc_f0a_h[7:0], 24'h0};              
      end

      `VPU_DTYPE_UN2 : begin
        fc_inmux_f0a_h = {opc_f0a_h[1:0], 30'h0};              
      end
    endcase
  end
end

//===================================================================================================================
// exponent datapath
//===================================================================================================================

///////////////////////////////////////////////////////////////////////////////
// exp hi greather than 126 used for rcp special case
///////////////////////////////////////////////////////////////////////////////

assign eb_hi_eq_254_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b11111110); //254
assign eb_hi_eq_253_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b11111101); //253
assign eb_hi_eq_125_f0a_h  = ((opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b01111101); //125
assign ec_gt_133_f0a_h     = ((opc_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])> 8'b10000101); //133
assign ec_eq_133_f0a_h     = ((opc_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1])==8'b10000101); //133

always_comb begin
  ea_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'b0;
  eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'b0;
  ec_inmux_f0a_h    = `TXFMA_EXP_PS_SZ'b0;
  ea_lo_inmux_f0a_h = `TXFMA_EXP_PH_SZ'b0;
  eb_lo_inmux_f0a_h = `TXFMA_EXP_PH_SZ'b0;

    //trans2
  if (trans2_op_f0a_h) begin
    eb_hi_inmux_f0a_h = opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1];

    if (cmd_f0a_h==`VPU_TRANS_EXP_FMA2 && !trans_exp_fma2_exc_f0a_h)
      eb_hi_inmux_f0a_h = opb_f0a_h[`FLOAT32_W-1:`FLOAT32_W-`FLOAT32_E];
  end

    //float
  else begin  
    case (op_dtype_f0a_h)
      `VPU_DTYPE_F32 : begin
        ea_hi_inmux_f0a_h = opa_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1];
        eb_hi_inmux_f0a_h = opb_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1];
        ec_inmux_f0a_h = opc_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1];
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN24F32) begin
          eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'd151; //24 + 127 (BIAS)
        end
        if ((cmd_f0a_h == `VPU_FCMD_CVT_UN16F32) | (cmd_f0a_h == `VPU_FCMD_CVT_SN16F32)) begin
          eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'd143;      
        end
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN10F32) begin
          eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'd137;      
        end
        if ((cmd_f0a_h == `VPU_FCMD_CVT_UN8F32) | (cmd_f0a_h == `VPU_FCMD_CVT_SN8F32)) begin
          eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'd135;   
        end
        if (cmd_f0a_h == `VPU_FCMD_CVT_UN2F32) begin
          eb_hi_inmux_f0a_h = `TXFMA_EXP_PS_SZ'd129;      
        end

        //exp frac
        if (cmd_f0a_h == `VPU_TRANS_EXP_FRAC) begin
          ea_hi_inmux_f0a_h = `FLOAT32_E'h85;
        end

        //sin fast
        if (cmd_f0a_h == `VPU_TRANS_SIN_P2) begin
          eb_hi_inmux_f0a_h = `FLOAT32_E'h84;
          ec_inmux_f0a_h = `FLOAT32_E'h81;
        end

         //rom sin p3
        if (cmd_f0a_h==`VPU_TRANS_SIN_P3 && !trans_taylor_f0a_h) begin
          ea_hi_inmux_f0a_h= `FLOAT32_E'h81;         
        end
      end

     `VPU_DTYPE_F16 : begin
       ec_inmux_f0a_h = {3'b0, opc_f0a_h[`FLOAT16_W-2:`FLOAT16_W-`FLOAT16_E-1]};        
     end

     `VPU_DTYPE_F16_F32 : begin
       ea_lo_inmux_f0a_h = opa_f0a_h[`FLOAT16_W-2:`FLOAT16_W-`FLOAT16_E-1];
       eb_lo_inmux_f0a_h = opb_f0a_h[`FLOAT16_W-2:`FLOAT16_W-`FLOAT16_E-1];
       ea_hi_inmux_f0a_h = {3'b0, opa_f0a_h[`FLOAT16_W*2-2:`FLOAT16_W*2-`FLOAT16_E-1]};
       eb_hi_inmux_f0a_h = {3'b0, opb_f0a_h[`FLOAT16_W*2-2:`FLOAT16_W*2-`FLOAT16_E-1]};
       ec_inmux_f0a_h = opc_f0a_h[`FLOAT32_W-2:`FLOAT32_W-`FLOAT32_E-1];       
     end

     `VPU_DTYPE_F11 : begin
       ec_inmux_f0a_h = {3'b0, opc_f0a_h[10:6]};
     end

     `VPU_DTYPE_F10 : begin
       ec_inmux_f0a_h = {3'b0, opc_f0a_h[9:5]};    
     end  
    endcase
  end
end

//===================================================================================================================
// sign datapath
//===================================================================================================================

always_comb begin
    //sign
  // for high bit 31 is always sign
  sa_hi_inmux_f0a_h = (opa_f0a_h[31] & ~trans2_op_f0a_h) ^ neg_fma_f0a_h;
  sb_hi_inmux_f0a_h = opb_f0a_h[31] | trans_sin_taylor_p2_op_f0a_h;
  sc_inmux_f0a_h = (opc_f0a_h[31] & !trans_sin_taylor_p2_op_f0a_h) ^ neg_fma_f0a_h;
  sa_lo_inmux_f0a_h = opa_f0a_h[`FLOAT16_W-1];
  sb_lo_inmux_f0a_h = opb_f0a_h[`FLOAT16_W-1];

  if (sigs_f0a_h[`TXFMA_NORM_F32]) begin
    sa_hi_inmux_f0a_h = opa_f0a_h[31];
    sb_hi_inmux_f0a_h = 0;
    sc_inmux_f0a_h = 0;  
  end

  if (op_dtype_f0a_h == `VPU_DTYPE_SN16) begin
    sa_hi_inmux_f0a_h = 0;
    sb_hi_inmux_f0a_h = 0;
    sc_inmux_f0a_h = opc_f0a_h[15];
  end

  if (op_dtype_f0a_h == `VPU_DTYPE_SN8) begin
    sa_hi_inmux_f0a_h = 0;
    sb_hi_inmux_f0a_h = 0;
    sc_inmux_f0a_h = opc_f0a_h[7];
  end

  if (op_dtype_f0a_h == `VPU_DTYPE_F16) begin
    sa_hi_inmux_f0a_h = 0;
    sb_hi_inmux_f0a_h = 0;
    sc_inmux_f0a_h = opc_f0a_h[15];
  end

  if ((op_dtype_f0a_h == `VPU_DTYPE_F11) | (op_dtype_f0a_h == `VPU_DTYPE_F10)) begin // FP10 and FP11 don't have sign
    sa_hi_inmux_f0a_h = 0;
    sb_hi_inmux_f0a_h = 0;
    sc_inmux_f0a_h = 0;
  end

  if (cmd_f0a_h==`VPU_FCMD_NR2_FRCPFXP) begin
    sa_hi_inmux_f0a_h = 1'b0;
    sb_hi_inmux_f0a_h = ~opb_f0a_h[31];
    sc_inmux_f0a_h = opc_f0a_h[31];
  end

end


//===================================================================================================================
// Exponent Special Detect 
//===================================================================================================================

txfma_exp_special_detect exp_special_detect
( 
  .opa               	( opa_f0a_h                ),
  .opb               	( opb_f0a_h                ),
  .opc  	            ( opc_f0a_h                ),
  .op_dtype          	( op_dtype_f0a_h           ),
  .sigs              	( sigs_f0a_h               ),  
  .cmd               	( cmd_f0a_h                ),
  .trans_exp_fma2_exc	( trans_exp_fma2_exc_f0a_h ),
  .trans_taylor 	    ( trans_taylor_f0a_h       ),
  .ea_hi_zero        	( ea_hi_zero_f0a_h         ),
  .eb_hi_zero        	( eb_hi_zero_f0a_h         ),
  .ec_zero        	    ( ec_zero_f0a_h            ),  
  .ea_hi_all_one     	( ea_hi_all_one_f0a_h      ), 
  .eb_hi_all_one     	( eb_hi_all_one_f0a_h      ), 
  .ec_all_one     	    ( ec_all_one_f0a_h         ),
  .ea_lo_zero        	( ea_lo_zero_f0a_h         ),
  .eb_lo_zero        	( eb_lo_zero_f0a_h         ),
  .ea_lo_all_one     	( ea_lo_all_one_f0a_h      ), 
  .eb_lo_all_one     	( eb_lo_all_one_f0a_h      ) 
);


//===================================================================================================================
// Operand Clearing - clear operands that are not used by the current Instruction 
//===================================================================================================================

txfma_clear clear
(
  .sigs              ( sigs_f0a_h       	   ),
  .op_dtype          ( op_dtype_f0a_h          ),
  .fa                ( fa_inmux_f0a_h          ),
  .fb                ( fb_inmux_f0a_h          ),
  .fc                ( fc_inmux_f0a_h          ),
  .sa_hi             ( sa_hi_inmux_f0a_h       ),
  .sb_hi             ( sb_hi_inmux_f0a_h       ),
  .sc                ( sc_inmux_f0a_h          ),
  .sa_lo       	     ( sa_lo_inmux_f0a_h       ),
  .sb_lo             ( sb_lo_inmux_f0a_h       ),
  .ea_hi             ( ea_hi_inmux_f0a_h       ),
  .eb_hi             ( eb_hi_inmux_f0a_h       ),
  .ec                ( ec_inmux_f0a_h          ), 
  .ea_lo             ( ea_lo_inmux_f0a_h       ),
  .eb_lo             ( eb_lo_inmux_f0a_h       ),
  .ea_hi_zero        ( ea_hi_zero_f0a_h        ),
  .eb_hi_zero        ( eb_hi_zero_f0a_h        ),
  .ec_zero           ( ec_zero_f0a_h           ),
  .ea_lo_zero        ( ea_lo_zero_f0a_h        ),
  .eb_lo_zero        ( eb_lo_zero_f0a_h        ),
  .ea_hi_all_one     ( ea_hi_all_one_f0a_h     ),
  .eb_hi_all_one     ( eb_hi_all_one_f0a_h     ), 
  .ec_all_one        ( ec_all_one_f0a_h        ),
  .ea_lo_all_one     ( ea_lo_all_one_f0a_h     ),
  .eb_lo_all_one     ( eb_lo_all_one_f0a_h     ), 
  .sa_hi_clr         ( sa_hi_f0a_h             ),
  .sb_hi_clr         ( sb_hi_f0a_h             ),
  .sc_clr            ( sc_f0a_h                ),
  .ea_hi_clr         ( ea_hi_f0a_h             ),
  .eb_hi_clr         ( eb_hi_f0a_h             ),
  .ec_clr            ( ec_f0a_h                ),  
  .fa_clr            ( fa_clr_f0a_h            ),
  .fb_clr            ( fb_clr_f0a_h            ),
  .fc_clr            ( fc_clr_f0a_h            ),
  .ea_hi_zero_clr    ( ea_hi_zero_clr_f0a_h    ),
  .eb_hi_zero_clr    ( eb_hi_zero_clr_f0a_h    ),
  .ec_zero_clr       ( ec_zero_clr_f0a_h       ),
  .ea_hi_all_one_clr ( ea_hi_all_one_clr_f0a_h ),
  .eb_hi_all_one_clr ( eb_hi_all_one_clr_f0a_h ), 
  .ec_all_one_clr    ( ec_all_one_clr_f0a_h    ),
  .sa_lo_clr         ( sa_lo_f0a_h             ),
  .sb_lo_clr         ( sb_lo_f0a_h             ),
  .ea_lo_clr         ( ea_lo_f0a_h             ),
  .eb_lo_clr         ( eb_lo_f0a_h       	   ),
  .ea_lo_zero_clr    ( ea_lo_zero_clr_f0a_h    ),
  .eb_lo_zero_clr    ( eb_lo_zero_clr_f0a_h    ),
  .ea_lo_all_one_clr ( ea_lo_all_one_clr_f0a_h ),
  .eb_lo_all_one_clr ( eb_lo_all_one_clr_f0a_h )		 
);


//========================================================================================
// Clearing fractional bits in case of denormals or zero
//=======================================================================================
always_comb begin
  fa_f0a_h=fa_clr_f0a_h;
  fb_f0a_h=fb_clr_f0a_h;
  fc_f0a_h=fc_clr_f0a_h;

  case (op_dtype_f0a_h)
    `VPU_DTYPE_F32,`VPU_DTYPE_INT, `VPU_DTYPE_F11, `VPU_DTYPE_F10 : begin
      fa_f0a_h = fa_clr_f0a_h & {$bits(fa_inmux_f0a_h){~ea_hi_zero_clr_f0a_h}};
      fb_f0a_h = fb_clr_f0a_h & {$bits(fb_inmux_f0a_h){~eb_hi_zero_clr_f0a_h}};
      fc_f0a_h = fc_clr_f0a_h & {$bits(fc_inmux_f0a_h){~ec_zero_clr_f0a_h}};
    end
    `VPU_DTYPE_F16 : begin
      fc_f0a_h[31:16] = fc_clr_f0a_h[31:16] & {$bits(fc_clr_f0a_h[31:16]){~ec_zero_clr_f0a_h}};
    end
    `VPU_DTYPE_F16_F32 : begin
      fa_f0a_h[15:0]  = fa_clr_f0a_h[15:0]  & {$bits(fa_clr_f0a_h[15:0]){~ea_lo_zero_clr_f0a_h}};
      fa_f0a_h[31:16] = fa_clr_f0a_h[31:16] & {$bits(fa_clr_f0a_h[31:16]){~ea_hi_zero_clr_f0a_h}};
      fb_f0a_h[15:0]  = fb_clr_f0a_h[15:0]  & {$bits(fb_clr_f0a_h[15:0]){~eb_lo_zero_clr_f0a_h}};
      fb_f0a_h[31:16] = fb_clr_f0a_h[31:16] & {$bits(fb_clr_f0a_h[31:16]){~eb_hi_zero_clr_f0a_h}};

      fc_f0a_h = fc_clr_f0a_h & {$bits(fc_clr_f0a_h){~ec_zero_clr_f0a_h}};
    end

  endcase
end


//===================================================================================================================
// Mantissa Special Detect 
//===================================================================================================================

// operand B is shifted 2 bits right in inmux and takes unmuxed bits 22:16 for trans
always_comb begin

  if (sigs_f0a_h[`TXFMA_TRANS2])
    fb_corrected_f0a_h =  {1'b1, fb_trans_f0a_h, {31-`FLOAT32_M{1'b0}}};
  else
    fb_corrected_f0a_h = fb_clr_f0a_h[`VPU_DATA_S_SZ-3:0] << 2;

end

txfma_special_detect frac_special_detect 
(
  .cmd                 ( cmd_f0a_h                ),
  .sigs                ( sigs_f0a_h               ),
  .op_dtype            ( op_dtype_f0a_h           ),
  .trans_exp_fma2_exc  ( trans_exp_fma2_exc_f0a_h ),
  .opa                 ( fa_clr_f0a_h             ),
  .opb                 ( fb_corrected_f0a_h       ),
  .opc                 ( fc_clr_f0a_h             ),
  .ea_hi_zero          ( ea_hi_zero_clr_f0a_h     ),
  .eb_hi_zero          ( eb_hi_zero_clr_f0a_h     ),
  .ec_zero             ( ec_zero_clr_f0a_h        ),  
  .ea_hi_all_one       ( ea_hi_all_one_clr_f0a_h  ),
  .eb_hi_all_one       ( eb_hi_all_one_clr_f0a_h  ),
  .ec_all_one          ( ec_all_one_clr_f0a_h     ),  
  .sa_hi               ( sa_hi_f0a_h              ),
  .sb_hi               ( sb_hi_f0a_h              ),
  .sc                  ( sc_f0a_h                 ),
  .ea_lo_zero          ( ea_lo_zero_f0a_h         ),
  .eb_lo_zero          ( eb_lo_zero_f0a_h         ),
  .ea_lo_all_one       ( ea_lo_all_one_clr_f0a_h  ),
  .eb_lo_all_one       ( eb_lo_all_one_clr_f0a_h  ),
  .eb_hi_eq_254        ( eb_hi_eq_254_f0a_h       ),
  .eb_hi_eq_253        ( eb_hi_eq_253_f0a_h       ),
  .eb_hi_eq_125        ( eb_hi_eq_125_f0a_h       ),
  .ec_gt_133           ( ec_gt_133_f0a_h          ),
  .ec_eq_133           ( ec_eq_133_f0a_h          ),
  .sa_lo               ( sa_lo_f0a_h              ),
  .sb_lo               ( sb_lo_f0a_h              ),
  .res_inf             ( res_inf_f0a_h            ),
  .res_snan            ( res_snan_f0a_h           ),
  .res_qnan            ( res_qnan_f0a_h           ),
  .classify_c          ( classify_f0a_h           ),
  .prod_inf_hi         ( prod_inf_hi_f0a_h        ),
  .c_inf               ( c_inf_f0a_h              ),
  .a_inf               ( a_inf_f0a_h              ),
  .flags               ( flags_f0a_h              ),
  .a_is_qnan_hi        ( a_is_qnan_hi_f0a_h       ),
  .c_is_qnan           ( c_is_qnan_f0a_h          ),
  .a_is_snan_hi        ( a_is_snan_hi_f0a_h       ),
  .c_is_snan           ( c_is_snan_f0a_h          ),
  .res_pos_int_nan     ( res_pos_int_nan_f0a_h    ),
  .res_neg_int_nan     ( res_neg_int_nan_f0a_h    ),
  .fc_zero30_8         ( fc_zero30_8_f0a_h        ),
  .res_uint_nan        ( res_uint_nan_f0a_h       ),
  .res_zero            ( res_zero_f0a_h           ),
  .res_one             ( res_one_f0a_h            ),
  .prod_inf_lo         ( prod_inf_lo_f0a_h        ),
  .fa_hi_zero          ( fa_hi_zero_f0a_h         ),
  .fb_hi_zero          ( fb_hi_zero_f0a_h         ),
  .res_unorm_one       ( res_unorm_one_f0a_h      ),
  .res_snorm_one       ( res_snorm_one_f0a_h      ),
  .flags_en            ( flags_en_f0a_h           ),
  .spec_res_en         ( spec_res_en_f0a_h        )
);


endmodule
