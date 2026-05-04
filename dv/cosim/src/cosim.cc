/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <algorithm>
#include <cassert>
#include <csignal>
#include <iomanip>
#include <sstream>
#include <string>

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/fmt/ostr.h"

#include "cosim.h"
#include "events/recorder.h"
#include "events/dispatcher.h"
#include "support/insn_desc.h"
#include "support/hart_id.h"
#include "support/file_sinks.h"
#include "support/file_utils.h"
#include "checker/riscv_disasm.h"
#include "cosim_error.h"

#include "system.h"
#include "emu_gio.h"
#include "esrs.h"
#include "processor.h"
#include "traps.h"


#define LOG_PREFIX "cyc:{}\t{}\t{}-{}\t"
#define LOG_PREFIX_SHORT "cyc:{}\t{}\t"

using namespace cosim_evt;

char get_prvname(uint32_t prv)
{
    constexpr char mode_names[] = "USHM";
    return prv > 3 ? '?' : mode_names[prv];
}

volatile bool cosim::saw_shutdown = false;

void cosim::shutdown(int signal)
{
    (void)signal;
    // Delete cosim and dispatcher on SIGTERM
    // Note: We shouldn't really be doing these in a signal handler..
    cosim::deleteInstance();
    dispatcher::deleteInstance();
    spdlog::shutdown(); // Flush log streams

    saw_shutdown = true;
}

cosim* cosim::pInstance = NULL;

uint64_t mem_mask(int32_t); // Implemented in checker.cc

cosim& cosim::getInstance()
{
    if (pInstance == NULL) pInstance = new cosim();
    return *pInstance;
}

void cosim::deleteInstance()
{
    if (pInstance == NULL) return;
    delete pInstance;
    pInstance = NULL;
}

//
// Constructor
//
cosim::cosim() : testBase("COSIM")
{
#ifndef MAXION_SA
    if (saw_shutdown) return;
    // Try different configuration options
    config_.parse_env();
    if (file_utils::exists("cosim.toml")) {
        config_.parse("cosim.toml");
    } else if (file_utils::exists("arg_desc.txt")) {
        config_.parse_arg_desc();
    } else {
        config_.parse_plusargs();
    }

    // Setup recorder to save DPI events
    if (config_.monitor.enabled) {
        rec = new recorder(config_.monitor.trace_path);
        dispatcher::instance()->add_handler(rec);
    }

    // Dump the CoSim config
    config_.write("cosim_config.dump");

    // Link with DPI events
    dispatcher::instance()->add_handler(this);

    // Setup checker
    if (!config_.checker.bypassed) {
        check = new checker(config_.checker.enabled);
        // Initialize PCs across threads
        for (unsigned t = 0; t < EMU_NUM_THREADS; ++t) {
            unsigned s = t / EMU_THREADS_PER_SHIRE;
#if EMU_HAS_SPIO
            if (s == EMU_IO_SHIRE_SP) {
                check->start_pc(t, config_.spio_start_pc);
            } else
#endif
            if (s != CONTROL_SHIRE_VIRT) {
                check->start_pc(t, config_.start_pc);
            } else {
                check->start_pc(t, config_.ctrl_start_pc);
            }
        }
    }

    // Setup logging
    if (spdlog::get("cosim")) spdlog::drop("cosim"); // Shouldn't really happen..
    if (!config_.log_path.empty()) {
        if ((size_t)config_.log_limit != 0 && config_.log_files != 0) {
            logger = rotating_logger_st("cosim", config_.log_path, (size_t)config_.log_limit, config_.log_files);
        } else {
            logger = file_logger_st("cosim", config_.log_path, true);
        }
    } else {
        logger = spdlog::stdout_logger_st("cosim");
    }
    logger->set_pattern("%n(%l)\t%v");

    // Set various log levels
    logger->set_level(config_.log_level);
    testLog::setGlobalLogLevel(config_.checker.log_level);
    if (check) {
        check->system().log.setLogLevel(config_.checker.log_level);
        check->system().log_thread.set(); // Enable logging on all threads
    }

    // Load memory contents
    if (!config_.checker.bypassed) {
        if (file_utils::exists(config_.mem_desc)) {
            parseMemFile(config_.mem_desc.c_str());
        }
    }

    logger->info("Cosim Checker: {}", config_.checker.enabled ? "ENABLED" : "DISABLED");
    logger->info("Event Monitor: {}", config_.monitor.enabled ? "ENABLED" : "DISABLED");

    // Register a signal handler for SLURM timeout (i.e. SIGTERM)
    if (config_.sigterm_shutdown) {
        struct sigaction sa;
        sa.sa_handler = cosim::shutdown;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGTERM, &sa, NULL);
    }
#endif
}

unsigned cosim::get_inst_retire_count(unsigned minion_id, bool thread_id)
{
    return nrCommits_[minion_id][thread_id];
}

void cosim::final_report()
{
    if (final_report_done++ > 0) {
        return;
    }

    logger->info("Total number of retired instructions: {}", retire_count);

    if (log.getLogLevel() < 3 && config_.minion_stats) dump_stats_2_file("statistics_minions.log");
    if (config_.event_stats) dispatcher::instance()->write_stats("cosim_evt.stats");

    if ((check != NULL) && (retire_count == 0)) {
        logger->log(check->is_enabled() ? spdlog::level::err : spdlog::level::warn, "No instructions were retired");
    }
}

//
// Destructor
//
cosim::~cosim()
{
    if (saw_shutdown) return;
    final_report();
    if (!is_empty()) {
        logger->log((testMain::cleanEnd() && check->is_enabled()) ? spdlog::level::err : spdlog::level::warn,
                    "Test did not finsish. Checker is not empty");
    }
    if (check != nullptr) {
        logger->info("End of Test: Deleting Checker TB");
        delete check;
        check = nullptr;
    }
    if (rec != nullptr) {
        delete rec;
        rec = nullptr;
    }
    dispatcher::deleteInstance();
}

//
// Parses a file that defines the memory regions plus contents to be
// loaded in the different regions
//
bool cosim::parseMemFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        logger->error("Unable to open file '{}'", filename);
        return false;
    }

    // Parses the contents
    char buffer[1024];
    char* buf_ptr = (char*)buffer;
    size_t buf_size = 1024;
    while (getline(&buf_ptr, &buf_size, file) != -1) {
        uint64_t base;
        uint64_t top;
        uint64_t size;
        char str[1024];
        if (sscanf(buffer, "New Mem Region: 40'h%llX, 40'h%llX, %s", (unsigned long long*)&base,
                   (unsigned long long*)&size, str) == 3) {
            logger->warn("New Mem Region ignored '{}'", str);
        } else if (sscanf(buffer, "File Load: 40'h%llX, %s", (unsigned long long*)&base, str) == 2) {
            logger->info("File Load: {:#010x}", base);
            try
            {
                check->system().load_raw(str, base);
            }
            catch (...)
            {
                logger->error("Unable to load file '{}'", str);
            }
        } else if (sscanf(buffer, "ELF Load: %s", str) == 1) {
            logger->info("ELF Load: {}", str);
            try
            {
                check->system().load_elf(str);
            }
            catch (...)
            {
                logger->error("Unable to load ELF '{}'", str);
            }
        } else if (sscanf(buffer, "Ignore Data: 40'h%llX, 40'h%llX", (unsigned long long*)&base,
                          (unsigned long long*)&top) == 2) {
            if (check != NULL) {
                logger->info("New ignored memory region: base={:#010x} top={:#010x}", base, top);
                check->add_ignored_mem_region(base, top);
            }
        }
    }
    // Closes the file
    fclose(file);
    return true;
}

/*** Monitor Interface ***/
void cosim::on(const inst_retire_t* evt) try
{
    // Get context of Retire event
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);
    const insn_desc desc(hart, evt->pc);

    logger->info(LOG_PREFIX "Retire({}): {} (enc: {:#x})", evt->cycle, hart, desc, evt->tag(), get_prvname(evt->prv),
                 riscv_disasm(evt->inst_bits), evt->inst_bits);

    if (!checker_is_on()) return;

    // Update minion statistics
    retire_count++;
    stat_minion[minion_id].num_instr++;
    stat_minion[minion_id].num_instr_thread[evt->thread_id]++;
    if (evt->tensor_fma) stat_minion[minion_id].num_fma++;
    if (evt->tensor_load) stat_minion[minion_id].num_tensor_load[evt->tenb]++;
    if (evt->tensor_store) stat_minion[minion_id].num_tensor_store++;
    if (evt->reduce) stat_minion[minion_id].num_tensor_reduce++;
    if (evt->tensor_quant) stat_minion[minion_id].num_tensor_quant++;

    // Update log windows (for debugging)
    set_log_level_4_window(evt->cycle);
    monitor_num_instr(evt->cycle, minion_id);

    // Create entry for pending instruction queue
    cosim_insn inst{}; // Default initialized to 0's
    inst.type_id = cosim_insn::type::INSTRUCTION;
    inst.hart = hart;
    inst.desc = desc;

    // Copy flags from event
    inst.delayed_write = evt->delayed_write;
    inst.load_miss = evt->load_miss;
    inst.delayed_store = evt->delayed_store && evt->store;
    inst.tensor_load = evt->tensor_load;
    inst.tenb = evt->tenb;
    inst.tensor_load_num_lines = evt->tensor_load ? evt->tensor_load_num_lines + 1 : 0;
    inst.tensor_fma = evt->tensor_fma;
    inst.tensor_quant = evt->tensor_quant;
    inst.reduce = evt->reduce;
    inst.tensor_store = evt->tensor_store;
    inst.store_age_id = evt->store_age_id;
    inst.pending_fcsr = evt->wflags;
    inst.delayed_addr = evt->wbx_rd; // == wbf_rd == wbm_rd
    inst.trans = evt->trans_id != 0;
    inst.trans_status = evt->trans_id;
    // Reset reduce info
    inst.reduce_action = REDUCE_NO_ACTION;
    inst.reduce_partner = -1;
    // Setup FLB book-keeping
    if (evt->flb_valid) {
        inst.is_flb = true;
        inst.flb_return_val = -1;
        inst.flb_min_to_wakeup = -1;
        inst.flb_dest = evt->flb_idx;
        inst.flb_idx = evt->flb_range & 0x1f;
        inst.flb_match = ((evt->flb_range >> 5) & 0xff) + 1;
        pending_flb[inst.hart.shire_id()][inst.flb_idx].push_back(inst.hart.u32());
    }

    // Populate state changes
    auto& changes = inst.state_change;
    changes.cycle = evt->cycle;
    changes.pc = evt->pc;
    changes.inst_bits = evt->inst_bits;
    changes.prv = evt->prv;

    // Int-Reg update
    changes.int_reg_mod = evt->wbx;
    changes.int_reg_rd = evt->wbx_rd;
    changes.int_reg_data = evt->wbx_data;
    if (evt->wbx) {
        if (!evt->delayed_write) { // We already have the data
            logger->info(LOG_PREFIX "  x{} = {:#x}", evt->cycle, hart, inst.desc, evt->tag(), evt->wbx_rd,
                         evt->wbx_data);
        } else if (evt->load_miss) { // Pending load
            logger->info(LOG_PREFIX "  x{} <= [{}] (load_miss)", evt->cycle, hart, inst.desc, evt->tag(), evt->wbx_rd,
                         delayed_write_int_t::tag());
        } else if (evt->flb_valid) { // FLB load
            logger->info(LOG_PREFIX "  x{} <= [{}] (flb_id={}, flb_match={})", evt->cycle, hart, inst.desc, evt->tag(),
                         evt->wbx_rd, flb_update_t::tag(), inst.flb_idx, inst.flb_match);
            logger->debug(LOG_PREFIX "FLB[{}]: New entry", evt->cycle, hart, inst.desc, evt->tag(), inst.flb_idx);
        } else { // Normal regwrite
            logger->info(LOG_PREFIX "  x{} <= [{}]", evt->cycle, hart, inst.desc, evt->tag(), evt->wbx_rd,
                         delayed_write_int_t::tag());
        }
    }

    // FP-Reg update
    changes.fp_reg_mod = evt->wbf;
    changes.fp_reg_rd = evt->wbf_rd;
    if (evt->wbf) {
        if (!evt->delayed_write) { // Copy data from vpu_rf iff available on retire
            // We can probably remove this, since the only way this data is available is if it is an all-zero gather
            const auto& vpu_data = vpu_rf[minion_id][evt->thread_id][evt->wbf_rd];
            logger->info(LOG_PREFIX "  f{} = <data>", evt->cycle, hart, inst.desc, evt->tag(), evt->wbf_rd);
            for (unsigned i = 0; i < bemu::VLENW; i += 2) { // FUTURE Review data size u32 vs. u64
                const uint64_t data0 = vpu_data[i];
                const uint64_t data1 = vpu_data[i + 1];
                changes.fp_reg_data[i / 2] = data0 | (data1 << 32);
                logger->debug(LOG_PREFIX "  f{}[{}] = {:#x}", evt->cycle, hart, inst.desc, evt->tag(), evt->wbf_rd, i,
                              data0);
                logger->debug(LOG_PREFIX "  f{}[{}] = {:#x}", evt->cycle, hart, inst.desc, evt->tag(), evt->wbf_rd,
                              i + 1, data1);
            }
        } else {
            logger->info(LOG_PREFIX "  f{} <= [{}]", evt->cycle, hart, inst.desc, evt->tag(), evt->wbf_rd,
                         delayed_write_fp_t::tag());
            last_vpu_update[hart.u32()][evt->wbf_rd] = inst.desc;
        }
    }

    // Flags update
    changes.fflags_mod = evt->wflags;
    if (evt->wflags) {
        logger->info(LOG_PREFIX "  fcsr <= [{}]", evt->cycle, hart, inst.desc, evt->tag(), update_fcsr_flags_t::tag());
    }

    // Mask-Reg update
    assert((!(evt->wbmall || evt->wbm) || evt->delayed_write) && "Mask updates are expected to be delayed writes");
    if (evt->wbmall) {
        changes.m_reg_mod.set();
        logger->info(LOG_PREFIX "  m[0..7] <= [{}] ", evt->cycle, hart, inst.desc, evt->tag(),
                     delayed_write_rm_t::tag());
    } else if (evt->wbm) {
        assert(evt->wbm_rd < 8);
        changes.m_reg_mod[evt->wbm_rd] = true;
        logger->info(LOG_PREFIX "  m{} <= [{}]", evt->cycle, hart, inst.desc, evt->tag(), evt->wbm_rd,
                     delayed_write_rm_t::tag());
    }

    // Stores
    if (evt->store) {
        const bool is_packed_single = evt->store_size == (4 * bemu::VLENW);
        if (evt->scatter) { // Store/Scatter
            changes.mem_mod = evt->store_mask;
            const uint64_t mask = mem_mask(evt->store_size);
            for (unsigned i = 0; i < bemu::VLENW; ++i) {
                if (!changes.mem_mod[i]) continue;
                changes.mem_size[i] = evt->store_size;
                changes.mem_vaddr[i] = evt->scatter_addr[i];
                if (evt->delayed_store) {
                    logger->info(LOG_PREFIX "  MEM[{:#010x}] <= [{}] (scatter, age_id={})", evt->cycle, hart, inst.desc,
                                 evt->tag(), changes.mem_vaddr[i], delayed_store_t::tag(), inst.store_age_id);
                } else {
                    changes.mem_data[i] = evt->scatter_data[i];
                    logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x} (scatter, age_id={})", evt->cycle, hart, inst.desc,
                                 evt->tag(), changes.mem_vaddr[i], changes.mem_data[i] & mask, inst.store_age_id);
                }
            }
        } else if (is_packed_single) { // Store/PS
            changes.mem_mod = evt->store_mask;
            const auto stride = evt->store_size / bemu::VLENW;
            const uint64_t mask = mem_mask(stride);
            for (unsigned i = 0; i < bemu::VLENW; ++i) {
                if (!changes.mem_mod[i]) continue;
                changes.mem_size[i] = stride;
                changes.mem_vaddr[i] = evt->store_addr + stride * i;
                if (evt->delayed_store) {
                    logger->info(LOG_PREFIX "  MEM[{:#010x}] <= [{}] (ps, age_id={}, size={})", evt->cycle, hart,
                                 inst.desc, evt->tag(), changes.mem_vaddr[i], delayed_store_t::tag(), inst.store_age_id,
                                 changes.mem_size[i]);
                } else {
                    changes.mem_data[i] = evt->store_data[i / 2] >> (32 * (i % 2));
                    logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x} (ps, age_id={}, size={})", evt->cycle, hart,
                                 inst.desc, evt->tag(), changes.mem_vaddr[i], changes.mem_data[i] & mask,
                                 inst.store_age_id, changes.mem_size[i]);
                }
            }
        } else { // Store/Normal
            assert(evt->store_size <= 8);
            changes.mem_mod[0] = true;
            changes.mem_size[0] = evt->store_size;
            changes.mem_vaddr[0] = evt->store_addr;
            const uint64_t mask = mem_mask(evt->store_size);
            if (evt->delayed_store) {
                logger->info(LOG_PREFIX "  MEM[{:#010x}] <= [{}] (age_id={}, size={})", evt->cycle, hart, inst.desc,
                             evt->tag(), changes.mem_vaddr[0], delayed_store_t::tag(), inst.store_age_id,
                             changes.mem_size[0]);
            } else {
                changes.mem_data[0] = evt->store_data[0];
                logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x} (age_id={}, size={})", evt->cycle, hart, inst.desc,
                             evt->tag(), changes.mem_vaddr[0], changes.mem_data[0] & mask, inst.store_age_id,
                             changes.mem_size[0]);
            }
        }
    }

    // Tensor..
    if (inst.tensor_load) {
        logger->info(LOG_PREFIX "TensorLoad pending [TLD]", evt->cycle, hart, inst.desc, evt->tag());
    } else if (inst.tensor_fma) {
        logger->info(LOG_PREFIX "TensorFMA pending [{}]", evt->cycle, hart, inst.desc, evt->tag(),
                     tensorfma_done_t::tag());
        last_tensorfma[hart.u32()] = inst.desc;
        last_vpu_update[hart.u32()].fill(inst.desc);
    } else if (inst.tensor_quant) {
        logger->info(LOG_PREFIX "TensorQuant pending [{}]", evt->cycle, hart, inst.desc, evt->tag(),
                     tensorquant_done_t::tag());
        last_tensorquant[hart.u32()] = inst.desc;
        last_vpu_update[hart.u32()].fill(inst.desc);
    } else if (inst.reduce) {
        logger->info(LOG_PREFIX "TensorReduce pending [{}]", evt->cycle, hart, inst.desc, evt->tag(),
                     reduce_done_t::tag());
        last_reduce[hart.u32()] = inst.desc;
        last_vpu_update[hart.u32()].fill(inst.desc);
    } else if (inst.tensor_store) {
        logger->info(LOG_PREFIX "TensorStore pending [{}]", evt->cycle, hart, inst.desc, evt->tag(),
                     tensorstore_done_t::tag());
        last_tensorstore[hart.u32()] = inst.desc;
    }

    pending_queue[minion_id].push_back(inst);
    check_ready_instructions(evt->cycle, minion_id);
    nrCommits_[minion_id][evt->thread_id]++;
    if (nrCommits_[minion_id][evt->thread_id] == config_.max_commits)
        max_commits(minion_id, evt->thread_id, evt->cycle);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const inst_exception_t* evt) try
{
    if (!checker_is_on()) return;

    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);
    const insn_desc desc(hart, evt->pc);

    logger->info(LOG_PREFIX "Except({}): {} (enc: {:#x}) - Cause: {:#x} - TVal: {:#x}", evt->cycle, hart, desc,
                 evt->tag(), get_prvname(evt->prv), riscv_disasm(evt->inst_bits), evt->inst_bits, evt->cause,
                 evt->tval);

    cosim_insn inst{};
    inst.hart = hart;
    inst.desc = desc;

    base_state_change rtl_state_change{};
    rtl_state_change.cycle = evt->cycle;
    rtl_state_change.pc = evt->pc;
    rtl_state_change.inst_bits = evt->inst_bits;
    rtl_state_change.int_reg_mod = false;
    rtl_state_change.int_reg_rd = 0;
    rtl_state_change.int_reg_data = 0;
    rtl_state_change.fp_reg_mod = evt->is_gsc && (!evt->is_store || evt->is_atomic);
    rtl_state_change.fp_reg_rd = evt->wbf_rd;
    rtl_state_change.fflags_mod = false;
    rtl_state_change.fflags_value = 0;
    rtl_state_change.trap_mod = true;

    for (unsigned i = 0; i < bemu::VLENW; i++) {
        rtl_state_change.mem_mod[i] = evt->is_gsc && evt->is_store && ((evt->store_mask >> i) & 0x1)
                                      && (i < evt->gsc_progress);
        rtl_state_change.mem_size[i] = evt->gsc_size;
        rtl_state_change.mem_vaddr[i] = evt->scatter_addr[i];
        rtl_state_change.mem_data[i] = evt->scatter_data[i];
        if (rtl_state_change.mem_mod[i]) {
            logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x} (size: {})", evt->cycle, hart, inst.desc, evt->tag(),
                         evt->scatter_addr[i], evt->scatter_data[i], evt->gsc_size);
        }
        rtl_state_change.m_reg_mod[i] = false;
    }

    rtl_state_change.mstatus = evt->mstatus;
    rtl_state_change.cause = evt->cause;
    rtl_state_change.tval = evt->tval;
    rtl_state_change.epc = evt->epc;
    rtl_state_change.mip = evt->mip;
    rtl_state_change.prv = evt->prv;
    rtl_state_change.gsc_progress_mod = evt->is_gsc;
    rtl_state_change.gsc_progress = evt->gsc_progress;
    rtl_state_change.bus_err_addr = 0;

    // Excepts a delayed write when is a gather with some
    // non-masked access before the exception
    bool expects_delayed_write = false;
    if (evt->is_gsc && (!evt->is_store || evt->is_atomic)) {
        for (unsigned i = 0; i < evt->gsc_progress; ++i) {
            if ((evt->store_mask >> i) & 0x1) expects_delayed_write = true;
        }
    }

    inst.state_change = rtl_state_change;
    inst.delayed_write = expects_delayed_write;
    inst.delayed_store = false;
    inst.delayed_addr = evt->wbf_rd;
    inst.tensor_load = false;
    inst.tenb = false;
    inst.tensor_load_op_id = 0;
    inst.tensor_load_done = false;
    inst.tensor_load_tenb_flush = false;
    inst.tensor_fma = false;
    inst.tensor_quant = false;
    inst.tensor_store = false;
    inst.reduce = false;
    inst.is_flb = false;
    inst.store_age_id = 0;
    inst.pending_fcsr = false;
    inst.trans = false;
    inst.trans_status = 0;
    inst.type_id = cosim_insn::type::EXCEPTION;

    // Push it to emu so it can emulate the trap and check partial results (gather/scatter)
    pending_queue[minion_id].push_back(inst);
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const warm_reset_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    logger->info(LOG_PREFIX_SHORT "Warm Reset", evt->cycle, hart);

    pending_queue[minion_id].clear();
    logger->debug(LOG_PREFIX_SHORT "Clearing all pending instructions", evt->cycle, hart);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const inst_interrupt_t* evt) try
{
    if (!checker_is_on()) return;

    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);
    const insn_desc desc(hart, evt->pc);

    const auto int_cause = evt->cause & 0x1f;

    logger->info(LOG_PREFIX "Inter({}):  {} (enc: {:#x}) - Type: {:#x}", evt->cycle, hart, desc, evt->tag(),
                 get_prvname(evt->prv), riscv_disasm(evt->inst_bits), evt->inst_bits, int_cause);

    cosim_insn inst{};
    inst.hart = hart;
    inst.desc = desc;

    base_state_change rtl_state_change{};
    rtl_state_change.cycle = evt->cycle;
    rtl_state_change.pc = evt->pc;
    rtl_state_change.inst_bits = evt->inst_bits;
    rtl_state_change.trap_mod = true;
    rtl_state_change.mstatus = evt->mstatus;
    rtl_state_change.cause = evt->cause;
    rtl_state_change.tval = evt->tval;
    rtl_state_change.epc = evt->epc;
    rtl_state_change.mip = evt->mip;
    rtl_state_change.prv = evt->prv;
    rtl_state_change.gsc_progress_mod = evt->is_gsc;
    rtl_state_change.gsc_progress = evt->gsc_progress;
    rtl_state_change.bus_err_addr = 0;

    if (int_cause == 0x17) {
        rtl_state_change.bus_err_addr = evt->bus_error_addr;
        const auto is_load_miss = [hart, evt](const cosim_insn& insn) {
            return insn.delayed_write && insn.load_miss && (insn.hart == hart) && insn.state_change.int_reg_mod
                   && insn.state_change.pc == evt->bus_error_pc;
        };
        auto& q = pending_queue[minion_id];
        auto it = std::find_if(q.begin(), q.end(), is_load_miss);
        if (it != q.end()) {
            it->delayed_write = false;
            it->load_miss = false;
            it->state_change.bus_error = true;
        }
    }

    // put the interrupt in the instructions queue (so that it is synchronized with the checker execution)
    inst.state_change = std::move(rtl_state_change);
    inst.type_id = cosim_insn::type::INTERRUPT;

    pending_queue[minion_id].push_back(inst);
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

uint32_t uniqify_interrupts(uint32_t mip)
{
    if (mip && 0x1)
        return (0);
    else if (mip && 0x2)
        return (1);
    else if (mip && 0x4)
        return (2);
    else if (mip && 0x8)
        return (3);
    else if (mip && 0x10)
        return (4);
    else {
        // log << LOG_ERR << "Invalid mip value" << std::hex << mip <<  " being cleared " << std::dec << endm;
        return 0xff;
    }
}

void cosim::on(const clear_interrupt_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);
    logger->info(LOG_PREFIX_SHORT "Clear external interrupt MIP: {:#x}", evt->cycle, hart, evt->mip);

    switch (uniqify_interrupts(evt->mip)) {
    case 0:
        check->system().cpu[hart.u32()].clear_interrupt(SUPERVISOR_EXTERNAL_INTERRUPT);
        break;
    case 1:
        check->system().cpu[hart.u32()].clear_interrupt(MACHINE_EXTERNAL_INTERRUPT);
        break;
    case 2:
        check->system().cpu[hart.u32()].clear_interrupt(MACHINE_SOFTWARE_INTERRUPT);
        break;
    case 3:
        check->system().cpu[hart.u32()].clear_interrupt(MACHINE_TIMER_INTERRUPT);
        break;
    }
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Deals with updating the FP flags
void cosim::on(const update_fcsr_flags_t* evt) try
{
    if (!checker_is_on()) return;
    static int fflags_released = 0;

    const auto minion_id = fixup_ioshire_id(evt->minion_id);

    const hart_id hart(minion_id, evt->thread_id);
    const unsigned eflags = (evt->flags >> 5) & 0x1;
    const unsigned fflags = evt->flags & 0x1F;
    const unsigned fcsr = (eflags << 31) | fflags;
    const bool trans_update = evt->trans_id != 0;

    auto& q = pending_queue[minion_id];

    const auto pending_fcsr = [hart](const cosim_insn& insn) { return insn.hart == hart && insn.pending_fcsr; };
    const auto is_tensor = [](const cosim_insn& insn) { return insn.tensor_fma || insn.reduce || insn.tensor_quant; };
    const auto is_trans = [evt, trans_update](const cosim_insn& insn) {
        return insn.trans == trans_update && insn.trans_status == evt->trans_id;
    };

    auto it = std::find_if(q.begin(), q.end(), [&pending_fcsr, &is_tensor, &is_trans](const cosim_insn& insn) {
        return pending_fcsr(insn) && (is_tensor(insn) || is_trans(insn));
    });

    if (it == q.end()) {
        if (fcsr == 0) {
            // Waive this error if fcsr flags are updated with 0
            logger->warn(LOG_PREFIX "  fcsr = 0x0 [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag());
        } else {
            logger->error(LOG_PREFIX "  fcsr = {:#x} [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(),
                          fcsr);
            throw unexpected_event(evt->event_id());
        }
    }

    it->state_change.fflags_value |= fcsr;
    if (is_tensor(*it)) return;

    it->pending_fcsr = false;
    if (fcsr == 0) { // no modification
        it->state_change.fflags_mod = false;
    }
    ++fflags_released;

    logger->info(LOG_PREFIX "  fcsr = {:#x} (fflags_released={})", evt->cycle, hart, it->desc, evt->tag(), fcsr,
                 fflags_released);
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Deals with writes delayed due long latency (divs, load misses, etc...)
void cosim::on(const delayed_write_int_t* evt) try
{
    if (!checker_is_on()) return;

    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    const auto writes_xreg = [evt, hart](const cosim_insn& insn) {
        return insn.delayed_write && insn.hart == hart && insn.state_change.int_reg_mod
               && (insn.delayed_addr == evt->wbx_rd);
    };
    auto& q = pending_queue[minion_id];
    auto it = std::find_if(q.begin(), q.end(), writes_xreg);

    if (it == q.end()) {
        logger->error(LOG_PREFIX "  x{} = {:#x} [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(),
                      evt->wbx_rd, evt->wbx_data);
        throw unexpected_event(evt->event_id());
    }

    logger->info(LOG_PREFIX "  x{} = {:#x}", evt->cycle, hart, it->desc, evt->tag(), evt->wbx_rd, evt->wbx_data);

    it->delayed_write = false;
    it->state_change.int_reg_data = evt->wbx_data;
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// FLB Updates
void cosim::on(const flb_update_t* evt) try
{
    if (!checker_is_on()) return;

    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    const auto waits_for_flb = [hart, evt](const cosim_insn& insn) {
        return insn.is_flb && (insn.flb_return_val == -1) && (insn.hart == hart) && insn.state_change.int_reg_mod
               && (insn.flb_dest == evt->wbx_rd);
    };

    auto& q = pending_queue[minion_id];
    auto it = std::find_if(q.begin(), q.end(), waits_for_flb);

    if (it == q.end()) {
        logger->error(LOG_PREFIX "  x{} = {} [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(), evt->wbx_rd,
                      evt->wbx_data);
        throw unexpected_event(evt->event_id());
    }

    logger->info(LOG_PREFIX "  x{} = {}", evt->cycle, hart, it->desc, evt->tag(), evt->wbx_rd, evt->wbx_data);

    it->delayed_write = false;
    it->flb_return_val = evt->wbx_data;
    it->state_change.int_reg_data = evt->wbx_data;
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Load Miss with Bus Error
void cosim::on(const load_miss_bus_err_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    auto& q = pending_queue[minion_id];
    const auto is_load_miss = [hart, evt](const cosim_insn& insn) {
        return insn.delayed_write && insn.load_miss && (insn.hart == hart) && insn.state_change.int_reg_mod
               && insn.state_change.pc == evt->pc;
    };
    auto it = std::find_if(q.begin(), q.end(), is_load_miss);
    if (it == q.end()) return; // LOG_ERR?

    logger->info(LOG_PREFIX "  x{} <= <bus_err>", evt->cycle, hart, it->desc, evt->tag(), it->delayed_addr);

    it->delayed_write = false;
    it->load_miss = false;
    it->state_change.bus_error = true;
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Deals with stores that had a miss
void cosim::on(const delayed_store_t* evt) try
{
    if (!checker_is_on()) return;
    const int minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    const auto is_delayed_store = [hart, evt](const cosim_insn& insn) {
        return insn.delayed_store && (insn.hart == hart) && (insn.store_age_id == evt->store_age_id);
    };
    auto& q = pending_queue[minion_id];
    // Looks for the first instruction that writes the addr
    auto it = std::find_if(q.begin(), q.end(), is_delayed_store);

    if (it == q.end()) {
        logger->error(LOG_PREFIX "Store with age_id: {} [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(),
                      evt->store_size);
        throw unexpected_event(evt->event_id());
    }

    it->delayed_store = false;

    const bool is_packed_single = evt->store_size == (4 * bemu::VLENW);

    auto& changes = it->state_change;

    // Updates the contents
    if (evt->scatter) {
        const uint64_t addr = changes.mem_vaddr[evt->scatter_cnt];
        const uint64_t data = evt->store_data[0];
        const uint64_t mask = mem_mask(changes.mem_size[evt->scatter_cnt]);
        if (!changes.mem_mod[evt->scatter_cnt]) {
            logger->error(LOG_PREFIX "MEM[?] = {:#x} (cnt={}, age_id={}) [unexpected]", evt->cycle, hart, it->desc,
                          evt->tag(), data & mask, evt->scatter_cnt, evt->store_age_id);
            throw unexpected_event(evt->event_id());
        }
        logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x} (cnt={})", evt->cycle, hart, it->desc, evt->tag(), addr,
                     data & mask, evt->scatter_cnt);
        changes.mem_data[evt->scatter_cnt] = data;
    } else if (is_packed_single) {
        for (unsigned i = 0; i < bemu::VLENW; i++) {
            if (!changes.mem_mod[i]) continue;
            const uint64_t addr = changes.mem_vaddr[i];
            const uint64_t data = evt->store_data[i / 2] >> (32 * (i % 2));
            const uint64_t mask = mem_mask(changes.mem_size[i]);
            logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x}", evt->cycle, hart, it->desc, evt->tag(), addr,
                         data & mask);
            changes.mem_data[i] = data;
        }
    } else {
        const uint64_t addr = changes.mem_vaddr[0];
        const uint64_t data = evt->store_data[0];
        const uint64_t mask = mem_mask(changes.mem_size[0]);
        logger->info(LOG_PREFIX "  MEM[{:#010x}] = {:#x}", evt->cycle, hart, it->desc, evt->tag(), addr, data & mask);
        changes.mem_data[0] = data;
    }

    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Deals with writes delayed to mask
void cosim::on(const delayed_write_rm_t* evt) try
{
    if (!checker_is_on()) return;
    const int minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    if (!evt->wbmall) assert(evt->wbm_rd < 8);

    const auto writes_all = [hart](const cosim_insn& insn) {
        return insn.delayed_write && insn.hart == hart && insn.state_change.m_reg_mod.all();
    };

    const auto writes_mreg = [hart, evt](const cosim_insn& insn) {
        return insn.delayed_write && insn.hart == hart && insn.state_change.m_reg_mod[evt->wbm_rd];
    };

    auto& q = pending_queue[minion_id];
    // Looks for the first instruction that writes the addr
    auto it = evt->wbmall ? std::find_if(q.begin(), q.end(), writes_all)
                          : std::find_if(q.begin(), q.end(), writes_mreg);

    if (it == q.end()) { // Shouldn't get here
        const char mask_fmt = evt->wbmall ? '@' : ('0' + evt->wbm_rd);
        logger->error(LOG_PREFIX "  m{} = {:#x} [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(), mask_fmt,
                      evt->wbm_data);
        throw unexpected_event(evt->event_id());
    }

    it->delayed_write = false;

    const auto update_mreg = [&](uint32_t rd, uint64_t data) {
        logger->info(LOG_PREFIX "  m{} = {:#02x}", evt->cycle, hart, it->desc, evt->tag(), rd, data);
        auto& mreg = it->state_change.m_reg_data[rd];
        for (unsigned i = 0; i < bemu::VLENW; ++i) {
            mreg[i] = (data >> i) & 1;
        }
    };

    if (evt->wbmall) {
        for (unsigned m = 0; m < bemu::NMREGS; ++m) {
            const uint64_t data = evt->wbm_data >> (bemu::VLENW * m) & 0xff;
            update_mreg(m, data);
        }
    } else {
        update_mreg(evt->wbm_rd, evt->wbm_data & 0xff);
    }

    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const ipi_received_t* evt) try
{
    if (!checker_is_on()) return;
    const int minion_id = fixup_ioshire_id(evt->minion_id);

    // Queue the event in the instruction queue
    cosim_insn ipi{};
    ipi.hart = hart_id(minion_id, evt->thread_id);
    ipi.ipi_pc = evt->pc;
    ipi.type_id = cosim_insn::type::IPI;

    pending_queue[minion_id].push_back(ipi);

    logger->debug("{} ns\t{}\tReceived IPI (PC:{:#010x})", evt->cycle, ipi.hart, ipi.ipi_pc);

    stat_minion[minion_id].num_ipi++;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorfma_write_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    const auto& desc = last_tensorfma[hart.u32()];
    const auto& vpu_data = vpu_rf[minion_id][0][evt->entry];

    logger->debug(LOG_PREFIX "TensorFMA write (entry={}, mask={:#x})", evt->cycle, hart, desc, evt->tag(), evt->entry,
                  evt->wmask);

    check->tensorfma_write(hart.u32(), evt->entry, evt->wmask, vpu_data.data());
    stat_minion[minion_id].num_fma_transactions++;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorfma_write8_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    const auto& desc = last_tensorfma[hart.u32()];

    // NOTE Data is written as part of vpu_rf_update
    logger->debug(LOG_PREFIX "TensorFMA write8 (entry={}, mask={:#x})", evt->cycle, hart, desc, evt->tag(), evt->entry,
                  evt->wmask);
    check->tensorfma_write(hart.u32(), evt->entry, evt->wmask, evt->data);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorquant_write_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    const auto& desc = last_tensorquant[hart.u32()];
    const auto& vpu_data = vpu_rf[minion_id][0][evt->entry];

    // NOTE Do not log mask, since it is hardcoded to all 1's
    logger->debug(LOG_PREFIX "TensorQuant write (entry={})", evt->cycle, hart, desc, evt->tag(), evt->entry);
    check->tensorquant_write(hart.u32(), evt->entry, evt->wmask, vpu_data.data());

    stat_minion[minion_id].num_tensor_quant_transactions++;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const reduce_write_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    const auto& desc = last_reduce[hart.u32()];
    const auto& vpu_data = vpu_rf[minion_id][0][evt->entry];

    logger->debug(LOG_PREFIX "TensorReduce write (entry={})", evt->cycle, hart, desc, evt->tag(), evt->entry);
    check->reduce_write(hart.u32(), evt->entry, vpu_data.data());

    stat_minion[minion_id].num_tensor_reduce_transactions++;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorstore_write_t* evt) try
{
    if (!checker_is_on()) return;
    std::bitset<4> qwen{ evt->qwen };

    const hart_id hart(evt->minion_id, 0);
    const auto& desc = last_tensorstore[hart.u32()];

    const uint64_t mask = mem_mask(evt->size);

    const auto tstore_write = [&](unsigned i, uint32_t data) {
        const uint64_t addr = evt->addr + 4 * i;
        logger->debug(LOG_PREFIX "  MEM[{:#010x}] = {:#x}", evt->cycle, hart, desc, evt->tag(), addr, data & mask);
        check->tensorstore_write(hart.u32(), addr, data);
    };

    unsigned num_trans = 0;
    if (evt->size == 6 && qwen.count() == 4 && !evt->is_256_bit) {
        num_trans = 16;
    } else if (evt->size == 6 && qwen.count() == 4 && evt->is_256_bit) {
        num_trans = 8;
    } else if (evt->size == 5 && qwen.count() == 2) {
        num_trans = 8;
    } else if (evt->size == 4 && qwen.count() == 1) {
        num_trans = 4;
    }

    for (unsigned i = 0; i < num_trans; ++i) {
        tstore_write(i, evt->data[i]);
    }
    logger->debug(LOG_PREFIX "TensorStore write (qwen={}, subcode={})", evt->cycle, hart, desc, evt->tag(), evt->qwen,
                  evt->subcode);

    stat_minion[evt->minion_id].num_tensor_store_transactions += num_trans;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorfma_done_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    auto& q = pending_queue[minion_id];
    const auto is_tensor_fma = [](const cosim_insn& insn) { return insn.tensor_fma; };
    auto it = std::find_if(q.begin(), q.end(), is_tensor_fma);
    if (it == q.end()) { // Shouldn't get here
        logger->error(LOG_PREFIX "TensorFMA done [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag());
        throw unexpected_event(evt->event_id());
    }
    it->tensor_fma = false;
    it->pending_fcsr = false;
    if (it->state_change.fflags_value == 0) { // no modification
        it->state_change.fflags_mod = false;
    }

    const size_t size = check->tensorfma_size(hart.u32());
    logger->info(LOG_PREFIX "TensorFMA done ({} writes)", evt->cycle, hart, it->desc, evt->tag(), size);
    last_tensorfma[hart.u32()] = insn_desc();
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorquant_done_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    auto& q = pending_queue[minion_id];
    const auto is_tensor_quant = [](const cosim_insn& insn) { return insn.tensor_quant; };
    auto it = std::find_if(q.begin(), q.end(), is_tensor_quant);
    if (it == q.end()) { // Shouldn't get here
        logger->error(LOG_PREFIX "TensorQuant done [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag());
        throw unexpected_event(evt->event_id());
    }
    it->tensor_quant = false;
    it->pending_fcsr = false;
    if (it->state_change.fflags_value == 0) { // no modification
        it->state_change.fflags_mod = false;
    }
    const size_t size = check->tensorquant_size(hart.u32());
    logger->info(LOG_PREFIX "TensorQuant done ({} writes)", evt->cycle, hart, it->desc, evt->tag(), size);
    last_tensorquant[hart.u32()] = insn_desc();
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const reduce_done_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);

    if (evt->action > REDUCE_NO_ACTION || (evt->partner_minion_id >= 0x2000 && evt->partner_minion_id != 0xFFFFFFFF)) {
        logger->error(LOG_PREFIX "Wrong reduce action (= {}) or partner (= {})", evt->cycle, hart, insn_desc(hart),
                      evt->tag(), evt->action, evt->partner_minion_id);
        throw unexpected_event(evt->event_id());
    }

    auto& q = pending_queue[minion_id];
    const auto is_pending_reduce
        = [](const cosim_insn& insn) { return insn.reduce && insn.reduce_action == REDUCE_NO_ACTION; };

    auto it = std::find_if(q.begin(), q.end(), is_pending_reduce);
    if (it == q.end()) { // Shouldn't get here
        logger->error(LOG_PREFIX "TensorReduce done [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag());
        throw unexpected_event(evt->event_id());
    }

    it->reduce_action = evt->action;
    it->reduce_partner = evt->partner_minion_id;
    it->pending_fcsr = false;
    if (it->state_change.fflags_value == 0) { // no modification
        it->state_change.fflags_mod = false;
    }

    bool invalid_reduce =  minion_id == evt->partner_minion_id ||
                           evt->partner_minion_id > REDUCE_MAX_PARTNER_ID ||
                           evt->auto_pair && evt->level > REDUCE_MAX_LEVEL;

    if (evt->action == REDUCE_NO_ACTION || invalid_reduce || evt->zero_reg_reduce) {
        it->reduce = false;
        it->reduce_action = REDUCE_NO_ACTION; // Downgrade to NOP
        logger->info(LOG_PREFIX "TensorReduce done (skipped)", evt->cycle, hart, it->desc, evt->tag());
        check_ready_instructions(evt->cycle, minion_id); // Skip pairing
        return;
    } else if (evt->action == REDUCE_RECV_ACTION && !ReduceOperation::is_valid(evt->operation)) {
        it->reduce = false;
        it->reduce_action = REDUCE_NO_ACTION; // Downgrade to NOP
        logger->info(LOG_PREFIX "TensorReduce done (invalid)", evt->cycle, hart, it->desc, evt->tag());
        check_ready_instructions(evt->cycle, minion_id); // Skip pairing
        return;
    }

    const size_t size = check->reduce_size(hart.u32());
    logger->info(LOG_PREFIX "TensorReduce done ({} writes)", evt->cycle, hart, it->desc, evt->tag(), size);

    // Pair reduce instructions
    auto& partner_q = pending_queue[it->reduce_partner];
    const auto partners_with_me = [evt](const cosim_insn& insn) {
        return insn.reduce && (insn.reduce_partner == evt->minion_id) && !insn.reduce_partner_done;
    };
    auto partner_it = std::find_if(partner_q.begin(), partner_q.end(), partners_with_me);
    if (partner_it == partner_q.end()) {
        logger->info(LOG_PREFIX "Pending reduce partner (minion: {})", evt->cycle, hart, it->desc, evt->tag(),
                     evt->partner_minion_id);
    } else {
        // Partner is ready, check if Partner is not at head of Queue, then try swapping
        logger->info(LOG_PREFIX "Found partner {} (minion: {})", evt->cycle, hart, it->desc, evt->tag(),
                     partner_it->desc, it->reduce_partner);
        partner_it->reduce_partner_done = true;
        // Check if reduce partner already received reduce_done in the past (its action will be set)
        if (partner_it->reduce_action != REDUCE_NO_ACTION) {
            it->reduce_partner_done = true;
        }
        if (partner_it != partner_q.begin()) {
            unblock_queue(evt->cycle, partner_it->hart.minion_id());
        }
    }

    if (evt->action == REDUCE_SEND_ACTION) {
        // Sender: Commit sender reduce, commit receiver reduce, then drain as much as possible from sender
        // Draining handles the case when all instructions after the reduce are done
        // Question: Do we need that? The receiver always finishes second...
        const int oldest_partner_id = form_reduce_pair(evt->cycle, minion_id);
        if (oldest_partner_id >= 0) {
            check_ready_instructions(evt->cycle, minion_id, true);
            check_ready_instructions(evt->cycle, oldest_partner_id, true);
            if (!q.empty()) check_ready_instructions(evt->cycle, minion_id);
            if (!pending_queue[oldest_partner_id].empty()) check_ready_instructions(evt->cycle, oldest_partner_id);
        }
    } else if (evt->action == REDUCE_RECV_ACTION) {
        // Receiver: Commit sender reduce, commit receiver reduce, then drain as much as possible from sender
        // Draining handles the case when all instructions after the reduce are done
        check->advanceReduceListWriteIdx(minion_id << 1);
        const int oldest_partner_id = form_reduce_pair(evt->cycle, evt->partner_minion_id);
        if (oldest_partner_id >= 0) {
            check_ready_instructions(evt->cycle, evt->partner_minion_id, true);
            check_ready_instructions(evt->cycle, oldest_partner_id, true);
            if (!pending_queue[evt->partner_minion_id].empty())
                check_ready_instructions(evt->cycle, evt->partner_minion_id);
            if (!pending_queue[oldest_partner_id].empty()) check_ready_instructions(evt->cycle, oldest_partner_id);
        }
    }
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const tensorstore_done_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, 0);
    const auto is_tensor_store = [](const cosim_insn& insn) { return insn.tensor_store; };
    auto& q = pending_queue[minion_id];
    auto it = std::find_if(q.begin(), q.end(), is_tensor_store);
    if (it == q.end()) { // No pending tensorstore
        logger->error(LOG_PREFIX "TensorStore done [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag());
        throw unexpected_event(evt->event_id());
    }
    it->tensor_store = false;
    it->pending_fcsr = false;
    if (it->state_change.fflags_value == 0) { // no modification
        it->state_change.fflags_mod = false;
    }
    const size_t size = check->tensorstore_size(hart.u32());
    logger->info(LOG_PREFIX "TensorStore done ({} writes)", evt->cycle, hart, it->desc, evt->tag(), size);
    check->createTensorStoreList(hart.u32());
    last_tensorstore[hart.u32()] = insn_desc();
    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const esr_write_t* evt) try
{
    if (!checker_is_on()) return;
    logger->info("ESR[{:#010x}] = {:#x}", evt->addr, evt->data);
    check->esr_write(evt->addr, evt->data);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

// Deals with writes delayed due long latency (divs, load misses, etc...)
void cosim::on(const vpu_rf_update_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);

    std::bitset<bemu::VLENW> wr_mask{ evt->wr_mask };
    std::bitset<bemu::VLENW> thread_ids{ evt->thread_ids };

    auto& rf = vpu_rf[minion_id];

    for (unsigned i = 0; i < bemu::VLENW; i++) {
        if (!wr_mask.test(i)) continue;
        rf[thread_ids.test(i)][evt->addr[i]][i] = evt->data[i];
        const hart_id hart(minion_id, thread_ids[i]);
        const auto& desc = last_vpu_update[hart.u32()][evt->addr[i]];
        logger->debug(LOG_PREFIX "  f{}[{}] = {:#x}", evt->cycle, hart, desc, evt->tag(), evt->addr[i], i,
                      evt->data[i]);
    }
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

void cosim::on(const icache_prefetch_done_t* evt) try
{
    if (!checker_is_on()) return;
    logger->debug("Icache Prefetch for shire {} done", evt->shire_id);
    check->set_icache_prefetch_done(evt->shire_id);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

const char* DELAYED_FP_SRC[] = { "GSC_XCPT", "VPU_MASKED", "WB_REG", "WB_REG_MULTIPASS", "LOAD", "GSC" };

// Deals with writes delayed due long latency (divs, load misses, etc...)
void cosim::on(const delayed_write_fp_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const hart_id hart(minion_id, evt->thread_id);

    const auto writes_freg = [hart, evt](const cosim_insn& insn) {
        return insn.delayed_write && insn.hart == hart && insn.state_change.fp_reg_mod && insn.delayed_addr
                                                                                          == evt->wbf_rd;
    };
    auto& q = pending_queue[minion_id];
    auto it = std::find_if(q.begin(), q.end(), writes_freg);
    if (it == q.end()) {
        logger->error(LOG_PREFIX "  f{}[*] = <{}> [unexpected]", evt->cycle, hart, insn_desc(hart), evt->tag(),
                      evt->wbf_rd, DELAYED_FP_SRC[evt->entry_point_id]);
        throw unexpected_event(evt->event_id());
    }

    logger->info(LOG_PREFIX "  f{}[*] = <{}>", evt->cycle, hart, it->desc, evt->tag(), evt->wbf_rd,
                 DELAYED_FP_SRC[evt->entry_point_id]);

    it->delayed_write = false;
    // get delayed data with what is currently in the RF
    const auto& reg = vpu_rf[minion_id][evt->thread_id ? 1 : 0][evt->wbf_rd];
    for (unsigned i = 0; i < bemu::VLEND; ++i) {
        it->state_change.fp_reg_data[i] = (reg[i << 1]) | ((uint64_t)reg[(i << 1) + 1] << 32);
    }

    check_ready_instructions(evt->cycle, minion_id);
}
catch (const unexpected_event& err)
{
    log << LOG_ERR << "Unexpected Event: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

//////////////////////////////////////////
// Msg tracking - UC
/////////////////////////////////////////

// Requests coming from AXI
void cosim::uc_new_msg_remote_req(unsigned int shire_id_dest, unsigned int shire_id_orign)
{
    (void)shire_id_dest; (void)shire_id_orign;
    throw checker_error("Unexpected message port event: uc_new_msg_remote_req called but message ports are not supported");
}

// Request going out (Comes from ET-LINK)
void cosim::uc_new_msg_req(unsigned int shire_id, unsigned int shire_dest, unsigned int neigh_id,
                           unsigned int minion_src)
{
    (void)shire_id; (void)shire_dest; (void)neigh_id; (void)minion_src;
    throw checker_error("Unexpected message port event: uc_new_msg_req called but message ports are not supported");
}

//////////////////////////////////////////
// Msg tracking - SC
/////////////////////////////////////////

void cosim::sc_local_new_msg(unsigned int shire_id, unsigned int neigh_id, unsigned int minion_src,
                             unsigned int n_bank)
{
    (void)shire_id; (void)neigh_id; (void)minion_src; (void)n_bank;
    throw checker_error("Unexpected message port event: sc_local_new_msg called but message ports are not supported");
}

void cosim::sc_local_new_msg_rbox(unsigned int shire_id, unsigned int rbox_local_id, unsigned int n_bank,
                                  unsigned int thread_dest, unsigned int port_id, unsigned int* data,
                                  unsigned int oob)
{
    (void)shire_id; (void)rbox_local_id; (void)n_bank;
    (void)thread_dest; (void)port_id; (void)data; (void)oob;
    throw checker_error("Unexpected RBOX event: sc_local_new_msg_rbox called but RBOX is not supported");
}

void cosim::sc_to_neigh_new_msg(unsigned int shire_id, unsigned int neigh_dest, unsigned int origin, // bank id
                                bool is_uc)
{
    (void)shire_id; (void)neigh_dest; (void)origin; (void)is_uc;
    throw checker_error("Unexpected message port event: sc_to_neigh_new_msg called but message ports are not supported");
}

//////////////////////////////////////////
// Msg tracking - Neigh
/////////////////////////////////////////

void cosim::neigh_localnet_msg(unsigned int shire_id, unsigned int neigh_id, unsigned int minion_dest,
                               unsigned int minion_src)
{
    (void)shire_id; (void)neigh_id; (void)minion_dest; (void)minion_src;
    throw checker_error("Unexpected message port event: neigh_localnet_msg called but message ports are not supported");
}

void cosim::neigh_fill_ff_msg(unsigned int shire_id, unsigned int neigh_id, bool is_localnet, bool is_tbox,
                              unsigned int minion_rcv, unsigned int thread_id)
{
    (void)shire_id; (void)neigh_id; (void)is_localnet; (void)is_tbox;
    (void)minion_rcv; (void)thread_id;
    throw checker_error("Unexpected message port event: neigh_fill_ff_msg called but message ports are not supported");
}

//////////////////////////////////////////
// Msg tracking - Minion
/////////////////////////////////////////

void cosim::minion_new_msg(unsigned int shire_id, unsigned int shire_dest, unsigned int hart_id_src,
                           unsigned int hart_id_dest, unsigned int port_id, bool to_localnet)
{
    (void)shire_id; (void)shire_dest; (void)hart_id_src;
    (void)hart_id_dest; (void)port_id; (void)to_localnet;
    throw checker_error("Unexpected message port event: minion_new_msg called but message ports are not supported");
}

void cosim::tbox_new_req(unsigned int shire_id, unsigned int hart_id_dest, unsigned int port_id)
{
    (void)shire_id; (void)hart_id_dest; (void)port_id;
    throw checker_error("Unexpected TBOX event: tbox_new_req called but TBOX is not supported");
}

// Function form_reduce pair checks whether there is a reduce pair ready to commit.
// The input parameter is the sender minion. It returns the receiver minion or -1 if no pair is ready to commit.
// It is called by reduce_done whenever reduce is completed and by check_ready_instructions to drain any reduce pairs
// that have not committed.
int cosim::form_reduce_pair(uint64_t cycle, unsigned minion_sender)
{
    constexpr const char* tag = reduce_done_t::tag();
    auto it = pending_queue[minion_sender].begin();
    bool partner_ready = false;

    if ((it != pending_queue[minion_sender].end()) && it->reduce && it->reduce_action == REDUCE_SEND_ACTION) {
        logger->debug(LOG_PREFIX "Trying to form reduce pair", cycle, it->hart, it->desc, tag);
        if (it->reduce_partner_done) {
            auto partner_it = pending_queue[it->reduce_partner].begin();
            partner_ready = partner_it->reduce && partner_it->reduce_action == REDUCE_RECV_ACTION
                            && partner_it->reduce_partner == minion_sender;
            logger->debug(LOG_PREFIX "Reduce paired: send={} recv={}", cycle, it->hart, it->desc, tag, minion_sender,
                          it->reduce_partner);
            if (partner_ready) {
                logger->debug(LOG_PREFIX "Partner ready: {}", cycle, it->hart, it->desc, tag, partner_it->desc);
                return it->reduce_partner;
            } else {
                logger->debug(LOG_PREFIX "Partner not ready. Found: {}", cycle, it->hart, it->desc, tag,
                              partner_it->desc);
            }
        } else {
            logger->debug(LOG_PREFIX "Reduce not paired: Waiting for receiver", cycle, it->hart, it->desc, tag);
        }
    }
    return -1;
}

void cosim::check_ready_instructions(uint64_t cycle, int minion_id, bool commit_reduce_pair) try
{
    if (!checker_is_on()) return;

    constexpr const char* tag = "CHK";
    std::queue<uint32_t> wake_minions;
    auto it = pending_queue[minion_id].begin();

    // If it is called to commit a reduce pair, clear the reduce flag at the top of the queue
    if (commit_reduce_pair) {
        assert(!pending_queue[minion_id].empty());
        if (!it->reduce) {
            logger->error(LOG_PREFIX "Trying to commit reduce pair, but head of queue is not a reduce", cycle, it->hart,
                          it->desc, tag);
            throw checker_error("Trying to commit reduce pair, but head of queue is not a reduce");
        }
        it->reduce = false;
    }

    int swap = 0; // Flag to check if T0 and T1 are both stuck waiting for FCC credits

    // while the head of the pending queue is ready
    while (!pending_queue[minion_id].empty() && pending_queue[minion_id].front().is_ready()) {
        // The iterator can get invalidated throughout this loop, so we reset it to the first item item in the queue
        it = pending_queue[minion_id].begin();

        switch (it->type_id) {
        case cosim_insn::type::INSTRUCTION:
        case cosim_insn::type::EXCEPTION: {
            // Instruction is FLB
            if (it->is_flb) {
                // FLB did not return form UC Block yet
                if (it->flb_return_val == -1) return;

                // Look for Harts in the Pending FLB queue
                auto& flb_q = pending_flb[it->hart.shire_id()][it->flb_idx];
                auto flb_entry = std::find(flb_q.begin(), flb_q.end(), it->hart.u32());
                if (flb_entry == flb_q.end()) {
                    logger->error(LOG_PREFIX "FLB[{}]: Unable to find entry", cycle, it->hart, it->desc, tag,
                                  it->flb_idx);
                    throw checker_error("Error in FLB queue");
                }
                if (it->flb_return_val == 0) { // Not last in barrier
                    flb_q.erase(flb_entry);
                    logger->debug(LOG_PREFIX "FLB[{}]: Remove entry", cycle, it->hart, it->desc, tag, it->flb_idx);

                    // Wake up last in barrier
                    if (it->flb_min_to_wakeup != -1) {
                        wake_minions.push(it->flb_min_to_wakeup);
                        logger->debug(LOG_PREFIX "FLB[{}]: Wakeup minion {}", cycle, it->hart, it->desc, tag,
                                      it->flb_idx, it->flb_min_to_wakeup);
                    }
                } else {                              // Last in barrier => Wait for others
                    if (flb_entry == flb_q.begin()) { // Head of the FLB queue
                        flb_q.pop_front();
                        logger->debug(LOG_PREFIX "FLB[{}]: Remove entry (last)", cycle, it->hart, it->desc, tag,
                                      it->flb_idx);
                    } else { // Not HOQ - So signal the previous Minion to wake me up
                        auto flb_prev = std::prev(flb_entry, 1); // flb_entry - 1;
                        auto& prev_pending = pending_queue[*flb_prev >> 1];
                        auto inst_flb_prev
                            = std::find_if(prev_pending.begin(), prev_pending.end(), [&](const cosim_insn& insn) {
                                return insn.is_flb && insn.hart == static_cast<uint32_t>(*flb_prev);
                            });
                        if (inst_flb_prev == prev_pending.end()) {
                            logger->error(LOG_PREFIX "FLB[{}]: No outstanding insns for hart {}", cycle, it->hart,
                                          it->desc, tag, it->flb_idx, *flb_prev);
                            throw checker_error("Error in FLB queue");
                        }
                        inst_flb_prev->flb_min_to_wakeup = it->hart.minion_id();
                        logger->debug(LOG_PREFIX "FLB[{}]: Waiting for hart {}", cycle, it->hart, it->desc, tag,
                                      it->flb_idx, *flb_prev);
                        unblock_queue(cycle, it->hart.minion_id());
                        return;
                    }
                }
            }
        } break;
        case cosim_insn::type::INTERRUPT: {
            const auto& changes = it->state_change;
            logger->debug(LOG_PREFIX "Forcing BEMU Interrupt", cycle, it->hart, it->desc, tag);
            logger->debug(LOG_PREFIX "  Cause : {}", cycle, it->hart, it->desc, tag, changes.cause);
            logger->debug(LOG_PREFIX "  MIP : {:#x}", cycle, it->hart, it->desc, tag, changes.mip);
            if ((it->state_change.cause & 0x1f) == 0x17) {
                logger->debug(LOG_PREFIX "  Bus Error Addr : {:#010x}", cycle, it->hart, it->desc, tag,
                              changes.bus_err_addr);
            }
            check->raise_interrupt(it->hart.u32(), it->state_change.cause, it->state_change.mip,
                                   it->state_change.bus_err_addr);
            break;
        }
        case cosim_insn::type::PENDING_MSG: {
            if (check != nullptr) {
                if (it->rmt_msg_info.is_tbox) {
                    check->tbox_port_write(it->rmt_msg_info.hart_id_dest, it->rmt_msg_info.port_id,
                                           it->rmt_msg_info.hart_id_src);
                    it = erase_cosim_inst(minion_id, it);
                } else if (it->rmt_msg_info.is_rbox) {
                    check->rbox_port_write(it->rmt_msg_info.hart_id_dest, it->rmt_msg_info.port_id,
                                           it->rmt_msg_info.hart_id_src);
                    it = erase_cosim_inst(minion_id, it);
                }
            }
            continue;
        }
        case cosim_insn::type::IPI: {
            logger->debug("{} ns\t{}\tCommit IPI (PC: {:#010x})", cycle, it->hart, it->ipi_pc);
            check->ipi_pc(it->hart.u32(), it->ipi_pc);
            it = erase_cosim_inst(minion_id, it);
            continue;
        }
        }

        // Call emu instr even if it is an interrupt. That allow us to check partial results
        check->clearReduceReceive();
        if (it->reduce_action == REDUCE_RECV_ACTION) {
            check->setReduceReceive();
        }

        checker_result res = check->emu_inst(cycle, it, wake_minions);

        check->clearReduceReceive();
        if (res == CHECKER_ERROR) {
            const auto hart = it->hart;
            const auto desc = it->desc;
            const auto& changes = it->state_change;
            logger->debug(LOG_PREFIX "Check:  {} (enc: {:#x}) (+{} cycles)", cycle, hart, desc, tag,
                          riscv_disasm(changes.inst_bits), changes.inst_bits, cycle - changes.cycle);
            logger->error(LOG_PREFIX "Checker Error: {}", cycle, hart, desc, tag, check->get_error_msg());
            fail(hart.minion_id(), hart.thread_in_minion(), cycle);
            if (config_.max_errors != 0 && ((++num_errors) >= config_.max_errors)) {
                logger->warn(LOG_PREFIX "Maximum number of errors reached ({})", cycle, hart, desc, tag,
                             config_.max_errors);
                checker_bypassed = true;
            }
            it = erase_cosim_inst(minion_id, it);
            throw checker_error(check->get_error_msg());
        } else if (res == CHECKER_WARNING) {
            const auto hart = it->hart;
            const auto desc = it->desc;
            const auto& changes = it->state_change;

            logger->debug(LOG_PREFIX "Check:  {} (enc: {:#x}) (+{} cycles)", cycle, hart, desc, tag,
                          riscv_disasm(changes.inst_bits), changes.inst_bits, cycle - changes.cycle);
            logger->warn(LOG_PREFIX "Checker Warning: {}", cycle, it->hart, it->desc, tag, check->get_error_msg());
            it = erase_cosim_inst(minion_id, it);

            // In case that a thread was awaken, check instructions for it
            while (!wake_minions.empty()) {
                logger->debug(LOG_PREFIX "Waking up minion {}", cycle, hart, it->desc, tag, wake_minions.front());
                check_ready_instructions(cycle, wake_minions.front());
                wake_minions.pop();
            }
        } else if (res == CHECKER_EXCL_ACCESS) {
            logger->debug(LOG_PREFIX "Waiting for other thread to come out of Exclusive Mode", cycle, it->hart,
                          it->desc, tag);
            if (!unblock_queue(cycle, minion_id)) return;
            it = pending_queue[minion_id].begin();

            const auto hart = it->hart;
            const auto desc = it->desc;

            // In case that a thread was awaken, check instructions for it
            while (!wake_minions.empty()) {
                logger->debug(LOG_PREFIX "Waking up minion {}", cycle, hart, desc, tag, wake_minions.front());
                check_ready_instructions(cycle, wake_minions.front());
                wake_minions.pop();
            }
        } else if (res == CHECKER_WAIT) {
            const auto hart = it->hart;
            const auto desc = it->desc;
            const auto& changes = it->state_change;
            logger->debug(LOG_PREFIX "Delay({}):  {} (enc: {:#x}) (+{} cycles)", cycle, hart, desc, tag,
                          get_prvname(changes.prv), riscv_disasm(changes.inst_bits), changes.inst_bits,
                          cycle - changes.cycle);
            if (swap++ > 1) return; // This means both Threads are stuck in FCC waiting on credits
            if (!unblock_queue(cycle, minion_id)) return;
            it = pending_queue[minion_id].begin();

            // In case that a thread was awaken, check instructions for it
            while (!wake_minions.empty()) {
                logger->debug(LOG_PREFIX "Waking up minion {}", cycle, hart, desc, tag, wake_minions.front());
                check_ready_instructions(cycle, wake_minions.front());
                wake_minions.pop();
            }
        } else if (res == CHECKER_OK) {
            const auto hart = it->hart;
            const auto desc = it->desc;
            const auto& changes = it->state_change;
            logger->info(LOG_PREFIX "Commit({}): {} (enc: {:#x}) (+{} cycles)", cycle, hart, desc, tag,
                         get_prvname(changes.prv), riscv_disasm(changes.inst_bits), changes.inst_bits,
                         cycle - changes.cycle);
            it = erase_cosim_inst(minion_id, it);

            // In case that a thread was awaken, check instructions for it
            while (!wake_minions.empty()) {
                logger->debug(LOG_PREFIX "Waking up minion {}", cycle, hart, desc, tag, wake_minions.front());
                check_ready_instructions(cycle, wake_minions.front());
                wake_minions.pop();
            }
        } else {
            logger->error(LOG_PREFIX "Unexpected Response from Checker: {}", cycle, it->hart, it->desc, tag,
                          check->get_error_msg());
            fail(it->hart.minion_id(), it->hart.thread_in_minion(), cycle);
            throw checker_error(check->get_error_msg());
        }
        // Do not commit anything after sender reduce so no changes happen to the RF before receiver reduce is checked
        if (commit_reduce_pair) return;
    }

    // If no more entries, just return
    if (pending_queue[minion_id].empty()) return;

    it = pending_queue[minion_id].begin();

    // We try to form reduce pairs here in case the test is done and we want to drain the pending queues.
    int reduce_partner_id = (it->reduce) ? form_reduce_pair(cycle, minion_id) : -1;
    if (reduce_partner_id >= 0) {
        check_ready_instructions(cycle, minion_id, true);
        check_ready_instructions(cycle, reduce_partner_id, true);
        check_ready_instructions(cycle, minion_id);
        check_ready_instructions(cycle, reduce_partner_id);
    }
}
catch (const checker_error& err)
{
    log << LOG_ERR << "Checker Error: " << err.what() << endm;
}

// Try to unblock pending_queue for minion <minion_id>
// When the instruction at the head of the queue is blocking, we search for the first instruction of the other thread,
// and -- if ready -- reorder the pending_queue for this minion accordingly.
bool cosim::unblock_queue(uint64_t cycle, unsigned minion_id)
{
    constexpr const char* tag = "UNB";
    // References to queue and head of queue
    auto& minion_queue = pending_queue[minion_id];
    assert(!minion_queue.empty());

    const auto& it_head = minion_queue.begin();

    // Get harts for this minion
    const hart_id hart_head = it_head->hart;
    const hart_id hart_other = hart_head.other();

    // Find first instruction of other thread
    const auto& it_other = std::find_if(minion_queue.begin(), minion_queue.end(),
                                        [hart_other](const cosim_insn& insn) { return insn.hart == hart_other; });
    if (it_other == minion_queue.end()) {
        logger->debug(LOG_PREFIX "Unable to unblock: No instructions pending for other thread", cycle, hart_head,
                      it_head->desc, tag);
        return false;
    }

    // Utility to check whether the partner of a reduce instruction is ready
    const auto partner_is_ready = [&](int partner_id) {
        // Check if the first instruction of T0 is a reduce and partners with <curr_min_id>
        // This allows us to unblock partnered tensor_reduce insns which are *both* blocked in the queue
        assert(partner_id != -1 && "invalid partner_id");
        const auto& partner_queue = pending_queue[partner_id];
        const auto partner_t0 = std::find_if(partner_queue.begin(), partner_queue.end(),
                                             [&](const cosim_insn& insn) { return insn.hart.thread_in_minion() == 0; });
        if (partner_t0 == partner_queue.end()) return false; // This shouldn't happen => change to assert?
        return partner_t0->reduce_partner == minion_id;
    };

    const bool reduce_is_ready = it_other->reduce && it_other->reduce_partner_done
                                 && partner_is_ready(it_other->reduce_partner);

    if (!(it_other->is_ready() || reduce_is_ready)) {
        logger->debug(LOG_PREFIX "Unable to unblock: Not ready", cycle, hart_other, it_other->desc, tag);
        return false;
    }

    logger->debug(LOG_PREFIX "Distance to {} is {}", cycle, hart_head, it_head->desc, tag, it_other->desc,
                  std::distance(it_head, it_other));

    // Alternative implementation: Move consecutive block of instructions of same hart
    // const auto& it_head_next = std::find_if(it_other + 1, minion_queue.end(),
    //                                         [hart_head](const cosim_insn& insn) { return insn.hart == hart_head; });
    // std::rotate(it_head, it_other, it_head_next);

    // Reorder pending_queue
    std::rotate(it_head, it_other, it_other + 1);

#ifdef DEBUG // Disabled by default since this gets too verbose very easily
    logger->debug("{} ns\tNew order of pending instruction queue for minion {} (size: {})", cycle, minion_id,
                  minion_queue.size());
    for (const auto& it : minion_queue) {
        logger->debug(LOG_PREFIX, cycle, it.hart, it->desc);
    }
#endif

    logger->debug(LOG_PREFIX "Thread unblocked", cycle, hart_other, minion_queue.front().desc, tag);
    return true;
}

std::ostream& operator<<(std::ostream& os, const cosim_insn& insn)
{
    // Handle special "instruction" types
    if (insn.type_id == cosim_insn::type::IPI) {
        fmt::print(os, "{}\tIPI (PC: {:#010x})", insn.hart, insn.ipi_pc);
        return os;
    }

    if (insn.type_id == cosim_insn::type::PENDING_MSG) {
        const auto& msg = insn.rmt_msg_info;
        fmt::print(os, "{}\tPending message from {} to {}", insn.hart, msg.shire_org, msg.shire_dest);
        return os;
    }

    // Normal instruction otherwise..
    const auto& enc = insn.state_change.inst_bits;
    fmt::print(os, "{}\t{}\t{} (enc:{})", insn.hart, insn.desc, riscv_disasm(enc), enc);
    if (insn.type_id == cosim_insn::type::EXCEPTION) fmt::print(os, " exception");
    if (insn.type_id == cosim_insn::type::INTERRUPT) fmt::print(os, " interrupt");
    if (insn.delayed_write) {
        fmt::print(os, " delayed_write(x{})", insn.delayed_addr);
        if (insn.load_miss) fmt::print(os, " load_miss");
    }
    if (insn.delayed_store) fmt::print(os, " delayed_store(age={})", insn.store_age_id);
    if (insn.tensor_load) {
        fmt::print(os, " tensor_load({}num_lines={})", insn.tenb ? "tenb, " : "", insn.tensor_load_num_lines);
    }
    if (insn.tensor_fma) fmt::print(os, " tensor_fma");
    if (insn.tensor_quant) fmt::print(os, "tensor_quant");
    if (insn.reduce) {
        fmt::print(os, " tensor_reduce(action={}, partner={})", insn.reduce_action, insn.reduce_partner);
    }
    if (insn.is_flb) {
        fmt::print(os, " flb(index={})", insn.flb_idx);
        if (insn.flb_min_to_wakeup != -1) fmt::print(os, " waiting_for(minion={})", insn.flb_min_to_wakeup);
    }
    return os;
}

// Dumps the queue of pending instructions
void cosim::dump_queue()
{
    const auto log_level = check->is_enabled() ? spdlog::level::err : spdlog::level::warn;

    for (int minion_id = 0; minion_id < EMU_NUM_MINIONS; ++minion_id) {
        const auto& minion_queue = pending_queue[minion_id];
        if (minion_queue.empty()) continue;

        logger->log(log_level, "Minion({})\tPending instruction queue not empty", minion_id);
        for (const auto& insn : minion_queue) {
            logger->log(log_level, "Minion({})\t{}", minion_id, insn);
        }
    }

    for (int shire_id = 0; shire_id < EMU_NUM_SHIRES; ++shire_id) {
        for (int flb_id = 0; flb_id < 32; ++flb_id) {
            const auto& harts_in_flb = pending_flb[shire_id][flb_id];
            if (!harts_in_flb.empty()) {
                logger->log(log_level, "Pending FLB queue is not empty for Shire: {} Barrier: {} Harts: [{}]", shire_id,
                            flb_id, fmt::join(harts_in_flb, ", "));
            }
        }
    }
}

// Returns if the checker is empty
bool cosim::is_empty()
{
    bool ret = true;

    // Instructions check
    for (int id = 0; id < EMU_NUM_MINIONS; id++) {
        if (!pending_queue[id].empty()) {
            ret = false;
        }
    }

    if (!ret && testMain::cleanEnd()) dump_queue();
    return ret;
}

// Repurpose IO Shire ID to Shire 33
constexpr unsigned cosim::fixup_ioshire_id(unsigned minion_id)
{
#if EMU_HAS_SPIO
    return ((minion_id >> 5) & 0xff) == IO_SHIRE_ID ? (EMU_IO_SHIRE_SP << 5) | (minion_id & 0x1f) : minion_id;
#else
    return minion_id;
#endif
}

// Discard the cosim instruction
cosim_insn_queue_it_t cosim::erase_cosim_inst(int id, cosim_insn_queue_it_t& it)
{
    // free the mem allocated
    delete it->tensor_load_entry_list;
    return (pending_queue[id].erase(it));
    // log<<LOG_DEBUG<<"Deleting entry from Pending Queue [" << id << "] Minion " << it->minion_id() << ", thread " <<
    // it->thread_id() << ". New size of instruction queue is "<< pending_queue[id].size() << endm;
}

void cosim::set_verbosity(int bemu_log_level)
{
    if (pInstance && pInstance->check) {
        pInstance->check->system().log.setLogLevel((logLevel)bemu_log_level);
    }
    testLog::setGlobalLogLevel((logLevel)bemu_log_level);
    spdlog::set_level(cosim_config::get_level_enum((logLevel)bemu_log_level));
}

// set the log verbosity for a given window to emu_log_level, 4 elsewhere
void cosim::set_log_level_4_window(uint64_t cycle)
{

    if (config_.checker.log_window[1] == 0 || config_.checker.log_window[0] >= config_.checker.log_window[1]) return;

    if (cycle < config_.checker.log_window[0]) {
        if (window_log_level != 4) {
            window_log_level = 4;
            set_verbosity(window_log_level);
        }
    } else if (cycle < config_.checker.log_window[1]) {
        if (window_log_level != config_.checker.log_level) {
            window_log_level = config_.checker.log_level;
            set_verbosity(window_log_level);
        }
    } else if (window_log_level != 4) {
        window_log_level = 4;
        set_verbosity(window_log_level);
    }
}

// Dump statistics to a file - for each minion
void cosim::dump_stats_2_file(std::string of_name)
{
    std::string s0, s1;
    int num_active_treads = 0, used_shire_id = -1;
    std::ofstream ofile;

    ofile.open(of_name);
    ofile << "\n##############################################################";
    ofile << "\n### dumping instructions statistics for each active minion ###";
    ofile << "\n##############################################################\n\n\n";

    for (unsigned i = 0; i < EMU_NUM_MINIONS; i++) {

        if (i % 32 == 0) {
            if (used_shire_id != -1) {
                if (s1 != "") s1 += ", ";
                s1 += std::to_string(used_shire_id);
            }
            used_shire_id = -1;
        }

        if (stat_minion[i].num_instr == 0) continue;
        used_shire_id = (i >> 5);

        s0 = "minion " + std::to_string(i) + " :\n" + "  num_instr                       = "
             + std::to_string(stat_minion[i].num_instr) + "\n" + "  num_instr_thread[0]             = "
             + std::to_string(stat_minion[i].num_instr_thread[0]) + "\n" + "  num_instr_thread[1]             = "
             + std::to_string(stat_minion[i].num_instr_thread[1]) + "\n" + "  num_ipi                         = "
             + std::to_string(stat_minion[i].num_ipi) + "\n" + "  num_fma                         = "
             + std::to_string(stat_minion[i].num_fma) + "\n" + "  num_fma_transactions            = "
             + std::to_string(stat_minion[i].num_fma_transactions) + "\n" + "  num_tensor_load[0]              = "
             + std::to_string(stat_minion[i].num_tensor_load[0]) + "\n" + "  num_tensor_load_transactions[0] = "
             + std::to_string(stat_minion[i].num_tensor_load_transactions[0]) + "\n"
             + "  num_tensor_load[1]              = " + std::to_string(stat_minion[i].num_tensor_load[1]) + "\n"
             + "  num_tensor_load_transactions[1] = " + std::to_string(stat_minion[i].num_tensor_load_transactions[1])
             + "\n" + "  num_tensor_store                = " + std::to_string(stat_minion[i].num_tensor_store) + "\n"
             + "  num_tensor_store_transactions   = " + std::to_string(stat_minion[i].num_tensor_store_transactions)
             + "\n" + "  num_tensor_reduce               = " + std::to_string(stat_minion[i].num_tensor_reduce) + "\n"
             + "  num_tensor_reduce_transactions  = " + std::to_string(stat_minion[i].num_tensor_reduce_transactions)
             + "\n" + "  num_tensor_quant                = " + std::to_string(stat_minion[i].num_tensor_quant) + "\n"
             + "  num_tensor_quant_transactions   = " + std::to_string(stat_minion[i].num_tensor_quant_transactions)
             + "\n";
        //"  num_cache_prefetch  = " + std::to_string(stat_minion[i].num_cache_prefetch)  + "\n" +

        num_active_treads += ((stat_minion[i].num_instr_thread[0] != 0) + (stat_minion[i].num_instr_thread[1] != 0));
        ofile << s0 << "\n";

    } // for

    s0 = "\n\nnum-active-threads = " + std::to_string(num_active_treads) + "\n" + "active-shires      = ( " + s1 + " )";
    ofile << s0 << "\n";

    ofile.close();
}

// Warning if the pending_queue gets too big
void cosim::monitor_num_instr(uint64_t cycle, unsigned minion_id)
{
    // Negative values mean we do not monitor pending instructions
    if (config_.sample_pending <= 0) return;

    const auto& minion_queue = pending_queue[minion_id];
    const auto qsize = minion_queue.size();
    auto& current_max = max_num_pending_instr_reached[minion_id];
    if (qsize <= current_max) return;

    // We found a new maximum length
    current_max = qsize;

    // Sample every <sample_pending> instructions
    if (qsize % config_.sample_pending != 0) return;
    const auto& hoq = minion_queue.front();

    logger->debug(LOG_PREFIX "Current size of queue: {}", cycle, hoq.hart, hoq.desc, "PEND", qsize);
    logger->debug(LOG_PREFIX "{}", cycle, hoq.hart, hoq.desc, "PEND", hoq);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Tensor Load - high level calls
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// tl_mon_op_start()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_op_start_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);

    const auto min = minion_id;
    const auto i = evt->tl_module;
    const auto j = tl_csr_struct_next_id[minion_id][evt->tl_module]++;
    const auto k = ++tl_next_op_id[minion_id];

    const auto transform = (evt->tl_module == 1) ? 0 : evt->transform; // transform : tl0 only

    cosim_insn_queue_it_t tl_op_inst_it = tl_get_retired_inst(evt->cycle, min, evt->tl_module, 0);

    tl_op_inst_it->tensor_load_op_id = k;
    tl_op[min][i][j].op_id = k;
    tl_op[min][i][j].state = 1;
    tl_op[min][i][j].is_transform = (transform != 0);
    tl_op[min][i][j].desc = tl_op_inst_it->desc;

    // reporting
    const hart_id hart(minion_id, 0);
    logger->info(LOG_PREFIX "tl{} start (op_id={}, num_lines={}, trans={})", evt->cycle, hart, tl_op_inst_it->desc,
                 evt->tag(), evt->tl_module, k, evt->num_lines + 1, transform);

    if (tl_csr_struct_next_id[min][evt->tl_module] == 4) tl_csr_struct_next_id[min][evt->tl_module] = 0;
    tl_num_op_running[min][i]++;

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "start", evt->cycle, hart, tl_op_inst_it->desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_mon_process_busy()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_process_busy_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const auto min = minion_id;
    const auto i = evt->tl_module;
    const auto j = tl_get_op_struct_i_4_state(evt->cycle, min, i, evt->state, false, -1);

    if (j == -1) return;

    const hart_id hart(minion_id, 0);
    const auto op_id = tl_op[min][i][j].op_id;
    const auto& desc = tl_op[min][i][j].desc;
    logger->debug(LOG_PREFIX "tl{} process-busy (op_id={}, state={})", evt->cycle, hart, desc, evt->tag(),
                  evt->tl_module, op_id, evt->state);

    tl_op[min][i][j].state++;

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "process-busy", evt->cycle, hart, desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }

    tl_check_4_done_ops(evt->cycle, min);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_mon_process_reqs()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_process_req_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const auto min = minion_id;
    const auto i = evt->tl_module;
    const auto j = tl_get_op_struct_i_4_state(evt->cycle, min, i, 2, true, -1);

    const hart_id hart(minion_id, 0);
    const auto& desc = tl_op[min][i][j].desc;
    const auto op_id = tl_op[min][i][j].op_id;

    if (j == -1) {
        logger->error(LOG_PREFIX "tl{} process-req (op_id={}, req_id={})"
                                 "- Checker Error!! - can't find a busy tl_op for l2 req",
                      evt->cycle, hart, desc, evt->tag(), evt->tl_module, op_id, evt->req_id_idx);
        throw checker_error("can't find a busy tl_op for l2 reg");
    }

    if (tl_op[min][i][j].req_state[evt->req_id_idx] != 0) {
        logger->error(LOG_PREFIX "tl{} process-req (op_id={}, req_id={})"
                                 " - Checker Error!! - new l2 req but outstanding l2 req wasn't completed",
                      evt->cycle, hart, desc, evt->tag(), evt->tl_module, op_id, evt->req_id_idx);
        throw checker_error("new l2 req but outstanding l2 req wasn't completed");
    }

    logger->debug(LOG_PREFIX "tl{} process-req (op_id={}, req_id={})", evt->cycle, hart, desc, evt->tag(),
                  evt->tl_module, op_id, evt->req_id_idx);

    tl_op[min][i][j].req_state[evt->req_id_idx] = 2;
    tl_op[min][i][j].num_req += 2;

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "process-req", evt->cycle, hart, desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }
}
catch (const checker_error& err)
{
    log << LOG_ERR << "Checker Error: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_mon_process_resp()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_process_resp_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const auto min = minion_id;
    const auto i = evt->tl_module;
    const auto j = tl_get_op_struct_i_4_resp(evt->cycle, min, i, evt->resp_id_idx);

    if (j == -1) return;

    const hart_id hart(minion_id, 0);
    const auto& desc = tl_op[min][i][j].desc;
    const auto op_id = tl_op[min][i][j].op_id;
    logger->debug(LOG_PREFIX "tl{} process-resp (op_id={}, resp_id={})", evt->cycle, hart, desc, evt->tag(),
                  evt->tl_module, op_id, evt->resp_id_idx);

    tl_op[min][i][j].req_state[evt->resp_id_idx]--;
    tl_op[min][i][j].num_resp++;

    // data transfer capture - tensor load 0 non-transform, tensor load 1
    if (tl_op[min][i][j].is_transform == false) tl_data_xfer_done(evt->cycle, min, i, j, evt->entry, 0, evt->data);

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "process-resp", evt->cycle, hart, desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_mon_process_trans_data_xfer()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_process_trans_data_xfer_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const auto min = minion_id;
    const auto j = tl_get_op_struct_i_4_state(evt->cycle, min, 0, 2, true, 1);

    const hart_id hart(minion_id, 0);

    if (j == -1) {
        logger->error(LOG_PREFIX "tl0 data-xfer"
                                 " - Checker Error!! - can't find a started tl_op for transform ",
                      evt->cycle, hart, insn_desc(hart), evt->tag());
        throw checker_error("can't find a started tl_op for transform");
    }

    tl_data_xfer_done(evt->cycle, min, 0, j, evt->entry, evt->banks, evt->data);

    const auto op_id = tl_op[min][0][j].op_id;
    const auto& desc = tl_op[min][0][j].desc;
    logger->debug(LOG_PREFIX "tl0 data-xfer (op_id={})", evt->cycle, hart, desc, evt->tag(), op_id);

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "data-xfer", evt->cycle, hart, desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }
}
catch (const checker_error& err)
{
    log << LOG_ERR << "Checker Error: " << err.what() << endm;
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_mon_process_tenb_flush()
////////////////////////////////////////////////////////////////////////////////
void cosim::on(const tl_mon_process_tenb_flush_t* evt) try
{
    if (!checker_is_on()) return;
    const auto minion_id = fixup_ioshire_id(evt->minion_id);
    const auto min = minion_id;
    int i = 1, j, first_trans_op_id = -1, first_trans_op_id_struct_idx = -1;

    const hart_id hart(minion_id, 0);

    // find the struct index
    for (j = 0; j < 4; j++) {

        if (tl_op[min][i][j].state == 0) continue;

        if (first_trans_op_id == -1 || (unsigned)first_trans_op_id > tl_op[min][i][j].op_id) {
            first_trans_op_id = tl_op[min][i][j].op_id;
            first_trans_op_id_struct_idx = j;
        }
    }

    j = first_trans_op_id_struct_idx;
    if (j == -1) return;

    const auto& desc = tl_op[min][i][j].desc;
    logger->debug(LOG_PREFIX "tl1 process-tenb-flush", evt->cycle, hart, desc, evt->tag());

    tl_op[min][i][j].tenb_flush = true;

    if (config_.tload_debug) {
        const auto prefix = fmt::format(LOG_PREFIX "process-tenb-flush", evt->cycle, hart, desc, evt->tag());
        tl_log_data_struct(prefix, minion_id);
    }

    tl_check_4_done_ops(evt->cycle, min);
}
catch (const std::exception& ex)
{
    log << LOG_ERR << "Exception: " << ex.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Tensor Load - utilities
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// tl_write_data()
////////////////////////////////////////////////////////////////////////////////
void cosim::tl_write_data(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx, uint32_t entry,
                          uint32_t banks, uint32_t dw_i, const uint64_t* data) try
{
    constexpr const char* tag = tl_mon_op_start_t::tag();
    int min = minion_id;
    int i = tl_module;
    int j = struct_idx;
    int tl_op_id = tl_op[min][i][j].op_id;
    std::ostringstream s0, s1;
    cosim_tensor_load_entry tl_entry;

    for (int i = 3; i >= 0; i--) s1 << std::setw(16) << std::setfill('0') << std::hex << std::right << data[i];

    cosim_insn_queue_it_t tl_op_inst_it = tl_get_retired_inst(cycle, min, tl_module, tl_op_id);

    const hart_id hart(minion_id, 0);

    // reporting
    logger->debug(LOG_PREFIX
                  "tl{} write (op_id={}, entry={}, banks={}, dw-i={}, data=0x{:016x}_{:016x}_{:016x}_{:016x}, n={})",
                  cycle, hart, tl_op_inst_it->desc, tag, tl_module, tl_op_id, entry, banks, dw_i, data[3], data[2],
                  data[1], data[0], tl_op_inst_it->tensor_load_num_hword);

    if (tl_op_inst_it->tensor_load_entry_list == NULL) {
        if (dw_i > 0) {
            logger->error(LOG_PREFIX "tl{} write (op_id={})"
                                     " - Checker Error!! - tl_op list is empty but not first word",
                          cycle, hart, tl_op_inst_it->desc, tag, tl_op_id);
            throw checker_error("tl_op list is empty but not first word");
        }
        tl_op_inst_it->tensor_load_entry_list = new std::list<cosim_tensor_load_entry>;
    }

    tl_op_inst_it->tensor_load_num_hword++;

    if (dw_i == 0) tl_op_inst_it->tensor_load_entry_list->push_back(tl_entry);

    cosim_tensor_load_entry& last_tl_entry = tl_op_inst_it->tensor_load_entry_list->back();
    last_tl_entry.entry = entry;
    last_tl_entry.banks = banks;
    for (int i = 0; i < 4; i++) last_tl_entry.data[i + (4 * dw_i)] = data[i];

    stat_minion[min].num_tensor_load_transactions[i]++;
}
catch (const checker_error& err)
{
    log << LOG_ERR << "Checker Error: " << err.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_get_retired_inst()
// Find the tensor_load instruction with the given op_id
////////////////////////////////////////////////////////////////////////////////
cosim_insn_queue_it_t cosim::tl_get_retired_inst(uint64_t cycle, uint32_t minion_id, uint32_t tl_module,
                                                 uint32_t tl_op_id)
{

    constexpr const char* tag = tl_mon_op_start_t::tag();
    auto& minion_queue = pending_queue[minion_id];
    const auto match_tload = [=](const cosim_insn& insn) {
        return insn.tensor_load && insn.tensor_load_op_id == tl_op_id && insn.tenb == tl_module;
    };

    auto it = std::find_if(minion_queue.begin(), minion_queue.end(), match_tload);
    if (it != minion_queue.end()) return it;

    const hart_id hart(minion_id, 0);
    const insn_desc desc(hart);

    logger->error(LOG_PREFIX "  tl{} look-up (op_id={})"
                             " - Checker Error!! - couldn't find matching retired instruction",
                  cycle, hart, desc, tag, tl_module, tl_op_id);
    throw checker_error("couldn't find matching retired instruction");

    return minion_queue.end();
}

////////////////////////////////////////////////////////////////////////////////
// tl_op_done()
////////////////////////////////////////////////////////////////////////////////
void cosim::tl_op_done(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx) try
{
    constexpr const char* tag = tl_mon_op_start_t::tag();
    const auto op = tl_op[minion_id][tl_module][struct_idx];
    auto inst = tl_get_retired_inst(cycle, minion_id, tl_module, op.op_id);

    const hart_id hart(minion_id, 0);
    const auto& desc = inst->desc;

    if (struct_idx == 1) {
        logger->info(LOG_PREFIX "tl{} done (op_id={}, tenb_flush={}, num_lines={})", cycle, hart, desc, tag, tl_module,
                     op.op_id, op.tenb_flush, inst->tensor_load_num_lines);
    } else {
        logger->info(LOG_PREFIX "tl{} done (op_id={}, num_lines={})", cycle, hart, desc, tag, tl_module, op.op_id,
                     inst->tensor_load_num_lines);
    }

    inst->tensor_load = false;
    inst->tensor_load_done = true;
    inst->tensor_load_tenb_flush = op.tenb_flush;

    check_ready_instructions(cycle, minion_id);

    const auto num_running = tl_num_op_running[minion_id][tl_module]--;

    if (num_running < 0) {
        logger->error(LOG_PREFIX "tl{} done"
                                 " - Checker Error!! - Number of running tensorload ops is less than zero",
                      cycle, hart, desc, tag, tl_module);
        throw checker_error("number of running tensorload ops is less than zero");
    }

    tl_op[minion_id][tl_module][struct_idx] = {};
}
catch (const checker_error& err)
{
    log << LOG_ERR << "Checker Error: " << err.what() << endm;
}

////////////////////////////////////////////////////////////////////////////////
// tl_get_op_struct_i_4_state()
////////////////////////////////////////////////////////////////////////////////
int cosim::tl_get_op_struct_i_4_state(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t state,
                                      bool is_unique, int is_trans)
{
    const hart_id hart(minion_id, 0);
    const insn_desc desc(hart);
    constexpr const char* tag = tl_mon_op_start_t::tag();

    const auto match_state
        = [=](const cosim_tl_op& op) { return op.state == state && (is_trans == -1 || op.is_transform == is_trans); };

    const auto& ops = tl_op[minion_id][tl_module];
    const auto it = std::find_if(ops.begin(), ops.end(), match_state);

    if (it == ops.end()) return -1; // Not found

    if (is_unique) { // Search for potential duplicates
        const auto dup = std::find_if(it + 1, ops.end(), match_state);
        if (dup != ops.end()) {
            logger->error(LOG_PREFIX "tl{} find-state (state={})"
                                     " - Checker Error!! - multiple tl_op with same state",
                          cycle, hart, desc, tag, tl_module, state);
            throw checker_error("multiple tl_op with same state");
        }
    }
    return it - ops.begin();
}

////////////////////////////////////////////////////////////////////////////////
// tl_get_op_struct_i_4_resp()
////////////////////////////////////////////////////////////////////////////////
int cosim::tl_get_op_struct_i_4_resp(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t resp_id_idx)
{
    const hart_id hart(minion_id, 0);
    const insn_desc desc(hart);
    constexpr const char* tag = tl_mon_op_start_t::tag();

    const auto match_resp = [=](const cosim_tl_op& op) {
        return (op.state == 2 || op.state == 3) && (op.req_state[resp_id_idx] == 1 || op.req_state[resp_id_idx] == 2);
    };

    const auto& ops = tl_op[minion_id][tl_module];
    const auto it = std::find_if(ops.begin(), ops.end(), match_resp);

    if (it == ops.end()) return -1; // Not found

    // Search for potential duplicate
    const auto dup = std::find_if(it + 1, ops.end(), match_resp);
    if (dup != ops.end()) {
        logger->error(LOG_PREFIX "tl{} find-resp (resp_id={})"
                                 " - Checker Error!! - multiple tl_op with resp_id",
                      cycle, hart, desc, tag, tl_module, resp_id_idx);
        throw checker_error("multiple tl_op with same resp_id");
    }

    return it - ops.begin();
}

////////////////////////////////////////////////////////////////////////////////
// tl_data_xfer_done()
////////////////////////////////////////////////////////////////////////////////
void cosim::tl_data_xfer_done(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx,
                              uint32_t entry, uint32_t banks, const uint64_t* data)
{
    auto& op = tl_op[minion_id][tl_module][struct_idx];
    const int dw_i = op.is_transform ? 0 : op.dw_i;
    if (op.dw_i == 0 && !op.is_transform) entry = 0;

    tl_write_data(cycle, minion_id, tl_module, struct_idx, entry, banks, dw_i, data);

    ++op.num_data_xfer;
    op.dw_i = (op.dw_i + 1) % 2;

    tl_check_4_done_ops(cycle, minion_id);
}

////////////////////////////////////////////////////////////////////////////////
// tl_check_4_done_ops()
////////////////////////////////////////////////////////////////////////////////
void cosim::tl_check_4_done_ops(uint64_t cycle, uint32_t minion_id)
{
    const hart_id hart(minion_id, 0);
    const insn_desc desc(hart);
    constexpr const char* tag = tl_mon_op_start_t::tag();

    int min = minion_id, op_done = 0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            int is_done = 0;
            if (i == 1 && tl_op[min][i][j].tenb_flush) is_done = 1; // tl1 only
            if (tl_op[min][i][j].state == 3 && tl_op[min][i][j].num_resp == tl_op[min][i][j].num_req) is_done = 1;
            if (is_done == 1) tl_op_done(cycle, min, i, j);
            op_done += is_done;
        }
    }

    if (config_.tload_debug && op_done > 0) {
        const auto prefix = fmt::format(LOG_PREFIX "check-done", cycle, hart, desc, tag);
        tl_log_data_struct(prefix, minion_id);
    }
}

////////////////////////////////////////////////////////////////////////////////
// tl_log_data_struct()
////////////////////////////////////////////////////////////////////////////////
void cosim::tl_log_data_struct(const std::string& prefix, uint32_t minion_id)
{
    const auto& minion_ops = tl_op[minion_id];
    for (size_t tl_module = 0; tl_module < minion_ops.size(); ++tl_module) {
        const auto& module_ops = minion_ops[tl_module];
        for (size_t struct_idx = 0; struct_idx < module_ops.size(); ++struct_idx) {
            const auto& op = module_ops[struct_idx];
            logger->debug("{}\ttl_op[{}][{}][{}] "
                          ": op_id={}"
                          ", state={}"
                          ", req_state=({})"
                          ", trans={}"
                          ", num_req={}"
                          ", num_resp={}"
                          ", num_data_xfer={}"
                          ", tenb_flush={}"
                          ", dw_i{}",
                          prefix, minion_id, tl_module, struct_idx, op.op_id, op.state, fmt::join(op.req_state, ", "),
                          op.is_transform, op.num_req, op.num_resp, op.num_data_xfer, op.tenb_flush, op.dw_i);
        }
    }
}
