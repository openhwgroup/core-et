/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "testLog.h"
#include "plusArgs.h"

#include "vpi_user.h"


// static members of testLog
logLevel testLog::globalLogLevel_  = LOG_DEBUG;
logLevel testLog::defaultLogLevel_ = LOG_INFO;
bool testLog::logLevelsSet_ = false;
unsigned testLog::errors_ = 0;
unsigned testLog::maxErrors_ = 100000000;
int testLog::use_evl_lib_ = 0;

long long unsigned int fake_time;

// define certain functions for verilator ( they come from DPI in VCS)
#ifdef VERILATOR
void endSim() {
  vl_finish(__FILE__, __LINE__, "c++");
}
bool simEnded(){
  return false;
}
#endif

int testLog::sendEVLMessage(logLevel, std::string) { return 0; }


uint64_t testLog::simTime() {
#ifdef VERILATOR
  return (uint64_t) sc_time_stamp()*1000;
#else
  #ifdef ZEBU_DPI
    return (fake_time++);
  #else
    s_vpi_time t;
    t.type = vpiSimTime;
    vpi_get_time( NULL, &t);
    return (uint64_t(t.low) | (uint64_t(t.high)<<32));
  #endif
#endif
}

std::string testLog::simTimeStr() {
  return std::to_string(simTime());
}


void testLog::setLogLevels() {
   #if !defined(ZEBU_DPI) && !defined(VERILATOR)
    int  logLvl;
    bool globalFound = false;

    if ( valuePlusArgs("GLOBAL_LOG_LEVEL", "%i", &logLvl) ) {
      globalLogLevel_ =  (logLevel) logLvl;
      globalFound = true;
    }

    if ( valuePlusArgs("DEFAULT_LOG_LEVEL", "%i", &logLvl) ) {
      defaultLogLevel_ = (logLevel) logLvl;
    } else if (globalFound) {
      defaultLogLevel_ = getGlobalLogLevel();
    }
  #endif

  // cannot mask errors or fatal
  if (globalLogLevel_  > LOG_ERR) globalLogLevel_  = LOG_ERR;
  if (defaultLogLevel_ > LOG_ERR) defaultLogLevel_ = LOG_ERR;

  logLevelsSet_ = true;
}

#ifdef ZEBU_DPI
  unsigned testLog::simCycle() {
    return fake_time++;
  }
#else
  unsigned testLog::simCycle() {
    return time2Cycles(simTime());
  }
#endif
