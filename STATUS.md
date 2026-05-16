# Project Status

## Packages

| Package | Description | Status |
|---------|-------------|--------|
| `dft_pkg` | DFT control struct (`dft_t`: scanmode, scan_reset_n, sram_clk_override, ram_rei, ram_wei) | Done |
| `ram_cfg_pkg` | SRAM config struct (`ram_cfg_t`: timing margins, deep_sleep, shut_down) | Done |
| `etlink_pkg` | ET-Link bus protocol types and opcodes | Done |
| `apb_pkg` | APB3 request/response structs for ESR register blocks | Done |
| `esr_pkg` | Shared ESR structs plus SPIO/neighborhood address constants | Done |
| `icache_geom_pkg` | Shared standalone-Icache geometry and SRAM-layout constants | Done |
| `icache_pkg` | Standalone-Icache constants, enums, TLB/error structs, and LRU helpers | Done |
| `minion_frontend_pkg` | Frontend fetch-buffer, ICache response, issue, and thread-buffer types | Done |
| `neigh_pkg` | Neighborhood shell types and response-agent constants | Done |
| `neigh_hv_logic_pkg` | Neighborhood HV/LV support-leaf APB/BPAM types and sizing constants | Done |
| `neigh_voltage_cross_pkg` | Neighborhood voltage-crossing wrapper local types and geometry constants | Done |
| `shirecache_pkg` | Cache constants, enums, L3 swizzle struct, pipeline/cbuf/atomic types, perfmon types | Done |
| `axi_pkg` | AXI4 channel types (AR, AW, W, R, B) | Done |
| `rbox_pkg` | RBOX constants, packet/control structs, APB/ET-Link helper types, and utility functions | Done |

## Primitives

| Module | Test | Cosim | Status |
|--------|------|-------|--------|
| `prim_clk_gate` | 2056 checks | 4103 comparisons vs `et_clk_gate` | Done |
| `prim_clk_mux` | — | — | Done (no original to cosim, foundry cell) |
| `prim_ram_1p` | 12 checks | 534 comparisons vs `gen_mem1p` | Done |
| `prim_ram_2p` | 21 checks | 1142 comparisons vs `gen_mem2p` | Done |
| `prim_rst_sync` | 9 checks | 25 comparisons vs `rst_repeat` | Done |
| `prim_phase_pair_lo_hi` | 3079 checks | 2048 comparisons vs `et_clk_gate_n` + `rlatchn` -> `et_clk_gate` + `rlatch` | Done |
| `prim_phase_pair_hi_lo` | 3079 checks | 2048 comparisons vs `et_clk_gate` + `rlatch` -> `et_clk_gate_n` + `rlatchn` | Done |
| `prim_write_preview_en` | 1539 checks | 2048 comparisons vs `et_clk_gate_n` + `rlatchn` | Done |
| `prim_write_commit_en` | 1539 checks | 2048 comparisons vs `et_clk_gate` + `rlatch` | Done |
| `prim_write_commit_rst_en` | 1028 checks | 2048 comparisons vs `et_clk_gate` + resettable `rlatch` | Done |
| `prim_mul_div` | 137 checks | 1,517,460 comparisons vs `intpipe_mul_div_top` | Done |
| `prim_rf_1r1w_preview` | 519 checks | 1028 comparisons vs `rlatchn` + `rf_latch_1r_1w` | Done |
| `prim_rf_1r1w_dec_preview` | 2055 checks | 4103 comparisons vs `rlatchn` + `rf_latch_1r_1w_dec` | Done |
| `prim_rf_1r1w_par_preview` | 2055 checks | 4103 comparisons vs `rlatchn` + `rf_latch_1r_1w_par` | Done |
| `prim_rf_1r1w_diff_preview` | 519 checks | 1028 comparisons vs `rlatchn` + `rf_latch_1r_1w_diff_widths` | Done |
| `prim_rf_1r1w_diff` | — (technology primitive body) | 62,427 comparisons vs `rf_latch_1r_1w_diff_widths` | Done |
| `prim_rf_1r1w_reg_preview` | 2057 checks | 4105 comparisons vs `rlatchn` + `rf_latch_1r_1w_reg` | Done |
| `prim_rf_single_1r1w_par_preview` | 2055 checks | 4103 comparisons vs `rlatchn` + `rf_latch_single_1r_1w_par` | Done |
| `prim_rf_2r1w_preview` | 6171 checks | 12315 comparisons vs `rlatchn` + `rf_latch_2r_1w` | Done |
| `prim_rf_3r2w_preview` | 6165 checks | 12309 comparisons vs `rlatchn` + `rf_latch_3r_2w` | Done |
| `prim_eco_ports` | 24 checks | 2048 comparisons vs `et_eco_ports` | Done |
| `prim_fifo_sync` | 28 checks | — (no direct original) | Done |
| `prim_fifo_sram` | 44 checks (Ports=1+2) | 3549 comparisons vs `rbox_fifo` | Done |
| `prim_fifo_reg` | 24 checks | 2091 comparisons vs `gen_fifo_reg` | Done |
| `prim_fifo` | 100 checks | 7122 comparisons vs `gen_fifo` | Done |
| `prim_fifo_async_hiv` | 30 checks | 18,921 comparisons vs `vcfifo_wr_hiv_gcd` | Done |
| `prim_fifo_async_lov` | 63 checks | 18,921 comparisons vs `vcfifo_wr_lov_gcd` | Done |
| `prim_fifo_semisync_hiv` | 50 checks | 7,668 comparisons vs `vcfifo_wr_hiv_ss` | Done |
| `prim_fifo_semisync_lov` | 50 checks | 7,668 comparisons vs `vcfifo_wr_lov_ss` | Done |
| `prim_arb_lru` | — (tested via cbuf) | — (tested via cbuf cosim) | Done |
| `prim_arb_lru_2bid` | 30 checks | 2264 comparisons vs `arb_lru_2_bid` | Done |
| `prim_arb_rr` | 29 checks | 1150 comparisons vs `arb_rr_data` | Done |
| `prim_arb_prio` | 24 checks | 1068 comparisons vs `arb_prio_data_held` | Done |
| `prim_hot2bin` | 311 checks | 12,168 comparisons vs `hot2bin` + `onehot_mux` | Done |
| `prim_rf_1r1w` | 26 checks | 2122 comparisons vs `rf_latch_1r_1w` | Done |
| `prim_rf_1r1w_dec` | 12 checks | 2517 comparisons vs `rf_latch_1r_1w_dec` | Done |
| `prim_rf_1r1w_reg` | 13 checks | 2265 comparisons vs `rf_latch_1r_1w_reg` | Done |
| `prim_rf_1r1w_par` | 12 checks | 17,736 comparisons vs `rf_latch_1r_1w_par` | Done |
| `prim_rf_1r2w_par` | 7 checks | 17,736 comparisons vs `rf_latch_1r_2w_par` | Done |
| `prim_rf_single_1r1w_par` | 4 checks | 2201 comparisons vs `rf_latch_single_1r_1w_par` | Done |
| `prim_rf_3r2w` | 6201 checks | 12918 comparisons vs `rf_latch_3r_2w` | Done |
| `prim_queue_arb_lru` | 6 checks | 2,209 comparisons vs `queue_arb_lru` | Done |
| `prim_arb_rr2` | 907 checks | 6036 comparisons vs `arb_rr2` | Done |
| `prim_ecc_enc` / `prim_ecc_dec` | 294 checks | 1783 comparisons vs `ecc_gen`/`ecc_corr` | Done |

## Shire Cache (`hw/ip/shirecache/`)

### Leaf modules

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `shirecache_pipe_lru_4w_calc` | `shire_cache_pipe_lru_4w_calc` | 12 checks | 9776 comparisons | Done |
| `shirecache_pipe_set_calc` | `shire_cache_pipe_set_calc` | 56 checks | 1808 comparisons | Done |
| `shirecache_pipe_l3_swizzle_get` | `shire_cache_pipe_l3_swizzle_get` | 1536 checks (round-trip TB) | 3584 comparisons (shared) | Done |
| `shirecache_pipe_l3_swizzle_put` | `shire_cache_pipe_l3_swizzle_put` | (same TB) | (same cosim) | Done |
| `shirecache_pipe_sbe_insert` | `shire_cache_pipe_sbe_insert` | — | — | Done — conditionally compiled SBE injection (outputs zero when disabled) |

### Pipeline modules

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `shirecache_pipe_cbuf` | `shire_cache_pipe_cbuf` | 20 checks | 748 comparisons | Done |
| `shirecache_pipe_atomic` | `shire_cache_pipe_atomic` | 13 checks | 20640 comparisons | Done |
| `shirecache_pipe_rbuf` | `shire_cache_pipe_rbuf` | 12 checks | 32004 comparisons | Done |
| `shirecache_pipe_idx_cop_sm` | `shire_cache_pipe_idx_cop_sm` | 17 checks | 322659 comparisons | Done |
| `shirecache_pipe_sub_bank` | `shire_cache_pipe_sub_bank` | — | 22244 comparisons | Done |
| `shirecache_pipe` | `shire_cache_pipe` | — | 16041 comparisons | Done |

### RAM wrappers

Standalone cosims are enabled with `-DET_ASSERT_OFF`; the original wrappers' hierarchical references (`pipe.sub_bank.*` / `mem.*`) are assertion-only.

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `shirecache_pipe_tag_ram_wrap` | `shire_cache_pipe_tag_ram_wrap` | 12 checks | 11,240 comparisons | Done — has dft_sram_clk_i, ram_delay_i, dft_i.ram_rei/wei, ram_cfg_i.deep_sleep/shut_down |
| `shirecache_pipe_tag_state_ram_wrap` | `shire_cache_pipe_tag_state_ram_wrap` | 12 checks | 2,837 comparisons | Done |
| `shirecache_pipe_data_ram_wrap` | `shire_cache_pipe_data_ram_wrap` | 8 checks | 51,858 comparisons | Done |
| `shirecache_pipe_sub_bank_mem` | `shire_cache_pipe_sub_bank_mem` | 7 checks | 90,912 comparisons | Done — variable read-delay pipeline (2/3/4 cycles) |

### Pipeline Build Order

Build order: 1→2→3→4→5→6 (dependencies flow down)

| # | Module | Original | LOC | Depends on | Status |
|---|--------|----------|-----|------------|--------|
| 1 | `shirecache_pipe_cbuf` | `shire_cache_pipe_cbuf` | ~600 | prim_arb_lru | Done |
| 2 | `shirecache_pipe_rbuf` | `shire_cache_pipe_rbuf` | ~800 | prim_fifo_reg, prim_arb_lru | Done |
| 3 | `shirecache_pipe_atomic` | `shire_cache_pipe_atomic` | ~400 | — | Done |
| 4 | `shirecache_pipe_idx_cop_sm` | `shire_cache_pipe_idx_cop_sm` | ~700 | prim_rst_sync | Done |
| 5 | `shirecache_pipe_sub_bank` | `shire_cache_pipe_sub_bank` | 3263 | lru_4w_calc, set_calc, l3_swizzle, atomic | Done |
| 6 | `shirecache_pipe` | `shire_cache_pipe` | ~1200 | sub_bank (×4), sub_bank_mem (×4), cbuf, rbuf, idx_cop_sm | Done |

### Request Queue Build Order

Build order: 7→8→9, 10, 11 (then combine)

| # | Module | Original | LOC | Depends on | Status |
|---|--------|----------|-----|------------|--------|
| 7 | `shirecache_bank_reqq_alloc` | `shire_cache_bank_reqq_alloc` | 1247 | prim_arb_rr2 | Done |
| 8 | `shirecache_bank_reqq_entry` | `shire_cache_bank_reqq_entry` | 3889 | l3_swizzle_get | Done |
| 9 | `shirecache_bank_reqq` | `shire_cache_bank_reqq` | 1694 | reqq_entry, reqq_alloc, prim_fifo_reg | Done |
| 10 | `shirecache_bank_dataq` | `shire_cache_bank_dataq` | ~500 | prim_rf_1r1w, prim_arb_rr, prim_arb_prio, prim_fifo_reg | Done |
| 11 | `shirecache_bank_rspmux` | `shire_cache_bank_rspmux` + `_xbar_arb` | ~300 | — | Done |

### Integration

Build order: 12→13→14→15→16→17→18

| # | Module | Original | Depends on | Status |
|---|--------|----------|------------|--------|
| 12 | `axi_pkg` | — | — | Done |
| 13 | `shirecache_xbar` | `shire_cache_xbar` | prim_fifo_reg, prim_arb_rr | Done |
| 14 | `shirecache_bank_mesh` | `shire_cache_bank_mesh` | prim_fifo_reg, prim_arb_prio | Done — 46 unit checks, 18682 cosim comparisons (incl. 512b data/addr/ben) |
| 15 | `shirecache_bank_l3_slave` | `shire_cache_bank_l3_slave` | prim_fifo_reg, prim_arb_rr | Done — 58 unit checks, 8849 cosim comparisons (incl. 512b data/addr/ben) |
| 16 | `shirecache_mesh_master` + `shirecache_mesh_master_axi_port` | `shire_cache_mesh_master` + `_axi_port` | prim_fifo_async_hiv/lov, prim_fifo_reg, prim_arb_rr, prim_clk_gate, axi_pkg, l3_swizzle_get | Done — 42 unit checks, 5327 cosim comparisons |
| 17 | `shirecache_mesh_slave` + `shirecache_mesh_slave_axi_port` | `shire_cache_mesh_slave` + `_axi_port` | prim_fifo_async_hiv/lov, prim_fifo_reg, prim_arb_rr, axi_pkg, l3_swizzle_get | Done — 21 unit checks, 6272 cosim comparisons |
| 18 | `shirecache_l3_to_sys_bridge` | `shire_cache_l3_to_sys_bridge` | prim_fifo_async_hiv/lov, prim_arb_rr, prim_hot2bin | Done — 41 unit checks, 14697 cosim comparisons |
| 19 | `shirecache_bank` | `shire_cache_bank` | pipe, reqq, dataq, rspmux, mesh, l3_slave, perfmon, err_logger, l2hpf, trace, bist_wrapper | Done — 5535 cosim comparisons (full hierarchy) |
| 20 | `shirecache_top` | `shire_cache` | bank (×4), xbar (×2), mesh_master, mesh_slave, rst_sync | Done — 7 free-clock smoke checks, 10 MBIST propagation checks; 46102 cosim comparisons (full hierarchy, all 4 banks, DFT SRAM clock override, DFT MBIST enable propagation) |

### Monitoring

| # | Module | Original | Test | Cosim | Status |
|---|--------|----------|------|-------|--------|
| 21 | `shirecache_bank_perfmon_qual` | `shire_cache_bank_perfmon_qual` | 83 checks | 2480 comparisons | Done — event/resource qualifier, IsP0/P1 modes |
| 21b | `shirecache_perfmon_ctl` | `shire_cache_perfmon_ctl` | — (tested via perfmon) | — (tested via perfmon cosim) | Done — counter control FSMs, ESR interface |
| 21c | `shirecache_bank_perfmon` | `shire_cache_bank_perfmon` | 32 checks | 1605 comparisons | Done — performance counter top (wraps ctl + 2x qual) |
| 22 | `shirecache_bank_err_logger` | `shire_cache_bank_err_logger` | 69 checks | 2374 comparisons | Done — 12-source error logger with priority arb |
| 23 | `shirecache_bank_trace` | `shire_cache_bank_trace` | 41 checks | 1028 comparisons | Done — 6 trace snippet types, segment packing |
| 24 | `shirecache_bank_l2hpf` | `shire_cache_bank_l2hpf` | 23 checks | 1011 comparisons | Done — L2 HPF interface monitor (L2HpfImplemented=1) |
| 25 | `shirecache_bist_mbx` | `shire_cache_bist_mbx` | 27 checks | 1890 comparisons | Done — BIST mailbox with back-to-back blocking |
| 25b | `shirecache_bist_wrapper` | `shire_cache_bist_wrapper` | 40 checks | 9064 comparisons | Done — BIST wrapper (4x mbx: MBS/MBT/MBD/MBI) |

## Minion Frontend (`hw/ip/minion/frontend/`)

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `minion_frontend_rvc_expander` | `frontend_rvc_expander` | 30 checks | 53,260 comparisons | Done |
| `minion_frontend_thread_sched` | `frontend_thread_sched` | — | 5,540 comparisons | Done |
| `minion_frontend_thread_buffer` | `frontend_thread_buffer` | — | 1,524,292 comparisons | Done |
| `intpipe_decode` | `intpipe_decode` | 6 checks (`EnableExtraTrans=0`) + 6 checks (`EnableExtraTrans=1`) | 2,000,069 comparisons (`EnableExtraTrans=0`) + 2,000,069 comparisons (`EnableExtraTrans=1`) | Done |
| `vpu_decoder` | `vpu_decoder` | 6 checks (`EnableExtraTrans=0`) + 6 checks (`EnableExtraTrans=1`) | 4,000,110 comparisons (`EnableExtraTrans=0`) + 4,000,110 comparisons (`EnableExtraTrans=1`) | Done |
| `minion_frontend` | `frontend_top` | — | 1,169,262 comparisons | Done |

## Minion Integer Pipeline (`hw/ip/minion/intpipe/`)

### Packages

| Package | Description | Status |
|---------|-------------|--------|
| `minion_pkg` | Shared minion types (ALU, decode, dcache, VPU, CSR, scoreboard, interrupt, debug) | Done |
| `intpipe_csr_pkg` | CSR type definitions and address enum (auto-generated from CSV) | Done |

### Phase 1-3: Leaf modules

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `intpipe_alu` | `intpipe_alu` | 44 checks | ~2M comparisons | Done |
| `intpipe_imm` | `intpipe_imm` | — | ~500K comparisons | Done |
| `intpipe_inst_bits_stage` | `intpipe_inst_bits_stage` | — | ~100K comparisons | Done |
| `intpipe_rf` | `intpipe_rf` | — | 80,270 comparisons | Done |
| `intpipe_int_scoreboard` | `intpipe_int_scoreboard` | 27 checks | ~1M comparisons | Done |
| `intpipe_fp_scoreboard` | `intpipe_fp_scoreboard` | 14 checks | ~1M comparisons | Done |
| `intpipe_mask_scoreboard` | `intpipe_mask_scoreboard` | 12 checks | ~1M comparisons | Done |
| `debug_breakpoint` | `debug_breakpoint` | 112 checks | ~1M comparisons | Done |
| `intpipe_mul_div_dp` | `intpipe_mul_div_dp` | — | ~16M comparisons | Done |
| `intpipe_mul_div_ctl` | `intpipe_mul_div_ctl` | — | ~16M comparisons | Done |
| `intpipe_mul_div_top` | `intpipe_mul_div_top` | — | ~16M comparisons | Done |
| `prim_rf_2r1w` | `rf_latch_2r_1w` | — | 224,876 comparisons | Done |
| `prim_cmp_32` | `r32cmp` | — | — (tested via mul_div_dp cosim) | Done |
| `prim_cmp_42` | `r42cmp` | — | — (tested via mul_div_dp cosim) | Done |
| `prim_clk_gate_n` | `et_clk_gate_n` | — | — (tested via mul_div_dp cosim) | Done |

### Phase 4: CSR subsystem

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `intpipe_csr_file_fl_barrier` | `intpipe_csr_file_fl_barrier` | — | 62,808 comparisons | Done |
| `intpipe_csr_file_conv` | `intpipe_csr_file_conv` | — | 22,440 comparisons | Done |
| `intpipe_csr_pmu_read_interface` | `intpipe_csr_pmu_read_interface` | — | 21,356 comparisons | Done |
| `intpipe_csr_replay` | `intpipe_csr_replay` | 122 checks (`VpuEn=1`) + 122 checks (`VpuEn=0`) | 10,466 comparisons | Done |
| `intpipe_csr_msgs` | `intpipe_csr_msgs` | 36 checks | 33,770 comparisons | Done |
| `intpipe_csr_file` | `intpipe_csr_file` | — | 187,040 comparisons | Done |

### Phase 5: Top integration

| Module | Original | Cosim | Status |
|--------|----------|-------|--------|
| `intpipe_top` | `intpipe_top` | 198,891 comparisons | Done |

## Minion Core Integration (`hw/ip/minion/rtl/`)

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `minion_debug_apb_slv` | `minion_debug_apb_slv` | 246 standalone checks + covered by `core_top` smoke test (9 checks) | 91,251 standalone comparisons + covered by `core_top` cosim (1,539,072 comparisons) | Done (standalone block-level DV/cosim plus minion-level integration coverage) |
| `core_top` | `core_top` | 9 smoke checks + 12 debug-APB-off checks | 1,539,072 comparisons | Done |
| `minion_top` | `minion_top` | 17 smoke checks + 5 VpuEn=0 checks + 17 debug-APB-off checks + 15 debug-off checks + 9 execution checks | 235,284 comparisons | Done (default `VpuEn=1` instantiates translated real `vpu_top`; cosim checks every practical functional input bit sees both 0 and 1, with DFT scan/OCC/test pins documented as constrained; intentional integer-only `VpuEn=0` path keeps `null_vpu`) |
| `null_vpu` | — | 55 checks | — | Done (new integer-only bring-up helper; intentionally non-faithful and not part of the CORE-ET translation set) |

## Minion VPU (`hw/ip/minion/vpu/`)

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `vpu_pkg` | internal VPU-only types from `vpu_types.vh` + `trans_types.vh` | — | — | Done |
| `vpu_defs_pkg` | selected constants/aliases from original VPU include stack | — | — | RTL present for TXFMA top-half support |
| `vpu_bypass` | `vpu_bypass` | 25 checks | 98,412 comparisons | Done |
| `vpu_mask` | `vpu_mask` | 15 checks | 57,827 comparisons | Done |
| `vpu_rf` | `vpu_rf` | 12,391 checks (`UseMmi=0`) + 12,391 checks (`UseMmi=1`) | 49,251 comparisons (`UseMmi=0`) + 49,251 comparisons (`UseMmi=1`) | Done |
| `vpu_sh_sw` | `vpu_sh_sw` | 87 checks | 30,697 comparisons | Done |
| `txfma_csa` | `txfma_csa` | 522 checks | 50,266 comparisons | Done |
| `txfma_4_2_compressor` | `txfma_4_2_compressor` | 3,168 checks | 12,384 comparisons | Done |
| `txfma_4_2_compressor_array` | `txfma_4_2_compressor_array` | 524 checks | 16,396 comparisons | Done |
| `txfma_booth_ppg_32r4_norm` | `txfma_booth_ppg_32r4_norm` | 608 checks | 25,608 comparisons | Done |
| `txfma_booth_ppg_32r4_msb` | `txfma_booth_ppg_32r4_msb` | 536 checks | 25,152 comparisons | Done |
| `txfma_booth_ppg_32r4` | `txfma_booth_ppg_32r4` | 9,842 checks | 155,762 comparisons | Done |
| `txfma_wallace1` | `txfma_wallace1` | 8,216 checks | 25,288 comparisons | Done |
| `txfma_wallace2` | `txfma_wallace2` | 8,200 checks | 25,074 comparisons | Done |
| `txfma_c0` | `txfma_c0` | 903 checks | 9,101,638 comparisons | Done |
| `txfma_c1` | `txfma_c1` | 295 checks | 65,648 comparisons | Done |
| `txfma_c2` | `txfma_c2` | 312 checks | 192,888 comparisons | Done |
| `txfma_c3` | `txfma_c3` | 351 checks | 61,545 comparisons | Done |
| `txfma_c4` | `txfma_c4` | 283 checks | 32,824 comparisons | Done |
| `txfma_c5` | `txfma_c5` | 301 checks | 90,266 comparisons | Done |
| `txfma_c6` | `txfma_c6` | 386 checks | 188,784 comparisons | Done |
| `txfma_e1` | `txfma_e1` | 4,224 checks | 40,088 comparisons | Done |
| `txfma_e2` | `txfma_e2` | 16,614 checks | 80,192 comparisons | Done |
| `txfma_e4` | `txfma_e4` | 8 checks | 2,048 comparisons | Done |
| `txfma_e5` | `txfma_e5` | 3,157 checks | 15,027 comparisons | Done |
| `txfma_e6` | `txfma_e6` | 10,382 checks | 50,130 comparisons | Done |
| `txfma_ediff_opdorder_logic` | `txfma_ediff_opdorder_logic` | 14,534 checks | 70,182 comparisons | Done |
| `txfma_exp_special_detect` | `txfma_exp_special_detect` | 10,416 checks | 50,160 comparisons | Done |
| `txfma_f0` | `txfma_f0` | 27 checks | 50,985 comparisons | Done |
| `txfma_f1` | `txfma_f1` | 13 checks | 25,260 comparisons | Done |
| `txfma_f2` | `txfma_f2` | 9 checks | 25,053 comparisons | Done |
| `txfma_f3` | `txfma_f3` | 12 checks | 50,106 comparisons | Done |
| `txfma_f4` | `txfma_f4` | 10 checks | 25,090 comparisons | Done |
| `txfma_f5` | `txfma_f5` | 16 checks | 50,310 comparisons | Done |
| `txfma_f6` | `txfma_f6` | 5 checks | 50,036 comparisons | Done |
| `txfma_frac_zero_detect` | `txfma_frac_zero_detect` | 4,101 checks | 2,502 comparisons | Done |
| `txfma_rnd_adder` | `txfma_rnd_adder` | 4,166 checks | 2,502 comparisons | Done |
| `txfmactl_top` | `txfmactl_top` | 79 checks | 996,194 comparisons | Done |
| `txfmaexp_top` | `txfmaexp_top` | 31 checks | 508,152 comparisons | Done |
| `txfmafrac_top` | `txfmafrac_top` | 43 checks | 390,468 comparisons | Done |
| `txfma_top` | `txfma_top` | 9 checks | 15,024 comparisons | Done |
| `txfma_top_fake` | `txfma_top_fake` | 18 checks | 15,024 comparisons | Done |
| `txfma_lxd` | `txfma_lxd` | 4,102 checks | 25,162 comparisons | Done |
| `txfma_trz` | `txfma_trz` | 4,102 checks | 25,054 comparisons | Done |
| `txfma_trz2` | `txfma_trz2` | 4,102 checks | 25,162 comparisons | Done |
| `txfma_adder` | `txfma_adder` | 4,101 checks | 25,197 comparisons | Done |
| `txfma_align_shf` | `txfma_align_shf` | 4,102 checks | 25,566 comparisons | Done |
| `txfma_norm_shf` | `txfma_norm_shf` | 4,104 checks | 25,234 comparisons | Done |
| `tima_adder` | `tima_adder` | 69 checks | 25,068 comparisons | Done |
| `tima_top` | `tima_top` | 8,205 checks | 50,018 comparisons | Done |
| `trans_exp_rom_case_c0` | `trans_exp_rom_case_c0` | — | 257 comparisons | Done (cosim only) |
| `trans_exp_rom_case_c1c2` | `trans_exp_rom_case_c1c2` | — | 257 comparisons | Done (cosim only) |
| `trans_log_rom_case_c0` | `trans_log_rom_case_c0` | — | 257 comparisons | Done (cosim only) |
| `trans_log_rom_case_c1c2` | `trans_log_rom_case_c1c2` | — | 257 comparisons | Done (cosim only) |
| `trans_rcp_rom_case_c0` | `trans_rcp_rom_case_c0` | — | 257 comparisons | Done (cosim only) |
| `trans_rcp_rom_case_c1c2` | `trans_rcp_rom_case_c1c2` | — | 257 comparisons | Done (cosim only) |
| `trans_rsqrt_rom_case_c0` | `trans_rsqrt_rom_case_c0` | — | 257 comparisons | Done (cosim only) |
| `trans_rsqrt_rom_case_c1c2` | `trans_rsqrt_rom_case_c1c2` | — | 257 comparisons | Done (cosim only) |
| `trans_sin_rom_case_c0` | `trans_sin_rom_case_c0` | — | 257 comparisons | Done (cosim only) |
| `trans_sin_rom_case_c1c2` | `trans_sin_rom_case_c1c2` | — | 257 comparisons | Done (cosim only) |
| `trans_top` | `trans_top` | 8,344 checks (`EnableExtraTrans=0`) + 8,372 checks (`EnableExtraTrans=1`) | 65,626 comparisons (`EnableExtraTrans=0`) + 65,659 comparisons (`EnableExtraTrans=1`) | Done |
| `vpu_trans` | `vpu_trans` | 49 checks (`EnableExtraTrans=0`) + 66 checks (`EnableExtraTrans=1`) | 900,990 comparisons (`EnableExtraTrans=0`) + 900,990 comparisons (`EnableExtraTrans=1`) | Done |
| `vpu_tensora_rf` | `vpu_tensora_rf` | 5 checks | 50,010 comparisons | Done |
| `vpu_tensorb_rf` | `vpu_tensorb_rf` | 5 checks | 50,010 comparisons | Done |
| `vpu_tensorc_rf` | `vpu_tensorc_rf` | 5 checks | 50,010 comparisons | Done |
| `vpu_tensortmp_rf` | `vpu_tensortmp_rf` | 5 checks | 50,010 comparisons | Done |
| `vpu_lane_tima` | `vpu_lane_tima` | 200,066 checks | 200,040 comparisons | Done |
| `vpu_uinst_decoder` | `vpu_uinst_decoder` | 19 checks (`EnableExtraTrans=0`) + 19 checks (`EnableExtraTrans=1`) | 50,294 comparisons (`EnableExtraTrans=0`) + 50,294 comparisons (`EnableExtraTrans=1`) | Done |
| `vpu_tensorreduce` | `vpu_tensorreduce` | 27 checks | 15,030 comparisons | Done |
| `vpu_txfma_trans_top` | `vpu_txfma_trans_top` | 16 checks (`UseFakeTxfma=0`) + 13 checks (`UseFakeTxfma=1`) | 524,588 comparisons (`UseFakeTxfma=0`) + 262,444 comparisons (`UseFakeTxfma=1`) | Done |
| `vpu_tensorfma` | `vpu_tensorfma` | 24 checks | 551,854 comparisons | Done |
| `vpu_tensorquant` | `vpu_tensorquant` | 41 checks | 259,600 comparisons | Done |
| `vpu_ml` | `vpu_ml` | 26 checks | 602,896 comparisons | Done |
| `vpu_ctrl` | `vpu_ctrl` | 25 checks | 2,019,808 comparisons | Done |
| `vpu_lane` | `vpu_lane` | 16 checks | 780,108 comparisons | Done |
| `vpu_top` | `vpu_top` | 53 checks | 4,947,579 comparisons | Done (standalone VPU-local DV/cosim with directed protocol/mode coverage plus real default `minion_top` integration) |

## Standalone Minion Shell

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `sp_apb_mux` | `sp_apb_mux` | 28 checks | 230,253 comparisons | Done |
| `neigh_miss_ff` | `neigh_miss_ff` | 45 checks | 81,135 comparisons | Done |
| `neigh_evict_ff_unit` | `neigh_evict_ff_unit` | 30 checks | 78,312 comparisons | Done |
| `neigh_evict_ff` | `neigh_evict_ff` | 27 checks | 117,624 comparisons | Done |
| `neigh_fill_ff` | `neigh_fill_ff` | 33 checks | 68,204 comparisons | Done |
| `neigh_fill_fifo` | `neigh_fill_fifo` | 17 checks | 130,761 comparisons | Done |
| `lru_sel` | `lru_sel` | 11 checks | 5,001 comparisons | Done |
| `icache_lru_array` | `icache_lru_array` | 2,056 checks | 5,000 comparisons | Done |
| `icache_tag_array` | `icache_tag_array` | 30 checks | 80,004 comparisons | Done |
| `icache_pma_unit` | `icache_pma_unit` | 256 checks | 12,002 comparisons | Done |
| `icache_micro_tag_array` | `icache_micro_tag_array` | 23 checks | 80,004 comparisons | Done |
| `icache_micro_data_array` | `icache_micro_data_array` | 12 checks | 80,004 comparisons | Done |
| `icache_micro_cache` | `icache_micro_cache` | 28 checks | 580,841 comparisons | Done |
| `icache_data_array` | `icache_data_array` | 34 checks | 80,512 comparisons | Done |
| `icache_data_ram_wrap` | `icache_data_ram_wrap` | 26 checks | 85,918 comparisons | Done |
| `icache_tlb_array` | `icache_tlb_array` | 18 checks | 44,066 comparisons | Done |
| `icache_top` | `icache_top` | 29 checks | 1,261,827 comparisons | Done |
| `ptw_top` | `ptw_top` | 32 checks | 72,288 comparisons | Done |
| `neigh_shared_ptw` | `neigh_shared_ptw` | 18 checks | 45,090 comparisons | Done |
| `pseudo_lru` | `pseudo_lru` | 266 checks | 5,000 comparisons | Done |
| `esr_spio` | `esr_spio` | 13 checks | 29,955 comparisons | Done |
| `esr_neigh` | `esr_neigh` | 129 checks | 98,096 comparisons | Done |
| `neigh_hv_logic_apb_req` | `neigh_hv_logic_apb_req` | 57 grouped support-leaf checks | 4,096 comparisons | Done |
| `neigh_hv_logic_apb_rsp` | `neigh_hv_logic_apb_rsp` | 57 grouped support-leaf checks | 6,144 comparisons | Done |
| `neigh_hv_logic_bpam` | `neigh_hv_logic_bpam` | 57 grouped support-leaf checks | 4,096 comparisons | Done |
| `neigh_hv_logic_bpam_rc_tbox_ack` | `neigh_hv_logic_bpam_rc_tbox_ack` | 57 grouped support-leaf checks | 8,192 comparisons | Done |
| `neigh_hv_logic_clock_feedback` | `neigh_hv_logic_clock_feedback` | 57 grouped support-leaf checks | 4,096 comparisons | Done |
| `neigh_hv_logic_constants` | `neigh_hv_logic_constants` | 57 grouped support-leaf checks | 2,048 comparisons | Done |
| `neigh_hv_logic_icache_err` | `neigh_hv_logic_icache_err` | 57 grouped support-leaf checks | 4,096 comparisons | Done |
| `neigh_hv_logic_ipi` | `neigh_hv_logic_ipi` | 57 grouped support-leaf checks | 2,048 comparisons | Done |
| `neigh_hv_logic_l2_bank` | `neigh_hv_logic_l2_bank` | 57 grouped support-leaf checks | 155,648 comparisons | Done |
| `neigh_hv_logic_neigh_sc` | `neigh_hv_logic_neigh_sc` | 57 grouped support-leaf checks | 163,840 comparisons | Done |
| `neigh_hv_logic_pwr_ctrl_tdr_isolate` | `neigh_hv_logic_pwr_ctrl_tdr_isolate` | 57 grouped support-leaf checks | 2,048 comparisons | Done |
| `neigh_hv_logic_uc_fcc` | `neigh_hv_logic_uc_fcc` | 57 grouped support-leaf checks | 2,048 comparisons | Done |
| `neigh_hv_logic_fcc` | `neigh_hv_logic_fcc` | — | — | Excluded — no instantiation in audited `neigh_top`/`neigh_channel` hierarchy; live path uses `neigh_hv_logic_uc_fcc` |
| `neigh_hi_voltage_cross` | `neigh_hi_voltage_cross` | 41 grouped crossing-wrapper checks | 144,496 comparisons | Done |
| `neigh_lo_voltage_cross` | `neigh_lo_voltage_cross` | 41 grouped crossing-wrapper checks | 59,744 comparisons | Done |
| `dll_dly_est_core` | `dll_dly_est_core` | 26 grouped estimator checks | 13,716 comparisons | Done |
| `dll_dly_est` | `dll_dly_est` | 26 grouped estimator checks | 6,166 default-width comparisons + 6,166 Width=4 wrapper-parameter comparisons | Done |
| `standalone_minion` | `standalone_minion` | 19 checks | 1,765,940 comparisons | Done |

## RBOX (`hw/ip/rbox/`)

RBOX status was added from the tracked RTL/DV tree on 2026-05-04. The exact
counts below are from local unit-test runs and Makefile cosim runs with
`ORIG_ROOT` pointing at a local original CORE-ET RTL checkout:
16 unit suites, 129 checks, 17 standalone cosims, 2,903,528 comparisons, 0
observed failures.

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `rbox_step_mul` | `rbox_step_mul` | 5 checks | 2,137 comparisons | Done |
| `rbox_depth_to_fp` | `rbox_depth_to_fp` | 8 checks | 3,108 comparisons | Done |
| `rbox_depth_to_un` | `rbox_depth_to_un` | 8 checks | 5,285 comparisons | Done |
| `rbox_sample_to_fp32` | `rbox_sample_to_fp32` | 4 checks | 5,667 comparisons | Done |
| `rbox_fp_shifter` | Extracted helper from RBOX FP adders | Covered by FP adder/depth-offset tests | Covered by FP adder/depth-offset cosims | Done |
| `rbox_fp_fast_adder` | `rbox_fp_fast_adder` | 15 checks | 45,216 comparisons | Done |
| `rbox_fp_adder_2stg` | `rbox_fp_adder_2stg` | 11 checks | 45,135 comparisons | Done |
| `rbox_sampler` | `rbox_sampler` | 4 checks | 3,803 comparisons | Done |
| `rbox_depth_offset` | `rbox_depth_offset` | 7 checks | 40,851 comparisons | Done |
| `rbox_input_fifo` | `rbox_input_fifo` | 9 checks | 7,200 comparisons | Done |
| `rbox_output_buffer` | `rbox_output_buffer` | 8 checks | 2,734 comparisons | Done |
| `rbox_depth_buffer` | `rbox_depth_buffer` | 7 checks | 29,893 comparisons | Done |
| `rbox_test` | `rbox_test` | 3 checks | 974,060 comparisons | Done |
| `rbox_traverse` | `rbox_traverse` | 7 checks | 796,096 comparisons | Done |
| `rbox_ctrl` | `rbox_ctrl` | 4 checks | 785,095 comparisons | Done |
| `rbox_esr` | `rbox_esr` | — (covered by top/debug integration) | 14,943 comparisons | Done |
| `rbox_dbg_ram` | `rbox_dbg_ram` | 18 checks | 7,821 comparisons | Done |
| `rbox_top` | `rbox_top` | 11 checks | 134,484 comparisons | Done |

## ESR/CSR Generator

| Component | Description | Status |
|-----------|-------------|--------|
| `hw/ip/esr/scripts/gen_esr.py` | Unified ESR/CSR register generator (reads CSV, produces SV) | Done |
| `hw/ip/esr/data/esr.csv` | ET System Register definitions (rbox, neigh, cache, etc.) | Done |
| `hw/ip/minion/intpipe/csr/data/csr.csv` | CSR register definitions (211 RISC-V + custom CSRs) | Done |
| `hw/ip/minion/intpipe/csr/data/csr_types/` | 26 CSR struct type definitions (CSV format) | Done |

## Minion DCache (`hw/ip/minion/dcache/`)

Standalone dcache status: complete for the original non-graphics minion
configuration. The only intentionally deferred branch is the original
`GFX_ENABLED` texsend path, which remains called out on the `minion_dcache_texsend`
row below.

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `minion_dcache_pkg` | shared minion/dcache types from `soc.vh` | — | — | Done |
| `minion_dcache_atomic_alu_unit` | `dcache_atomic_alu_unit` | 108 checks | 4,113 comparisons | Done |
| `minion_dcache_store_merge_unit` | `dcache_store_merge_unit` | 608 checks | 8,328 comparisons | Done |
| `minion_dcache_pma_unit` | `dcache_pma_unit` | 352 checks | 4,130 comparisons | Done |
| `minion_dcache_lru_array` | `dcache_lru_array` | 148 checks | 4,129 comparisons | Done |
| `minion_dcache_metadata_array` | `dcache_metadata_array` | 2,349 checks | 37,161 comparisons | Done |
| `minion_dcache_buffer_array` | `dcache_buffer_array` | 36 checks | 12,492 comparisons | Done |
| `minion_dcache_128x64_1r1w_lram` | `dcache_128x64_1r1w_lram` | 264 checks | 4,101 comparisons | Done |
| `minion_dcache_128x72_1r1w_lram` | `dcache_128x72_1r1w_lram` | 528 checks | 8,202 comparisons | Done |
| `minion_dcache_data_array` | `dcache_data_array` | 4,112 checks | 16,936 comparisons | Done |
| `minion_dcache_tlb_array` | `dcache_tlb_array` | 25 checks | 73,860 comparisons | Done |
| `minion_dcache_replay_queue` | `dcache_replay_queue` | 24 checks | 175,598 comparisons | Done |
| `minion_dcache_miss_handler` | `dcache_miss_handler` | 27 checks | 325,796 comparisons | Done |
| `minion_dcache_miss_handler_unit` | `dcache_miss_handler_unit` | 27 checks | 357,831 comparisons | Done |
| `minion_dcache_cache_op_unit_l2` | `dcache_cache_op_unit_l2` | 31 checks | 172,494 comparisons | Done |
| `minion_dcache_cache_op_unit` | `dcache_cache_op_unit` | 34 checks | 1,070,403 comparisons | Done |
| `minion_dcache_writeback_unit` | `dcache_writeback_unit` | 34 checks | 49,704 comparisons | Done |
| `minion_dcache_scratchpad_ctrl` | `dcache_scratchpad_ctrl` | 18 checks | 18,090 comparisons | Done |
| `minion_dcache_texsend` | `dcache_texsend` | 11 checks | 24,608 comparisons | Done (`!GFX_ENABLED` stub path only; graphics/Tbox support still requires porting the original `GFX_ENABLED` path or a dedicated graphics-enabled variant) |
| `minion_dcache_reduce` | `dcache_reduce` | 13 checks | 806,077 comparisons | Done |
| `minion_dcache_tensor_load` | `dcache_tensor_load` | 19 checks | 4,248,708 comparisons | Done |
| `minion_dcache_top` | `dcache_top` | 73 checks + 14 debug-APB-off checks | 1,425,906 comparisons | Done |

## Minion TLB (`hw/ip/minion/tlb/`)

| Module | Original | Test | Cosim | Status |
|--------|----------|------|-------|--------|
| `tlb_pkg` | TLB-local cache-entry + FSM types from `soc.vh` | — | — | Done |
| `minion_tlb` | `tlb_top` | 123 checks | 517,146 comparisons | Done |

## Totals

Structural discovery counts were refreshed from tracked Makefiles on
2026-05-16. Repo-wide check/comparison totals are not carried as exact sums
until every unit test and cosim is rerun and all legacy approximate rows are
backfilled.

| Metric | Count |
|--------|-------|
| Unit-test Makefiles | 72 |
| Test suites discovered by `make test` | 229 |
| RTL cosim Makefiles discovered by `make -C dv/rtlcosim test` | 264 |
| Total checks | Not maintained as an exact repo-wide sum in this file |
| Total comparisons | Not maintained as an exact repo-wide sum in this file |
| Targeted update runs | 228 unit suites + 264 cosim Makefile runs + 1 Width=4 wrapper-parameter cosim run |
| Targeted update failures | 0 |
