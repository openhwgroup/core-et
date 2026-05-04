// Co-simulation test: intpipe_csr_msgs (new) vs original.
//
// Exercises message port configuration, PGET/PGETNB, enable/disable,
// L2 response delivery, and OOB storage.

#include "Vcosim_msgs_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>

using DUT = Vcosim_msgs_tb;

static uint32_t rng_state = 0xDEAD1234;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("msg_port_pget_block",       sim.dut->orig_msg_port_pget_block,       sim.dut->new_msg_port_pget_block);
    sim.compare("msg_port_xcpt",             sim.dut->orig_msg_port_xcpt,             sim.dut->new_msg_port_xcpt);
    sim.compare("msg_port_stall",            sim.dut->orig_msg_port_stall,            sim.dut->new_msg_port_stall);
    sim.compare("portctl",                   sim.dut->orig_portctl,                   sim.dut->new_portctl);
    sim.compare("mem_rdata_next",            sim.dut->orig_mem_rdata_next,            sim.dut->new_mem_rdata_next);
    sim.compare("msg_port_wr_en_reg_next",   sim.dut->orig_msg_port_wr_en_reg_next,   sim.dut->new_msg_port_wr_en_reg_next);
    sim.compare("msg_port_scp_way_reg_next", sim.dut->orig_msg_port_scp_way_reg_next, sim.dut->new_msg_port_scp_way_reg_next);
    sim.compare("msg_port_wr_addr_reg_next", sim.dut->orig_msg_port_wr_addr_reg_next, sim.dut->new_msg_port_wr_addr_reg_next);
    sim.compare("l2_resp_msg_valid",         sim.dut->orig_l2_resp_msg_valid,         sim.dut->new_l2_resp_msg_valid);
    sim.compare("req_port_info_enabled",     sim.dut->orig_req_port_info_enabled,     sim.dut->new_req_port_info_enabled);
    sim.compare("req_port_info_reply",       sim.dut->orig_req_port_info_reply,       sim.dut->new_req_port_info_reply);
}

static void idle_inputs(DUT* d) {
    d->stim_cpu_mem_porthead_en_any   = 0;
    d->stim_cpu_mem_portheadnb_en_any = 0;
    d->stim_cpu_mem_sen               = 0;
    d->stim_mem_addr                  = 0;
    d->stim_mem_cmd                   = 0;
    d->stim_mem_valid_qual            = 0;
    d->stim_mem_thread_id             = 0;
    d->stim_mem_wdata                 = 0;
    d->stim_mpp                       = 0;
    d->stim_cpu_wb_sen                = 0;
    d->stim_wb_thread_id              = 0;
    d->stim_wb_wdata                  = 0;
    d->stim_l2_resp_valid             = 0;
    d->stim_l2_resp_ready             = 0;
    // l2_resp is a wide bus — zero all words (275 bits = 9 words)
    for (int i = 0; i < 9; i++) d->stim_l2_resp[i] = 0;
    d->stim_req_port_info_thread_id   = 0;
    d->stim_req_port_info_port_id     = 0;
}

// Build l2_resp flat vector.  531 bits = 17 x 32-bit words (last partial).
// Struct (packed, MSB-first): id[7:0], dest, wdata, opcode[1:0], data[511:0], size[2:0], qwen[3:0]
// Total: 8+1+1+2+512+3+4 = 531
static void set_l2_resp(DUT* d, uint8_t id, uint8_t dest, uint8_t wdata,
                        uint8_t opcode, uint8_t size, uint8_t qwen) {
    // Zero everything first (275 bits = 9 x 32-bit words)
    for (int i = 0; i < 9; i++) d->stim_l2_resp[i] = 0;
    // Build the 275-bit packed struct in LSB-first word order for Verilator.
    // Packed struct (MSB to LSB):
    //   id[7:0]        — bits 274:267
    //   dest           — bit  266
    //   wdata          — bit  265
    //   opcode[1:0]    — bits 264:263
    //   data[255:0]    — bits 262:7
    //   size[2:0]      — bits 6:4
    //   qwen[3:0]      — bits 3:0
    //
    // Verilator word layout: word[0] = bits [31:0], ..., word[8] = bits [287:256]
    d->stim_l2_resp[0] = ((uint32_t)qwen & 0xF) | (((uint32_t)size & 0x7) << 4);
    // data[255:0] at bits 262:7 — leave as zero (test focuses on control)
    // Fields in word[8] (bits 274:256):
    //   id[7:0]    at word bits 18:11  (267-256=11)
    //   dest       at word bit  10     (266-256=10)
    //   wdata      at word bit  9      (265-256=9)
    //   opcode[1:0] at word bits 8:7   (263-256=7)
    //   data[255:250] at word bits 6:0 (262-256=6)
    d->stim_l2_resp[8] = ((uint32_t)(id & 0xFF) << 11)
                        | ((uint32_t)(dest & 1) << 10)
                        | ((uint32_t)(wdata & 1) << 9)
                        | ((uint32_t)(opcode & 3) << 7);
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 400000;
    sim.reset(5);
    idle_inputs(sim.dut.get());

    // Phase 1: Post-reset idle (5 cycles)
    for (int i = 0; i < 5; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Phase 2: Enable ports via cpu_wb_sen with configuration data.
    // Enable port 0 for thread 0: cpu_wb_sen[port=0][thread=0] = bit 0
    for (int port = 0; port < 4; port++) {
        for (int thread = 0; thread < 2; thread++) {
            idle_inputs(sim.dut.get());
            uint8_t sen_bit = 1 << (port * 2 + thread);
            sim.dut->stim_cpu_wb_sen = sen_bit;
            sim.dut->stim_wb_thread_id = thread;
            // wb_wdata: bit[0]=enable, bit[1]=enable_oob, bit[4]=umode,
            //           bits[7:5]=logsize(2=32B,3=64B,4=128B,5=256B),
            //           bits[11:8]=max_msgs, bits[16+:4]=scp_set, bits[24+:2]=scp_way
            uint64_t wdata = 0;
            wdata |= 1;               // enable
            wdata |= (1 << 1);        // enable_oob
            wdata |= (1 << 4);        // umode
            wdata |= ((uint64_t)(port + 2) << 5);  // logsize (2..5 for different ports)
            wdata |= ((uint64_t)(port) << 8);       // max_msgs
            wdata |= ((uint64_t)(port & 0xF) << 16); // scp_set
            wdata |= ((uint64_t)(port & 0x3) << 24); // scp_way
            sim.dut->stim_wb_wdata = wdata;
            sim.tick();
            compare_all(sim);
        }
    }

    // Some idle cycles after config
    idle_inputs(sim.dut.get());
    for (int i = 0; i < 3; i++) {
        sim.tick();
        compare_all(sim);
    }

    // Phase 3: PGET operations — read port heads.
    for (int port = 0; port < 4; port++) {
        idle_inputs(sim.dut.get());
        sim.dut->stim_cpu_mem_porthead_en_any = 1;
        sim.dut->stim_mem_thread_id = 0;
        sim.dut->stim_mem_addr = port;
        sim.dut->stim_mem_wdata = 0b0000;  // PGET pattern ??00
        sim.dut->stim_mpp = 3;  // M-mode
        sim.dut->stim_mem_valid_qual = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);
    }

    // Phase 4: L2 response message delivery.
    // EtLinkRspMsgRcvData = 2, need id < EtLinkMinMsgIdReduceReady (0xC0)
    for (int msg = 0; msg < 8; msg++) {
        idle_inputs(sim.dut.get());
        uint8_t thread = msg & 1;
        uint8_t port = (msg >> 1) & 3;
        // Build id: port_id at bits [4:3], valid bit at bit 2
        uint8_t id = (port << 3) | (1 << 2);  // set valid bit
        set_l2_resp(sim.dut.get(), id, thread, 0, 2 /*MsgRcvData*/, 0, 0);
        sim.dut->stim_l2_resp_valid = 1;
        sim.dut->stim_l2_resp_ready = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);
    }

    // Phase 4b: Now try PGET again — buffers should have data
    for (int port = 0; port < 4; port++) {
        for (int tid = 0; tid < 2; tid++) {
            idle_inputs(sim.dut.get());
            sim.dut->stim_cpu_mem_porthead_en_any = 1;
            sim.dut->stim_mem_thread_id = tid;
            sim.dut->stim_mem_addr = port;
            sim.dut->stim_mem_wdata = 0b0000;  // PGET ??00
            sim.dut->stim_mpp = 3;
            sim.dut->stim_mem_valid_qual = 1;
            sim.tick();
            compare_all(sim);

            // WB stage: allow the pget to retire
            idle_inputs(sim.dut.get());
            sim.dut->stim_wb_thread_id = tid;
            sim.tick();
            compare_all(sim);
        }
    }

    // Phase 4c: PGETNB operations
    for (int port = 0; port < 4; port++) {
        idle_inputs(sim.dut.get());
        sim.dut->stim_cpu_mem_portheadnb_en_any = 1;
        sim.dut->stim_mem_thread_id = 0;
        sim.dut->stim_mem_addr = port;
        sim.dut->stim_mem_wdata = 0b0001;  // PGETNB ??01
        sim.dut->stim_mpp = 3;
        sim.dut->stim_mem_valid_qual = 1;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);
    }

    // Phase 4d: Enable/Disable operations
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_cpu_mem_sen = 0x01;  // sen[port=0][thread=0]
        sim.dut->stim_mem_wdata = 0b0111;  // ENABLE_R
        sim.dut->stim_mpp = 3;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.dut->stim_cpu_mem_sen = 0x02;  // sen[port=0][thread=1]
        sim.dut->stim_mem_wdata = 0b0011;  // DISABLE
        sim.dut->stim_mpp = 3;
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);
    }

    // Phase 4e: Exception path — user mode access
    {
        idle_inputs(sim.dut.get());
        sim.dut->stim_cpu_mem_sen = 0x01;
        sim.dut->stim_mem_wdata = 0b0111;  // ENABLE_R in user mode
        sim.dut->stim_mpp = 0;  // U-mode
        sim.tick();
        compare_all(sim);

        idle_inputs(sim.dut.get());
        sim.tick();
        compare_all(sim);
    }

    // Phase 5: Random stimulus (~3000 cycles)
    for (int i = 0; i < 3000; i++) {
        uint32_t r0 = xorshift32();
        uint32_t r1 = xorshift32();
        uint32_t r2 = xorshift32();
        uint32_t r3 = xorshift32();

        sim.dut->stim_cpu_mem_porthead_en_any   = (r0 >> 0) & 1;
        sim.dut->stim_cpu_mem_portheadnb_en_any = (r0 >> 1) & 1;
        sim.dut->stim_cpu_mem_sen               = (r0 >> 2) & 0xFF;
        sim.dut->stim_mem_addr                  = (r0 >> 10) & 0x3;
        sim.dut->stim_mem_cmd                   = (r0 >> 12) & 0x7;
        // Keep mem_cmd in valid range (0-5)
        if (sim.dut->stim_mem_cmd > 5) sim.dut->stim_mem_cmd = 0;
        sim.dut->stim_mem_valid_qual            = (r0 >> 15) & 1;
        sim.dut->stim_mem_thread_id             = (r0 >> 16) & 1;
        sim.dut->stim_mem_wdata                 = (r0 >> 17) & 0xF;
        sim.dut->stim_mpp                       = (r0 >> 21) & 0x3;

        sim.dut->stim_cpu_wb_sen                = (r1 >> 0) & 0xFF;
        sim.dut->stim_wb_thread_id              = (r1 >> 8) & 1;
        sim.dut->stim_wb_wdata                  = ((uint64_t)r2 << 32) | r1;

        sim.dut->stim_l2_resp_valid             = (r3 >> 0) & 1;
        sim.dut->stim_l2_resp_ready             = (r3 >> 1) & 1;

        // Build a random l2_resp — only control fields matter
        uint8_t id = (r3 >> 2) & 0xFF;
        uint8_t dest = (r3 >> 10) & 1;
        uint8_t wdata_bit = (r3 >> 11) & 1;
        uint8_t opcode = (r3 >> 12) & 3;
        uint8_t size = (r3 >> 14) & 7;
        uint8_t qwen = (r3 >> 17) & 0xF;
        set_l2_resp(sim.dut.get(), id, dest, wdata_bit, opcode, size, qwen);

        sim.dut->stim_req_port_info_thread_id = (r3 >> 21) & 1;
        sim.dut->stim_req_port_info_port_id   = (r3 >> 22) & 3;

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
