// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_pipe_lru_4w_calc vs new shirecache_pipe_lru_4w_calc.

`include "soc.vh"

module cosim_lru_tb (
  input  logic [4:0]  lru_state_i,
  input  logic [1:0]  hit_way_i,
  input  logic        hit_locked_i,
  input  logic [3:0]  valid_bits_i,
  input  logic [3:0]  locked_bits_i,
  input  logic        locking_way_i,
  input  logic        unlocking_way_i,
  input  logic        update_mru_i,
  input  logic        update_lru_i,

  output logic [4:0]  new_lru_state_o,
  output logic        new_lru_lock_err_o,
  output logic [4:0]  orig_lru_state_o,
  output logic        orig_lru_lock_err_o
);

  // ── New module ────────────────────────────────────────

  shirecache_pipe_lru_4w_calc u_new (
    .lru_state_i,
    .hit_way_i,
    .hit_locked_i,
    .valid_bits_i,
    .locked_bits_i,
    .locking_way_i,
    .unlocking_way_i,
    .update_mru_i,
    .update_lru_i,
    .lru_state_o    (new_lru_state_o),
    .lru_lock_err_o (new_lru_lock_err_o)
  );

  // ── Original module ───────────────────────────────────

  shire_cache_pipe_lru_4w_calc u_orig (
    .lru_state_in   (lru_state_i),
    .hit_way        (hit_way_i),
    .hit_locked     (hit_locked_i),
    .valid_bits     (valid_bits_i),
    .locked_bits    (locked_bits_i),
    .locking_way    (locking_way_i),
    .unlocking_way  (unlocking_way_i),
    .update_mru     (update_mru_i),
    .update_lru     (update_lru_i),
    .lru_state_out  (orig_lru_state_o),
    .lru_lock_err   (orig_lru_lock_err_o)
  );

endmodule
