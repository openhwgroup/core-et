/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cosim.h"
#include "perf_event_monitor.h"

#include "vlog_defines.h"

using namespace n_Perf;

DPI_IMPORT_NOCONTEXT(void, perf_event_initialize,
                     (svBitVecVal *shire_mask, svBitVecVal *minion_mask, svBitVecVal *thread_mask),
		     Perf, perf_event_initialize,
                     (shire_mask, minion_mask, thread_mask));

DPI_IMPORT_NOCONTEXT(void, perf_event_min_retire,
                     (svBitVecVal *cycle_, unsigned thread_id_, svBitVecVal *pc_, svBitVecVal *inst_bits_),
		     Perf, perf_event_min_retire,
                     (cycle_, thread_id_, pc_, inst_bits_));

DPI_IMPORT_NOCONTEXT(void, perf_event_icache_req,
		     (svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *pc_, bool miss_, svBitVecVal *latency_),
                     Perf, perf_event_icache_req,
		     (cycle_, minion_id_, pc_, miss_, latency_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tfma_start,
		     (svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *fma_control_, svBitVecVal *mask_bits_),
		     Perf, perf_event_tfma_start,
		     (cycle_, minion_id_, fma_control_, mask_bits_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tfma_finish,
                     (svBitVecVal *cycle_, unsigned minion_id_),
                     Perf, perf_event_tfma_finish,
                     (cycle_, minion_id_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tquant_start,
                     (svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *tquant_control_),
                     Perf, perf_event_tquant_start,
                     (cycle_, minion_id_, tquant_control_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tquant_finish,
                     (svBitVecVal *cycle_, unsigned minion_id_),
                     Perf, perf_event_tquant_finish,
                     (cycle_, minion_id_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tload_start,
		     (svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_),
		     Perf,perf_event_tload_start,
		     (cycle_, minion_id_, load_id_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tload_finish,
		     (svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_),
		     Perf,perf_event_tload_finish,
		     (cycle_, minion_id_, load_id_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tstore_start,
		     (svBitVecVal *cycle_, unsigned minion_id_),
		     Perf,perf_event_tstore_start,
		     (cycle_, minion_id_));

DPI_IMPORT_NOCONTEXT(void, perf_event_tstore_finish,
		     (svBitVecVal *cycle_, unsigned minion_id_),
		     Perf,perf_event_tstore_finish,
		     (cycle_, minion_id_));


// Events to check -- uncomment when test build issues are resolved
//DPI_IMPORT_NOCONTEXT(void, perf_checker_event_count_precise,
//                     (int thread, int id, int exp_count),
//                     Perf, perf_checker_event_count_precise,
//                     (thread, id, exp_count));

//DPI_EXPORT(void, event_check_count_precise,
//	   (int thread, int evt, int expected_count),
//	   Perf, event_check_count_precise,
//	   (thread, evt, expected_count));


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Checked events
// Each type of checked event should implemnent its own constructore and EvaluateEvent function
//
PreciseCountEvent::PreciseCountEvent(std::string name_, uint64_t id_, uint64_t expected_count_):
    CheckedEvent(name_),
    id(id_),
    expected_count(expected_count_)
{}


bool PreciseCountEvent::evaluateEvent()
{
    uint64_t actual_count = analyzer->getCount(id);
    
    if (actual_count == expected_count) {
	printf ("Check: %s passed\n", name.c_str());
	return true;
    } else {
        printf ("Check:t %s failed: Expected count: %lu, Actual count %lu\n", name.c_str(), expected_count, actual_count);
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Perf (top level functions)

Perf& Perf::getInstance() {
  static Perf instance;
  return instance;
}


// Top-level perf functions, mostly for initialization
// and to call individual analyzer functions

Perf::Perf(): testBase("Perf"),
	      num_shires(0), has_master_shire(false),
	      num_minions(0), num_compute_minions(0),
	      num_instructions(0), num_tensor_instructions(0), 
	      init_done(0)
{
    init_done = false;    
    thread_analyzer.resize(0);
    printf ("Instantiated Performance Analyzer\n");
}

Perf::~Perf() 
{
    for (uint64_t i=0; i < thread_analyzer.size(); i++) {
	delete thread_analyzer[i];
    }
}


// Process masks to initilialize structures
void Perf::perf_event_initialize(svBitVecVal *shire_mask_, svBitVecVal *minion_mask_, svBitVecVal *thread_mask_)
{
   uint64_t shire_mask = dpi2cpp(shire_mask_, 0, 64);
   uint64_t minion_mask = dpi2cpp(minion_mask_, 0, 64);
   uint64_t thread_mask = dpi2cpp(thread_mask_, 0, 64);

   if (init_done) return;

   bool has_master_shire = ((shire_mask & master_shire_mask) == master_shire_mask);

   uint64_t num_minions_per_shire = 0;
   if (!has_master_shire) {   
       for (uint64_t i=0; i < 32; i++) {
	   uint32_t mask = (1 << i);
	   if ((shire_mask & mask) == mask) {
	       num_shires++;
	   }
	   if ((minion_mask & mask) == mask) {
	       num_minions_per_shire++;
	   }
       }
   } else {
       // If there is master shire, the thread numbering goes over 2048 anyway, so accommodate to that.
       num_shires = 33;
       num_minions_per_shire = 32;
   }
   
   bool has_compute_thread = ((thread_mask & 0x1) == 0x1);
   bool has_helper_thread = ((thread_mask & 0x2) == 0x2);     
   
   // Setup the thread analyzers now
   // For code robustmess, we always assume that there will be helper thread
   // The only difference is that we "deactivate" the analyzer of the helper thread
   // if no helper thread is enabled
   num_minions = num_shires * num_minions_per_shire;
   num_compute_minions = num_minions - static_cast<uint64_t>(has_master_shire) * num_minions_per_shire;
   thread_analyzer.resize(num_minions * 2);

   for (uint64_t i=0; i < num_compute_minions; i++) {
       thread_analyzer[i*2] = new ComputeThreadAnalyzer(this, i*2);
       thread_analyzer[i*2+1] = new ThreadAnalyzer(this, i*2+1, has_helper_thread);
   }

   if (has_master_shire) {
       for (uint64_t i=num_compute_minions; i < num_minions; i++) {
	   thread_analyzer[i*2] = new ThreadAnalyzer(this, i*2);
	   thread_analyzer[i*2+1] = new ThreadAnalyzer(this, i*2+1, true);
       }
   }
   init_done = true;

}

// Uncomment when test build issues are resolved
//void Perf::perf_checker_event_count_precise(int thread, int id, int expected_count)
//{
//    // TBD: See what kind of names you are going to create
//    PreciseCountEvent *evt = new PreciseCountEvent("EVENT", id, expected_count);
//    thread_analyzer[thread]->addCheckedEvent(evt);
//    evt->setAnalyzer(thread_analyzer[thread]);
//}


Analyzer::Analyzer(Perf *top_):
    top(top_)
{
    cycle_event_data.resize(EVT_MAX_EVENT);
    for (uint64_t i=0; i < EVT_MAX_EVENT; i++) {
	cycle_event_data[i].resize(0);
    } 
}

ThreadAnalyzer::ThreadAnalyzer(Perf *top_, uint64_t thread_id_, bool is_active_): 
    Analyzer(top_),							      
    thread_id(thread_id_),
    is_active(is_active_),
    num_instructions(0),
    thread_state(THREAD_STATE_RET),
    thread_event(EVT_UNKNOWN),
    last_event_cycle(0),
    thread_finished(false),
    cycle_finished(0),
    latest_cycle_trace(0),
    ret_duration(0)
{    
    minion_id = thread_id >> 1;
    retire_trace.resize(0);    
    unknown_stall_duration.resize(0);
}

ComputeThreadAnalyzer::ComputeThreadAnalyzer(Perf *top_, uint64_t thread_id_):
    ThreadAnalyzer(top_, thread_id_, true),
    tvpu_state(TNONE),
    num_tensor_instructions(0),
    num_tensor_fma16a32(0),
    num_tensor_ima8a32(0),
    num_tensor_quant(0),
    num_tensor_load0(0),
    num_tensor_load1(0),
    total_tensorfma_duration(0),
    total_tensorquant_duration(0),
    total_tensorload0_duration(0),
    total_tensorload1_duration(0),
    total_tensorstore_duration(0),
    vpu_duration(0),
    tl0_duration(0),
    tl1_duration(0),
    vpu_tl0_duration(0),
    vpu_tl1_duration(0),
    vpu_tl0_tl1_duration(0),
    unknown_duration(0)
{
} 


// Top-level functions -- pass them over to the correct analyzer

void Perf::perf_event_min_retire(svBitVecVal *cycle_, unsigned thread_id_, svBitVecVal *pc_, svBitVecVal *inst_bits_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    uint64_t pc = dpi2cpp(pc_, 0, 64);
    uint32_t inst_bits = dpi2cpp(inst_bits_, 0, 32);

    num_instructions++;
    if (thread_id_ >= thread_analyzer.size()) {
	printf ("LOOK HERE: %lu %lu\n", thread_id_, thread_analyzer.size());
    }
    ThreadAnalyzer *t_an = thread_analyzer[thread_id_];
    t_an->retireInstruction(cycle, pc, inst_bits);
}

void Perf::perf_event_icache_req(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *pc_, bool miss_, svBitVecVal *latency)
{
  // Nothing yet
}


void Perf::perf_event_tfma_start(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *fma_control_, svBitVecVal *mask_bits_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    uint64_t fma_control = dpi2cpp(fma_control_, 0, 64);
    uint64_t mask_bits = dpi2cpp(mask_bits_, 0, 64);

    // Do this to catch any stray signal activity during reset
    if (num_instructions == 0) {
	return;
    }
    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->startFmaEvent(cycle, fma_control, mask_bits);
    } else {
	// Warning or assertion here?
    }
    printf ("%lu Minion: %u FMA started\n", cycle, minion_id_);
}

void Perf::perf_event_tfma_finish(svBitVecVal *cycle_, unsigned minion_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    if (num_instructions == 0) {
	return;
    }

    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->finishFmaEvent(cycle);
    } else {
	// Warning or assertion here
    }
    printf ("%lu Minion: %u FMA finished\n", cycle, minion_id_);
}


void Perf::perf_event_tquant_start(svBitVecVal *cycle_, unsigned minion_id_, svBitVecVal *tquant_control_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    uint64_t tquant_control = dpi2cpp(tquant_control_, 0, 64);

    if (num_instructions == 0) {
	return;
    }
   
   ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
   if (ct_an) {
      ct_an->startTQuantEvent(cycle, tquant_control);
   } else  {
     // Warning or assetion here
   }
}


void Perf::perf_event_tquant_finish(svBitVecVal *cycle_, unsigned minion_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    if (num_instructions == 0) {
	return;
    }

    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->finishTQuantEvent(cycle);
    } else {
	// Warning or assertion here
    }
    printf ("%lu Minion: %u FMA finished\n", cycle, minion_id_);
}

void Perf::perf_event_tload_start(svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);

    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->startTLoadEvent(cycle, load_id_);
    } else {
	// Warning or assertion here
    }
    printf ("%lu Minion: %u TensorLoad%u started\n", cycle, minion_id_, load_id_);
}


void Perf::perf_event_tload_finish(svBitVecVal *cycle_, unsigned minion_id_, unsigned load_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);

    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->finishTLoadEvent(cycle, load_id_);
    } else {
	// Warning or assertion here
    }
    
    printf ("%lu Minion: %u TensorLoad%u finished\n", cycle, minion_id_, load_id_);	

   
}

void Perf::perf_event_tstore_start(svBitVecVal *cycle_, unsigned minion_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    
    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->startTStoreEvent(cycle);
    } else {
	// Warning or assertion here
    }
    printf ("%lu Minion: %u TensorStore started\n", cycle, minion_id_);
}

void Perf::perf_event_tstore_finish(svBitVecVal *cycle_, unsigned minion_id_)
{
    uint64_t cycle = dpi2cpp(cycle_, 0, 64);
    
    ComputeThreadAnalyzer *ct_an = dynamic_cast<ComputeThreadAnalyzer *>(thread_analyzer[2*minion_id_]);
    if (ct_an) {
	ct_an->finishTStoreEvent(cycle);
    } else {
	// Warning or assertion here
    }
    printf ("%lu Minion: %u TensorStore started\n", cycle, minion_id_);
}

void Perf::threadFinished(uint64_t thread_id, uint64_t cycle)
{
    
    // Tentative: When we have master shire, we just need Thread 0 (2048) of master shire to finish
    // When we do not we wait all the threads to finish
    printf ("THREAD FINISHED: %lu\n", thread_id);

    bool all_done = true;
    if (num_minions > 1024) {
	if (thread_id != 2048) {
	    all_done = false;
	}
    } else {
	for (uint64_t i=0; i < thread_analyzer.size(); i++) {
	    if (thread_analyzer[i]->threadFinished() || thread_analyzer[i]->numInstructions() == 0) {
		continue;
	    } else {
		all_done = false;
		break;
	    } 
	}
    }
    
    if (!all_done) return;
    
    // Uncomment hen test build issues are resolved
    //bool passed = true;
    //for (uint64_t i=0; i < thread_analyzer.size(); i++) {
    //	passed = passed & thread_analyzer[i]->evaluateEvents();
	// TODO: Make passed accessible to perf and in that way to tests?
    //}

    FILE *fp = fopen("testme.stats","w");	 
    fprintf (fp, "Statistics:\n");
    fprintf (fp, "Total cycles: %lu\n", cycle);       
    for (uint64_t i=0; i < thread_analyzer.size(); i = i+2) {
	if (thread_analyzer[i]->numInstructions() > 0 || thread_analyzer[i]->numInstructions() > 0) {    	   
	    fprintf (fp,"  MINION %d\n", i/2);	
	    thread_analyzer[i]->printStats(fp);
	    thread_analyzer[i+1]->printStats(fp);	  
	}
    }
    fclose(fp);
}


bool Analyzer::evaluateEvents() {
    bool result = true;
    for (uint64_t i =0; i < checked_events.size(); i++) {
	result = result & checked_events[i]->evaluateEvent();
    }
    return result;
}

/// Thread Analyzer functions

void ThreadAnalyzer::printStats(FILE *fp)
{
    fprintf (fp, "\tT[%d]: Instructions: %lu\n", thread_id&1, num_instructions);
}

void ThreadAnalyzer::retireInstruction(uint64_t cycle, uint64_t pc, uint32_t inst_bits)
{
    char str[128] = "";
    checker::emu_disasm(str, 128, inst_bits);
    
    num_instructions++;

    printf("RETIRE %llu 1 %i %012llx %08x %s\n", cycle, thread_id, pc, inst_bits, str);
    TraceRecord retire_record = TraceRecord(cycle, pc);

    uint64_t ret_diff;

    // Do this check because other events can be before first retirement
    // and this is beyond our control. For example FMA working signal gets asserted...
    if (retire_trace.size() == 0) {
	ret_diff = cycle;
    } else {
	ret_diff = cycle - latest_cycle_trace; 
    }
    if (ret_diff > 1) {
	unknown_stall_duration.push_back(ret_diff - 1);
	printf ("M[%d] ADDING: %lu to Unknown\n", minion_id, ret_diff-1);	    
    }

    latest_cycle_trace = cycle;
    if (thread_state == THREAD_STATE_RET) {
	ret_duration += cycle -  last_event_cycle;
    }
    last_event_cycle = cycle;

    retire_trace.emplace_back(cycle,pc);
    
    if (inst_bits == 0x10500073 || inst_bits == 0x00000073 || inst_bits == 0x82201073) {
	printf ("WFI / ECALL / CSR_STALL!\n");
	thread_finished = true;
	top->threadFinished(thread_id, cycle);
    }  else {
	thread_finished = false;
    }
}

void ComputeThreadAnalyzer::printStats(FILE *fp)
{
    fprintf (fp, "\tT[%d]: Instructions: %lu\n", thread_id&1, num_instructions);
    fprintf (fp, "\t       Tensor Instructions: %lu\n", num_tensor_instructions);
    fprintf (fp, "\t       Tensor FMA16A32: %lu\n", num_tensor_fma16a32);
    fprintf (fp, "\t       Tensor IMA8A32: %lu\n", num_tensor_ima8a32);
    fprintf (fp, "\t       Tensor QUANT: %lu\n", num_tensor_quant);
    fprintf (fp, "\t       Tensor STORE :%lu\n", num_tensor_store);
    fprintf (fp, "\t       Tensor Load 0 Cycles: %lu\n", total_tensorload0_duration);
    fprintf (fp, "\t       Tensor Load 1 Cycles: %lu\n", total_tensorload1_duration);    
    fprintf (fp, "\t       VPU (FMA) Cycles: %lu\n", total_tensorfma_duration);
    fprintf (fp, "\t       Tensor Store Duration: %lu\n", total_tensorstore_duration);
    fprintf (fp, "\t       RET (RISCV) Cycles: %lu\n", ret_duration);
    fprintf (fp, "\t       TL0 Cycles %lu\n", tl0_duration);
    fprintf (fp, "\t       TL1 Cycles %lu\n", tl1_duration);
    fprintf (fp, "\t       TL0_TL1 Cycles %lu\n", tl0_tl1_duration);
    fprintf (fp, "\t       VPU Cycles %lu\n", vpu_duration);
    fprintf (fp, "\t       VPU_TL0 Cycles %lu\n", vpu_tl0_duration);
    fprintf (fp, "\t       VPU_TL1 Cycles %lu\n", vpu_tl1_duration);
    fprintf (fp, "\t       VPU_TL0_TL1 Cycles %lu\n", vpu_tl0_tl1_duration);
    fprintf (fp, "\t       Unknown Cycles %lu\n", unknown_duration);
}

void ComputeThreadAnalyzer::startFmaEvent(uint64_t cycle, uint64_t fma_control, uint64_t mask_bits)
{
    cycle_event_data[EVT_FMA_START].push_back(cycle);

    thread_event = EVT_VPU_START;
    advanceTVPUState(EVT_FMA_START);
    advanceState(EVT_VPU_START, cycle);
    num_tensor_instructions++;

    printf ("CTRL: %lx\n", fma_control);
    
    // Categorize FMA/IMA based on control
    if((fma_control & 0xE) == 0x02) {
        num_tensor_fma16a32++;
    } else if ((fma_control & 0xE) == 0x06) {
        num_tensor_ima8a32++;
    }

}

void ComputeThreadAnalyzer::finishFmaEvent(uint64_t cycle)
{
    cycle_event_data[EVT_FMA_FINISH].push_back(cycle);
    thread_event = EVT_VPU_FINISH;
    total_tensorfma_duration += cycle - cycle_event_data[EVT_FMA_START].back();
    printf ("FMA DURATION: %lu %lu\n", cycle - cycle_event_data[EVT_FMA_START].back(), total_tensorfma_duration);
    uint64_t cycle_diff = cycle - latest_cycle_trace;
    printf ("M[%d] ADDING: %lu to ", minion_id, cycle_diff);
    printAnalyzerState();
    advanceTVPUState(EVT_FMA_FINISH);
    advanceState(EVT_VPU_FINISH, cycle);
    latest_cycle_trace = cycle;
}

void ComputeThreadAnalyzer::startTQuantEvent(uint64_t cycle, uint64_t tquant_control)
{
    cycle_event_data[EVT_TQUANT_START].push_back(cycle);
    thread_event = EVT_VPU_START;
    advanceTVPUState(EVT_TQUANT_START);
    advanceState(EVT_VPU_START, cycle);
    num_tensor_quant++;
}


void ComputeThreadAnalyzer::finishTQuantEvent(uint64_t cycle) {
    cycle_event_data[EVT_TQUANT_FINISH].push_back(cycle);
    thread_event = EVT_VPU_FINISH;
    total_tensorquant_duration += cycle - cycle_event_data[EVT_TQUANT_START].back();
    printf ("TQUANT DURATION: %lu %lu\n", cycle - cycle_event_data[EVT_TQUANT_START].back(), total_tensorquant_duration);
    uint64_t cycle_diff = cycle - latest_cycle_trace;
    printf ("M[%d] ADDING: %lu to ", minion_id, cycle_diff);
    printAnalyzerState();
    advanceTVPUState(EVT_TQUANT_FINISH);
    advanceState(EVT_VPU_FINISH, cycle);
    latest_cycle_trace = cycle;
}

void ComputeThreadAnalyzer::startTLoadEvent(uint64_t cycle, unsigned load_id)
{
    num_tensor_instructions++; 
    if (load_id == 0) {
	cycle_event_data[EVT_TL0_START].push_back(cycle);
	thread_event = EVT_TL0_START;
	advanceState(EVT_TL0_START, cycle);
	num_tensor_load0++;
    } else if (load_id == 1) {
	cycle_event_data[EVT_TL1_START].push_back(cycle);
	thread_event = EVT_TL1_START;
	advanceState(EVT_TL1_START, cycle);
	num_tensor_load1++;
    }
}

void ComputeThreadAnalyzer::finishTLoadEvent(uint64_t cycle, unsigned load_id)
{
    if (load_id == 0) {
	cycle_event_data[EVT_TL0_FINISH].push_back(cycle);
	total_tensorload0_duration += cycle - cycle_event_data[EVT_TL0_START].back();
	printf ("TL0 DURATION: %lu %lu\n", total_tensorload0_duration, cycle - cycle_event_data[EVT_TL0_START].back());
	uint64_t cycle_diff = cycle - latest_cycle_trace;
	printf ("M[%d] ADDING: %lu to ", minion_id, cycle_diff);
	printAnalyzerState();
	advanceState(EVT_TL0_FINISH, cycle);
	latest_cycle_trace = cycle;
    } else if (load_id == 1) {
	cycle_event_data[EVT_TL1_FINISH].push_back(cycle);
	total_tensorload1_duration += cycle -  cycle_event_data[EVT_TL1_START].back();
	printf ("TL1 DURATION: %lu %lu\n", total_tensorload1_duration, cycle -  cycle_event_data[EVT_TL1_START].back());
	uint64_t cycle_diff = cycle - latest_cycle_trace;
	printf ("M[%d] ADDING: %lu to ", minion_id, cycle_diff);
	printAnalyzerState();
	advanceState(EVT_TL1_FINISH, cycle);
	latest_cycle_trace = cycle;
    }
}

void ComputeThreadAnalyzer::startTStoreEvent(uint64_t cycle)
{
    num_tensor_instructions++;
    cycle_event_data[EVT_TSTORE_START].push_back(cycle);
    thread_event = EVT_VPU_START;
    advanceTVPUState(EVT_TSTORE_START);
    advanceState(EVT_VPU_START, cycle);
    num_tensor_store++;    
}

void ComputeThreadAnalyzer::finishTStoreEvent(uint64_t cycle)
{
    cycle_event_data[EVT_TSTORE_FINISH].push_back(cycle);
    thread_event = EVT_VPU_FINISH;
    total_tensorstore_duration += cycle - cycle_event_data[EVT_TSTORE_START].back();
    printf ("TSTORE DURATION: %lu %lu\n", cycle - cycle_event_data[EVT_TSTORE_START].back(), total_tensorstore_duration);
    uint64_t cycle_diff = cycle - latest_cycle_trace;
    printf ("M[%d] ADDING: %lu to ", minion_id, cycle_diff);
    printAnalyzerState();
    advanceTVPUState(EVT_TSTORE_FINISH);
    advanceState(EVT_VPU_FINISH, cycle);
    latest_cycle_trace = cycle;      
}

void ThreadAnalyzer::advanceState(uint64_t event, uint64_t cycle)
{       
    ThreadStateEvent cur_state_event = ThreadStateEvent(thread_state,event);
    thread_state = thread_state_table[cur_state_event];
    printf ("New state: ");
    printAnalyzerState();
    printf ("\n");
    last_event_cycle = cycle;
}

void ComputeThreadAnalyzer::advanceState(uint64_t event, uint64_t cycle)
{
    printf ("THREAD STATE: %d %lu %lu \n", thread_state, cycle, last_event_cycle);
    switch (thread_state) {
    case THREAD_STATE_RET: ret_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TL0: tl0_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TL1: tl1_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TL0_TL1: tl0_tl1_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TVPU: vpu_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TVPU_TL0: vpu_tl0_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TVPU_TL1: vpu_tl1_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_TVPU_TL0_TL1: vpu_tl0_tl1_duration += cycle - last_event_cycle; break;
    case THREAD_STATE_UNKNOWN: unknown_duration += cycle - last_event_cycle; break;
    default: break;
    }
    ThreadStateEvent cur_state_event = ThreadStateEvent(thread_state,event);
    thread_state = thread_state_table[cur_state_event];
    printf ("New state: ");
    printAnalyzerState();
    printf ("\n");
    last_event_cycle = cycle;
}

void ComputeThreadAnalyzer::advanceTVPUState(uint64_t event)
{
    TVPUStateEvent cur_state_event = TVPUStateEvent(tvpu_state,event);
    tvpu_state = tvpu_state_table[cur_state_event];
}

void ThreadAnalyzer::printAnalyzerState()
{
    switch (thread_state) {
    case THREAD_STATE_RET: printf ("RET\n"); break;
    case THREAD_STATE_UNKNOWN: printf ("UNKNOWN\n"); break;
    default: break;
    }	       
}

void ComputeThreadAnalyzer::printAnalyzerState()
{
    switch (thread_state) {
    case THREAD_STATE_RET: printf ("RET\n"); break;
    case THREAD_STATE_TL0: printf ("TL0\n"); break;
    case THREAD_STATE_TL1: printf ("TL1\n"); break;
    case THREAD_STATE_TVPU: printf ("FMA\n"); break;
    case THREAD_STATE_TL0_TL1: printf ("TL0_TL1\n"); break;
    case THREAD_STATE_TVPU_TL0: printf ("VPU_TL0\n"); break;
    case THREAD_STATE_TVPU_TL1: printf ("VPU_TL1\n"); break;
    case THREAD_STATE_TVPU_TL0_TL1: printf ("VPU_TL0_TL1\n"); break;
    case THREAD_STATE_UNKNOWN: printf ("UNKNOWN\n"); break;
    default: break;
    }	       
}
