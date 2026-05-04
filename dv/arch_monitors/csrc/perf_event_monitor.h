/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PERF_EVENT_MONITOR_H_
#define _PERF_EVENT_MONITOR_H_

#include "testBase.h"
#include "checker.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

namespace n_Perf {

    // Minion States: Retirement, or one of 7 tensor states
    // RET means we are not in tensor state
    // TBD: Should we break RET into more detailed states (Branch, IC, Load stall, Fence) ?
    enum ThreadState {

	THREAD_STATE_RET = 0,
	THREAD_STATE_TL0 = 1,
        THREAD_STATE_TL1 = 2,
        THREAD_STATE_TL0_TL1 = 3,
	THREAD_STATE_TVPU = 4,
        THREAD_STATE_TVPU_TL0 = 5,
        THREAD_STATE_TVPU_TL1 = 6,
        THREAD_STATE_TVPU_TL0_TL1 = 7,
	THREAD_STATE_UNKNOWN = 8
    };

    // List of the possible TVPU states depending on what is in the VPU datapath.
    // TFMA can be either FMA16A32 or IMA8A32.
    // The combinations are for IMA8A32 that does not access the RF plus ops that access the RF.
    // They should be rare...
    enum TVPUState {
	TFMA = 1,
	TQUANT = 2,
	TREDUCE = 3,
	TSTORE = 4,
	TFMA_TQUANT = 5,
	TFMA_TREDUCE = 6,
	TFMA_STORE = 7,
	TNONE = 8
    };

   
    // List of events
    const uint64_t EVT_FMA_START = 0;
    const uint64_t EVT_FMA_FINISH = 1;
    const uint64_t EVT_TL0_START = 2;
    const uint64_t EVT_TL0_FINISH = 3;
    const uint64_t EVT_TL1_START = 4;
    const uint64_t EVT_TL1_FINISH = 5;
    const uint64_t EVT_TQUANT_START = 6;
    const uint64_t EVT_TQUANT_FINISH = 7;
    const uint64_t EVT_TSTORE_START = 8;
    const uint64_t EVT_TSTORE_FINISH = 9;
    const uint64_t EVT_VPU_START = 10;
    const uint64_t EVT_VPU_FINISH = 11;
    const uint64_t EVT_UNKNOWN = 12;
    const uint64_t EVT_MAX_EVENT = 13;

    const uint64_t master_shire_mask = 0x100000000;

};

typedef std::pair<n_Perf::ThreadState, uint64_t> ThreadStateEvent;
typedef std::pair<n_Perf::TVPUState, uint64_t> TVPUStateEvent;
typedef std::pair<ThreadStateEvent, n_Perf::ThreadState> ThreadTransition;

class Analyzer;
class ThreadAnalyzer;

// Instruction trace class
class TraceRecord
{
 private:
    uint64_t cycle;
    uint64_t pc;
 public:
    TraceRecord() {}
    TraceRecord(uint64_t c, uint64_t p):
    cycle(c), pc(p) {}
    ~TraceRecord() {}
    uint64_t getCycle() { return cycle; }
};

class FMARecord
{
 private:
    uint64_t tena_addr;
    uint64_t tena_rows;
    uint64_t tena_cols;    
    uint64_t tena_mask;
    uint64_t tenb_addr;
    uint64_t tenb_cols;
    bool both_in_scp;
    bool is_ima8a32;
    
    // Formula for latency -- what is the intrinsic latency ?
    uint64_t latency();

    // Formula for usage -- how much of the VPU are we using?
    uint64_t usage();

 public:
    FMARecord();
    ~FMARecord() {};
    FMARecord(uint64_t tenaddr_, uint64_t tena_rows_, uint64_t tena_cols, uint64_t tena_mask, uint64_t tenb_addr, uint64_t tenb_cols, bool both_in_scp, bool is_ima8a32);        
};

////////////////////////////////////////////////////////////
// Checked Events
// CheckedEvent: Abstract class
// PreciseCountEvent: Count number of occurrences of event
//
class CheckedEvent {

 protected:
    std::string name;
    Analyzer *analyzer;
    
 public:
    CheckedEvent() = delete;
    ~CheckedEvent() {};
    CheckedEvent(std::string name_): name(name_) {}
    void setAnalyzer (Analyzer *analyzer_) { analyzer = analyzer_; }
    virtual bool evaluateEvent() = 0;
};


class PreciseCountEvent: public CheckedEvent {

 private:
    uint64_t id;
    uint64_t expected_count;

 public:
    PreciseCountEvent() = delete;
    ~PreciseCountEvent() {};
    PreciseCountEvent(std::string name_, uint64_t id_, uint64_t expected_count_);

    virtual bool evaluateEvent();
};

////////////////////////////////////////////////////////////

// Top-level class: TBD: If you add DPI-export, should we switch to inheriting from testMod ?
class Perf: public testBase {

  private:
    uint64_t num_shires;
    bool has_master_shire;
    uint64_t num_minions;
    uint64_t num_compute_minions;
    uint64_t num_instructions;
    uint64_t num_tensor_instructions;
    bool init_done;
    std::vector<ThreadAnalyzer *> thread_analyzer; 

  public:
    Perf();
    ~Perf();

    //  DPI Functions
    void perf_event_initialize(svBitVecVal *shire_mask_, svBitVecVal *minion_mask_, svBitVecVal *thread_mask);
    void perf_event_min_retire(svBitVecVal *cycle_, unsigned thread_id_, svBitVecVal* pc_, svBitVecVal *inst_bits_);
    void perf_event_tfma_start(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *fma_control_, svBitVecVal *mask_bits_);
    void perf_event_tfma_finish(svBitVecVal *cycle_, unsigned minion_id_);
    void perf_event_tquant_start(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *tquant_control_);
    void perf_event_tquant_finish(svBitVecVal *cycle_, unsigned minion_id_);
    void perf_event_icache_req(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *pc_, bool miss_, svBitVecVal *latency);
    void perf_event_tload_start(svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_);
    void perf_event_tload_finish(svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_);
    void perf_event_tstore_start(svBitVecVal *cycle_, unsigned minion_id_);
    void perf_event_tstore_finish(svBitVecVal *cycle_, unsigned minion_id_);

    // Uncomment when test build issue are resolved
    // void event_check_count_precise(int thread, int evt, int expected_count);
    // void perf_checker_event_count_precise(int thread, int evt, int expected_count);

    static Perf& getInstance();

    void threadFinished(uint64_t thread_id, uint64_t cycle);
};


// Top-level analyzer
// Currently contains event data and checked events:
// Should we move this to ThreadAnalyzer ?
class Analyzer {

 protected:
    Perf *top;
    std::vector<std::vector<uint64_t>> cycle_event_data;
    std::vector<CheckedEvent *> checked_events;

  public:
    Analyzer() = delete;
    Analyzer(Perf *top_);
    ~Analyzer() {};
    virtual void advanceState(uint64_t event, uint64_t cycle) = 0;
    virtual void printAnalyzerState() = 0;
    virtual void printStats(FILE *fp) = 0;

    // For event checking
    void addCheckedEvent(CheckedEvent *evt) { checked_events.push_back(evt); }
    uint64_t getCount(uint64_t id) { return cycle_event_data[id].size(); }
    bool evaluateEvents();
   
    // State transition tables
    static std::map<ThreadStateEvent, n_Perf::ThreadState> thread_state_table;
    static std::map<TVPUStateEvent, n_Perf::TVPUState> tvpu_state_table;
};

// Add VPU START WHEN YOU ARE ALREADY AT VPU ?
std::map<ThreadStateEvent, n_Perf::ThreadState> Analyzer::thread_state_table = 
    { {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_VPU_START}, n_Perf::THREAD_STATE_TVPU},
      {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_TL0_START}, n_Perf::THREAD_STATE_TL0},
      {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_TL1_START}, n_Perf::THREAD_STATE_TL1},
      {{n_Perf::THREAD_STATE_TVPU,n_Perf::EVT_TL0_START}, n_Perf::THREAD_STATE_TVPU_TL0},
      {{n_Perf::THREAD_STATE_TVPU,n_Perf::EVT_TL1_START}, n_Perf::THREAD_STATE_TVPU_TL1},
      {{n_Perf::THREAD_STATE_TL0,n_Perf::EVT_VPU_START}, n_Perf::THREAD_STATE_TVPU_TL0},
      {{n_Perf::THREAD_STATE_TL0,n_Perf::EVT_TL1_START}, n_Perf::THREAD_STATE_TL0_TL1},
      {{n_Perf::THREAD_STATE_TL1,n_Perf::EVT_VPU_START}, n_Perf::THREAD_STATE_TVPU_TL1},
      {{n_Perf::THREAD_STATE_TL1,n_Perf::EVT_TL0_START}, n_Perf::THREAD_STATE_TL0_TL1},
      {{n_Perf::THREAD_STATE_TL0_TL1,n_Perf::EVT_VPU_START}, n_Perf::THREAD_STATE_TVPU_TL0_TL1},
      {{n_Perf::THREAD_STATE_TVPU_TL0,n_Perf::EVT_TL1_START}, n_Perf::THREAD_STATE_TVPU_TL0_TL1},
      {{n_Perf::THREAD_STATE_TVPU_TL1,n_Perf::EVT_TL0_START}, n_Perf::THREAD_STATE_TVPU_TL0_TL1},
      {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_VPU_FINISH}, n_Perf::THREAD_STATE_RET},
      {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_TL0_FINISH}, n_Perf::THREAD_STATE_RET},
      {{n_Perf::THREAD_STATE_RET,n_Perf::EVT_TL1_FINISH}, n_Perf::THREAD_STATE_RET},
      {{n_Perf::THREAD_STATE_TL0,n_Perf::EVT_TL0_FINISH}, n_Perf::THREAD_STATE_RET},
      {{n_Perf::THREAD_STATE_TL1,n_Perf::EVT_TL1_FINISH}, n_Perf::THREAD_STATE_RET},
      {{n_Perf::THREAD_STATE_TVPU,n_Perf::EVT_VPU_FINISH}, n_Perf::THREAD_STATE_RET},    
      {{n_Perf::THREAD_STATE_TL0_TL1,n_Perf::EVT_TL0_FINISH}, n_Perf::THREAD_STATE_TL1},
      {{n_Perf::THREAD_STATE_TL0_TL1,n_Perf::EVT_TL1_FINISH}, n_Perf::THREAD_STATE_TL0},
      {{n_Perf::THREAD_STATE_TVPU_TL0,n_Perf::EVT_TL0_FINISH}, n_Perf::THREAD_STATE_TVPU},
      {{n_Perf::THREAD_STATE_TVPU_TL0,n_Perf::EVT_VPU_FINISH}, n_Perf::THREAD_STATE_TL0},
      {{n_Perf::THREAD_STATE_TVPU_TL1,n_Perf::EVT_TL1_FINISH}, n_Perf::THREAD_STATE_TVPU},
      {{n_Perf::THREAD_STATE_TVPU_TL1,n_Perf::EVT_VPU_FINISH}, n_Perf::THREAD_STATE_TL1},
      {{n_Perf::THREAD_STATE_TVPU_TL0_TL1,n_Perf::EVT_TL0_FINISH}, n_Perf::THREAD_STATE_TVPU_TL1},
      {{n_Perf::THREAD_STATE_TVPU_TL0_TL1,n_Perf::EVT_TL1_FINISH}, n_Perf::THREAD_STATE_TVPU_TL0},
      {{n_Perf::THREAD_STATE_TVPU_TL0_TL1,n_Perf::EVT_VPU_FINISH}, n_Perf::THREAD_STATE_TL0_TL1}
    };

std::map<TVPUStateEvent, n_Perf::TVPUState> Analyzer::tvpu_state_table =
    { {{n_Perf::TNONE, n_Perf::EVT_FMA_START}, n_Perf::TFMA},
      {{n_Perf::TNONE, n_Perf::EVT_TQUANT_START}, n_Perf::TQUANT},
      {{n_Perf::TNONE, n_Perf::EVT_TSTORE_START}, n_Perf::TSTORE},
      {{n_Perf::TFMA, n_Perf::EVT_FMA_FINISH}, n_Perf::TNONE},
      {{n_Perf::TQUANT, n_Perf::EVT_TQUANT_FINISH}, n_Perf::TNONE},
      {{n_Perf::TSTORE, n_Perf::EVT_TSTORE_FINISH}, n_Perf::TNONE}
    };


// Generic thread analyzer
// Useful for RISCV only threads or threads for which we do not care about tensor work
class ThreadAnalyzer: public Analyzer {

 protected:
    
    uint64_t thread_id;
    uint64_t minion_id;
    bool is_active;

    uint64_t num_instructions;

    // Thread states / events
    n_Perf::ThreadState thread_state;
    uint64_t thread_event;

    uint64_t last_event_cycle;
    
    bool thread_finished;
    uint64_t cycle_finished;

    // Retirement trace
    std::vector<TraceRecord> retire_trace;
    uint64_t latest_cycle_trace;
    uint64_t ret_duration;

    // Other stall stats
    std::vector<uint64_t> unknown_stall_duration;

  public:

    ThreadAnalyzer() = delete;
    ThreadAnalyzer(Perf *top_, uint64_t thread_id_, bool is_active_ = true);
    ~ThreadAnalyzer() {};

    bool threadFinished() { return thread_finished; }
    uint64_t numInstructions() { return num_instructions; }
    virtual void advanceState(uint64_t event, uint64_t cycle);
    virtual void printAnalyzerState();
    virtual void printStats(FILE *fp);
    void retireInstruction(uint64_t cycle, uint64_t pc, uint32_t inst_bits);
    
};


// Thread analyzer of tensor work
class ComputeThreadAnalyzer: public ThreadAnalyzer {

  private:    

    n_Perf::TVPUState tvpu_state;
    
    // Tensor instruction stats
    uint64_t num_tensor_instructions;
    uint64_t num_tensor_fma16a32;
    uint64_t num_tensor_ima8a32;
    uint64_t num_tensor_quant;
    uint64_t num_tensor_load0;
    uint64_t num_tensor_load1;   
    uint64_t num_tensor_store;
   
    // Tensor duration stats
    uint64_t total_tensorfma_duration;
    uint64_t total_tensorquant_duration;
    uint64_t total_tensorload0_duration;
    uint64_t total_tensorload1_duration;
    uint64_t total_tensorstore_duration;

    // State combination stats
    uint64_t vpu_duration;
    uint64_t tl0_duration;
    uint64_t tl1_duration;
    uint64_t vpu_tl0_duration;
    uint64_t vpu_tl1_duration;
    uint64_t tl0_tl1_duration;
    uint64_t vpu_tl0_tl1_duration;
    uint64_t unknown_duration;

  public:
    ComputeThreadAnalyzer() = delete;
    ComputeThreadAnalyzer(Perf *top_, uint64_t thread_id_);
    ~ComputeThreadAnalyzer() {};

    virtual void advanceState(uint64_t event, uint64_t cycle);
    void startFmaEvent(uint64_t cycle, uint64_t fma_control, uint64_t mask_bits);
    void finishFmaEvent(uint64_t  cycle);
    void startTQuantEvent(uint64_t cycle, uint64_t tquant_control);
    void finishTQuantEvent(uint64_t  cycle);
    void startTLoadEvent(uint64_t cycle, unsigned load_id);
    void finishTLoadEvent(uint64_t cycle, unsigned load_id);
    void startTStoreEvent(uint64_t cycle);
    void finishTStoreEvent(uint64_t cycle);

    void advanceTVPUState(uint64_t event);
    virtual void printAnalyzerState();
    virtual void printStats(FILE *fp);
};

#endif
