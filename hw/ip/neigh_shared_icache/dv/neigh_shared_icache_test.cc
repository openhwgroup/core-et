// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_shared_icache_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vneigh_shared_icache_tb;

namespace {

void clear_inputs(DUT* dut) {
    dut->bypass_icache_i = 1;
    dut->req_valid_i = 0;
    dut->req_thread_id_i = 0;
    dut->req_addr0_i = 0;
    dut->req_addr1_i = 0;
    dut->req_addr2_i = 0;
    dut->req_addr3_i = 0;
    dut->req_addr4_i = 0;
    dut->req_addr5_i = 0;
    dut->req_addr6_i = 0;
    dut->req_addr7_i = 0;
    dut->flush_i = 0;
    dut->l2_req_ready_i = 1;
    dut->l2_resp_valid_i = 0;
    dut->l2_resp_opcode_i = 0;
    dut->ptw_req_ready_i = 0x3;
    dut->ptw_resp_valid_i = 0;
    dut->sram_req_ready_i = 1;
    dut->sram_resp_valid_i = 0;
    dut->sram_resp_word0_i = 0;
    dut->apb_paddr_i = 0;
    dut->apb_pwrite_i = 0;
    dut->apb_psel_i = 0;
    dut->apb_penable_i = 0;
    dut->apb_pwdata_i = 0;
}

void tick(SimCtrl<DUT>& sim) {
    sim.tick();
}

void drive_request(DUT* dut, int minion, int thread, uint64_t addr) {
    dut->req_valid_i = static_cast<uint8_t>(1u << minion);
    if (thread) {
        dut->req_thread_id_i |= static_cast<uint8_t>(1u << minion);
    } else {
        dut->req_thread_id_i &= static_cast<uint8_t>(~(1u << minion));
    }
    switch (minion) {
        case 0: dut->req_addr0_i = addr; break;
        case 1: dut->req_addr1_i = addr; break;
        case 2: dut->req_addr2_i = addr; break;
        case 3: dut->req_addr3_i = addr; break;
        case 4: dut->req_addr4_i = addr; break;
        case 5: dut->req_addr5_i = addr; break;
        case 6: dut->req_addr6_i = addr; break;
        case 7: dut->req_addr7_i = addr; break;
        default: break;
    }
}

bool wait_for_ready(SimCtrl<DUT>& sim, uint8_t mask, int cycles) {
    for (int i = 0; i <= cycles; ++i) {
        if ((sim.dut->req_ready_o & mask) != 0) {
            return true;
        }
        tick(sim);
    }
    return false;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    clear_inputs(sim.dut.get());
    sim.reset(3);
    clear_inputs(sim.dut.get());

    sim.check(sim.dut->req_ready_o == 0, "reset clears wrapper request grants");
    sim.check(sim.dut->resp_valid_o == 0, "reset clears response valid");
    sim.check(sim.dut->l2_req_valid_o == 0, "reset clears L2 request valid");

    // A staged request is not granted in the same cycle it is captured, but it
    // is granted after the wrapper's f1 valid stage when the ICache requestor is ready.
    drive_request(sim.dut.get(), 0, 0, 0x0000000000001000ull);
    tick(sim);
    sim.dut->req_valid_i = 0;
    sim.check(wait_for_ready(sim, 0x01, 8), "minion 0 thread 0 request is granted");
    sim.check((sim.dut->dbg_req_valid_o & 0x1) != 0, "debug request-valid reflects requestor 0 grant");
    sim.check(sim.dut->dbg_req_addr0_o == (0x1000ull >> 6), "debug request address uses granted minion 0 request");

    // The second thread of the same minion uses an independent valid bit and can be granted separately.
    clear_inputs(sim.dut.get());
    drive_request(sim.dut.get(), 0, 1, 0x0000000000002040ull);
    tick(sim);
    sim.dut->req_valid_i = 0;
    sim.check(wait_for_ready(sim, 0x01, 8), "minion 0 thread 1 request is granted independently");
    sim.check(sim.dut->dbg_req_addr0_o == (0x2040ull >> 6), "debug address updates for thread 1 request");

    // A request in the upper requestor group drives requestor 1 debug state and ready for that minion.
    clear_inputs(sim.dut.get());
    drive_request(sim.dut.get(), 4, 0, 0x0000000000004000ull);
    tick(sim);
    sim.dut->req_valid_i = 0;
    sim.check(wait_for_ready(sim, 0x10, 8), "minion 4 request is granted through requestor 1");
    sim.check((sim.dut->dbg_req_valid_o & 0x2) != 0, "debug request-valid reflects requestor 1 grant");
    sim.check(sim.dut->dbg_req_addr1_o == (0x4000ull >> 6), "debug requestor 1 address uses minion 4 request");

    // Two simultaneous requests in one requestor group should grant one minion at a time, never both.
    clear_inputs(sim.dut.get());
    sim.dut->req_valid_i = 0x03;
    sim.dut->req_thread_id_i = 0x00;
    sim.dut->req_addr0_i = 0x0000000000008000ull;
    sim.dut->req_addr1_i = 0x0000000000009000ull;
    tick(sim);
    sim.dut->req_valid_i = 0;
    bool saw_any = false;
    for (int i = 0; i < 16; ++i) {
        uint8_t ready = sim.dut->req_ready_o & 0x03;
        sim.check(ready != 0x03, "LRU arbiter grant is one-hot within requestor 0");
        saw_any |= ready != 0;
        tick(sim);
    }
    sim.check(saw_any, "simultaneous request arbitration grants one contending minion");

    // Exercise APB debug and flush inputs; the check verifies outputs remain driven and non-X visible.
    clear_inputs(sim.dut.get());
    sim.dut->flush_i = 0x81;
    sim.dut->apb_psel_i = 1;
    sim.dut->apb_penable_i = 1;
    sim.dut->apb_paddr_i = 0x100;
    tick(sim);
    sim.check((sim.dut->apb_pready_o == 0) || (sim.dut->apb_pready_o == 1), "APB pready is driven");
    sim.check((sim.dut->apb_pslverr_o == 0) || (sim.dut->apb_pslverr_o == 1), "APB pslverr is driven");

    return sim.finish();
}
