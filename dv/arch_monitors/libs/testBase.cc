/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include <iostream>

#include  "testBase.h"

#ifndef DPI_SV_TOP_SCOPE
 #define TB_DPI_SCOPE "TOP"
#else
 #define TB_DPI_SCOPE "top"
#endif

// static members of testMain
int testMain::instanceCount_ = 0;
bool testMain::timeOut_ = false;
std::map<svScope, testMod*> testMain::modMap_;
std::vector<testMod*> testMain::modToDestroy_;
testMain::clean_end_f testMain::cleanEnd_;

void testMain::addTestMod(testMod& m, bool destroy){
  testLog log("testMain", LOG_INFO);
  svScope scope = m.getScope();
  std::string path = m.getPath();
  std::string name = m.getName();
  if ( scope == NULL){
    log<<LOG_ERR<<name<<" in path "<<path.c_str()<<" not found" <<endm;
    return;
  }
  if (modMap_.count(scope) == 0) {
    log<<LOG_DEBUG<<"Adding "<<path.c_str()<<endm;
    modMap_[scope]= &m;
    if (destroy) modToDestroy_.push_back(&m);
  }
  else {
    log<<LOG_ERR<<(name + " " + path).c_str()<<" already added"<<endm;
  }
}


void cpp2dpi(int width, const void* in, svBitVecVal* &v) {
  unsigned w = sizeof(svBitVecVal)*8;
  unsigned words = (width + w -1) / w;
  if (v == NULL) v = new svBitVecVal [words];
  memcpy(v,  in, words*sizeof(svBitVecVal));
}

void setBits2dpi(int lsb, int width, const void *inData, svBitVecVal* &v) {
  unsigned w = sizeof(svBitVecVal)*8;
  uint8_t *in = (uint8_t*) inData;
  for (int i=0; i < width; i++){
    int wO=(lsb+i)/w;
    int bO=(lsb+i)%w;
    int wI=i/8;
    int bI=i%8;
    if ( ((in[wI]>>bI) & 1) == 0)
      v[wO] &= ~(1<<bO);
    else
      v[wO] |= (1<<bO);
  }
}

#ifndef VERILATOR
static bool finish = false;
static bool zebu_printed= false;
bool simEnded() { return finish; }
#ifdef SIM_END_WITH_VPI_CB
EXTERN_DPI PLI_INT32 endSimCb (struct t_cb_data *) {
  if (!finish) {
    vpi_control( vpiFinish, 1);
    finish = true;
  }
  return 0;
}

void endSim() {
  if (!finish) {
    vpi_control( vpiFinish, 1);
    finish=true;
  }
}


void endSimAt(uint32_t extraTime) {
  if (extraTime == 0) vpi_control( vpiFinish, 1);
  else {
    s_vpi_time time;
    time.type = vpiSimTime;
    time.high=0;
    time.low=extraTime;
    s_cb_data cb;
    cb.reason = cbAfterDelay;
    cb.cb_rtn = endSimCb;
    cb.time = &time;
    cb.value = NULL;
    cb.user_data = NULL;

    vpiHandle h = vpi_register_cb (&cb);
    vpi_free_object(h);
  }
}
#else  // no verilator, end sim without vpi cb


#ifdef COCOTB_TEST_END
void simEnd(int code);
EXTERN_DPI void cpu_done_DPI(uint32_t nErrors);
#else
EXTERN_DPI void endSimAt_DPI(uint32_t extraTime);
#endif

void endSimAt(uint32_t extraTime) {
#ifdef COCOTB_TEST_END
    if (finish) return;
    finish = true; // Finish here to avoid reentrence
    // Report errors to cocotb
    svScope s = svGetScopeFromName("tb");
    if ( s != NULL){
        svSetScope(s);
        cpu_done_DPI(testLog::getErrors());
    } else {
        std::cout << "ERROR_DBG: tb scope not found!" << std::endl;
        exit(1);
    }
#else
  svScope s = svGetScopeFromName(TB_DPI_SCOPE);    
  if ( s != NULL){
    svSetScope(s);
    endSimAt_DPI(time2Cycles(extraTime));
  } else {
    testLog log;
    log<<LOG_ERR<<"Cannot find scope for TOP to call simEndAt => ending sim without delay"<<endm;
    endSim();
  }
#endif
}

void endSim() {
  endSimAt(0);
  finish = true;
}


#endif // no verilator, end sim without vpi cb
#else // no checker, but verilator
void finish_after(std::size_t);
 void endSimAt(uint32_t extraTime) {
   finish_after(extraTime);
 }

#endif



void setTimeOut(){
  testMain::timeOut_ = true;
#ifndef VERILATOR
  finish = true;
#endif
}


void setEnded(){
#ifndef VERILATOR
  finish = true;
#endif
}
