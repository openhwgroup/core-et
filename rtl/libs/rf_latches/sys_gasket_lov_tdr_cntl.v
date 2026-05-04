// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module sys_gasket_lov_tdr_cntl (
  input  logic  use_shire_tdr     ,
  input  logic  reset_warm_in     ,
  input  logic  gpio_ndmreset_in  ,
  input  logic  ndmreset_in       ,
  input  logic  dmactive_in       ,
  input  logic  reset_cold_in     ,
  input  logic  reset_warm_tdr    ,
  input  logic  gpio_ndmreset_tdr ,
  input  logic  ndmreset_tdr      ,
  input  logic  dmactive_tdr      ,
  input  logic  reset_cold_tdr    ,
  output logic  reset_warm_out    ,
  output logic  gpio_ndmreset_out ,
  output logic  ndmreset_out      ,
  output logic  dmactive_out      ,  
  output logic  reset_cold_out     
);              

  assign reset_warm_out    = use_shire_tdr ?  reset_warm_tdr    : reset_warm_in    ;
  assign gpio_ndmreset_out = use_shire_tdr ?  gpio_ndmreset_tdr : gpio_ndmreset_in ;
  assign ndmreset_out      = use_shire_tdr ?  ndmreset_tdr      : ndmreset_in      ;
  assign dmactive_out      = use_shire_tdr ?  dmactive_tdr      : dmactive_in      ;
  assign reset_cold_out    = use_shire_tdr ?  reset_cold_tdr    : reset_cold_in    ;

endmodule
 
