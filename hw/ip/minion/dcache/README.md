# Minion DCache

L1 data-cache support logic for the minion core.

This IP is the standalone minion L1 dcache reimplementation. It includes the
full `minion_dcache_top` plus the translated helper blocks it instantiates:

Status: the standalone dcache IP is complete for the original non-graphics
configuration that this repo is currently targeting. The only explicitly
deferred path is the original `GFX_ENABLED` branch inside `dcache_texsend`,
which was disabled in the original minion configuration and remains documented
as a separate follow-on item.

| Module | File | Replaces |
|--------|------|----------|
| `minion_dcache_pkg` | `rtl/minion_dcache_pkg.sv` | shared dcache/minion types from `soc.vh` |
| `minion_dcache_atomic_alu_unit` | `rtl/minion_dcache_atomic_alu_unit.sv` | `dcache_atomic_alu_unit` |
| `minion_dcache_store_merge_unit` | `rtl/minion_dcache_store_merge_unit.sv` | `dcache_store_merge_unit` |
| `minion_dcache_pma_unit` | `rtl/minion_dcache_pma_unit.sv` | `dcache_pma_unit` |
| `minion_dcache_lru_array` | `rtl/minion_dcache_lru_array.sv` | `dcache_lru_array` |
| `minion_dcache_metadata_array` | `rtl/minion_dcache_metadata_array.sv` | `dcache_metadata_array` |
| `minion_dcache_buffer_array` | `rtl/minion_dcache_buffer_array.sv` | `dcache_buffer_array` |
| `minion_dcache_128x64_1r1w_lram` | `rtl/minion_dcache_128x64_1r1w_lram.sv` | `dcache_128x64_1r1w_lram` |
| `minion_dcache_128x72_1r1w_lram` | `rtl/minion_dcache_128x72_1r1w_lram.sv` | `dcache_128x72_1r1w_lram` |
| `minion_dcache_data_array` | `rtl/minion_dcache_data_array.sv` | `dcache_data_array` |
| `minion_dcache_tlb_array` | `rtl/minion_dcache_tlb_array.sv` | `dcache_tlb_array` |
| `minion_dcache_replay_queue` | `rtl/minion_dcache_replay_queue.sv` | `dcache_replay_queue` |
| `minion_dcache_miss_handler` | `rtl/minion_dcache_miss_handler.sv` | `dcache_miss_handler` |
| `minion_dcache_miss_handler_unit` | `rtl/minion_dcache_miss_handler_unit.sv` | `dcache_miss_handler_unit` |
| `minion_dcache_cache_op_unit_l2` | `rtl/minion_dcache_cache_op_unit_l2.sv` | `dcache_cache_op_unit_l2` |
| `minion_dcache_cache_op_unit` | `rtl/minion_dcache_cache_op_unit.sv` | `dcache_cache_op_unit` |
| `minion_dcache_writeback_unit` | `rtl/minion_dcache_writeback_unit.sv` | `dcache_writeback_unit` |
| `minion_dcache_scratchpad_ctrl` | `rtl/minion_dcache_scratchpad_ctrl.sv` | `dcache_scratchpad_ctrl` |
| `minion_dcache_texsend` | `rtl/minion_dcache_texsend.sv` | `dcache_texsend` |
| `minion_dcache_reduce` | `rtl/minion_dcache_reduce.sv` | `dcache_reduce` |
| `minion_dcache_tensor_load` | `rtl/minion_dcache_tensor_load.sv` | `dcache_tensor_load` |
| `minion_dcache_top` | `rtl/minion_dcache_top.sv` | `dcache_top` |

## What It Does

- `minion_dcache_atomic_alu_unit` performs the AMO data operation and byte-lane masking.
- `minion_dcache_store_merge_unit` merges a store payload into a 256-bit cache line.
- `minion_dcache_pma_unit` checks the minion physical memory attributes and access permissions.
- `minion_dcache_lru_array` stores the per-set replacement state.
- `minion_dcache_metadata_array` stores tags, coherency state, and valid bits.
- `minion_dcache_buffer_array` tracks outstanding dcache data-buffer allocations and payload storage.
- `minion_dcache_128x64_1r1w_lram` preserves the original 128x64 registered-read local-RAM interface over the repo tech-RAM abstraction.
- `minion_dcache_128x72_1r1w_lram` preserves the original 128x72 registered-read local-RAM interface over the repo tech-RAM abstraction.
- `minion_dcache_data_array` banks four 128x64 LRAMs behind the original stage-1 read and stage-4 write interface.
- `minion_dcache_tlb_array` preserves the original dcache-local SATP/MATP latch timing and adapts the dcache TLB request/response bundles onto `minion_tlb`.
- `minion_dcache_replay_queue` tracks replayed dcache requests, preserves the original queue-age ordering and conflict matrix behavior, and exposes the scoreboard/deallocation side effects back to the minion core.
- `minion_dcache_writeback_unit` sequences dcache data-array read passes into the original L2 evict/message-send request stream, including nack rollback and L2 backpressure replay.
- `minion_dcache_cache_op_unit` preserves the original L1 cache-op FSM, including lock/soft-lock metadata updates, writeback sequencing, prefetch injection, and downstream cache-op propagation.
- `minion_dcache_scratchpad_ctrl` sequences scratchpad/split/shared mode changes, including the original clear-all, clear-low, and scratchpad-allocation control pulses.
- `minion_dcache_texsend` currently implements only the exact original non-graphics stub behavior: always idle/ready, no L2 requests, no writeback requests, and zero debug outputs when `GFX_ENABLED` is absent.
- `minion_dcache_reduce` sequences reduce-message handshakes and tensor-store traffic, including the original address-translation side engine, BA/VPU coordination, partner-ready tracking, rate limiting, and write-around accounting.
- `minion_dcache_tensor_load` sequences tensor-load control, request-control translation, TENB credit tracking, L2 miss traffic, cooperative sequencing, interleave/transpose refill formatting, and the original debug/flush/error side effects.
- `minion_dcache_top` preserves the original minion dcache integration boundary: core request/response pipeline, replay/buffer arrays, metadata/data arrays, TLB/PTW/PMA path, writeback/cache-op/tensor/reduce side engines, APB debug access, ET-link L2 request/response routing, and the original debug/error/event outputs.
- `minion_dcache_top` also supports a scalar-only build mode through
  `VpuEn=0`, which structurally compiles out the tensor/offload engines and
  their shared arbitration users while preserving the scalar cache/MMU/cache-op
  contract.

## Parameters

Most leaf blocks are fixed-shape translations of the original design. Shared
widths and enums live in `minion_dcache_pkg`.

`minion_dcache_top` also exposes three compile-time feature cuts:

- `DebugApbEn`
  Removes the APB debug window and array debug read/write access when `0`.
  Selected APB access phases still complete with `pslverr_o=1` and zero
  `prdata_o` instead of hanging the bus.
- `DebugMonEn`
  Removes the exported dcache debug-monitor packing where the top-level monitor
  mux does not need those sideband observability signals.
- `VpuEn`
  Removes the tensor/offload side engines when `0`: both
  `minion_dcache_tensor_load` instances, `minion_dcache_reduce`,
  `minion_dcache_texsend`, and the related VPU/SCP/TENB muxing and L2 traffic.
  The scalar LSU/MMU/cache contract remains intact, and the offload-ready
  fields in `core_ctrl_resp_o` are driven to benign ready/complete values.

## Port Interface

### `minion_dcache_atomic_alu_unit`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Preserved minion clock input from the original interface |
| `rst_ni` | input | logic | Preserved reset input from the original interface |
| `addr_i` | input | logic [2:0] | Byte address within the 64-bit AMO operand |
| `cmd_i` | input | `dcache_cmd_e` | AMO opcode |
| `typ_i` | input | logic [1:0] | Byte/half/word/dword selector |
| `lhs_i` | input | logic [63:0] | Original memory value |
| `rhs_i` | input | logic [63:0] | Store operand |
| `out_o` | output | logic [63:0] | Updated value after AMO + byte masking |

### `minion_dcache_store_merge_unit`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `addr_i` | input | logic [4:0] | Byte offset within the 256-bit cache line |
| `typ_i` | input | `dcache_type_e` | Scalar/PS store type |
| `ps_mask_i` | input | logic [7:0] | 32-byte partial-store mask for PS operations |
| `orig_data_i` | input | logic [255:0] | Existing cache-line data |
| `store_data_i` | input | logic [255:0] | Unaligned store data |
| `enable_lo_i` | input | logic | Original lower-half enable input |
| `enable_hi_i` | input | logic | Original upper-half enable input |
| `merge_out_o` | output | logic [255:0] | Merged cache-line result |

### `minion_dcache_pma_unit`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `ioshire_i` | input | logic | Set when minion runs in IOshire service-processor mode |
| `af_skip_i` | input | logic | Suppress access-fault generation |
| `req_data_i` | input | `dcache_pma_req_t` | PMA request bundle |
| `mprot_i` | input | `esr_mprot_t` | ESR memory-protection control |
| `valid_addr_i` | input | logic | Physical address is valid |
| `vm_status_i` | input | `vm_status_t` | Current privilege / VM state |
| `resp_data_o` | output | `dcache_pma_resp_t` | Access-fault + cacheable response |

### `minion_dcache_lru_array`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Array clock |
| `rst_ni` | input | logic | Async-assert, sync-deassert reset |
| `read_set_i` | input | logic [`DcacheSetIdxWidth-1:0`] | Set index to read |
| `read_data_o` | output | logic [`DcacheLruSetSize-1:0`] | Stored LRU state for `read_set_i` |
| `write_en_i` | input | logic | Per-set write enable |
| `write_set_i` | input | logic [`DcacheSetIdxWidth-1:0`] | Set index to update |
| `write_data_i` | input | logic [`DcacheLruSetSize-1:0`] | Next LRU state |

### `minion_dcache_metadata_array`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Metadata array clock |
| `rst_ni` | input | logic | Async-assert, sync-deassert reset |
| `s1_read_set_i` | input | logic [`DcacheSetIdxWidth-1:0`] | Stage-1 read set |
| `s1_write_valid_i` | input | logic | Stage-1 metadata write enable |
| `s1_write_valid_prev_i` | input | logic | One-phase-early data-capture enable from the original interface |
| `s1_write_req_i` | input | `dcache_meta_write_req_t` | Set/way/tag/coherency write bundle |
| `s1_read_resp_valid_o` | output | logic [`DcacheWays-1:0`] | Per-way valid bits for `s1_read_set_i` |
| `s1_read_resp_o` | output | `dcache_meta_data_t [DcacheWays]` | Per-way tag/coherency read data |
| `cfg_clear_all_i` | input | logic | Invalidate all sets |
| `cfg_clear_low_i` | input | logic | Invalidate the low SCP-partition sets |

### `minion_dcache_buffer_array`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` / `rst_ni` | input | logic | Array clock and reset |
| `full_o` | output | logic | No free buffer entries remain |
| `reduce_full_o` | output | logic | No reduce-reserved entries remain |
| `alloc_req_pre_i` / `alloc_req_val_i` | input | logic | Pre-allocation handshake from the request pipe |
| `alloc_gsc_i` / `reduce_alloc_req_i` | input | logic | Gather/scatter and reduce allocation qualifiers |
| `s2_write_needed_core_i` / `s2_write_needed_i` | input | logic | Entry-consumption control from the dcache pipeline |
| `s2_write_en_i` / `s2_write_replay_i` / `s2_write_replay_entry_i` | input | logic / index | Internal write path and replay override |
| `s3_write_data_i` | input | logic [`DcacheDataSize-1:0`] | Internal 256-bit write payload |
| `s2_write_entry_o` / `s3_write_en_o` | output | index / logic | Selected buffer entry and delayed internal write valid |
| `s3_write_en_ext_i` / `s2_write_en_ext_prev_i` | input | logic | External write valid and one-phase-early capture enable |
| `s3_write_entry_ext_i` / `s3_write_data_ext_i` | input | index / data | External write destination and payload |
| `s2_dealloc_*` / `s1_dealloc_uc_*` / `reduce_dealloc_*` | input | logic / index | Entry release paths |
| `s1_read_entry_i` / `s1_read_data_o` | input / output | index / data | Combinational read port |

### `minion_dcache_128x64_1r1w_lram`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Array clock |
| `rd_addr_i` / `wr_addr_i` | input | logic [6:0] | Registered-read and write addresses |
| `rd_enable_i` | input | logic | Read-address capture enable |
| `rd_enablelob_i` / `rd_enablehib_i` | input | logic | Per-half read disables from the original interface |
| `wr_enablelo_pre_i` / `wr_enablehi_pre_i` | input | logic | One-cycle-early data-capture enables |
| `wr_enablelo_i` / `wr_enablehi_i` | input | logic | Per-half write commits |
| `wr_data_i` | input | logic [63:0] | Two 32-bit halves written through the wrapper’s preserved early-capture timing |
| `rd_data_o` | output | logic [63:0] | Two 32-bit registered-read halves |
| `ram_cfg_i` | input | `ram_cfg_t` | RAM tuning/test bundle retained as a wrapper-level integration hook; unused by the current latch/RF-backed implementation |

### `minion_dcache_128x72_1r1w_lram`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Array clock |
| `rd_addr_i` / `wr_addr_i` | input | logic [6:0] | Registered-read and write addresses |
| `rd_enable_i` | input | logic | Read-address capture enable |
| `rd_enablelob_i` / `rd_enablehib_i` | input | logic | Per-half read disables from the original interface |
| `wr_enablelo_pre_i` / `wr_enablehi_pre_i` | input | logic | One-cycle-early data-capture enables |
| `wr_enablelo_i` / `wr_enablehi_i` | input | logic | Per-half write commits |
| `wr_data_i` | input | logic [71:0] | Two 36-bit halves written through the wrapper’s preserved early-capture timing |
| `rd_data_o` | output | logic [71:0] | Two 36-bit registered-read halves |
| `ram_cfg_i` | input | `ram_cfg_t` | RAM tuning/test bundle retained as a wrapper-level integration hook; unused by the current latch/RF-backed implementation |

### `minion_dcache_data_array`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Shared data-array clock |
| `s1_read_valid_i` | input | logic | Stage-1 read request valid |
| `s1_read_req_i` | input | `dcache_da_read_req_t [DcacheLramNumBanks]` | Per-bank read bundle: half enables, way, and original 10-bit LRAM address |
| `s2_read_resp_o` | output | `dcache_da_data_t` | Per-bank 64-bit read data with independent half-word address hold behavior |
| `s3_write_valid_i` | input | logic | One-cycle-early write-data capture enable |
| `s4_write_valid_i` | input | logic | Stage-4 write commit valid |
| `s4_write_req_i` | input | `dcache_da_write_req_t [DcacheLramNumBanks]` | Per-bank write bundle: half enables, way, original 10-bit address, and 64-bit payload |
| `ram_cfg_i` | input | `ram_cfg_t` | RAM tuning/test bundle forwarded to each bank wrapper; unused by the current latch/RF-backed implementation |

### `minion_dcache_tlb_array`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Wrapper clock |
| `rst_ni` | input | logic | Async-assert, sync-deassert reset |
| `dft_i` | input | `dft_t` | Standard DFT bundle forwarded to `minion_tlb` |
| `esr_vmspagesize_i` | input | `tlb_entry_type` | Page-size control from the ESR |
| `req_data_i` | input | `dcache_tlb_req_t` | Dcache-local TLB request bundle |
| `req_valid_i` | input | logic | Request valid |
| `resp_data_o` | output | `dcache_tlb_resp_t` | Dcache-local TLB response bundle |
| `satp_info_i` / `matp_info_i` | input | `minion_satp_info` | S- and M-mode translation roots captured with the original delayed-enable timing |
| `satp_info_en_i` / `matp_info_en_i` | input | logic | One-cycle-early SATP/MATP capture strobes from the original interface |
| `tlb_invalidate_i` | input | logic | Dcache-driven TLB invalidate request |
| `ptw_req_data_o` | output | `minion_ptw_req` | PTW request bundle generated by the shared TLB |
| `ptw_req_valid_o` | output | logic | PTW request valid |
| `ptw_req_ready_i` | input | logic | PTW request ready |
| `ptw_resp_data_i` | input | `minion_ptw_pte` | PTW response bundle |
| `ptw_resp_valid_i` | input | logic | PTW response valid |
| `vm_enabled_o` | output | logic | VM enabled flag returned by the shared TLB |

### `minion_dcache_writeback_unit`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` / `rst_ni` | input | logic | Writeback pipeline clock and async-assert reset |
| `req_ready_o` | output | logic | Unit idle/accept-ready flag |
| `req_valid_i` | input | logic | Start a new writeback/message request |
| `req_data_i` | input | `dcache_wb_req_t` | Original request bundle: tag, set, way, tbox flag, mem index, and low address bits |
| `data_req_bid_o` | output | logic | Bid for a dcache data-array read slot |
| `data_req_grant_i` | input | logic | Data-array read slot granted |
| `data_req_valid_o` | output | logic | Accepted data-array read now enters the internal R1/R2/R3 pipeline |
| `data_req_o` | output | `dcache_da_read_req_t` | Data-array read request: half enables, way, and original LRAM address |
| `data_req_nack_i` | input | logic | Data-array response invalid; rollback and replay from the failed pass |
| `data_resp_i` | input | `dcache_da_data_t` | 256-bit read data returned from the dcache banks |
| `l2_req_ready_i` | input | logic | L2 writeback/message sink ready |
| `l2_req_valid_o` | output | logic | Final-stage L2 request valid |
| `l2_req_data_o` | output | `et_link_minion_evict_req_info_t` | Original ET-link writeback/message descriptor |

### `minion_dcache_scratchpad_ctrl`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` / `rst_ni` | input | logic | Controller clock and async-assert reset |
| `scratchpad_ctrl_i` | input | `dcache_mode_t` | Requested dcache operating mode from the core |
| `dcache_idle_i` | input | logic | Indicates the surrounding dcache is quiescent and safe to reconfigure |
| `dcache_cfg_stall_o` | output | logic | Stall the dcache while the mode transition is in progress |
| `dcache_cfg_clear_all_o` | output | logic | Clear the full cache when entering or leaving shared mode |
| `dcache_cfg_clear_low_o` | output | logic | Clear only the low partition when switching between split and scratchpad modes |
| `dcache_cfg_alloc_scp_o` | output | logic | Allocate scratchpad ownership during split-to-scratchpad transition |
| `da_clear_en_o` / `da_clear_val_o` | output | logic | Zero-write control for the data array clear walk |
| `da_clear_set_o` / `da_clear_way_o` / `da_clear_idx_o` | output | set / way / half selector | Current row being zeroed |
| `da_clear_rdy_i` | input | logic | Data-array clear handshake ready |
| `dcache_cfg_mode_o` | output | `dcache_mode_t` | Latched active mode once the clear sequence completes |

### `minion_dcache_texsend`

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` / `rst_ni` | input | logic | Preserved original interface clock/reset, unused in the non-graphics stub path |
| `neigh_min_id_i` | input | logic | Neighborhood-local minion ID, preserved for the future graphics-enabled path |
| `chicken_bit_dcache_i` | input | logic | Original clock-gating chicken bit, preserved for the future graphics-enabled path |
| `idle_o` | output | logic | Always `1` in the non-graphics stub path |
| `tex_send_ready_o` | output | logic | Always `1` in the non-graphics stub path |
| `tex_send_valid_i` / `tex_send_ctrl_i` | input | valid / `texsend_control_t` | Preserved texture-send request interface, ignored in the stub path |
| `l2_resp_valid_i` / `l2_resp_i` | input | valid / `et_link_minion_rsp_info_t` | Preserved Tbox response interface, ignored in the stub path |
| `l2_req_ready_i` | input | logic | Preserved Tbox request ready signal, ignored in the stub path |
| `l2_req_valid_o` / `l2_req_o` | output | valid / `et_link_minion_miss_req_info_t` | Always zero in the non-graphics stub path |
| `writeback_ready_i` / `writeback_done_i` | input | logic | Preserved writeback-unit handshake inputs, ignored in the stub path |
| `writeback_valid_o` / `writeback_req_o` | output | valid / `dcache_wb_req_t` | Always zero in the non-graphics stub path |
| `csr_debug_signals_o` | output | logic [`DcacheDbgCsrTxsndSize-1:0`] | Always zero in the non-graphics stub path |

### `minion_dcache_top`

`minion_dcache_top` is the full minion L1 dcache boundary. Its interface is the
same architectural surface as the original `dcache_top`, renamed to the repo’s
`_i` / `_o` convention:

| Port Group | Direction | Description |
|------------|-----------|-------------|
| Core request / response | input / output | Scalar load/store request issue, replay, scoreboard, exception, and final S3 response path |
| VPU / tensor sideband | input / output | Tensor-load, tensor-store, reduce, scratchpad, and TENB/SCP control/data paths |
| VM / PTW / PMA | input / output | SATP/MATP state, privilege state, TLB invalidate, PTW request/response, and PMA protection inputs |
| ET-link L2 | input / output | Miss, evict, and refill traffic to the outer memory hierarchy |
| Scratchpad / cache-mode control | input / output | Split/shared/scratchpad mode changes, clear-all/clear-low sequencing, and idle status |
| APB debug | input / output | Metadata/data-array debug read/write access and debug readback |
| Error / debug outputs | output | Cache-op, tensor-load, tensor-reduce, bus-error, IO event, and debug signal bundles |

## Integration Notes

- The package is local to this IP for now so the leaf blocks can land cleanly.
- As more minion blocks are ported, the shared subset in `minion_dcache_pkg`
  can move into a common `minion_pkg`.
- The metadata and buffer arrays preserve the original phase-2 write-data timing,
  including the original stale-data behavior when the early capture phase is not primed.
- The metadata and buffer arrays now express the original one-phase-early
  write-preview plus RF timing through `prim_rf_1r1w_preview`.
- The 64-bit and 72-bit LRAM wrappers now express the original one-phase-early
  write-enable plus registered-read RF contract through
  `prim_rf_1r1w_reg_preview`, preserving the staged write-data behavior without
  leaving raw low-phase latch helpers in active shared RTL.
- The replay queue now expresses the original one-phase-early write-preview
  plus parallel-readback RF timing through `prim_rf_1r1w_par_preview`,
  instead of open-coding a split preview latch and RF composition.
- The tensor-load single-entry capture path now expresses the original
  one-phase-early write-preview plus single-entry write-capture timing through
  `prim_rf_single_1r1w_par_preview`, keeping the ECP5-sensitive preview
  contract inside a single named seam.
- `minion_dcache_data_array` preserves the original banked half-line behavior:
  each 32-bit half tracks its own read-address hold state, and an unprimed
  stage-4 write reuses the previously captured write payload just like the
  original latch-based implementation.
- `minion_dcache_tlb_array` preserves the original delayed SATP/MATP capture:
  `*_info_en_i` is sampled one cycle before the corresponding info struct, and
  the wrapper intentionally drops `minion_tlb.ptw_invalidate_o` because the
  original `dcache_tlb_array` left that output unused.
- `minion_dcache_tlb_array` also preserves the original PTW-response write
  quirk observed in cosim: a single-cycle PTW response pulse can leave stale
  RF contents visible on the next lookup, while a two-cycle hold writes the
  expected cached translation and permission bits.
- `minion_dcache_writeback_unit` preserves the original three-stage release
  pipeline and its exact replay behavior: a data-array nack or L2 backpressure
  clears the flying request state and rolls `data_req_cnt` back by the number
  of in-flight passes so the failing half-line is retried rather than skipped.
- `minion_dcache_scratchpad_ctrl` preserves the original configuration timing:
  the requested mode is only committed after the clear walk finishes, and the
  clear-row counter is emitted directly as `{set, idx, way}` just like the
  original packed LRAM-row address.
- `minion_dcache_texsend` is intentionally limited to the exact `ifndef GFX_ENABLED`
  branch from the original RTL. Enabling graphics/Tbox behavior in this repo
  will require either extending `minion_dcache_texsend` with the original
  `GFX_ENABLED` request/pull state machines, FIFO, and writeback sequencing,
  or introducing a separate graphics-enabled texsend module that preserves the
  same external contract.
- `minion_dcache_top` preserves the original invalid-metadata refill quirk:
  invalid metadata entries keep the set-MSB bits folded into the stored tag
  field, which matters for the APB metadata/debug readback path and therefore
  for cycle-accurate top-level cosim.
- The LRAM wrappers preserve the original minion dcache local-RAM clocking
  surface: the original data-bank path has only the functional `clock`, so the
  reimplementation does not add a DFT SRAM clock override at this level.
- The LRAM wrappers do not have reset ports; like the original local RAMs, their
  contents are undefined until the owning pipeline writes them.

## Differences From Original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Reset | Active-high `reset` on AMO leaf | Active-low `rst_ni` | Project reset convention |
| Naming | `clock`, `reset`, bare signal names | lowRISC `_i` / `_o` names | Project naming convention |
| Types | `include "soc.vh"` global typedefs | `minion_dcache_pkg` package | Local package-based ownership |
| Module names | `dcache_*` | `minion_dcache_*` | Avoid name collisions in cosim and repo namespace |
| Shared TLB IP | Local wrapper instantiates `tlb_top` directly | Local wrapper instantiates `minion_tlb` and adapts dcache-local request/response bundles | Reuse the standalone TLB IP while preserving the original dcache-facing contract |
| SRAM abstraction | Original array path can elaborate against different memory implementations | Reimplementation keeps the dcache-facing wrapper and latch/RF-backed storage seam, with `ram_cfg_i` retained as an unused integration hook | Match the repo technology-abstraction rules while preserving the legacy wrapper contract |

No functional changes are intended. The logic is translated directly from the
original leaf modules, including interface quirks such as the unused clock/reset
inputs on the atomic ALU and the preserved lower-half intermediates in the
store-merge path.
