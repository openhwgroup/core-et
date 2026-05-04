// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

////////////////////////////////////////////////////////////////
// types
////////////////////////////////////////////////////////////////
typedef logic [1:0] prv_t; // TODO: Enum

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

// Instruction function
//
typedef enum logic [4:0]
{
    // ALU pipe operations
    core_alu_func_NONE    = 5'b1xxxx, // Don't care
    core_alu_func_ADD     = 5'b00000, // Add
    core_alu_func_SL      = 5'b00001, // Shift left
    core_alu_func_SEQ     = 5'b00010, // Signed equal
    core_alu_func_SNE     = 5'b00011, // Signed not equal
    core_alu_func_XOR     = 5'b00100, // Xor
    core_alu_func_SR      = 5'b00101, // Shift right
    core_alu_func_OR      = 5'b00110, // Or
    core_alu_func_AND     = 5'b00111, // And
    core_alu_func_PACKB   = 5'b01000, // Pack byte
    core_alu_func_BITMIXB = 5'b01001, // Bitmix byte
    core_alu_func_SUB     = 5'b01010, // Sub
    core_alu_func_SRA     = 5'b01011, // Shift right arithmentic
    core_alu_func_SLT     = 5'b01100, // Signed less than
    core_alu_func_SGE     = 5'b01101, // Signed greater equal
    core_alu_func_SLTU    = 5'b01110, // Unsigned less than 
    core_alu_func_SGEU    = 5'b01111, // Unsigned greater equal
    core_alu_func_MUL     = 5'b10000, // Mul
    core_alu_func_MULH    = 5'b10001, // Mul high signed
    core_alu_func_DIV     = 5'b10100, // Signed div
    core_alu_func_DIVU    = 5'b10101, // Unsigned div
    core_alu_func_REM     = 5'b10110, // Signed reminder
    core_alu_func_REMU    = 5'b10111, // Unsigned reminder
    core_alu_func_MULHSU  = 5'b11100, // Mul high signed/unsigned
    core_alu_func_MULHU   = 5'b11110  // Mul high unsigned
} core_alu_func;

// ALU source1 select
typedef enum logic [1:0]
{
    core_a1_ctrl_X    = 2'bxx, // Don't care
    core_a1_ctrl_ZERO = 2'b00, // Select zero
    core_a1_ctrl_RS1  = 2'b01, // Select RF read port 1
    core_a1_ctrl_PC   = 2'b10, // PC
    core_a1_ctrl_FS1  = 2'b11  // Floating data
} core_a1_ctrl;


// ALU source2 select
typedef enum logic [1:0]
{
    core_a2_ctrl_X    = 2'bxx, // Don't care
    core_a2_ctrl_ZERO = 2'b00, // Select zero
    core_a2_ctrl_SIZE = 2'b01, // Size of the instruction
    core_a2_ctrl_RS2  = 2'b10, // Select RF read port 2
    core_a2_ctrl_IMM  = 2'b11  // Immediate
} core_a2_ctrl;

// Immediate select
typedef enum logic [2:0]
{
    core_imm_ctrl_X  = 3'bxxx, // Don't care
    core_imm_ctrl_S  = 3'b000, // S immediate
    core_imm_ctrl_SB = 3'b001, // SB immediate
    core_imm_ctrl_U  = 3'b010, // U immediate
    core_imm_ctrl_UJ = 3'b011, // UJ immediate
    core_imm_ctrl_I  = 3'b100, // I immediate
    core_imm_ctrl_Z  = 3'b101  // Z immediate
} core_imm_ctrl;

// Operation size
typedef enum logic
{
    core_dw_ctrl_X   = 1'bx, // Don't care
    core_dw_ctrl_32  = 1'b0, // 32b
    core_dw_ctrl_64  = 1'b1  // 64b
} core_dw_ctrl;

// CSR types
typedef enum logic [2:0]
{
    core_csr_ctrl_X = 3'bxxx, // Don't care
    core_csr_ctrl_N = 3'b000, // No action
    core_csr_ctrl_W = 3'b001, // Write
    core_csr_ctrl_S = 3'b010, // Set
    core_csr_ctrl_C = 3'b011, // Clear
    core_csr_ctrl_I = 3'b100, // ??
    core_csr_ctrl_R = 3'b101  // Read
} core_csr_ctrl;

// Exception causes

typedef enum logic [`CORE_USED_XCPT_CAUSE_BITS-1:0] {
  // exceptions
  core_xcpt_cause_MISALIGNED_FETCH       = 5'h0,  // Instruction address misaligned
  core_xcpt_cause_FETCH_ACCESS           = 5'h1,  // Instruction access fault      
  core_xcpt_cause_ILLEGAL_INSTRUCTION    = 5'h2,  // Illegal instruction           
  core_xcpt_cause_BREAKPOINT             = 5'h3,  // Breakpoint                    
  core_xcpt_cause_MISALIGNED_LOAD        = 5'h4,  // Load address misaligned       
  core_xcpt_cause_LOAD_ACCESS            = 5'h5,  // Load access fault             
  core_xcpt_cause_MISALIGNED_STORE       = 5'h6,  // Store/AMO address misaligned  
  core_xcpt_cause_STORE_ACCESS           = 5'h7,  // Store/AMO access fault        
  core_xcpt_cause_USER_ECALL             = 5'h8,  // Environment call from U-mode  
  core_xcpt_cause_SUPERVISOR_ECALL       = 5'h9,  // Environment call from S-mode  
  core_xcpt_cause_MACHINE_ECALL          = 5'hb,  // Environment call from M-mode  
  core_xcpt_cause_FETCH_PAGE_FAULT       = 5'hc,  // Instruction page fault        
  core_xcpt_cause_LOAD_PAGE_FAULT        = 5'hd,  // Load page fault
  core_xcpt_cause_STORE_PAGE_FAULT       = 5'hf,  // Store/AMO page fault          
  core_xcpt_cause_FETCH_BUS_ERROR        = 5'h19, // l2 err reported by icache
  core_xcpt_cause_FETCH_ECC_ERROR        = 5'h1a, // double bit error reported by icache
  core_xcpt_cause_LOAD_PAGE_SPLIT_FAULT  = 5'h1b, // Load page-split fault
  core_xcpt_cause_STORE_PAGE_SPLIT_FAULT = 5'h1c, // Store page-split fault
  core_xcpt_cause_MCODE_INSTRUCTION      = 5'h1e, // Instruction to be implemented in m-code
  core_xcpt_cause_DEBUG                  = 5'h1f  // debugger
 } core_xcpt_cause_t;

typedef enum logic [`CORE_USED_XCPT_CAUSE_BITS-1:0] {
  // interrupts
  core_xcpt_cause_MEIP     =  5'd11, //Machine external interrupt
  core_xcpt_cause_SEIP     = 5'd9,   //Supervisor external interrupt
  `ifdef CSR_U_INTERRUPTS 
  core_xcpt_cause_UEIP     = 5'd8,   //User external interrupt
  `endif
  core_xcpt_cause_MSIP     = 5'd3,   //Machine software interrupt
  core_xcpt_cause_SSIP     = 5'd1,   //Supervisor software interrupt
  `ifdef CSR_U_INTERRUPTS 
  core_xcpt_cause_USIP     = 5'd0,   //User software interrupt
  `endif
  core_xcpt_cause_MTIP     = 5'd7,   //Machine timer interrupt
  core_xcpt_cause_STIP     = 5'd5,   //Supervisor timer interrupt
  `ifdef CSR_U_INTERRUPTS 
  core_xcpt_cause_UTIP     = 5'd4,   //User timer interrupt
  `endif
 //  core_xcpt_cause_DEBUG    = 5'h1f    // interrupt to enter debug mode (when halt signal is asserted).. code shared with xcpt
  core_xcpt_cause_MBAD_RED   = 5'd16,  // bad trigger redirect
  core_xcpt_cause_MIECO      = 5'd19,  // icache ecc error counter overflow
  core_xcpt_cause_BUS_ERROR  = 5'd23   // dcache bus error

} core_int_cause_t;

typedef logic[`CORE_USED_XCPT_CAUSE_BITS-1:0] core_xcpt_cause;

 typedef enum logic [1:0]
{
    tlb_state_ready   = 2'b00,
    tlb_state_request = 2'b01,
    tlb_state_wait    = 2'b10
} tlb_state;

// PTW states
typedef enum logic [2:0]
{
    ptw_state_ready      = 3'b000,
    ptw_state_req        = 3'b001,
    ptw_state_wait       = 3'b010,
    ptw_state_set_dirty  = 3'b011,
    ptw_state_wait_dirty = 3'b100,
    ptw_state_done       = 3'b101
} ptw_state;

// List of dcache commands
typedef enum logic [4:0]
{
    dcache_cmd_XRD       = 5'b00000, // load
    dcache_cmd_XWR       = 5'b00001, // store
    dcache_cmd_PFR       = 5'b00010, // prefetch with intent to read
    dcache_cmd_PFW       = 5'b00011, // prefetch with intent to write
    dcache_cmd_XA_SWAP   = 5'b00100,
    dcache_cmd_FLUSH_ALL = 5'b00101, // flush all lines
    dcache_cmd_XLR       = 5'b00110,
    dcache_cmd_XSC       = 5'b00111,
    dcache_cmd_XA_ADD    = 5'b01000,
    dcache_cmd_XA_XOR    = 5'b01001,
    dcache_cmd_XA_OR     = 5'b01010,
    dcache_cmd_XA_AND    = 5'b01011,
    dcache_cmd_XA_MIN    = 5'b01100,
    dcache_cmd_XA_MAX    = 5'b01101,
    dcache_cmd_XA_MINU   = 5'b01110,
    dcache_cmd_XA_MAXU   = 5'b01111,
    dcache_cmd_FLUSH     = 5'b10000, // write back dirty data and cede R/W permissions
    dcache_cmd_PRODUCE   = 5'b10001, // write back dirty data and cede W permissions
    dcache_cmd_CLEAN     = 5'b10011, // write back dirty data and retain R/W permissions
    dcache_cmd_XA_CMPSWP = 5'b11000, // atomic compare exchange
    dcache_cmd_G32       = 5'b11001, // gather on 32-byte block
    dcache_cmd_SC32      = 5'b11010, // scatter on 32-byte block
    dcache_cmd_XWRB      = 5'b11011, // store data bypassing L1
    dcache_cmd_XRDB      = 5'b11100, // load data bypassing L1
    dcache_cmd_XA_MIN_PS = 5'b11101,
    dcache_cmd_XA_MAX_PS = 5'b11111
} dcache_cmd;

typedef enum logic [2:0]
{
 debug_bp_load,
 debug_bp_store,
 debug_bp_tensor_load,
 debug_bp_tensor_store,
 debug_bp_amo,
 debug_bp_co
 } debug_bp_t;


// List of dcache types
// bit  [3]   = PS operation (1) / Scalar (0)
// bit  [2]   = gather/scatter for PS, zero extend for scalar
// bits [1:0] = read size => {0,1,2,3}={8,16,32,64/128}
typedef enum logic [3:0]
{
    dcache_type_B       = 4'b0000, // Byte sign extend
    dcache_type_H       = 4'b0001, // Half word sign extend
    dcache_type_W       = 4'b0010, // Word sign extend
    dcache_type_D       = 4'b0011, // DWord
    dcache_type_BU      = 4'b0100, // Byte zero extend
    dcache_type_HU      = 4'b0101, // Half word zero extend
    dcache_type_WU      = 4'b0110, // Word zero extend
    dcache_type_PS      = 4'b1011, // 256-bit
    dcache_type_PS_BR   = 4'b1010, // Broadcast 32b to 128b
    dcache_type_PS_GS8  = 4'b1100, // gather/scatter 8 bits
    dcache_type_PS_GS16 = 4'b1101, // gather/scatter 16 bits
    dcache_type_PS_GS32 = 4'b1110  // gather/scatter 32 bits
} dcache_type;

typedef enum logic [8:0] 
{
 minion_debug_nxdata0       = 9'h0f0,
 minion_debug_nxdata1       = 9'h0f1,
 minion_debug_axdata0       = 9'h0f2,
 minion_debug_axdata1       = 9'h0f3,
 minion_debug_axprogbuf0    = 9'h0f4,
 minion_debug_axprogbuf1    = 9'h0f5,
 minion_debug_nxprogbuf0    = 9'h0f6,
 minion_debug_nxprogbuf1    = 9'h0f7,
 minion_debug_abscmd        = 9'h0f8,
 minion_debug_invalid       = 9'h0f9
} minion_debug_apb_regs_t;

// Virtual page sizes for Sv39/Sv48 supported in the TLB
typedef enum logic[1:0] {
    tlb_entry_type_4K    = 2'h0,
    tlb_entry_type_RSVD  = 2'h1, // Reserved for future use
    tlb_entry_type_2M    = 2'h2,
    tlb_entry_type_1G    = 2'h3
} tlb_entry_type;

////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////

// Dest register info
typedef struct packed
{
    logic                    fp;        // Dest is FP register
    logic [`XREG_ADDR_RANGE] addr;      // Dest register
    logic                    thread_id; // Thread of the register
} minion_reg_dest;

// Dest mask register info
typedef struct packed
{
    logic [`MREG_ADDR_RANGE] addr;      // Dest mask register
    logic                    thread_id; // Thread of the register
} minion_mreg_dest;


// Control signals for an instruction in the minion
typedef struct packed
{
    logic         legal;
    logic         mcode;
    logic         fp;
    logic         br;
    logic         jal;
    logic         jalr;
    logic         rxs2;
    logic         rxs1;
    core_a2_ctrl  sel_alu2;
    core_a1_ctrl  sel_alu1;
    core_imm_ctrl sel_imm;
    core_dw_ctrl  alu_dw;
    core_alu_func alu_fn;
    logic         mem;
    dcache_cmd    mem_cmd;
    dcache_type   mem_type;
    logic         rma;
    logic         wmd;
    logic         wmad;
    logic         wfd;
    logic         div;
    logic         wxd;
    core_csr_ctrl csr;
    logic         fence;
    logic         mem_g;
    logic         gsc;
    logic         gfx;
    logic         x31;
} minion_control;


// subset of fields from mstatus that are used in the intpipetop
typedef struct packed {
   logic       mprv;
   logic [1:0] mpp;
   logic       sum;
   logic       mxr;
   
   logic       fs;
} intpipe_status_t;

// Useful info from satp register
typedef struct packed
{
   logic [`CSR_SATP_MODE_SZ-1:0] mode;
   logic [`PA_TRANS_SIZE-1:0]    ppn;
} minion_satp_info;

// Useful status bits for VM
typedef struct packed {
   logic [1:0] prv;
   logic       mprv;
   logic [1:0] mpp;
   logic       sum;
   logic       mxr;
   logic       debug;
} minion_vm_status;

// Page table walker request
typedef struct packed
{
   logic [`CSR_SATP_MODE_SZ-1:0] satp_mode;
   logic [`PA_TRANS_SIZE-1:0]    satp_ppn;
   logic [1:0]                   prv;
   logic [`VA_TRANS_RANGE]       addr;
} minion_ptw_req;

// Page table entry
typedef struct packed
{
    logic        canceled_req;
    logic        access_fault;
    logic [7:0]  rsvd_for_hw;
    logic [`CSR_SATP_PPN_SZ-1:0] ppn;
    logic [1:0]  rsvd_for_sw;
    logic        d;
    logic        a;
    logic        g;
    logic        u;
    logic        x;
    logic        w;
    logic        r;
    logic        v;
} minion_ptw_pte;

// Minion request to dcache
typedef struct packed
{
    `ifdef DCACHE_REPORT_PC
    logic [`PC_RANGE_EXT]            pc;           // PC of the instruction
    `endif
    logic [`VA_RANGE]                addr;         // Address of the request
    logic                            addr_msb_err; // bits addr[63:48] != addr[47]
    minion_reg_dest                  dest;         // Dest register for loads
    dcache_cmd                       cmd;          // Command: read,write,...
    dcache_type                      typ;          // Type of access (
    logic [`CORE_GSC_CNT_RANGE]      gsc_cnt;      // Gather/scatter element
    logic [`DMEM_REQ_PS_MASK_SZ-1:0] ps_mask;      // Mask for PS ld/st
    logic                            phys;         // Skip TLB
    logic [`CORE_GSC32_IDX_V_RANGE]  gsc32_idx;    // Gather/scatter on 32-byte block indices
    logic                            mem_global;   // Flag indicating if memory access is global or local
} minion_dcache_req;

// Dcache response to core
typedef struct packed
{
    minion_reg_dest                  dest;    // Register destination for loads
    dcache_type                      typ;     // Type of access
    logic [`CORE_GSC_CNT_RANGE]      gsc_cnt; // Gather element
    logic [`DMEM_REQ_PS_MASK_SZ-1:0] ps_mask; // Mask enable for PS accesses
    logic [`DCACHE_DATA_RANGE]       data;    // Data to Core/VPU
    logic [`VA_RANGE_EXT]            addr;    // Address to report exceptions
} dcache_minion_resp;

// Dcache scoreboard signals to core (replayQ)
typedef struct packed
{
    logic           [`DCACHE_REPLAYQ_SIZE-1:0] valid; // Which entries are valid
    minion_reg_dest [`DCACHE_REPLAYQ_SIZE-1:0] dest;  // Dest register
} dcache_minion_scoreboard;

// VPU scoreboard signals to core
typedef struct packed
{
    logic            [`VPU_FP_SCOREBOARD_SIZE-1:0]    fp_valid;    // Which entries are valid
    minion_reg_dest  [`VPU_FP_SCOREBOARD_SIZE-1:0]    fp_dest;     // Dest register
    logic            [`VPU_TOINT_SCOREBOARD_SIZE-1:0] toint_valid; // Which entries are valid
    minion_reg_dest  [`VPU_TOINT_SCOREBOARD_SIZE-1:0] toint_dest;  // Dest register
    logic            [`VPU_MASK_SCOREBOARD_SIZE-1:0]  mask_valid;  // Which entries are valid
    minion_mreg_dest [`VPU_MASK_SCOREBOARD_SIZE-1:0]  mask_dest;   // Dest register
} vpu_minion_scoreboard;

// Dcache response to VPU
typedef struct packed
{
    logic                            thread_id; // Thread ID
    logic [`CORE_GSC_CNT_RANGE]      gdst;      // Gather/Scatter element
    dcache_type                      typ;       // Type of access
    logic [`FREG_ADDR_RANGE]         addr;      // Register dest
    logic [`VPU_DATA_RANGE]          data;      // Load data
    logic [`DMEM_REQ_PS_MASK_SZ-1:0] ps_mask;   // Mask for packed instructions
} dcache_vpu_resp;

// Minion request signals to VPU for EX stage
typedef struct packed
{
    logic                         gscing;       // Doing gather/scatter
    logic [`CORE_GSC_CNT_RANGE]   gsc_src;      // Gather/scatter element
    logic                         kill;         // Kill the instruction
    logic [`XREG_RANGE]           fromint_data; // Integer data to VPU
    logic                         thread_id;    // Thread ID
} minion_vpu_ex_req;

// Minion request signals to VPU for ID stage
typedef struct packed
{
    logic               valid;     // Instruction is valid
    logic               thread_id; // Thread ID
    logic [2:0]         fcsr_rm;   // Rounding mode in FCSR
    logic [`INST_RANGE] inst_bits;
} minion_vpu_id_req;

// VPU control signals to Core for ID stage
typedef struct packed
{
    logic                 m0ren;             // Instruction reads mask entry 0
    logic                 mallren;           // Instruction reads all mask registers
    logic                 mren1;             // Instruction reads mask src1
    logic                 mren2;             // Instruction reads mask src2
    logic                 wen;               // Instruction has a destination
    logic                 ren1;              // Instruction needs to read src1
    logic                 ren2;              // Instruction needs to read src2
    logic                 ren3;              // Instruction needs to read src3
    logic                 rend;              // Instruction needs to read rend
    logic                 is_trans;          // Instruction is using trascendental unit
    logic                 trans_busy;        // Trans is currently executing any instruction
    logic                 id_trans_insert;   // Trans will write in same cycle as ID instruction, so don't execute if conflict
    vpu_minion_scoreboard scoreboard;        // In-flight entries that have long latency
    logic                 tfma_enabled;      // TFMA module is enabled doing TensorFMA/TensorIMA
    logic                 tfma_wrrf_enabled; // TFMA module is enabled doing TensorFMA/TensorIMA that writes VPU RF
    logic                 tquant_enabled;    // TQuant module is enabled doing TensorQuant
    logic                 reduce_enabled;    // Reduce module is enabled doing TensorReduce/TensorStore
    logic                 fflags_stall;      // instruction in f2..f4 can update the flags
    logic                 fromint;
} vpu_minion_id_ctrl;

// VPU control signals to Core for EX stage
typedef struct packed
{
    logic                            tointm;     // Comparison instruction writing to mask
    logic                            illegal_rm; // Illegal rounding mode
    logic [`DMEM_REQ_PS_MASK_SZ-1:0] ps_mask;    // PS mask for dcache access
    logic [31:0]                     gsc_fs;     // Gather/Scatter index
} vpu_minion_ex_ctrl;

// VPU control signals to Core for TAG stage
typedef struct packed
{
    logic                               fma;          // FMA instruction
    logic [`CORE_VPU_STORE_DATA_SZ-1:0] store_data;   // Store data for vector and scalars
    logic [`VPU_DATA_S_SZ-1:0]          scatter_data; // Store data for scatters
    logic                               tointm;       // Comparison instruction writing to mask
} vpu_minion_tag_ctrl;

// VPU control signals to Core for MEM stage
typedef struct packed
{
    logic fma;        // FMA instruction
    logic tointm;     // Comparison instruction writing to mask
} vpu_minion_mem_ctrl;

// VPU control signals to Core for WB stage
typedef struct packed
{
    logic                    fma;              // fma instruction
    logic [`XREG_RANGE]      toint_data;       // int data from vpu uniy
    logic                    mova_mx;          // mova.m.m instruction
    logic                    fcsr_flags_valid; // FCSR flag update
    logic [5:0]              fcsr_flags;       // FCSR flag update contents
    logic                    thread_id;        // Thread id
    logic                    tointm;           // Comparison instruction writing to mask    
} vpu_minion_wb_ctrl;

// Interrupts to minion
typedef struct packed
{
   logic [`CORE_NR_THREADS-1:0]                mieco;
   logic [`CORE_NR_THREADS-1:0]                mtip;
   logic [`CORE_NR_THREADS-1:0]                msip;
   logic [`CORE_NR_THREADS-1:0]                meip;
   logic [`CORE_NR_THREADS-1:0]                seip;
  `ifdef CSR_U_INTERRUPTS 
   logic [`CORE_NR_THREADS-1:0]                ueip;
  `endif
   logic [(`CORE_NR_THREADS*`FCC_PER_MIN)-1:0] fcc;         // Fast Credit Counter
   logic [`CORE_NR_THREADS-1:0]                ipi_with_pc;
   logic [`PC_SIZE-2:0]                        ipi_pc; // PC without the LSB, which is always 0
} minion_interrupts;

// breakpoint configuration (mainly extracted from tdata1 csr)
typedef struct packed {
   logic       dmode;
   logic       action;  // 0=raise breakpoint exception, 1=enter debug mode (only if dmode=1)
   logic       match;
   logic       m;
   logic       s;
   logic       u;
   logic       execute;
   logic       store;
   logic       load;
} minion_bp_ctrl;

// Request from VPU to DCache to read straight from LRAM
typedef struct packed
{
    logic                           read_en; // Enable the read
    logic [`DCACHE_WAY_RANGE]       way;     // Which way to read from
    logic [`DCACHE_LRAM_ADDR_RANGE] addr;    // Which block to read from
    logic                           size64;  // Size of the read (64b)
    logic                           size256; // Size of the read (256b)
    logic                           bid;     // Bids for the Dcache pipeline (prevents replayQ or core to get into pipeline)
} vpu_dcache_scp_req;

// Request from VPU to DCache to read straight from LRAM
typedef struct packed
{
    vpu_dcache_scp_req                   scp_req;           // Scratchpad request
    logic                                tenb_credit;       // Returns a TENB credit to Dcache
    logic [$clog2(`VPU_TENB_SIZE/2)-1:0] tenb_credit_entry; // Which TENB credit is returned
    logic                                tfma_enabled;      // The TFMA is enabled
    logic                                reduce_wait;       // Reduce has to wait until dependencies with other tensorops are resolved
    logic                                tfma_rf_write;     // The TFMA is using the RF wen[1](load) port    
} vpu_dcache_ctrl;

// Dcache control signals to perform reduce operations to VPU
typedef struct packed
{
    logic send_reg; // Dcache reduce module is requesting to the VPU to send a register
    logic exec_op;  // Dcache reduce module is requesting to the VPU to execute the reduce operation
    logic nothing;  // The reduce action is do nothing, so clear the state
} dcache_vpu_reduce_ctrl;

// Dcache control signals for scratchpad fills
typedef struct packed
{
    logic       fill_is_tenb_early; // The fill is related to a TENB (early version)
    logic       fill_is_tenb;       // The fill is related to a TENB
    logic       tenb_flush;         // Flush the current TENB buffer
    logic [3:0] tenb_line;          // Which TENB line it is related
} dcache_vpu_scp_resp;

// Request to the TLB
typedef struct packed
{
    minion_vm_status        status;
    logic [`VA_TRANS_RANGE] vpn;    // Virtual Page Number
    logic passthrough;              // Disable virtual memory
    logic instruction;              // Request is a instruction fetch
    logic store;                    // Request is a store
    logic msb_err;                  // malformed VA48 address
} tlb_req;

// TLB cache entry
typedef struct packed
{
    logic                   ptw_access_fault; // PTW access fault
    logic                   expected_ex_mode; // Expected execution mode
    logic                   dirty;            // PTE dirty bit
    logic                   access;           // PTE access bit
    logic                   sx;               // execute permission
    logic                   sw;               // write permission
    logic                   sr;               // read permission
    logic                   u;                // user privilege
    logic [`PA_TRANS_RANGE] ppn;
} tlb_entry_data_t;

// TLB response
typedef struct packed
{
    logic                   fill_pending; // TLB fill is pending after a TLB miss
    logic                   miss;         // TLB miss
    logic [`PA_TRANS_RANGE] ppn;          // Physical Page Number
    logic                   xcpt_ld;      // Load exception
    logic                   xcpt_st;      // Store exception
    logic                   xcpt_if;      // Fetch exception
    logic                   access_fault; // Exception is "Access fault" or "Page fault"
} tlb_resp;

// D-cache PMA request
typedef struct packed
{
    logic [`PA_RANGE] paddr;      // Physical address of the request
    dcache_type       typ;        // Size of operation
    logic             amo;        // Access is AMO (global or local)
    logic             write;      // Access is write (global or local)
    logic             mem_global; // Access is Memory global
    logic             ts_tl;      // Access is TensorLoad or TensorStore
    logic             ts_tl_co;   // Access is TensorLoad, TensorStore or CacheOp (prefetch)
} dcache_pma_req;

// D-cache PMA response
typedef struct packed
{
    logic  access_fault;    // Load/store access fault
    logic  cacheable;       // Address is cacheable
} dcache_pma_resp;

// I-cache PMA response
typedef struct packed
{
    logic  access_fault;    // Fetch access fault
    logic  cacheable;       // Address is cacheable
} icache_pma_resp;

// intpipe to frond end request - debug information
typedef struct packed
{
  logic halt;        // FE req generated by a single step or ebreak instruction outside debug mode
  logic resume;      // FE req generated by a dret or resume signal
} minion_fe_req_debug;


// intpipe to front end request
typedef struct packed
{
    logic [`PC_RANGE_EXT]         pc;                // PC of the request
    logic                         speculative;       // Speculative request
    minion_fe_req_debug           debug_info;
} minion_fe_req;



// Reduce control struct
typedef struct packed
{
    logic [1:0]  padding3;  // Padding                                                        // bits 62 and 63
    logic [4:0]  start_reg; // Starting register                                              // bit 61-57
    logic [28:0] padding2;  // Padding                                                        // bit 56-28
    logic [3:0]  op;        // 0 -> ADD, 8 -> FGET                                            // bits 27-24
    logic        padding1;  // Padding                                                        // bit 23
    logic [6:0]  num_regs;  // Number of registers to send                                    // bits 22-16
    logic [12:0] partner;   // ID of the partner minion                                       // bits 15-3 
    logic        padding0;  // Padding                                                        // bit 2
    logic [1:0]  action;    // Auto (11) / Send (01) / Receive (00)                           // bits 1 and 0
} reduce_control;

// Tensor store control struct
typedef struct packed
{
    logic [1:0]          src_inc;   // Increment of source for every element store
    logic [4:0]          start_reg; // Starting register
    logic [1:0]          cols;      // Number of register per col
    logic [3:0]          rows;      // Rows to store
    logic [1:0]          coop;      // Cooperative: 0 => No, 1 => 2 minions, 3 => 4 minions   // bits 50-49
    logic                scp;       // From scratchpad                                        // bit 48
    logic [`VA_SIZE-1:4] addr;      // Base address to store the tensor                       // bits 47-4
    logic [3:0]          rate;      // Rate                                                   // bits 3-0
} tensorstore_control;

// Tensor quantization control struct
typedef struct packed
{
    logic [1:0]  padding2;  // Padding                            
    logic [4:0]  start_reg; // Starting register                  
    logic [1:0]  cols;      // Number of register per col         
    logic [3:0]  rows;      // Rows to store                      
    logic [5:0]  scp_src;   // Where in scratchpad sources start  
    logic [4:0]  padding;   // Padding                            
    logic [39:0] trans;     // Up to 10 transformations           
} tensorquant_control;

typedef struct packed
{
    logic [1:0]          stride_entry; // Scratchpad entry stride
    logic [5:0]          start_entry;  // Scratchpad start entry
    logic                padding2;     // Padding                                             // bit 55
    logic [3:0]          rows;         // Rows to store                                       // bits 54-51
    logic [1:0]          padding1;     // Padding                                             // bits 50-49
    logic                scp;          // From scratchpad                                     // bits 48
    logic [`VA_SIZE-1:6] addr;         // Base address to store the tensor                    // bits 47-6
    logic [1:0]          padding0;     // Padding                                             // bits 5-4
    logic [3:0]          rate;         // Rate                                                // bits 3-0
} tensorstore_scp_control;

typedef union packed
{
    reduce_control          reduce;
    tensorstore_control     tensorstore;
    tensorstore_scp_control tensorstore_scp;
} reduce_tensorstore_control;

// TensorFMA control struct
typedef struct packed
{
    logic        is_conv;    // Use convolution CSR to skip computation of rows
    logic [5:0]  padding3;   // Padding to align with CSR
    logic [1:0]  cols_b;     // Number of rows of B matrix to be computed (writing a 15 means 16 rows)
    logic [3:0]  rows_a;     // Number of rows of A matrix to be computed (writing a 7 means 8 rows)
    logic [3:0]  cols_a;     // Number of cols of A matrix to be computed (writing a 15 means 16 rows)
    logic [3:0]  start_a;    // Position within cacheline where the A element is at 32b granularity
    logic [18:0] padding2;   // Padding to align with CSR
    logic        to_vrf;     // Store results to VPU RF (IMA only)
    logic        u_b;        // Matrix B is unsigned (IMA only)
    logic        u_a;        // Matrix A is unsigned (IMA only)
    logic        ten_b;      // Read data straight from TENB
    logic [1:0]  padding1;   // Padding to align with CSR
    logic [5:0]  scp_b;      // Where in the scratchpad the matrix B is stored
    logic [1:0]  padding0;   // Padding to align with CSR
    logic [5:0]  scp_a;      // Where in the scratchpad the matrix A is stored
    logic [2:0]  mode;       // Doing an 000 -> FP32, 001 -> *FP16+FP32, 010 -> FP16, 011 -> *INT8+INT32 operation
    logic        first_pass; // 1 -> 1st pass is a MUL, 0 -> 1st pass is an FMA
} tensorfma_control;

// Core to VPU control signals
typedef struct packed {
    logic                         tensorfma_start;           // Write to TensorFMA CSR
    tensorfma_control             tensorfma_ctrl;            // TensorFMA control bits
    logic                         tensorfma_conv_bits_ready; // Convolution CSR bits are ready
    logic [`TENSOR_MASK_BITS-1:0] tensorfma_conv_bits;       // Convolution CSR bit results
    logic                         reduce_start;              // Start a new reduce instruction
    logic                         tensorstore_start;         // Start a new tensor store instruction
    reduce_tensorstore_control    reduce_ctrl;               // Reduce information
    logic                         tensorquant_start;         // Start
    tensorquant_control           tensorquant_ctrl;          // Information of tensor quant
    logic                         pending_scoreboard_t0;     // There's a pending write in the scoreboard for thread 0
} core_vpu_ctrl;

// debug input signals
typedef struct packed {
   logic [`CORE_NR_THREADS-1:0]       halt;
   logic [`CORE_NR_THREADS-1:0]       resume;
   logic [`CORE_NR_THREADS-1:0]       resethalt;
   logic [`CORE_NR_THREADS-1:0]       ackhavereset;
} minion_debug_in_t;


// debug output signals
typedef struct packed {
   logic [`CORE_NR_THREADS-1:0]      halted;
   logic [`CORE_NR_THREADS-1:0]      running;
   logic [`CORE_NR_THREADS-1:0]      have_reset;
   logic [`CORE_NR_THREADS-1:0]      busy;
   logic [`CORE_NR_THREADS-1:0]      exception;
   logic [`CORE_NR_THREADS-1:0]      error;
} minion_debug_out_t;


// breakpoint configuration
typedef struct packed {
  logic                     thread_id;
   logic [`VA_SIZE_EXT-1:0] address;
   debug_bp_t               cmd;
} dcache_core_bp_t;

// chicken bits
typedef struct packed {
   logic rsv_1; //reserved
   logic rsv_0; //reserved
   logic min_frontend_clock_gate_disable;
   logic min_dcache_clock_gate_disable;
   logic min_vputrans_clock_gate_disable;
   logic min_vputima_clock_gate_disable;
   logic min_vpulane_clock_gate_disable;
   logic min_intpipe_clock_gate_disable;
} minion_chicken_bits_t;

typedef struct packed {
   logic min_frontend_clock_gate_disable;
   logic min_dcache_clock_gate_disable;
   logic min_vputrans_clock_gate_disable;
   logic min_vputima_clock_gate_disable;
   logic min_vpulane_clock_gate_disable;
   logic min_intpipe_clock_gate_disable;
} minion_chicken_bits_vc_t;


////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
// function automatic [`MINION_DEBUG_APB_AD_WIDTH-1:0] bpam2minion_ad;
//    input [`MIN_DM_APB_ADDR_WIDTH-1:0] inAd;
//    begin
//      bpam2minion_ad = { inAd[`NEIGH_DM_APB_ADDR_REG_RANGE],   // register address
// 			inAd[`NEIGH_DM_APB_ADDR_THREAD_RANGE],   // selected thread
//                         inAd[`NEIGH_DM_APB_ADDR_PP_RANGE] // PP
//                         }; 
 
//    end
// endfunction

// Minion Memory map
// +---------------------+---------------------------------+-----------------+
// |    Address range    |      Address range (hex)        |                 |
// |   From   |    To    |      From      |      To        | Maps to         |
// +----------+----------+----------------+----------------+-----------------+
// |     0G   |    32M   | 0x00_0000_0000 | 0x00_01ff_ffff | Reserved        |
// |    32M   |   32M+4K | 0x00_0200_0000 | 0x00_0200_0fff | System registers|
// |   32M+4K |   32M+8K | 0x00_0200_1000 | 0x00_0200_1fff | MRAM registers  |
// |   32M+8K |   32M+12K| 0x00_0200_2000 | 0x00_0200_2fff | IO region       |
// |   32M+12K|   32M+16K| 0x00_0200_3000 | 0x00_0200_3fff | Hyperbus region |
// |   32M+16K|   32M+40K| 0x00_0200_4000 | 0x00_0200_9fff | Reserved        |
// |   32M+40K|   32M+48K| 0x00_0200_A000 | 0x00_0200_Bfff | BootROM region  |
// |   32M+48K|   32M+56K| 0x00_0200_C000 | 0x00_0200_Dfff | Reserved        |
// |   32M+56K|   32M+58K| 0x00_0200_E000 | 0x00_0200_E7ff | SRAM region     |
// |   32M+58K|     1G   | 0x00_0200_E800 | 0x00_3fff_ffff | Reserved        |
// |     1G   |   1G+16M | 0x00_4000_0000 | 0x00_40ff_ffff | MRAM region     |
// |   1G+16M | 1G+16M+4K| 0x00_4100_0000 | 0x00_4100_0fff | OTP region      |
// | 1G+16M+4K|     2G   | 0x00_4100_1000 | 0x00_7fff_ffff | Reserved        |
// |     2G   |     4G   | 0x00_8000_0000 | 0x00_ffff_ffff | ESR region      |
// +----------+----------+----------------+----------------+-----------------+

function automatic logic paddr_is_systemreg_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_SYSTEMREG_REGION_RANGE] == `AD_SYSTEMREG_REGION;
endfunction

function automatic logic paddr_is_mramreg_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_MRAMREG_REGION_RANGE] == `AD_MRAMREG_REGION;
endfunction

function automatic logic paddr_is_io_space_space(logic [`PA_RANGE] paddr);
    return (paddr[`AD_IO1_REGION_RANGE] == `AD_IO1_REGION) || (paddr[`AD_IO2_REGION_RANGE] == `AD_IO2_REGION);
endfunction

function automatic logic paddr_is_xspi_space(logic [`PA_RANGE] paddr);
    return (paddr[`AD_XSPI_REGION_RANGE] == `AD_XSPI_REGION);
endfunction

function automatic logic paddr_is_nic_space(logic [`PA_RANGE] paddr);
    return (paddr[`AD_NIC_REGION_RANGE] == `AD_NIC_REGION);
endfunction

function automatic logic paddr_is_bootrom_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_BOOTROM_REGION_RANGE] == `AD_BOOTROM_REGION;
endfunction

function automatic logic paddr_is_sram_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_SRAM_REGION_RANGE] == `AD_SRAM_REGION;
endfunction

function automatic logic paddr_is_mram_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_DDR_REGION_RANGE] == `AD_DDR_REGION;
endfunction

function automatic logic paddr_is_mram_mbox(logic [`PA_RANGE] paddr, esr_mprot_t mprot);
    return paddr_is_mram_space(paddr)
        && mprot.mprot_en
        && ((paddr[(`AD_DDR_REGION_START-1) -: `AD_MRAM_SUBREGION_SIZE] >> mprot.mmode_size) == '0);
endfunction

function automatic logic paddr_is_mram_sbox(logic [`PA_RANGE] paddr, esr_mprot_t mprot);
    return paddr_is_mram_space(paddr)
        && mprot.mprot_en
        && ((paddr[(`AD_DDR_REGION_START-1) -: `AD_MRAM_SUBREGION_SIZE] >> mprot.mmode_size) != '0)
        && ((paddr[(`AD_DDR_REGION_START-1) -: `AD_MRAM_SUBREGION_SIZE] >> mprot.smode_size) == '0);
endfunction

function automatic logic paddr_is_mram_other(logic [`PA_RANGE] paddr, esr_mprot_t mprot);
    return  paddr_is_mram_space(paddr)
        && !paddr_is_mram_mbox(paddr, mprot)
        && !paddr_is_mram_sbox(paddr, mprot);
endfunction

function automatic logic paddr_is_otp_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_OTP_REGION_RANGE] == `AD_OTP_REGION;
endfunction

function automatic logic paddr_is_esr_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_ESR_REGION_RANGE] == `AD_ESR_REGION;
endfunction

function automatic logic paddr_is_plic_space(logic [`PA_RANGE] paddr);
    return paddr[`AD_PLIC_REGION_RANGE] == `AD_PLIC_REGION;
endfunction

function automatic logic [`VA_RANGE_EXT] extend_VA;
   input [`XREG_RANGE] in;
   begin
      extend_VA[`VA_RANGE] = in[`VA_RANGE];
      if (&in[`XREG_SIZE-1:`VA_SIZE-1] == 1'b1 ||     // bits 47 to 63 are all one
          |in[`XREG_SIZE-1:`VA_SIZE-1] == 1'b0)         // bits 47 to 63 are all zeros
        extend_VA[`VA_EXT_MSB] = extend_VA[`VA_MSB];  // extra bit is sign extension
      else
        extend_VA[`VA_EXT_MSB] = !extend_VA[`VA_MSB]; // extra bit is negating
   end
endfunction

