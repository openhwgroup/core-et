# prim_rf_1r1w_diff_preview

`prim_rf_1r1w_diff_preview` is a mixed-width 1-read/1-write register-file
primitive with per-read-lane write-data preview enables.

It preserves the original `LATCH_P2` plus `rf_latch_1r_1w_diff_widths`
contract: the caller presents one preview bit per `RWidth` write lane before
the write phase, the primitive stages only the enabled write lanes, and
`wr_en_i` commits staged lanes into one `WWidth` entry.

## Contract

The RF stores `Entries` write-width entries. Each entry is `WWidth` bits and is
split into `WWidth / RWidth` independently staged and committed lanes.

Reads are combinational and use an `RWidth` window over the concatenated RF
contents. Adjacent read windows are separated by `RAlignment`, so the read port
can address overlapping windows when `RAlignment < RWidth`.

Writes are two-step:

1. `wr_data_en_1p_next_i[lane]` is captured into the internal preview enable
   during the low phase of `preview_clk_i`.
2. On the low phase of `rf_clk_i`, each previewed lane captures
   `wr_data_i[lane*RWidth +: RWidth]` into staged write data.
3. On the write phase of `rf_clk_i`, each asserted `wr_en_i[lane]` commits the
   corresponding staged lane into `rf[wr_addr_i]`.

`wr_data_en_1p_next_i` is not a same-cycle bypass. A lane commit without a
previously active preview enable commits that lane's previously staged payload,
not the current raw `wr_data_i`.

The original latch-array implementation is transparent during the write phase.
When a read window overlaps a lane being committed, the parent-visible value is
the staged lane for that write phase and the stored lane after commit.

## Parameters

| Parameter | Meaning | Legal values | Notes |
|---|---|---|---|
| `RWidth` | Read window width and write-lane width | `> 0` | One preview/commit bit per `RWidth` lane |
| `RAlignment` | Distance in bits between adjacent read windows | `> 0`, `<= RWidth`, must divide `RWidth` | Allows overlapping read windows |
| `WWidth` | Stored entry width and write payload width | `> 0`, must be a multiple of `RWidth` | Defines `WWidth / RWidth` write lanes |
| `Entries` | Number of write-width entries | `> 0` | `wr_addr_i` selects one entry |
| `PreviewClkSameAsRf` | Technology hint for the preview/RF clock relationship | `0` or `1` | Set to `1` only when `preview_clk_i` and `rf_clk_i` are the same physical clock |
| `R2WRatio` | Derived write-lane count | `WWidth / RWidth` | Localparam |
| `REntries` | Derived read-window count | `(Entries * WWidth) / RWidth * (RWidth / RAlignment)` | Localparam |
| `RAddrW` | Derived read address width | `$clog2(REntries)` | Localparam |
| `WAddrW` | Derived write address width | `$clog2(Entries)` | Localparam |

Technology implementations may support only a subset of legal parameter
values. Unsupported technology-specific configurations must fail at
elaboration/synthesis instead of silently changing behavior.

## Ports

| Port | Direction | Timing owner | Meaning | Stability requirements |
|---|---|---|---|---|
| `preview_clk_i` | input | Preview phase | Captures `wr_data_en_1p_next_i` | Same clock as `rf_clk_i`, or phase-equivalent local derivative |
| `rf_clk_i` | input | RF storage | Stages previewed payload lanes and commits RF writes | Phase-related to `preview_clk_i` |
| `wr_data_en_1p_next_i` | input | Caller | One-phase-early per-lane payload capture qualifier | Valid during the low phase before the intended lane write |
| `rd_data_o` | output | Primitive | Combinational read data | Follows `rd_addr_i`, RF contents, and write-phase transparency |
| `rd_addr_i` | input | Caller | Read-window address | May change combinationally |
| `wr_data_i` | input | Caller | Write payload containing all lanes | Lane data valid when its previewed enable allows staging |
| `wr_addr_i` | input | Caller | Write entry address | Valid when any `wr_en_i` bit commits |
| `wr_en_i` | input | Caller | Per-lane RF commit enables | Valid at the write phase |

## Timing

### Normal Lane Write

```text
clk                         ___/^^^^\____/^^^^\____/^^^^\____
wr_data_en_1p_next_i[lane]     ===1===       0
previewed enable[lane]         ===1===       0
wr_data_i[lane]                ---- D ----
wr_en_i[lane]                                ----1----
wr_addr_i                                    ---- A ----
staged lane                         <= D
rf[A][lane]                                  <= D
rd_data_o                                    D when read window overlaps A/lane
```

### Partial-Lane Write

```text
wr_data_en_1p_next_i       00101100
wr_en_i                    00101100
wr_data_i                  lanes 2,3,5 valid
rf[wr_addr_i]              only lanes 2,3,5 are updated
other lanes                unchanged
```

### Preview Without Commit

```text
wr_data_en_1p_next_i[lane]     1
wr_data_i[lane]                D
wr_en_i[lane]                         0
staged lane                  may capture D
rf[...]                      unchanged because wr_en_i[lane]=0
```

### Commit Without Preview

```text
wr_data_en_1p_next_i[lane]     0
wr_data_i[lane]                Dnew
wr_en_i[lane]                         1
staged lane                  unchanged from prior previewed capture
rf[wr_addr_i][lane]                   <= prior staged lane, not raw Dnew
```

### Overlapping Read Windows

```text
RWidth=32, RAlignment=16
rf bits                    [63:32] [31:0]
rd_addr_i=0                rf[31:0]
rd_addr_i=1                rf[47:16]
rd_addr_i=2                rf[63:32]
```

## Legal Use

- `preview_clk_i` and `rf_clk_i` are the same clock, or phase-equivalent
  local derivatives from the same owning clock domain.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- `wr_data_en_1p_next_i` is generated one phase before the corresponding
  `wr_en_i`, `wr_addr_i`, and `wr_data_i` commit.
- `wr_data_en_1p_next_i` may be tied high for lanes whose payload should always
  be staged.
- Read users accept combinational RF output and write-phase transparency.

## Illegal Use

- Do not use unrelated or asynchronous clocks on `preview_clk_i` and
  `rf_clk_i`.
- Do not use `wr_data_en_1p_next_i` as a same-cycle write-data bypass.
- Do not assume inactive `wr_en_i` bits update the RF, even if their preview
  bits captured payload.
- Do not instantiate unsupported width/alignment combinations in a technology
  tree that reports them as unsupported.

## Tests

```bash
make -C hw/ip/prim_rf_1r1w_diff_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_1r1w_diff_preview test
```
