# shirecache

L2 Shire Cache — the shared cache for all minion cores in a neighborhood.

This IP reimplements the CORE-ET `shire_cache` as a set of `shirecache_*` modules. The `shirecache_` prefix avoids name collision with the original during RTL co-simulation.

## Architecture

4 MB, 4-way set-associative, 512-bit (64-byte) cache lines.
4 independent banks, each with 4 sub-banks. Runtime-configurable L2/L3/SCP partitioning.

```
shirecache_top
├── shirecache_xbar (req + rsp crossbars)
├── 4× shirecache_bank
│   ├── shirecache_pipe (pipeline controller)
│   │   ├── 4× shirecache_pipe_sub_bank (datapath)
│   │   │   ├── shirecache_pipe_lru_4w_calc
│   │   │   ├── shirecache_pipe_set_calc (×3: L2, L3, SCP)
│   │   │   ├── shirecache_pipe_l3_swizzle_get / _put
│   │   │   └── shirecache_pipe_atomic
│   │   ├── shirecache_pipe_sub_bank_mem
│   │   │   ├── shirecache_pipe_tag_ram_wrap
│   │   │   ├── shirecache_pipe_tag_state_ram_wrap
│   │   │   └── shirecache_pipe_data_ram_wrap
│   │   ├── shirecache_pipe_rbuf (read buffer)
│   │   ├── shirecache_pipe_cbuf (coalesce buffer)
│   │   └── shirecache_pipe_idx_cop_sm (cache maintenance FSM)
│   ├── shirecache_bank_reqq (request queue)
│   │   ├── shirecache_bank_reqq_entry (×64 per-entry FSM)
│   │   └── shirecache_bank_reqq_alloc
│   ├── shirecache_bank_dataq
│   ├── shirecache_bank_rspmux
│   ├── shirecache_bank_mesh (×2: to_l3, to_sys)
│   ├── shirecache_bank_l3_slave
│   ├── shirecache_bank_perfmon
│   │   ├── shirecache_perfmon_ctl (counter FSMs + ESR)
│   │   └── shirecache_bank_perfmon_qual (×2: P0, P1)
│   ├── shirecache_bank_err_logger (12-source error logger)
│   ├── shirecache_bank_l2hpf (L2 hardware prefetcher)
│   ├── shirecache_bank_trace (6 trace snippet types)
│   └── shirecache_bist_wrapper
│       └── shirecache_bist_mbx (×4: MBS, MBT, MBD, MBI)
├── shirecache_mesh_master (+_axi_port, ×2: to_l3, to_sys)
├── shirecache_mesh_slave (+_axi_port)
└── shirecache_l3_to_sys_bridge
```

## Package: `shirecache_pkg`

All cache constants (geometry, widths, buffer sizes), enums (opcodes, address spaces, error types), and the L3 swizzle control struct. Import with `import shirecache_pkg::*;`.

Key constants:
- `Banks = 4`, `SubBanks = 4`, `Ways = 4`
- `SetsPerSubBank` = derived from 4 MB total
- `LineDataSize = 512` bits, `LineByteSize = 64` bytes
- `TagMaxSize = 23`, `ReqqDepth = 64`

## Modules

### shirecache_pipe_lru_4w_calc

4-way LRU replacement calculator. Compressed 5-bit LRU state encoding. Supports MRU promotion, LRU demotion, and locked-way awareness. Pure combinational.

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `lru_state_i` | input | 5 | Current LRU state (compressed) |
| `hit_way_i` | input | 2 | Way being accessed |
| `hit_locked_i` | input | 1 | Hit way is locked |
| `valid_bits_i` | input | 4 | Per-way valid bits |
| `locked_bits_i` | input | 4 | Per-way lock bits |
| `locking_way_i` | input | 1 | Locking this way |
| `unlocking_way_i` | input | 1 | Unlocking this way |
| `update_mru_i` | input | 1 | Promote to MRU |
| `update_lru_i` | input | 1 | Demote to LRU |
| `lru_state_o` | output | 5 | Updated LRU state |
| `lru_lock_err_o` | output | 1 | Error: tried to lock last unlocked way |

### shirecache_pipe_set_calc

Cache set index calculator. Determines the pipe set based on address and ESR configuration (base, size, mask). Supports runtime-configurable partitioning. Pure combinational.

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `region_op_i` | input | 1 | Region is enabled |
| `address_set_i` | input | `SetIdSize` | Address set bits |
| `esr_set_base_i` | input | `SetIdSize` | Allocated region base |
| `esr_set_size_i` | input | `SetAllocSize` | Allocated region size |
| `esr_set_mask_i` | input | `SetIdSize` | Set mask (thermometer code) |
| `esr_tag_mask_i` | input | `SetIdSize` | Tag mask (assertion only) |
| `pipe_set_o` | output | `SetIdSize` | Computed set index |

### shirecache_pipe_l3_swizzle_get

Extracts sub_bank, bank, and shire_id fields from an L3 physical address based on ESR swizzle configuration. Pure combinational.

### shirecache_pipe_l3_swizzle_put

Inserts sub_bank, bank, and shire_id fields into an L3 physical address. Output is OR'd with a partial address to form the complete L3 eviction address. Pure combinational.

### shirecache_pipe_tag_ram_wrap

Tag RAM wrapper. Single-port, `TagRamDataSize` bits wide (116), `SetsPerSubBank` deep. Clock gated via `prim_clk_gate`. Read and write are mutually exclusive. 1-cycle read latency.

| Port | Direction | Description |
|------|-----------|-------------|
| `clock_en_i` | input | Clock enable for gating |
| `rd_en_i` | input | Read enable |
| `wr_en_i` | input | Write enable |
| `addr_i` | input | Set address |
| `wr_data_i` | input | Write data (all ways) |
| `rd_data_o` | output | Read data (registered) |
| `ram_cfg_i` | input | SRAM timing config |
| `dft_i` | input | DFT control |

### shirecache_pipe_tag_state_ram_wrap

Tag state RAM (LRU + flags). Supports single-port (`DualPort=0`) and dual-port (`DualPort=1`, independent read/write addresses). `TagStateRamDataSize` bits wide (40). Clock gated.

### shirecache_pipe_data_ram_wrap

Data RAM wrapper. Internally uses `LineQwSize` (4) independent `prim_ram_1p` instances, one per quadword. Supports per-quadword write enable (`wr_qwen_i`). `DataRamDataSize` bits wide (576 = 512 data + 64 ECC). Clock gated.

### shirecache_pipe_cbuf

Coalesce buffer (32 entries). Manages write coalescing with LRU replacement. Supports Install, MRU update, Clear, Replace, and Bypass operations. When an install evicts a valid entry from a full buffer, generates a victim notification to the request queue. 1-cycle response pipeline. Uses `prim_arb_lru` for replacement.

Replaces: `shire_cache_pipe_cbuf`. Differences: reset polarity, `prim_arb_lru` instead of `arb_lru`, package types instead of `define` types, explicit `always_ff` instead of FF macros.

### shirecache_pipe_atomic

Atomic operation unit. Performs RISC-V AMO operations (swap, add, XOR, AND, OR, min, max, compare-exchange) plus partial writes. Supports SIMD (8×32b), scalar 64-bit, and scalar 32-bit modes. Includes RISC-V NaN handling for floating-point min/max. 3-stage pipeline (_d0 extract, _d1 ALU, _d2 output). Combinational `atomic_data_rsp_data_o` for L3/SCP response. No sub-module instantiations.

Replaces: `shire_cache_pipe_atomic`. Differences: reset polarity, package constants instead of `define` macros, `w'(sig)` instead of `` `ZX `` macro.

### shirecache_pipe_rbuf

Read buffer (8 entries). Caches read data with LRU replacement. Features a 7-stage pending-install pipeline matching variable RAM read delay (2/3/4 cycles). Pending installs can be squashed by evictions or cleared by the request queue. Credit-based FIFO (`prim_fifo_reg`, depth 4) for read responses. Supports separate L2 and SCP namespaces. Uses `prim_arb_lru` for replacement.

Replaces: `shire_cache_pipe_rbuf`. Differences: reset polarity, `prim_arb_lru`/`prim_fifo_reg` instead of `arb_lru`/`gen_fifo_reg`, package types, explicit `always_ff`.

### shirecache_pipe_idx_cop_sm

Cache maintenance FSM (index cache-op state machine). One-hot 7-state FSM for cache maintenance operations triggered via ESR registers. Post-reset automatic ALL_INV with cold/warm reset detection. 2-cycle start/abort pipeline for multicycle ESR timing. Set/way/subbank/qw counters iterate over cache entries. Supports ALL_INV, L2/L3 Inv/Flush/Evict, SCP Zero, CBuf invalidation, and debug read/write modes. Uses 2× `prim_rst_sync` for warm and cold reset synchronization.

| Port | Direction | Description |
|------|-----------|-------------|
| `rst_ni` | input | Warm reset (active-low) |
| `rst_c_ni` | input | Cold reset condition (active-low) |
| `dft_i` | input | DFT control struct |
| `mbist_on_i` | input | MBIST active — abort post-reset ALL_INV |
| `pipe_idx_cop_aborted_o` | output | Abort notification (1-cycle pulse) |
| `pipe_idx_cop_l2_busy_o` | output | L2 busy during operation |
| `pipe_idx_cop_l3_busy_o` | output | L3 busy during operation |
| `pipe_idx_cop_req_ready_i` | input | Reqq ready for request |
| `pipe_idx_cop_req_valid_o` | output | Request valid to reqq |
| `pipe_idx_cop_req_info_o` | output | Request info (idx_cop_req_t) |
| `pipe_idx_cop_is_active_o` | output | FSM not idle (keep clock on) |
| `pipe_cbuf_state_info_i` | input | CBuf state for CB_INV mode |
| `esr_dbg_wr_info_o` | output | Debug write to sub_bank RAMs |
| `dbg_rd_info_i` | input | Debug read from sub_bank RAMs |
| `esr_ctl_i` | input | Full bank ESR config (bank_esr_ctl_t) |
| `pipe_idx_cop_sm_esr_status_o` | output | ESR status read-back |
| `my_bank_id_i` | input | Bank ID for address construction |
| `my_scp_virtual_shire_id_i` | input | SCP shire ID for address fabrication |

Parameter `SkipPowerUpAllInv` (default 0) skips the automatic post-reset ALL_INV — set to 1 when RAMs are pre-initialized in simulation.

Replaces: `shire_cache_pipe_idx_cop_sm`. Differences: reset polarity, `prim_rst_sync` instead of `rst_repeat`, DFT struct, package types, explicit `always_ff`, `SkipPowerUpAllInv` parameter instead of sim-only `dv_skip_power_up_all_inv_const`.

### shirecache_bank_dataq

Data queue for in-flight requests. Holds cacheline data (512-bit × 64 entries) associated with each reqq entry. 4 write ports (bank_l2, to_l3_mesh, bank_l3, to_sys_mesh) plus pipeline write (always-accepted priority). 4 read ports (to_l3_mesh, rspmux_l2, to_sys_mesh, rspmux_l3) plus pipeline read (3-cycle latency). Write side: register pipeline write, FIFO others, RR arb → prio arb with pipe priority. Read side: same structure with read sink FIFOs (ctl, data, ben per port). Per-QW register files (4× 128-bit `prim_rf_1r1w_preview`) plus BEN register file. Uses `prim_fifo_reg`, `prim_arb_rr`, `prim_arb_prio`.

Replaces: `shire_cache_bank_dataq`. Differences: reset polarity, `prim_rf_1r1w_preview` instead of the open-coded `LATCH_P2` plus `rf_latch_1r_1w` seam, `prim_arb_rr`/`prim_arb_prio` instead of `arb_rr_data`/`arb_prio_data_held`, DFT struct, package types.

### shirecache_bank_reqq_entry

Per-entry FSM for the request queue (×64 per bank). Tracks one in-flight cache request through allocation → pipeline/mesh operations → response → deallocation. Supports all address spaces (L2, L3, SCP, Index, Message) and all operation types (read, write, writePartial, writeAround, atomic, flush, evict, prefetch, lock/unlock, msgSendData, scpFill/zero). Paired entry support for atomics and partial writes (2-allocation flow). Linked-list dependency tracking with mesh-level victim queuing. L2/L3 ESR bypass modes. Clock-gated (`prim_clk_gate`) when entry is not in use. 3936 lines — the largest module in the project.

Key interfaces: `reqq_alloc_t` input (allocation info), `reqq_entry_state_t` output (full state for arbitration), eligibility flags for pipeline/rbuf/mesh/rspmux, dependency clear signals.

Replaces: `shire_cache_bank_reqq_entry`. Differences: reset polarity, `prim_rst_sync`/`prim_clk_gate` primitives, DFT struct, package types, explicit `always_ff`/`always_comb`, no SV interface (cross-entry state access is assertion-only in original).

### shirecache_bank_reqq_alloc

Request queue allocator (×2 per bank: one L2, one L3). Selects a free entry from the 64-entry pool via `prim_arb_rr2` dual-grant round-robin, maps incoming ET-Link opcodes to reqq orig_opcodes, computes address-based dependencies across the linked list, detects allocation errors (illegal opcodes, SCP remote disabled, bypass+atomic), handles paired entry allocation for atomics/write-arounds/write-partials, and detects rbuf hits. Parameterized by `IsL2`.

Replaces: `shire_cache_bank_reqq_alloc`. Differences: `prim_arb_rr2` instead of `arb_rr2`, inline `hot2bin`/`onehot_mux`, reset polarity, DFT struct, package types.

### shirecache_bank_reqq

Request queue controller (1 per bank). Instantiates 2× `shirecache_bank_reqq_alloc` (L2/L3) + 64× `shirecache_bank_reqq_entry` via generate. Handles pipeline arbitration (per-subbank RR → cross-subbank RR → L2/L3 priority with yield), mesh arbitration (RR → FIFO → nodata/wdata split), rbuf read arbitration, rspmux response arbitration (nodata/wdata split), tag victim linked list insertion, coalescing victim management, meshll upgrades, pipe_data_rsp_modifier computation, and performance monitoring.

Replaces: `shire_cache_bank_reqq`. Differences: `prim_arb_rr`/`prim_arb_prio`/`prim_fifo_reg` instead of `arb_rr_data`/`arb_prio_data_held`/`gen_fifo_reg`, `prim_rst_sync` instead of `rst_repeat`, reset polarity, DFT struct, package types.

### shirecache_bank_rspmux

Response multiplexer (1 per bank). Muxes 6 response sources (rbuf, pipe data_rsp, l3_mesh fill, sys_mesh fill, reqq nodata, dataq wdata) into per-port FIFOs with RR arbitration, then selects via LRU xbar arbiter (`prim_arb_lru_2bid`) with priority/opportunistic grants and credit-based broadcast flow control. Formats responses as ET-Link `rsp_t` with correct opcode (RspAck/RspAckData/RspMsgRcvData/RspErr), filters VictimRsp and L3-source responses, and handles ECC error suppression.

Replaces: `shire_cache_bank_rspmux` + `shire_cache_bank_rspmux_xbar_arb` (combined into one module). Differences: `prim_arb_rr`/`prim_arb_lru_2bid`/`prim_fifo_reg` instead of `arb_rr_data`/`arb_lru_2_bid`/`gen_fifo_reg`, reset polarity, package types.

### shirecache_xbar

Parameterized NxM crossbar with credit-based flow control. Routes `NumSrc` sources to `NumDest` destinations through per-destination FIFO banks. Credit-counting FIFOs near the source track available space in the data FIFOs near the destination. A relay stage sits between source arbitration and destination FIFOs. Per-source arbitration is either xbar-side round-robin (one-hot ready) or source-side multi-hot (controlled per-source by `SrcArb`). Per-destination arbitration is always round-robin with data muxing (`prim_arb_rr`). Optional output relay FIFO per destination (`AddOutputRelay`).

| Parameter | Default | Description |
|-----------|---------|-------------|
| `FifoDepth` | 3 | Depth of data and credit FIFOs |
| `NumSrc` | 8 | Number of source ports |
| `NumDest` | 8 | Number of destination ports |
| `DataWidth` | 128 | Data bus width |
| `NumSrcMask` | all-ones | Bitmask to disable unused sources |
| `SrcArb` | 0 | Per-source bit: 1 = source does arbitration (multi-hot ready) |
| `AddOutputRelay` | 0 | Per-destination bit: 1 = add output relay FIFO (depth 2) |

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `src_ready_o` | output | `[NumSrc][NumDest]` | Per-source, per-dest ready (one-hot or multi-hot) |
| `src_2_credits_o` | output | `[NumSrc][NumDest]` | FIFO has room for 2+ entries |
| `src_valid_i` | input | `[NumSrc][NumDest]` | Per-source, per-dest valid |
| `src_data_i` | input | `[NumSrc][DataWidth]` | Per-source data |
| `dest_ready_i` | input | `[NumDest]` | Per-dest backpressure |
| `dest_valid_o` | output | `[NumDest]` | Per-dest data valid |
| `dest_data_o` | output | `[NumDest][DataWidth]` | Per-dest data |

Replaces: `shire_cache_xbar`. Differences: `prim_fifo_reg` instead of `gen_fifo_reg` (no GATING_TYPE), `prim_arb_rr` instead of `arb_rr`/`arb_rr_data`, explicit `always_ff` instead of `en_ff_gated_struct`/`RST_EN_FF`, reset polarity, parameter naming (UpperCamelCase).

### shirecache_bank_mesh

Per-bank mesh interface controller. Arbitrates outbound requests from reqq (no-data), dataq (with-data), and pipeline victim data; maps internal opcodes to ET-Link mesh opcodes; computes addresses (normal, ScpFill, atomic response); pushes through a depth-2 request FIFO. Inbound responses are received into a depth-2 response FIFO and fanned out to up to 4 destinations (reqq, dataq, rspmux_l2, l3_slave) with independent valid/ready handshaking and per-destination "sent" tracking.

| Parameter | Default | Description |
|-----------|---------|-------------|
| `ToL3` | 1 | 1 = to_l3 mesh (L3/DDR), 0 = to_sys mesh (system/UC) |
| `NumEntries` | `ReqqDepth` | Number of reqq entries (for `reqq_state_i` array) |

| Port | Direction | Description |
|------|-----------|-------------|
| `mesh_master_bank_req_valid_o/info_o/ready_i` | out/out/in | Mesh request valid/ready (to mesh master) |
| `mesh_master_bank_rsp_valid_i/info_i/ready_o` | in/in/out | Mesh response valid/ready (from mesh master) |
| `reqq_mesh_req_valid_nodata_i/info_i/ready_o` | in/in/out | Reqq no-data request (highest priority) |
| `dataq_mesh_req_valid_i/info_i/ready_o` | in/in/out | Dataq with-data request |
| `pipe_data_rsp_valid_i/info_i/modifier_i` | in/in/in | Pipeline victim data (lowest priority) |
| `mesh_bypassed_victim_o` | output | Victim bypassed to mesh |
| `mesh_rsp_valid_{reqq,dataq,rspmux_l2,l3_slave}_o` | output | Per-destination response valid |
| `mesh_rsp_ready_{reqq,dataq,rspmux_l2,l3_slave}_i` | input | Per-destination response ready |
| `reqq_state_i` | input | Array of reqq entry states (opcode/address lookups) |
| `esr_ctl_i` | input | ESR control (bypass/enable flags) |

Replaces: `shire_cache_bank_mesh`.

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Clock gate | `et_clk_gate` with `.te(1'b0)` | `prim_clk_gate` with `.dft_i(dft_i)` | DFT fix: original disables scan override (`te=0`), so gated-clock flops are not scan-testable. New version routes `scanmode` to force clock on during scan. |
| FIFO | `gen_fifo_reg` with `GATING_TYPE` | `prim_fifo_reg` | GATING_TYPE is power-only, no logic change |
| Arbiter | `arb_prio_data_held` | `prim_arb_prio` | Same priority+hold behavior |
| FF macros | `RST_EN_FF`/`EN_FF` | Explicit `always_ff` | Standard SV style |
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert | Project convention |
| Naming | `_ff`/`_nxt` | `_d`/`_q`, `_i`/`_o` | lowRISC convention |

### shirecache_bank_l3_slave

L3 slave bank interface. Receives L3 mesh slave requests via a depth-2 FIFO and maps them to `reqq_bank_req_t` for the request queue. Arbitrates 5 response sources (pipe data rsp, to_sys mesh rsp, reqq nodata, dataq, rbuf read rsp) via round-robin, with error-zero data gating and ECC error filtering. Sends responses through a depth-2 FIFO back to the L3 mesh slave.

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NumEntries` | `ReqqDepth` | Number of reqq entries |
| `NumEntriesId` | `ReqqIdSize` | Reqq entry ID width |

| Port | Direction | Description |
|------|-----------|-------------|
| `l3_mesh_slave_bank_req_*` | in/out | L3 mesh slave request interface |
| `l3_mesh_slave_bank_rsp_*` | out/in | L3 mesh slave response interface |
| `bank_l3_valid_o/info_o/ready_i` | out/out/in | Bank request to reqq |
| `pipe_data_rsp_*` | in/out | Pipe data response (source 0) |
| `to_sys_mesh_rsp_*` | in/out | To-sys mesh response (source 1) |
| `reqq_rspmux_*` | in/out | Reqq nodata response (source 2) |
| `dataq_rspmux_*` | in/out | Dataq response (source 3) |
| `pipe_rbuf_rd_rsp_*` | in/out | Rbuf read response (source 4) |
| `rspmux_sent_*` | output | Sent notification (valid, id, valid_hot) |

Replaces: `shire_cache_bank_l3_slave`. Differences: `prim_fifo_reg` instead of `gen_fifo_reg`, `prim_arb_rr` instead of `arb_rr_data`, active-low async reset, UpperCamelCase parameters.

### shirecache_mesh_master (+_axi_port)

Routes bank mesh requests to AXI master ports via CDC FIFOs. Per-bank request FIFOs with L3 swizzle for port routing, per-port AR/AW arbitration, per-port AXI port instances (CDC bridge via `prim_fifo_async`), per-bank response arbitration + response FIFOs. Dual clock domains (shire + NOC) with gated clocks.

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Clock gate | `et_clk_gate .te(1'b0)` | `prim_clk_gate .dft_i(dft_i)` | DFT fix: scanmode forces gated clocks on |
| Level shifter | `vclevel_shft_h2l` | Pass-through | Voltage domains handled by UPF |
| CDC sync | `et_cdc_sync` | Inline 2-FF synchronizer | Same function, no external module |
| Async FIFOs | `vcfifo_wr_hiv_gcd`/`_lov_gcd` | `prim_fifo_async` | Technology-abstracted, Depth=8 vs original 6 (power-of-2 for gray code) |
| Debug port | `debug_axi_port_t` | Omitted | Deferred to monitoring modules (#21-25) |
| Register FIFOs | `gen_fifo_reg` with GATING_TYPE | `prim_fifo_reg` | No data gating (power-only) |

### shirecache_mesh_slave (+_axi_port)

Routes AXI slave port requests to banks. Mirror of mesh_master: per-bank response FIFOs with port routing, per-port R/B response arbitration, per-port AXI port instances (CDC via `prim_fifo_async`), per-bank request arbitration + request FIFOs, clock enable output to banks.

Same differences as mesh_master (clock gate, level shifter, CDC, async FIFOs, debug port, register FIFOs). No clock gating in slave (original has none either).

### shirecache_l3_to_sys_bridge

L3-to-sys bypass bridge. Passes Writes, Reads, and Atomics through to the sys mesh master. Auto-acks Flush/Evict/Prefetch with immediate B response. Translates FlushToMem/EvictToMem opcodes to Write. Per-port (×4) L3 slave async FIFOs + multi-port request arbitration → ID remapping via 64-entry reqq → single sys master async FIFOs. Response path restores original L3 AXI ID from reqq and routes to the correct L3 port.

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Async FIFOs | `vcfifo_wr_hiv/lov_gcd` | `prim_fifo_async_hiv/lov` | Preserves voltage domain intent |
| Arbiters | `arb_rr` + `arb_rr_data` | `prim_arb_rr` | Same round-robin behavior |
| One-hot encoder | `hot2bin` + `onehot_mux` | `prim_hot2bin` | Same OR-based encoding |
| Debug port | present | Omitted | Deferred to monitoring modules |
| Reset | Active-high synchronous | Active-low async | Project convention |

### shirecache_pipe_sub_bank_mem

Memory subsystem for one sub-bank. Instantiates tag RAM, tag state RAM, and data RAM wrappers. Implements variable read-delay pipeline (2/3/4 cycles) based on `esr_sc_ram_delay`. Manages BIST mux for DFT memory testing. Clock gated per-RAM.

Replaces: `shire_cache_pipe_sub_bank_mem`.

### shirecache_pipe_sub_bank

Full sub-bank datapath. Instantiates `sub_bank_mem`, `lru_4w_calc`, `set_calc` (×3), `l3_swizzle_get`, `l3_swizzle_put`, `atomic`, and `sbe_insert`. Implements the tag-check (TC) and data-check (DC) pipeline stages. Generates tag response, data response, pipe info, and victim address computation.

Replaces: `shire_cache_pipe_sub_bank` (3263 lines).

### shirecache_pipe

Cache pipeline top. Instantiates 4× `sub_bank` + 4× `sub_bank_mem` + `cbuf` + `rbuf` + `idx_cop_sm`. Arbitrates sub-bank access, manages pipeline hazards, coalescing, and read buffer. Broadcasts ESR config to sub-banks.

Replaces: `shire_cache_pipe` (~1200 lines).

### shirecache_bist_mbx

BIST mailbox — routes MBIST signals to one logical memory with back-to-back access blocking for multi-cycle RAM delays. Feedthrough path with shift-register blocking when `RamDelay > 1`.

| Parameter | Default | Description |
|-----------|---------|-------------|
| `DataSize` | 16 | MBIST data bus width |
| `AddrSize` | 8 | MBIST address bus width |
| `RamDelay` | 1 | RAM read/write latency (cycles) |

Replaces: `shire_cache_bist_mbx`. Differences: drops DV-only `et_bist_tester`, explicit `always_ff` instead of `RST_EN_FF`.

### shirecache_bist_wrapper

BIST wrapper containing 4 `shirecache_bist_mbx` instances (MBS: tag state, MBT: tag, MBD: data, MBI: icache). Decodes shared `mbist_sel` to one-hot, routes MBIST control/address/data to the selected mailbox, muxes read data back. `mbist_on` registered output for BIST insertion tool hierarchy.

Replaces: `shire_cache_bist_wrapper`. Differences: `dft_mbist_en_i` instead of `dft__mbist_en`, always instantiates MBI (no `ET_PIPE_STANDALONE` conditional).

### shirecache_bank_l2hpf

L2 hardware prefetcher interface monitor. Observes tag responses and generates HPF sideband signals for L2 Read/Write/WritePartial operations. Conditional generate: `L2HpfImplemented=0` (default) stubs outputs to zero.

Replaces: `shire_cache_bank_l2hpf`. Differences: reset polarity, explicit `always_ff` instead of `RST_EN_FF`/`EN_FF`.

### shirecache_bank_perfmon_qual

Performance monitor qualifier/event filter. Two modes: resource mode (accumulate/count/max with min/max bounds) and event mode (51 qualifier bits for opcode/hit/miss/eviction/qwen filtering). Contains `match_opcode` function covering all 37 reqq opcodes and `count_qwen` popcount function. Purely combinational.

| Parameter | Default | Description |
|-----------|---------|-------------|
| `IsP0` | 1 | Selects resource mode set (P0 vs P1) |
| `PerfmonCovfSize` | 41 | Counter width (with overflow bit) |
| `PerfmonQualSize` | 51 | Qualifier bit vector size |

Replaces: `shire_cache_bank_perfmon_qual`.

### shirecache_perfmon_ctl

Performance monitor control FSMs and ESR interface. Three independent one-hot 6-state FSMs (IDLE→START→DLY1→DLY2→DLY3→ACTIVE) for cycle counter, P0 counter, and P1 counter. ESR write/read for control status, counters, and qualifier registers. Counter overflow detection, saturation, and interrupt generation. Clock gated via `prim_clk_gate`.

Replaces: `shire_cache_perfmon_ctl` (596 lines). Differences: `prim_clk_gate` instead of `et_clk_gate`, reset polarity, DFT struct, explicit `always_ff`.

### shirecache_bank_perfmon

Performance monitor top. Wraps 1× `shirecache_perfmon_ctl` + 2× `shirecache_bank_perfmon_qual` (P0 with `IsP0=1`, P1 with `IsP0=0`). Extracts ESR fields, routes qualifier/mode/event signals, packs `perfmon_esr_status_t` output and `perfmon_cntr_overflow_t`.

Replaces: `shire_cache_bank_perfmon` (250 lines).

### shirecache_bank_err_logger

Error logger with 12 error sources and priority arbitration. Sources (highest→lowest priority): L2/L3 allocation decode errors, tag decode error, L3/sys mesh response errors, tag/tag_state/data ECC SBE/DBE, ECC saturation, perfmon saturation. Rising-edge detection on ECC/perfmon overflow signals (2-cycle delay pipeline). ESR write-back for error log info. REQQ debug status capture (valid bits, IO valid/ready, per-entry state query). Uses `prim_arb_prio` and `prim_hot2bin`.

Replaces: `shire_cache_bank_err_logger` (649 lines). Differences: `prim_arb_prio` instead of `arb_prio_data`, `prim_hot2bin` instead of `hot2bin`, drops DV-only `onehot_mux`, reset polarity, explicit `always_ff`.

### shirecache_bank_trace

Trace packet generator with 6 trace snippet types: L2 alloc, L3 alloc, tag check, RBUF read, mesh activity, REQQ state change. Each snippet type is independently enabled via ESR `trace_type_hot_enable` bits. Snippets are packed into 3-segment trace packets with round-robin segment placement. Clock gated via `prim_clk_gate`. REQQ state change detection tracks opcode + 12 state bits.

Replaces: `shire_cache_bank_trace` (424 lines). Differences: `prim_clk_gate` instead of `et_clk_gate`, package trace types, reset polarity, explicit `always_ff`.

### shirecache_bank

Per-bank container. Wires together all 12 sub-modules: pipe, reqq, dataq, rspmux, 2× bank_mesh (to_l3, to_sys), l3_slave, perfmon, err_logger, l2hpf, trace, bist_wrapper. Bank-level clock gating, 3× reset synchronizers (warm, cold, debug), L2 request FIFO (depth 2), and L2 request mapping (ET-Link → reqq). ESR control input (external ESR block not instantiated — deferred to ESR generator porting).

Replaces: `shire_cache_bank` (1370 lines).

### shirecache_top

Top-level cache. Instantiates 4× `shirecache_bank`, 2× `shirecache_xbar` (request + response crossbars), 2× `shirecache_mesh_master` (to_l3, to_sys), 1× `shirecache_mesh_slave`, 3× `prim_rst_sync` (cold, warm, debug), 1× `prim_fifo_reg` (UC response relay). Routes neighborhood request/response ports through crossbars to banks. Clock domains are `clk_i` for cache logic, `noc_clk_i` for NOC/mesh CDC endpoints, and `clk_free_i` for bank-level trace/performance-monitor logic that must remain observable when functional clock gating is applied. The top-level `dft_sram_clk_i` port is routed unchanged to every bank; `dft_i.sram_clk_override` selects that clock inside the RAM-wrapper `prim_clk_mux` seams.

Key top-level clock/reset ports:
- `clk_i`: cache/uncore functional clock.
- `clk_free_i`: free-running monitor clock forwarded unchanged to every `shirecache_bank.clk_free_i` for trace/perfmon logic.
- `noc_clk_i`: mesh/NOC clock.
- `rst_cold_ni`, `rst_ni`, `rst_debug_ni`, `noc_rst_ni`: active-low reset domains matching the translated reset split.

Key top-level DFT ports:
- `dft_i`: consolidated DFT control struct; `dft_i.sram_clk_override` selects the SRAM DFT clock path inside RAM-wrapper muxes.
- `dft_sram_clk_i`: SRAM DFT clock override source forwarded unchanged to every `shirecache_bank.dft_sram_clk_i`.

Replaces: `shire_cache` (706 lines). Differences: `prim_rst_sync` instead of `rst_repeat`, `prim_fifo_reg` instead of `gen_fifo_reg`, ESR externalized, adds translated top-level `clk_free_i` hookup for the bank monitor-clock contract, `dft_sram_clk_i` instead of `dft__sram_clock`, drops simulation-only interface.

## Differences from CORE-ET `shire_cache`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Module prefix | `shire_cache_` | `shirecache_` | Avoids cosim name collision |
| Package | `shire_cache_defines.vh` + `shire_cache_types.vh` (2000 lines of `define`) | `shirecache_pkg` (SV package, localparams) | Clean, no macro pollution |
| RAM instances | `gen_mem1p`/`gen_mem2p` or foundry SRAMs | `prim_ram_1p`/`prim_ram_2p` | Technology abstraction |
| Clock gating | `et_clk_gate` + `et_clk_mux2` | `prim_clk_gate` + `prim_clk_mux` | Technology abstraction with DFT |
| DFT | Individual `dft__*` signals | `dft_pkg::dft_t` struct | Consolidated |
| Free-running monitor clock | No separate `shire_cache` top port; banks receive the functional `clock` as `fclock` | `shirecache_top.clk_free_i` forwards a distinct clock to each bank `clk_free_i` | Preserves the translated bank-level trace/perfmon free-clock contract for integration wrappers; top cosim ties it to `clk_i` for original-interface equivalence |
| RAM config | `esr_shire_cache_ram_cfg_t` | `ram_cfg_pkg::ram_cfg_t` | Standardized |
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert | Project coding style |
| Flip-flop macros | `RST_FF`, `EN_FF`, `RST_EN_FF` | Explicit `always_ff` | No macro dependencies |
| Assertions | `ASSERT_ALWAYS` macro | Verilator-compatible `$error` | Portable |
| Hierarchical refs | `pipe.sub_bank.esr_sc_ram_delay_*` accessed from RAM wrappers | Explicit `ram_delay_i` port on each RAM wrapper | No hierarchical references — all inputs via ports |
| DFT SRAM clock | `dft__sram_clock` + `dft__clk_override` individual signals | `dft_sram_clk_i` port + `dft_i.sram_clk_override` field | Clock is separate port (not in struct), override flag in `dft_t` |
| RAM power modes | `esr_sc_ram_deep_sleep` + `esr_sc_ram_shut_down` individual signals | `ram_cfg_i.deep_sleep` + `ram_cfg_i.shut_down` fields | Consolidated into `ram_cfg_t` |
| RAM DFT inhibit | `esr_shire_cache_ram_cfg.dft__ram_rei/wei` | `dft_i.ram_rei` + `dft_i.ram_wei` | Consolidated into `dft_t` |

## Tests

30 unit test suites, 44 RTL cosims (all passing, 0 mismatches).

```bash
make -C dv test              # all 30 shirecache unit tests
make -C dv lint              # Verilator lint (full hierarchy)

# Individual tests (see dv/Makefile for full list):
make -C dv test-lru          # LRU calculator
make -C dv test-cbuf         # coalesce buffer
make -C dv test-reqq         # request queue (full hierarchy)
make -C dv test-bmbx         # BIST mailbox
make -C dv test-bwrap        # BIST wrapper
make -C dv test-l2hpf        # L2 hardware prefetcher
make -C dv test-pmq          # perfmon qualifier
make -C dv test-pm           # perfmon top
make -C dv test-errl         # error logger
make -C dv test-trc          # trace
make -C dv test-topclk       # shirecache_top free-running clock hookup smoke test
```
