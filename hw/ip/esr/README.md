# esr

ESR (ET System Registers) generator and shared ESR type package.

## Description

This directory contains the ET System Register CSV database plus the generator
used to emit clean SystemVerilog ESR/CSR blocks. ESR blocks provide the
APB-accessible configuration and status registers for standalone-shell and
subsystem integration logic.

The shared package at [`rtl/esr_pkg.sv`](rtl/esr_pkg.sv) carries ESR-wide type
definitions, reset values, and address constants that are needed by hand-ported
ESR blocks such as `esr_spio`, `esr_neigh`, and the DLL delay estimator.
Neighborhood ICache error-log payload structs remain in `icache_pkg` and are
referenced by `esr_neigh` to keep one packed-layout definition for the
ICache/ESR boundary.

## Status

The unified generator at [`scripts/gen_esr.py`](scripts/gen_esr.py) is ported
and is already used for the Minion CSR subsystem. ESR block-by-block adoption is
incremental: some ESR clients are still hand-wired today, while newly ported ESR
blocks reuse the shared package and translated APB logic directly.

## Contents

- `data/` — ESR register definitions
- `rtl/esr_pkg.sv` — shared ESR structs/constants used by translated ESR blocks
- `scripts/` — generator scripts
- `Makefile` — build rules for ESR generation
