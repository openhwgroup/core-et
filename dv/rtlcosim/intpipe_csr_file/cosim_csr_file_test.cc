// Co-simulation test: intpipe_csr_file (new) vs original.
//
// Phase 1: post-reset idle (10 cycles)
// Phase 2: basic CSR write/read (mstatus, mepc, mtvec, etc.)
// Phase 3: privilege mode changes (mret)
// Phase 4: exception handling
// Phase 5: interrupt delivery
// Phase 6: debug mode (halt, resume, step)
// Phase 7: FLB operations
// Phase 8: random stimulus (~3000 cycles)

#include "Vcosim_csr_file_tb.h"
#include "cosim_ctrl.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>

using DUT = Vcosim_csr_file_tb;

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

// CSR command encodings (matches csr_cmd_e)
enum CsrCmd {
    CSR_CMD_N = 0, // No op
    CSR_CMD_W = 1, // Write
    CSR_CMD_S = 2, // Set bits
    CSR_CMD_C = 3, // Clear bits
    CSR_CMD_I = 4, // Idle/special
    CSR_CMD_R = 5  // Read
};

// Commonly used CSR addresses (from intpipe_csr_pkg)
enum CsrAddr {
    ADDR_FFLAGS    = 0x001,
    ADDR_FRM       = 0x002,
    ADDR_FCSR      = 0x003,
    ADDR_SSTATUS   = 0x100,
    ADDR_SIE       = 0x104,
    ADDR_STVEC     = 0x105,
    ADDR_SSCRATCH  = 0x140,
    ADDR_SEPC      = 0x141,
    ADDR_SCAUSE    = 0x142,
    ADDR_STVAL     = 0x143,
    ADDR_SIP       = 0x144,
    ADDR_SATP      = 0x180,
    ADDR_MSTATUS   = 0x300,
    ADDR_MISA      = 0x301,
    ADDR_MEDELEG   = 0x302,
    ADDR_MIDELEG   = 0x303,
    ADDR_MIE       = 0x304,
    ADDR_MTVEC     = 0x305,
    ADDR_MSCRATCH  = 0x340,
    ADDR_MEPC      = 0x341,
    ADDR_MCAUSE    = 0x342,
    ADDR_MTVAL     = 0x343,
    ADDR_MIP       = 0x344,
    ADDR_MATP      = 0x382,
    ADDR_DCSR      = 0x7b0,
    ADDR_DPC       = 0x7b1,
    ADDR_DSCRATCH  = 0x7b2,
    ADDR_TSELECT   = 0x7a0,
    ADDR_TDATA1    = 0x7a1,
    ADDR_TDATA2    = 0x7a2,
    ADDR_CYCLE     = 0xc00,
    ADDR_INSTRET   = 0xc02,
    ADDR_HARTID    = 0xcd0,
    ADDR_TENSOR_FMA = 0x801,
    ADDR_FLB       = 0x820,
    ADDR_FCC       = 0x821,
    ADDR_STALL     = 0x822,
};

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

// Compare ALL key outputs every cycle.
static void compare_all(CosimCtrl<DUT>& sim) {
    auto* d = sim.dut.get();

    // ── Control signals ──
    sim.compare("io_rw_wb_rdata",       d->orig_io_rw_wb_rdata,       d->new_io_rw_wb_rdata);
    sim.compare("io_csr_stall",         d->orig_io_csr_stall,         d->new_io_csr_stall);
    sim.compare("io_csr_xcpt",          d->orig_io_csr_xcpt,          d->new_io_csr_xcpt);
    sim.compare("io_eret",              d->orig_io_eret,              d->new_io_eret);
    sim.compare("io_replay",            d->orig_io_replay,            d->new_io_replay);
    sim.compare("io_resume",            d->orig_io_resume,            d->new_io_resume);
    sim.compare("io_singleStep",        d->orig_io_singleStep,        d->new_io_singleStep);
    sim.compare("io_excl_mode",         d->orig_io_excl_mode,         d->new_io_excl_mode);
    sim.compare("io_excl_mode_sel",     d->orig_io_excl_mode_sel,     d->new_io_excl_mode_sel);
    sim.compare("io_excl_mode_transition", d->orig_io_excl_mode_transition, d->new_io_excl_mode_transition);

    // ── Status / privilege / debug (flat packed: 2*6=12 bits, 2*2=4 bits) ──
    sim.compare("io_status",            d->orig_io_status,            d->new_io_status);
    sim.compare("io_prv",               d->orig_io_prv,               d->new_io_prv);
    sim.compare("io_debug",             d->orig_io_debug,             d->new_io_debug);

    // ── SATP / MATP ──
    sim.compare("io_satp",              d->orig_io_satp,              d->new_io_satp);
    sim.compare("io_satp_en",           d->orig_io_satp_en,           d->new_io_satp_en);
    sim.compare("io_matp",              d->orig_io_matp,              d->new_io_matp);
    sim.compare("io_matp_en",           d->orig_io_matp_en,           d->new_io_matp_en);

    // ── Evec ──
    sim.compare("io_evec",              d->orig_io_evec,              d->new_io_evec);
    sim.compare("io_evec_resume",       d->orig_io_evec_resume,       d->new_io_evec_resume);

    // ── Frontend / cache ──
    sim.compare("fe_ctrl_stall",        d->orig_fe_ctrl_stall,        d->new_fe_ctrl_stall);
    sim.compare("icache_invalidate",    d->orig_icache_invalidate,    d->new_icache_invalidate);
    sim.compare("tlb_invalidate",       d->orig_tlb_invalidate,       d->new_tlb_invalidate);

    // ── FCSR ──
    sim.compare("io_fcsr_rm_thread0",   d->orig_io_fcsr_rm_thread0,   d->new_io_fcsr_rm_thread0);
    sim.compare("io_fcsr_rm_thread1",   d->orig_io_fcsr_rm_thread1,   d->new_io_fcsr_rm_thread1);

    // ── Interrupts (flat: 2 bits, 2*5=10 bits) ──
    sim.compare("io_interrupt",         d->orig_io_interrupt,         d->new_io_interrupt);
    sim.compare("io_interrupt_cause",   d->orig_io_interrupt_cause,   d->new_io_interrupt_cause);

    // ── Dcache / VPU ctrl (VlWide comparison) ──
    compare_wide(sim, "dcache_ctrl",    d->orig_dcache_ctrl,          d->new_dcache_ctrl);
    compare_wide(sim, "vpu_ctrl",       d->orig_vpu_ctrl,             d->new_vpu_ctrl);

    // ── Breakpoint (flat: 2*9=18 bits, 2*48=96 bits) ──
    sim.compare("bp_control",           d->orig_bp_control,           d->new_bp_control);
    compare_wide(sim, "bp_address",     d->orig_bp_address,           d->new_bp_address);

    // ── Debug ──
    sim.compare("update_ddata0",        d->orig_update_ddata0,        d->new_update_ddata0);
    sim.compare("ddata0_wdata",         d->orig_ddata0_wdata,         d->new_ddata0_wdata);
    sim.compare("busy",                 d->orig_busy,                 d->new_busy);
    sim.compare("debug_out_exception",  d->orig_debug_out_exception,  d->new_debug_out_exception);
    sim.compare("xcpt_traps_to_debug",  d->orig_xcpt_traps_to_debug,  d->new_xcpt_traps_to_debug);

    // ── FLB ──
    sim.compare("flb_neigh_req_valid",  d->orig_flb_neigh_req_valid,  d->new_flb_neigh_req_valid);
    sim.compare("flb_neigh_req_data",   d->orig_flb_neigh_req_data,   d->new_flb_neigh_req_data);
    sim.compare("flb_rf_wen_valid",     d->orig_flb_rf_wen_valid,     d->new_flb_rf_wen_valid);
    sim.compare("flb_rf_wen_valid_early", d->orig_flb_rf_wen_valid_early, d->new_flb_rf_wen_valid_early);
    sim.compare("flb_rf_wen_thread_id", d->orig_flb_rf_wen_thread_id, d->new_flb_rf_wen_thread_id);
    sim.compare("flb_rf_wen_addr",      d->orig_flb_rf_wen_addr,      d->new_flb_rf_wen_addr);
    sim.compare("flb_rf_wen_data",      d->orig_flb_rf_wen_data,      d->new_flb_rf_wen_data);
    sim.compare("flb_scoreboard_valid", d->orig_flb_scoreboard_valid, d->new_flb_scoreboard_valid);
    sim.compare("flb_scoreboard_addr",  d->orig_flb_scoreboard_addr,  d->new_flb_scoreboard_addr);

    // ── Trace encoder ──
    sim.compare("TE_prv",               d->orig_TE_prv,               d->new_TE_prv);
    sim.compare("TE_exception",         d->orig_TE_exception,         d->new_TE_exception);
    sim.compare("TE_tval",              d->orig_TE_tval,              d->new_TE_tval);
    sim.compare("TE_wb_reg_cause_ie",   d->orig_TE_wb_reg_cause_ie,   d->new_TE_wb_reg_cause_ie);

    // ── Minstmask / minstmatch (flat: 2*33=66 bits, 2*32=64 bits) ──
    compare_wide(sim, "io_minstmask",   d->orig_io_minstmask,         d->new_io_minstmask);
    sim.compare("io_minstmatch",        d->orig_io_minstmatch,        d->new_io_minstmatch);

    // ── GSC progress (flat: 2*3=6 bits) ──
    sim.compare("gsc_progress",         d->orig_gsc_progress,         d->new_gsc_progress);

    // ── PMU ──
    sim.compare("pmu_count_up",         d->orig_pmu_count_up,         d->new_pmu_count_up);
    sim.compare("pmu_read_sel",         d->orig_pmu_read_sel,         d->new_pmu_read_sel);
    sim.compare("pmu_write_en",         d->orig_pmu_write_en,         d->new_pmu_write_en);
    sim.compare("pmu_write_data",       d->orig_pmu_write_data,       d->new_pmu_write_data);
    sim.compare("pmu_neigh_event_sel",  d->orig_pmu_neigh_event_sel,  d->new_pmu_neigh_event_sel);
}

// Set all stimulus inputs to idle (inactive) state.
static void idle_inputs(DUT* d) {
    d->stim_test_en = 0;
    d->stim_dft_reset_byp = 0;
    d->stim_dft_reset = 0;

    d->stim_int_mieco = 0;
    d->stim_int_mtip = 0;
    d->stim_int_msip = 0;
    d->stim_int_meip = 0;
    d->stim_int_seip = 0;
    d->stim_int_fcc = 0;
    d->stim_shire_id = 0;
    d->stim_shire_min_id = 0;
    d->stim_wb_thread_id = 0;
    d->stim_mem_thread_id = 0;

    d->stim_pending_scoreboard_t0 = 0;
    d->stim_vpu_tfma_enabled = 0;
    d->stim_vpu_tfma_tenb_working = 0;
    d->stim_vpu_tquant_enabled = 0;
    d->stim_vpu_treduce_enabled = 0;
    d->stim_dcache_ctrl_resp = 0;

    d->stim_io_rw_mem_addr = 0;
    d->stim_io_rw_wb_cmd = CSR_CMD_N;
    d->stim_io_rw_mem_cmd = CSR_CMD_N;
    d->stim_io_rw_wb_inst = 0;
    d->stim_io_rw_mem_wdata = 0;
    d->stim_io_rw_wb_wdata = 0;
    d->stim_io_rw_wb_waddr = 0;
    d->stim_wb_x31_reg = 0;

    d->stim_io_exception = 0;
    d->stim_io_retire = 0;
    d->stim_io_cause = 0;
    d->stim_io_pc = 0;
    d->stim_io_badaddr = 0;
    d->stim_io_inst_bits = 0;
    d->stim_io_inst_rvc = 0;
    d->stim_io_bad_redirect = 0;
    d->stim_io_redirect = 0;

    d->stim_io_fcsr_flags_valid = 0;
    d->stim_io_fcsr_flags_bits = 0;
    d->stim_io_fcsr_thread_id = 0;

    d->stim_dcache_bus_error = 0;
    memset(&d->stim_dcache_bus_error_addr, 0, sizeof(d->stim_dcache_bus_error_addr));

    d->stim_debug_timing = 0;
    d->stim_bp_progress_save = 0;
    memset(&d->stim_read_ddata0, 0, sizeof(d->stim_read_ddata0));
    d->stim_halt = 0;
    d->stim_resume = 0;
    d->stim_ex_progbuf = 0;

    d->stim_flb_neigh_resp_valid = 0;
    d->stim_flb_neigh_resp_data = 0;
    d->stim_flb_rf_wen_ready = 1;  // Ready by default

    d->stim_l2_resp_valid = 0;
    d->stim_l2_resp_ready = 0;
    memset(&d->stim_l2_resp, 0, sizeof(d->stim_l2_resp));

    d->stim_io_events_ext = 0;
    d->stim_te_thread_sel = 0;

    d->stim_esr_shire_coop_mode = 0;
    d->stim_esr_features = 0;

    d->stim_tensor_load_err_flags = 0;
    d->stim_cache_ops_err_flags = 0;  // flat 4-bit packed vector
    d->stim_tensor_reduce_err_flags = 0;

    memset(&d->stim_pmu_read_data, 0, sizeof(d->stim_pmu_read_data));

    d->stim_dcache_debug_signals = 0;
}

// Issue a CSR write in the 2-stage MEM/WB protocol.
// Cycle 1: set mem_addr, mem_cmd, mem_wdata (MEM stage)
// Cycle 2: set wb_cmd, wb_wdata (WB stage with retire)
static void csr_write(CosimCtrl<DUT>& sim, uint16_t addr, uint64_t data,
                      int cmd = CSR_CMD_W, int thread = 0) {
    auto* d = sim.dut.get();

    // MEM stage: present address and command
    idle_inputs(d);
    d->stim_io_rw_mem_addr = addr;
    d->stim_io_rw_mem_cmd = cmd;
    d->stim_io_rw_mem_wdata = data;
    d->stim_mem_thread_id = thread;
    d->stim_wb_thread_id = thread;
    sim.tick();
    compare_all(sim);

    // WB stage: present write data and command, with retire
    idle_inputs(d);
    d->stim_io_rw_wb_cmd = cmd;
    d->stim_io_rw_wb_wdata = data;
    d->stim_io_retire = 1;
    d->stim_wb_thread_id = thread;
    d->stim_mem_thread_id = thread;
    sim.tick();
    compare_all(sim);
}

// Issue a CSR read: MEM stage sets addr+read cmd, WB stage reads data.
static void csr_read(CosimCtrl<DUT>& sim, uint16_t addr, int thread = 0) {
    auto* d = sim.dut.get();

    // MEM stage
    idle_inputs(d);
    d->stim_io_rw_mem_addr = addr;
    d->stim_io_rw_mem_cmd = CSR_CMD_R;
    d->stim_mem_thread_id = thread;
    d->stim_wb_thread_id = thread;
    sim.tick();
    compare_all(sim);

    // WB stage
    idle_inputs(d);
    d->stim_io_rw_wb_cmd = CSR_CMD_R;
    d->stim_io_retire = 1;
    d->stim_wb_thread_id = thread;
    d->stim_mem_thread_id = thread;
    sim.tick();
    compare_all(sim);
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
    // Phase 1: Post-reset idle — verify initial state is identical
    // ════════════════════════════════════════════════════════════════
    for (int i = 0; i < 10; i++) {
        sim.tick();
        compare_all(sim);
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 2: Basic CSR write/read
    // ════════════════════════════════════════════════════════════════

    // Write MTVEC (M-mode trap vector)
    csr_write(sim, ADDR_MTVEC, 0x0000000080001000ULL);
    // Read it back
    csr_read(sim, ADDR_MTVEC);

    // Idle cycles between operations
    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Write MSCRATCH
    csr_write(sim, ADDR_MSCRATCH, 0xDEADBEEFCAFE0123ULL);
    csr_read(sim, ADDR_MSCRATCH);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Write MEPC
    csr_write(sim, ADDR_MEPC, 0x0000000080002000ULL);
    csr_read(sim, ADDR_MEPC);

    // Write MSTATUS (set MIE, MPP to U-mode)
    // mstatus MPP field is bits [12:11], MIE is bit [3]
    csr_write(sim, ADDR_MSTATUS, (0ULL << 11) | (1ULL << 3));
    csr_read(sim, ADDR_MSTATUS);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Write to MSTATUS with MPP = M (3), MIE=1
    csr_write(sim, ADDR_MSTATUS, (3ULL << 11) | (1ULL << 3));
    csr_read(sim, ADDR_MSTATUS);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Read MISA (read-only)
    csr_read(sim, ADDR_MISA);

    // Write FCSR
    csr_write(sim, ADDR_FCSR, 0x000000FF);
    csr_read(sim, ADDR_FCSR);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Test FFLAGS
    csr_write(sim, ADDR_FFLAGS, 0x1F);
    csr_read(sim, ADDR_FFLAGS);

    // Test FRM
    csr_write(sim, ADDR_FRM, 0x4);
    csr_read(sim, ADDR_FRM);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Write HARTID CSR (read via csr_read)
    csr_read(sim, ADDR_HARTID);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 2b: Test thread 1 CSR access
    // ════════════════════════════════════════════════════════════════
    csr_write(sim, ADDR_MSCRATCH, 0x1111111122222222ULL, CSR_CMD_W, 1);
    csr_read(sim, ADDR_MSCRATCH, 1);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Read thread 0 scratch — should be the value we wrote earlier
    csr_read(sim, ADDR_MSCRATCH, 0);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 2c: CSR set/clear operations
    // ════════════════════════════════════════════════════════════════
    // Set bits in MIE
    csr_write(sim, ADDR_MIE, (1ULL << 11) | (1ULL << 7) | (1ULL << 3), CSR_CMD_S);
    csr_read(sim, ADDR_MIE);

    // Clear bits in MIE
    csr_write(sim, ADDR_MIE, (1ULL << 3), CSR_CMD_C);
    csr_read(sim, ADDR_MIE);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 3: Privilege mode changes (mret)
    // ════════════════════════════════════════════════════════════════

    // First set MSTATUS.MPP = S-mode (1) and set MEPC
    csr_write(sim, ADDR_MSTATUS, (1ULL << 11) | (1ULL << 7)); // MPP=S, MPIE=1
    csr_write(sim, ADDR_MEPC, 0x0000000080004000ULL);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Execute mret: wb_inst = 0b001 (MRET encoding in bits 22:20 = 3'b001, but
    // the system insn decode uses io_rw_wb_inst).
    // MRET = system insn with funct3=PRIV, rs2=0b00010
    // io_rw_wb_cmd should be non-N for the system insn path.
    // Actually, mret goes through the system_insn_wb path in the CSR file.
    // The instruction bits [22:20] = 3'b000 for ECALL, 3'b001 for EBREAK,
    // 3'b010 for URET, 3'b100 for SRET, 3'b110 for MRET, 3'b001 for WFI
    // (varies — use 3'b110 = MRET)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_rw_mem_addr = ADDR_MSTATUS;  // system insn
        d->stim_io_rw_mem_cmd = CSR_CMD_I;       // system instruction
        d->stim_mem_thread_id = 0;
        d->stim_wb_thread_id = 0;
        sim.tick();
        compare_all(sim);

        idle_inputs(d);
        d->stim_io_rw_wb_cmd = CSR_CMD_I;
        d->stim_io_rw_wb_inst = 6;  // 3'b110 = MRET
        d->stim_io_retire = 1;
        d->stim_wb_thread_id = 0;
        d->stim_mem_thread_id = 0;
        sim.tick();
        compare_all(sim);
    }

    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 4: Exception handling
    // ════════════════════════════════════════════════════════════════

    // First, ensure we are in M-mode by writing MSTATUS.MPP=M and doing mret
    csr_write(sim, ADDR_MSTATUS, (3ULL << 11) | (1ULL << 7)); // MPP=M, MPIE=1
    csr_write(sim, ADDR_MEPC, 0x0000000080000000ULL);

    {
        auto* d = sim.dut.get();
        // MRET system insn
        idle_inputs(d);
        d->stim_io_rw_mem_addr = ADDR_MSTATUS;
        d->stim_io_rw_mem_cmd = CSR_CMD_I;
        d->stim_mem_thread_id = 0;
        d->stim_wb_thread_id = 0;
        sim.tick(); compare_all(sim);

        idle_inputs(d);
        d->stim_io_rw_wb_cmd = CSR_CMD_I;
        d->stim_io_rw_wb_inst = 6;
        d->stim_io_retire = 1;
        d->stim_wb_thread_id = 0;
        d->stim_mem_thread_id = 0;
        sim.tick(); compare_all(sim);
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Now trigger an exception: illegal instruction (cause = 2)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_exception = 1;
        d->stim_io_cause = 2;  // illegal instruction
        d->stim_io_pc = 0x80003000;
        d->stim_io_badaddr = 0;
        d->stim_io_inst_bits = 0xDEADBEEF;
        d->stim_wb_thread_id = 0;
        d->stim_mem_thread_id = 0;
        sim.tick();
        compare_all(sim);
    }

    idle_inputs(sim.dut.get());
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }

    // Trigger another exception type: load page fault (cause = 13)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_exception = 1;
        d->stim_io_cause = 0xD;  // load page fault
        d->stim_io_pc = 0x80004000;
        d->stim_io_badaddr = 0x7FFFFFFFDEADULL;
        d->stim_io_inst_bits = 0x00052503; // lw a0, 0(a0)
        d->stim_wb_thread_id = 0;
        d->stim_mem_thread_id = 0;
        sim.tick();
        compare_all(sim);
    }

    idle_inputs(sim.dut.get());
    for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 5: Interrupt delivery
    // ════════════════════════════════════════════════════════════════

    // Enable machine interrupts in MIE
    csr_write(sim, ADDR_MIE, (1ULL << 11) | (1ULL << 7) | (1ULL << 3), CSR_CMD_S);
    // Enable MIE bit in MSTATUS
    csr_write(sim, ADDR_MSTATUS, (1ULL << 3), CSR_CMD_S);

    for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

    // Assert MTIP (machine timer interrupt pending)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_int_mtip = 0x1;  // thread 0 timer interrupt
        sim.tick();
        compare_all(sim);

        // Keep it asserted for a few cycles
        for (int i = 0; i < 5; i++) {
            sim.tick();
            compare_all(sim);
        }

        // Deassert
        d->stim_int_mtip = 0;
        sim.tick();
        compare_all(sim);
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Assert MEIP (machine external interrupt)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_int_meip = 0x1;
        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }
        d->stim_int_meip = 0;
        sim.tick(); compare_all(sim);
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Assert MSIP (machine software interrupt)
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_int_msip = 0x2;  // thread 1
        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }
        d->stim_int_msip = 0;
        sim.tick(); compare_all(sim);
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 6: Debug mode (halt, resume, step)
    // ════════════════════════════════════════════════════════════════

    // Halt thread 0
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_halt = 0x1;
        sim.tick(); compare_all(sim);
        d->stim_halt = 0;

        // Stay in debug for a few cycles
        for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }

        // Resume
        d->stim_resume = 0x1;
        sim.tick(); compare_all(sim);
        d->stim_resume = 0;

        for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    }

    // Halt thread 1
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_halt = 0x2;
        sim.tick(); compare_all(sim);
        d->stim_halt = 0;

        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

        d->stim_resume = 0x2;
        sim.tick(); compare_all(sim);
        d->stim_resume = 0;

        for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 7: FLB operations
    // ════════════════════════════════════════════════════════════════

    // Write to FLB CSR address to start a barrier
    csr_write(sim, ADDR_FLB, 0x0001);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Simulate neighbor response
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_flb_neigh_resp_valid = 1;
        d->stim_flb_neigh_resp_data = 1;
        sim.tick(); compare_all(sim);
        d->stim_flb_neigh_resp_valid = 0;

        for (int i = 0; i < 10; i++) { sim.tick(); compare_all(sim); }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 7b: FCSR flags from VPU
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_fcsr_flags_valid = 1;
        d->stim_io_fcsr_flags_bits = 0x1F;  // all flags
        d->stim_io_fcsr_thread_id = 0;
        sim.tick(); compare_all(sim);

        idle_inputs(d);
        d->stim_io_fcsr_flags_valid = 1;
        d->stim_io_fcsr_flags_bits = 0x0A;
        d->stim_io_fcsr_thread_id = 1;
        sim.tick(); compare_all(sim);
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Read FCSR for both threads
    csr_read(sim, ADDR_FCSR, 0);
    csr_read(sim, ADDR_FCSR, 1);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7c: Delegation and S-mode CSR writes
    // ════════════════════════════════════════════════════════════════

    // Set MEDELEG to delegate page faults to S-mode
    csr_write(sim, ADDR_MEDELEG, (1ULL << 12) | (1ULL << 13) | (1ULL << 15));
    csr_read(sim, ADDR_MEDELEG);

    // Set MIDELEG to delegate S-mode interrupts
    csr_write(sim, ADDR_MIDELEG, (1ULL << 1) | (1ULL << 5) | (1ULL << 9));
    csr_read(sim, ADDR_MIDELEG);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // Write STVEC
    csr_write(sim, ADDR_STVEC, 0x80010000ULL);
    csr_read(sim, ADDR_STVEC);

    // Write SATP
    csr_write(sim, ADDR_SATP, 0x8000000000001000ULL);
    csr_read(sim, ADDR_SATP);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7d: PMU counter reads and events
    // ════════════════════════════════════════════════════════════════
    csr_read(sim, ADDR_CYCLE);
    csr_read(sim, ADDR_INSTRET);

    // Pulse events
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_events_ext = 0x1FFFFFF;  // all events active
        d->stim_io_retire = 1;
        sim.tick(); compare_all(sim);
    }

    idle_inputs(sim.dut.get());
    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7e: Bus error inputs
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_dcache_bus_error = 0x1;  // thread 0 bus error
        // stim_dcache_bus_error_addr is flat 80 bits [1:0][39:0]
        d->stim_dcache_bus_error_addr.m_storage[0] = 0xBEEF0000U;
        d->stim_dcache_bus_error_addr.m_storage[1] = 0;
        d->stim_dcache_bus_error_addr.m_storage[2] = 0;
        sim.tick(); compare_all(sim);
        d->stim_dcache_bus_error = 0;
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7f: Exclusive mode
    // ════════════════════════════════════════════════════════════════
    // Write STALL CSR to trigger exclusive mode
    csr_write(sim, ADDR_STALL, 0x1);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    csr_write(sim, ADDR_STALL, 0x0);

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7g: Dcache ctrl resp inputs
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        // Set cache_op_ready bit (MSB of dcache_core_ctrl_t)
        d->stim_dcache_ctrl_resp = 1;  // some control response bits
        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }
        d->stim_dcache_ctrl_resp = 0;
    }

    for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }

    // ════════════════════════════════════════════════════════════════
    // Phase 7h: Redirect inputs
    // ════════════════════════════════════════════════════════════════
    {
        auto* d = sim.dut.get();
        idle_inputs(d);
        d->stim_io_redirect = 0x1;  // thread 0 redirect
        sim.tick(); compare_all(sim);
        d->stim_io_redirect = 0;

        for (int i = 0; i < 3; i++) { sim.tick(); compare_all(sim); }

        d->stim_io_bad_redirect = 0x2;  // thread 1 bad redirect
        sim.tick(); compare_all(sim);
        d->stim_io_bad_redirect = 0;

        for (int i = 0; i < 5; i++) { sim.tick(); compare_all(sim); }
    }

    // ════════════════════════════════════════════════════════════════
    // Phase 8: Random stimulus (~3000 cycles)
    // ════════════════════════════════════════════════════════════════

    // Table of valid CSR addresses for random access (M-mode only for simplicity)
    static const uint16_t csr_addrs[] = {
        ADDR_FFLAGS, ADDR_FRM, ADDR_FCSR,
        ADDR_MSTATUS, ADDR_MIE, ADDR_MTVEC, ADDR_MSCRATCH,
        ADDR_MEPC, ADDR_MCAUSE, ADDR_MTVAL, ADDR_MIP,
        ADDR_MEDELEG, ADDR_MIDELEG,
        ADDR_SSTATUS, ADDR_SIE, ADDR_STVEC, ADDR_SSCRATCH,
        ADDR_SEPC, ADDR_SCAUSE, ADDR_STVAL, ADDR_SIP,
        ADDR_SATP, ADDR_MATP,
        ADDR_DCSR, ADDR_DPC, ADDR_DSCRATCH,
        ADDR_TSELECT, ADDR_TDATA1, ADDR_TDATA2,
        ADDR_CYCLE, ADDR_INSTRET,
        ADDR_HARTID,
        ADDR_TENSOR_FMA, ADDR_FCC, ADDR_STALL,
    };
    static const int num_addrs = sizeof(csr_addrs) / sizeof(csr_addrs[0]);

    for (int cycle = 0; cycle < 3000; cycle++) {
        auto* d = sim.dut.get();
        uint32_t r = xorshift32();

        idle_inputs(d);

        // Thread IDs
        d->stim_wb_thread_id = (r >> 0) & 1;
        d->stim_mem_thread_id = (r >> 1) & 1;

        // Random CSR command (low probability of writes to avoid too many state changes)
        int action = (r >> 2) & 0xF;  // 4 bits

        if (action == 0) {
            // CSR write
            uint16_t addr = csr_addrs[xorshift32() % num_addrs];
            int cmd = 1 + (xorshift32() % 3);  // W, S, or C
            d->stim_io_rw_mem_addr = addr;
            d->stim_io_rw_mem_cmd = cmd;
            d->stim_io_rw_mem_wdata = xorshift64();
        } else if (action == 1) {
            // CSR WB stage command
            int cmd = 1 + (xorshift32() % 3);
            d->stim_io_rw_wb_cmd = cmd;
            d->stim_io_rw_wb_wdata = xorshift64();
            d->stim_io_retire = 1;
        } else if (action == 2) {
            // CSR read (MEM stage)
            uint16_t addr = csr_addrs[xorshift32() % num_addrs];
            d->stim_io_rw_mem_addr = addr;
            d->stim_io_rw_mem_cmd = CSR_CMD_R;
        } else if (action == 3) {
            // CSR read WB stage
            d->stim_io_rw_wb_cmd = CSR_CMD_R;
            d->stim_io_retire = 1;
        }

        // Random interrupt inputs (low probability)
        if ((r >> 6) & 0x1) d->stim_int_mieco = xorshift32() & 0x3;
        if ((r >> 7) & 0x1) d->stim_int_mtip  = xorshift32() & 0x3;
        if ((r >> 8) & 0x1) d->stim_int_msip  = xorshift32() & 0x3;
        if ((r >> 9) & 0x1) d->stim_int_meip  = xorshift32() & 0x3;
        if ((r >> 10) & 0x1) d->stim_int_seip = xorshift32() & 0x3;

        // Random FCC
        if ((r >> 11) & 0x1) d->stim_int_fcc = xorshift32() & 0xF;

        // Random exception (rare)
        if (((r >> 12) & 0x1F) == 0) {
            d->stim_io_exception = 1;
            d->stim_io_cause = xorshift32() & 0x1F;
            d->stim_io_pc = xorshift64() & 0x1FFFFFFFFFFFFULL; // 49-bit
            d->stim_io_badaddr = xorshift64() & 0x1FFFFFFFFFFFFULL;
            d->stim_io_inst_bits = xorshift32();
            d->stim_io_inst_rvc = xorshift32() & 1;
        }

        // Random retire
        if ((r >> 17) & 0x1) d->stim_io_retire = 1;

        // Random redirect
        if (((r >> 18) & 0x7) == 0) d->stim_io_redirect = xorshift32() & 0x3;
        if (((r >> 21) & 0xF) == 0) d->stim_io_bad_redirect = xorshift32() & 0x3;

        // Random FCSR flags
        if (((r >> 24) & 0x7) == 0) {
            d->stim_io_fcsr_flags_valid = 1;
            d->stim_io_fcsr_flags_bits = xorshift32() & 0x3F;
            d->stim_io_fcsr_thread_id = xorshift32() & 1;
        }

        // Random events
        d->stim_io_events_ext = xorshift32() & 0x1FFFFFF;

        // Random debug
        if (((r >> 27) & 0x1F) == 0) d->stim_halt = xorshift32() & 0x3;
        if (((r >> 28) & 0x1F) == 0) d->stim_resume = xorshift32() & 0x3;

        // Random dcache ctrl resp
        if ((r >> 30) & 0x1) d->stim_dcache_ctrl_resp = xorshift64();

        // Random PMU data (128-bit VlWide<4>)
        if ((r >> 31) & 0x1) {
            for (int w = 0; w < 4; w++)
                d->stim_pmu_read_data.m_storage[w] = xorshift32();
        }

        // Random bus error (rare)
        if (((xorshift32() >> 0) & 0x3F) == 0) {
            d->stim_dcache_bus_error = xorshift32() & 0x3;
            // 80-bit flat VlWide<3>: [thread0 39:0] [thread1 39:0]
            for (int w = 0; w < 3; w++)
                d->stim_dcache_bus_error_addr.m_storage[w] = xorshift32();
            d->stim_dcache_bus_error_addr.m_storage[2] &= 0xFFFF; // mask to 80 bits
        }

        // Random FLB inputs
        if (((xorshift32() >> 4) & 0xF) == 0) {
            d->stim_flb_neigh_resp_valid = 1;
            d->stim_flb_neigh_resp_data = xorshift32() & 1;
        }

        // Random dcache debug signals
        d->stim_dcache_debug_signals = xorshift64();

        // Random error flags
        d->stim_tensor_load_err_flags = xorshift32() & 0x7;
        d->stim_tensor_reduce_err_flags = xorshift32() & 0x1;
        d->stim_cache_ops_err_flags = xorshift32() & 0xF;  // flat 4-bit packed

        // Random ESR features (rare changes)
        if (((xorshift32() >> 8) & 0x3F) == 0) {
            d->stim_esr_features = xorshift32() & 0x3F;
            d->stim_esr_shire_coop_mode = xorshift32() & 1;
        }

        sim.tick();
        compare_all(sim);
    }

    return sim.finish();
}
