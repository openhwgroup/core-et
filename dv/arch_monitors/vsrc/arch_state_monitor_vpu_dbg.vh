
   
   // trans u-instructions. Only used for debugging







   //trans u-instructions
   import "DPI-C" function int     dpi_TRANS_FMA1          (input bit [`TRANS_C2_MAX_SIZE-1:0] c2v, input bit [31:0] xv, input bit [`TRANS_C1_MAX_SIZE-1:0] c1v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_SIN_FMA1      (input bit [`TRANS_C2_MAX_SIZE-1:0] c2v, input bit [31:0] xv, input bit [`TRANS_C1_MAX_SIZE-1:0] c1v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_EXP_FMA2      (input bit [31:0]fma1v, input bit [31:0] xv, input bit [`TRANS_C0_MAX_SIZE-1:0] c0v, input bit [`TRANS_MASK_SIZE-1:0] maskv, input bit [0:0] excv);
   import "DPI-C" function int     dpi_TRANS_RCP_FMA2      (input bit [31:0]fma1v, input bit [31:0] xv, input bit [`TRANS_C0_MAX_SIZE-1:0] c0v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_RSQRT_FMA2    (input bit [31:0]fma1v, input bit [31:0] xv, input bit [`TRANS_C0_MAX_SIZE-1:0] c0v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_LOG_FMA2      (input bit [31:0]fma1v, input bit [31:0] xv, input bit [`TRANS_C0_MAX_SIZE-1:0] c0v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_SIN_FMA2      (input bit [31:0]fma1v, input bit [31:0] xv, input bit [`TRANS_C0_MAX_SIZE-1:0] c0v, input bit [`TRANS_MASK_SIZE-1:0] maskv);
   import "DPI-C" function int     dpi_TRANS_LOG_MUL       (input bit [31:0]fma2v, input bit [31:0] cv, input bit [31:0] xv);
   import "DPI-C" function int     dpi_TRANS_SIN_P3        (input bit [31:0] p2v,  input bit [31:0] xv);
   import "DPI-C" function int     dpi_TRANS_SIN_P3_TAYLOR (input bit [31:0] p2v,  input bit [31:0] xv);
   import "DPI-C" function int     dpi_NR1_FXP             (input bit [31:0] x1v,  input bit [31:0] x2v);
   import "DPI-C" function int     dpi_NR2_FXP             (input bit [31:0] nr1v, input bit [31:0] x2v);
   import "DPI-C" function int     dpi_TRANS_LOG_CVT       (input bit [31:0] xv);
   import "DPI-C" function int     dpi_TRANS_SIN_TRANSFORM (input bit [31:0] xv);
   import "DPI-C" function longint dpi_TRANS_EXP_FRAC      (input bit [31:0] xv);
   import "DPI-C" function int     dpi_TRANS_SIN_TAYLOR1   (input bit [31:0] xv);
   import "DPI-C" function int     dpi_TRANS_SIN_TAYLOR2   (input bit [31:0] p1v);
   import "DPI-C" function int     dpi_TRANS_FRAC          (input bit [31:0] xv);
   import "DPI-C" function int     dpi_CVT_FXP1615_FP32    (input bit [31:0] x1v);
   import "DPI-C" function int     dpi_CVT_FP32_FXP1714    (input bit [31:0] x1v);





   
   
   // trans u-instructions. Only used for debugging
   generate
   for (genvar lane=0;lane<`VPU_LANE_NUM;lane++)
   begin
     logic                    trans_valid;
     logic [31:0]             trans_data_in1;
     logic [31:0]             trans_data_in2;
     logic [31:0]             trans_data_in3;
     trans_fma_coefficients   trans_coefficients;
     logic [`VPU_FCMD_SZ-1:0] trans_cmd;
     longint                  trans_expconv;
     logic [31:0]             trans_data_out0, trans_data_out1, trans_data_out2, trans_data_out3, trans_data_out4, trans_data_out5, trans_data_out6, trans_data_out7;


       //inputs from TXFMA
     assign trans_data_in1     = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.in_data.in1;
     assign trans_data_in2     = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.in_data.in2;
     assign trans_data_in3     = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.in_data.in3;
     assign trans_coefficients = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.trans_coefficients;
     assign trans_cmd          = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.in_data.sigs.cmd;
     assign trans_valid        = `MINION_VPU.VPU_LANES[lane].lane.txfma_trans_top.txfma.in_valid;

     always_ff @ (posedge clock)
     begin
       if (trans_valid) begin
         casex(trans_cmd)
           `VPU_TRANS_RCP_FMA1:      trans_data_out0 = dpi_TRANS_FMA1          (trans_coefficients.c2, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_RCP_FMA2:      trans_data_out0 = dpi_TRANS_RCP_FMA2      (trans_data_in1, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_RSQRT_FMA1:    trans_data_out0 = dpi_TRANS_FMA1          (trans_coefficients.c2, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_RSQRT_FMA2:    trans_data_out0 = dpi_TRANS_RSQRT_FMA2    (trans_data_in1, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_LOG_RR:        trans_data_out0 = dpi_TRANS_LOG_CVT       (trans_data_in2);
           `VPU_TRANS_LOG_FMA1:      trans_data_out0 = dpi_TRANS_FMA1          (trans_coefficients.c2, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_LOG_FMA2:      trans_data_out0 = dpi_TRANS_LOG_FMA2      (trans_data_in1, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_LOG_MUL:       trans_data_out0 = dpi_TRANS_LOG_MUL       (trans_data_in1, trans_data_in2, trans_data_in3);
           `VPU_TRANS_SIN_TRANSFORM: trans_data_out0 = dpi_TRANS_SIN_TRANSFORM (trans_data_in3);
           `VPU_FCMD_FFRC:           trans_data_out0 = dpi_TRANS_FRAC          (trans_data_in3);
           `VPU_TRANS_EXP_FRAC:    begin
                                     trans_expconv  = dpi_TRANS_EXP_FRAC      (trans_data_in1);
                                     trans_data_out0 = trans_expconv[31:0];
           end
           `VPU_TRANS_EXP_FMA1:      trans_data_out0 = dpi_TRANS_FMA1          (trans_coefficients.c2, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_EXP_FMA2:      trans_data_out0 = dpi_TRANS_EXP_FMA2      (trans_data_in1, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask, trans_coefficients.exc);
           `VPU_TRANS_SIN_P1:        trans_data_out0 = trans_coefficients.taylor?
                                                  dpi_TRANS_SIN_TAYLOR1       (trans_data_in2) :
                                                  dpi_TRANS_SIN_FMA1          (trans_coefficients.c2, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_SIN_P2:        trans_data_out0 = trans_coefficients.taylor?
                                                  dpi_TRANS_SIN_TAYLOR2       (trans_data_in1) :
                                                  dpi_TRANS_SIN_FMA2          (trans_data_in1, trans_data_in2, trans_coefficients.c0, trans_coefficients.mask);
           `VPU_TRANS_SIN_P3:        trans_data_out0 = trans_coefficients.taylor?
                                                  dpi_TRANS_SIN_P3_TAYLOR     (trans_data_in1, trans_data_in2) :
                                                  dpi_TRANS_SIN_P3            (trans_data_in1, trans_data_in2);
           `VPU_FCMD_CVT_F32RAST:    trans_data_out0 = dpi_CVT_FXP1615_FP32    (trans_data_in1);
           `VPU_FCMD_CVT_RASTF32:    trans_data_out0 = dpi_CVT_FP32_FXP1714    (trans_data_in1);
           `VPU_FCMD_NR1_FRCPFXP:    trans_data_out0 = dpi_NR1_FXP             (trans_data_in1, trans_data_in2);
           `VPU_FCMD_NR2_FRCPFXP:    trans_data_out0 = dpi_NR2_FXP             (trans_data_in1, trans_data_in2);

           default:                  trans_data_out0 = '0;
         endcase
       end
     end

     //      CLK     RST     DOUT              DIN              DEF
     `RST_FF(clock,  reset,  trans_data_out1,  trans_data_out0, '0)
     `RST_FF(clock,  reset,  trans_data_out2,  trans_data_out1, '0)
     `RST_FF(clock,  reset,  trans_data_out3,  trans_data_out2, '0)
     `RST_FF(clock,  reset,  trans_data_out4,  trans_data_out3, '0)
     `RST_FF(clock,  reset,  trans_data_out5,  trans_data_out4, '0)
     `RST_FF(clock,  reset,  trans_data_out6,  trans_data_out5, '0)
     `RST_FF(clock,  reset,  trans_data_out7,  trans_data_out6, '0)
   end
   endgenerate
