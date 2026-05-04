# Minion Frontend

Instruction fetch, decode, and issue for the Minion RV64IMFC + Zicsr + Zifencei core.

Replaces: `frontend_top` from the CORE-ET source tree.

## Architecture

Two-thread barrel-processor frontend with 7-stage ICache pipeline (F0-F6), instruction decode at F7, and 2-entry per-thread instruction FIFOs feeding the ID stage.

### Sub-modules

| Module | File | Replaces |
|--------|------|----------|
| `minion_frontend` | `rtl/minion_frontend.sv` | `frontend_top` |
| `minion_frontend_thread_buffer` | `rtl/minion_frontend_thread_buffer.sv` | `frontend_thread_buffer` |
| `minion_frontend_thread_sched` | `rtl/minion_frontend_thread_sched.sv` | `frontend_thread_sched` |
| `minion_frontend_rvc_expander` | `rtl/minion_frontend_rvc_expander.sv` | `frontend_rvc_expander` |
| `intpipe_decode` | `rtl/intpipe_decode.sv` | `intpipe_decode` |
| `vpu_decoder` | `rtl/vpu_decoder.sv` | `vpu_decoder` |
| `minion_frontend_pkg` | `rtl/minion_frontend_pkg.sv` | Types from `soc.vh` includes |

### Pipeline stages

- **F0**: PC generation, ICache request, buffer selection
- **F1**: ICache request pipeline (stall-gated)
- **F2-F5**: ICache pipeline stages
- **F6**: ICache response write to fetch buffer + instruction read/expand
- **F7**: RVC expansion, intpipe decode, VPU decode, FIFO push
- **ID**: Thread scheduling, FIFO read, instruction issue

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NrThreads` | 2 | Number of hardware threads |
| `EnableExtraTrans` | 0 | Enables the original `ENABLE_EXTRA_TRANS` decode cases (`FRSQ_PS`, `FSIN_PS`) in both shared decoders |

## Port interface

### System
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Always-on clock |
| `rst_ni` | input | logic | Active-low async reset |
| `reset_debug_i` | input | logic | Debug reset (clears PFB FFs) |
| `chicken_bit_i` | input | logic | Forces clock gate on |
| `dft_i` | input | dft_t | DFT control (scanmode, scan_reset) |

### Thread control
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `f0_thread_enabled_i` | input | logic [NrThreads-1:0] | Per-thread enable |
| `f0_reset_vector_i` | input | logic [47:0] | Reset PC (48-bit VA) |
| `vm_status_i` | input | vm_status_t [NrThreads-1:0] | Per-thread VM status for TLB |

### ICache request (to ICache)
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `f1_icache_req_ready_i` | input | logic | ICache accepts request |
| `f1_icache_req_valid_o` | output | logic | Request valid |
| `f1_icache_req_o` | output | fe_icache_req_t | Request (thread_id, vm_status, addr) |

### ICache response (from ICache)
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `f5_icache_resp_valid_i` | input | logic | Response valid |
| `f5_icache_resp_miss_i` | input | logic | Response is a miss (F5 stage, for early write predict) |
| `f5_icache_resp_i` | input | icache_fe_resp_t | Response data (256-bit block + fault flags) |
| `f6_icache_fill_done_i` | input | logic | Miss fill complete |

### Core redirect
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `f0_core_req_valid_i` | input | logic [NrThreads-1:0] | Per-thread redirect valid |
| `f0_core_req_i` | input | fe_req_t [NrThreads-1:0] | Redirect PC + debug halt/resume |
| `id_core_stall_i` | input | logic [NrThreads-1:0] | Per-thread pipeline stall |

### ID stage output
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `id_inst_ready_i` | input | logic | Consumer accepts instruction |
| `id_inst_valid_o` | output | logic | Instruction valid |
| `id_inst_thread_id_o` | output | logic | Thread ID of issued instruction |
| `id_inst_data_o` | output | fe_core_resp_t | Instruction data (PC, bits, faults) |
| `id_intpipe_ctrl_o` | output | minion_control_t | Integer pipeline decode signals |
| `id_vpu_decoder_sigs_o` | output | vpu_ctrl_sigs_t | VPU decoder signals |
| `id_vpu_core_ctrl_o` | output | vpu_id_ctrl_t | VPU core control (mapped from decoder) |

### Debug program fetch buffer
| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `debug_ffb_wdata_i` | input | logic [63:0] | Debug write data |
| `debug_ffb_en_i` | input | logic [3:0] | Debug write word enable |
| `debug_ffb_thread_sel_i` | input | logic | Thread select for debug writes |
| `debug_ffb_exec_i` | input | logic [NrThreads-1:0] | Per-thread PFB execute |
| `halt_i` | input | logic [NrThreads-1:0] | Debug halt request |
| `halted_i` | input | logic [NrThreads-1:0] | Thread is halted (in debug mode) |

## Differences from original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|-----------------|-----------|
| Reset | Active-high synchronous (`reset`) | Active-low async-assert/sync-deassert (`rst_ni`) | ASIC/FPGA standard |
| Naming | `clock`, `f0_icache_req_valid` | `clk_i`, `f0_icache_req_valid_o` | lowRISC convention |
| DFT | `test_en` wire | `dft_pkg::dft_t dft_i` struct | Consolidated DFT |
| Clock gate | `et_clk_gate` | `prim_clk_gate` | Technology abstraction |
| Fetch-buffer preview seam | open-coded `LATCH_P2` + `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff_preview` | Named semantic seam for exact generic / FPGA-safe tech mapping |
| ICache arbiter | `arb_lru_data` | `prim_arb_lru` + data mux | Primitive mapping |
| Types | `include "soc.vh"` defines | `minion_frontend_pkg` enums/structs | Package-based types |
| Extra trans feature select | `` `ifdef ENABLE_EXTRA_TRANS `` in shared decoders | `EnableExtraTrans` parameter threaded through `minion_frontend` | Functional configuration is explicit in RTL instead of preprocessor-controlled |
| Coding style | `RST_FF`/`EN_FF` macros | Explicit `always_ff`/`always_comb` | Standard SV |

No functional or architectural changes. Same pipeline stages, same FIFO depth, same cycle-by-cycle timing.

## Verification

6 RTL co-simulations, all passing with 0 mismatches:

| Cosim | Comparisons | Stimulus |
|-------|------------|----------|
| `minion_frontend_rvc_expander` | 53,260 | Exhaustive 16-bit + 32-bit |
| `minion_frontend_thread_sched` | 5,540 | Random enables/stalls |
| `minion_frontend_thread_buffer` | 1,427,157 | Miss/fill, PFB, faults, half-cycle response stress, and 40K random cycles |
| `minion_frontend_intpipe_decode` | 2,000,069 | 2M random instructions |
| `minion_frontend_vpu_decoder` | 4,000,110 | 2M random instructions |
| `minion_frontend_top` | 1,169,262 | Dual-thread, mispredict, stall, half-cycle preview stress, debug PFB, and 25K random cycles |

Total: 8,655,398 comparisons, 0 mismatches.
