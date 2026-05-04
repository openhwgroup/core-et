/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _CHECKER_
#define _CHECKER_

// STD
#include <iostream>
#include <list>
#include <queue>
#include <string>

// Local
#include "cosim_insn.h"
#include "insn.h"
#include "log.h"
#include "system.h"
#include "testLog.h"

// Checker results
typedef enum {
    CHECKER_OK = 0,
    CHECKER_ERROR = 1,
    CHECKER_WAIT = 2,
    CHECKER_WARNING = 3,
    CHECKER_EXCL_ACCESS = 4
} checker_result;

typedef struct {
    int entry;
    uint32_t wmask;
    std::array<uint32_t, bemu::VLENW> data;
} tensorfma_entry;

// In theory there may be many reduces / tensor stores done and waiting to commit
// after an earlier instruction but these values seem a good tradeoff. I have not seen anything mre than 4 here...
#define MAX_REDUCES 80

typedef struct {
    uint64_t addr;
    uint32_t data;
} tensorstore_entry;

typedef struct {
    uint64_t base;
    uint64_t top;
} ignored_mem_region;

typedef struct {
    uint64_t match;
    uint64_t mask;
} ignored_mem_mask;

class checker {
public:
    using tfma_entry_list = std::deque<tensorfma_entry>;

    // Constructor and destructor
    checker(bool checker_en);
    ~checker();

    // Sets the PC
    void start_pc(uint32_t thread, uint64_t pc);
    void ipi_pc(uint32_t thread, uint64_t pc);
    // Emulates current instruction and compares the state changes
    checker_result emu_inst(uint64_t cycle, cosim_insn_queue_it_t cosim_inst, std::queue<uint32_t>& wake_minions);
    // Decoder Helper functions
    static void emu_disasm(char* str, size_t size, uint32_t bits);
    // Function to CoSIM to tell BEMU an Interrupt was taken by HW
    void raise_interrupt(unsigned minionId, int cause, uint64_t mip, uint64_t bus_err_addr);
    // Gets an error in string format
    std::string get_error_msg();

    bemu::MainMemory* memory()
    {
        return &system_.memory;
    }

    bemu::System& system() { return system_; }

    // enable or disable 2nd thread
    void thread1_enabled(unsigned minionId, uint64_t en, uint64_t pc);

    // Tensor operations
    void tensorfma_write(uint32_t thread, int entry, uint32_t wmask, const uint32_t* data);
    void tensorquant_write(uint32_t thread, uint32_t entry, uint32_t tensorquant_regfile_wmask, const uint32_t* data);
    void reduce_write(uint32_t thread, uint32_t entry, const uint32_t* data);
    void tensorstore_write(uint32_t thread, uint64_t addr, uint32_t data);
    void aggregate_tl_data(cosim_insn_queue_it_t cosim_inst);

    void update_fcsr_flags(unsigned minionId, unsigned flags);

    // Register a region to ignore checks on
    void add_ignored_mem_region(uint64_t base, uint64_t top);
    void add_ignored_mem_mask(uint64_t match, uint64_t mask);
    bool address_is_in_ignored_region(uint64_t addr);

    // TBOX write to Minion Message Port
    void thread_port_write(uint32_t target_thread, uint32_t port_id, uint32_t source_thread);
    void tbox_port_write(uint32_t target_thread, uint32_t port_id, uint32_t tbox_id);
    void rbox_port_write(uint32_t target_thread, uint32_t port_id, uint32_t rbox_id);

    // ICache Prefetch done
    void set_icache_prefetch_done(uint32_t shire_id);

    // ESR Write
    void esr_write(uint64_t addr, uint64_t data);

    // Is this checker enabled or disabled
    bool is_enabled()
    {
        return checker_enabled;
    }

    // printing utility
    std::string get_inst_info(uint64_t cycle, uint32_t thread);

    void advanceReduceListWriteIdx(uint64_t thread)
    {
        reduce_list_write_idx[thread] = (reduce_list_write_idx[thread] + 1) % MAX_REDUCES;
    }
    void advanceReduceListReadIdx(uint64_t thread)
    {
        reduce_list_read_idx[thread] = (reduce_list_read_idx[thread] + 1) % MAX_REDUCES;
    }
    void createTensorStoreList(uint64_t thread)
    {
        std::list<tensorstore_entry> new_list;
        tensorstore_list[thread].push_back(new_list);
    }
    void setReduceReceive()
    {
        reduce_is_receive = true;
    }
    void clearReduceReceive()
    {
        reduce_is_receive = false;
    }

    size_t tensorfma_size(uint32_t thread) const
    {
        return tensorfma_list[thread].size();
    }

    size_t tensorquant_size(uint32_t thread) const
    {
        return tensorquant_list[thread].size();
    }

    size_t tensorstore_size(uint32_t thread) const
    {
        return tensorstore_list[thread].empty() ? 0 : tensorstore_list[thread].front().size();
    }

    size_t reduce_size(uint32_t thread) const
    {
        return reduce_list[thread][reduce_list_read_idx[thread]].size();
    }

private:
    bemu::System system_;

    checker_result check_state_changes(uint64_t cycle, uint32_t thread, base_state_change* changes,
                                       const bemu::Instruction& inst, cosim_insn_queue_it_t cosim_inst);

    template <typename T>
    using per_thread_t = std::array<T, EMU_NUM_THREADS>;

    inst_state_change emu_state_change;             // Struct that holds the state change for the emu
    std::string error_msg;                          // Stores the error message
    testLog& log;                                   // Logger
    per_thread_t<uint64_t> threadEnabled;           // thread is enabled / disabled
    per_thread_t<tfma_entry_list> tensorfma_list;   // List of RTL written tensorfma entries
    per_thread_t<tfma_entry_list> tensorquant_list; // List of RTL written tensorquant entries
    per_thread_t<std::array<tfma_entry_list, MAX_REDUCES> > reduce_list; // List of RTL written reduce entries.
    // FUTURE emizan: Use 2D list like tensor stores

    per_thread_t<uint64_t> reduce_list_write_idx; // Write index for reduce list
    per_thread_t<uint64_t> reduce_list_read_idx;  // Read index for reduce list
    bool reduce_is_receive;                       // Is this reduce a receive?

    per_thread_t<std::list<std::list<tensorstore_entry> > > tensorstore_list; // List of RTL written tensor store
                                                                              // entries. 2D list because multiple
                                                                              // tensor stores may be waiting to
                                                                              // commit

    std::deque<uint16_t> waived_csrs;                   // List of CSRs whose checking is waived
    std::deque<uint64_t> waived_esrs;                   // List of ESRs whose checking is waived
    std::deque<ignored_mem_region> ignored_mem_regions; // List of memory regions whose data won't be checked
    std::deque<ignored_mem_mask> ignored_mem_masks; // List of masks for which a matching address would not be checked
    bool checker_enabled = false;                   // If false, this checkers ERRORS are downgraded to WARNINGS
};

#endif // _CHECKER_
