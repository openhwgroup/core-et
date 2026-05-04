/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _DREAMS_EVENT_MONITOR_H_
#define _DREAMS_EVENT_MONITOR_H_

#include "testBase.h"
#include "checker.h"
#include <stdlib.h>
#include <stdio.h>

class DreamsEventMonitor : public testBase {
   public:
      DreamsEventMonitor();
      ~DreamsEventMonitor();

      // DPI Events
      void event_min_retire   (double cycle, unsigned thread_id, svBitVecVal * pc_, svBitVecVal * inst_bits_);
      void event_tfma_change  (double cycle, unsigned thread_id, bool val);
      void event_tload_req    (double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_);
      void event_tload_resp   (double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_);
      void event_tquant_change(double cycle, unsigned thread_id, bool value);
      void event_tstore_req   (double cycle, unsigned thread_id, bool ready, svBitVecVal * address_);
      void event_cacheop_req  (double cycle, unsigned thread_id, bool ready, svBitVecVal * address_, svBitVecVal * id_);
      void event_cacheop_resp (double cycle, unsigned thread_id, svBitVecVal * id_);

      static DreamsEventMonitor& getInstance();
};

#endif
