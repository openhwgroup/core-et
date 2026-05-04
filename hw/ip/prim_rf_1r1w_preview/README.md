# prim_rf_1r1w_preview

`prim_rf_1r1w_preview` is a same-width 1-read/1-write register-file
primitive with an explicit one-phase-early write-data preview qualifier.

It preserves the original low-phase preview plus latch-timed RF contract:
the caller presents `wr_data_en_1p_next_i` before the write phase, the
primitive captures that preview during the low phase, captures write data
under the previewed enable, and commits the staged data to the RF on the write
edge when `wr_en_i` is asserted.

## Contract

The primitive has zero-latency combinational reads. `rd_addr_i` selects the RF
entry visible on `rd_data_o`.

Writes are two-step:

1. `wr_data_en_1p_next_i` is captured into an internal preview enable during
   the low phase of `preview_clk_i`.
2. The previewed enable controls write-data staging for the RF write performed
   by `wr_en_i`, `wr_addr_i`, and `wr_data_i` on `rf_clk_i`.

`wr_data_en_1p_next_i` is not a same-cycle bypass. A write commit without a
previously active preview enable commits the previously staged payload, not the
current raw `wr_data_i`.

## Parameters

| Parameter | Meaning | Legal values | Notes |
|---|---|---|---|
| `Width` | Data width of each RF entry | `> 0` | Read and write data have the same width |
| `Depth` | Number of RF entries | `> 0` | Address width is `$clog2(Depth)` |
| `PreviewClkSameAsRf` | Technology hint for the preview/RF clock relationship | `0` or `1` | Set to `1` only when `preview_clk_i` and `rf_clk_i` are the same physical clock |
| `AddrW` | Derived address width | `$clog2(Depth)` | Localparam |

## Ports

| Port | Direction | Timing owner | Meaning | Stability requirements |
|---|---|---|---|---|
| `preview_clk_i` | input | Preview phase | Clock used to capture `wr_data_en_1p_next_i` | Must be the same clock as `rf_clk_i`, or a proven phase-equivalent local derivative |
| `rf_clk_i` | input | RF storage | Clock used for payload staging and RF commit | Must be phase-related to `preview_clk_i` |
| `wr_data_en_1p_next_i` | input | Caller | One-phase-early write-data capture qualifier | Valid during the low phase before the intended write |
| `wr_en_i` | input | Caller | RF commit enable | Valid at the RF commit edge |
| `wr_addr_i` | input | Caller | RF write address | Valid when `wr_en_i` commits |
| `wr_data_i` | input | Caller | RF write payload | Valid when the previewed enable allows payload staging |
| `rd_addr_i` | input | Caller | RF read address | May change combinationally |
| `rd_data_o` | output | Primitive | RF read data | Combinationally follows `rd_addr_i` and RF contents |

## Timing

### Normal Write

```text
clk                     ___/^^^^\____/^^^^\____/^^^^\____
wr_data_en_1p_next_i       ===1===       0
previewed enable           ===1===       0
wr_data_i                  ---- D ----
wr_en_i                                 ----1----
wr_addr_i                               ---- A ----
staged payload                   <= D
rf[A]                                      <= D
rd_data_o                                  D when rd_addr_i == A after commit
```

### Preview Without Commit

```text
clk                     ___/^^^^\____/^^^^\____
wr_data_en_1p_next_i       1
wr_data_i                  D
wr_en_i                              0
staged payload           may capture D
rf[...]                  unchanged because wr_en_i=0
```

### Commit Without Preview

```text
clk                     ___/^^^^\____/^^^^\____
wr_data_en_1p_next_i       0
wr_data_i                  Dnew
wr_en_i                              1
staged payload           unchanged from prior previewed capture
rf[wr_addr_i]                       <= prior staged payload, not raw Dnew
```

### Combinational Read

```text
rd_addr_i               A ------ B ------ A
rd_data_o               rf[A] -- rf[B] -- rf[A]
```

Same-edge read/write collision behavior is not a bypass contract. Callers must
not rely on reading a just-written value in the same scheduling instant unless a
parent module explicitly specifies and proves that behavior.

## Legal Use

- `preview_clk_i` and `rf_clk_i` are the same clock, or a phase-equivalent
  local derivative with the same parent timing.
- Set `PreviewClkSameAsRf=0` when `preview_clk_i` can be gated, held, or
  otherwise separated while `rf_clk_i` continues to toggle.
- `wr_data_en_1p_next_i` is generated one phase before the corresponding
  `wr_en_i`, `wr_addr_i`, and `wr_data_i` commit.
- `wr_data_en_1p_next_i` may be tied high if the caller cannot generate an
  early qualifier and accepts unconditional payload staging.
- Reads are used as asynchronous/combinational RF reads.

## Illegal Use

- Do not use `wr_data_en_1p_next_i` as a same-cycle write-data bypass.
- Do not use unrelated or asynchronous clocks on `preview_clk_i` and
  `rf_clk_i`.
- Do not assume same-edge read/write bypass behavior.
- Do not treat `rd_data_o` as registered output data.

## Tests

```bash
make -C hw/ip/prim_rf_1r1w_preview/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/prim_rf_1r1w_preview test
```
