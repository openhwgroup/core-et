/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Global
#include <cassert>
#include <cmath>
#include <exception>
#include <fcntl.h>
#include <queue>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Local
#include "cache.h"
#include "checker.h"
#include "cosim_error.h"
#include "system.h"
#include "emu_defines.h"
#include "emu_gio.h"
#include "esrs.h"
#include "fpu/fpu_casts.h"
#include "insn.h"
#include "processor.h"
#include "riscv_disasm.h"
#include "sysreg_error.h"
#include "tensor.h"

// Defines
#define TBOX_REGION_START 0xFFF80000
#define TBOX_REGION_END (TBOX_REGION_START + 512)

#define ESR_SHIRE_REGION_START 0x100340000L
#define ESR_SHIRE_REGION_END 0x1FFF5FFF8L
#define ESR_MTIME 0x80f40200ull

#ifdef DEBUG_STATE_CHANGES
// Used for debugging the checker
std::ostringstream& operator<<(std::ostringstream& os, const inst_state_change& state)
{
    std::ios_base::fmtflags ff = os.flags();
    os.setf(std::ios_base::showbase);

    os << "{pc_mod=" << (state.pc_mod ? "Y" : "N") << std::hex << ", pc=" << state.pc
       << ", inst_bits=" << state.inst_bits << std::dec;
    if (state.int_reg_mod) {
        os << ", x" << state.int_reg_rd << "=" << std::hex << state.int_reg_rd << std::dec;
    }
    if (state.fp_reg_mod) {
        os << ", f" << state.fp_reg_rd << "=";
        for (int i = 0; i < bemu::VLEND; ++i) {
            os << (i == 0 ? "[" : ", ") << std::hex << uint32_t(state.fp_reg_data[i] & 0xffffffff) << ", "
               << uint32_t((state.fp_reg_data[i] >> 32) & 0xffffffff) << std::dec;
        }
        os << "]";
    }
    for (int m = 0; m < 8; ++m) {
        if (!state.m_reg_mod[m]) continue;
        uint32_t mval = 0;
        for (int i = 0; i < bemu::VLENW; ++i) mval |= ((state.m_reg_data[m][i] != 0) << i);
        os << ", m" << m << "=" << std::hex << mval << std::dec;
    }
    for (int i = 0; i < bemu::VLENW; ++i) {
        if (!state.mem_mod[i]) continue;
        os << ", MEM[" << std::hex << state.mem_vaddr[i] << "..." << (state.mem_vaddr[i] + state.mem_size[i] - 1)
           << "]=" << state.mem_data[i] << std::dec;
    }
    os << "}";
    os.flags(ff);
    return os;
}
#endif // DEBUG_STATE_CHANGES

bool fp_1ulp_check(uint32_t gold, uint32_t rtl)
{
    int exp_gold = (gold >> 23) & 0xFF;

    if ((exp_gold > 0) && (exp_gold < 253)) // just check regular cases (skip special denom, NaN, Inf)
    {
        uint32_t gold_clean = gold & 0x7F800000; // clean mantissa and sign from gold
        float err_1ulp = fpu::FLT(gold_clean);
        err_1ulp = err_1ulp / float(1 << 23); // put '1' in the unit of less precision

        float goldf = fpu::FLT(gold);
        float rtlf = fpu::FLT(rtl);
        float diff = fabsf(goldf - rtlf);
        // printf("Gold: %.12e, RTL: %.12e, Diff: %.12e, Max: %.12e\n", goldf, rtlf, diff, err_1ulp);
        // printf("Hex Gold: %08X, Hex RTL: %08X\n", gold, rtl);
        return (diff <= err_1ulp);
    } else if ((gold & 0x7FFFFFFF) == 0) // allow sign (+/-) mismatch in case of zero
    {
        return ((rtl & 0x7FFFFFFF) == 0);
    } else // regular full check for special cases
    {
        return (gold == rtl);
    }
}

// Used to generate which store data bits to check for different store data sizes
uint64_t mem_mask(int32_t size)
{
    uint64_t mask;
    switch (size) {
    case 1:
        mask = 0x00000000000000FFULL;
        break;
    case 2:
        mask = 0x000000000000FFFFULL;
        break;
    case 4:
        mask = 0x00000000FFFFFFFFULL;
        break;
    default:
        mask = 0xFFFFFFFFFFFFFFFFULL;
    }
    return mask;
}

// Singleton class
checker* checker_instance
    = NULL; // this is used when enabling the second thread from the emu, to have an object to handle the call
            // if there is more than 1 checker instance (e.g. one per shire), this will have to be an array

void checker_thread1_enabled(unsigned minionId, uint64_t en, uint64_t pc)
{
    checker_instance->thread1_enabled(minionId, en, pc);
}

static void set_xreg(bemu::System& system, unsigned thread, bemu::xreg dst, uint64_t val)
{
    if (dst != bemu::x0) {
        system.cpu[thread].xregs[dst] = val;
    }
}

static void set_freg(bemu::System& system, unsigned thread, bemu::freg dst, uint64_t val[bemu::VLEND])
{
    for (size_t i = 0; i < bemu::VLEND; ++i) {
        system.cpu[thread].fregs[dst].u64[i] = val[i];
    }
}

static unsigned long get_mask(bemu::System& system, unsigned thread)
{
    return system.cpu[thread].mregs[bemu::m0].to_ulong();
}

// Creates a new checker
checker::checker(bool checker_en) : log(system_.log)
{
    waived_csrs.push_back(bemu::CSR_VALIDATION0);
    waived_csrs.push_back(bemu::CSR_VALIDATION1);
    waived_csrs.push_back(bemu::CSR_VALIDATION2);
    waived_csrs.push_back(bemu::CSR_VALIDATION3);

    waived_csrs.push_back(bemu::CSR_MCYCLE);
    waived_csrs.push_back(bemu::CSR_MINSTRET);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER3);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER4);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER5);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER6);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER7);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER8);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER9);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER10);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER11);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER12);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER13);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER14);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER15);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER16);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER17);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER18);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER19);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER20);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER21);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER22);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER23);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER24);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER25);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER26);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER27);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER28);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER29);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER30);
    waived_csrs.push_back(bemu::CSR_MHPMCOUNTER31);

    waived_csrs.push_back(bemu::CSR_CYCLE);
    waived_csrs.push_back(bemu::CSR_INSTRET);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER3);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER4);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER5);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER6);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER7);
    waived_csrs.push_back(bemu::CSR_HPMCOUNTER8);

    waived_csrs.push_back(bemu::CSR_DCACHE_DEBUG);

    waived_csrs.push_back(bemu::CSR_FCCNB);

    waived_esrs.push_back(ESR_MTIME);

    checker_instance = this;
    checker_enabled = checker_en;
    system_.init(bemu::System::Stepping::A0);
    system_.set_delayed_msg_port_write(true);
    system_.cold_reset();
    for (int i = 0; i < EMU_NUM_SHIRES; i++) {
        system_.begin_warm_reset(i);
        system_.end_warm_reset(i);
    }
    for (int i = 0; i < EMU_NUM_THREADS; i++) {
        system_.cpu[i].warm_reset();
        // TODO: on Erbium use ESR?
        system_.cpu[i].start_running();
    }

    for (int i = 0; i < EMU_NUM_THREADS; i++) {
        threadEnabled[i] = true;
        reduce_list_read_idx[i] = 0;
        reduce_list_write_idx[i] = 0;
        std::list<tensorstore_entry> new_list;
        tensorstore_list[i].push_back(new_list);
    }
    reduce_is_receive = false;

    // Do not check addresses falling in some unpredictable ESR regions
    add_ignored_mem_mask(0x100300000ULL, 0xFF003E0000ULL); // Shire Cache ESRs
    add_ignored_mem_mask(0x100320000ULL, 0xFF003E0000ULL); // Shire Rbox  ESRs

    // Do not check addresses falling in the Memshire and DDRC ESR regions
    for (uint64_t shire = 232; shire <= 239; ++shire) {
        add_ignored_mem_mask(0x100000000ULL | (shire << 22), 0x13FC00000ULL);
    }

    // Do not check addresses falling in PCIE region
    add_ignored_mem_region(0x4000000000ULL, 0x7FFFFFFFFFULL);

#if EMU_HAS_PU || EMU_HAS_SPIO
    // Set UART TX file descriptors to /dev/null or stdout
    int uart_fd = open("/dev/null", O_WRONLY, 0666);
    if (uart_fd < 0) uart_fd = STDOUT_FILENO;
#if EMU_HAS_PU
    if (system_.pu_uart0_get_tx_fd() < 0)
        system_.pu_uart0_set_tx_fd(uart_fd);
    if (system_.pu_uart1_get_tx_fd() < 0)
        system_.pu_uart1_set_tx_fd(uart_fd);
#endif
#if EMU_HAS_SPIO
    if (system_.spio_uart0_get_tx_fd() < 0)
        system_.spio_uart0_set_tx_fd(uart_fd);
    if (system_.spio_uart1_get_tx_fd() < 0)
        system_.spio_uart1_set_tx_fd(uart_fd);
#endif
#endif
}

// Destroys the checker
checker::~checker()
{
    checker_instance = NULL;
}

// Sets the PC
void checker::start_pc(uint32_t thread, uint64_t pc)
{
    if (thread >= EMU_NUM_THREADS) log << LOG_ERR << "start pc with thread invalid (" << thread << ")" << endm;
    system_.cpu[thread].pc = pc;
}

// Sets the PC due IPI
void checker::ipi_pc(uint32_t thread, uint64_t pc)
{
    if (thread >= EMU_NUM_THREADS) log << LOG_ERR << "IPI pc with thread invalid (" << thread << ")" << endm;
    system_.cpu[thread].pc = pc;
}

void checker::set_icache_prefetch_done(uint32_t shire_id)
{
    system_.finish_icache_prefetch(shire_id);
}

void checker::emu_disasm(char* str, size_t size, uint32_t bits)
{
    riscv_disasm(str, size, bits);
}

// Emulates next instruction in the flow and compares state changes against the changes
// passed as a parameter
checker_result checker::emu_inst(uint64_t cycle, cosim_insn_queue_it_t cosim_inst, std::queue<uint32_t>& wake_minions)
{
    checker_result check_res = CHECKER_OK;
    uint32_t thread = cosim_inst->hart.u32();
    base_state_change* changes = &(cosim_inst->state_change);

    if (thread >= EMU_NUM_THREADS) log << LOG_ERR << "emu_inst with thread invalid (" << thread << ")" << endm;

    if (!threadEnabled[thread])
        log << LOG_ERR << "emu_inst called for thread " << thread << ", which is disabled" << endm;

    setlogstate(&emu_state_change); // This is done every time just in case we have several checkers

    // Initialize emu_state_change
    clearlogstate();

    bool unimpl_esr = false;

    if (system_.cpu[thread].is_blocked()) {
        log << LOG_INFO << "Delaying retire because thread is blocked" << endm;
        return CHECKER_EXCL_ACCESS;
    }

    try
    {
        system_.cpu[thread].check_pending_interrupts(); // This need to be here because it can trap
        // Fetch new instruction (may trap)
        system_.cpu[thread].fetch();

        // Execute the instruction (may trap)
        system_.cpu[thread].execute();

        // check if we have to wake any minions
        for (auto& hart : system_.awaking) {
            uint32_t minion = bemu::hart_index(hart) / EMU_THREADS_PER_MINION;
            wake_minions.push(minion);
        }
        system_.awaking.clear();
    }
    catch (const bemu::memory_error& e)
    {
        // BEMU does not model certain memory regions. We need to inject the data from RTL
        if (address_is_in_ignored_region(e.addr) && !changes->bus_error) {
            log << LOG_INFO << "[H" << thread << "]: Ignore access to 0x" << std::hex << e.addr << " @ PC: 0x"
                << system_.cpu[thread].pc << std::dec << endm;
            emu_state_change = *changes;
            // Ensure mem_p/vaddr are set correctly
            emu_state_change.mem_paddr[0] = e.addr;
            emu_state_change.mem_vaddr[0] = e.addr;
        } else {
            log << LOG_INFO << "[H" << thread << "]: Execution raised Bus Error @ PC: 0x" << std::hex
                << system_.cpu[thread].pc << std::dec << endm;
            // BEMU (and potentially RTL) raised a bus error
            // Set trap_mod to avoid checking the result of the execution
            emu_state_change.pc_mod = false;
            emu_state_change.trap_mod = true;
        }
    }
    catch (const bemu::sysreg_error& e)
    {
        const char* oper = system_.cpu[thread].inst.is_load() ? "Load from" : "Store to";
        log << LOG_INFO << oper << " unimplemented ESR 0x" << std::hex << e.addr << std::dec << endm;
        const auto is_waived_esr = std::find(waived_esrs.begin(), waived_esrs.end(), e.addr) != waived_esrs.end();
        if (!is_waived_esr) {
            log << LOG_ERR << "BEMU didnt implement the ESR @" << std::hex << e.addr << std::dec << endm;
            return CHECKER_ERROR;
        }
        if (system_.cpu[thread].inst.is_load()) {
            log << LOG_INFO << "Injected load data RTL->BEMU with value: 0x" << std::hex << changes->int_reg_data
                << std::dec << endm;
            const auto& cur_insn = system_.cpu[thread].inst;
            const auto op_bits = cur_insn.bits & 0x3;
            // consider moving this to bemu..
            const auto emu_rd = [&]() { // get correct rd based on bits
                switch (op_bits) {
                    case 0x0: return cur_insn.rvc_rs2p();  // c_ld, c_lw
                    case 0x2: return cur_insn.rvc_rs1(); // c_ldsp, c_lwsp
                    case 0x3: return cur_insn.rd(); // 32b instruction
                    default: assert(0 && "Unreachable"); return 0u;
                }
            }();
            set_xreg(system_, thread, emu_rd, changes->int_reg_data);
        }
        unimpl_esr = true;
    }
    catch (const bemu::Trap& t)
    {
        system_.cpu[thread].take_trap(t);
        log << LOG_INFO << "[H" << thread << "]: Took trap @ PC: 0x" << std::hex << system_.cpu[thread].pc << std::dec
            << endm;
        check_res = check_state_changes(cycle, thread, changes, system_.cpu[thread].inst,
                                        cosim_inst); // Check partial results here before the
                                                     // trap. Traping will create mismatches
    }
    catch (const bemu::instruction_restart)
    {
        log << LOG_INFO << "Delaying retire because instruction was restarted" << endm;
        return CHECKER_WAIT;
    }
    catch (const std::domain_error& e)
    {
        log << LOG_INFO << "Delaying retire because of: " << e.what() << endm;
        return CHECKER_WAIT;
    }
    catch (const std::exception& e)
    {
        log << LOG_ERR << e.what() << endm;
    }

    // Check results unless we did trap
    if (emu_state_change.trap_mod) {
        log << LOG_INFO << "[H" << thread << "]: Skipping check_state_changes since PC: 0x " << std::hex
            << system_.cpu[thread].pc << " marked as taken trap" << std::dec << endm;
    } else if (unimpl_esr) {
        log << LOG_INFO << "[H" << thread << "]: Skipping check_state_changes (unimplemented ESR)" << endm;
    } else {
        check_res = check_state_changes(cycle, thread, changes, system_.cpu[thread].inst, cosim_inst);
    }

    // PC update
    system_.cpu[thread].advance_pc();

    return check_res;
}

checker_result checker::check_state_changes(uint64_t cycle, uint32_t thread, base_state_change* changes,
                                            const bemu::Instruction& inst, cosim_insn_queue_it_t cosim_inst)
{
    checker_result check_res = CHECKER_OK;
    std::ostringstream stream;
    error_msg = "";

    // Checks modified fields
    if (changes != NULL) {
#ifdef DEBUG_STATE_CHANGES
        inst_state_change tmp_state_change = emu_state_change;
        // to avoid false diffs
        if (!emu_state_change.pc_mod) tmp_state_change.pc = system_.cpu[thread].pc;
        tmp_state_change.inst_bits = changes->inst_bits;
        log << LOG_DEBUG << "EMU changes: " << tmp_state_change << endm;
        log << LOG_DEBUG << "RTL changes: " << (*changes) << endm;
#endif
        char insn_disasm[128];
        riscv_disasm(insn_disasm, 128, inst.bits);
        stream << "BEMU Checker mismatch for thread " << thread << " @ PC 0x" << std::hex << system_.cpu[thread].pc
               << std::dec << " (" << insn_disasm << ") -> ";

        // The instruction trap
        if (changes->trap_mod != emu_state_change.trap_mod) {
            if (changes->trap_mod)
                stream << "BEMU Checker TRAP mismatch. BEMU expects to NOT TRAP : but DUT TRAP with cause=0x"
                       << std::hex << changes->cause << " tval=0x" << changes->tval << std::dec << std::endl;
            else
                stream << "BEMU Checker TRAP mismatch. DUT expects to NOT TRAP : but BEMU TRAP with cause=0x"
                       << std::hex << emu_state_change.cause << " tval=0x" << emu_state_change.tval << std::dec
                       << std::endl;
            check_res = CHECKER_ERROR;
            // don't change anthing else. More than likelly to find mismatches everywhere
            goto finished_checking;
        } else if (changes->trap_mod) {
            // Check system registers
            if (changes->gsc_progress_mod != emu_state_change.gsc_progress_mod) {
                stream << "BEMU Checker GSC_PROGRESS CSR mismatch. BEMU expects  " << emu_state_change.gsc_progress_mod
                       << " : but DUT " << changes->gsc_progress_mod << std::endl;
                check_res = CHECKER_ERROR;
            }
            if (changes->gsc_progress_mod) {

                if (changes->gsc_progress != emu_state_change.gsc_progress) {
                    stream << "BEMU Checker GSC_PROGRESS CSR mismatch. BEMU expects  " << emu_state_change.gsc_progress
                           << " : but DUT " << changes->gsc_progress << std::endl;
                    check_res = CHECKER_ERROR;
                }
            }

            // Ignore SD bit because it's not available (computed on the fly)
            if ((changes->mstatus & 0x7FFFFFFFFFFFFFFFULL) != (emu_state_change.mstatus & 0x7FFFFFFFFFFFFFFFULL)) {
                stream << "BEMU Checker MSTATUS CSR mismatch. BEMU expects  0x" << std::hex << emu_state_change.mstatus
                       << " : but DUT 0x" << changes->mstatus << std::dec << std::endl;
                check_res = CHECKER_ERROR;
            }
            if (changes->cause != emu_state_change.cause) {
                stream << "BEMU Checker MCAUSE CSR mismatch. BEMU expects  0x" << std::hex << emu_state_change.cause
                       << " : but DUT 0x" << changes->cause << std::dec << std::endl;
                check_res = CHECKER_ERROR;
            }
            if (changes->tval != emu_state_change.tval) {
                stream << "BEMU Checker TVAL CSR mismatch. BEMU expects  0x" << std::hex << emu_state_change.tval
                       << " : but DUT 0x" << changes->tval << std::dec << std::endl;
                check_res = CHECKER_ERROR;
            }
            if (changes->epc != emu_state_change.epc) {
                stream << "BEMU Checker EPC CSR mismatch. BEMU expects  0x" << std::hex << emu_state_change.epc
                       << " : but DUT 0x" << changes->epc << std::dec << std::endl;
                check_res = CHECKER_ERROR;
            }
            goto finished_checking;
        }

        // Check PC
        if (changes->pc != system_.cpu[thread].pc) {
            stream << "BEMU Checker PC mismatch. BEMU expects PC: 0x" << std::hex << system_.cpu[thread].pc
                   << " but DUT reported PC: 0x" << changes->pc << std::dec << std::endl;
            // don't check anything else when PC mismatches... everything would mismatch
            check_res = CHECKER_ERROR;
            goto finished_checking;
        }

        // Instruction bits -- the RTL monitor shows the uncompressed version of the instruction always,
        // while bemu shows the original instruction bits, so we cannot really compare them here
        /*if(changes->inst_bits != inst.bits)
        {
            stream << "BEMU Checker Inst mismatch. BEMU expects inst is 0x" << std::hex << inst.bits << " but DUT
        reported 0x" << changes->inst_bits << std::dec << std::endl;
            // don't check anything else when instruction bits mismatch... everything would mismatch
            check_res = CHECKER_ERROR;
            goto finished_checking;
        }*/

        // Changing integer register
        if (changes->int_reg_mod != emu_state_change.int_reg_mod) {
            stream << "BEMU Checker Int Register write mismatch. BEMU expects register write is "
                   << emu_state_change.int_reg_mod << " but DUT reported " << changes->int_reg_mod << std::endl;
            check_res = CHECKER_ERROR;
        } else if (emu_state_change.int_reg_mod) {
            if (changes->int_reg_rd != emu_state_change.int_reg_rd) {
                stream << "BEMU Checker Int Register dest mismatch. BEMU expects register dest is x"
                       << emu_state_change.int_reg_rd << " but DUT reported x" << changes->int_reg_rd << std::endl;
                check_res = CHECKER_ERROR;
            }

            // Check if load comes from an unpredictible memory region, accept RTL value if so.
            if (inst.is_load() && address_is_in_ignored_region(emu_state_change.mem_paddr[0])) {
                log << LOG_INFO << get_inst_info(cycle, thread) << " Ignore Region Access: Instruction:" << insn_disasm
                    << " Addr: " << emu_state_change.mem_paddr[0]
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Check if we read a CSR that we want to waive checking for
            if (inst.is_csr_read()
                && (std::find(waived_csrs.begin(), waived_csrs.end(), inst.csrimm()) != waived_csrs.end())) {
                log << LOG_INFO << get_inst_info(cycle, thread)
                    << " Waived CSR Read Access: Instruction:" << insn_disasm
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Check if we just read a cycle register, in which case the RTL drives value
            if (inst.is_csr_read() && ((inst.csrimm() == bemu::CSR_CYCLE) || (inst.csrimm() == bemu::CSR_MCYCLE))) {
                log << LOG_INFO << get_inst_info(cycle, thread)
                    << " Waived Cycle CSR Read Access: Instruction:" << insn_disasm
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Check if it_mask is an CMO (Coherent Memory Operation: AMO, etc) (special case where RTL drives value)
            if (inst.is_cmo() && (emu_state_change.int_reg_rd != 0)) {
                log << LOG_INFO << get_inst_info(cycle, thread) << " Waived AMO Cycle: Instruction:" << insn_disasm
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                // Set EMU state to what RTL says
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Check if this is the last Minion in the Fast Local Barrier (special case where RTL drives value)
            if (inst.is_flb() && (emu_state_change.int_reg_rd != 0)) {
                log << LOG_INFO << get_inst_info(cycle, thread) << "Waiving FLB load data check (" << insn_disasm
                    << ") -injecting data RTL-> BEMU with value: " << changes->int_reg_data << endm;
                // Set EMU state to what RTL says
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Check for TBOX accesses here
            if (inst.is_load() && (emu_state_change.mem_paddr[0] >= TBOX_REGION_START)
                && (emu_state_change.mem_paddr[0] < TBOX_REGION_END)) {
                log << LOG_INFO << get_inst_info(cycle, thread) << " Waived TBOX: Instruction:" << insn_disasm
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                // Set EMU state to what RTL says
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Read esr_icache_trigger status
            if (inst.is_load() && (emu_state_change.mem_paddr[0] >= ESR_SHIRE_REGION_START)
                && (emu_state_change.mem_paddr[0] < ESR_SHIRE_REGION_END)) {
                log << LOG_INFO << get_inst_info(cycle, thread)
                    << " Waived Access to SHIRE ESR: Instruction:" << insn_disasm
                    << " injecting data RTL-> BEMU with value:" << changes->int_reg_data << endm;
                // Set EMU state to what RTL says
                emu_state_change.int_reg_data = changes->int_reg_data;
                set_xreg(system_, thread, emu_state_change.int_reg_rd, emu_state_change.int_reg_data);
            }

            // Writes to X0/Zero are ignored
            if ((changes->int_reg_data != emu_state_change.int_reg_data) && (emu_state_change.int_reg_rd != 0)
                && (changes->bus_error == false)) {
                stream << "EMU Checker Int Register data mismatch. BEMU expects data is 0x" << std::hex
                       << emu_state_change.int_reg_data << " but DUT reported 0x" << changes->int_reg_data << std::dec
                       << std::endl;
                check_res = CHECKER_ERROR;
                // Set EMU state to what RTL says
                set_xreg(system_, thread, emu_state_change.int_reg_rd, changes->int_reg_data);
            }
        }

        // Changing fflags
        if (changes->fflags_mod != emu_state_change.fflags_mod) {
            // Someone changed the flags
            std::string changer = emu_state_change.fflags_mod ? "EMU" : "RTL";
            stream << "BEMU Checker fflags changed by " << changer << ". BEMU expects new flags 0x" << std::hex
                   << emu_state_change.fflags_value << " but DUT reported 0x" << changes->fflags_value << std::dec
                   << std::endl;
            check_res = CHECKER_ERROR;
        } else if (emu_state_change.fflags_mod) {
            if (changes->fflags_value != emu_state_change.fflags_value) {
                stream << "BEMU Checker fflags values change. BEMU expects new flags 0x" << std::hex
                       << emu_state_change.fflags_value << " but DUT reported 0x" << changes->fflags_value << std::dec
                       << std::endl;
                check_res = CHECKER_ERROR;
            }
        }

        // Changing floating register
        if (changes->fp_reg_mod != emu_state_change.fp_reg_mod) {
            stream << "BEMU Checker FP Register write mismatch. BEMU expects write is " << emu_state_change.fp_reg_mod
                   << " but DUT reported " << changes->fp_reg_mod << std::endl;
            check_res = CHECKER_ERROR;
        } else if (emu_state_change.fp_reg_mod) {
            if (changes->fp_reg_rd != emu_state_change.fp_reg_rd) {
                stream << "BEMU Checker FP Register dest mismatch. BEMU expects dest is f" << emu_state_change.fp_reg_rd
                       << " but DUT reported f" << changes->fp_reg_rd << std::endl;
                check_res = CHECKER_ERROR;
            }

            // Check if load comes from an unpredictible memory region, accept RTL value if so.
            if (inst.is_load() && address_is_in_ignored_region(emu_state_change.mem_paddr[0])) {
                log << LOG_INFO << get_inst_info(cycle, thread) << "Waiving ignored memory region data check ("
                    << insn_disasm << ") -injecting data RTL-> BEMU with value " << endm;
                for (size_t i = 0; i < bemu::VLEND; i++) {
                    emu_state_change.fp_reg_data[i] = changes->fp_reg_data[i];
                    log << LOG_INFO << "FP Reg [" << i << "]" << changes->int_reg_data << endm;
                }
                set_freg(system_, thread, emu_state_change.fp_reg_rd, emu_state_change.fp_reg_data.data());
            }

            // Check if load comes from an (Coherent Memory Operation: AMO, etc) (special case where RTL drives value)
            if (inst.is_cmo()) {
                log << LOG_INFO << get_inst_info(cycle, thread) << "Waiving FAMO load data check (" << insn_disasm
                    << ") -injecting data RTL-> BEMU with value " << endm;
                // Set EMU state to what RTL says
                for (size_t i = 0; i < bemu::VLEND; i++) {
                    emu_state_change.fp_reg_data[i] = changes->fp_reg_data[i];
                    log << LOG_INFO << "FP Reg [" << i << "]" << changes->int_reg_data << endm;
                }
                set_freg(system_, thread, emu_state_change.fp_reg_rd, emu_state_change.fp_reg_data.data());
            }

            bool fp_reg_data_mismatch = false;
            for (size_t i = 0; i < bemu::VLEND; i++) {
                if (inst.is_1ulp()) {
                    bool errlo = !fp_1ulp_check(emu_state_change.fp_reg_data[i] & 0xFFFFFFFF,
                                                changes->fp_reg_data[i] & 0xFFFFFFFF);
                    bool errhi = !fp_1ulp_check(emu_state_change.fp_reg_data[i] >> 32, changes->fp_reg_data[i] >> 32);
                    if (errlo || errhi) {
                        uint32_t emu_datalo = emu_state_change.fp_reg_data[i] & 0xFFFFFFFF;
                        uint32_t rtl_datalo = changes->fp_reg_data[i] & 0xFFFFFFFF;
                        uint32_t emu_datahi = emu_state_change.fp_reg_data[i] >> 32;
                        uint32_t rtl_datahi = changes->fp_reg_data[i] >> 32;
                        stream << "BEMU Checker FP Register data_error f" << emu_state_change.fp_reg_rd;
                        if (errlo && errhi)
                            stream << "[" << (2 * i + 1) << ", " << (2 * i) << "]";
                        else
                            stream << "[" << (errlo ? (2 * i) : (2 * i + 1)) << "]";
                        stream << ". BEMU expects data is ";
                        if (errlo && errhi)
                            stream << "{0x" << std::hex << emu_datahi << ", 0x" << emu_datalo << "}";
                        else
                            stream << "0x" << std::hex << (errlo ? emu_datalo : emu_datahi);
                        stream << " but DUT reported ";
                        if (errlo && errhi)
                            stream << "{0x" << std::hex << rtl_datahi << ", 0x" << rtl_datalo << "}";
                        else
                            stream << "0x" << std::hex << (errlo ? rtl_datalo : rtl_datahi);
                        stream << " Current mask: 0x" << get_mask(system_, thread) << std::dec << std::endl;
                        check_res = CHECKER_ERROR;
                    }
                } else {
                    if (changes->fp_reg_data[i] != emu_state_change.fp_reg_data[i]) {
                        bool errlo = (emu_state_change.fp_reg_data[i] & 0xFFFFFFFF)
                                     != (changes->fp_reg_data[i] & 0xFFFFFFFF);
                        bool errhi = (emu_state_change.fp_reg_data[i] >> 32) != (changes->fp_reg_data[i] >> 32);
                        if (errlo || errhi) {
                            uint32_t emu_datalo = emu_state_change.fp_reg_data[i] & 0xFFFFFFFF;
                            uint32_t rtl_datalo = changes->fp_reg_data[i] & 0xFFFFFFFF;
                            uint32_t emu_datahi = emu_state_change.fp_reg_data[i] >> 32;
                            uint32_t rtl_datahi = changes->fp_reg_data[i] >> 32;
                            stream << "BEMU Checker FP Register data_error f" << emu_state_change.fp_reg_rd;
                            if (errlo && errhi)
                                stream << "[" << (2 * i + 1) << ", " << (2 * i) << "]";
                            else
                                stream << "[" << (errlo ? (2 * i) : (2 * i + 1)) << "]";
                            stream << ". BEMU expects data is ";
                            if (errlo && errhi)
                                stream << "{0x" << std::hex << emu_datahi << ", 0x" << emu_datalo << "}";
                            else
                                stream << "0x" << std::hex << (errlo ? emu_datalo : emu_datahi);
                            stream << " but DUT reported ";
                            if (errlo && errhi)
                                stream << "(0x" << std::hex << rtl_datahi << ", 0x" << rtl_datalo << ")";
                            else
                                stream << "0x" << std::hex << (errlo ? rtl_datalo : rtl_datahi);
                            stream << " Current mask: 0x" << get_mask(system_, thread) << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                        }
                    }
                }
                fp_reg_data_mismatch = fp_reg_data_mismatch
                                       || (changes->fp_reg_data[i] != emu_state_change.fp_reg_data[i]);
            }

            // Set EMU state to what RTL says
            if (fp_reg_data_mismatch) {
                set_freg(system_, thread, changes->fp_reg_rd, changes->fp_reg_data.data());
            }
        }

        // Changing mask register
        for (int m = 0; m < 8; m++) {
            if (changes->m_reg_mod[m] != emu_state_change.m_reg_mod[m]) {
                stream << "BEMU Checker Mask Register write mismatch. BEMU expects write is "
                       << emu_state_change.m_reg_mod[m] << " but DUT reported " << changes->m_reg_mod[m] << std::endl;
                check_res = CHECKER_ERROR;
            } else if (emu_state_change.m_reg_mod[m]) {
                for (size_t i = 0; i < bemu::VLENW; i++) {
                    if (changes->m_reg_data[m][i] != emu_state_change.m_reg_data[m][i]) {
                        stream << "BEMU Checker Mask Register data mismatch m" << m << "[" << i
                               << "]. BEMU expects data is " << std::hex << (uint32_t)emu_state_change.m_reg_data[m][i]
                               << " but DUT reported " << (uint32_t)changes->m_reg_data[m][i] << std::dec << std::endl;
                        check_res = CHECKER_ERROR;
                    }
                }
            }
        }

        // Memory changes
        for (size_t i = 0; i < bemu::VLENW; i++) {
            if (changes->mem_mod[i] != emu_state_change.mem_mod[i]) {
                stream << "BEMU Checker Memory write mismatch (" << i << "). BEMU expects write is "
                       << emu_state_change.mem_mod[i] << " but DUT reported " << changes->mem_mod[i] << std::endl;
                check_res = CHECKER_ERROR;
            } else if (emu_state_change.mem_mod[i]) {
                if (changes->mem_size[i] != emu_state_change.mem_size[i]) {
                    stream << "BEMU Checker Memory write size mismatch (" << i << "). BEMU expects size is "
                           << emu_state_change.mem_size[i] << " but DUT reported " << changes->mem_size[i] << std::endl;
                    check_res = CHECKER_ERROR;
                }
                uint64_t rtl_mem_addr = changes->mem_vaddr[i] & VA_M;
                uint64_t emu_mem_addr = emu_state_change.mem_vaddr[i] & VA_M;
                if (rtl_mem_addr != emu_mem_addr) {
                    stream << "BEMU Checker Memory write address mismatch (" << i << "). BEMU expects addr is 0x"
                           << std::hex << emu_mem_addr << " but DUT reported 0x" << rtl_mem_addr << std::dec
                           << std::endl;
                    check_res = CHECKER_ERROR;
                }
                uint64_t rtl_mem_data = changes->mem_data[i] & mem_mask(changes->mem_size[i]);
                uint64_t emu_mem_data = emu_state_change.mem_data[i] & mem_mask(changes->mem_size[i]);
                if (rtl_mem_data != emu_mem_data) {
                    stream << "BEMU Checker Memory write data_error (" << i << "). BEMU expects data is 0x" << std::hex
                           << emu_mem_data << " but DUT reported 0x" << rtl_mem_data << std::dec << std::endl;
                    check_res = CHECKER_ERROR;
                }
            }
        }

        // TensorLoad
        if (inst.is_tensor_load_write()) {

            log << LOG_DEBUG << "dbg-tensorload checker::check_state_changes() - thread=" << std::dec << thread
                << " - emu_state_change = ( "
                << " tensor_mod=" << (uint)emu_state_change.tensor_mod
                << " tl_transform=" << (uint)emu_state_change.tl_transform
                << " tl_tenb=" << (uint)emu_state_change.tl_tenb
                << " tl_scp_first=" << (uint)emu_state_change.tl_scp_first
                << " tl_scp_count=" << (uint)emu_state_change.tl_scp_count << std::hex << " tl_conv_skip=0x"
                << (uint)emu_state_change.tl_conv_skip << " tensor_error=0x" << (uint)emu_state_change.tensor_error
                << " )" << endm;

            unsigned num_entries_checked = 0;

            if (cosim_inst->tensor_load_tenb_flush || emu_state_change.tl_scp_count == 0) {
                log << LOG_DEBUG << std::dec << "dbg-tensorload checker::check_state_changes() - thread=" << thread
                    << " checking tensorload tl" << cosim_inst->tenb << " tl-op-id=" << cosim_inst->tensor_load_op_id
                    << " - skip checking" << endm;
                goto finished_checking;
            } else {
                log << LOG_DEBUG << std::dec << "dbg-tensorload checker::check_state_changes() - thread=" << thread
                    << " - checking tensorload tl" << cosim_inst->tenb << " tl-op-id=" << cosim_inst->tensor_load_op_id
                    << endm;
            }

            if (emu_state_change.tl_transform && !(emu_state_change.tl_conv_skip == 0xFFFF))
                aggregate_tl_data(cosim_inst);

            // For all the written entries
            int start = emu_state_change.tl_scp_first;
            int adj = 0;
            if (start == 48) {
                adj = 48;
                start = 0;
            }

            for (int i = 0; i < emu_state_change.tl_scp_count && emu_state_change.tensor_mod; i++) {
                // Load was skipped due conv CSR, ignore check
                if (emu_state_change.tl_conv_skip & (1 << i)) {
                    log << LOG_DEBUG << "dbg-tensorload checker::check_state_changes() - thread=" << thread
                        << " - conv CSR -> skipping emu entry i=" << i << endm;
                    continue;
                } else {
                    log << LOG_DEBUG << "dbg-tensorload checker::check_state_changes() - thread=" << thread
                        << " - checking emu entry i=" << i << endm;
                }

                // Looks for the 1st entry in the list of RTL written lines with same destination
                int scp_index = adj + ((start + i) % 48);

                // TL with transformations doesn't require mapping to cache space
                int cache_index = (emu_state_change.tl_transform) ? scp_index
                                                                  : bemu::scp_index_to_cache_index(scp_index);

                if (cosim_inst->tensor_load_entry_list == NULL) {
                    stream << "TensorLoad DUT reported scratchpad list is empty" << std::endl;
                    check_res = CHECKER_ERROR;
                    goto finished_checking;
                }

                auto it_mask = std::find_if(cosim_inst->tensor_load_entry_list->begin(),
                                            cosim_inst->tensor_load_entry_list->end(),
                                            [=](const cosim_tensor_load_entry& x) { return x.entry == cache_index; });

                // Checks that an entry was actually found
                if (it_mask == cosim_inst->tensor_load_entry_list->end()) {
                    stream << "BEMU Checker couldn't find scratchpad destination " << std::dec << scp_index
                           << " in the DUT reported scratchpad list!!" << std::endl;
                    stream << "SCP Candidates: " << std::endl;
                    for (auto e : *cosim_inst->tensor_load_entry_list) {
                        stream << "\tEntry : " << e.entry << std::endl;
                    }
                    check_res = CHECKER_ERROR;
                } else {
                    // Compares the data
                    for (int j = 0; j < (L1D_LINE_SIZE / 4); j += 2) {
                        uint32_t lo
                            = emu_state_change.tensordata[0][2 * i + (j + 0) / bemu::VLENW][(j + 0) % bemu::VLENW];
                        uint32_t hi
                            = emu_state_change.tensordata[0][2 * i + (j + 1) / bemu::VLENW][(j + 1) % bemu::VLENW];
                        uint64_t data = uint64_t(lo) + (uint64_t(hi) << 32);
                        if (data != it_mask->data[j / 2]) {
                            stream << "BEMU Checker TensorLoad write data_error for cacheline " << i
                                   << " written in entry " << start + i << " data lane " << j
                                   << ". BEMU expects data is 0x" << std::hex << data << " but DUT reported 0x"
                                   << it_mask->data[j] << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                        }
                    }
                    cosim_inst->tensor_load_entry_list->erase(it_mask);
                    num_entries_checked++;
                }
            }
        }

        // TensorFMA
        if (inst.is_tensor_fma_write()) {

            // For all the passes
            for (int pass = 0; pass < emu_state_change.tensorfma_passes; pass++) {
                // For all the written entries
                for (int reg = 0; reg < int(bemu::NFREGS); reg++) {

                    if (~emu_state_change.tensorfma_mod[pass] & (1u << reg)) {
                        // log << LOG_DEBUG << "TensorFMA skipping check for this pass[" << pass << "]-entry[" <<  reg
                        // << "]" << " since BEMU didnt update this entry" << endm;
                        continue;
                    }

                    // Looks for the 1st entry in the list of RTL written lines with same destination
                    auto it_mask = std::find_if(tensorfma_list[thread].begin(), tensorfma_list[thread].end(),
                                                [=](const tensorfma_entry& x) { return x.entry == reg; });
                    // Checks that an entry was actually found
                    if (it_mask == tensorfma_list[thread].end()) {
                        stream << "BEMU Checker couldn't find TensorFMA destination " << reg
                               << " in the DUT reported TensorFMA list for pass " << pass << "!!" << std::endl;
                        check_res = CHECKER_ERROR;
                        continue;
                    }

                    // Compares the data for all the lanes (8 x 32b lanes)
                    for (size_t lane = 0; lane < bemu::VLENW; lane++) {
                        uint32_t exp_write_mask = (uint32_t)emu_state_change.tensorfma_mask[pass][reg][lane];
                        if ((((it_mask->wmask) >> lane) & 0x1) != exp_write_mask) {
                            stream << "BEMU Checker TensorFMA write mask mismatch for pass[" << pass << "]-entry["
                                   << it_mask->entry << "]-lane[" << lane << "]. BEMU expects mask " << exp_write_mask
                                   << " but DUT reported 0x" << std::hex << (((it_mask->wmask) >> lane) & 0x1)
                                   << " ( Mask=0x" << it_mask->wmask << " )" << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                            continue;
                        }

                        uint32_t data = emu_state_change.tensordata[pass][reg][lane];
                        if (data != it_mask->data[lane] && exp_write_mask) {
                            stream << "BEMU Checker TensorFMA write data_error for register f" << reg << "[" << lane
                                   << "] pass " << pass << ". BEMU expects data is 0x" << std::hex << data
                                   << " but DUT reported 0x" << it_mask->data[lane] << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                        }
                    }
                    // log << LOG_DEBUG << "TensorFMA Check completed. Erasing " << reg << "from tensorfma list" <<
                    // endm;
                    tensorfma_list[thread].erase(it_mask);
                }
            }
        }

        // TensorQuant
        if (inst.is_tensor_quant_write()) {
            // For all the transforms
            for (int trans = 0; trans < emu_state_change.tensorquant_trans; trans++) {
                // For all the written entries
                for (int reg = 0; reg < 32; reg++) {
                    if (~emu_state_change.tensorquant_mod[trans] & (1u << reg)) continue;

                    auto it_mask = std::find_if(tensorquant_list[thread].begin(), tensorquant_list[thread].end(),
                                                [=](const tensorfma_entry& x) { return x.entry == reg; });
                    if (emu_state_change.tensorquant_skip_write[trans] && it_mask != tensorquant_list[thread].end()) {
                        // Pack 128b writes the destination once or twice depending on number of columns
                        tensorquant_list[thread].erase(it_mask);
                        it_mask = std::find_if(tensorquant_list[thread].begin(), tensorquant_list[thread].end(),
                                               [=](const tensorfma_entry& x) { return x.entry == reg; });
                    }
                    // Checks that an entry was actually found
                    if (it_mask == tensorquant_list[thread].end()) {
                        stream << "BEMU Checker couldn't find TensorQuant destination " << reg
                               << " in the DUT reported TensorQuant list for trans " << trans << "!!" << std::endl;
                        check_res = CHECKER_ERROR;
                        goto finished_checking;
                    }
                    // Compares the data for all the lanes (VL x 32b lanes)
                    for (size_t lane = 0; lane < bemu::VLENW; lane++) {
                        if (!emu_state_change.tensorquant_mask[trans][reg][lane]) continue;
                        uint32_t data = emu_state_change.tensordata[trans][reg][lane];
                        if (data != it_mask->data[lane]) {
                            stream << "BEMU Checker TensorQuant write data_error for register f" << reg << "[" << lane
                                   << "] trans " << trans << ". BEMU expects data is 0x" << std::hex << data
                                   << " but DUT reported 0x" << it_mask->data[lane] << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                        }
                    }
                    tensorquant_list[thread].erase(it_mask);
                }
            }
        }

        // Reduce
        if (inst.is_reduce()) {
            uint64_t read_list_idx = reduce_list_read_idx[thread];
            if (reduce_is_receive) {
                advanceReduceListReadIdx(thread);
            }
            int start_entry = emu_state_change.tensorreduce_first;
            int size;

            // Handling multiple wrap-arounds
            int reduce_list_size = reduce_list[thread][read_list_idx].size();

            if (emu_state_change.tensorreduce_count > 32) {
                size = 32;
            } else {
                size = emu_state_change.tensorreduce_count;
            }

            while (reduce_list_size > 32) {
                reduce_list[thread][read_list_idx].pop_front();
                reduce_list_size--;
            }

            // For all the written entries
            for (int entry = start_entry; entry < (start_entry + size); entry++) {
                int entry_wrap = entry % 32;
                // Looks for the 1st entry in the list of RTL written lines with same destination
                auto it_mask = reduce_list[thread][read_list_idx].begin();

                while (it_mask != reduce_list[thread][read_list_idx].end()) {
                    if (it_mask->entry == entry_wrap) {
                        break;
                    }
                    it_mask++;
                }

                // Checks that an entry was actually found
                if (it_mask == reduce_list[thread][read_list_idx].end()) {
                    stream << "BEMU Checker couldn't find Reduce destination " << entry_wrap
                           << " in the DUT provided Reduce list!!" << std::endl;
                    check_res = CHECKER_ERROR;
                } else {
                    // Compares the data for all the lanes (8 x 32b lanes)
                    for (int lane = 0; lane < 8; lane++) {
                        uint32_t data = emu_state_change.tensordata[0][entry_wrap][lane];
                        if (data != it_mask->data[lane]) {
                            stream << "BEMU Checker Reduce write data_error for register " << entry_wrap << " lane "
                                   << lane << ". BEMU expects data is 0x" << std::hex << data << " but DUT reported 0x"
                                   << it_mask->data[lane] << std::dec << std::endl;
                            check_res = CHECKER_ERROR;
                        }
                    }
                    reduce_list[thread][read_list_idx].erase(it_mask);
                }
            }
        }

        // Tensor store
        if (inst.is_tensor_store_write()) {

            check_res = CHECKER_OK;
            uint32_t total_stores_emu = emu_state_change.tensorstore_idx;
            uint32_t total_stores_rtl = tensorstore_list[thread].size() > 0 ? tensorstore_list[thread].front().size()
                                                                            : 0;

            // Tensor store with tensor error, and possibly with start and done.
            if (total_stores_emu == 0 && emu_state_change.tensorstore_error != 0) {
                log << LOG_DEBUG << "Tensor Store does not make any stores due to TensorError " << std::hex << "0x"
                    << emu_state_change.tensorstore_error << std::dec << ", skipping Checker" << endm;
                check_res = CHECKER_OK;

                // For mem access errors, there is start and done, so advance pointer.
                if (((emu_state_change.tensorstore_error & 0x80) == 0x80) && total_stores_rtl == 0) {
                    tensorstore_list[thread].pop_front();
                }
                goto finished_checking;
            }

            // In all other cases with no stores we do not have start and done, so do not advance the pointer
            else if (total_stores_emu == 0 && total_stores_rtl == 0) {
                log << LOG_DEBUG << "Tensor Store treated as a nop " << endm;
                goto finished_checking;
            } else if (total_stores_emu != total_stores_rtl) {
                stream << "BEMU Checker Tensor Store error: BEMU expects " << total_stores_emu
                       << " 32-bit stores, RTL does " << total_stores_rtl << std::endl;
                check_res = CHECKER_ERROR;
                goto finished_checking;
            }

            // Individual lines of the same tesnor store can go out of order at the neighborhood boundary.
            // Thus this nested loop. However since it will be mostly in order it will behave like a single loop.
            else {
                std::list<tensorstore_entry>::iterator ts_it;
                std::list<tensorstore_entry>& cur_list = tensorstore_list[thread].front();
                for (uint32_t emu_idx = 0; emu_idx < total_stores_emu; emu_idx++) {
                    ts_it = cur_list.begin();
                    while (emu_state_change.tensorstore_write_addr[emu_idx] != ts_it->addr
                           && (ts_it != cur_list.end())) {
                        ts_it++;
                    }
                    if (ts_it == cur_list.end()) {
                        stream << "BEMU Checker Tensor Store error: BEMU writes to address " << std::hex
                               << emu_state_change.tensorstore_write_addr[emu_idx] << " but DUT does not! " << std::dec
                               << std::endl;
                        check_res = CHECKER_ERROR;
                    } else if (emu_state_change.tensorstore_write_data[emu_idx] != ts_it->data) {
                        stream << "BEMU Checker Tensor Store error: Address: " << std::hex
                               << emu_state_change.tensorstore_write_addr[emu_idx] << " -- BEMU writes data "
                               << emu_state_change.tensorstore_write_data[emu_idx] << " but DUT writes data "
                               << ts_it->data << std::dec << std::endl;
                        cur_list.erase(ts_it);
                        check_res = CHECKER_ERROR;
                    } else {
                        log << LOG_DEBUG << "Tensor Store Matched: " << std::hex
                            << emu_state_change.tensorstore_write_addr[emu_idx] << " "
                            << emu_state_change.tensorstore_write_data[emu_idx] << std::dec << endm;
                        cur_list.erase(ts_it);
                    }
                }
                if (!cur_list.empty()) {
                    check_res = CHECKER_ERROR;
                    for (ts_it = cur_list.begin(); ts_it != cur_list.end();) {
                        stream << "BEMU Checker Tensor Store error: DUT writes to address " << std::hex << ts_it->addr
                               << " but BEMU does not! " << std::dec << std::endl;
                        ts_it = cur_list.erase(ts_it);
                    }
                }
                tensorstore_list[thread].pop_front();
            }
        }
    }

finished_checking:

    if (check_res != CHECKER_OK) {
        error_msg += stream.str();
        if (!checker_enabled) {
            check_res = CHECKER_WARNING;
        }
    }
    return check_res;
}

void checker::esr_write(uint64_t addr, uint64_t data)
{
    log << LOG_DEBUG << "ESR Write to Addr: " << std::hex << addr << " Data: " << data << std::dec << endm;
    system_.esr_write(system_.noagent, addr, data);
}

void checker::raise_interrupt(unsigned minionId, int cause, uint64_t mip, uint64_t bus_err_addr)
{
    log << LOG_DEBUG << "Raising interrupt 0x" << std::hex << cause << std::dec << endm;
    (void)mip; // mip not used in sysemu - it computes mip internally as (1 << cause)
    system_.cpu[minionId].raise_interrupt(cause, bus_err_addr);
}

// Return the last error message
std::string checker::get_error_msg()
{
    return error_msg;
}

// enables or disables the 2nd thread
void checker::thread1_enabled(unsigned minionId, uint64_t en, uint64_t pc)
{
    unsigned thread = minionId | 1;
    if (en != threadEnabled[thread]) {
        threadEnabled[thread] = en;
        if (en) {
            system_.cpu[thread].pc = pc;
        }
    }
}

/*
 * This function merge half lines into full lines
 */
void checker::aggregate_tl_data(cosim_insn_queue_it_t cosim_inst)
{
    const int OFFSET_HLINE_BIT = 0; // offset in entry that sets which half line is
    const int HLINE_DWORDS = (L1D_LINE_SIZE / 2) / sizeof(uint64_t);

    std::list<cosim_tensor_load_entry> aggregated_scp_entries;

    log << LOG_DEBUG << "Aggregating partial writes to dcache. size : " << cosim_inst->tensor_load_entry_list->size()
        << endm;
    log << LOG_DEBUG << "Dumping SCP entries Before aggregation " << endm;

    for (auto e : *cosim_inst->tensor_load_entry_list) {
        log << LOG_DEBUG << "Entry: " << std::dec << e.entry << std::hex << " Banks: " << e.banks
            << " data: " << e.data[0] << " " << e.data[1] << " " << e.data[2] << " " << e.data[3] << " " << e.data[4]
            << " " << e.data[5] << " " << e.data[6] << " " << e.data[7] << std::dec << endm;
    }

    // empty the queue, merge all that can be merged
    while (!cosim_inst->tensor_load_entry_list->empty()) {
        auto it_mask = cosim_inst->tensor_load_entry_list->begin();
        // Get the entry and copy it_mask. Move data to next half-line if it_mask's needed
        cosim_tensor_load_entry entry = *it_mask;
        it_mask = cosim_inst->tensor_load_entry_list->erase(it_mask);
        int addr_fhl = entry.entry & ~(1 << OFFSET_HLINE_BIT); // build first half line addres for the current entry
        int addr_shl = entry.entry | (1 << OFFSET_HLINE_BIT);  // build second half line addres for the current entry
        if (entry.entry == addr_shl) {
            // This entry address writes to a second half-line.
            for (int i = 0; i < HLINE_DWORDS; ++i) {
                entry.data[HLINE_DWORDS + i] = entry.data[i];
            }
        }

        // Search for all mergeable entries
        log << LOG_DEBUG << "Merging Entry: " << std::dec << entry.entry << std::hex << " Banks: " << entry.banks
            << " addr_fhl: " << addr_fhl << " addr_shl: " << addr_shl << std::dec << endm;
        auto it2 = cosim_inst->tensor_load_entry_list->begin();
        while (it2 != cosim_inst->tensor_load_entry_list->end()) {
            auto entry2 = *it2;
            if (entry2.entry == addr_fhl || entry2.entry == addr_shl) {
                log << LOG_DEBUG << "Merging and deleting entry " << std::dec << entry2.entry << std::hex
                    << " with banks " << entry2.banks << " with entry " << std::dec << entry.entry << std::hex
                    << " with banks " << entry.banks << endm;

                int offset = entry2.entry == addr_fhl ? 0 : HLINE_DWORDS;
                for (int i = 0; i < HLINE_DWORDS; ++i) {
                    uint32_t valid = entry2.banks & (uint32_t(1) << i);
                    if (valid) {
                        entry.data[offset + i] = entry2.data[i];
                        entry.banks |= (valid << offset);
                    }
                    if (entry.banks > 0xFF)
                        log << LOG_ERR << "Error merging: Entry " << std::dec << entry.entry
                            << " is bigger than line size " << endm;
                }
                it2 = cosim_inst->tensor_load_entry_list->erase(it2);
            } else {
                ++it2;
            }
        }
        if (entry.banks != 0xFF)
            log << LOG_ERR << "Error merging: Entry " << std::dec << entry.entry << " with banks " << std::hex
                << entry.banks << std::dec
                << " but expecting 0xFF. Looks like the RTL forget to update some parts of the line" << endm;

        entry.entry >>= OFFSET_HLINE_BIT + 1; // remove entry offset and line bit
        aggregated_scp_entries.push_back(entry);
    }

    // Replace old list with the new merged list
    std::swap(*cosim_inst->tensor_load_entry_list, aggregated_scp_entries);

    log << LOG_DEBUG << "Dumping SCP entries After aggregation " << endm;

    for (auto e : *cosim_inst->tensor_load_entry_list) {
        log << LOG_DEBUG << "Entry: " << std::dec << e.entry << std::hex << " Banks: " << e.banks
            << " data: " << e.data[0] << " " << e.data[1] << " " << e.data[2] << " " << e.data[3] << " " << e.data[4]
            << " " << e.data[5] << " " << e.data[6] << " " << e.data[7] << std::dec << endm;
    }
}

// TensorFMA write
void checker::tensorfma_write(uint32_t thread, int entry, uint32_t wmask, const uint32_t* data)
{
    tensorfma_entry tensorfma;

    tensorfma.entry = entry;
    tensorfma.wmask = wmask;
    // log << LOG_DEBUG << "Entry: " << entry << " Copied WMask: " << std::hex << tensorfma.wmask << "
    // Original WMask: "  << wmask << endm;
    for (size_t i = 0; i < bemu::VLENW; i++) {
        tensorfma.data[i] = data[i];
        // log << LOG_DEBUG << "Index: " << i << ": Data: " << data[i] << endm;
    }

    tensorfma_list[thread].push_back(tensorfma);
}

// TensorQuant write
void checker::tensorquant_write(uint32_t thread, uint32_t entry, uint32_t wmask, const uint32_t* data)
{
    tensorfma_entry tensorquant;

    tensorquant.entry = entry;
    for (size_t i = 0; i < bemu::VLENW; i++) {
        tensorquant.data[i] = data[i];
        // log << LOG_DEBUG << "Index: " << i << ": Data: " << data[i] << endm;
    }
    tensorquant.wmask = wmask;

    tensorquant_list[thread].push_back(tensorquant);
}

// Reduce write
void checker::reduce_write(uint32_t thread, uint32_t entry, const uint32_t* data)
{
    tensorfma_entry reduce;

    reduce.entry = entry;
    for (unsigned i = 0; i < bemu::VLENW; i++) {
        reduce.data[i] = data[i];
        // log << LOG_DEBUG << "Index: " << i << ": Data: " << data[i] << endm;
    }

    reduce_list[thread][reduce_list_write_idx[thread]].push_back(reduce);
}

// Tensor store
void checker::tensorstore_write(uint32_t thread, uint64_t addr, uint32_t data)
{
    tensorstore_entry entry;
    entry.addr = addr;
    entry.data = data;
    std::list<tensorstore_entry>& cur_list = tensorstore_list[thread].back();
    cur_list.push_back(entry);
}

bool checker::address_is_in_ignored_region(uint64_t addr)
{
    auto it_region = ignored_mem_regions.begin();
    while (it_region != ignored_mem_regions.end()) {
        if ((addr >= it_region->base) && (addr <= it_region->top)) return true;
        it_region++;
    }

    auto it_mask = ignored_mem_masks.begin();
    while (it_mask != ignored_mem_masks.end()) {
        if (((addr & it_mask->mask) ^ it_mask->match) == 0) return true;
        it_mask++;
    }
    return false;
}

void checker::add_ignored_mem_region(uint64_t base, uint64_t top)
{
    ignored_mem_region region;
    region.base = base;
    region.top = top;
    ignored_mem_regions.push_back(region);
}

void checker::add_ignored_mem_mask(uint64_t match, uint64_t mask)
{
    ignored_mem_mask ignored_mask;
    ignored_mask.match = match;
    ignored_mask.mask = mask;
    ignored_mem_masks.push_back(ignored_mask);
}

void checker::thread_port_write(uint32_t target_thread, uint32_t port_id, uint32_t source_thread)
{
    system_.commit_msg_port_data(target_thread, port_id, source_thread);
}

void checker::tbox_port_write(uint32_t target_thread, uint32_t port_id, uint32_t tbox_id)
{
    (void)target_thread; (void)port_id; (void)tbox_id;
    throw checker_error("Unexpected TBOX event: tbox_port_write called but TBOX is not supported");
}

void checker::rbox_port_write(uint32_t target_thread, uint32_t port_id, uint32_t rbox_id)
{
    (void)target_thread; (void)port_id; (void)rbox_id;
    throw checker_error("Unexpected RBOX event: rbox_port_write called but RBOX is not supported");
}

std::string checker::get_inst_info(uint64_t cycle, uint32_t thread)
{
    std::ostringstream s0;
    s0 << " - cycle=" << cycle << " thread=" << thread << " @ PC 0x" << std::hex << system_.cpu[thread].pc;
    return s0.str();
}
