/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "testEndMonitor.h"

#include "cosim.h"

DPI_IMPORT_NOCONTEXT ( void, test_end_system_enable, (unsigned count),
                       testEndMonitor, systemEnable, (count) );

DPI_IMPORT_NOCONTEXT ( void, test_end_system_done, (unsigned fail, unsigned timeout, uint64_t cycle),
                       testEndMonitor, systemDone, (fail, timeout, cycle) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_done, (unsigned id, unsigned thread, uint64_t cycle),
                       testEndMonitor, done, (id, thread, cycle) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_done_all, (),
                       testEndMonitor, done_all, () );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_update_wfi, (unsigned id, unsigned wfi),
                       testEndMonitor, updateWfi, (id, wfi) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_update_enabled, (unsigned id, unsigned enabled),
                       testEndMonitor, updateEnabled, (id, enabled) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_update_pget_stall, (unsigned id, unsigned stall),
                       testEndMonitor, updatePgetStall, (id, stall) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_set_mask, (svBitVecVal* mask),
                       testEndMonitor, setMask, (mask) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_set_shire_mask, (svBitVecVal* mask),
                       testEndMonitor, setShireMask, (mask) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_timeout, (unsigned id, unsigned thread, uint64_t cycle),
                       testEndMonitor, timeout, (id, thread, cycle) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_fail, (unsigned id, unsigned thread, uint64_t cycle),
                       testEndMonitor, fail, (id, thread, cycle) );

DPI_IMPORT_NOCONTEXT ( void, test_end_monitor_power, (unsigned id, unsigned power_off),
                       testEndMonitor, power, (id, power_off) );


testEndMonitor* testEndMonitor::pInstance = NULL;

testEndMonitor::testEndMonitor (const std::string &name) :
   testBase (name)
{
   bzero(done_, sizeof(done_));
   bzero(maxCommits_, sizeof(maxCommits_));
   memset(mask_, 0xFF, sizeof(mask_));
   endOnWfi_ = false;
   systemCount_ = 0;
   systemDone_ = false;
   testMain::setCleanEnd(boost::bind(&testEndMonitor::cleanEnd, this));

   // do not wait for second thread unless it is enabled
   int secondThread = 0;
   !valuePlusArgs("ENABLE_SECOND_THREAD", "%d", &secondThread);
   if (secondThread == 0) {
      for (unsigned i = 0; i < NR_MINIONS; i++) { done_[i][1] = true; }
   }

   for (size_t minion_id = 0; minion_id < NR_MINIONS; ++minion_id) {
      enabled_valid_[minion_id] = false;
      power_on_[minion_id] = true;
      for (size_t thread_id = 0; thread_id < NR_THREADS; ++thread_id) {
         enabled_[minion_id][thread_id] = false;
      }
   }

   pgetAsWFIMask_ = 0x0; //interpret pget as wfi (might cause test to end)
   !valuePlusArgs("PGET_AS_WFI_MASK", "%x", &pgetAsWFIMask_);

   dropped_all_objections_ = false;

   // Link callbacks into cosim
   // This should probably be done by the main testbench (main_tb.cc),
   // but I am not sure of the implications..
   cosim::getInstance().on_fail([this](unsigned minion, unsigned thread,
                                       uint64_t cycle) { fail(minion, thread, cycle); });
   cosim::getInstance().on_max_commits([this](unsigned minion, unsigned thread,
                                              uint64_t cycle) {
      maxCommits(minion, thread, cycle);
   });
}

void testEndMonitor::systemEnable(unsigned count)
{
   systemCount_ = count;
}

void testEndMonitor::systemDone(unsigned fail, unsigned timeout, uint64_t cycle)
{
   systemDone_ = true;
   if (timeout) {
      log << LOG_WARN << "Received system done, due to Timeout" << endm;
      drop_objection(-1, 0xFFFFFFFF, 0x3, cycle);
   } else if (fail) {
      log << LOG_WARN << "Received system done, due to Error" << endm;
      drop_objection(1, 0xFFFFFFFF, 0x3, cycle);
   } else {
      #ifndef ZEBU_DPI
         if (getDone() == false) {
            log << LOG_ERR << "Received system done, but there are still active minions" << endm;
            for (size_t minion_id = 0; minion_id < NR_MINIONS; ++minion_id) {
               for (size_t thread_id = 0; thread_id < NR_THREADS; ++thread_id) {
                  if (!done_[minion_id][thread_id] && enabled_valid_[minion_id] && enabled_[minion_id][thread_id] && power_on_[minion_id]) {
                     log << LOG_ERR << "minion " << minion_id << " thread " << thread_id << " is still active" << endm;
                  }
               }
            }
         }
      #endif
      drop_all_objections_if_done(cycle);
   }
}

void testEndMonitor::drop_all_objections_if_done(uint64_t cycle)
{
   if ((systemCount_ > 0) && !systemDone_) { return; }

   if (!getDoneMaxions()) { return; }

   if (!dropped_all_objections_) {

     dropped_all_objections_ = true;

     log << LOG_INFO_FORCE << "Received 'done' from all the enabled minions and maxions.  Now dropping all objections..." << endm;

     drop_objection(0, 0xFFFFFFFF, 0x3, cycle);

   }
}

void testEndMonitor::done(unsigned id, unsigned tid, uint64_t cycle, bool msg )
{
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   if (!done_[id][tid]) {

      if (msg)log << LOG_INFO_FORCE << "minion " << id << " thread " << tid << " is done" << endm;

      done_[id][tid] = true;

   }
   drop_all_objections_if_done(cycle);
}

void testEndMonitor::done_maxion(unsigned hartid, unsigned exit_code)
{
   auto log_lvl = exit_code ? LOG_ERR : LOG_INFO;
   log << log_lvl << "maxion " << hartid << " is done with exit_code 0x" << std::hex << exit_code << endm;

   if (!done_maxion_[hartid]) {
      done_maxion_[hartid] = true;
      drop_all_objections_if_done(0x0);
   }
}

void testEndMonitor::drop_objection (int result, unsigned id, unsigned tid, uint64_t cycle) {
   bool system_drop = id == 0xFFFFFFFF && tid == 0x3;
   if (result == 0 ) {
      std::cout << "PASS. All Threads finished successfully" << std::endl;
      endSimAt(MINION_DONE_TO_TEST_END_GUARD);
   }
   else if (result == 1 ) {
       if (system_drop) {
           std::cout      << "ERROR: there are minions that jumped to FAIL" << std::endl;
           log << LOG_ERR << "ERROR: there are minions that jumped to FAIL" << endm;
       } else {
           std::cout      << "ERROR:" << cycle << " minion " << id << " thread " << tid << " jumped to FAIL" << std::endl;
           log << LOG_ERR << "ERROR:" << cycle << " minion " << id << " thread " << tid << " jumped to FAIL" << endm;
       }
   }
   else {
       if (system_drop) {
           std::cout      << "TIMEOUT: hart took too much time without new instructions" << std::endl;
           log << LOG_FTL << "TIMEOUT: hart took too much time without new instructions" << endm;
       } else {
           std::cout      << "TIMEOUT:" << cycle << " minion " << id << " thread " << tid << " too much time without new instructions" << std::endl;
           log << LOG_FTL << "TIMEOUT:" << cycle << " minion " << id << " thread " << tid << " too much time without new instructions" << endm;
       }
   }
}

void testEndMonitor::done_all(){
   for (unsigned i = 0; i < NR_MINIONS; i++) {
      for (unsigned j = 0; j< NR_THREADS; j++) {
         if (!done_[i][j]) done(i, j, false);
      }
   }
}

void testEndMonitor::updateWfi (unsigned id, unsigned wfi) {
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   for (int i = 0; i< NR_THREADS; i++){
      wfi_[id][i] = ((wfi >> i) & 1) == 1;
   }
   if (endOnWfi_) {
      for (int i = 0; i< NR_MINIONS; i++) {
         for (int j = 0; j < NR_THREADS; j++) {
            if (!(wfi_[i][j] || done_[i][j])) return;
         }
      }

      for (int i = 0; i< NR_MINIONS; i++) {
         for (int j = 0; j < NR_THREADS; j++) {
            if (!done_[i][j]) done(i, j, false);
         }
      }
   }
}

void testEndMonitor::updateEnabled(unsigned id, unsigned enabled) {
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) { id = (NUM_MINION_SHIRES*MIN_PER_S); enabled = (enabled & 0x1); }

   bool enabled0 = (enabled & 1) != 0;
   if (!enabled_valid_[id] || enabled_[id][0] != enabled0) {
      enabled_[id][0] = enabled0;
      log << LOG_INFO << "minion " << id << " thread 0 is " << (enabled0 ? "enabled" : "disabled") << endm;
   }

   bool enabled1 = (enabled & 2) != 0;
   if (!enabled_valid_[id] || enabled_[id][1] != enabled1) {
      enabled_[id][1] = enabled1;
      log << LOG_INFO << "minion " << id << " thread 1 is " << (enabled1 ? "enabled" : "disabled") << endm;
   }

   enabled_valid_[id] = true;
}

void testEndMonitor::updatePgetStall(unsigned id, unsigned stall) {
   unsigned wfi = 0;

   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   // get current WFI
   for (int i = 0; i< NR_THREADS; i++) {
      wfi |= (wfi_[id][i] ? 1 : 0) << i;
   }

   // and set to 1 if a pget stall is to be considered a wfi
   for (int thread = 0; thread < NR_THREADS; thread ++) {
      for (int port = 0; port < 4; port++) {
         if ((((pgetAsWFIMask_ >> port) % 2) > 0) && ((stall % 2) > 0)) {
            wfi |= 1 << thread;
         }
         stall >>= 1;
      }
   }

   updateWfi(id, wfi);
}

void testEndMonitor::setEndOnWfi() {
   endOnWfi_ = true;
   checkEndOnWfi();
}

void testEndMonitor::checkEndOnWfi(){
   for (int i = 0; i< NR_MINIONS; i++) {
      for (int j = 0; j < NR_THREADS; j++) {
         if (!(wfi_[i][j] || done_[i][j])) return;
      }
   }

   for (int i = 0; i< NR_MINIONS; i++) {
      for (int j = 0; j < NR_THREADS; j++) {
         if (!done_[i][j]) done(i, j, false);
      }
   }
}

void testEndMonitor::setMask(svBitVecVal *mask){
   uint64_t m = dpi2cpp(mask);
   setMask(m);
}

void testEndMonitor::setShireMask(svBitVecVal *mask){
   uint64_t m = dpi2cpp(mask);
   assert(m != 0);
   assert(!(m & 0xfffffff800000000));
   setShireMask(m);
}

void testEndMonitor::setShireMask(uint64_t mask){
   uint64_t shire_en = mask;
   log << LOG_INFO << "set Shire Mask value 0x" << std::hex << mask << endm;

   for (int sh = 0; sh <= NUM_MINION_SHIRES; sh++) {
      // save mask
      if ((shire_en & 1) == 0) {
         mask_[sh] = 0;
         // set done in disabled minions
         for (int m = 0; m < MIN_PER_S; m++) {
            for (int thread = 0; thread < NR_THREADS; thread++) {
               done(m + MIN_PER_S*sh, thread, false);
            }
         }
      }
      shire_en >>= 1;
   }
}

void testEndMonitor::setMask(uint64_t min_mask, uint64_t shire_mask){
   assert(MIN_PER_S <= 32); // if asserts fail, mask has to be an array rather than a single uint64
   assert(NUM_MINION_SHIRES <= 34);

   uint64_t shire_en = shire_mask;
   for (int sh = 0; sh <= NUM_MINION_SHIRES; sh++) {
      // save mask
      if ((shire_en & 1) == 0) mask_[sh] = 0;
      else mask_[sh] &= min_mask; // AND so that it doesn't enable already disabled minions (e.g. stub tb setting mask to 0 via DPI)

      // set done in disabled minions
      uint64_t min_en = mask_[sh];
      for (int m = 0; m < MIN_PER_S; m++) {
         if ((min_en & 1) == 0) {
            for (int thread = 0; thread < NR_THREADS; thread++) {
               done(m + MIN_PER_S*sh, thread, false);
            }
         }
         min_en >>= 1;
      }
      shire_en >>= 1;
   }
}

void testEndMonitor::initMinionThreadStatus(uint64_t shire_id, uint64_t minion_id, uint32_t thread_id, bool val)
{
   done_[shire_id*MIN_PER_S+minion_id][thread_id] = val;
}

void testEndMonitor::initShires(uint64_t shire_mask, uint64_t minion_mask, uint64_t thread_mask)
{
   bool shire_en  = false;
   bool minion_en = false;
   bool thread_en = false;
   bool state     = false;

   log << LOG_INFO_FORCE << "Shire mask: 0x" << std::hex << shire_mask << " Minion Mask: 0x" << std::hex << minion_mask << " Thread Mask: 0x" << std::hex << thread_mask << endm;

   for (uint64_t S = 0; S <= NUM_MINION_SHIRES; S++) {
      shire_en = (shire_mask >> S) & 1UL;
      for (uint32_t M = 0; M < MIN_PER_S; M++) {
         minion_en = (minion_mask >> M) & 1UL;
         for (uint32_t T = 0; T < NR_THREADS; T++) {
            thread_en = (thread_mask >> T) & 1UL;
            state = (shire_en & minion_en & thread_en);
            if (state) log << LOG_DEBUG << "Initialiazing hart S" << std::dec << S << ":N" << (M / 8) << ":M" << (M % 8) << ":H" << T << " Valid: " << state << endm;
            initMinionThreadStatus(S, M, T, !state);
         }
      }
   }
}

void testEndMonitor::initMaxions(uint64_t maxions_mask) {
   bool maxion_en = false;

   log << LOG_INFO << "Maxions mask: 0x" << std::hex << maxions_mask << endm;

   for (uint64_t i = 0; i < NUM_MAXIONS; ++i) {
      maxion_en = (maxions_mask >> i) & 1UL;
      if (maxion_en) {
         log << LOG_DEBUG << "Initializing maxion hartid " << i << endm;
         done_maxion_[i] = false;
      } else {
         done_maxion_[i] = true;
      }
   }
}

bool testEndMonitor::getDone(){
   for (size_t minion_id = 0; minion_id < NR_MINIONS; ++minion_id) {
      for (size_t thread_id = 0; thread_id < NR_THREADS; ++thread_id) {
         if (done_[minion_id][thread_id] || (!enabled_valid_[minion_id]) || (!enabled_[minion_id][thread_id]) || (!power_on_[minion_id])) {
            continue;
         }
         return false;
      }
   }
   return true;
}

bool testEndMonitor::getDoneMaxions(){
   for (unsigned i = 0; i < NUM_MAXIONS; ++i) {
      if (!done_maxion_[i]) {
         return false;
      }
   }
   return true;
}

void testEndMonitor::timeout(unsigned id, unsigned thread, uint64_t cycle){
  // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   if (done_[id][thread] || !power_on_[id]) return;
 
   if (systemCount_ <= 0) {
      log << LOG_WARN << "Minion " << id << " Thread " << thread << " timed out at " << cycle << endm; 
      drop_objection (-1, id, thread, cycle);
   }
}

void testEndMonitor::maxCommits(unsigned id, unsigned thread, bool msg){
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   log << LOG_WARN << "max nr of commits reached for minion" << id << ", thread " << thread << endm;
   maxCommits_[id][thread] = true;
   #ifndef ZEBU_DPI
      done(id, thread, true);  //zebu does not have commit limit
   #endif
}

testEndMonitor& testEndMonitor::getInstance() {
   if (pInstance == NULL) pInstance = new testEndMonitor("testEndMon");
   return *pInstance;
}

void testEndMonitor::deleteInstance() {
   delete pInstance;
   pInstance = NULL;
}

bool testEndMonitor::cleanEnd(){
   // if a minion is not done (either max commits or done), return true so that errors are reported
   // if one minion reached max_commits, return false
   // otherwise (all minions done without reaching max commits), return true

   bool maxCommitsReached = false;
   for (unsigned i = 0; i < NR_MINIONS; i++) {
      for (unsigned j = 0; j < NR_THREADS; j++) {
         if (!done_[i][j] && power_on_[i]) return true;
         if (getMaxCommits(i, j)) maxCommitsReached = true;
      }
   }
   return !maxCommitsReached;
}

void testEndMonitor::fail(unsigned id, unsigned tid, uint64_t cycle){
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   if (systemCount_ <= 0) {
      drop_objection(1, id, tid, cycle);
   }
}

void testEndMonitor::power(unsigned id, unsigned power_off){
   // Repurpose IO Shire from 254 to 34
   if (id == IO_SHIRE_ID*MIN_PER_S) id = (NUM_MINION_SHIRES*MIN_PER_S);

   if (power_off) {
      log << LOG_INFO << "Powering off minion " << id << endm;
      power_on_[id] = false;
   } else {
      log << LOG_INFO << "Powering on minion " << id << endm;
      power_on_[id] = true;
   }
}

testEndMonitor::~testEndMonitor() {
}
