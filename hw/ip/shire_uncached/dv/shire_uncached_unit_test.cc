// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vshire_uncached_unit_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

namespace {
constexpr uint32_t ReqWrite = 0;
constexpr uint32_t ReqRead = 1;
constexpr uint32_t ReqMsgSendData = 4;
constexpr uint32_t ReqAtomic = 5;
constexpr uint32_t SizeWord = 2;
constexpr uint32_t SizeDWord = 3;
constexpr uint32_t SizeLine = 6;
constexpr uint32_t RespOkay = 0;
constexpr uint32_t RespSlvErr = 2;
constexpr uint32_t RspAck = 0;
constexpr uint32_t RspAckData = 1;
constexpr uint32_t RspMsgRcvData = 2;
constexpr uint32_t RspErr = 3;

constexpr uint64_t DdrAddr = 0x8000000040ull;
constexpr uint64_t SysAddr = 0x0000001020ull;
constexpr uint64_t BroadcastAddrEsr = 0x013ff5fff0ull;
constexpr uint64_t BroadcastAddrUm = 0x013ff5fff8ull;

uint64_t atomic_response_addr() {
    return (1ull << 32) | (2ull << 30) | (3ull << 20) | (0x3800ull << 6);
}

uint64_t flb_req(unsigned barrier, unsigned limit) {
    return ((uint64_t)limit << 5) | barrier;
}

void eval(Vshire_uncached_unit_tb *d) { d->eval(); }

void init_inputs(Vshire_uncached_unit_tb *d) {
    d->fcc_esr_enables_i = 0;
    d->fcc_esr_wdata_i = 0;
    d->flb_esr_enables_i = 0;
    d->flb_esr_wdata_i = 0;
    d->flb_req_valid_i = 0;
    d->flb_req_data_flat_i = 0;

    d->ax_qos_dram_i = 0;
    d->ax_wr_valid_i = 0;
    d->ax_wr_l3_i = 0;
    d->ax_wr_opcode_i = ReqWrite;
    d->ax_wr_size_i = SizeDWord;
    d->ax_wr_addr_i = SysAddr;
    d->ax_wr_axi_id_i = 0x12;
    d->ax_wr_data_lo_i = 0;
    d->ax_wr_atomic_subop_i = 0;
    d->ax_rd_valid_i = 0;
    d->ax_rd_l3_i = 0;
    d->ax_rd_opcode_i = ReqRead;
    d->ax_rd_size_i = SizeDWord;
    d->ax_rd_addr_i = SysAddr;
    d->ax_rd_axi_id_i = 0x13;
    d->ax_to_sys_ar_ready_i = 1;
    d->ax_to_sys_aw_ready_i = 1;
    d->ax_to_sys_w_ready_i = 1;
    d->ax_to_l3_ar_ready_i = 1;
    d->ax_to_l3_aw_ready_i = 1;
    d->ax_to_l3_w_ready_i = 1;

    d->arb_bid_i = 0;
    d->arb_stall_i = 0;

    d->rsp_req_id_i = 0x55;
    d->rsp_req_source_i = 3;
    d->rsp_req_port_id_i = 2;
    d->rsp_req_trans_id_i = 0x15;
    d->rsp_req_resp_i = 1;
    d->rsp_req_opcode_i = ReqWrite;
    d->rsp_req_size_i = SizeDWord;
    d->rsp_req_addr_i = DdrAddr;
    d->rsp_reqq_valid_i = 1;

    d->slv_credit_i = 1;
    d->slv_aw_valid_i = 0;
    d->slv_w_valid_i = 0;
    d->slv_aw_id_i = 0x12345;
    d->slv_aw_addr_i = atomic_response_addr();
    d->slv_aw_len_i = 0;
    d->slv_aw_size_i = SizeDWord;
    d->slv_aw_user_i = 7;
    d->slv_w_data_lo_i = 0;

    d->tol3_l3_enabled_i = 1;
    d->tol3_remote_scp_enabled_i = 1;
    d->tol3_b_valid_i = 0;
    d->tol3_b_id_i = 0;
    d->tol3_b_resp_i = RespOkay;
    d->tol3_r_valid_i = 0;
    d->tol3_r_id_i = 0;
    d->tol3_r_resp_i = RespOkay;
    d->tol3_r_data_lo_i = 0;
    d->tol3_r_data_hi_i = 0;

    d->tosys_b_valid_i = 0;
    d->tosys_b_id_i = 0;
    d->tosys_b_resp_i = RespOkay;
    d->tosys_r_valid_i = 0;
    d->tosys_r_id_i = 0;
    d->tosys_r_resp_i = RespOkay;
    d->tosys_r_data_lo_i = 0;
    d->tosys_r_data_hi_i = 0;
    d->tosys_bcast_req_index_i = 0;
    d->tosys_bcast_error_reg_i = 0;
    d->tosys_bcast_acks_i = 1;

    d->nr_rsp_send_i = 0;
    d->nr_credit_i = 1;
    d->nr_from_sys_b_ready_i = 1;
    d->nr_from_sys_aw_valid_i = 0;
    d->nr_from_sys_w_valid_i = 0;
    d->nr_to_l3_b_valid_i = 0;
    d->nr_to_l3_r_valid_i = 0;
    d->nr_to_sys_b_valid_i = 0;
    d->nr_to_sys_r_valid_i = 0;

    d->hi_logic_l3_ar_ready_i = 0;
    d->hi_logic_l3_ar_valid_i = 0;
    d->hi_logic_l3_aw_ready_i = 0;
    d->hi_logic_l3_w_ready_i = 0;
    d->hi_logic_l3_aw_valid_i = 0;
    d->hi_logic_l3_w_valid_i = 0;
    d->hi_logic_sys_ar_ready_i = 0;
    d->hi_logic_sys_ar_valid_i = 0;
    d->hi_logic_sys_b_ready_i = 0;
    d->hi_logic_sys_b_valid_i = 0;
    d->hi_logic_sys_b_bits_i = 0;

    d->lo_logic_sbm_aw_fifo_valid_i = 0;
    d->lo_logic_sbm_w_fifo_valid_i = 0;
    d->lo_logic_uc_aw_fifo_valid_i = 0;
    d->lo_logic_uc_w_fifo_valid_i = 0;
    d->lo_logic_aw_valid_i = 0;
    d->lo_logic_w_valid_i = 0;
    d->lo_logic_aw_vcvalid_i = 0;
    d->lo_logic_w_vcvalid_i = 0;
    d->lo_logic_sbm_aw_ready_i = 0;
    d->lo_logic_sbm_w_ready_i = 0;
    d->lo_logic_uc_aw_ready_i = 0;
    d->lo_logic_uc_w_ready_i = 0;
    d->lo_logic_sbm_aw_fifo_ready_i = 0;
    d->lo_logic_uc_aw_fifo_ready_i = 0;
    d->lo_logic_sbm_w_fifo_ready_i = 0;
    d->lo_logic_uc_w_fifo_ready_i = 0;

    d->cross_dft_hv_scanmode_i = 0;
    d->cross_dft_hv_scan_reset_ni = 1;
    d->cross_dft_lv_scanmode_i = 0;
    d->cross_dft_lv_scan_reset_ni = 1;
    d->hi_cross_ar_valid_i = 0;
    d->hi_cross_ar_addr_i = 0;
    d->hi_cross_ar_id_i = 0;
    d->hi_cross_ar_ready_lo_i = 0;
    d->hi_cross_sys_b_valid_i = 0;
    d->hi_cross_sys_b_id_i = 0;
    d->hi_cross_sys_b_resp_i = RespOkay;
    d->hi_cross_sys_b_ready_lo_i = 0;
    d->hi_cross_credit_i = 0;
    d->lo_cross_l3_b_valid_lo_i = 0;
    d->lo_cross_l3_b_id_lo_i = 0;
    d->lo_cross_l3_b_resp_lo_i = RespOkay;
    d->lo_cross_l3_b_ready_i = 0;
    d->lo_cross_ar_valid_lo_i = 0;
    d->lo_cross_ar_addr_lo_i = 0;
    d->lo_cross_ar_id_lo_i = 0;
    d->lo_cross_ar_ready_i = 0;

    d->noc_shire_id_i = 0x23;
    d->noc_req_valid_i = 0;
    d->noc_req_wdata_i = 0;
    d->noc_req_opcode_i = ReqRead;
    d->noc_req_size_i = SizeDWord;
    d->noc_req_addr_i = SysAddr;
    d->noc_req_data_lo_i = 0;
    d->noc_req_subopcode_i = 0;
    d->noc_req_id_i = 0x31;
    d->noc_req_source_i = 2;
    d->noc_req_port_id_i = 1;
    d->noc_req_trans_id_i = 0x21;
    d->noc_credit_i = 1;
    d->noc_l3_enabled_i = 1;
    d->noc_remote_scp_enabled_i = 1;
    d->noc_to_l3_ar_ready_i = 1;
    d->noc_to_l3_aw_ready_i = 1;
    d->noc_to_l3_w_ready_i = 1;
    d->noc_to_sys_ar_ready_i = 1;
    d->noc_to_sys_aw_ready_i = 1;
    d->noc_to_sys_w_ready_i = 1;
    d->noc_rsp_ready_i = 0x1f;

    d->top_fcc_enables_i = 0;
    d->top_esr_wdata_i = 0;
}

bool wait_for_hi_ar(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.tick();
        if (sim.dut->hi_cross_ar_valid_lo_o) return true;
    }
    return false;
}

bool wait_for_hi_b(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.tick();
        if (sim.dut->hi_cross_sys_b_valid_lo_o) return true;
    }
    return false;
}

bool wait_for_lo_b(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.tick();
        if (sim.dut->lo_cross_l3_b_valid_o) return true;
    }
    return false;
}

bool wait_for_lo_ar(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.tick();
        if (sim.dut->lo_cross_ar_valid_o) return true;
    }
    return false;
}

bool wait_for_noc_sys_aw(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.dut->eval();
        if (sim.dut->noc_to_sys_aw_valid_o) return true;
        sim.tick();
    }
    return false;
}

bool wait_for_noc_l3_ar(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.dut->eval();
        if (sim.dut->noc_to_l3_ar_valid_o) return true;
        sim.tick();
    }
    return false;
}

bool wait_for_noc_rsp(SimCtrl<Vshire_uncached_unit_tb> &sim, int cycles) {
    for (int i = 0; i < cycles; ++i) {
        sim.dut->eval();
        if (sim.dut->noc_rsp_valid_o) return true;
        sim.tick();
    }
    return false;
}
}  // namespace

int main(int argc, char **argv) {
    SimCtrl<Vshire_uncached_unit_tb> sim(argc, argv);
    auto *d = sim.dut.get();
    init_inputs(d);
    sim.reset();

    // FCC combinational ESR fanout.
    d->fcc_esr_wdata_i = 0xa5a55a5aull;
    d->fcc_esr_enables_i = 1;
    eval(d);
    sim.check(d->fcc_flat_o == 0xa5a55a5au, "FCC register 0 forwards ESR data");
    sim.check(d->fcc_target_o == 0, "FCC register 0 target");
    d->fcc_esr_enables_i = 4;
    eval(d);
    sim.check(d->fcc_flat_o == 0xa5a55a5au, "FCC register 2 forwards ESR data");
    sim.check(d->fcc_target_o == 2, "FCC register 2 target");
    d->fcc_esr_enables_i = 0;
    eval(d);
    sim.check(d->fcc_flat_o == 0, "FCC clears when no ESR enable is set");

    // FLB ESR update and two requests to the same barrier.
    d->flb_esr_wdata_i = 0;
    d->flb_esr_enables_i = 1;
    sim.tick();
    d->flb_esr_enables_i = 0;
    d->flb_req_data_flat_i = flb_req(0, 1);
    d->flb_req_valid_i = 1;
    sim.tick();
    d->flb_req_valid_i = 0;
    bool saw_first = false;
    for (int i = 0; i < 8; ++i) {
        sim.tick();
        if (d->flb_resp_valid_o & 0x1) {
            saw_first = true;
            sim.check((d->flb_resp_data_o & 0x1) == 0, "FLB first arrival before limit returns 0");
            break;
        }
    }
    sim.check(saw_first, "FLB produces first response");
    sim.check(d->flb_barrier0_o == 1, "FLB increments barrier after first arrival");
    d->flb_req_valid_i = 1;
    sim.tick();
    d->flb_req_valid_i = 0;
    bool saw_second = false;
    for (int i = 0; i < 8; ++i) {
        sim.tick();
        if (d->flb_resp_valid_o & 0x1) {
            saw_second = true;
            sim.check((d->flb_resp_data_o & 0x1) == 1, "FLB second arrival releases barrier");
            break;
        }
    }
    sim.check(saw_second, "FLB produces second response");
    sim.check(d->flb_barrier0_o == 0, "FLB clears barrier after release");

    // ET-Link to AXI conversion: SYS write, L3 read, atomic write.
    d->ax_qos_dram_i = 1;
    d->ax_wr_valid_i = 1;
    d->ax_wr_l3_i = 0;
    d->ax_wr_opcode_i = ReqWrite;
    d->ax_wr_size_i = SizeDWord;
    d->ax_wr_addr_i = DdrAddr + 8;
    d->ax_wr_axi_id_i = 0x1a;
    d->ax_wr_data_lo_i = 0x1122334455667788ull;
    eval(d);
    sim.check(d->ax_wr_ready_sys_o == 1, "AXI SYS write ready follows AW/W ready");
    sim.check(d->ax_to_sys_aw_valid_o == 1 && d->ax_to_l3_aw_valid_o == 0, "AXI SYS AW selected for l3_en=0");
    sim.check(d->ax_to_sys_aw_addr_o == DdrAddr + 8, "AXI SYS AW address");
    sim.check(d->ax_to_sys_aw_id_o == 0x1a, "AXI SYS AW ID");
    sim.check(d->ax_to_sys_aw_qos_o == 1, "AXI SYS DDR write QOS uses config");
    sim.check(d->ax_to_sys_w_valid_o == 1 && d->ax_to_sys_w_last_o == 1, "AXI SYS W valid/last");
    sim.check(d->ax_to_sys_w_strb_o == 0xff00ull, "AXI write strobe shifts by byte address");
    sim.check(d->ax_to_sys_w_data_lo_o == 0, "AXI write data shifts out of low word for byte offset 8");

    d->ax_wr_valid_i = 0;
    d->ax_rd_valid_i = 1;
    d->ax_rd_l3_i = 1;
    d->ax_rd_opcode_i = ReqRead;
    d->ax_rd_size_i = SizeWord;
    d->ax_rd_addr_i = SysAddr;
    d->ax_rd_axi_id_i = 0x1b;
    eval(d);
    sim.check(d->ax_rd_ready_l3_o == 1, "AXI L3 read ready follows AR ready");
    sim.check(d->ax_to_l3_ar_valid_o == 1 && d->ax_to_sys_ar_valid_o == 0, "AXI L3 AR selected for l3_en=1");
    sim.check(d->ax_to_l3_ar_addr_o == SysAddr, "AXI L3 AR address");
    sim.check(d->ax_to_l3_ar_id_o == 0x1b, "AXI L3 AR ID");
    sim.check(d->ax_to_l3_ar_qos_o == 3, "AXI non-DDR read uses ESR QOS");

    d->ax_rd_valid_i = 0;
    d->ax_wr_valid_i = 1;
    d->ax_wr_l3_i = 1;
    d->ax_wr_opcode_i = ReqAtomic;
    d->ax_wr_size_i = SizeWord;
    d->ax_wr_addr_i = SysAddr;
    d->ax_wr_atomic_subop_i = 0xb;
    eval(d);
    sim.check(d->ax_to_l3_aw_valid_o == 1, "AXI atomic routes to L3 when l3_en=1");
    sim.check(d->ax_to_l3_aw_size_o == SizeLine, "AXI atomic write expands AW size to line");
    sim.check(d->ax_to_l3_w_strb_o == 0x0000000f00000f0full, "AXI atomic compare strobe pattern");
    d->ax_wr_valid_i = 0;

    // Grant-visible-on-stall arbiter.
    d->arb_bid_i = 0x3;
    d->arb_stall_i = 0;
    eval(d);
    sim.check(d->arb_grant_o == 0x1, "LRU grant reset priority selects client 0");
    sim.tick();
    d->arb_bid_i = 0x3;
    d->arb_stall_i = 1;
    eval(d);
    sim.check(d->arb_grant_o == 0x2, "LRU grant remains visible while stalled");
    uint8_t stalled_grant = d->arb_grant_o;
    sim.tick();
    eval(d);
    sim.check(d->arb_grant_o == stalled_grant, "LRU stall freezes priority update");
    d->arb_stall_i = 0;
    sim.tick();
    eval(d);
    sim.check(d->arb_grant_o == 0x1, "LRU priority updates after stall releases");

    // Response handler behavior.
    d->rsp_req_id_i = 0x42;
    d->rsp_req_source_i = 5;
    d->rsp_req_port_id_i = 3;
    d->rsp_req_trans_id_i = 0x2a;
    d->rsp_req_resp_i = 1;
    d->rsp_req_opcode_i = ReqWrite;
    d->rsp_req_size_i = SizeDWord;
    d->rsp_req_addr_i = DdrAddr;
    d->slv_credit_i = 1;
    d->slv_aw_valid_i = 1;
    d->slv_w_valid_i = 1;
    d->slv_aw_addr_i = atomic_response_addr();
    d->slv_aw_user_i = 9;
    d->slv_w_data_lo_i = 0xdeadbeefcafef00dull;
    eval(d);
    sim.check(d->slv_b_valid_o == 1 && d->slv_b_resp_o == RespOkay, "SYS-slave atomic B response is OKAY");
    sim.check(d->slv_reqq_index_o == 9, "SYS-slave request index comes from AW user");
    sim.check(d->slv_new_rsp_o == 1, "SYS-slave atomic generates neighborhood response");
    sim.check(d->slv_rsp_opcode_o == RspAckData, "SYS-slave atomic response opcode");
    sim.check(d->slv_rsp_id_o == 0x42 && d->slv_rsp_dest_o == 5, "SYS-slave atomic response routing");
    sim.check(d->slv_rsp_data_lo_o == 0xdeadbeefcafef00dull, "SYS-slave atomic response data");
    sim.check(d->slv_clear_wait_from_sys_o == 1, "SYS-slave atomic waits for system completion");

    d->tol3_b_valid_i = 1;
    d->tol3_b_resp_i = RespOkay;
    d->rsp_req_opcode_i = ReqWrite;
    d->rsp_req_addr_i = DdrAddr;
    eval(d);
    sim.check(d->tol3_wr_new_rsp_o == 1, "L3 B response generates write response");
    sim.check(d->tol3_wr_rsp_opcode_o == RspAck, "L3 OKAY B maps to ACK");
    sim.check(d->tol3_wr_clear_wait_ack_o == 1, "L3 B clears wait_ack");
    d->tol3_b_valid_i = 0;
    d->tol3_r_valid_i = 1;
    d->tol3_r_data_lo_i = 0x0102030405060708ull;
    d->rsp_req_opcode_i = ReqRead;
    eval(d);
    sim.check(d->tol3_rd_new_rsp_o == 1, "L3 R response generates read response");
    sim.check(d->tol3_rd_rsp_opcode_o == RspAckData, "L3 OKAY R maps to ACKDATA");
    sim.check(d->tol3_rd_rsp_data_lo_o == 0x0102030405060708ull, "L3 R data forwarding");
    d->tol3_r_valid_i = 0;

    d->tosys_b_valid_i = 1;
    d->tosys_b_resp_i = RespSlvErr;
    d->rsp_req_opcode_i = ReqWrite;
    d->rsp_req_addr_i = SysAddr;
    eval(d);
    sim.check(d->tosys_wr_new_rsp_o == 1, "SYS B response generates write response");
    sim.check(d->tosys_wr_rsp_opcode_o == RspErr, "SYS SLVERR B maps to ERR");
    d->tosys_b_id_i = 40;
    d->tosys_bcast_acks_i = 1;
    eval(d);
    sim.check(d->tosys_bcast_new_ack_o == 1 && d->tosys_bcast_done_o == 1, "SYS broadcast B ack/done asserted on last ack");
    d->tosys_b_valid_i = 0;
    d->tosys_r_valid_i = 1;
    d->tosys_r_resp_i = RespOkay;
    d->tosys_r_data_lo_i = 0x8899aabbccddeeffull;
    d->rsp_req_opcode_i = ReqRead;
    eval(d);
    sim.check(d->tosys_rd_new_rsp_o == 1, "SYS R response generates read response");
    sim.check(d->tosys_rd_rsp_opcode_o == RspAckData, "SYS OKAY R maps to ACKDATA");
    sim.check(d->tosys_rd_rsp_data_lo_o == 0x8899aabbccddeeffull, "SYS R data forwarding");
    d->tosys_r_valid_i = 0;

    // Response arbiter/FIFO wrapper around the handlers.
    d->rsp_req_opcode_i = ReqWrite;
    d->rsp_req_addr_i = DdrAddr;
    d->tol3_b_valid_i = 0;
    d->nr_to_l3_b_valid_i = 1;
    d->nr_rsp_send_i = 0;
    sim.tick();
    d->nr_to_l3_b_valid_i = 0;
    bool nr_valid = false;
    for (int i = 0; i < 6; ++i) {
        eval(d);
        if (d->nr_rsp_valid_o) { nr_valid = true; break; }
        sim.tick();
    }
    sim.check(nr_valid, "NoC response arbiter pushes selected L3 B response into FIFO");
    sim.check(d->nr_rsp_opcode_o == RspAck, "NoC response FIFO carries ACK opcode");
    d->nr_rsp_send_i = 1;
    sim.tick();
    d->nr_rsp_send_i = 0;

    // LV helper logic for both crossing directions.
    d->hi_logic_l3_ar_ready_i = 1;
    d->hi_logic_l3_ar_valid_i = 1;
    d->hi_logic_l3_aw_ready_i = 1;
    d->hi_logic_l3_w_ready_i = 0;
    d->hi_logic_l3_aw_valid_i = 1;
    d->hi_logic_l3_w_valid_i = 1;
    d->hi_logic_sys_ar_ready_i = 0;
    d->hi_logic_sys_ar_valid_i = 1;
    d->hi_logic_sys_b_ready_i = 1;
    d->hi_logic_sys_b_valid_i = 1;
    d->hi_logic_sys_b_bits_i = 0x15555;
    eval(d);
    sim.check(d->hi_logic_l3_ar_valid_o == 1 && d->hi_logic_l3_ar_pop_o == 1, "HI-cross LV AR valid/pop handshake");
    sim.check(d->hi_logic_l3_aw_valid_o == 1 && d->hi_logic_l3_aw_pop_o == 0, "HI-cross LV AW waits for paired W ready");
    sim.check(d->hi_logic_l3_w_valid_o == 1 && d->hi_logic_l3_w_pop_o == 0, "HI-cross LV W waits for paired AW ready");
    sim.check(d->hi_logic_sys_ar_valid_o == 1 && d->hi_logic_sys_ar_pop_o == 0, "HI-cross LV SYS AR valid without ready does not pop");
    sim.check(d->hi_logic_sys_b_valid_o == 1 && d->hi_logic_sys_b_pop_o == 1, "HI-cross LV SYS B valid/pop handshake");
    sim.check(d->hi_logic_sys_b_bits_o == 0x15555, "HI-cross LV SYS B payload forwards unmodified");

    d->lo_logic_sbm_aw_fifo_valid_i = 1;
    d->lo_logic_sbm_w_fifo_valid_i = 1;
    d->lo_logic_uc_aw_fifo_valid_i = 1;
    d->lo_logic_uc_w_fifo_valid_i = 1;
    d->lo_logic_aw_valid_i = 1;
    d->lo_logic_w_valid_i = 1;
    d->lo_logic_aw_vcvalid_i = 0x3;
    d->lo_logic_w_vcvalid_i = 0x1;
    d->lo_logic_sbm_aw_ready_i = 1;
    d->lo_logic_sbm_w_ready_i = 1;
    d->lo_logic_uc_aw_ready_i = 1;
    d->lo_logic_uc_w_ready_i = 1;
    d->lo_logic_sbm_aw_fifo_ready_i = 1;
    d->lo_logic_uc_aw_fifo_ready_i = 0;
    d->lo_logic_sbm_w_fifo_ready_i = 0;
    d->lo_logic_uc_w_fifo_ready_i = 1;
    eval(d);
    sim.check(d->lo_logic_sbm_aw_valid_o == 1 && d->lo_logic_sbm_aw_push_o == 1, "LO-cross LV SBM AW valid/push");
    sim.check(d->lo_logic_sbm_w_push_o == 0, "LO-cross LV SBM W push follows VC bit 1");
    sim.check(d->lo_logic_uc_w_push_o == 1, "LO-cross LV UC W push follows VC bit 0 and both ready signals");
    sim.check(d->lo_logic_aw_ready_o == 1 && d->lo_logic_w_ready_o == 1, "LO-cross LV ready ORs destination FIFO readiness");
    sim.check(d->lo_logic_sbm_aw_pop_o == 1 && d->lo_logic_sbm_w_pop_o == 1, "LO-cross LV SBM paired pop");
    sim.check(d->lo_logic_uc_aw_pop_o == 1 && d->lo_logic_uc_w_pop_o == 1, "LO-cross LV UC paired pop");

    // Full voltage crossing FIFOs plus DFT reset bypass.
    d->cross_dft_hv_scanmode_i = 1;
    d->cross_dft_hv_scan_reset_ni = 0;
    eval(d);
    sim.check(d->hi_cross_ar_ready_o == 0 && d->hi_cross_sys_b_ready_o == 0, "HI-cross HV DFT scan reset inhibits write-side ready");
    d->cross_dft_hv_scanmode_i = 0;
    d->cross_dft_hv_scan_reset_ni = 1;
    d->cross_dft_lv_scanmode_i = 1;
    d->cross_dft_lv_scan_reset_ni = 0;
    eval(d);
    sim.check(d->hi_cross_ar_valid_lo_o == 0, "HI-cross LV DFT scan reset inhibits read-side valid");
    d->cross_dft_lv_scanmode_i = 0;
    d->cross_dft_lv_scan_reset_ni = 1;

    d->hi_cross_ar_addr_i = 0x123456789ull;
    d->hi_cross_ar_id_i = 0x12;
    d->hi_cross_ar_ready_lo_i = 0;
    d->hi_cross_ar_valid_i = 1;
    sim.tick();
    d->hi_cross_ar_valid_i = 0;
    bool hi_ar_seen = wait_for_hi_ar(sim, 12);
    sim.check(hi_ar_seen, "HI-cross transfers L3 AR to LV side");
    sim.check(d->hi_cross_ar_addr_lo_o == 0x123456789ull && d->hi_cross_ar_id_lo_o == 0x12, "HI-cross preserves L3 AR payload");
    d->hi_cross_ar_ready_lo_i = 1;
    sim.tick();
    d->hi_cross_ar_ready_lo_i = 0;

    d->hi_cross_sys_b_id_i = 0x15555;
    d->hi_cross_sys_b_resp_i = RespSlvErr;
    d->hi_cross_sys_b_ready_lo_i = 0;
    d->hi_cross_sys_b_valid_i = 1;
    d->hi_cross_credit_i = 1;
    sim.tick();
    d->hi_cross_sys_b_valid_i = 0;
    d->hi_cross_credit_i = 0;
    bool hi_b_seen = wait_for_hi_b(sim, 12);
    sim.check(hi_b_seen, "HI-cross transfers SYS B to LV side");
    sim.check(d->hi_cross_sys_b_id_lo_o == 0x15555 && d->hi_cross_sys_b_resp_lo_o == RespSlvErr, "HI-cross preserves SYS B payload");
    sim.check((d->hi_cross_credit_lo_o & 1) == 1, "HI-cross transfers UC credit pulse");
    d->hi_cross_sys_b_ready_lo_i = 1;
    sim.tick();
    d->hi_cross_sys_b_ready_lo_i = 0;

    d->cross_dft_lv_scanmode_i = 1;
    d->cross_dft_lv_scan_reset_ni = 0;
    eval(d);
    sim.check(d->lo_cross_l3_b_ready_lo_o == 0 && d->lo_cross_ar_ready_lo_o == 0, "LO-cross LV DFT scan reset inhibits write-side ready");
    d->cross_dft_lv_scanmode_i = 0;
    d->cross_dft_lv_scan_reset_ni = 1;
    d->cross_dft_hv_scanmode_i = 1;
    d->cross_dft_hv_scan_reset_ni = 0;
    eval(d);
    sim.check(d->lo_cross_l3_b_valid_o == 0 && d->lo_cross_ar_valid_o == 0, "LO-cross HV DFT scan reset inhibits read-side valid");
    d->cross_dft_hv_scanmode_i = 0;
    d->cross_dft_hv_scan_reset_ni = 1;

    d->lo_cross_l3_b_id_lo_i = 0x33;
    d->lo_cross_l3_b_resp_lo_i = RespSlvErr;
    d->lo_cross_l3_b_ready_i = 0;
    d->lo_cross_l3_b_valid_lo_i = 1;
    sim.tick();
    d->lo_cross_l3_b_valid_lo_i = 0;
    bool lo_b_seen = wait_for_lo_b(sim, 12);
    sim.check(lo_b_seen, "LO-cross transfers L3 B to HV side");
    sim.check(d->lo_cross_l3_b_id_o == 0x33 && d->lo_cross_l3_b_resp_o == RespSlvErr, "LO-cross preserves L3 B payload");
    d->lo_cross_l3_b_ready_i = 1;
    sim.tick();
    d->lo_cross_l3_b_ready_i = 0;

    d->lo_cross_ar_addr_lo_i = 0x223344556ull;
    d->lo_cross_ar_id_lo_i = 0x12345;
    d->lo_cross_ar_ready_i = 0;
    d->lo_cross_ar_valid_lo_i = 1;
    sim.tick();
    d->lo_cross_ar_valid_lo_i = 0;
    bool lo_ar_seen = wait_for_lo_ar(sim, 12);
    sim.check(lo_ar_seen, "LO-cross transfers SYS AR to HV side");
    sim.check(d->lo_cross_ar_addr_o == 0x223344556ull && d->lo_cross_ar_id_o == 0x12345, "LO-cross preserves SYS AR payload");
    d->lo_cross_ar_ready_i = 1;
    sim.tick();
    d->lo_cross_ar_ready_i = 0;

    // Uncached NoC directed unit behavior: unicast SYS write, L3 read, decode-error response,
    // and broadcast ESR/data sequence.
    d->noc_req_wdata_i = 1;
    d->noc_req_opcode_i = ReqWrite;
    d->noc_req_size_i = SizeDWord;
    d->noc_req_addr_i = SysAddr;
    d->noc_req_data_lo_i = 0xaabbccdd00112233ull;
    d->noc_req_valid_i = 1;
    sim.tick();
    d->noc_req_valid_i = 0;
    bool sys_aw_seen = wait_for_noc_sys_aw(sim, 12);
    sim.check(sys_aw_seen, "Uncached NoC emits SYS AW for unicast non-L3 write");
    sim.check(d->noc_to_sys_aw_addr_o == SysAddr, "Uncached NoC SYS AW address for unicast write");
    sim.tick();

    d->noc_req_wdata_i = 0;
    d->noc_req_opcode_i = ReqRead;
    d->noc_req_size_i = SizeDWord;
    d->noc_req_addr_i = DdrAddr;
    d->noc_req_valid_i = 1;
    sim.tick();
    d->noc_req_valid_i = 0;
    bool l3_ar_seen = wait_for_noc_l3_ar(sim, 12);
    sim.check(l3_ar_seen, "Uncached NoC emits L3 AR for DDR read when L3 is enabled");
    sim.check(d->noc_to_l3_ar_addr_o == DdrAddr, "Uncached NoC L3 AR address for DDR read");
    sim.tick();

    d->noc_req_wdata_i = 0;
    d->noc_req_opcode_i = ReqRead;
    d->noc_req_size_i = SizeLine;
    d->noc_req_addr_i = SysAddr;
    d->noc_req_id_i = 0x77;
    d->noc_req_valid_i = 1;
    sim.tick();
    d->noc_req_valid_i = 0;
    bool err_seen = wait_for_noc_rsp(sim, 12);
    sim.check(err_seen, "Uncached NoC emits decode-error response for invalid SizeLine request");
    sim.check(d->noc_rsp_opcode_o == RspErr && d->noc_rsp_id_o == 0x77, "Uncached NoC decode-error response payload");
    sim.tick();

    d->noc_req_wdata_i = 1;
    d->noc_req_opcode_i = ReqWrite;
    d->noc_req_size_i = SizeDWord;
    d->noc_req_addr_i = BroadcastAddrEsr;
    d->noc_req_source_i = 2;
    d->noc_req_port_id_i = 1;
    d->noc_req_data_lo_i = (1ull << 40) | 0x3ull;
    d->noc_req_valid_i = 1;
    sim.tick();
    d->noc_req_valid_i = 0;
    (void)wait_for_noc_rsp(sim, 8);
    sim.tick();
    d->noc_req_addr_i = BroadcastAddrUm;
    d->noc_req_data_lo_i = (1ull << 40) | 0x3ull;
    d->noc_req_valid_i = 1;
    sim.tick();
    d->noc_req_valid_i = 0;
    bool bcast_seen = wait_for_noc_sys_aw(sim, 64);
    sim.check(bcast_seen, "Uncached NoC emits SYS AW for valid broadcast write");
    sim.check(d->noc_to_sys_aw_id_o >= 480, "Uncached NoC broadcast uses broadcast AXI ID range");

    // Top-level wrapper smoke for reset and FCC integration.
    d->top_fcc_enables_i = 2;
    d->top_esr_wdata_i = 0x13572468u;
    eval(d);
    sim.check(d->top_fcc_flat_o == 0x13572468u, "Top uncacheable forwards FCC data");
    sim.check(d->top_fcc_target_o == 1, "Top uncacheable forwards FCC target");
    sim.check(d->top_neigh_ready_o == 1, "Top uncacheable idle request path is ready after reset");

    return sim.finish();
}
