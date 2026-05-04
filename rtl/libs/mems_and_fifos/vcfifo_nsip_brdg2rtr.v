// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// cdcfifo_nsip
//
// This module connects two interfaces to cross power domain as well as clock 
// domain. The router interface is at low voltage domain. The bridge interface
// is at high voltage domain. The bus widths are parameterized so that the
// module can be applied in multiple instances. 
//
// This module is meant to support the debug routers.

module vcfifo_nsip_brdg2rtr
#(parameter
   INT_WIDTH = 2, 
   HP_WIDTH = 2,
   QOS_WIDTH = 4,
   MSG_DATA_WIDTH = 128,
   EVT_DATA_WIDTH = 32
)
(
  ////////////////////////////////////////////
  // Debug Router side; low voltage
  ////////////////////////////////////////////
  input                         clk__noc_debug,
  input                         reset_n_system_debug,

  // msg/evt tx
  output [INT_WIDTH - 1:0]      host_int_msg,
  output [INT_WIDTH - 1:0]      host_int_evt,
  output [HP_WIDTH - 1:0]       host_hp_msg,
  output [HP_WIDTH - 1:0]       host_hp_evt,
  output [QOS_WIDTH - 1:0]      host_qos_msg,
  output [QOS_WIDTH - 1:0]      host_qos_evt,
  output                        host_sop_msg,
  output                        host_sop_evt,
  output                        host_eop_msg,
  output                        host_eop_evt,
  output                        host_valid_msg,
  output                        host_valid_evt,
  output [MSG_DATA_WIDTH - 1:0]     host_data_msg,
  output [EVT_DATA_WIDTH - 1:0]     host_data_evt,
  input                         noc_credit_inc_msg,
  input                         noc_credit_inc_evt,

  // msg/evt rx
  input                         noc_sop_msg,
  input                         noc_sop_evt,
  input                         noc_eop_msg,
  input                         noc_eop_evt,
  input                         noc_valid_msg,
  input                         noc_valid_evt,
  input  [MSG_DATA_WIDTH - 1:0]     noc_data_msg,
  input  [EVT_DATA_WIDTH - 1:0]     noc_data_evt,
  output                        host_credit_inc_msg,
  output                        host_credit_inc_evt,

  //////////////////////////////////////
  // Bridge Side; high voltage
  //////////////////////////////////////
  input                         clk_udb,
  input                         reset_udb,

  // msg/evt tx
  input  [INT_WIDTH - 1:0]      ust_out_nsip_msg_dest_int_op,
  input  [INT_WIDTH - 1:0]      ust_out_nsip_evt_dest_int_op,
  input  [HP_WIDTH - 1:0]       ust_out_nsip_msg_dest_hp_op,
  input  [HP_WIDTH - 1:0]       ust_out_nsip_evt_dest_hp_op,
  input  [QOS_WIDTH - 1:0]      ust_out_nsip_msg_xfer_qos_op,
  input  [QOS_WIDTH - 1:0]      ust_out_nsip_evt_xfer_qos_op,
  input                         ust_out_nsip_msg_beat_sop_op,
  input                         ust_out_nsip_evt_beat_sop_op,
  input                         ust_out_nsip_msg_beat_eop_op,
  input                         ust_out_nsip_evt_beat_eop_op,
  input                         ust_out_nsip_msg_beat_valid_op,
  input                         ust_out_nsip_evt_beat_valid_op,
  input  [MSG_DATA_WIDTH - 1:0]     ust_out_nsip_msg_beat_data_op,
  input  [EVT_DATA_WIDTH - 1:0]     ust_out_nsip_evt_beat_data_op,
  output                        ust_out_nsip_msg_credit_inc_ip,
  output                        ust_out_nsip_evt_credit_inc_ip,

  // msg/evt rx
  output                        ust_in_nsip_msg_beat_sop_ip,
  output                        ust_in_nsip_evt_beat_sop_ip,
  output                        ust_in_nsip_msg_beat_eop_ip,
  output                        ust_in_nsip_evt_beat_eop_ip,
  output                        ust_in_nsip_msg_beat_valid_ip,
  output                        ust_in_nsip_evt_beat_valid_ip,
  output [MSG_DATA_WIDTH - 1:0]     ust_in_nsip_msg_beat_data_ip,
  output [EVT_DATA_WIDTH - 1:0]     ust_in_nsip_evt_beat_data_ip,
  input                         ust_in_nsip_msg_credit_inc_op,
  input                         ust_in_nsip_evt_credit_inc_op,

  input                         dft__reset_byp,
  input                         dft__reset
);

// This module includes two sets of vcfifo modules. One set is for messages.
// The other set is for events.
//
// Each set contains two vcfifo modules. One is going from high voltage to low
// voltage, i.e., from the bridge side to the router side. The other is from
// low voltage to high voltage, i.e., from the router side to the bridge side.

//////////////////////////////////////////////////////
// Bridge to Router Message
//////////////////////////////////////////////////////


   // NSIP TX message interface
   vcfifo_wr_hiv_credit_gcd #(
      .ELEM_SIZE(
                 $bits(host_hp_msg)+
                 $bits(host_int_msg)+
                 $bits(host_qos_msg)+
                 $bits(host_sop_msg)+
                 $bits(host_eop_msg)+
                 $bits(host_data_msg))
      ,.NUM_ELEMS(`MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH)
      ,.DOWNSTREAM_CREDITS(`MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH)
      ,.WR_PTR_SYNC_STAGES(`PSHIRE_HI_TO_LO_SYNC_STAGES)
      ,.RD_PTR_SYNC_STAGES(`PSHIRE_LO_TO_HI_SYNC_STAGES)
   ) brdg_to_rtr_msg_tx_fifo (
      .clock_pop (clk__noc_debug),
      .reset_pop (~reset_n_system_debug),
      .clock_push (clk_udb),
      .reset_push (reset_udb),
      .push  (ust_out_nsip_msg_beat_valid_op),   // credit based interface, valid only asserted when want to push
      .push_credit(ust_out_nsip_msg_credit_inc_ip),
      .push_data ({
                   ust_out_nsip_msg_dest_hp_op,
                   ust_out_nsip_msg_dest_int_op,
                   ust_out_nsip_msg_xfer_qos_op,
                   ust_out_nsip_msg_beat_sop_op,
                   ust_out_nsip_msg_beat_eop_op,
                   ust_out_nsip_msg_beat_data_op
                   }),
      .ready  (),
      .valid (host_valid_msg),
      .empty (),
      .pop_data ({
                  host_hp_msg,    // Host ID
                  host_int_msg,   // Interface (wired to 0)
                  host_qos_msg,   // QoS (wired to 0)
                  host_sop_msg,   // Start of packet
                  host_eop_msg,   // End of packet
                  host_data_msg  // Message data
                  }),
      .pop_credit(noc_credit_inc_msg),
      .dft__reset_byp_push ( dft__reset_byp ),
      .dft__reset_push     ( dft__reset ),
      .dft__reset_byp_pop  ( dft__reset_byp ),
      .dft__reset_pop      ( dft__reset )
                            );

   // NSIP RX message interface
   vcfifo_wr_lov_credit_gcd #(
      .ELEM_SIZE(
                 $bits(noc_sop_msg)+
                 $bits(noc_eop_msg)+
                 $bits(noc_data_msg))
      ,.NUM_ELEMS(`MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH)
      ,.DOWNSTREAM_CREDITS(`MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH)
      ,.WR_PTR_SYNC_STAGES(`PSHIRE_LO_TO_HI_SYNC_STAGES)
      ,.RD_PTR_SYNC_STAGES(`PSHIRE_HI_TO_LO_SYNC_STAGES)
   ) brdg_to_rtr_msg_rx_fifo (
      .clock_push (clk__noc_debug),
      .reset_push (~reset_n_system_debug),
      .clock_pop (clk_udb),
      .reset_pop (reset_udb),
      .push(noc_valid_msg),
      .push_data ({
                  noc_sop_msg,   // Start of packet
                  noc_eop_msg,   // End of packet
                  noc_data_msg  // Message data
                  }),
      .push_credit(host_credit_inc_msg),
      .ready  (),
      .empty (),
      .valid  (ust_in_nsip_msg_beat_valid_ip),   // credit based interface, valid only asserted when want to push
      .pop_credit(ust_in_nsip_msg_credit_inc_op),
      .pop_data ({
                   ust_in_nsip_msg_beat_sop_ip,
                   ust_in_nsip_msg_beat_eop_ip,
                   ust_in_nsip_msg_beat_data_ip
                   }),
      .dft__reset_byp_push ( dft__reset_byp ),
      .dft__reset_push     ( dft__reset ),
      .dft__reset_byp_pop  ( dft__reset_byp ),
      .dft__reset_pop      ( dft__reset )
                            );

//////////////////////////////////////////////////////
// Bridge to Router Event
//////////////////////////////////////////////////////

   // NSIP TX event interface
   vcfifo_wr_hiv_credit_gcd #(
      .ELEM_SIZE(
                 $bits(host_hp_evt)+
                 $bits(host_int_evt)+
                 $bits(host_qos_evt)+
                 $bits(host_sop_evt)+
                 $bits(host_eop_evt)+
                 $bits(host_data_evt))
      ,.NUM_ELEMS(`MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH_EVENT)
      ,.DOWNSTREAM_CREDITS(`MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH_EVENT)
      ,.WR_PTR_SYNC_STAGES(`PSHIRE_HI_TO_LO_SYNC_STAGES)
      ,.RD_PTR_SYNC_STAGES(`PSHIRE_LO_TO_HI_SYNC_STAGES)
   ) brdg_to_rtr_evt_tx_fifo (
      .clock_pop (clk__noc_debug),
      .reset_pop (~reset_n_system_debug),
      .clock_push (clk_udb),
      .reset_push (reset_udb),
      .push  (ust_out_nsip_evt_beat_valid_op),   // credit based interface, valid only asserted when want to push
      .push_credit(ust_out_nsip_evt_credit_inc_ip),
      .push_data ({
                   ust_out_nsip_evt_dest_hp_op,
                   ust_out_nsip_evt_dest_int_op,
                   ust_out_nsip_evt_xfer_qos_op,
                   ust_out_nsip_evt_beat_sop_op,
                   ust_out_nsip_evt_beat_eop_op,
                   ust_out_nsip_evt_beat_data_op
                   }),
      .ready  (),
      .valid (host_valid_evt),
      .empty (),
      .pop_data ({
                  host_hp_evt,    // Host ID
                  host_int_evt,   // Interface (wired to 0)
                  host_qos_evt,   // QoS (wired to 0)
                  host_sop_evt,   // Start of packet
                  host_eop_evt,   // End of packet
                  host_data_evt  // Message data
                  }),
      .pop_credit(noc_credit_inc_evt),
      .dft__reset_byp_push ( dft__reset_byp ),
      .dft__reset_push     ( dft__reset ),
      .dft__reset_byp_pop  ( dft__reset_byp ),
      .dft__reset_pop      ( dft__reset )
                            );

   // NSIP RX event interface
   vcfifo_wr_lov_credit_gcd #(
      .ELEM_SIZE(
                 $bits(noc_sop_evt)+
                 $bits(noc_eop_evt)+
                 $bits(noc_data_evt))
      ,.NUM_ELEMS(`MEMSHIRE_NORMAL_BRIDGE_NOC_FLOW_DEPTH_EVENT)
      ,.DOWNSTREAM_CREDITS(`MEMSHIRE_NORMAL_BRIDGE_FLOW_DEPTH_EVENT)
      ,.WR_PTR_SYNC_STAGES(`PSHIRE_LO_TO_HI_SYNC_STAGES)
      ,.RD_PTR_SYNC_STAGES(`PSHIRE_HI_TO_LO_SYNC_STAGES)
   ) brdg_to_rtr_evt_rx_fifo (
      .clock_push (clk__noc_debug),
      .reset_push (~reset_n_system_debug),
      .clock_pop (clk_udb),
      .reset_pop (reset_udb),
      .push(noc_valid_evt),
      .push_data ({
                  noc_sop_evt,   // Start of packet
                  noc_eop_evt,   // End of packet
                  noc_data_evt  // Message data
                  }),
      .push_credit(host_credit_inc_evt),
      .ready  (),
      .empty (),
      .valid  (ust_in_nsip_evt_beat_valid_ip),   // credit based interface, valid only asserted when want to push
      .pop_credit(ust_in_nsip_evt_credit_inc_op),
      .pop_data ({
                   ust_in_nsip_evt_beat_sop_ip,
                   ust_in_nsip_evt_beat_eop_ip,
                   ust_in_nsip_evt_beat_data_ip
                   }),
      .dft__reset_byp_push ( dft__reset_byp ),
      .dft__reset_push     ( dft__reset ),
      .dft__reset_byp_pop  ( dft__reset_byp ),
      .dft__reset_pop      ( dft__reset )
                            );

endmodule
