# prim_rf_1r1w_reg_preview

`prim_rf_1r1w_reg_preview` is a same-width 1-read/1-write register-file
primitive with one-phase-early write-data preview and a registered read
address.

It preserves the original `LATCH_P2` plus `rf_latch_1r_1w_reg` contract used
by the D-cache local RAM wrappers: write payload is staged under a previewed
enable, writes commit later under `wr_en_i`, and reads use a held read address
with one-cycle address latency.

## Contract

The RF stores `Depth` entries of `Width` bits.

Writes are two-step:

1. `wr_data_en_1p_next_i` is captured into the internal preview enable during
   the low phase of `preview_clk_i`.
2. On the low phase of `rf_clk_i`, the previewed enable captures `wr_data_i`
   into staged write data.
3. On the write phase of `rf_clk_i`, `wr_en_i` commits the staged write data
   into `rf[wr_addr_i]`.

Reads register the address, not the output data. On a write phase of
`rf_clk_i`, `rd_en_i` captures `rd_addr_i` into the held read address. The
output is combinational from `rf[held_rd_addr]`, so the read address has one
cycle of latency while the data output itself is not an additional flop.

`wr_data_en_1p_next_i` is not a same-cycle bypass. A write commit without a
previously active preview enable commits the previously staged payload, not the
current raw `wr_data_i`.

The original latch-array implementation is transparent during the write phase.
When the held read address selects the same entry being committed, the
parent-visible value is the staged write data for that write phase and the
stored value after commit.

## Parameters

| Parameter | Meaning | Legal values | Notes |
|---|---|---|---|
| `Width` | Data width of each RF entry | `> 0` | Read and write data have the same width |
| `Depth` | Number of RF entries | `> 0` | `Depth=1` uses a 1-bit address port |
| `PreviewClkSameAsRf` | Technology hint for the preview/RF clock relationship | `0` or `1` | Set to `1` only when `preview_clk_i` and `rf_clk_i` are the same physical clock |
| `AddrW` | Derived address width | `(Depth > 1) ? $clog2(Depth) : 1` | Localparam |

Technology implementations may support only a subset of legal parameter
values. Unsupported technology-specific configurations must fail at
elaboration/synthesis instead of silently changing behavior.

## Ports

| Port | Direction | Timing owner | Meaning | Stability requirements |
|---|---|---|---|---|
| `preview_clk_i` | input | Preview phase | Captures `wr_data_en_1p_next_i` | Same clock as `rf_clk_i`, or phase-equivalent local derivative |
| `rf_clk_i` | input | RF storage | Stages previewed payload, commits writes, and captures read address | Phase-related to `preview_clk_i` |
| `wr_data_en_1p_next_i` | input | Caller | One-phase-early payload capture qualifier | Valid during the low phase before the intended write |
| `wr_data_i` | input | Caller | Write payload | Valid when the previewed enable allows staging |
| `wr_addr_i` | input | Caller | Write entry address | Valid when `wr_en_i` commits |
| `wr_en_i` | input | Caller | RF commit enable | Valid at the write phase |
| `rd_addr_i` | input | Caller | Read address to capture | Valid when `rd_en_i` is asserted at the write phase |
| `rd_en_i` | input | Caller | Read-address capture enable | Valid at the write phase |
| `rd_data_o` | output | Primitive | Combinational data from held read address | Follows RF contents and write-phase transparency |

## Timing

### Normal Write And Registered Read

```text
clk                     ___/^^^^\____/^^^^\____/^^^^\____
wr_data_en_1p_next_i       ===1===       0
previewed enable           ===1===       0
wr_data_i                  ---- D ----
wr_en_i                                 ----1----
wr_addr_i                               ---- A ----
rd_en_i                                 ----1----
rd_addr_i                               ---- R ----
staged payload                   <= D
rf[A]                                      <= D
held read address                          <= R
rd_data_o                                  rf[R] after held address updates
```

### Read Address Hold

```text
rd_en_i                 1          0          0
rd_addr_i               A          B          C
held read address       <= A       holds A    holds A
rd_data_o               rf[A]      rf[A]      rf[A]
```

### Commit Without Preview

```text
wr_data_en_1p_next_i       0
wr_data_i                  Dnew
wr_en_i                              1
staged payload           unchanged from prior previewed capture
rf[wr_addr_i]                       <= prior staged payload, not raw Dnew
```

## Legal Use

- `preview_clk_i` and `rf_clk_i` are the same clock, or phase-equivalent local
  derivatives from the same owning clock domain.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- `wr_data_en_1p_next_i` is generated one phase before the corresponding
  `wr_en_i`, `wr_addr_i`, and `wr_data_i` commit.
- `wr_data_en_1p_next_i` may be tied high when unconditional write-payload
  staging is acceptable.
- Read users account for one cycle of read-address latency.

## Illegal Use

- Do not use unrelated or asynchronous clocks on `preview_clk_i` and
  `rf_clk_i`.
- Do not use `wr_data_en_1p_next_i` as a same-cycle write-data bypass.
- Do not treat `rd_addr_i` as a combinational read address.
- Do not treat `rd_data_o` as an additionally registered output.

## Tests

```bash
make -C hw/ip/prim_rf_1r1w_reg_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_1r1w_reg_preview test
```
