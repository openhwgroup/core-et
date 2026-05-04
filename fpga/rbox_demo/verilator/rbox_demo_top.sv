// Wrapper around rbox_top for the Verilator demo.
//
// Exposes ET-Link and APB packed-struct ports as raw logic vectors.
// The C++ testbench manipulates bits directly; the wrapper casts to/from
// the packed struct types at the module boundary.  This matches the
// approach used in the RTL cosimulation testbenches.

module rbox_demo_top
  import rbox_pkg::*;
  import etlink_pkg::*;
  import apb_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_d_ni,
  input  logic                       rst_w_ni,

  input  logic [ShireIdSize-1:0]     shire_id_i,

  output logic [$bits(req_t)-1:0]    rbox_sc_req_o,
  output logic [ScBanks:0]           rbox_sc_req_valid_o,
  input  logic [ScBanks:0]           rbox_sc_req_ready_i,

  input  logic [$bits(rsp_t)-1:0]    sc_rbox_rsp_i,
  input  logic                       sc_rbox_rsp_valid_i,
  output logic                       sc_rbox_rsp_ready_o,

  input  rc_t                        rc_i,
  output rc_ack_t                    rc_ack_o,

  input  logic [$bits(apb_pkg::req_t)-1:0]  apb_req_i,
  output logic [$bits(apb_pkg::rsp_t)-1:0]  apb_rsp_o,

  input  ram_cfg_t                   ram_cfg_i,
  input  dft_t                       dft_i
);

  etlink_pkg::rsp_t  sc_rbox_rsp_cast;
  etlink_pkg::req_t  rbox_sc_req_cast;
  apb_pkg::req_t     apb_req_cast;
  apb_pkg::rsp_t     apb_rsp_cast;

  assign sc_rbox_rsp_cast = etlink_pkg::rsp_t'(sc_rbox_rsp_i);
  assign rbox_sc_req_o    = $bits(etlink_pkg::req_t)'(rbox_sc_req_cast);
  assign apb_req_cast     = apb_pkg::req_t'(apb_req_i);
  assign apb_rsp_o        = $bits(apb_pkg::rsp_t)'(apb_rsp_cast);

  rbox_top u_rbox (
    .clk_i                (clk_i),
    .rst_d_ni             (rst_d_ni),
    .rst_w_ni             (rst_w_ni),
    .shire_id_i           (shire_id_i),
    .rbox_sc_req_o        (rbox_sc_req_cast),
    .rbox_sc_req_valid_o  (rbox_sc_req_valid_o),
    .rbox_sc_req_ready_i  (rbox_sc_req_ready_i),
    .sc_rbox_rsp_i        (sc_rbox_rsp_cast),
    .sc_rbox_rsp_valid_i  (sc_rbox_rsp_valid_i),
    .sc_rbox_rsp_ready_o  (sc_rbox_rsp_ready_o),
    .rc_i                 (rc_i),
    .rc_ack_o             (rc_ack_o),
    .apb_req_i            (apb_req_cast),
    .apb_rsp_o            (apb_rsp_cast),
    .ram_cfg_i            (ram_cfg_i),
    .dft_i                (dft_i)
  );

endmodule
