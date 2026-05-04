// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _MINION_DEFINES_
`define _MINION_DEFINES_

////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

`define N 1'b0
`define Y 1'b1

// Synthesis and PTPX tests do not have to use minion validation CSR. ifndef is
// needed becasue CSR CSV ($RTLROOT/shire/minion/intpipe/csr/scripts/csr.csv) makes
// use of ifdef to instantiate the CSRs. This only affects PTPX and synthesis
`ifndef ET_SIMULATION
    `define NOT_VERIF_SIMULATION
`endif

// Integer Register defines
`define XREG_SIZE  64
`define XREG_MSB   (`XREG_SIZE-1)
`define XREG_RANGE `XREG_MSB:0
`define WREG_SIZE  32
`define WREG_MSB   (`WREG_SIZE-1)
`define WREG_RANGE `WREG_MSB:0

// Address sizes
`define XREG_ADDR_SIZE  5
`define XREG_ADDR_RANGE (`XREG_ADDR_SIZE-1):0
`define FREG_ADDR_SIZE  5
`define FREG_ADDR_RANGE (`FREG_ADDR_SIZE-1):0
`define MREG_ADDR_SIZE  3
`define MREG_ADDR_RANGE (`MREG_ADDR_SIZE-1):0

// Instruction fields
`define INST_SIZE  32
`define INST_RANGE (`INST_SIZE-1):0

// PC
`define PC_SIZE  48
`define PC_RANGE (`PC_SIZE-1):0

// extended PC/VA => adding 1 bit: if bits 48..64 are all equal, then bit47, else !bit47
`define PC_SIZE_EXT  (`PC_SIZE+1)
`define PC_RANGE_EXT (`PC_SIZE_EXT-1):0

`define VA_SIZE_EXT  (`VA_SIZE+1)
`define VA_EXT_MSB   (`VA_SIZE_EXT-1)
`define VA_RANGE_EXT (`VA_EXT_MSB):0
`define VA_RANGE_CA_EXT  `VA_EXT_MSB:6

// VA to PA translation
`define VA_UNTRANS_SIZE 12 // 12 LSB are not translated
`define VA_TRANS_SIZE   (`VA_SIZE-`VA_UNTRANS_SIZE)
`define VA_TRANS_MSB    `VA_MSB
`define VA_TRANS_RANGE  `VA_TRANS_MSB:`VA_UNTRANS_SIZE
`define PA_UNTRANS_SIZE 12 // 12 LSB are not translated
`define PA_TRANS_SIZE   (`PA_SIZE-`PA_UNTRANS_SIZE)
`define PA_TRANS_MSB    `PA_MSB
`define PA_TRANS_RANGE  `PA_TRANS_MSB:`PA_UNTRANS_SIZE

// VA and PA cacheline addresses
`define PA_CLINE_SIZE  (`PA_SIZE-6)
`define PA_CLINE_MSB   `PA_MSB
`define PA_CLINE_RANGE `PA_CLINE_MSB:6

// PTW
`define PTW_CACHE_DEPTH 8
`define PTW_PTE_BYTES 8
`define PTW_SV39_PG_LEVELS 3 // Sv39: Page-Based 39-bit Virtual-Memory System
`define PTW_SV48_PG_LEVELS 4 // Sv48: Page-Based 48-bit Virtual-Memory System
`define PTW_PG_IDX_SZ 9

// Core
`define CORE_FCSR_RM_SZ 3
`define CORE_FCSR_FLAG_BITS_SZ 6
`define CORE_INST_DEC_CMD_SZ 5

// Gather scatter counter
`define CORE_GSC_CNT_SIZE  3
`define CORE_GSC_CNT_RANGE (`CORE_GSC_CNT_SIZE-1):0

`define DMEM_REQ_PS_MASK_SZ  8
`define DMEM_RESP_PS_MASK_SZ 8

// Core control to dcache
`define CORE_NR_MSG_PORTS 4
`define CORE_NR_MSG_PORTS_L $clog2(`CORE_NR_MSG_PORTS)
`define CORE_NR_MSG_PORTS_RANGE  (`CORE_NR_MSG_PORTS_L-1):0

// Gather/scatter on 32-byte blocks indices
`define CORE_GSC32_IDX_SIZE 5
`define CORE_GSC32_IDX_RANGE (`CORE_GSC32_IDX_SIZE-1):0

// used bits in exception or interrupt causes
`define CORE_USED_XCPT_CAUSE_BITS 5
`define CORE_XCPT_CAUSE_BITS_RANGE (`CORE_USED_XCPT_CAUSE_BITS-1):0

// Dcache sizes
// ------------
// Dcache block size
`define DCACHE_LINE_SIZE             512
`define DCACHE_LINE_ADDR_LSB         $clog2(`DCACHE_LINE_SIZE/8)
`define DCACHE_DATA_SIZE             256
`define DCACHE_DATA_RANGE            (`DCACHE_DATA_SIZE-1):0
`define DCACHE_DATA_DWORD_SIZE       64
`define DCACHE_DATA_NUM_DWORDS       (`DCACHE_DATA_SIZE/`DCACHE_DATA_DWORD_SIZE)
`define DCACHE_DATA_NUM_DWORDS_RANGE (`DCACHE_DATA_NUM_DWORDS-1):0
`define DCACHE_DATA_DWORD_RANGE      (`DCACHE_DATA_DWORD_SIZE-1):0
`define DCACHE_BLOCK_ADDR_RANGE      $clog2(`DCACHE_DATA_SIZE/8) +: $clog2(`DCACHE_LINE_SIZE/`DCACHE_DATA_SIZE)
`define DCACHE_BLOCK_ADDR_RANGE2     ($clog2(`DCACHE_LINE_SIZE/`DCACHE_DATA_SIZE)-1):0
`define DCACHE_BLOCK_ADDR_LSB        $clog2(`DCACHE_DATA_SIZE/8)
`define DCACHE_DATA_NUM_WORDS        (`DCACHE_DATA_SIZE/32)
`define DCACHE_DATA_NUM_WORDS_RANGE  (`DCACHE_DATA_NUM_WORDS-1):0
`define DCACHE_DATA_WORD_SIZE        32
`define DCACHE_DATA_WORD_RANGE       (`DCACHE_DATA_WORD_SIZE-1):0
`define DCACHE_DATA_NUM_BYTES        (`DCACHE_DATA_SIZE/8)
`define DCACHE_DATA_NUM_BYTES_RANGE  (`DCACHE_DATA_NUM_BYTES-1):0
`define DCACHE_DATA_HWORD_SIZE       16
`define DCACHE_DATA_BYTE_SIZE        8

// Sets
`define DCACHE_SETS            16
`define DCACHE_SET_RANGE       ($clog2(`DCACHE_SETS)-1):0
`define DCACHE_SET_ADDR_RANGE  ($clog2(`DCACHE_SETS)-1+6):6
`define DCACHE_SET_SZ          $clog2(`DCACHE_SETS)

// Sets (reduced)
`define DCACHE_SETS_RED            2
`define DCACHE_SET_RED_RANGE       ($clog2(`DCACHE_SETS_RED)-1):0
`define DCACHE_SET_RED_ADDR_RANGE  ($clog2(`DCACHE_SETS_RED)-1+6):6
`define DCACHE_SET_RED_SZ          $clog2(`DCACHE_SETS_RED)

`define DCACHE_SET_MSB_SZ          $clog2(`DCACHE_SETS/`DCACHE_SETS_RED)
`define DCACHE_SET_MSB_RANGE       ($clog2(`DCACHE_SETS)-1):$clog2(`DCACHE_SETS_RED)

`define DCACHE_SETS_SCP_SPLIT      14
`define DCACHE_SETS_FOR_SCP        12

// Ways
`define DCACHE_WAYS            4
`define DCACHE_WAY_RANGE       ($clog2(`DCACHE_WAYS)-1):0
`define DCACHE_WAY_ADDR_RANGE  ($clog2(`DCACHE_WAYS)-1):0
`define DCACHE_WAY_SZ          $clog2(`DCACHE_WAYS)

`define DCACHE_SET_WAY_RANGE   ($clog2(`DCACHE_SETS)+$clog2(`DCACHE_WAYS)-1):0

// Replay queue
`define DCACHE_REPLAYQ_SIZE    8

// SRAM address (control which byte of the SRAMs we write)
`define DCACHE_LRAM_ADDR_SIZE  ($clog2(`DCACHE_SETS)+6)
`define DCACHE_LRAM_ADDR_MSB   (`DCACHE_LRAM_ADDR_SIZE-1)
`define DCACHE_LRAM_ADDR_RANGE `DCACHE_LRAM_ADDR_MSB:0

// Number of handshake interfaces for miss request L2 interface
`define DCACHE_L2_MISS_REQ_PORTS 3
`define DCACHE_L2_MISS_IDX_TENB  0 // TLoads to TenB
`define DCACHE_L2_MISS_IDX_SCP   1 // TLoads to Scp
`define DCACHE_L2_MISS_IDX_COMM  2 // Regular requests

// Number of handshake interfaces for evict request L2 interface
`define DCACHE_L2_EVICT_REQ_PORTS  2
`define DCACHE_L2_EVICT_IDX_TSTORE 0
`define DCACHE_L2_EVICT_IDX_COMM   1

// CSR
// ---
`define CSR_SATP_MODE_SZ 4
`define CSR_SATP_ASID_SZ 4
`define CSR_SATP_PPN_SZ 44

// Paged virtual-memory modes
`define CSR_SATP_MODE_BARE 4'd0
`define CSR_SATP_MODE_SV39 4'd8
`define CSR_SATP_MODE_SV48 4'd9

// Cache Control
`define CSR_XCACHE_CTRL_SZ 9
`define CSR_UCACHE_CTRL_SZ 11
`define CSR_MCACHE_CTRL_SZ 2

`define CSR_UCACHE_CTRL_RESET_VAL `CSR_UCACHE_CTRL_SZ'h200

// VPU
// ---
`define VPU_LANE_NUM   8
`define VPU_LANE_RANGE (`VPU_LANE_NUM-1):0
`define VPU_DATA_S_SZ  32
`define VPU_DATA_SZ    (`VPU_LANE_NUM*`VPU_DATA_S_SZ)
`define VPU_DATA_RANGE (`VPU_DATA_SZ-1):0
`define CORE_VPU_STORE_DATA_SZ `VPU_DATA_SZ
`define VPU_FP_SCOREBOARD_SIZE (`VPU_TRANS_SCOREBOARD_SIZE + 2)          // Number of entries that scoreboard in core needs to look at
`define VPU_TRANS_SCOREBOARD_SIZE 12                                     // Number of entries for trans scoreboard
`define VPU_TOINT_LAT             8                                      // From F2 until result is ready
`define VPU_MASK_LAT              8                                      // From F2 until result is ready
`define VPU_TOINT_SCOREBOARD_SIZE (`VPU_TOINT_LAT-4)                     // Number of entries that scoreboard in core needs to look at
`define VPU_MASK_SCOREBOARD_SIZE (`VPU_MASK_LAT-4)                       // Number of entries that scoreboard in core needs to look at
`define CORE_GSC32_IDX_V_SIZE (`CORE_GSC32_IDX_SIZE*`VPU_LANE_NUM)
`define CORE_GSC32_IDX_V_RANGE (`CORE_GSC32_IDX_V_SIZE-1):0


// Regmask
`define VPU_REGMASK_NUM 8

// Fast Local Barrier
`define CSR_FL_BARRIER_SIZE  13
`define CSR_FL_BARRIER_RANGE (`CSR_FL_BARRIER_SIZE-1):0
`define CSR_FL_NUM_REGISTERS 32

`define CSR_FL_BARRIER_NUM_RANGE   4:0
`define CSR_FL_BARRIER_LIMIT_RANGE 12:5

// Tensor Wait condition field
`define CSR_TENSOR_WAIT_ID_R 3:0

// Tensor Mask
`define TENSOR_MASK_BITS  16
`define TMASK_COUNT_BITS  4
`define TMASK_COUNT_RANGE (`TMASK_COUNT_BITS-1):0

// Short names
`define CORE_FUNC_X       core_alu_func_NONE
`define CORE_FUNC_ADD     core_alu_func_ADD
`define CORE_FUNC_SL      core_alu_func_SL
`define CORE_FUNC_SEQ     core_alu_func_SEQ
`define CORE_FUNC_SNE     core_alu_func_SNE
`define CORE_FUNC_XOR     core_alu_func_XOR
`define CORE_FUNC_SR      core_alu_func_SR
`define CORE_FUNC_OR      core_alu_func_OR
`define CORE_FUNC_AND     core_alu_func_AND
`define CORE_FUNC_SUB     core_alu_func_SUB
`define CORE_FUNC_SRA     core_alu_func_SRA
`define CORE_FUNC_SLT     core_alu_func_SLT
`define CORE_FUNC_SGE     core_alu_func_SGE
`define CORE_FUNC_SLTU    core_alu_func_SLTU
`define CORE_FUNC_SGEU    core_alu_func_SGEU
`define CORE_FUNC_PACKB   core_alu_func_PACKB
`define CORE_FUNC_BITMIXB core_alu_func_BITMIXB
`define CORE_FUNC_MUL     core_alu_func_MUL
`define CORE_FUNC_MULH    core_alu_func_MULH
`define CORE_FUNC_MULHSU  core_alu_func_MULHSU
`define CORE_FUNC_MULHU   core_alu_func_MULHU
`define CORE_FUNC_DIV     core_alu_func_DIV
`define CORE_FUNC_DIVU    core_alu_func_DIVU
`define CORE_FUNC_REM     core_alu_func_REM
`define CORE_FUNC_REMU    core_alu_func_REMU

// Short names
`define A1_X    core_a1_ctrl_X
`define A1_ZERO core_a1_ctrl_ZERO
`define A1_RS1  core_a1_ctrl_RS1
`define A1_PC   core_a1_ctrl_PC
`define A1_FS1  core_a1_ctrl_FS1

// Short names
`define A2_X    core_a2_ctrl_X
`define A2_ZERO core_a2_ctrl_ZERO
`define A2_SIZE core_a2_ctrl_SIZE
`define A2_RS2  core_a2_ctrl_RS2
`define A2_IMM  core_a2_ctrl_IMM

// Short names
`define IMM_X  core_imm_ctrl_X
`define IMM_S  core_imm_ctrl_S
`define IMM_SB core_imm_ctrl_SB
`define IMM_U  core_imm_ctrl_U
`define IMM_UJ core_imm_ctrl_UJ
`define IMM_I  core_imm_ctrl_I
`define IMM_Z  core_imm_ctrl_Z

// Short names
`define DW_X   core_dw_ctrl_X  
`define DW_32  core_dw_ctrl_32 
`define DW_64  core_dw_ctrl_64 
`define DW_XPR core_dw_ctrl_64

// Short names
`define CSR_X core_csr_ctrl_X
`define CSR_N core_csr_ctrl_N
`define CSR_W core_csr_ctrl_W
`define CSR_S core_csr_ctrl_S
`define CSR_C core_csr_ctrl_C
`define CSR_I core_csr_ctrl_I
`define CSR_R core_csr_ctrl_R

// TLB FSM states (short name)
`define TLB_S_READY   tlb_state_ready
`define TLB_S_REQUEST tlb_state_request
`define TLB_S_WAIT    tlb_state_wait

// Short names
`define PTW_S_READY      ptw_state_ready
`define PTW_S_REQ        ptw_state_req
`define PTW_S_WAIT       ptw_state_wait
`define PTW_S_SET_DIRTY  ptw_state_set_dirty
`define PTW_S_WAIT_DIRTY ptw_state_wait_dirty
`define PTW_S_DONE       ptw_state_done

// Memory commands (short name)
`define M_X         5'bx
`define M_XRD       dcache_cmd_XRD
`define M_XWR       dcache_cmd_XWR
`define M_XWRB      dcache_cmd_XWRB
`define M_XRDB      dcache_cmd_XRDB
`define M_PFR       dcache_cmd_PFR
`define M_PFW       dcache_cmd_PFW
`define M_XA_SWAP   dcache_cmd_XA_SWAP
`define M_FLUSH_ALL dcache_cmd_FLUSH_ALL
`define M_XLR       dcache_cmd_XLR
`define M_XSC       dcache_cmd_XSC
`define M_XA_ADD    dcache_cmd_XA_ADD
`define M_XA_XOR    dcache_cmd_XA_XOR
`define M_XA_OR     dcache_cmd_XA_OR
`define M_XA_AND    dcache_cmd_XA_AND
`define M_XA_MIN    dcache_cmd_XA_MIN
`define M_XA_MAX    dcache_cmd_XA_MAX
`define M_XA_MINU   dcache_cmd_XA_MINU
`define M_XA_MAXU   dcache_cmd_XA_MAXU
`define M_XA_MIN_PS dcache_cmd_XA_MIN_PS
`define M_XA_MAX_PS dcache_cmd_XA_MAX_PS
`define M_XA_CMPSWP dcache_cmd_XA_CMPSWP
`define M_FLUSH     dcache_cmd_FLUSH
`define M_PRODUCE   dcache_cmd_PRODUCE
`define M_CLEAN     dcache_cmd_CLEAN
`define M_G32       dcache_cmd_G32
`define M_SC32      dcache_cmd_SC32

// defines for debug
`define MINION_DEBUG_APB_D_WIDTH 64
`define MINION_DEBUG_APB_AD_WIDTH 10

`define MINION_DEBUG_FFB_WORDS 4
`define FIRST_MINION_DEBUG_REG minion_debug_nxdata0
`define MINION_DEBUG_APB_REGS_AD_WIDTH  4
`define MINION_DEBUG_APB_REGS_SEL_RANGE  minion_debug_invalid:`FIRST_MINION_DEBUG_REG
`define MINION_DEBUG_APB_REGS_AD_RANGE  $bits(minion_debug_apb_regs_t)-1:0

`define MINION_DEBUG_APB_ADDR_UPPER_BITS 1
`define MINION_DEBUG_APB_ADDR_UPPER_RANGE (`MIN_DM_APB_ADDR_WIDTH-1):(`MIN_DM_APB_ADDR_WIDTH-`MINION_DEBUG_APB_ADDR_UPPER_BITS)
`define MINION_DEBUG_APB_ADDR_DEBUG  `MINION_DEBUG_APB_ADDR_UPPER_BITS'd0
`define MINION_DEBUG_APB_ADDR_DCACHE `MINION_DEBUG_APB_ADDR_UPPER_BITS'd1

// Tensor TMP registers
`define VPU_TENA_SIZE 2
`define VPU_TENA_ADDR ($clog2(`VPU_TENA_SIZE)-1):0
`define VPU_TENB_SIZE 8
`define VPU_TENB_ADDR ($clog2(`VPU_TENB_SIZE)-1):0
`define VPU_TENB_CREDIT_BITS  $clog2(`VPU_TENB_SIZE)
`define VPU_TENB_CREDIT_RANGE (`VPU_TENB_CREDIT_BITS-1):0
`define VPU_TENB_RCV_ENTRY_BITS  (`VPU_TENB_SIZE/2)
`define VPU_TENB_RCV_ENTRY_RANGE (`VPU_TENB_RCV_ENTRY_BITS-1):0
`define VPU_TENC_SIZE 16
`define VPU_TENC_ADDR $clog2(`VPU_TENC_SIZE):1
`define VPU_TENTMP_SIZE 8
`define VPU_TENTMP_ADDR ($clog2(`VPU_TENTMP_SIZE)-1):0

// clock gating for inst bits

// default bits to propagate
`define EX_INST_BITS_FF_MASK      `INST_SIZE'hf80  
`define TAG_INST_BITS_FF_MASK     `INST_SIZE'hf80  
`define MEM_INST_BITS_FF_MASK     `INST_SIZE'hf80       
`define WB_INST_BITS_FF_MASK      `INST_SIZE'hf80

// bits propagated for jumps or branches
`define EX_INST_BITS_FF_BR_MASK      `INST_SIZE'hffffff80
`define TAG_INST_BITS_FF_BR_MASK     `INST_SIZE'hffffff80
`define MEM_INST_BITS_FF_BR_MASK     `INST_SIZE'h0       
`define WB_INST_BITS_FF_BR_MASK      `INST_SIZE'h0

// Memory override bits
`define MINION_MEM_CTRL_BITS 2
`define MINION_MEM_CTRL_WE   1
`define MINION_MEM_CTRL_RE   0

`define ID_CTRL_STALL_CNT_SZ 3

// Implement DCache that reports PC of instruction that received the bus error
`ifdef ET_SIMULATION
`define DCACHE_REPORT_PC
`endif

`endif // _MINION_DEFINES_

