// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// 4-way LRU replacement calculator.
//
// Maintains a compressed 5-bit LRU state encoding the ordering of
// 4 cache ways from MRU to LRU. Supports MRU promotion (hit),
// LRU demotion (eviction), and locked-way awareness.
//
// Hardcoded for 4 ways (SC_WAYS=4).

module shirecache_pipe_lru_4w_calc
  import shirecache_pkg::*;
(
  input  logic [TagStateLruSize-1:0] lru_state_i,
  input  logic [WayIdSize-1:0]       hit_way_i,
  input  logic                       hit_locked_i,
  input  logic [Ways-1:0]            valid_bits_i,
  input  logic [Ways-1:0]            locked_bits_i,
  input  logic                       locking_way_i,
  input  logic                       unlocking_way_i,
  input  logic                       update_mru_i,
  input  logic                       update_lru_i,
  output logic [TagStateLruSize-1:0] lru_state_o,
  output logic                       lru_lock_err_o
);

  // ── Decode 5-bit compressed → 8-bit full LRU state ────
  // full_lru[7:6] = MRU way, full_lru[1:0] = LRU way.

  logic any_valid;
  assign any_valid = |valid_bits_i;

  logic [2*Ways-1:0] full_lru;
  always_comb begin
    full_lru[3:0] = lru_state_i[3:0];
    if (any_valid) begin
      unique case (lru_state_i)
        5'b00100: full_lru[7:4] = 4'b1110;
        5'b10100: full_lru[7:4] = 4'b1011;
        5'b00001: full_lru[7:4] = 4'b1110;
        5'b10001: full_lru[7:4] = 4'b1011;
        5'b01000: full_lru[7:4] = 4'b1101;
        5'b11000: full_lru[7:4] = 4'b0111;
        5'b00010: full_lru[7:4] = 4'b1101;
        5'b10010: full_lru[7:4] = 4'b0111;
        5'b01100: full_lru[7:4] = 4'b1001;
        5'b11100: full_lru[7:4] = 4'b0110;
        5'b00011: full_lru[7:4] = 4'b1001;
        5'b10011: full_lru[7:4] = 4'b0110;
        5'b01001: full_lru[7:4] = 4'b1100;
        5'b11001: full_lru[7:4] = 4'b0011;
        5'b00110: full_lru[7:4] = 4'b1100;
        5'b10110: full_lru[7:4] = 4'b0011;
        5'b01101: full_lru[7:4] = 4'b1000;
        5'b11101: full_lru[7:4] = 4'b0010;
        5'b00111: full_lru[7:4] = 4'b1000;
        5'b10111: full_lru[7:4] = 4'b0010;
        5'b01110: full_lru[7:4] = 4'b0100;
        5'b11110: full_lru[7:4] = 4'b0001;
        5'b01011: full_lru[7:4] = 4'b0100;
        5'b11011: full_lru[7:4] = 4'b0001;
        default:  full_lru[7:4] = 4'b0000;
      endcase
    end else begin
      full_lru = 8'b00100111; // default if none valid
    end
  end

  // ── LRU update (move hit_way to LRU position) ────────

  logic [Ways-2:0] lru_left_shift_en;
  assign lru_left_shift_en =
    (hit_way_i == full_lru[7:6]) ? 3'b111 :
    (hit_way_i == full_lru[5:4]) ? 3'b011 :
    (hit_way_i == full_lru[3:2]) ? 3'b001 :
                                   3'b000;

  logic [2*Ways-1:0] full_lru_next;
  always_comb begin
    full_lru_next = full_lru;
    full_lru_next[1:0] = hit_way_i;
    for (int unsigned ii = 0; ii < Ways - 1; ii++) begin
      if (lru_left_shift_en[ii]) begin
        full_lru_next[(ii+1)*2 +: 2] = full_lru[ii*2 +: 2];
      end
    end
  end

  // ── MRU update (move hit_way to MRU position) ────────
  // Respects locked ways: a locked way can't be pushed below
  // its current position unless it's being unlocked.

  logic [Ways-2:0] mru_right_shift_prelock_en;
  assign mru_right_shift_prelock_en =
    (hit_way_i == full_lru[1:0]) ? 3'b111 :
    (hit_way_i == full_lru[3:2]) ? 3'b110 :
    (hit_way_i == full_lru[5:4]) ? 3'b100 :
                                   3'b000;

  logic only_one_way_locked;
  assign only_one_way_locked =
    (locked_bits_i == 4'b0001) ||
    (locked_bits_i == 4'b0010) ||
    (locked_bits_i == 4'b0100) ||
    (locked_bits_i == 4'b1000);

  logic [Ways-1:0] ordered_locked;
  always_comb begin
    for (int unsigned ii = 0; ii < Ways; ii++) begin
      ordered_locked[ii] = locked_bits_i[full_lru[2*ii +: 2]];
    end
  end

  logic hit_way_locked, int_hit_locked, unlocking_last_lock;
  assign hit_way_locked     = hit_locked_i;
  assign int_hit_locked     = hit_way_locked && !unlocking_way_i;
  assign unlocking_last_lock = unlocking_way_i && hit_way_locked && only_one_way_locked;

  logic [Ways-1:0] int_ordered_locked;
  assign int_ordered_locked = {Ways{!(locking_way_i || int_hit_locked || unlocking_last_lock)}} & ordered_locked;

  logic [Ways-2:0] mru_right_shift_en;
  assign mru_right_shift_en = mru_right_shift_prelock_en & ~int_ordered_locked[Ways-1:1];

  logic [2*Ways-1:0] full_mru_next;
  always_comb begin
    full_mru_next = full_lru;
    for (int unsigned ii = 0; ii < Ways - 1; ii++) begin
      if (mru_right_shift_en[ii]) begin
        full_mru_next[ii*2 +: 2] = full_lru[(ii+1)*2 +: 2];
      end
    end
    unique case (int_ordered_locked)
      4'b1110: full_mru_next[1:0] = hit_way_i;
      4'b1100: full_mru_next[3:2] = hit_way_i;
      4'b1000: full_mru_next[5:4] = hit_way_i;
      default: full_mru_next[7:6] = hit_way_i;
    endcase
  end

  // ── Lock error detection ──────────────────────────────
  // Can't lock the last unlocked way.

  logic one_unlocked_remains;
  assign one_unlocked_remains =
    (locked_bits_i == 4'b1110) ||
    (locked_bits_i == 4'b1101) ||
    (locked_bits_i == 4'b1011) ||
    (locked_bits_i == 4'b0111);

  assign lru_lock_err_o = locking_way_i && !hit_way_locked && one_unlocked_remains;

  // ── Output selection ──────────────────────────────────

  logic [2*Ways-1:0] full_out;
  assign full_out =
    (update_mru_i && !lru_lock_err_o) ? full_mru_next :
    update_lru_i                      ? full_lru_next :
                                        full_lru;

  // Encode back to 5-bit compressed.
  assign lru_state_o[3:0] = full_out[3:0];
  assign lru_state_o[4]   = (full_out[5:4] < full_out[7:6]) ? 1'b0 : 1'b1;

endmodule : shirecache_pipe_lru_4w_calc
