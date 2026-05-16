// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vintpipe_csr_msgs_tb.h"
#include "sim_ctrl.h"

#include <cstdio>
#include <cstdint>

using DUT = Vintpipe_csr_msgs_tb;

namespace {

constexpr uint8_t kCsrCmdN = 0;
constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kRspAckData = 1;
constexpr uint8_t kRspMsgRcvData = 2;

void idle_inputs(DUT* d) {
  d->cpu_mem_porthead_en_any_i = 0;
  d->cpu_mem_portheadnb_en_any_i = 0;
  d->cpu_mem_sen_i = 0;
  d->mem_addr_i = 0;
  d->mem_cmd_i = kCsrCmdN;
  d->mem_valid_qual_i = 0;
  d->mem_thread_id_i = 0;
  d->mem_wdata_i = 0;
  d->mpp_i = kPrvM;
  d->cpu_wb_sen_i = 0;
  d->wb_thread_id_i = 0;
  d->wb_wdata_i = 0;
  d->l2_resp_valid_i = 0;
  d->l2_resp_ready_i = 0;
  d->l2_resp_id_i = 0;
  d->l2_resp_dest_i = 0;
  d->l2_resp_wdata_i = 0;
  d->l2_resp_opcode_i = 0;
  d->l2_resp_size_i = 0;
  d->l2_resp_qwen_i = 0;
  d->req_port_info_thread_id_i = 0;
  d->req_port_info_port_id_i = 0;
}

void tick_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

uint64_t cfg_wdata(bool enable, bool enable_oob, bool umode, uint8_t logsize,
                   uint8_t max_msgs, uint8_t scp_set, uint8_t scp_way) {
  return static_cast<uint64_t>(enable) |
         (static_cast<uint64_t>(enable_oob) << 1) |
         (static_cast<uint64_t>(umode) << 4) |
         (static_cast<uint64_t>(logsize & 0x7) << 5) |
         (static_cast<uint64_t>(max_msgs & 0xf) << 8) |
         (static_cast<uint64_t>(scp_set & 0xf) << 16) |
         (static_cast<uint64_t>(scp_way & 0x3) << 24);
}

uint16_t conf_flat(bool umode, uint8_t logsize, uint8_t max_msgs,
                   uint8_t scp_set, uint8_t scp_way, bool enable_oob) {
  return (static_cast<uint16_t>(umode) << 14) |
         (static_cast<uint16_t>(logsize & 0x7) << 11) |
         (static_cast<uint16_t>(max_msgs & 0xf) << 7) |
         (static_cast<uint16_t>(scp_set & 0xf) << 3) |
         (static_cast<uint16_t>(scp_way & 0x3) << 1) |
         static_cast<uint16_t>(enable_oob);
}

uint64_t portctl_value(bool enable, bool enable_oob, bool umode, uint8_t logsize,
                       uint8_t max_msgs, uint8_t scp_set, uint8_t scp_way) {
  return static_cast<uint64_t>(enable) |
         (static_cast<uint64_t>(enable_oob) << 1) |
         (static_cast<uint64_t>(umode) << 4) |
         (static_cast<uint64_t>(logsize & 0x7) << 5) |
         (static_cast<uint64_t>(max_msgs & 0xf) << 8) |
         (1ull << 15) |
         (static_cast<uint64_t>(scp_set & 0xf) << 16) |
         (static_cast<uint64_t>(scp_way & 0x3) << 24);
}

uint8_t sen_bit(uint8_t port, uint8_t thread) {
  return static_cast<uint8_t>(1u << (port * 2 + thread));
}

void check_u64(SimCtrl<DUT>& sim, const char* name, uint64_t got, uint64_t exp) {
  char msg[192];
  std::snprintf(msg, sizeof(msg), "%s got=0x%016llx exp=0x%016llx",
                name, static_cast<unsigned long long>(got),
                static_cast<unsigned long long>(exp));
  sim.check(got == exp, msg);
}

void check_u32(SimCtrl<DUT>& sim, const char* name, uint32_t got, uint32_t exp) {
  char msg[160];
  std::snprintf(msg, sizeof(msg), "%s got=0x%x exp=0x%x", name, got, exp);
  sim.check(got == exp, msg);
}

void configure_port(SimCtrl<DUT>& sim, uint8_t port, uint8_t thread, uint64_t wdata) {
  idle_inputs(sim.dut.get());
  sim.dut->cpu_wb_sen_i = sen_bit(port, thread);
  sim.dut->wb_thread_id_i = thread;
  sim.dut->wb_wdata_i = wdata;
  tick_eval(sim);
  idle_inputs(sim.dut.get());
  sim.dut->eval();
}

void drive_l2_msg(DUT* d, uint8_t port, uint8_t thread, bool oob) {
  d->l2_resp_valid_i = 1;
  d->l2_resp_ready_i = 1;
  d->l2_resp_opcode_i = kRspMsgRcvData;
  d->l2_resp_dest_i = thread;
  d->l2_resp_id_i = static_cast<uint8_t>((port << 3) | (static_cast<uint8_t>(oob) << 2));
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  idle_inputs(sim.dut.get());
  sim.reset(2);
  idle_inputs(sim.dut.get());
  sim.dut->eval();

  check_u32(sim, "reset_pget_block", sim.dut->msg_port_pget_block_o, 0);
  check_u32(sim, "reset_xcpt", sim.dut->msg_port_xcpt_o, 0);
  check_u32(sim, "reset_stall", sim.dut->msg_port_stall_o, 0);
  check_u64(sim, "reset_mem_rdata_default", sim.dut->mem_rdata_next_o, ~0ull);
  check_u32(sim, "reset_req_port_disabled", sim.dut->req_port_info_enabled_o, 0);

  const uint64_t port0_cfg = cfg_wdata(true, true, true, 3, 2, 5, 2);
  configure_port(sim, 0, 0, port0_cfg);
  sim.dut->req_port_info_thread_id_i = 0;
  sim.dut->req_port_info_port_id_i = 0;
  sim.dut->wb_thread_id_i = 0;
  sim.dut->mem_addr_i = 0;
  sim.dut->eval();
  check_u32(sim, "port0_info_enabled", sim.dut->req_port_info_enabled_o, 1);
  check_u32(sim, "port0_info_flat", sim.dut->req_port_info_reply_o,
            conf_flat(true, 3, 2, 5, 2, true));
  check_u32(sim, "port0_info_umode", sim.dut->req_port_info_umode_o, 1);
  check_u32(sim, "port0_info_logsize", sim.dut->req_port_info_logsize_o, 3);
  check_u32(sim, "port0_info_max_msgs", sim.dut->req_port_info_max_msgs_o, 2);
  check_u32(sim, "port0_info_scp_set", sim.dut->req_port_info_scp_set_o, 5);
  check_u32(sim, "port0_info_scp_way", sim.dut->req_port_info_scp_way_o, 2);
  check_u32(sim, "port0_info_oob", sim.dut->req_port_info_enable_oob_o, 1);
  check_u64(sim, "port0_portctl", sim.dut->portctl_o,
            portctl_value(true, true, true, 3, 2, 5, 2));

  configure_port(sim, 1, 0, cfg_wdata(true, false, true, 2, 1, 6, 1));

  idle_inputs(sim.dut.get());
  drive_l2_msg(sim.dut.get(), 0, 0, true);
  sim.dut->eval();
  check_u32(sim, "l2_msg_valid_decode", sim.dut->l2_resp_msg_valid_o, 1);
  check_u32(sim, "l2_msg_wr_en", sim.dut->msg_port_wr_en_reg_next_o, 1);
  check_u32(sim, "l2_msg_scp_way", sim.dut->msg_port_scp_way_reg_next_o, 2);
  check_u32(sim, "l2_msg_wr_addr", sim.dut->msg_port_wr_addr_reg_next_o, (5u << 6) + 32u);
  tick_eval(sim);

  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_porthead_en_any_i = 1;
  sim.dut->mem_thread_id_i = 0;
  sim.dut->mem_addr_i = 0;
  sim.dut->mem_wdata_i = 0b0000;
  sim.dut->mpp_i = kPrvM;
  sim.dut->mem_valid_qual_i = 1;
  sim.dut->eval();
  check_u64(sim, "pget_buffered_rdata", sim.dut->mem_rdata_next_o, 1);
  tick_eval(sim);
  check_u32(sim, "pget_buffered_no_block", sim.dut->msg_port_pget_block_o, 0);
  check_u32(sim, "pget_buffered_no_stall", sim.dut->msg_port_stall_o, 0);

  idle_inputs(sim.dut.get());
  sim.dut->wb_thread_id_i = 0;
  tick_eval(sim);

  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_porthead_en_any_i = 1;
  sim.dut->mem_thread_id_i = 0;
  sim.dut->mem_addr_i = 0;
  sim.dut->mem_wdata_i = 0b0000;
  sim.dut->mpp_i = kPrvM;
  sim.dut->mem_valid_qual_i = 1;
  sim.dut->eval();
  check_u64(sim, "pget_empty_rdata", sim.dut->mem_rdata_next_o, ~0ull);
  tick_eval(sim);
  check_u32(sim, "pget_empty_blocks", sim.dut->msg_port_pget_block_o, 1);
  check_u32(sim, "pget_empty_sets_stall", sim.dut->msg_port_stall_o & 0x1, 0x1);

  idle_inputs(sim.dut.get());
  drive_l2_msg(sim.dut.get(), 0, 0, false);
  tick_eval(sim);
  idle_inputs(sim.dut.get());
  tick_eval(sim);
  check_u32(sim, "l2_message_clears_pget_stall", sim.dut->msg_port_stall_o & 0x1, 0x0);

  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_portheadnb_en_any_i = 1;
  sim.dut->mem_thread_id_i = 0;
  sim.dut->mem_addr_i = 1;
  sim.dut->mem_wdata_i = 0b0001;
  sim.dut->mpp_i = kPrvM;
  sim.dut->mem_valid_qual_i = 1;
  sim.dut->eval();
  check_u64(sim, "pgetnb_empty_rdata", sim.dut->mem_rdata_next_o, ~0ull);
  tick_eval(sim);
  check_u32(sim, "pgetnb_does_not_block", sim.dut->msg_port_pget_block_o, 0);
  check_u32(sim, "pgetnb_does_not_stall", sim.dut->msg_port_stall_o & 0x2, 0x0);

  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_sen_i = sen_bit(1, 0);
  sim.dut->mem_wdata_i = 0b0111;
  sim.dut->mpp_i = kPrvM;
  sim.dut->eval();
  check_u64(sim, "enable_r_mem_rdata_zero", sim.dut->mem_rdata_next_o, 0);
  tick_eval(sim);
  check_u32(sim, "enable_r_no_xcpt_without_head", sim.dut->msg_port_xcpt_o, 0);

  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_sen_i = sen_bit(1, 0);
  sim.dut->mem_wdata_i = 0b0011;
  sim.dut->mpp_i = kPrvM;
  sim.dut->eval();
  check_u64(sim, "disable_mem_rdata_zero", sim.dut->mem_rdata_next_o, 0);
  tick_eval(sim);
  check_u32(sim, "disable_no_xcpt_without_head", sim.dut->msg_port_xcpt_o, 0);

  configure_port(sim, 2, 0, cfg_wdata(true, false, false, 2, 1, 2, 0));
  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_porthead_en_any_i = 1;
  sim.dut->mem_thread_id_i = 0;
  sim.dut->mem_addr_i = 2;
  sim.dut->mem_wdata_i = 0b0000;
  sim.dut->mpp_i = kPrvU;
  tick_eval(sim);
  check_u32(sim, "user_pget_without_umode_xcpt", sim.dut->msg_port_xcpt_o, 1);

  configure_port(sim, 3, 0, cfg_wdata(false, false, true, 2, 1, 3, 0));
  idle_inputs(sim.dut.get());
  sim.dut->cpu_mem_porthead_en_any_i = 1;
  sim.dut->mem_thread_id_i = 0;
  sim.dut->mem_addr_i = 3;
  sim.dut->mem_wdata_i = 0b0000;
  sim.dut->mpp_i = kPrvM;
  tick_eval(sim);
  check_u32(sim, "disabled_port_pget_xcpt", sim.dut->msg_port_xcpt_o, 1);

  idle_inputs(sim.dut.get());
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_ready_i = 1;
  sim.dut->l2_resp_opcode_i = kRspAckData;
  sim.dut->l2_resp_id_i = 0;
  sim.dut->eval();
  check_u32(sim, "l2_ackdata_not_msg", sim.dut->l2_resp_msg_valid_o, 0);

  idle_inputs(sim.dut.get());
  sim.dut->l2_resp_valid_i = 1;
  sim.dut->l2_resp_ready_i = 1;
  sim.dut->l2_resp_opcode_i = kRspMsgRcvData;
  sim.dut->l2_resp_id_i = 0xc0;
  sim.dut->eval();
  check_u32(sim, "l2_reduce_ready_id_not_port_msg", sim.dut->l2_resp_msg_valid_o, 0);

  return sim.finish();
}
