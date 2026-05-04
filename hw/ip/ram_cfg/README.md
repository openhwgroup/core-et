# ram_cfg

RAM configuration type definitions package.

Provides a standardized `ram_cfg_t` struct for SRAM timing margin tuning and test modes. Replaces the CORE-ET `esr_shire_cache_ram_cfg_t`. In simulation and FPGA, `ram_cfg_i = '0`. For ASIC, these are driven by ESR registers for post-silicon yield tuning.

## Package: `ram_cfg_pkg`

```systemverilog
import ram_cfg_pkg::*;
```

### Fields of `ram_cfg_t`

| Field | Width | Description |
|-------|-------|-------------|
| `test_en` | 1 | RAM test mode enable |
| `rm` | 4 | Read margin |
| `rme` | 1 | Read margin enable |
| `ra` | 2 | Read assist |
| `wa` | 2 | Write assist |
| `wpulse` | 3 | Write pulse width |
| `deep_sleep` | 1 | RAM power-down (low-leakage retention mode) |
| `shut_down` | 1 | RAM shutdown (data lost) |

### Differences from CORE-ET

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Type | `esr_shire_cache_ram_cfg_t` — nested struct-of-structs with `cfg1`/`cfg2`/`cfg3`/`cfg4` plus DFT fields | Flat `ram_cfg_t` with essential fields only | Simpler, foundry-independent |
| Scope | Threaded through entire design from ESR to every RAM | Same — passed via `cfg_i` port on `prim_ram_*` | Same intent, cleaner type |
