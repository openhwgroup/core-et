// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0



typedef enum logic [4:0]
{
    // PMU defined events
    pmu_event_NOCOUNT          = 5'd0,  // No count
    pmu_event_MCYCLES          = 5'd1,  // Mcycles 
    pmu_event_RETIRED_INST0    = 5'd2,  // Retired Inst TH0
    pmu_event_RETIRED_INST1    = 5'd3,  // Retired Inst TH1
    pmu_event_BRANCHES0        = 5'd4,  // Branches taken TH0
    pmu_event_BRANCHES1        = 5'd5,  // Branches taken TH1
    
    pmu_event_DCACHE_ACCESS0   = 5'd6,  // Dcache Accesses TH0
    pmu_event_DCACHE_ACCESS1   = 5'd7,  // Dcache Accesses TH1
    pmu_event_DCACHE_MISSES0   = 5'd8,  // Dcache Misses TH0
    pmu_event_DCACHE_MISSES1   = 5'd9,  // Dcache Misses TH1
    pmu_event_L2_MISS_REQ      = 5'd10, // L2 Miss requests sent
    pmu_event_L2_MISS_REQ_REJ  = 5'd11, // L2 Miss requests rejected
    pmu_event_L2_EVICT_REQ     = 5'd12, // L2 Evict requests sent
    pmu_event_L2_EVICT_REQ_REJ = 5'd13, // L2 Evict requests rejected
    pmu_event_TL_INST          = 5'd14, // Tensor Load Instruction
    pmu_event_TL_OPS           = 5'd15, // Tensor Load Operation
    pmu_event_TS_INST          = 5'd16, // Tensor Store Instruction
    pmu_event_TS_OPS           = 5'd17, // Tensor Store Operation
    
    pmu_event_TFMA_WAIT_TENB   = 5'd18, // Tensor FMA waiting TENB
    pmu_event_TIMA_OPS         = 5'd19, // TIMA Operation
    pmu_event_TXFMA_3216_OPS   = 5'd20, // TXFMA Operation fp16a32
    pmu_event_TXFMA_32_OPS     = 5'd21, // TXFMA Operation fp32
    pmu_event_TXFMA_INT_OPS    = 5'd22, // TXFMA Operation int
    pmu_event_TRANS_OPS        = 5'd23, // TRANS Operation
    pmu_event_SHORT_OPS        = 5'd24, // SHORT Operation
    pmu_event_MASK_OPS         = 5'd25, // MASK Operation
    pmu_event_TFMA_INST        = 5'd26, // Tensor Fma Instruction
    pmu_event_TREDUCE_INST     = 5'd27, // Tensor Reduce Instruction
    pmu_event_TQUANT_INST      = 5'd28, // Tensor Quant Instruction
    
    pmu_event_UNUSED3          = 5'd29, // 
    pmu_event_UNUSED4          = 5'd30, // 
    pmu_event_UNUSED5          = 5'd31  // 
} pmu_event_t;
