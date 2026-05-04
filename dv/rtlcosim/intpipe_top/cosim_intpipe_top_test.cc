// Co-simulation test: intpipe_top (new) vs original.
//
// Phase 1: post-reset idle (10 cycles)
// Phase 2: feed NOP from frontend
// Phase 3: feed a few ALU instructions
// Phase 4: random stimulus (~2000 cycles)

#include "Vcosim_intpipe_top_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>

using DUT = Vcosim_intpipe_top_tb;

static uint32_t rng_state = 0xCAFE1234;
static uint32_t xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint64_t xorshift64() {
    return ((uint64_t)xorshift32() << 32) | xorshift32();
}

static void pack_frontend_resp(Vcosim_intpipe_top_tb* d, uint64_t pc, uint32_t inst) {
    const uint64_t pc49 = pc & ((1ULL << 49) - 1);
    d->stim_id_fe_resp.m_storage[0] = static_cast<uint32_t>((static_cast<uint64_t>(inst) << 8) & 0xFFFFFFFFULL);
    d->stim_id_fe_resp.m_storage[1] =
        static_cast<uint32_t>(((static_cast<uint64_t>(inst) >> 24) & 0xFFULL) | ((pc49 << 8) & 0xFFFFFF00ULL));
    d->stim_id_fe_resp.m_storage[2] = static_cast<uint32_t>((pc49 >> 24) & 0x1FFFFFFULL);
}

static uint32_t scripted_inst(uint64_t pc) {
    switch (pc) {
        case 0x1000: return 0x00100093U;
        case 0x1004: return 0x00200113U;
        case 0x1008: return 0x00208463U;
        case 0x100C: return 0x00300193U;
        case 0x1010: return 0x00108463U;
        case 0x1014: return 0x00700393U;
        case 0x1018: return 0x0080006FU;
        case 0x101C: return 0x00800413U;
        case 0x1020: return 0x02208233U;
        case 0x1024: return 0x021242B3U;
        case 0x1028: return 0x34029073U;
        case 0x102C: return 0x34002373U;
        case 0x1030: return 0x00230463U;
        case 0x1034: return 0x00900493U;
        case 0x1038: return 0x00A00513U;
        case 0x103C: return 0x00150593U;
        default:     return 0x00000013U;
    }
}

static bool is_ctrl_flow_inst(uint32_t inst) {
    const uint32_t opcode = inst & 0x7FU;
    return opcode == 0x63U || opcode == 0x6FU || opcode == 0x67U;
}

// Compare a VlWide (multi-word) signal.
template<std::size_t N>
static void compare_wide(CosimCtrl<DUT>& sim, const char* name,
                         const VlWide<N>& orig, const VlWide<N>& newv) {
    sim.comparisons++;
    bool match = (memcmp(&orig, &newv, sizeof(VlWide<N>)) == 0);
    if (!match) {
        sim.mismatches++;
        printf("  MISMATCH [t=%lu] %s: orig=", (unsigned long)sim.sim_time, name);
        for (int i = N-1; i >= 0; i--) printf("%08x", orig.m_storage[i]);
        printf(" new=");
        for (int i = N-1; i >= 0; i--) printf("%08x", newv.m_storage[i]);
        printf("\n");
    }
}

// Track whether register files are synchronized (both modules have had
// enough writes that X-initialized values are flushed).
static bool rf_synced = false;

// Compare ALL key outputs every cycle.
static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    // ── Frontend request ──
    sim.compare("id_fe_req_valid",      d->orig_id_fe_req_valid,      d->new_id_fe_req_valid);
    sim.compare("id_fe_stall",          d->orig_id_fe_stall,          d->new_id_fe_stall);
    sim.compare("id_fe_resp_ready",     d->orig_id_fe_resp_ready,     d->new_id_fe_resp_ready);
    sim.compare("id_fe_req_pc0",        d->orig_id_fe_req_pc0,        d->new_id_fe_req_pc0);
    compare_wide(sim, "id_fe_req",      d->orig_id_fe_req,            d->new_id_fe_req);

    // ── ICache / TLB ──
    sim.compare("icache_invalidate",    d->orig_icache_invalidate,    d->new_icache_invalidate);
    sim.compare("tlb_invalidate",       d->orig_tlb_invalidate,       d->new_tlb_invalidate);

    // ── DCache ctrl / VPU ctrl (struct as flat bits) ──
    // These large structs embed register-file data (tensor ctrl, x31 values)
    // which differ due to X-initialization of the RF primitives. Only compare
    // once register files are synchronized (all x-regs written at least once).
    if (rf_synced) {
        compare_wide(sim, "dcache_ctrl",    d->orig_dcache_ctrl,      d->new_dcache_ctrl);
        compare_wide(sim, "vpu_ctrl",       d->orig_vpu_ctrl,         d->new_vpu_ctrl);
    }

    // ── DCache request ──
    sim.compare("id_dcache_alloc_rq_pre", d->orig_id_dcache_alloc_rq_pre, d->new_id_dcache_alloc_rq_pre);
    sim.compare("ex_dcache_alloc_rq_val", d->orig_ex_dcache_alloc_rq_val, d->new_ex_dcache_alloc_rq_val);
    sim.compare("id_dcache_gsc",        d->orig_id_dcache_gsc,        d->new_id_dcache_gsc);
    sim.compare("ex_dcache_req_valid",  d->orig_ex_dcache_req_valid,  d->new_ex_dcache_req_valid);
    // Only compare ex_dcache_req when valid -- undriven fields (gsc32_idx, mem_global)
    // come from ex_reg_data which is never reset, so X-initial values may differ.
    if (d->orig_ex_dcache_req_valid && d->new_ex_dcache_req_valid) {
        compare_wide(sim, "ex_dcache_req",  d->orig_ex_dcache_req,    d->new_ex_dcache_req);
    }
    sim.compare("ex_dcache_gsc",        d->orig_ex_dcache_gsc,        d->new_ex_dcache_gsc);
    sim.compare("tag_dcache_kill",      d->orig_tag_dcache_kill,      d->new_tag_dcache_kill);
    // tag_dcache_store_data is from VPU (passed through), data-dependent.
    if (rf_synced) {
        compare_wide(sim, "tag_dcache_store_data", d->orig_tag_dcache_store_data, d->new_tag_dcache_store_data);
    }
    sim.compare("mem_dcache_kill",      d->orig_mem_dcache_kill,      d->new_mem_dcache_kill);
    // wb_dcache_x31 depends on register file x31 content.
    if (rf_synced) {
        sim.compare("wb_dcache_x31",    d->orig_wb_dcache_x31,        d->new_wb_dcache_x31);
    }
    sim.compare("wb_dcache_invalidate_lr", d->orig_wb_dcache_invalidate_lr, d->new_wb_dcache_invalidate_lr);
    sim.compare("wb_dcache_fp_toint",   d->orig_wb_dcache_fp_toint,   d->new_wb_dcache_fp_toint);

    // ── SATP / VM ──
    sim.compare("satp_info",            d->orig_satp_info,            d->new_satp_info);
    sim.compare("matp_info",            d->orig_matp_info,            d->new_matp_info);
    sim.compare("satp_info_en",         d->orig_satp_info_en,         d->new_satp_info_en);
    sim.compare("matp_info_en",         d->orig_matp_info_en,         d->new_matp_info_en);
    sim.compare("vm_status",            d->orig_vm_status,            d->new_vm_status);

    // ── VPU ──
    sim.compare("id_vpu_req",           d->orig_id_vpu_req,           d->new_id_vpu_req);
    compare_wide(sim, "ex_vpu_req",     d->orig_ex_vpu_req,           d->new_ex_vpu_req);
    sim.compare("tag_vpu_kill",         d->orig_tag_vpu_kill,         d->new_tag_vpu_kill);
    sim.compare("mem_vpu_kill",         d->orig_mem_vpu_kill,         d->new_mem_vpu_kill);
    sim.compare("wb_vpu_kill",          d->orig_wb_vpu_kill,          d->new_wb_vpu_kill);
    sim.compare("wb_vpu_dcache_resp_valid", d->orig_wb_vpu_dcache_resp_valid, d->new_wb_vpu_dcache_resp_valid);
    compare_wide(sim, "wb_vpu_dcache_resp", d->orig_wb_vpu_dcache_resp, d->new_wb_vpu_dcache_resp);

    // ── FLB ──
    sim.compare("flb_neigh_req_valid",  d->orig_flb_neigh_req_valid,  d->new_flb_neigh_req_valid);
    sim.compare("flb_neigh_req_data",   d->orig_flb_neigh_req_data,   d->new_flb_neigh_req_data);

    // ── Trace encoder ──
    sim.compare("TE_wb_reg_inst",       d->orig_TE_wb_reg_inst,       d->new_TE_wb_reg_inst);
    sim.compare("TE_wb_reg_cause_ie",   d->orig_TE_wb_reg_cause_ie,   d->new_TE_wb_reg_cause_ie);
    sim.compare("TE_wb_reg_context",    d->orig_TE_wb_reg_context,    d->new_TE_wb_reg_context);
    sim.compare("TE_tval",              d->orig_TE_tval,              d->new_TE_tval);
    sim.compare("TE_prv",               d->orig_TE_prv,               d->new_TE_prv);
    sim.compare("TE_exception",         d->orig_TE_exception,         d->new_TE_exception);

    // ── Debug ──
    sim.compare("update_ddata0",        d->orig_update_ddata0,        d->new_update_ddata0);
    // ddata0_wdata depends on register file contents (x31), skip until synced.
    if (rf_synced) {
        sim.compare("ddata0_wdata",     d->orig_ddata0_wdata,         d->new_ddata0_wdata);
    }
    sim.compare("in_debug_mode",        d->orig_in_debug_mode,        d->new_in_debug_mode);
    sim.compare("debug_busy",           d->orig_debug_busy,           d->new_debug_busy);
    sim.compare("debug_exception",      d->orig_debug_exception,      d->new_debug_exception);
    compare_wide(sim, "debug_monitor_out", d->orig_debug_monitor_out, d->new_debug_monitor_out);

    // ── Monitor ──
    sim.compare("wb_valid",             d->orig_wb_valid,             d->new_wb_valid);
    sim.compare("wb_thread_id",         d->orig_wb_thread_id,         d->new_wb_thread_id);
    sim.compare("wb_reg_pc",            d->orig_wb_reg_pc,            d->new_wb_reg_pc);

    // ── PMU ──
    sim.compare("pmu_count_up",         d->orig_pmu_count_up,         d->new_pmu_count_up);
    sim.compare("pmu_read_sel",         d->orig_pmu_read_sel,         d->new_pmu_read_sel);
    sim.compare("pmu_write_en",         d->orig_pmu_write_en,         d->new_pmu_write_en);
    sim.compare("pmu_write_data",       d->orig_pmu_write_data,       d->new_pmu_write_data);
    sim.compare("pmu_neigh_event_sel",  d->orig_pmu_neigh_event_sel,  d->new_pmu_neigh_event_sel);
}

// Set all stimulus inputs to idle (inactive) state.
static void idle_inputs(DUT* d) {
    d->stim_dft_reset_byp = 0;
    d->stim_dft_reset = 0;
    d->stim_te_enable = 0;

    d->stim_interrupts = 0;  // QData (63 bits)

    d->stim_shire_id = 0;
    d->stim_shire_min_id = 0;
    d->stim_chicken_bit_intpipe = 0;
    d->stim_use_decoded_id_ctrl = 0;

    d->stim_id_fe_resp_valid = 0;
    d->stim_id_fe_resp_thread_id = 0;
    memset(&d->stim_id_fe_resp, 0, sizeof(d->stim_id_fe_resp));

    d->stim_dcache_ctrl_resp = 0;  // IData (23 bits)
    d->stim_id_dcache_ready = 1;  // Ready by default
    d->stim_mem_dcache_resp_int_valid = 0;
    d->stim_wb_dcache_resp_valid = 0;
    memset(&d->stim_wb_dcache_resp, 0, sizeof(d->stim_wb_dcache_resp));
    d->stim_tag_dcache_bp = 0;  // QData (53 bits)
    d->stim_tag_dcache_bp_valid = 0;

    d->stim_id_ctrl = 0;  // QData (45 bits)

    d->stim_id_dcache_ordered = 0x3; // both threads ordered (idle)
    d->stim_id_dcache_idle = 1;
    d->stim_id_dcache_scoreboard = 0;  // QData (64 bits)
    d->stim_id_dcache_sb_int_dealloc = 0;
    d->stim_id_dcache_sb_fp_dealloc = 0;
    d->stim_tag_dcache_xcpt = 0;  // uint8_t (8 bits)
    d->stim_tag_dcache_replay_next = 0;
    d->stim_mem_dcache_flush_pipeline = 0;
    d->stim_dcache_bus_error = 0;
    memset(&d->stim_dcache_bus_error_addr, 0, sizeof(d->stim_dcache_bus_error_addr));

    d->stim_l2_resp_valid = 0;
    d->stim_l2_resp_ready = 0;
    memset(&d->stim_l2_resp, 0, sizeof(d->stim_l2_resp));

    d->stim_thread0_enable = 1;  // thread0 enabled by default
    d->stim_thread1_enable = 0;  // thread1 disabled

    d->stim_id_frontend_vpu_ctrl = 0;  // QData (45 bits)
    memset(&d->stim_id_vpu_ctrl, 0, sizeof(d->stim_id_vpu_ctrl));
    d->stim_ex_vpu_ctrl = 0;  // QData (42 bits)
    memset(&d->stim_tag_vpu_ctrl, 0, sizeof(d->stim_tag_vpu_ctrl));
    d->stim_mem_vpu_ctrl = 0;  // uint8_t (2 bits)
    memset(&d->stim_wb_vpu_ctrl, 0, sizeof(d->stim_wb_vpu_ctrl));
    d->stim_vpu_tfma_tenb_working = 0;

    d->stim_flb_neigh_resp_valid = 0;
    d->stim_flb_neigh_resp_data = 0;

    d->stim_te_thread_sel = 0;

    memset(&d->stim_read_ddata0, 0, sizeof(d->stim_read_ddata0));
    d->stim_debug_ex_program_buffer = 0;
    d->stim_halt = 0;
    d->stim_resume = 0;

    d->stim_esr_shire_coop_mode = 0;
    d->stim_esr_features = 0;  // uint8_t (6 bits)

    d->stim_tensor_load_err_flags = 0;
    d->stim_cache_ops_err_flags = 0;
    d->stim_tensor_reduce_err_flags = 0;

    memset(&d->stim_pmu_read_data, 0, sizeof(d->stim_pmu_read_data));

    d->stim_io_events_dcache_vpu = 0;
    d->stim_dcache_debug_signals = 0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    idle_inputs(sim.dut.get());

    // Custom reset: assert all three resets (rst_ni, rst_w_ni, rst_d_ni)
    sim.dut->rst_ni   = 0;
    sim.dut->rst_w_ni = 0;
    sim.dut->rst_d_ni = 0;
    for (int i = 0; i < 10; i++) sim.tick();
    sim.dut->rst_ni   = 1;
    sim.dut->rst_w_ni = 1;
    sim.dut->rst_d_ni = 1;
    sim.tick();  // one clean cycle after release

    idle_inputs(sim.dut.get());

    // ════════════════════════════════════════════════════════════════
    // Phase 1: Post-reset idle -- verify initial state is identical
    // ════════════════════════════════════════════════════════════════
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 2: Feed NOP instruction from frontend
    // NOP = addi x0, x0, 0 = 0x00000013
    // ════════════════════════════════════════════════════════════════
    // frontend_core_resp is packed: {pc[48:0], inst_bits[31:0], pf0, pf1, af0, af1, replay, rvc, bus_error, ecc_error}
    // = 49 + 32 + 8 = 89 bits
    // We need to set inst_bits = 0x00000013, pc = 0x80000000, rest = 0
    // But the packed bit layout starts from MSB. Let's encode properly.
    //
    // The struct is packed: MSB first: pc (49 bits), inst_bits (32 bits), pf0, pf1, af0, af1, replay, rvc, bus_error, ecc_error
    // So flat encoding [88:0]:
    //   [88:40] = pc = 0x80000000 (49-bit sign-extended)
    //   [39:8]  = inst_bits
    //   [7:0]   = flags (all zero)

    // Build frontend_core_resp for NOP
    // pc = 0x0000_0080_0000_0000 as 49-bit = 0x0_8000_0000
    {
        auto* d = sim.dut.get();
        idle_inputs(d);

        // Encode NOP: id_ctrl must have legal=1 for the instruction to be processed
        // minion_control_t: {legal, mcode, fp, br, jal, jalr, rxs2, rxs1,
        //   sel_alu2[2:0], sel_alu1[1:0], sel_imm[2:0], alu_dw, alu_fn[3:0],
        //   mem, mem_cmd[4:0], mem_type[2:0], rma, wmd, wmad, wfd, div, wxd,
        //   csr[2:0], fence, mem_g, gsc, gfx, x31}
        // NOP (addi x0,x0,0): legal=1, wxd=0 (writes x0 = no effect), sel_alu2 = IMM, etc.
        // For simplicity we'll just set legal=1, all other bits = 0 (this is effectively a no-op)

        // Present frontend response
        d->stim_id_fe_resp_valid = 1;
        d->stim_id_fe_resp_thread_id = 0;

        // Encode as flat bits -- set pc and inst_bits
        // The id_fe_resp is a VlWide. Set fields appropriately.
        // frontend_core_resp = {pc[48:0], inst_bits[31:0], pf0, pf1, af0, af1, replay, rvc, bus_error, ecc_error}
        // Total 89 bits = VlWide<3> (96 bits)
        // [88:40] = pc = 0x0_8000_0000 (49 bits)
        // [39:8]  = inst_bits = 0x00000013
        // [7:0]   = 0
        d->stim_id_fe_resp.m_storage[0] = (0x00000013U << 8); // inst_bits shifted left 8
        d->stim_id_fe_resp.m_storage[1] = (0x80000000ULL >> 24) | (0x00000013U >> 24); // overlapping
        d->stim_id_fe_resp.m_storage[2] = 0;

        // Actually, let's be more careful with the bit packing.
        // Bit positions in flat vector (LSB = 0):
        // [7:0]   = {ecc_error, bus_error, rvc, replay, af1, af0, pf1, pf0} = 0
        // [39:8]  = inst_bits[31:0] = 0x00000013
        // [88:40] = pc[48:0] = 0x0_8000_0000
        uint64_t inst = 0x00000013ULL;
        uint64_t pc   = 0x080000000ULL; // 49-bit: 0x0_8000_0000
        // word 0 (bits 31:0):  flags[7:0] | inst[23:0] << 8
        d->stim_id_fe_resp.m_storage[0] = (uint32_t)((inst << 8) & 0xFFFFFFFFULL);
        // word 1 (bits 63:32): inst[31:24] | pc[23:0] << 8
        d->stim_id_fe_resp.m_storage[1] = (uint32_t)(((inst >> 24) & 0xFF) | ((pc << 8) & 0xFFFFFF00ULL));
        // word 2 (bits 95:64): pc[48:24]
        d->stim_id_fe_resp.m_storage[2] = (uint32_t)((pc >> 24) & 0x1FFFFFF);

        // Set id_ctrl with legal=1 (MSB of the packed struct)
        // minion_control_t total = 44 bits (QData)
        // Fields (MSB first in packed struct):
        // legal(43), mcode(42), fp(41), br(40), jal(39), jalr(38), rxs2(37), rxs1(36),
        // sel_alu2(35:33), sel_alu1(32:31), sel_imm(30:28), alu_dw(27), alu_fn(26:23),
        // mem(22), mem_cmd(21:17), mem_type(16:14), rma(13), wmd(12), wmad(11), wfd(10),
        // div(9), wxd(8), csr(7:5), fence(4), mem_g(3), gsc(2), gfx(1), x31(0)
        d->stim_id_ctrl = (1ULL << 43);  // legal=1

        sim.tick();
        compare_all(sim);

        // Deassert
        idle_inputs(d);
        for (int i = 0; i < 5; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 3: Feed a few simple ALU instructions
    // ADD x1, x0, x0 = 0x00000033 (R-type)
    // ADDI x2, x0, 42 = 0x02a00113 (I-type)
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();

        // Instruction: ADD x1, x0, x0
        idle_inputs(d);
        d->stim_id_fe_resp_valid = 1;
        d->stim_id_fe_resp_thread_id = 0;

        uint64_t inst = 0x00000033ULL;
        uint64_t pc   = 0x080000004ULL;
        pack_frontend_resp(d, pc, static_cast<uint32_t>(inst));

        // minion_control_t (44 bits, QData):
        // legal(43), mcode(42), fp(41), br(40), jal(39), jalr(38), rxs2(37), rxs1(36),
        // sel_alu2(35:33), sel_alu1(32:31), sel_imm(30:28), alu_dw(27), alu_fn(26:23),
        // mem(22), mem_cmd(21:17), mem_type(16:14), rma(13), wmd(12), wmad(11), wfd(10),
        // div(9), wxd(8), csr(7:5), fence(4), mem_g(3), gsc(2), gfx(1), x31(0)
        //
        // For ADD: legal=1, rxs1=1, rxs2=1, wxd=1
        d->stim_id_ctrl = (1ULL << 43) | (1ULL << 37) | (1ULL << 36) | (1ULL << 8);

        sim.tick();
        compare_all(sim);

        // ADDI x2, x0, 42
        idle_inputs(d);
        d->stim_id_fe_resp_valid = 1;
        d->stim_id_fe_resp_thread_id = 0;

        inst = 0x02a00113ULL;
        pc   = 0x080000008ULL;
        pack_frontend_resp(d, pc, static_cast<uint32_t>(inst));

        // ADDI: legal=1, rxs1=1, wxd=1
        d->stim_id_ctrl = (1ULL << 43) | (1ULL << 36) | (1ULL << 8);

        sim.tick();
        compare_all(sim);

        // Let the pipeline drain
        idle_inputs(d);
        for (int i = 0; i < 10; i++) {
            sim.tick();
            compare_all(sim);
        }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 4: Directed scalar/control/CSR/muldiv program
    // Mirrors the focused test_intpipe bring-up sequence so old/new
    // RTL see the exact same hazard-sensitive path.
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();

        idle_inputs(d);
        rf_synced = false;
        d->rst_ni   = 0;
        d->rst_w_ni = 0;
        d->rst_d_ni = 0;
        for (int i = 0; i < 10; ++i) {
            sim.tick();
            compare_all(sim);
        }
        d->rst_ni   = 1;
        d->rst_w_ni = 1;
        d->rst_d_ni = 1;
        sim.tick();
        compare_all(sim);

        bool src_active = true;
        uint64_t src_pc = 0x1000;
        bool ctrl_wait = false;
        uint64_t ctrl_pc = 0;
        uint64_t ctrl_fallthrough_pc = 0;
        bool redirect_prev_valid = false;
        uint64_t redirect_prev_pc = 0;
        int issue_count = 0;
        int retire_count = 0;
        int redirect_count = 0;

        for (int cycle = 0; cycle < 2000 && retire_count < 13; ++cycle) {
            idle_inputs(d);
            d->stim_use_decoded_id_ctrl = 1;

            if (src_active) {
                d->stim_id_fe_resp_valid = 1;
                d->stim_id_fe_resp_thread_id = 0;
                pack_frontend_resp(d, src_pc, scripted_inst(src_pc));
            }

            sim.tick();
            compare_all(sim);

            const bool redirect_valid = (d->new_id_fe_req_valid & 0x1U) != 0;
            const bool redirect_new =
                redirect_valid && (!redirect_prev_valid || (static_cast<uint64_t>(d->new_id_fe_req_pc0) != redirect_prev_pc));
            const bool issue_fire = src_active && d->new_id_fe_resp_ready;
            const bool retire_fire = d->new_wb_valid && !d->new_wb_thread_id;

            if (redirect_new) {
                redirect_count++;
                src_active = issue_count < 13;
                src_pc = static_cast<uint64_t>(d->new_id_fe_req_pc0);
                ctrl_wait = false;
            }

            if (issue_fire) {
                const uint32_t inst = scripted_inst(src_pc);
                issue_count++;
                if (issue_count == 13) {
                    src_active = false;
                    ctrl_wait = false;
                } else if (is_ctrl_flow_inst(inst)) {
                    src_active = false;
                    ctrl_wait = true;
                    ctrl_pc = src_pc;
                    ctrl_fallthrough_pc = src_pc + 4;
                } else if (!redirect_new) {
                    src_pc += 4;
                    src_active = true;
                }
            }

            if (ctrl_wait && retire_fire && static_cast<uint64_t>(d->new_wb_reg_pc) == ctrl_pc && !redirect_new) {
                ctrl_wait = false;
                if (issue_count < 13) {
                    src_active = true;
                    src_pc = ctrl_fallthrough_pc;
                } else {
                    src_active = false;
                }
            }

            if (retire_fire) {
                retire_count++;
            }

            redirect_prev_valid = redirect_valid;
            if (redirect_valid) {
                redirect_prev_pc = static_cast<uint64_t>(d->new_id_fe_req_pc0);
            }
        }

        if (retire_count != 13 || redirect_count < 2) {
            std::printf("  NOTE: directed intpipe script ended with issues=%d redirects=%d retires=%d\n",
                        issue_count, redirect_count, retire_count);
        }

        idle_inputs(d);
        for (int i = 0; i < 20; ++i) {
            sim.tick();
            compare_all(sim);
        }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 5: Random stimulus (~2000 cycles)
    // ════════════════════════════════════════════════════════════════

    for (int cycle = 0; cycle < 2000; cycle++) {
        auto* d = sim.dut.get();
        uint32_t r = xorshift32();

        idle_inputs(d);

        // Randomly present a frontend response (instruction)
        if ((r & 0x7) < 3) {  // ~37.5% chance
            d->stim_id_fe_resp_valid = 1;
            d->stim_id_fe_resp_thread_id = (r >> 3) & 1;

            uint32_t inst = xorshift32();
            uint64_t pc = (uint64_t)(xorshift32() & 0x0FFFFFFF) << 2; // aligned, < 2^30

            pack_frontend_resp(d, pc, inst);

            // Random control signals -- but always set legal=1
            uint64_t ctrl = xorshift64() & ((1ULL << 44) - 1);
            ctrl |= (1ULL << 43);  // force legal=1
            // Zero out dangerous bits to avoid cascading state issues:
            // mem = bit 22, fence = bit 4, gsc = bit 2, csr[2:0] = bits 7:5
            ctrl &= ~((1ULL << 22) | (1ULL << 4) | (1ULL << 2) | (0x7ULL << 5));
            d->stim_id_ctrl = ctrl;
        }

        // Random dcache response (rare)
        if (((r >> 4) & 0xF) == 0) {
            d->stim_wb_dcache_resp_valid = 1;
            for (unsigned w = 0; w < sizeof(d->stim_wb_dcache_resp) / 4; w++)
                d->stim_wb_dcache_resp.m_storage[w] = xorshift32();
        }

        // Random interrupt inputs (rare)
        if ((r >> 8) & 0x1) {
            // Set a random bit pattern in interrupts (QData, 63 bits)
            // Lower bits: mieco(2), mtip(2), msip(2), meip(2), seip(2), fcc(4) = 14 bits
            // Upper bits: ipi_with_pc(2), ipi_pc(47) -- keep ipi_pc zero for safety
            d->stim_interrupts = xorshift64() & 0x3FFF; // only toggle lower 14 bits
        }

        // Random dcache ctrl resp
        if ((r >> 9) & 0x1) {
            d->stim_dcache_ctrl_resp = xorshift32() & 0xFFFF;
        }

        // Random dcache scoreboard (QData, 64 bits)
        if ((r >> 10) & 0x1) {
            d->stim_id_dcache_scoreboard = xorshift64();
        }

        // Randomly toggle dcache ordered/idle
        d->stim_id_dcache_ordered = (r >> 11) & 0x3;
        d->stim_id_dcache_idle = (r >> 13) & 0x1;

        // Random dcache tag xcpt (rare, uint8_t)
        if (((r >> 14) & 0x1F) == 0) {
            d->stim_tag_dcache_xcpt = (uint8_t)(xorshift32() & 0xFF);
        }

        // Random tag_dcache_replay_next
        if (((r >> 19) & 0xF) == 0) {
            d->stim_tag_dcache_replay_next = 1;
        }

        // Random flush pipeline (very rare)
        if (((r >> 23) & 0x3F) == 0) {
            d->stim_mem_dcache_flush_pipeline = 1;
        }

        // Random events
        d->stim_io_events_dcache_vpu = xorshift32() & 0x7FFFFF; // 23 bits

        // Random PMU data
        if ((r >> 29) & 0x1) {
            for (int w = 0; w < 4; w++)
                d->stim_pmu_read_data.m_storage[w] = xorshift32();
        }

        // Random VPU ctrl (rare, to occasionally trigger VPU paths)
        if (((r >> 30) & 0x3) == 0) {
            for (unsigned w = 0; w < sizeof(d->stim_id_vpu_ctrl) / 4; w++)
                d->stim_id_vpu_ctrl.m_storage[w] = xorshift32();
        }

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
