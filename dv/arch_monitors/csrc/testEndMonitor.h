/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_END_MONITOR_H_
#define _TEST_END_MONITOR_H_
#include <climits>
#include "testBase.h"
#include "vlog_defines.h"

#define MINION_DONE_TO_TEST_END_GUARD (400*DEFAULT_CLOCK_PERIOD)
#define NR_MINIONS (MIN_PER_S * (NUM_MINION_SHIRES + 1))
#define NR_THREADS 2
#define NUM_MAXIONS 4

class testEndMonitor: public testBase {
 public:
  testEndMonitor (const std::string &name);
  ~testEndMonitor();
  void setMask(svBitVecVal* mask);
  void setShireMask(svBitVecVal* mask);

  void setMask(uint64_t min_mask, uint64_t shire_mask = 1);
  void initMinionThreadStatus(uint64_t shire_id, uint64_t minion_id, uint32_t thread_id, bool val);
  void initShires(uint64_t shire_mask, uint64_t minion_mask, uint64_t thread_mask);
  void setShireMask(uint64_t mask);
  void initMaxions(uint64_t maxions_mask);

  uint64_t getMask (unsigned shire_nr) { return mask_[shire_nr]; }
  bool getDone();
  void systemEnable(unsigned int count);
  void systemDone(unsigned int fail, unsigned int timeout, uint64_t cycle);
  void done(unsigned id, unsigned thread, uint64_t cycle, bool msg = true);
  void done_maxion(unsigned hartid, unsigned exit_code);
  void fail(unsigned id, unsigned thread, uint64_t cycle);
  void power(unsigned id, unsigned power_off);
  void done_all();
  void updateEnabled(unsigned id, unsigned enabled);
  void updateWfi(unsigned id, unsigned wfi);
  void updatePgetStall(unsigned id, unsigned stall);
  void timeout(unsigned id, unsigned thread, uint64_t cycle);
  void maxCommits(unsigned id, unsigned thread, bool msg = true);
  bool getMaxCommits(unsigned id, unsigned thread) { return maxCommits_[id][thread];}
  static testEndMonitor& getInstance();
  static void            deleteInstance();
  bool cleanEnd();
  void setEndOnWfi();
  void checkEndOnWfi();
 private:
  bool getDoneMaxions();
  void drop_objection (int result, unsigned id, unsigned tid, uint64_t cycle);
  void drop_all_objections_if_done(uint64_t cycle);
  static testEndMonitor *pInstance;
  int systemCount_; 
  bool systemDone_; 
  bool done_[NR_MINIONS][NR_THREADS];
  bool done_maxion_[NUM_MAXIONS];
  bool maxCommits_[NR_MINIONS][NR_THREADS];
  bool wfi_[NR_MINIONS][NR_THREADS];
  bool enabled_valid_[NR_MINIONS];
  bool enabled_[NR_MINIONS][NR_THREADS];
  bool power_on_[NR_MINIONS];
  bool dropped_all_objections_;
  bool endOnWfi_;
  uint64_t mask_[NUM_SHIRES];
  unsigned pgetAsWFIMask_;
};
#endif
