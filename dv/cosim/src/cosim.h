/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _COSIM_H_
#define _COSIM_H_

#include "checker/checker.h"
#include "cosim_insn.h"
#include "cosim_config.h"
#include "events/cosim_evt.h"
#include "events/recorder.h"
#include "support/multi_dim_array.h"
#include "spdlog/spdlog.h"
#include "testBase.h"
#include <array>
#include <functional>
#include <memory>
#include <set>
#include <stdio.h>
#include <stdlib.h>

#define CONTROL_SHIRE_PHY 13
#define CONTROL_SHIRE_VIRT 32

#define REDUCE_SEND_ACTION 0
#define REDUCE_RECV_ACTION 1
#define REDUCE_NO_ACTION 2
#define REDUCE_MAX_PARTNER_ID 7
#define REDUCE_MAX_LEVEL 2

class cosim : public cosim_evt::handler, public testBase {
public:
    ~cosim();

    bemu::MainMemory* getMemory()
    {
        return check->memory();
    }
    checker* getChecker()
    {
        return check;
    }
    bool parseMemFile(const char* filename);
    unsigned get_inst_retire_count(unsigned minion_id, bool thread_id);

    bool is_empty();

    // Monitor interface
    void on(const cosim_evt::inst_retire_t* evt);
    void on(const cosim_evt::inst_exception_t* evt);
    void on(const cosim_evt::inst_interrupt_t* evt);
    void on(const cosim_evt::warm_reset_t* evt);
    void on(const cosim_evt::clear_interrupt_t* evt);
    void on(const cosim_evt::update_fcsr_flags_t* evt);
    void on(const cosim_evt::delayed_write_int_t* evt);
    void on(const cosim_evt::flb_update_t* evt);
    void on(const cosim_evt::load_miss_bus_err_t* evt);
    void on(const cosim_evt::delayed_store_t* evt);
    void on(const cosim_evt::delayed_write_rm_t* evt);
    void on(const cosim_evt::ipi_received_t* evt);
    void on(const cosim_evt::tensorfma_write_t* evt);
    void on(const cosim_evt::tensorfma_write8_t* evt);
    void on(const cosim_evt::tensorquant_write_t* evt);
    void on(const cosim_evt::reduce_write_t* evt);
    void on(const cosim_evt::tensorstore_write_t* evt);
    void on(const cosim_evt::tensorfma_done_t* evt);
    void on(const cosim_evt::tensorquant_done_t* evt);
    void on(const cosim_evt::reduce_done_t* evt);
    void on(const cosim_evt::tensorstore_done_t* evt);
    void on(const cosim_evt::vpu_rf_update_t* evt);
    void on(const cosim_evt::delayed_write_fp_t* evt);
    void on(const cosim_evt::esr_write_t* evt);
    void on(const cosim_evt::icache_prefetch_done_t* evt);

    /* void emu_inst(unsigned minion_id, bool thread_id); */
    void uc_new_msg_remote_req(unsigned int shire_id_dest, unsigned int shire_id_orign);
    void uc_new_msg_req(unsigned int shire_id, unsigned int shire_dest, unsigned int neigh_id, unsigned int minion_src);
    void sc_local_new_msg(unsigned int shire_id, unsigned int neigh_id, unsigned int minion_src, unsigned int n_bank);
    void sc_local_new_msg_rbox(unsigned int shire_id, unsigned int rbox_local_id, unsigned int n_bank,
                               unsigned int thread_dest, unsigned int port_id, unsigned int* data, unsigned int oob);
    void sc_to_neigh_new_msg(unsigned int shire_id, unsigned int neigh_dest, unsigned int origin, bool is_uc);
    void neigh_localnet_msg(unsigned int shire_id, unsigned int neigh_id, unsigned int minion_dest,
                            unsigned int minion_src);
    void neigh_fill_ff_msg(unsigned int shire_id, unsigned int neigh_id, bool is_localnet, bool is_tbox,
                           unsigned int minion_rcv, unsigned int thread_id);
    void minion_new_msg(unsigned int shire_id, unsigned int shire_dest, unsigned int hart_id_src,
                        unsigned int hart_id_dest, unsigned int port_id, bool to_localnet);
    void tbox_new_req(unsigned int shire_id, unsigned int hart_id_dest, unsigned int port_id);

    cosim_insn_queue_it_t erase_cosim_inst(int id, cosim_insn_queue_it_t& it);
    void set_log_level_4_window(uint64_t cycle);
    void dump_stats_2_file(std::string of_name);
    void monitor_num_instr(uint64_t cycle, unsigned minion_id);
    void final_report();

    // tensor loads - DPI
    void on(const cosim_evt::tl_mon_op_start_t* evt);
    void on(const cosim_evt::tl_mon_process_busy_t* evt);
    void on(const cosim_evt::tl_mon_process_req_t* evt);
    void on(const cosim_evt::tl_mon_process_resp_t* evt);
    void on(const cosim_evt::tl_mon_process_trans_data_xfer_t* evt);
    void on(const cosim_evt::tl_mon_process_tenb_flush_t* evt);

    // tensor loads - utilities
    void tl_write_data(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx, uint32_t entry,
                       uint32_t banks, uint32_t dw_i, const uint64_t* data);
    cosim_insn_queue_it_t tl_get_retired_inst(uint64_t cycle, uint32_t minion_id, uint32_t tl_op_id,
                                              uint32_t tl_module);
    void tl_check_4_done_ops(uint64_t cycle, uint32_t minion_id);
    void tl_op_done(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx);
    int tl_get_op_struct_i_4_state(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t state,
                                   bool is_unique = false, int is_trans = -1);
    int tl_get_op_struct_i_4_resp(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t resp_id_idx);
    void tl_data_xfer_done(uint64_t cycle, uint32_t minion_id, uint32_t tl_module, uint32_t struct_idx, uint32_t entry,
                           uint32_t banks, const uint64_t* data);
    void tl_log_data_struct(const std::string& prefix, uint32_t minion_id);

    // Static methods
    static cosim& getInstance();
    static void deleteInstance();
    static constexpr unsigned fixup_ioshire_id(unsigned minion_id);
    static void set_verbosity(int bemu_log_level);

    using exit_fn = std::function<void(unsigned, unsigned, int)>;

    void on_fail(exit_fn cb)
    {
        fail_cb = cb;
    }
    void on_max_commits(exit_fn cb)
    {
        max_commits_cb = cb;
    }

    const cosim_config& config() const
    {
        return config_;
    }

    bool checker_is_on() const
    {
        return check != nullptr && !checker_bypassed;
    }

private:
    cosim();

    static void default_exit_cb(unsigned minion, unsigned thread, int cycle)
    {
        (void)minion;
        (void)thread;
        (void)cycle;
    }

    exit_fn fail_cb = default_exit_cb;
    exit_fn max_commits_cb = default_exit_cb;

    void fail(unsigned minion, unsigned thread, int cycle)
    {
        fail_cb(minion, thread, cycle);
    }
    void max_commits(unsigned minion, unsigned thread, int cycle)
    {
        max_commits_cb(minion, thread, cycle);
    }
    static cosim* pInstance; // FUTURE Should refactor this
    static volatile bool saw_shutdown;

    template <typename T>
    using per_minion_t = std::array<T, EMU_NUM_MINIONS>;
    template <typename T>
    using per_minion_thread_t = multi_dim_array<T, EMU_NUM_MINIONS, EMU_THREADS_PER_MINION>;
    template <typename T>
    using per_shire_t = std::array<T, EMU_NUM_SHIRES>;
    template <typename T>
    using per_neigh_t = std::array<T, EMU_NUM_NEIGHS>;
    template <typename T>
    using per_thread_t = std::array<T, EMU_NUM_THREADS>;
    using pending_msg_list = std::list<cosim_pending_msg>;

    // Signal handler for SIGTERM
    static void shutdown(int signal);

    // NOTE : this should be a <list> rather than a vector
    per_minion_t<std::deque<cosim_insn> > pending_queue;

    multi_dim_array<uint32_t, EMU_NUM_MINIONS, EMU_THREADS_PER_SHIRE, 32, bemu::VLENW> vpu_rf; // mirror of the VPU RF

    void check_ready_instructions(uint64_t cycle, int minion_id, bool commit_reduce_pair = false);
    bool unblock_queue(uint64_t cycle, uint32_t minion_id);
    std::string format_insn(const cosim_insn& insn);
    void dump_queue();
    int form_reduce_pair(uint64_t cycle, unsigned minion_sender);

    // config parameters
    cosim_config config_;

    checker* check = nullptr;
    recorder* rec = nullptr;
    bool checker_bypassed = false;
    int retire_count = 0;
    int final_report_done = 0;
    per_minion_thread_t<unsigned> nrCommits_{};

    per_shire_t<std::array<std::list<int>, 32> > pending_flb; // List of Pending FLB total 32 entries/ SHIRE

    // tensor load
    per_minion_thread_t<std::array<cosim_tl_op, 4> > tl_op{};
    per_minion_t<uint32_t> tl_next_op_id{};
    per_minion_thread_t<uint32_t> tl_csr_struct_next_id{};
    per_minion_thread_t<int> tl_num_op_running{};

    // counters
    int window_log_level = -1;
    size_t num_errors = 0;

    // monitor pending_queue size
    per_minion_t<size_t> max_num_pending_instr_reached{};

    // Alternative logger implementation
    std::shared_ptr<spdlog::logger> logger;

    // statistics
    per_minion_t<cosim_minion_stat> stat_minion{};

    // Keep track of last tensor_ops and vpu_updates for logging
    per_thread_t<insn_desc> last_tensorfma;
    per_thread_t<insn_desc> last_tensorquant;
    per_thread_t<insn_desc> last_tensorstore;
    per_thread_t<insn_desc> last_reduce;
    multi_dim_array<insn_desc, EMU_NUM_THREADS, 32> last_vpu_update;

    // Should this be an enum class?
    struct ReduceOperation {
        enum values {
            fadd = 0, // 0000
            fmax = 2, // 0010
            fmin = 3, // 0011
            iadd = 4, // 0100
            imax = 6, // 0110
            imin = 7, // 0111
            fget = 8, // 1000
        };
        static bool is_valid(uint8_t op)
        {
            switch (op) {
            case fadd:
            case fmax:
            case fmin:
            case iadd:
            case imax:
            case imin:
            case fget:
                return true;
            default:
                return false;
            }
        }
    };
};
#endif
