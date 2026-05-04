`ifndef _MONITOR_DEFINES_
`define _MONITOR_DEFINES_

// common defines for monitors

// RTL path definitions
`define MINION_CORETOP    minion.core
`define MINION_CORE       minion.core.intpipe
`define MINION_DCACHE     minion.core.dcache
`define MINION_CSR        minion.core.intpipe.csr_file
`define MINION_DCACHE_MH0 minion.core.dcache.miss_handler_unit.handlers[0].miss_handler
`define MINION_DCACHE_MH1 minion.core.dcache.miss_handler_unit.handlers[1].miss_handler
`define MINION_DCACHE_TXS minion.core.dcache.texsend

// VPU
`define MINION_VPU        minion.vpu
`define MINION_VPU_CTRL   `MINION_VPU.ctrl
`define MINION_VPU_REDUCE `MINION_VPU_CTRL.ml.tensorreduce

// Monitors
`define ARCH_STATE_MON    minion.arch_state_mon
`define TESTEND_MON       minion.testend_mon
`define PERF_EVENT_MON    minion.perf_event_monitor
`define DREAMS_EVENT_MON  minion.dreams_event_monitor

`ifndef EVL_THREAD_FAIL
   `define EVL_THREAD_FAIL 32'h50BAD000
`endif
`ifndef EVL_THREAD_PASS
   `define EVL_THREAD_PASS 32'h1FEED000
`endif

`ifndef REG_VALIDATION0
   `define REG_VALIDATION0 12'h8d0
   `define REG_VALIDATION1 12'h8d1
   `define REG_VALIDATION2 12'h8d2
   `define REG_VALIDATION3 12'h8d3
`endif

`endif // _MONITOR_DEFINES_
