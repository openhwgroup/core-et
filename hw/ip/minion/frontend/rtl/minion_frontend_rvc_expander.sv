// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RVC (RISC-V Compressed) instruction expander.
//
// Pure combinational. Expands 16-bit compressed instructions (C extension)
// to their 32-bit equivalents. If the input is already a 32-bit instruction
// (bits[1:0] == 2'b11), it passes through unchanged.
//
// Expands the original frontend's RV64 compressed encoding map.
// The ET-Minion architectural ISA is RV64IMFC + Zicsr + Zifencei; this list
// describes expander-recognized encodings, not additional D-extension support:
//   Quadrant 0: C.ADDI4SPN, C.FLD, C.LW, C.LD, C.FSD, C.SW, C.SD
//   Quadrant 1: C.NOP, C.ADDI, C.ADDIW, C.LI, C.ADDI16SP, C.LUI,
//               C.SRLI, C.SRAI, C.ANDI, C.SUB, C.XOR, C.OR, C.AND,
//               C.SUBW, C.ADDW, C.J, C.BEQZ, C.BNEZ
//   Quadrant 2: C.SLLI, C.FLDSP, C.LWSP, C.LDSP, C.JR, C.MV,
//               C.EBREAK, C.JALR, C.ADD, C.FSDSP, C.SWSP, C.SDSP
//
// Replaces: frontend_rvc_expander (etcore-soc, Chisel-generated)
//
// Differences from original:
//   - Clean SystemVerilog rewrite from RISC-V spec (not Chisel output)
//   - Same interface: in_bits[31:0] → out_bits[31:0]

module minion_frontend_rvc_expander (
  input  logic [31:0] in_bits,
  output logic [31:0] out_bits
);

  // ── Compressed instruction fields ─────────────────────

  logic [1:0]  c_op;        // bits[1:0]: quadrant
  logic [2:0]  c_funct3;    // bits[15:13]: function
  logic [4:0]  c_rs2;       // bits[6:2]: full rs2 (Q2)
  logic [4:0]  c_rd;        // bits[11:7]: full rd (Q2)

  // Compressed register mapping: 3-bit → 5-bit (x8-x15)
  logic [4:0]  c_rs1p;      // bits[9:7] + 8
  logic [4:0]  c_rs2p;      // bits[4:2] + 8
  logic [4:0]  c_rdp;       // bits[9:7] + 8 (alias of rs1p for Q0/Q1)

  assign c_op     = in_bits[1:0];
  assign c_funct3 = in_bits[15:13];
  assign c_rs2    = in_bits[6:2];
  assign c_rd     = in_bits[11:7];
  assign c_rs1p   = {2'b01, in_bits[9:7]};
  assign c_rs2p   = {2'b01, in_bits[4:2]};
  assign c_rdp    = {2'b01, in_bits[9:7]};

  // ── RV32/64 opcode constants ──────────────────────────

  localparam logic [6:0] OpLoad    = 7'b0000011;
  localparam logic [6:0] OpLoadFp  = 7'b0000111;
  localparam logic [6:0] OpStore   = 7'b0100011;
  localparam logic [6:0] OpStoreFp = 7'b0100111;
  localparam logic [6:0] OpImm     = 7'b0010011;
  localparam logic [6:0] OpImm32   = 7'b0011011;
  localparam logic [6:0] OpReg     = 7'b0110011;
  localparam logic [6:0] OpReg32   = 7'b0111011;
  localparam logic [6:0] OpLui     = 7'b0110111;
  localparam logic [6:0] OpBranch  = 7'b1100011;
  localparam logic [6:0] OpJal     = 7'b1101111;
  localparam logic [6:0] OpJalr    = 7'b1100111;

  // ── Illegal instruction marker ─────────────────────────
  // The original Chisel decoder marks reserved/illegal compressed
  // encodings by setting opcode bits[4:0] = 5'b11111 (0x1F).
  // This is a reserved opcode in the base spec, guaranteed to trap.

  logic illegal;
  logic [31:0] expanded;

  // ── Expansion logic ───────────────────────────────────

  always_comb begin
    // Default: pass through (for 32-bit instructions and illegal cases)
    expanded = in_bits;
    illegal = 1'b0;
    out_bits = in_bits;

    if (c_op != 2'b11) begin
      unique case ({c_funct3, c_op})

        // ════════════════════════════════════════════════
        // Quadrant 0
        // ════════════════════════════════════════════════

        // C.ADDI4SPN: addi rd', x2, nzuimm
        // CIW-type: rd' = bits[4:2]+8 = c_rs2p
        // Reserved when nzuimm=0 (in[12:5]=0)
        5'b000_00: begin
          // nzuimm[5:4|9:6|2|3] = in[12:5]
          expanded = {2'b0, in_bits[10:7], in_bits[12:11], in_bits[5],
                      in_bits[6], 2'b00,
                      5'd2, 3'b000, c_rs2p, OpImm};
          if (in_bits[12:5] == 8'b0) illegal = 1'b1;
        end

        // C.FLD: fld rd', offset(rs1')  [RV32/64DC]
        // CL-type: rd' = bits[4:2]+8 = c_rs2p, rs1' = bits[9:7]+8 = c_rs1p
        5'b001_00: begin
          // offset[5:3|7:6] = in[12:10|6:5]
          expanded = {4'b0, in_bits[6:5], in_bits[12:10], 3'b000,
                      c_rs1p, 3'b011, c_rs2p, OpLoadFp};
        end

        // C.LW: lw rd', offset(rs1')
        // CL-type: rd' = bits[4:2]+8 = c_rs2p
        5'b010_00: begin
          // offset[5:3|2|6] = in[12:10|6|5]
          expanded = {5'b0, in_bits[5], in_bits[12:10], in_bits[6],
                      2'b00,
                      c_rs1p, 3'b010, c_rs2p, OpLoad};
        end

        // C.LD: ld rd', offset(rs1')  [RV64C]
        // CL-type: rd' = bits[4:2]+8 = c_rs2p
        5'b011_00: begin
          // offset[5:3|7:6] = in[12:10|6:5]
          expanded = {4'b0, in_bits[6:5], in_bits[12:10], 3'b000,
                      c_rs1p, 3'b011, c_rs2p, OpLoad};
        end

        // Reserved (Q0, funct3=100)
        5'b100_00: begin
          expanded = 32'h0000_0000;  // illegal
        end

        // C.FSD: fsd rs2', offset(rs1')  [RV32/64DC]
        // CS-type: rs2' = bits[4:2]+8 = c_rs2p, rs1' = bits[9:7]+8 = c_rs1p
        5'b101_00: begin
          // offset[5:3|7:6] = in[12:10|6:5]
          // S-type: imm[11:5]=bits[31:25], imm[4:0]=bits[11:7]
          expanded = {4'b0, in_bits[6:5], in_bits[12],
                      c_rs2p, c_rs1p, 3'b011,
                      in_bits[11:10], 3'b000, OpStoreFp};
        end

        // C.SW: sw rs2', offset(rs1')
        5'b110_00: begin
          // offset[5:3|2|6] = in[12:10|6|5]
          expanded = {5'b0, in_bits[5], in_bits[12],
                      c_rs2p, c_rs1p, 3'b010,
                      in_bits[11:10], in_bits[6], 2'b00, OpStore};
        end

        // C.SD: sd rs2', offset(rs1')  [RV64C]
        5'b111_00: begin
          // offset[5:3|7:6] = in[12:10|6:5]
          expanded = {4'b0, in_bits[6:5], in_bits[12],
                      c_rs2p, c_rs1p, 3'b011,
                      in_bits[11:10], 3'b000, OpStore};
        end

        // ════════════════════════════════════════════════
        // Quadrant 1
        // ════════════════════════════════════════════════

        // C.NOP / C.ADDI: addi rd, rd, nzimm
        5'b000_01: begin
          // nzimm[5|4:0] = in[12|6:2], sign-extended
          expanded = {{6{in_bits[12]}}, in_bits[12], in_bits[6:2],
                      c_rd, 3'b000, c_rd, OpImm};
        end

        // C.ADDIW: addiw rd, rd, imm  [RV64C]
        // Reserved when rd=x0
        5'b001_01: begin
          // imm[5|4:0] = in[12|6:2], sign-extended
          expanded = {{6{in_bits[12]}}, in_bits[12], in_bits[6:2],
                      c_rd, 3'b000, c_rd, OpImm32};
          if (c_rd == 5'd0) illegal = 1'b1;
        end

        // C.LI: addi rd, x0, imm
        5'b010_01: begin
          // imm[5|4:0] = in[12|6:2], sign-extended
          expanded = {{6{in_bits[12]}}, in_bits[12], in_bits[6:2],
                      5'd0, 3'b000, c_rd, OpImm};
        end

        // C.ADDI16SP / C.LUI
        // Reserved when nzimm=0 (in[12]=0 && in[6:2]=0)
        5'b011_01: begin
          if (c_rd == 5'd2 || c_rd == 5'd0) begin
            // C.ADDI16SP (rd=x2) or C.LUI rd=x0 hint (uses ADDI16SP encoding)
            // Both encode as: addi rd, rd, nzimm
            // nzimm[9|4|6|8:7|5] = in[12|6|5|4:3|2], sign-extended
            expanded = {{2{in_bits[12]}}, in_bits[12], in_bits[4:3],
                        in_bits[5], in_bits[2], in_bits[6], 4'b0000,
                        c_rd, 3'b000, c_rd, OpImm};
          end else begin
            // C.LUI: lui rd, nzimm
            // nzimm[17|16:12] = in[12|6:2], sign-extended
            expanded = {{14{in_bits[12]}}, in_bits[12], in_bits[6:2],
                        c_rd, OpLui};
          end
          if (in_bits[12] == 1'b0 && in_bits[6:2] == 5'b0) illegal = 1'b1;
        end

        // C.SRLI / C.SRAI / C.ANDI / C.SUB / C.XOR / C.OR / C.AND /
        // C.SUBW / C.ADDW
        5'b100_01: begin
          unique case (in_bits[11:10])
            // C.SRLI: srli rd', rd', shamt
            2'b00: begin
              expanded = {6'b000000, in_bits[12], in_bits[6:2],
                          c_rs1p, 3'b101, c_rdp, OpImm};
            end
            // C.SRAI: srai rd', rd', shamt
            2'b01: begin
              expanded = {6'b010000, in_bits[12], in_bits[6:2],
                          c_rs1p, 3'b101, c_rdp, OpImm};
            end
            // C.ANDI: andi rd', rd', imm
            2'b10: begin
              expanded = {{6{in_bits[12]}}, in_bits[12], in_bits[6:2],
                          c_rs1p, 3'b111, c_rdp, OpImm};
            end
            // ALU register-register
            2'b11: begin
              unique case ({in_bits[12], in_bits[6:5]})
                // C.SUB: sub rd', rd', rs2'
                3'b000: expanded = {7'b0100000, c_rs2p, c_rs1p, 3'b000, c_rdp, OpReg};
                // C.XOR: xor rd', rd', rs2'
                3'b001: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b100, c_rdp, OpReg};
                // C.OR: or rd', rd', rs2'
                3'b010: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b110, c_rdp, OpReg};
                // C.AND: and rd', rd', rs2'
                3'b011: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b111, c_rdp, OpReg};
                // C.SUBW: subw rd', rd', rs2'  [RV64C]
                3'b100: expanded = {7'b0100000, c_rs2p, c_rs1p, 3'b000, c_rdp, OpReg32};
                // C.ADDW: addw rd', rd', rs2'  [RV64C]
                3'b101: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b000, c_rdp, OpReg32};
                // Reserved (CORE-ET custom: map to OP-32 with funct3 from bits[6:5])
                3'b110: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b010, c_rdp, OpReg32};
                3'b111: expanded = {7'b0000000, c_rs2p, c_rs1p, 3'b011, c_rdp, OpReg32};
              endcase
            end
          endcase
        end

        // C.J: jal x0, offset
        5'b101_01: begin
          // offset[11|4|9:8|10|6|7|3:1|5] = in[12|11|10:9|8|7|6|5:3|2]
          expanded = {in_bits[12], in_bits[8], in_bits[10:9], in_bits[6],
                      in_bits[7], in_bits[2], in_bits[11], in_bits[5:3],
                      {9{in_bits[12]}},
                      5'd0, OpJal};
        end

        // C.BEQZ: beq rs1', x0, offset
        5'b110_01: begin
          // offset[8|4:3|7:6|2:1|5] = in[12|11:10|6:5|4:3|2]
          // B-type: imm[12|10:5] rs2 rs1 funct3 imm[4:1|11] opcode
          expanded = {{3{in_bits[12]}}, in_bits[12], in_bits[6:5], in_bits[2],
                      5'd0, c_rs1p, 3'b000,
                      in_bits[11:10], in_bits[4:3], in_bits[12], OpBranch};
        end

        // C.BNEZ: bne rs1', x0, offset
        5'b111_01: begin
          // offset[8|4:3|7:6|2:1|5] = in[12|11:10|6:5|4:3|2]
          // B-type: imm[12|10:5] rs2 rs1 funct3 imm[4:1|11] opcode
          expanded = {{3{in_bits[12]}}, in_bits[12], in_bits[6:5], in_bits[2],
                      5'd0, c_rs1p, 3'b001,
                      in_bits[11:10], in_bits[4:3], in_bits[12], OpBranch};
        end

        // ════════════════════════════════════════════════
        // Quadrant 2
        // ════════════════════════════════════════════════

        // C.SLLI: slli rd, rd, shamt
        5'b000_10: begin
          expanded = {6'b000000, in_bits[12], in_bits[6:2],
                      c_rd, 3'b001, c_rd, OpImm};
        end

        // C.FLDSP: fld rd, offset(x2)
        5'b001_10: begin
          // offset[5|4:3|8:6] = in[12|6:5|4:2]
          expanded = {3'b0, in_bits[4:2], in_bits[12], in_bits[6:5], 3'b000,
                      5'd2, 3'b011, c_rd, OpLoadFp};
        end

        // C.LWSP: lw rd, offset(x2)
        // Reserved when rd=x0
        5'b010_10: begin
          // offset[5|4:2|7:6] = in[12|6:4|3:2]
          expanded = {4'b0, in_bits[3:2], in_bits[12], in_bits[6:4], 2'b00,
                      5'd2, 3'b010, c_rd, OpLoad};
          if (c_rd == 5'd0) illegal = 1'b1;
        end

        // C.LDSP: ld rd, offset(x2)  [RV64C]
        // Reserved when rd=x0
        5'b011_10: begin
          // offset[5|4:3|8:6] = in[12|6:5|4:2]
          expanded = {3'b0, in_bits[4:2], in_bits[12], in_bits[6:5], 3'b000,
                      5'd2, 3'b011, c_rd, OpLoad};
          if (c_rd == 5'd0) illegal = 1'b1;
        end

        // C.JR / C.MV / C.EBREAK / C.JALR / C.ADD
        5'b100_10: begin
          if (in_bits[12] == 1'b0) begin
            if (c_rs2 == 5'd0) begin
              // C.JR: jalr x0, rs1, 0
              // Reserved when rs1=x0 (expand as ADDI-based illegal)
              if (c_rd == 5'd0) begin
                expanded = {12'b0, 5'd0, 3'b000, 5'd0, OpImm};
                illegal = 1'b1;
              end else begin
                expanded = {12'b0, c_rd, 3'b000, 5'd0, OpJalr};
              end
            end else begin
              // C.MV: add rd, x0, rs2
              expanded = {7'b0, c_rs2, 5'd0, 3'b000, c_rd, OpReg};
            end
          end else begin
            if (c_rs2 == 5'd0 && c_rd == 5'd0) begin
              // C.EBREAK: ebreak
              expanded = 32'h0010_0073;
            end else if (c_rs2 == 5'd0) begin
              // C.JALR: jalr x1, rs1, 0
              expanded = {12'b0, c_rd, 3'b000, 5'd1, OpJalr};
            end else begin
              // C.ADD: add rd, rd, rs2
              expanded = {7'b0, c_rs2, c_rd, 3'b000, c_rd, OpReg};
            end
          end
        end

        // C.FSDSP: fsd rs2, offset(x2)
        5'b101_10: begin
          // offset[5:3|8:6] = in[12:10|9:7]
          expanded = {3'b0, in_bits[9:7], in_bits[12],
                      c_rs2, 5'd2, 3'b011,
                      in_bits[11:10], 3'b000, OpStoreFp};
        end

        // C.SWSP: sw rs2, offset(x2)
        5'b110_10: begin
          // offset[5:2|7:6] = in[12:9|8:7]
          expanded = {4'b0, in_bits[8:7], in_bits[12],
                      c_rs2, 5'd2, 3'b010,
                      in_bits[11:9], 2'b00, OpStore};
        end

        // C.SDSP: sd rs2, offset(x2)  [RV64C]
        5'b111_10: begin
          // offset[5:3|8:6] = in[12:10|9:7]
          expanded = {3'b0, in_bits[9:7], in_bits[12],
                      c_rs2, 5'd2, 3'b011,
                      in_bits[11:10], 3'b000, OpStore};
        end

        default: begin
          expanded = in_bits;
        end
      endcase

      // Apply illegal marker: replace opcode bits[4:0] with 5'b11111.
      // Special case: all-zero input → all-zero output (defined illegal).
      if (in_bits[15:0] == 16'h0000)
        out_bits = 32'h0000_0000;
      else if (illegal)
        out_bits = {expanded[31:5], 5'b11111};
      else
        out_bits = expanded;
    end
  end

endmodule
