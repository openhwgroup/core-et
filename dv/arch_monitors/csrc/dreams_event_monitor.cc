/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "math.h"
#include "dreams_event_monitor.h"

DPI_IMPORT_NOCONTEXT(void, event_min_retire, (double cycle, unsigned thread_id_, svBitVecVal *pc_, svBitVecVal *inst_bits_),
                     DreamsEventMonitor, event_min_retire, (cycle, thread_id_, pc_, inst_bits_));

DPI_IMPORT_NOCONTEXT(void,  event_tfma_change,   (double cycle, unsigned thread_id, bool val),
                     DreamsEventMonitor, event_tfma_change,   (cycle, thread_id, val) );

DPI_IMPORT_NOCONTEXT(void,  event_tload_req,     (double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_),
                     DreamsEventMonitor, event_tload_req,     (cycle, thread_id, unit, ready, address_, id_) );

DPI_IMPORT_NOCONTEXT(void,  event_tload_resp,    (double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_),
                     DreamsEventMonitor, event_tload_resp,    (cycle, thread_id, unit, ready, address_, id_) );

DPI_IMPORT_NOCONTEXT(void,  event_tquant_change, (double cycle, unsigned thread_id, bool value),
                     DreamsEventMonitor, event_tquant_change, (cycle, thread_id, value) );

DPI_IMPORT_NOCONTEXT(void,  event_tstore_req,    (double cycle, unsigned thread_id, bool ready, svBitVecVal * address_),
                     DreamsEventMonitor, event_tstore_req,    (cycle, thread_id, ready, address_) );

DPI_IMPORT_NOCONTEXT(void,  event_cacheop_req,   (double cycle, unsigned thread_id, bool ready, svBitVecVal * address_, svBitVecVal * id_),
                     DreamsEventMonitor, event_cacheop_req,   (cycle, thread_id, ready, address_, id_) );

DPI_IMPORT_NOCONTEXT(void,  event_cacheop_resp,  (double cycle, unsigned thread_id, svBitVecVal * id_),
                     DreamsEventMonitor, event_cacheop_resp,  (cycle, thread_id, id_) );

DreamsEventMonitor& DreamsEventMonitor::getInstance() {
    static DreamsEventMonitor instance;
    return instance;
}

DreamsEventMonitor::DreamsEventMonitor() : testBase("DreamsEventMonitor") {}
DreamsEventMonitor::~DreamsEventMonitor()  {}

////////////////////////////////////////////////////////////////
// DPI Events
///////////////////////////////////////////////////////////////

// New instruction retired
void DreamsEventMonitor::event_min_retire(double cycle, unsigned thread_id, svBitVecVal * pc_, svBitVecVal * inst_bits_)
{
   uint64_t pc        = dpi2cpp(pc_,        0, 48);
   uint32_t inst_bits = dpi2cpp(inst_bits_, 0, 32);

   char str[128] = "";
   checker::emu_disasm(str, 128, inst_bits);
   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 1 %i %012llx %08x %s\n", cycle_int, thread_id, pc, inst_bits, str);
}

// TFMA valid change
void DreamsEventMonitor::event_tfma_change(double cycle, unsigned thread_id, bool val)
{
   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 2 %i %i\n", cycle_int, thread_id, val);
}

// TLoad requested
void DreamsEventMonitor::event_tload_req(double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_)
{
   uint64_t address = dpi2cpp(address_, 0, 40);
   uint32_t id      = dpi2cpp(id_,     0, 16);
   uint32_t event   = 3 + (unit ? 1 : 0);

   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu %i %i %010llx %04x %i\n", cycle_int, event, thread_id, address, id, ready);
}

// TLoad response
void DreamsEventMonitor::event_tload_resp(double cycle, unsigned thread_id, bool unit, bool ready, svBitVecVal * address_, svBitVecVal * id_)
{
   uint64_t address = dpi2cpp(address_, 0, 40);
   uint32_t id      = dpi2cpp(id_,   0, 16);
   uint32_t event   = 5 + (unit ? 1 : 0);

   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu %i %i %010llx %04x %i\n", cycle_int, event, thread_id, address, id, ready);
}

// TQuant change
void DreamsEventMonitor::event_tquant_change(double cycle, unsigned thread_id, bool value)
{
   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 7 %i %i\n", cycle_int, thread_id, value);
}

// TStore requested
void DreamsEventMonitor::event_tstore_req(double cycle, unsigned thread_id, bool ready, svBitVecVal * address_)
{
   uint64_t address = dpi2cpp(address_, 0, 40);

   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 8 %i %010llx %i\n", cycle_int, thread_id, address, ready);
}

// CacheOp requested
void DreamsEventMonitor::event_cacheop_req(double cycle, unsigned thread_id, bool ready, svBitVecVal * address_, svBitVecVal * id_)
{
   uint64_t address = dpi2cpp(address_, 0, 40);
   uint32_t id      = dpi2cpp(id_,     0, 16);

   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 9 %i %010llx %04x %i\n", cycle_int, thread_id, address, id, ready);
}

// CacheOp response
void DreamsEventMonitor::event_cacheop_resp(double cycle, unsigned thread_id, svBitVecVal * id_)
{
   uint32_t id    = dpi2cpp(id_,   0, 16);

   uint64_t cycle_int = floor(cycle + 0.25); // Floors, add 0.25 extra skew in case clock is jittering on .0

   printf("EVENT %llu 10 %i %04x\n", cycle_int, thread_id, id);
}
