// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache pipe atomic operations.
//
// Takes read data (pipe_data) and write operands (atomic_operands) and produces
// the updated cache line for write-back. Supports SIMD (8×32b), scalar 64-bit,
// scalar 32-bit, and partial-write operations. Returns original read data for
// the atomic response (LS-aligned, zero-extended).
//
// Three-stage pipeline: _d0 (extract/qualify), _d1 (ALU), _d2 (output register).
// atomic_data_rsp_data_o is purely combinational (no pipeline delay).
//
// Replaces: shire_cache_pipe_atomic (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions
//   - Package constants instead of `define macros
//   - Explicit always_ff/always_comb instead of RST_EN_FF/EN_FF/ZX macros

/* verilator lint_off UNUSEDSIGNAL */  // pipe_info_t/reqq_pipe_req_t passed whole; atomic only reads a few fields
module shirecache_pipe_atomic
  import shirecache_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  input  logic                        pipe_valid_i,
  input  pipe_info_t                  pipe_info_i,
  input  reqq_pipe_req_t              pipe_req_info_i,
  input  logic [LineDataSize-1:0]     pipe_data_i,

  // Incoming write data / atomic operands
  input  logic [LineDataSize-1:0]     atomic_operands_i,
  input  logic [LineByteSize-1:0]     partial_wr_data_ben_i,

  // Combinational: read-data response for L3/Scp atomic
  output logic [LineDataSize-1:0]     atomic_data_rsp_data_o,

  // Registered result (2-cycle latency)
  output logic                        atomic_result_wr_en_o,
  output logic [DataRamAddrSize-1:0]  atomic_result_wr_addr_o,
  output logic [LineDataSize-1:0]     atomic_result_wr_data_o,
  output logic [LineQwSize-1:0]       atomic_result_wr_qwen_o
);

  // pipe_info_t and reqq_pipe_req_t carry full pipeline context;
  // atomic only uses a few fields (hit_way, qwen, address offset, opcode).
  /* verilator lint_off UNUSEDSIGNAL */  // struct fields (trans_id, reqq_id, etc.) unused by atomic logic
  // rst_ni used in both async reset sensitivity and assertion guards
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used in both always_ff and assertions

  localparam int unsigned ConfSize    = $bits(amo_conf_req_t);     // 7
  localparam int unsigned OffsetSize  = LineOffsetIdSize;           // 6
  localparam int unsigned Swords      = AtomicSimdSize;             // 8
  localparam int unsigned AtomicQwens = AtomicDataSize / QwDataSize; // 2

  // ═══════════════════════════════════════════════════════
  // _d0 stage — extract fields, qualify valid
  // ═══════════════════════════════════════════════════════

  logic pipe_valid_d0;
  assign pipe_valid_d0 = pipe_valid_i && pipe_info_i.tag_hit && !pipe_info_i.victim &&
    (pipe_req_info_i.opcode == L2Atomic       ||
     pipe_req_info_i.opcode == L3Atomic       ||
     pipe_req_info_i.opcode == ScpAtomic      ||
     pipe_req_info_i.opcode == L2WritePartial ||
     pipe_req_info_i.opcode == L3WritePartial ||
     pipe_req_info_i.opcode == ScpWritePartial);

  // Extract packed fields from atomic_operands
  logic [OffsetSize-1:0]       atomic_addr_offset_d0;
  amo_conf_req_t               atomic_conf_d0;
  logic [QwDataSize-1:0]       atomic_routing_d0;
  logic [AtomicDataSize-1:0]   atomic_operand_simd_d0;

  assign atomic_addr_offset_d0  = atomic_operands_i[(AtomicDataSize+ConfSize) +: OffsetSize];
  assign atomic_conf_d0         = amo_conf_req_t'(atomic_operands_i[AtomicDataSize +: ConfSize]);
  assign atomic_routing_d0      = atomic_operands_i[AtomicDataSize +: QwDataSize];
  assign atomic_operand_simd_d0 = atomic_operands_i[0 +: AtomicDataSize];

  // Replicate operands for scalar modes (share SIMD datapaths)
  logic [AtomicDataSize-1:0]   atomic_operand_sc64_d0;
  logic [AtomicDataSize-1:0]   atomic_operand_sc32_d0;
  assign atomic_operand_sc64_d0 = {(AtomicDataSize/DwDataSize){atomic_operand_simd_d0[0 +: DwDataSize]}};
  assign atomic_operand_sc32_d0 = {(AtomicDataSize/SwDataSize){atomic_operand_simd_d0[0 +: SwDataSize]}};

  // CompareExchange expected values
  logic [DwDataSize-1:0] atomic_expected_sc64_d0;
  logic [SwDataSize-1:0] atomic_expected_sc32_d0;
  assign atomic_expected_sc64_d0 = atomic_operand_simd_d0[DwDataSize +: DwDataSize];
  assign atomic_expected_sc32_d0 = atomic_operand_simd_d0[DwDataSize +: SwDataSize];

  // Operation type
  logic partial_op_d0, simd_op_d0, sc64_op_d0;
  assign partial_op_d0 = (pipe_req_info_i.opcode == L2WritePartial) ||
                         (pipe_req_info_i.opcode == L3WritePartial) ||
                         (pipe_req_info_i.opcode == ScpWritePartial);
  assign simd_op_d0    =  atomic_conf_d0.sc_vec;
  assign sc64_op_d0    = !atomic_conf_d0.sc_vec &&  atomic_conf_d0.size;

  // Compose operand: partial uses raw operands, atomic uses type-specific layout
  logic [LineDataSize-1:0] atomic_operand_d0;
  always_comb begin
    if (partial_op_d0)
      atomic_operand_d0 = atomic_operands_i;
    else if (simd_op_d0)
      atomic_operand_d0 = LineDataSize'(atomic_operand_simd_d0);
    else if (sc64_op_d0)
      atomic_operand_d0 = LineDataSize'({atomic_expected_sc64_d0, atomic_operand_sc64_d0});
    else // sc32
      atomic_operand_d0 = LineDataSize'({atomic_expected_sc32_d0, atomic_operand_sc32_d0});
  end

  // Read data at atomic offset
  logic [AtomicDataSize-1:0] atomic_rd_data_d0;
  assign atomic_rd_data_d0 = pipe_data_i[
    AtomicDataSize * atomic_addr_offset_d0[AtomicDataSelMsb:AtomicDataSelLsb] +: AtomicDataSize];

  // ── Combinational response data (no pipeline delay) ───
  assign atomic_data_rsp_data_o =
    ((pipe_req_info_i.opcode == L3Atomic) || (pipe_req_info_i.opcode == ScpAtomic))
      ? LineDataSize'({atomic_routing_d0, atomic_rd_data_d0})
      : pipe_data_i;

  // ── Result address / qwen computation ─────────────────
  logic [DataRamAddrSize-1:0] atomic_result_addr_d0;
  assign atomic_result_addr_d0 = {pipe_info_i.pipe_set, pipe_info_i.tag_ram_hit_way};

  localparam logic [LineQwSize-1:0] QwenEnable = LineQwSize'((1 << AtomicQwens) - 1);
  logic [LineQwSize-1:0] atomic_result_qwen_d0;
  always_comb begin
    if (pipe_info_i.data_rsp_zero)
      atomic_result_qwen_d0 = '1;
    else if (partial_op_d0)
      atomic_result_qwen_d0 = pipe_req_info_i.qwen;
    else
      atomic_result_qwen_d0 = QwenEnable <<
        (AtomicQwens * atomic_addr_offset_d0[AtomicDataSelMsb:AtomicDataSelLsb]);
  end

  // ═══════════════════════════════════════════════════════
  // _d1 stage — register inputs, perform ALU
  // ═══════════════════════════════════════════════════════

  logic                        pipe_valid_d1;
  logic                        partial_op_d1;
  logic [LineByteSize-1:0]     partial_ben_d1;
  logic [LineDataSize-1:0]     orig_rd_data_d1;
  logic [AtomicDataSize-1:0]   atomic_rd_data_d1;
  logic [LineDataSize-1:0]     atomic_operand_d1;
  amo_conf_req_t               atomic_conf_d1;
  logic [DataRamAddrSize-1:0]  atomic_result_addr_d1;
  logic [LineQwSize-1:0]       atomic_result_qwen_d1;
  logic [OffsetSize-1:0]       atomic_addr_offset_d1;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                          pipe_valid_d1 <= 1'b0;
    else if (pipe_valid_d1 | pipe_valid_d0) pipe_valid_d1 <= pipe_valid_d0;
  end

  always_ff @(posedge clk_i) begin
    if (pipe_valid_d0) begin
      partial_op_d1         <= partial_op_d0;
      partial_ben_d1        <= partial_wr_data_ben_i;
      orig_rd_data_d1       <= pipe_data_i;
      atomic_rd_data_d1     <= atomic_rd_data_d0;
      atomic_operand_d1     <= atomic_operand_d0;
      atomic_conf_d1        <= atomic_conf_d0;
      atomic_result_addr_d1 <= atomic_result_addr_d0;
      atomic_result_qwen_d1 <= atomic_result_qwen_d0;
      atomic_addr_offset_d1 <= atomic_addr_offset_d0;
    end
  end

  // ── Operation types (d1) ──────────────────────────────
  logic simd_op_d1, sc64_op_d1;
  assign simd_op_d1 =  atomic_conf_d1.sc_vec;
  assign sc64_op_d1 = !atomic_conf_d1.sc_vec && atomic_conf_d1.size;

  // ── Vector logical operations ─────────────────────────
  logic [AtomicDataSize-1:0] atomic_xor, atomic_and, atomic_or;
  assign atomic_xor = atomic_rd_data_d1 ^ atomic_operand_d1[AtomicDataSize-1:0];
  assign atomic_and = atomic_rd_data_d1 & atomic_operand_d1[AtomicDataSize-1:0];
  assign atomic_or  = atomic_rd_data_d1 | atomic_operand_d1[AtomicDataSize-1:0];

  // ── SIMD / scalar-32 ALU ──────────────────────────────
  logic [Swords-1:0][SwDataSize-1:0] vec_rd_data_d1;
  logic [Swords-1:0][SwDataSize-1:0] vec_operand_d1;
  assign vec_rd_data_d1 = atomic_rd_data_d1;
  assign vec_operand_d1 = atomic_operand_d1[AtomicDataSize-1:0];

  // CompareExchange (SIMD/sc32 share path)
  logic [SwDataSize-1:0] sc32_expected_d1;
  assign sc32_expected_d1 = atomic_operand_d1[AtomicDataSize +: SwDataSize];

  logic [AtomicDataSize-1:0] simd_comp_exch;
  assign simd_comp_exch =
    (!simd_op_d1 &&
     (vec_rd_data_d1[atomic_addr_offset_d1[AtomicSc32Msb:AtomicSc32Lsb]] == sc32_expected_d1))
      ? atomic_operand_d1[AtomicDataSize-1:0]
      : atomic_rd_data_d1;

  localparam logic [SwDataSize-1:0] SimdCnan = 32'h7fc0_0000;

  logic [Swords-1:0][SwDataSize-1:0] simd_iadd, simd_imin, simd_imax;
  logic [Swords-1:0][SwDataSize-1:0] simd_umin, simd_umax;
  logic [Swords-1:0][SwDataSize-1:0] simd_psmin, simd_psmax;

  always_comb begin
    for (int unsigned ii = 0; ii < Swords; ii++) begin
      automatic logic ugt = vec_rd_data_d1[ii] > vec_operand_d1[ii];
      automatic logic sgt = ugt ^ (vec_rd_data_d1[ii][31] ^ vec_operand_d1[ii][31]);
      automatic logic fgt = ugt ^ (vec_rd_data_d1[ii][31] | vec_operand_d1[ii][31]);

      automatic logic qnan_rd  = (vec_rd_data_d1[ii][30:23] == 8'hff) &&  vec_rd_data_d1[ii][22];
      automatic logic qnan_op  = (vec_operand_d1[ii][30:23] == 8'hff) &&  vec_operand_d1[ii][22];
      automatic logic snan_rd  = (vec_rd_data_d1[ii][30:23] == 8'hff) && !vec_rd_data_d1[ii][22] && |vec_rd_data_d1[ii][21:0];
      automatic logic snan_op  = (vec_operand_d1[ii][30:23] == 8'hff) && !vec_operand_d1[ii][22] && |vec_operand_d1[ii][21:0];
      automatic logic any_nan  = qnan_rd | qnan_op | snan_rd | snan_op;
      automatic logic [SwDataSize-1:0] nan_result =
        (snan_rd || snan_op || (qnan_rd && qnan_op)) ? SimdCnan :
        qnan_rd                                       ? vec_operand_d1[ii] :
                                                        vec_rd_data_d1[ii];

      simd_iadd[ii] = vec_rd_data_d1[ii] + vec_operand_d1[ii];
      simd_imin[ii] = !sgt ? vec_rd_data_d1[ii] : vec_operand_d1[ii];
      simd_imax[ii] =  sgt ? vec_rd_data_d1[ii] : vec_operand_d1[ii];
      simd_umin[ii] = !ugt ? vec_rd_data_d1[ii] : vec_operand_d1[ii];
      simd_umax[ii] =  ugt ? vec_rd_data_d1[ii] : vec_operand_d1[ii];
      simd_psmin[ii] = any_nan ? nan_result : (!fgt ? vec_rd_data_d1[ii] : vec_operand_d1[ii]);
      simd_psmax[ii] = any_nan ? nan_result : ( fgt ? vec_rd_data_d1[ii] : vec_operand_d1[ii]);
    end
  end

  // ── Scalar-64 ALU ─────────────────────────────────────
  logic [DwDataSize-1:0] sc64_rd_data_d1;
  logic [DwDataSize-1:0] sc64_operand_d1;
  assign sc64_rd_data_d1 = atomic_rd_data_d1[
    atomic_addr_offset_d1[AtomicSc64Msb:AtomicSc64Lsb] * DwDataSize +: DwDataSize];
  assign sc64_operand_d1 = atomic_operand_d1[0 +: DwDataSize];

  logic [DwDataSize-1:0] sc64_expected_d1;
  assign sc64_expected_d1 = atomic_operand_d1[AtomicDataSize +: DwDataSize];

  logic [DwDataSize-1:0] sc64_comp_exch;
  assign sc64_comp_exch = (sc64_rd_data_d1 == sc64_expected_d1)
                          ? sc64_operand_d1 : sc64_rd_data_d1;

  localparam logic [DwDataSize-1:0] Sc64Cnan = 64'h7ff8_0000_0000_0000;

  logic [DwDataSize-1:0] sc64_iadd, sc64_imin, sc64_imax;
  logic [DwDataSize-1:0] sc64_umin, sc64_umax;
  logic [DwDataSize-1:0] sc64_psmin, sc64_psmax;

  always_comb begin
    automatic logic ugt = sc64_rd_data_d1 > sc64_operand_d1;
    automatic logic sgt = ugt ^ (sc64_rd_data_d1[63] ^ sc64_operand_d1[63]);
    automatic logic fgt = ugt ^ (sc64_rd_data_d1[63] | sc64_operand_d1[63]);

    automatic logic qnan_rd = (sc64_rd_data_d1[62:52] == 11'h7ff) &&  sc64_rd_data_d1[51];
    automatic logic qnan_op = (sc64_operand_d1[62:52] == 11'h7ff) &&  sc64_operand_d1[51];
    automatic logic snan_rd = (sc64_rd_data_d1[62:52] == 11'h7ff) && !sc64_rd_data_d1[51] && |sc64_rd_data_d1[50:0];
    automatic logic snan_op = (sc64_operand_d1[62:52] == 11'h7ff) && !sc64_operand_d1[51] && |sc64_operand_d1[50:0];
    automatic logic any_nan = qnan_rd | qnan_op | snan_rd | snan_op;
    automatic logic [DwDataSize-1:0] nan_result =
      (snan_rd || snan_op || (qnan_rd && qnan_op)) ? Sc64Cnan :
      qnan_rd                                       ? sc64_operand_d1 :
                                                      sc64_rd_data_d1;

    sc64_iadd  = sc64_rd_data_d1 + sc64_operand_d1;
    sc64_imin  = !sgt ? sc64_rd_data_d1 : sc64_operand_d1;
    sc64_imax  =  sgt ? sc64_rd_data_d1 : sc64_operand_d1;
    sc64_umin  = !ugt ? sc64_rd_data_d1 : sc64_operand_d1;
    sc64_umax  =  ugt ? sc64_rd_data_d1 : sc64_operand_d1;
    sc64_psmin = any_nan ? nan_result : (!fgt ? sc64_rd_data_d1 : sc64_operand_d1);
    sc64_psmax = any_nan ? nan_result : ( fgt ? sc64_rd_data_d1 : sc64_operand_d1);
  end

  // ── SIMD/sc32 opcode mux ─────────────────────────────
  logic [AtomicDataSize-1:0] atomic_simd_alu_d1;
  always_comb begin
    unique case (atomic_conf_d1.opcode)
      AmoSwap:     atomic_simd_alu_d1 = atomic_operand_d1[AtomicDataSize-1:0];
      AmoAdd:      atomic_simd_alu_d1 = simd_iadd;
      AmoXor:      atomic_simd_alu_d1 = atomic_xor;
      AmoAnd:      atomic_simd_alu_d1 = atomic_and;
      AmoOr:       atomic_simd_alu_d1 = atomic_or;
      AmoMin:      atomic_simd_alu_d1 = simd_imin;
      AmoMax:      atomic_simd_alu_d1 = simd_imax;
      AmoMinU:     atomic_simd_alu_d1 = simd_umin;
      AmoMaxU:     atomic_simd_alu_d1 = simd_umax;
      AmoMinPs:    atomic_simd_alu_d1 = simd_psmin;
      AmoMaxPs:    atomic_simd_alu_d1 = simd_psmax;
      AmoCompExch: atomic_simd_alu_d1 = simd_comp_exch;
      default:     atomic_simd_alu_d1 = atomic_rd_data_d1;
    endcase
  end

  // ── Scalar-64 opcode mux ──────────────────────────────
  logic [DwDataSize-1:0] atomic_sc64_alu_d1;
  always_comb begin
    unique case (atomic_conf_d1.opcode)
      AmoSwap:     atomic_sc64_alu_d1 = atomic_operand_d1[0 +: DwDataSize];
      AmoAdd:      atomic_sc64_alu_d1 = sc64_iadd;
      AmoXor:      atomic_sc64_alu_d1 = atomic_xor[
                     atomic_addr_offset_d1[AtomicSc64Msb:AtomicSc64Lsb] * DwDataSize +: DwDataSize];
      AmoAnd:      atomic_sc64_alu_d1 = atomic_and[
                     atomic_addr_offset_d1[AtomicSc64Msb:AtomicSc64Lsb] * DwDataSize +: DwDataSize];
      AmoOr:       atomic_sc64_alu_d1 = atomic_or[
                     atomic_addr_offset_d1[AtomicSc64Msb:AtomicSc64Lsb] * DwDataSize +: DwDataSize];
      AmoMin:      atomic_sc64_alu_d1 = sc64_imin;
      AmoMax:      atomic_sc64_alu_d1 = sc64_imax;
      AmoMinU:     atomic_sc64_alu_d1 = sc64_umin;
      AmoMaxU:     atomic_sc64_alu_d1 = sc64_umax;
      AmoMinPs:    atomic_sc64_alu_d1 = sc64_psmin;
      AmoMaxPs:    atomic_sc64_alu_d1 = sc64_psmax;
      AmoCompExch: atomic_sc64_alu_d1 = sc64_comp_exch;
      default:     atomic_sc64_alu_d1 = sc64_rd_data_d1;
    endcase
  end

  // ── Partial write merge ───────────────────────────────
  logic [LineDataSize-1:0] partial_data_d1;
  always_comb begin
    for (int unsigned ii = 0; ii < LineByteSize; ii++) begin
      partial_data_d1[ii*8 +: 8] = partial_ben_d1[ii]
        ? atomic_operand_d1[ii*8 +: 8]
        : orig_rd_data_d1[ii*8 +: 8];
    end
  end

  // ── Final result compose ──────────────────────────────
  logic [LineDataSize-1:0] atomic_result_data_d1;
  always_comb begin
    if (partial_op_d1) begin
      atomic_result_data_d1 = partial_data_d1;
    end else begin
      atomic_result_data_d1 = orig_rd_data_d1;
      if (simd_op_d1) begin
        atomic_result_data_d1[
          atomic_addr_offset_d1[AtomicDataSelMsb:AtomicDataSelLsb] * AtomicDataSize +: AtomicDataSize
        ] = atomic_simd_alu_d1;
      end else if (sc64_op_d1) begin
        atomic_result_data_d1[
          atomic_addr_offset_d1[AtomicDwSelMsb:AtomicDwSelLsb] * DwDataSize +: DwDataSize
        ] = atomic_sc64_alu_d1;
      end else begin // sc32
        atomic_result_data_d1[
          atomic_addr_offset_d1[AtomicSwSelMsb:AtomicSwSelLsb] * SwDataSize +: SwDataSize
        ] = atomic_simd_alu_d1[
              atomic_addr_offset_d1[AtomicSc32Msb:AtomicSc32Lsb] * SwDataSize +: SwDataSize];
      end
    end
  end

  // ═══════════════════════════════════════════════════════
  // _d2 stage — register result, assign outputs
  // ═══════════════════════════════════════════════════════

  logic                       pipe_valid_d2;
  logic [LineDataSize-1:0]    atomic_result_data_d2;
  logic [DataRamAddrSize-1:0] atomic_result_addr_d2;
  logic [LineQwSize-1:0]      atomic_result_qwen_d2;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)                            pipe_valid_d2 <= 1'b0;
    else if (pipe_valid_d2 || pipe_valid_d1) pipe_valid_d2 <= pipe_valid_d1;
  end

  always_ff @(posedge clk_i) begin
    if (pipe_valid_d1) begin
      atomic_result_data_d2 <= atomic_result_data_d1;
      atomic_result_addr_d2 <= atomic_result_addr_d1;
      atomic_result_qwen_d2 <= atomic_result_qwen_d1;
    end
  end

  assign atomic_result_wr_en_o   = pipe_valid_d2;
  assign atomic_result_wr_addr_o = atomic_result_addr_d2;
  assign atomic_result_wr_data_o = atomic_result_data_d2;
  assign atomic_result_wr_qwen_o = atomic_result_qwen_d2;

  // ── Assertions ────────────────────────────────────────
  // synthesis translate_off
`ifdef VERILATOR
  always_ff @(posedge clk_i) begin
    // L2 atomic: offset packed in operands must match address LSBs
    if (rst_ni && pipe_valid_d0 && (pipe_req_info_i.opcode == L2Atomic) &&
        (atomic_addr_offset_d0 != pipe_req_info_i.address[OffsetSize-1:0]))
      $error("ERROR_PIPE_ATOMIC_L2_address_offset_check");

    // L2 atomic must have dest==0
    if (rst_ni && pipe_valid_d0 && (pipe_req_info_i.opcode == L2Atomic) &&
        atomic_conf_d0.dest)
      $error("ERROR_PIPE_ATOMIC_L2_atomic_dest_check");

    // L3 atomic must have dest==1
    if (rst_ni && pipe_valid_d0 && (pipe_req_info_i.opcode == L3Atomic) &&
        !atomic_conf_d0.dest)
      $error("ERROR_PIPE_ATOMIC_L3_atomic_dest_check");

    // Scp atomic must have dest==1
    if (rst_ni && pipe_valid_d0 && (pipe_req_info_i.opcode == ScpAtomic) &&
        !atomic_conf_d0.dest)
      $error("ERROR_PIPE_ATOMIC_Scp_atomic_dest_check");
  end
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_d0 && (pipe_req_info_i.opcode == L2Atomic)) |->
      (atomic_addr_offset_d0 == pipe_req_info_i.address[OffsetSize-1:0]))
  else $error("ERROR_PIPE_ATOMIC_L2_address_offset_check");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_d0 && (pipe_req_info_i.opcode == L2Atomic)) |-> !atomic_conf_d0.dest)
  else $error("ERROR_PIPE_ATOMIC_L2_atomic_dest_check");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_d0 && (pipe_req_info_i.opcode == L3Atomic)) |-> atomic_conf_d0.dest)
  else $error("ERROR_PIPE_ATOMIC_L3_atomic_dest_check");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    (pipe_valid_d0 && (pipe_req_info_i.opcode == ScpAtomic)) |-> atomic_conf_d0.dest)
  else $error("ERROR_PIPE_ATOMIC_Scp_atomic_dest_check");
`endif
  // synthesis translate_on

  /* verilator lint_on SYNCASYNCNET */

endmodule : shirecache_pipe_atomic
/* verilator lint_on UNUSEDSIGNAL */
