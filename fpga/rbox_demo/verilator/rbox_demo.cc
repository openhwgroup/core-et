// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX depth-test demo: two overlapping triangles.
//
// Drives the real rbox_top RTL in Verilator to render two overlapping
// triangles at different depths with depth testing enabled.
// Triangle A (white, depth=0.3) occludes Triangle B (gray, depth=0.7)
// where they overlap.  Writes the result to a 64x64 PPM image file.
//
// Acts as the full system: CPU (APB configuration), cache (memory model),
// and framebuffer (PPM output).  No software runs on the hardware -- the
// test harness does everything that the minion cores would normally do.

#include "Vrbox_demo_top.h"
#include <verilated.h>
#ifdef TRACE
#include <verilated_vcd_c.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cmath>
#include <array>
#include <map>
#include <vector>
#include <algorithm>

using DUT = Vrbox_demo_top;

// ════════════════════════════════════════════════════════════════
// Constants
// ════════════════════════════════════════════════════════════════

static constexpr int FB_W = 64;
static constexpr int FB_H = 64;

// Physical address layout (2MB-page aligned)
static constexpr uint64_t INPUT_BUF_ADDR  = 0x00000000ULL;
static constexpr uint64_t OUTPUT_BUF_ADDR = 0x00200000ULL;
static constexpr uint64_t DEPTH_BUF_ADDR  = 0x00400000ULL;

// ET-Link request opcodes
static constexpr uint32_t REQ_WRITE         = 0;
static constexpr uint32_t REQ_READ          = 1;
static constexpr uint32_t REQ_MSG_SEND_DATA = 4;

// ET-Link response opcodes
static constexpr uint32_t RSP_ACK      = 0;
static constexpr uint32_t RSP_ACK_DATA = 1;

// ET-Link sizes
static constexpr uint32_t SIZE_LINE  = 6;  // 512b = 64B
static constexpr uint32_t SIZE_HLINE = 5;  // 256b = 32B

// Input packet types
static constexpr uint32_t INPCK_TRI_TILE_64x64 = 0;
static constexpr uint32_t INPCK_RBOX_STATE      = 5;
static constexpr uint32_t INPCK_FSH_STATE       = 6;
static constexpr uint32_t INPCK_END_OF_BUFFER   = 7;

// Output packet types
static constexpr uint32_t OUTPCK_STATE_INFO = 0;
static constexpr uint32_t OUTPCK_QUAD_INFO  = 1;
static constexpr uint32_t OUTPCK_END_PHASE  = 2;

// RBOX status values
static constexpr uint32_t STATUS_FINISHED = 0x02;

// APB ESR register addresses (via rbox_dbg_ram, bit 15 clear = ESR path)
static constexpr uint16_t ESR_CONFIG      = 0x0;
static constexpr uint16_t ESR_IN_BUF_PG   = 0x1;
static constexpr uint16_t ESR_IN_BUF_CFG  = 0x2;
static constexpr uint16_t ESR_OUT_BUF_PG  = 0x3;
static constexpr uint16_t ESR_OUT_BUF_CFG = 0x4;
static constexpr uint16_t ESR_STATUS      = 0x5;
static constexpr uint16_t ESR_START       = 0x6;
static constexpr uint16_t ESR_CONSUME     = 0x7;

// Triangle data addresses (used as color identifiers in QUAD_INFO)
static constexpr uint32_t TRI_DATA_ADDR_A = 1;  // white
static constexpr uint32_t TRI_DATA_ADDR_B = 2;  // gray

// ════════════════════════════════════════════════════════════════
// Simulation harness
// ════════════════════════════════════════════════════════════════

struct Sim {
    std::unique_ptr<DUT> dut;
    uint64_t sim_time = 0;
    uint64_t max_time = 20000000;
#ifdef TRACE
    VerilatedVcdC* tfp = nullptr;
#endif

    Sim(int argc, char** argv) {
        Verilated::commandArgs(argc, argv);
        dut = std::make_unique<DUT>();
#ifdef TRACE
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        dut->trace(tfp, 99);
        tfp->open("rbox_demo.vcd");
#endif
    }

    ~Sim() {
#ifdef TRACE
        if (tfp) { tfp->close(); delete tfp; }
#endif
    }

    bool cache_active = false;

    void tick() {
        if (cache_active) cache_pre_tick_fn(*this);
        dut->clk_i = 0;
        dut->eval();
#ifdef TRACE
        if (tfp) tfp->dump(sim_time);
#endif
        sim_time++;
        dut->clk_i = 1;
        dut->eval();
#ifdef TRACE
        if (tfp) tfp->dump(sim_time);
#endif
        sim_time++;
        if (cache_active) cache_post_tick_fn(*this);
        if (sim_time >= max_time) {
            printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim_time);
            std::exit(2);
        }
    }

    static void (*cache_pre_tick_fn)(Sim&);
    static void (*cache_post_tick_fn)(Sim&);

    void reset(int cycles = 10) {
        dut->rst_d_ni = 0;
        dut->rst_w_ni = 0;
        for (int i = 0; i < cycles; i++) tick();
        dut->rst_d_ni = 1;
        dut->rst_w_ni = 1;
        tick();
    }
};

// ════════════════════════════════════════════════════════════════
// VlWide helpers
// ════════════════════════════════════════════════════════════════

template <std::size_t N>
static void zero_wide(VlWide<N>& v) {
    for (std::size_t w = 0; w < N; w++) v[w] = 0;
}

// Extract bits [hi:lo] from a VlWide as a uint64_t.
template <std::size_t N>
static uint64_t vlwide_bits(const VlWide<N>& w, int hi, int lo) {
    uint64_t result = 0;
    for (int b = lo; b <= hi && b < (int)(N * 32); b++) {
        if (w[b / 32] & (1u << (b % 32)))
            result |= 1ULL << (b - lo);
    }
    return result;
}

// Set bits [hi:lo] of a VlWide to value.
template <std::size_t N>
static void vlwide_set_bits(VlWide<N>& w, int hi, int lo, uint64_t value) {
    for (int b = lo; b <= hi && b < (int)(N * 32); b++) {
        if (value & (1ULL << (b - lo)))
            w[b / 32] |= (1u << (b % 32));
        else
            w[b / 32] &= ~(1u << (b % 32));
    }
}

// ════════════════════════════════════════════════════════════════
// Memory model
// ════════════════════════════════════════════════════════════════

static std::map<uint64_t, std::array<uint8_t, 64>> g_memory;

static void mem_write_line(uint64_t addr, const uint8_t* data) {
    addr &= ~63ULL;
    memcpy(g_memory[addr].data(), data, 64);
}

static void mem_read_line(uint64_t addr, uint8_t* data) {
    addr &= ~63ULL;
    auto it = g_memory.find(addr);
    if (it != g_memory.end()) memcpy(data, it->second.data(), 64);
    else                      memset(data, 0, 64);
}

// Write a 256-bit (32-byte) half-line. addr must be 32-byte aligned.
static void mem_write_hline(uint64_t addr, const uint8_t* data) {
    uint64_t line_addr = addr & ~63ULL;
    int offset = (int)(addr & 32);
    auto& line = g_memory[line_addr];
    if (g_memory.find(line_addr) == g_memory.end())
        memset(line.data(), 0, 64);
    memcpy(line.data() + offset, data, 32);
}

static void mem_write_vlwide16(uint64_t addr, const VlWide<16>& data) {
    uint8_t buf[64];
    memcpy(buf, &data, 64);
    mem_write_line(addr, buf);
}

static void mem_read_vlwide16(uint64_t addr, VlWide<16>& data) {
    uint8_t buf[64];
    mem_read_line(addr, buf);
    memcpy(&data, buf, 64);
}

// ════════════════════════════════════════════════════════════════
// APB driver
// ════════════════════════════════════════════════════════════════
//
// req_t (83 bits = VlWide<3>):
//   [63:0]  pwdata, [64] penable, [65] psel, [66] pwrite, [82:67] paddr
//
// rsp_t (66 bits = VlWide<3>):
//   [0] pslverr, [64:1] prdata, [65] pready

static void apb_idle(DUT* d) {
    for (int w = 0; w < 3; w++) d->apb_req_i[w] = 0;
}

static void apb_write(Sim& sim, uint16_t addr, uint64_t wdata) {
    auto* d = sim.dut.get();
    auto& req = d->apb_req_i;

    // Setup phase: psel=1, pwrite=1, paddr, pwdata
    for (int w = 0; w < 3; w++) req[w] = 0;
    req[0] = (uint32_t)(wdata & 0xFFFFFFFF);
    req[1] = (uint32_t)(wdata >> 32);
    req[2] |= (1u << (65 - 64));  // psel
    req[2] |= (1u << (66 - 64));  // pwrite
    req[2] |= (uint32_t)(addr & 0xFFFF) << (67 - 64);
    sim.tick();

    // Access phase: assert penable
    req[2] |= (1u << (64 - 64));
    for (int i = 0; i < 20; i++) {
        sim.tick();
        if ((d->apb_rsp_o[2] >> (65 - 64)) & 1) break;
    }
    apb_idle(d);
    sim.tick();
}

static uint64_t apb_read(Sim& sim, uint16_t addr) {
    auto* d = sim.dut.get();
    auto& req = d->apb_req_i;

    // Setup phase: psel=1, pwrite=0
    for (int w = 0; w < 3; w++) req[w] = 0;
    req[2] |= (1u << (65 - 64));  // psel
    req[2] |= (uint32_t)(addr & 0xFFFF) << (67 - 64);
    sim.tick();

    // Access phase: assert penable
    req[2] |= (1u << (64 - 64));
    for (int i = 0; i < 20; i++) {
        sim.tick();
        if ((d->apb_rsp_o[2] >> (65 - 64)) & 1) break;
    }

    // Extract prdata from rsp_t bits [64:1]
    uint64_t lo = d->apb_rsp_o[0] >> 1;
    lo |= ((uint64_t)(d->apb_rsp_o[1]) << 31);
    uint64_t hi_bit = ((uint64_t)(d->apb_rsp_o[2]) & 1) << 63;
    uint64_t prdata = lo | hi_bit;
    apb_idle(d);
    sim.tick();
    return prdata;
}

// ════════════════════════════════════════════════════════════════
// Cache model (ET-Link request/response)
// ════════════════════════════════════════════════════════════════

static int g_msg_count = 0;

struct DelayedRsp { VlWide<17> rsp; int delay; };
static std::vector<DelayedRsp> g_rsp_queue;

struct OutPkt { uint32_t words[8]; uint64_t addr; };
static std::vector<OutPkt> g_output_pkts;

static void build_ack_rsp(VlWide<17>& rsp, uint8_t id) {
    for (int w = 0; w < 17; w++) rsp[w] = 0;
    vlwide_set_bits(rsp, 520, 519, RSP_ACK);
    vlwide_set_bits(rsp, 533, 526, id);
}

static void build_ack_data_rsp(VlWide<17>& rsp, uint8_t id, uint32_t size,
                                const VlWide<16>& data) {
    for (int w = 0; w < 17; w++) rsp[w] = 0;
    vlwide_set_bits(rsp, 6, 4, size);
    for (int b = 0; b < 512; b++) {
        if (data[b / 32] & (1u << (b % 32))) {
            int dst = b + 7;
            rsp[dst / 32] |= (1u << (dst % 32));
        }
    }
    vlwide_set_bits(rsp, 520, 519, RSP_ACK_DATA);
    vlwide_set_bits(rsp, 521, 521, 1);   // wdata = 1
    vlwide_set_bits(rsp, 533, 526, id);
}

static void enqueue_rsp(const VlWide<17>& rsp, bool is_input_read = false) {
    static int input_read_idx = 0;
    int delay = is_input_read ? (input_read_idx++ * 4) : 0;
    g_rsp_queue.push_back({rsp, delay});
}

static void cache_pre_tick(Sim& sim) {
    auto* d = sim.dut.get();
    d->rbox_sc_req_ready_i = 0x1F;
    for (auto& e : g_rsp_queue)
        if (e.delay > 0) e.delay--;
    if (!g_rsp_queue.empty() && g_rsp_queue.front().delay == 0) {
        for (int w = 0; w < 17; w++)
            d->sc_rbox_rsp_i[w] = g_rsp_queue.front().rsp[w];
        d->sc_rbox_rsp_valid_i = 1;
        g_rsp_queue.erase(g_rsp_queue.begin());
    } else {
        d->sc_rbox_rsp_valid_i = 0;
    }
}

static void cache_post_tick(Sim& sim) {
    auto* d = sim.dut.get();
    uint32_t req_valid = d->rbox_sc_req_valid_o;
    if (!req_valid) return;

    const auto& req = d->rbox_sc_req_o;
    uint32_t opcode  = (uint32_t)vlwide_bits(req, 570, 566);
    uint64_t address = vlwide_bits(req, 565, 526);
    uint32_t size    = (uint32_t)vlwide_bits(req, 13, 11);
    uint32_t id      = (uint32_t)vlwide_bits(req, 583, 576);
    VlWide<17> rsp;

    if (opcode == REQ_READ) {
        VlWide<16> data;
        mem_read_vlwide16(address, data);
        build_ack_data_rsp(rsp, (uint8_t)id, size, data);
        bool is_input_read = !(id & 0x80);
        enqueue_rsp(rsp, is_input_read);
    } else if (opcode == REQ_WRITE) {
        if (size == SIZE_LINE) {
            VlWide<16> data;
            for (int w = 0; w < 16; w++)
                data[w] = (uint32_t)vlwide_bits(req, 14 + w * 32 + 31, 14 + w * 32);
            mem_write_vlwide16(address, data);
        } else if (size == SIZE_HLINE) {
            uint8_t buf[32];
            OutPkt out;
            out.addr = address;
            for (int i = 0; i < 8; i++) {
                uint32_t word = (uint32_t)vlwide_bits(req, 14 + i * 32 + 31, 14 + i * 32);
                memcpy(buf + i * 4, &word, 4);
                out.words[i] = word;
            }
            mem_write_hline(address, buf);
            if (address >= OUTPUT_BUF_ADDR && address < OUTPUT_BUF_ADDR + 0x200000)
                g_output_pkts.push_back(out);
        }
        build_ack_rsp(rsp, (uint8_t)id);
        enqueue_rsp(rsp);
    } else if (opcode == REQ_MSG_SEND_DATA) {
        g_msg_count++;
        build_ack_rsp(rsp, (uint8_t)id);
        enqueue_rsp(rsp);
    }
}

void (*Sim::cache_pre_tick_fn)(Sim&) = cache_pre_tick;
void (*Sim::cache_post_tick_fn)(Sim&) = cache_post_tick;

// ════════════════════════════════════════════════════════════════
// Input packet construction
// ════════════════════════════════════════════════════════════════

// Build RBOX_STATE packet (512 bits).  bits [2:0] = inpck_type = 5.
static void build_rbox_state(VlWide<16>& pkt) {
    zero_wide(pkt);
    auto set = [&](int hi, int lo, uint64_t val) {
        vlwide_set_bits(pkt, hi, lo, val);
    };

    set(2, 0, INPCK_RBOX_STATE);  // inpck_type = 5

    int b = 8;
    b += 56;  // QW0: MSAA config -> b=64
    set(b+31, b, (uint32_t)(DEPTH_BUF_ADDR & 0xFFFFFFFF));
    set(b+63, b+32, (uint32_t)(DEPTH_BUF_ADDR >> 32));
    b += 64;  // b=128
    // QW2: depth/stencil + fragment shader config
    set(b+8, b, 126);  b += 9;   // ds_buffer_format = D32_SFLOAT
    b += 1 + 13 + 1 + 1;             // tiled, pitch, clamp, bound
    set(b, b, 1);      b += 1;   // depth_test_enable = 1
    set(b, b, 1);      b += 1;   // depth_test_write_enable = 1
    set(b+2, b, 1);    b += 3;   // depth_test_compare_op = 1 (CompOpLess)
    b += 1 + 1 + 1;                  // early_frag, stencil, fsh_disabled
    set(b, b, 1);      b += 1;   // fragment_shader_reads_bary = 1
    b += 3;                           // reads_depth, reads_coverage, per_sample
    set(b+1, b, 2);  b += 2;     // hart_tile_width = 2
    set(b+1, b, 2);  b += 2;     // hart_tile_height = 2
    set(b+1, b, 2);  b += 2;     // shire_layout_width = 2
    set(b+1, b, 2);  b += 2;     // shire_layout_height = 2
    b += 19;                          // reserved1 -> b=192
    b += 64 + 64;  // QW3+QW4: depth bounds/range -> b=320
    // QW5: scissor
    b += 14 + 14;                     // start_x, start_y = 0
    set(b+13, b, 64);  b += 14;  // scissor_height = 64
    set(b+13, b, 64);  b += 14;  // scissor_width = 64
}

static void build_fsh_state(VlWide<16>& pkt) {
    zero_wide(pkt);
    pkt[0] = INPCK_FSH_STATE;
}

// Build TRI_WITH_TILE_64x64 packet (512 bits) with precomputed edge equations.
//
// Bit layout (from inpck_tri_with_tile_64x64_t packed struct):
//   [2:0]     inpck_type        = 0
//   [15:3]    tile_left          13  tile X in 4-pixel units
//   [28:16]   tile_top           13  tile Y in 4-pixel units
//   [29]      tri_facing          1  0=front
//   [32:30]   tile_size           3  0=64x64
//   [63:33]   reserved0          31
//   [127:64]  edge_eq[0]         64  {pad[8:0], b_coef[22:0], pad[8:0], a_coef[22:0]}
//   [191:128] edge_eq[1]         64  same
//   [255:192] edge_eq[2]         64  same
//   [319:256] depth_eq           64  {b_coef_fp32[31:0], a_coef_fp32[31:0]}
//   [383:320] triangle_data_addr 64
//   [415:384] edge_sample[0]     32  {pad[2:0], sample[28:0]}
//   [447:416] edge_sample[1]     32  same
//   [479:448] edge_sample[2]     32  same
//   [511:480] depth_sample       32  FP32
struct EdgeEq {
    int32_t a, b, sample;
};

static void build_tri_tile(VlWide<16>& pkt,
                           const EdgeEq edges[3],
                           uint32_t depth_fp32,
                           uint64_t triangle_data_addr) {
    zero_wide(pkt);
    // pkt[0..1] = bits [63:0]: header — inpck_type=0, tile at origin, all zero → OK

    // edge_eq[i] at words [2+2i .. 3+2i]  (bits [127+64i : 64+64i])
    // Each 64-bit eq = {9'b0, b_coef[22:0], 9'b0, a_coef[22:0]}
    for (int i = 0; i < 3; i++) {
        uint32_t a_enc = (uint32_t)edges[i].a & 0x7FFFFF;
        uint32_t b_enc = (uint32_t)edges[i].b & 0x7FFFFF;
        pkt[2 + i * 2]     = a_enc;  // {9'b0, a_coef}
        pkt[2 + i * 2 + 1] = b_enc;  // {9'b0, b_coef}
    }

    // depth_eq at words [8..9] (bits [319:256]) — flat depth: a=0, b=0
    pkt[8] = 0;
    pkt[9] = 0;

    // triangle_data_addr at words [10..11] (bits [383:320])
    pkt[10] = (uint32_t)(triangle_data_addr & 0xFFFFFFFF);
    pkt[11] = (uint32_t)(triangle_data_addr >> 32);

    // edge_sample[i] at word [12+i] (bits [415+32i : 384+32i])
    // Each 32-bit sample = {3'b0, sample[28:0]}
    for (int i = 0; i < 3; i++)
        pkt[12 + i] = (uint32_t)edges[i].sample & 0x1FFFFFFF;

    // depth_sample at word [15] (bits [511:480])
    pkt[15] = depth_fp32;
}

static void build_end_of_buffer(VlWide<16>& pkt) {
    zero_wide(pkt);
    pkt[0] = INPCK_END_OF_BUFFER;
}

// ════════════════════════════════════════════════════════════════
// Depth buffer initialization
// ════════════════════════════════════════════════════════════════

static void init_depth_buffer() {
    // 64x64 framebuffer, D32_SFLOAT format: 64*64*4 = 16384 bytes = 256 cache lines.
    // Fill with FP32 1.0 (0x3F800000) at every 32-bit position.
    const uint32_t fp32_one = 0x3F800000;
    for (int line = 0; line < 256; line++) {
        uint64_t addr = DEPTH_BUF_ADDR + (uint64_t)line * 64;
        std::array<uint8_t, 64>& data = g_memory[addr];
        for (int w = 0; w < 16; w++)
            memcpy(data.data() + w * 4, &fp32_one, 4);
    }
}

// ════════════════════════════════════════════════════════════════
// Write input buffer to memory
// ════════════════════════════════════════════════════════════════

// Packed word stream layout (4 cache lines):
// Line 0: RBOX_STATE                          (words 0-1)
// Line 1: {TRI_A[255:0], FSH_STATE}           (words 2-3)
// Line 2: {TRI_B[255:0], TRI_A[511:256]}      (words 4-5)
// Line 3: {END_BUF, TRI_B[511:256]}           (words 6-7)
static constexpr int INPUT_LINE_COUNT = 4;

static void prepare_input_buffer() {
    // Triangle A (white, depth=0.3): vertices (10,8), (50,8), (30,56) -- points down
    // Triangle B (gray,  depth=0.7): vertices (14,56), (54,56), (34,8) -- points up
    //
    // Edge equations (opposite-vertex convention, negated for CCW/positive-inside):
    EdgeEq edges_a[3] = {
        { -48, -20, 2560 },   // edge[0]
        {  48, -20, -320 },   // edge[1]
        {   0,  40, -320 },   // edge[2]
    };
    EdgeEq edges_b[3] = {
        { -48,  20,  1472 },  // edge[0]
        {  48,  20, -1792 },  // edge[1]
        {   0, -40,  2240 },  // edge[2]
    };

    // Depth values as FP32
    uint32_t depth_a_fp32 = 0x3E99999A;  // 0.3f
    uint32_t depth_b_fp32 = 0x3F333333;  // 0.7f

    // Build packets
    VlWide<16> rbox_state, fsh_state, tri_a, tri_b, end_buf;
    build_rbox_state(rbox_state);
    build_fsh_state(fsh_state);
    build_tri_tile(tri_a, edges_a, depth_a_fp32, TRI_DATA_ADDR_A);
    build_tri_tile(tri_b, edges_b, depth_b_fp32, TRI_DATA_ADDR_B);
    build_end_of_buffer(end_buf);

    // Pack into cache lines
    VlWide<16> lines[INPUT_LINE_COUNT];
    for (int i = 0; i < INPUT_LINE_COUNT; i++)
        zero_wide(lines[i]);

    // Helper: write 8 uint32_t words into a half (lower=0, upper=1) of a cache line
    auto write_half = [&](int line_idx, int half, const uint32_t* src) {
        for (int w = 0; w < 8; w++)
            lines[line_idx][half * 8 + w] = src[w];
    };

    // Line 0: RBOX_STATE (lower=word0, upper=word1)
    write_half(0, 0, &rbox_state[0]);
    write_half(0, 1, &rbox_state[8]);

    // Line 1: lower=FSH_STATE (word2), upper=TRI_A[255:0] (word3)
    write_half(1, 0, &fsh_state[0]);
    write_half(1, 1, &tri_a[0]);

    // Line 2: lower=TRI_A[511:256] (word4), upper=TRI_B[255:0] (word5)
    write_half(2, 0, &tri_a[8]);
    write_half(2, 1, &tri_b[0]);

    // Line 3: lower=TRI_B[511:256] (word6), upper=END_BUF (word7)
    write_half(3, 0, &tri_b[8]);
    write_half(3, 1, &end_buf[0]);

    // Write cache lines to memory
    uint64_t addr = INPUT_BUF_ADDR;
    for (int i = 0; i < INPUT_LINE_COUNT; i++) {
        mem_write_vlwide16(addr, lines[i]);
        addr += 64;
    }
}

// ════════════════════════════════════════════════════════════════
// ESR configuration via APB
// ════════════════════════════════════════════════════════════════

static void configure_esr(Sim& sim) {
    printf("  Configuring ESR registers...\n");

    apb_write(sim, ESR_CONFIG, 1);  // enable

    // IN_BUF_PG: page0 = INPUT_BUF_ADDR >> 21, page_active0 = 1
    uint64_t in_buf_pg = (INPUT_BUF_ADDR >> 21) & 0x7FFFF;
    in_buf_pg |= (1ULL << 31);
    apb_write(sim, ESR_IN_BUF_PG, in_buf_pg);

    // IN_BUF_CFG: start_offset=0, line_count in upper 32 bits
    apb_write(sim, ESR_IN_BUF_CFG, (uint64_t)INPUT_LINE_COUNT << 32);

    // OUT_BUF_PG: page = OUTPUT_BUF_ADDR >> 21, page_active = 1
    uint32_t out_buf_pg = (uint32_t)((OUTPUT_BUF_ADDR >> 21) & 0x7FFFF) | (1u << 31);
    apb_write(sim, ESR_OUT_BUF_PG, (uint64_t)out_buf_pg);

    // OUT_BUF_CFG upper 32 bits:
    //   [2:0] hart_buf_lines=7  -> 2^7=128 initial pckt credits
    //   [4:3] hart_port_id=0
    //   [6:5] hart_max_msg=3   -> 4 initial msg credits
    //   [9:7] max_pckts_msg=7  -> message every 2^7=128 packets
    uint32_t upper = 7 | (3u << 5) | (7u << 7);
    apb_write(sim, ESR_OUT_BUF_CFG, (uint64_t)upper << 32);
}

// ════════════════════════════════════════════════════════════════
// Output packet decoder -- per-hart stream decoding
// ════════════════════════════════════════════════════════════════

static uint8_t g_fb[FB_H][FB_W][3];

// Decode per-hart output.  QUAD_INFO carries the triangle_data_addr
// in quad0_triangle_ptr (bits [191:128] = words[4..5]).
// TRI_DATA_ADDR_A (1) -> white, TRI_DATA_ADDR_B (2) -> gray (0x80).
static int decode_hart_stream(const std::vector<const OutPkt*>& pkts) {
    int pixels_shaded = 0;

    // Quad pair state: RBOX outputs pairs of 2x2 quads
    int qx[2] = {}, qy[2] = {};   // quad0/quad1 pixel positions
    uint8_t mask = 0;               // bits [3:0]=quad0, [7:4]=quad1
    uint32_t tri_ptr = 0;           // triangle_data_addr from QUAD_INFO
    bool have_quad = false, have_bi = false;

    for (size_t p = 0; p < pkts.size(); p++) {
        const uint32_t* w = pkts[p]->words;
        uint32_t type = w[0] & 0x3;

        if (type == OUTPCK_END_PHASE) break;
        if (type == OUTPCK_STATE_INFO && !have_quad) continue;

        if (type == OUTPCK_QUAD_INFO) {
            qx[0] = (int)((w[0] >> 16) & 0x7FFF);
            qx[1] = (int)((w[1] >>  0) & 0x7FFF);
            qy[0] = (int)((w[1] >> 16) & 0x7FFF);
            qy[1] = (int)((w[2] >>  0) & 0x7FFF);
            mask   = (uint8_t)((w[2] >> 24) & 0xFF);
            // triangle_data_addr is in bits [191:128] of QUAD_INFO.
            // QUAD_INFO is 256 bits = 8 words.  words[4..5] = bits [191:128].
            tri_ptr = w[4];  // low 32 bits of triangle_data_addr
            have_quad = true;
            have_bi = false;
            continue;
        }

        // After QUAD_INFO, type=0 packets carry bary_i then bary_j.
        // We need to consume both before shading.
        if (have_quad && type == OUTPCK_STATE_INFO) {
            if (!have_bi) { have_bi = true; continue; }

            // Both bary_i and bary_j received -- determine shade from tri_ptr.
            uint8_t shade;
            if (tri_ptr == TRI_DATA_ADDR_A)
                shade = 255;  // white
            else if (tri_ptr == TRI_DATA_ADDR_B)
                shade = 0x80; // gray
            else
                shade = 0xFF; // fallback: white

            for (int q = 0; q < 2; q++) {
                int base_x = qx[q], base_y = qy[q];
                int mask_shift = q * 4;
                for (int i = 0; i < 4; i++) {
                    if (!(mask & (1 << (mask_shift + i)))) continue;
                    int px = base_x + (i & 1);
                    int py = base_y + (i >> 1);
                    if (px < 0 || px >= FB_W || py < 0 || py >= FB_H) continue;
                    g_fb[py][px][0] = shade;
                    g_fb[py][px][1] = shade;
                    g_fb[py][px][2] = shade;
                    pixels_shaded++;
                }
            }
            have_quad = false;
            continue;
        }
    }
    return pixels_shaded;
}

static int decode_output_buffer() {
    // Group packets by hart buffer address, then decode each stream.
    // Hart buffer stride = 2^hart_buf_lines * 32 bytes = 128 * 32 = 0x1000.
    std::map<uint64_t, std::vector<const OutPkt*>> hart_streams;
    for (auto& p : g_output_pkts) {
        uint64_t hart = (p.addr - OUTPUT_BUF_ADDR) / 0x1000;
        hart_streams[hart].push_back(&p);
    }
    int total = 0;
    for (auto& [hart, stream] : hart_streams)
        total += decode_hart_stream(stream);
    return total;
}

// ════════════════════════════════════════════════════════════════
// PPM image writer
// ════════════════════════════════════════════════════════════════

static void write_ppm(const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) { printf("ERROR: cannot open %s\n", filename); return; }
    fprintf(f, "P6\n%d %d\n255\n", FB_W, FB_H);
    fwrite(g_fb, 1, FB_W * FB_H * 3, f);
    fclose(f);
}

// ════════════════════════════════════════════════════════════════
// Main
// ════════════════════════════════════════════════════════════════

int main(int argc, char** argv) {
    Sim sim(argc, argv);
    auto* d = sim.dut.get();
    memset(g_fb, 0, sizeof(g_fb));

    // Tie off unused inputs
    d->shire_id_i = 0;
    d->rbox_sc_req_ready_i = 0x1F;
    d->sc_rbox_rsp_valid_i = 0;
    apb_idle(d);
    d->rc_i = 0;
    d->dft_i = 0;
    d->ram_cfg_i = 0;

    printf("RBOX Depth Test Demo: Two Overlapping Triangles\n");
    printf("================================================\n\n");

    printf("  Resetting...\n");
    sim.reset(10);

    printf("  Initializing depth buffer (64x64 D32_SFLOAT = 1.0)...\n");
    init_depth_buffer();

    printf("  Preparing input buffer (%d cache lines, 2 triangles)...\n",
           INPUT_LINE_COUNT);
    prepare_input_buffer();

    configure_esr(sim);

    printf("  Starting RBOX...\n");
    sim.cache_active = true;
    apb_write(sim, ESR_START, 0x00000001);  // start=1, flush=0

    // Pre-load extra credits.  Initial: 128 pckt + 4 msg.
    // 8-bit pckt counter maxes at 255.  Add 127 to reach 255.
    // 3-bit msg counter (MaxHartMsgL+1=3 bits): initial 4, add 3 -> 7.
    apb_write(sim, ESR_CONSUME, 127 | (3u << 8));

    {
        uint64_t v = apb_read(sim, ESR_STATUS);
        printf("    STATUS after START = 0x%02x\n", (unsigned)(v & 0xFF));
    }

    printf("  Running simulation...\n");
    int cycle = 0;
    bool finished = false;
    int last_msg_count = 0;

    for (cycle = 0; cycle < 2000000; cycle++) {
        if (cycle > 0 && (cycle % 50000) == 0) {
            printf("    [%dk] msgs=%d out_pkts=%zu\n",
                   cycle / 1000, g_msg_count, g_output_pkts.size());
        }

        if (g_msg_count > last_msg_count) {
            printf("    [cycle %d] Received MsgSendData (#%d)\n", cycle, g_msg_count);
            for (int h = 0; h < 16; h++) {
                uint32_t consume = 50 | (3u << 8) | ((uint32_t)h << 16);
                apb_write(sim, ESR_CONSUME, consume);
            }
            last_msg_count = g_msg_count;
        }

        if (cycle > 0 && (cycle % 10000) == 0) {
            uint64_t status = apb_read(sim, ESR_STATUS);
            if ((status & 0xFF) == STATUS_FINISHED) {
                printf("    [cycle %d] STATUS = FINISHED\n", cycle);
                finished = true;
                break;
            }
        }
    }

    if (!finished) {
        uint64_t status = apb_read(sim, ESR_STATUS);
        if ((status & 0xFF) == STATUS_FINISHED) {
            finished = true;
        } else {
            printf("  Not finished after %d cycles (status=0x%02x), extending...\n",
                   cycle, (unsigned)(status & 0xFF));
            for (int extra = 0; extra < 100000; extra++) {
                sim.tick();
                if ((extra % 10000) == 0) {
                    status = apb_read(sim, ESR_STATUS);
                    if ((status & 0xFF) == STATUS_FINISHED) {
                        finished = true;
                        cycle += extra;
                        break;
                    }
                }
            }
        }
    }

    if (!finished) {
        printf("ERROR: RBOX did not finish!\n");
        return 1;
    }

    printf("  RBOX finished after ~%d cycles.\n", cycle);
    printf("  Total MsgSendData messages: %d\n", g_msg_count);

    printf("  Decoding output buffer (%zu packets captured)...\n", g_output_pkts.size());
    int pixels = decode_output_buffer();
    printf("  Shaded %d pixels.\n", pixels);

    printf("  Writing depth_test.ppm...\n");
    write_ppm("depth_test.ppm");

    printf("\nDone. Output: depth_test.ppm (%dx%d)\n", FB_W, FB_H);
    printf("TEST PASSED\n");
    return 0;
}
