// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_bypass (
  input [$clog2(`VPU_LANE_NUM)-1:0]           lane_id,
  input                                       clock,   
  // EX/F1 inputs  
  input  logic [2:0][`FREG_ADDR_RANGE]        ex_rf_raddr,
  input  logic [2:0]                          ex_regfile_thread_id,   
  input  logic [2:0][`VPU_DATA_S_SZ-1:0]      ex_rf_rdata,
  input  vpu_input_t                          ex_req_lane,
  input  logic [`VPU_DATA_S_SZ-1:0]           ex_fromint_data,
  input  logic [`VPU_DATA_S_SZ-1:0]           ex_tena_rf_rdata,
  input  logic [`VPU_DATA_S_SZ-1:0]           ex_tenb_rf_rdata,
  input  logic                                ex_tena_regfile_bypass_en,
  input  logic                                ex_tenb_regfile_bypass_en,
  input  vpu_bypass_force_ctrl                ex_bypass_force_ctrl,
  // F3 inputs  
  input  vpu_output_t                         f3_sh_sw_bits,
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f3_regfile_waddr,
  input  logic                                f3_regfile_wmask,  
  input  logic                                f3_thread_id,
  input  logic                                f3_bypass_clk_en,
  input  logic                                f3_data_vector,
  // F4 inputs
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f4_regfile_waddr,
  input  logic                                f4_regfile_wmask,  
  input  logic                                f4_thread_id,
  input  logic                                f4_bypass_clk_en,
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f4_regfile_waddr_l,
  input  logic [1:0]                          f4_regfile_wmask_l,
  input  logic                                f4_regfile_thrid_l,
  input  logic [`VPU_DATA_S_SZ-1:0]           f4_regfile_wdata_l,
  // F5 inputs
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f5_regfile_waddr,
  input  logic                                f5_regfile_wmask,   
  input  logic                                f5_thread_id,  
  input  logic                                f5_bypass_clk_en,
  // F6 inputs
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f6_regfile_waddr,
  input  logic                                f6_regfile_wmask,   
  input  logic                                f6_thread_id,  
  input  logic                                f6_bypass_clk_en,
  // F7 inputs
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f7_regfile_waddr,
  input  logic                                f7_regfile_wmask,   
  input  logic                                f7_thread_id,  
  input  logic                                f7_bypass_clk_en,
  // F8 inputs
  input  logic [$clog2(`VPU_REGFILE_NUM)-1:0] f8_regfile_waddr,
  input  logic                                f8_regfile_wmask,
  input  logic                                f8_thread_id,
  input  vpu_output_t                         f8_txfma_res,
  input  logic                                f8_data_vector,
  input  logic                                f8_txfma_op,
  // EX/F1 outputs  
  output vpu_input_t                          ex_req,
  // F3 outputs  
  output logic                                f3_maskop_bit,
  // F8 outputs  
  output logic [`CORE_FCSR_FLAG_BITS_SZ-1:0]  f8_wflags,
  output logic [`VPU_DATA_S_SZ-1:0]           f8_wdata,
  output logic [`VPU_DATA_S_SZ-1:0]           f8_sh_sw_wdata  
);

vpu_output_t                    f3_bits_mux;
logic [`VPU_DATA_S_SZ-1:0]      f3_wdata;
vpu_output_t                    f4_bits;
logic [`VPU_DATA_S_SZ-1:0]      f4_wdata;
vpu_output_t                    f5_bits;
logic [`VPU_DATA_S_SZ-1:0]      f5_wdata;
vpu_output_t                    f6_bits;
logic [`VPU_DATA_S_SZ-1:0]      f6_wdata;
vpu_output_t                    f7_bits;
logic [`VPU_DATA_S_SZ-1:0]      f7_wdata;
vpu_output_t                    f8_bits_mux;
vpu_output_t                    f8_bits;
logic [2:0][`VPU_DATA_S_SZ-1:0] ex_regfile_rdata;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// EX/F1 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// bypass muxes
logic f4_regfile_wmask_l_muxed; // Selects bit 0 for sources 0 and 1, and bit 1 for source 2. Used
                                // to disable bypass in source 2 for reduce operations (want to read
                                // old value)
always_comb begin
  for(integer i = 0; i < 3; i++) begin
    f4_regfile_wmask_l_muxed = (i == 0) ? f4_regfile_wmask_l[0] : f4_regfile_wmask_l[1];
    ex_regfile_rdata[i] = 
                         //src 0
                           ((i==0) && ex_tena_regfile_bypass_en)                                                                             ? ex_tena_rf_rdata
                         : ((i==0) && ex_bypass_force_ctrl.txfma_in0)                                                                        ? f8_txfma_res.data
                         : ((i==0) && ex_req_lane.sigs.fromint)                                                                              ? ex_fromint_data
                         //src 1
                         : ((i==1) && ex_tenb_regfile_bypass_en)                                                                             ? ex_tenb_rf_rdata
                         : ((i==1) && ex_bypass_force_ctrl.shsw_in1)                                                                         ? f8_bits.data
                         : ((i==1) && ex_bypass_force_ctrl.txfma_in1)                                                                        ? f8_txfma_res.data
                         //src 2                               
                         : ((i==2) && ex_bypass_force_ctrl.shsw_in2)                                                                         ? f8_bits.data
                         : ((i==2) && ex_bypass_force_ctrl.txfma_in2)                                                                        ? f8_txfma_res.data
                         : ((i==2) && ex_req_lane.sigs.fromint & ex_req_lane.sigs.txfma)                                                     ? ex_fromint_data
                         //common                               
                         : (ex_rf_raddr[i]==f4_regfile_waddr_l) && f4_regfile_wmask_l_muxed && (ex_regfile_thread_id[i]==f4_regfile_thrid_l) ? f4_regfile_wdata_l
                         : (ex_rf_raddr[i]==f3_regfile_waddr)   && f3_regfile_wmask         && (ex_regfile_thread_id[i]==f3_thread_id)       ? f3_wdata
                         : (ex_rf_raddr[i]==f4_regfile_waddr)   && f4_regfile_wmask         && (ex_regfile_thread_id[i]==f4_thread_id)       ? f4_wdata
                         : (ex_rf_raddr[i]==f5_regfile_waddr)   && f5_regfile_wmask         && (ex_regfile_thread_id[i]==f5_thread_id)       ? f5_wdata
                         : (ex_rf_raddr[i]==f6_regfile_waddr)   && f6_regfile_wmask         && (ex_regfile_thread_id[i]==f6_thread_id)       ? f6_wdata
                         : (ex_rf_raddr[i]==f7_regfile_waddr)   && f7_regfile_wmask         && (ex_regfile_thread_id[i]==f7_thread_id)       ? f7_wdata
                         : (ex_rf_raddr[i]==f8_regfile_waddr)   && f8_regfile_wmask         && (ex_regfile_thread_id[i]==f8_thread_id)       ? f8_wdata                            
                         :                                                                                                                     ex_rf_rdata[i];
  end
end

    //input operands
always_comb begin
  ex_req                     = ex_req_lane;
  ex_req.in1                 = ex_regfile_rdata[0];
  ex_req.in2                 = ex_regfile_rdata[1];
  ex_req.in3                 = ex_regfile_rdata[2];
end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  //wdata mux
always_comb begin
  f3_bits_mux = f3_sh_sw_bits;

    //clean data for scalar operations
  if (lane_id!=0 && !f3_data_vector)
    f3_bits_mux = '0;
end

assign f3_wdata = f3_bits_mux.data;
assign f3_maskop_bit = f3_bits_mux.data[0];


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F4 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK     EN                     DOUT               DIN
`EN_FF (clock,  f3_bypass_clk_en,      f4_bits,           f3_bits_mux)
assign f4_wdata = f4_bits.data;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F5 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK     EN                     DOUT               DIN
`EN_FF (clock,  f4_bypass_clk_en,      f5_bits,           f4_bits)
assign f5_wdata = f5_bits.data;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F6 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK     EN                     DOUT               DIN
`EN_FF (clock,  f5_bypass_clk_en,      f6_bits,           f5_bits)
assign f6_wdata = f6_bits.data;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F7 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK     EN                     DOUT               DIN
`EN_FF (clock,  f6_bypass_clk_en,      f7_bits,           f6_bits)
assign f7_wdata = f7_bits.data;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F8 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//      CLK     EN                DOUT     DIN
`EN_FF (clock,  f7_bypass_clk_en, f8_bits, f7_bits)

////////////////////////////////////////////////////////////////////////////////
// Select the flags and data from functional modules
////////////////////////////////////////////////////////////////////////////////
always_comb begin
   f8_bits_mux  = f8_bits; //sh-sw by default

     //select txfma result
   if (f8_txfma_op) begin
     f8_bits_mux.data = f8_txfma_res.data;
     f8_bits_mux.exc  = f8_txfma_res.exc;
   end

     //clean data for scalar operations
   if (lane_id!=0 && !f8_data_vector)
     f8_bits_mux.data = '0;
end

assign f8_sh_sw_wdata = f8_bits.data;
assign f8_wdata       = f8_bits_mux.data;
assign f8_wflags      = f8_bits_mux.exc;


endmodule
