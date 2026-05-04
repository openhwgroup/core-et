// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RAM configuration type definitions.
//
// Standardized SRAM configuration interface for post-silicon timing
// tuning, test modes, and power management. Replaces the CORE-ET
// esr_shire_cache_ram_cfg_t.
// In simulation and FPGA, ram_cfg_i = '0. For ASIC, these are driven
// by ESR registers.

package ram_cfg_pkg;

  typedef struct packed {
    logic       test_en;     // RAM test mode enable
    logic [3:0] rm;          // Read margin
    logic       rme;         // Read margin enable
    logic [1:0] ra;          // Read assist
    logic [1:0] wa;          // Write assist
    logic [2:0] wpulse;      // Write pulse width
    logic       deep_sleep;  // RAM power-down (low-leakage retention mode)
    logic       shut_down;   // RAM shutdown (data lost)
  } ram_cfg_t;

endpackage
