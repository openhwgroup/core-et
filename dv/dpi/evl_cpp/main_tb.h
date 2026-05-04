/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MAIN_TB_
#define _MAIN_TB_

// Local
#include "testBase.h"
#include "testEndMonitor.h"
#include "cosim.h"
#include "checker.h"
#include "log.h"
#include "events/dispatcher.h"

#include "vlog_defines.h"
// STD
#include <vector>

#ifdef __cplusplus
#define EXTERN_CPP extern "C"
#else
#define EXTERN_CPP extern
#endif

#ifdef ENABLE_UEVENT
#include <uevent/uevent.h>
#endif

class main_tb : public testMain {
public:

  // Constructor, destructor
  main_tb();
  ~main_tb();

  void reset();

  // Sim
  void sim();

#ifdef ENABLE_UEVENT
  uevent_trace* trace = nullptr;
#endif

};

#endif // _MAIN_TB_
