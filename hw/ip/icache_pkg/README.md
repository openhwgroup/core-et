<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_pkg

Shared standalone-Icache constants and types.

## Description

`icache_pkg` collects the old frontend/Icache constants, enums, structs, and
LRU helper functions that are still needed by the standalone-neighborhood
Icache path. It is the package home for modules such as:

- `icache_data_ram_wrap`
- `icache_top`

The package is intentionally narrower than the old global header surface:

- generic Minion architectural constants still live in `minion_pkg`
- frontend-core interface structs already translated for Minion stay in
  `minion_frontend_pkg`
- Shire-cache MBIST and cache-system constants stay in `shirecache_pkg`

`icache_pkg` only carries the Icache-local pieces that do not already have a
cleaner package home.

The fixed geometry and SRAM-layout constants shared with other blocks live in
`icache_geom_pkg`, so there is a single source of truth for values also needed
by `shirecache_pkg`.

## Contents

- geometry constants imported from `icache_geom_pkg`
- APB debug address constants used by the SRAM wrappers
- Icache-local FSM enums
- Icache TLB/tag/error-log structs
- aliases for frontend/Icache request/response structs
- `icache_lru_victim()` and `icache_lru_update()`

## Differences From Original

| Original | This repo | Reason |
|----------|-----------|--------|
| `frontend_defines.vh` + `frontend_types.vh` | `icache_pkg.sv` package | Replace global macro/type headers with typed package content |
| shared frontend structs duplicated in old headers | aliases to `minion_frontend_pkg` | Reuse the already translated frontend interface package |
