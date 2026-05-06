# CORE-ET Agentic Silicon Platform (ETASP)

The **CORE-ET Agentic Silicon Platform (ETASP)** is an Ainekko project for
collecting hardware IP in a form that can be translated, verified, documented,
and integrated by agentic hardware-development workflows.

ETASP is currently seeded by an active translation of CORE-ET modules into clean
SystemVerilog. That translation work is the first campaign, not the whole
platform: the broader goal is to build a reusable IP catalog that supports both
FPGA prototyping and ASIC-oriented implementation through explicit technology
abstraction, verification collateral, and integration documentation.

## Platform Model

ETASP is organized around two agentic workflows:

| Workflow | Role |
|----------|------|
| **Translator** | Takes legally available, license-compatible source IP and rewrites it into ETASP form: lowRISC-style SystemVerilog, per-IP documentation, explicit parameters, technology primitives, unit tests, RTL co-simulation, and coverage. |
| **Integrator** | Builds systems from existing ETASP components. For translated modules, integration should preserve the translated behavior and steer changes toward documented parameters, wrappers, or new system RTL rather than ad hoc edits to the translated block. |

## Current Scope

The repository is actively translating CORE-ET modules and support
infrastructure into ETASP. The current contents include:

| Area | Status |
|------|--------|
| CORE-ET IP blocks | Actively translated into per-IP directories under `hw/ip/`. |
| Technology primitives | Generic, iCE40, Xilinx, and future ASIC seams for clocks, resets, RAMs, latch/phase constructs, FIFOs, and register files. |
| Verification | Verilator-based unit tests plus RTL co-simulation against the original source tree. |
| FPGA projects | Example synthesis and simulation heads under `fpga/`. |
| Coverage | Unit-test coverage generated locally and checked-in co-simulation LCOV data for CI dashboard generation. |

See [STATUS.md](STATUS.md) for the current module, test, and co-simulation
status.

## Quick Start

### 1. Install Prerequisites

The tested CI environment is Ubuntu 24.04 with `bash`.

```bash
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    build-essential git make autoconf flex bison \
    libfl2 libfl-dev help2man perl python3 zlib1g-dev
```

Install or place on `PATH`:

| Tool | Tested version / requirement | Use |
|------|------------------------------|-----|
| `bash` | GNU bash on Ubuntu 24.04 | Supported shell for documented commands. |
| `python3` | Ubuntu 24.04 Python 3 | Coverage dashboard generation and helper scripts. |
| `verilator` | CI pins Verilator `v5.046`; Verilator 5+ is required. | RTL lint, unit simulation, co-simulation, coverage. |
| `yosys` + `slang` | CI pins oss-cad-suite `2026-04-12`. | SystemVerilog parsing and FPGA synthesis. |
| `g++` | Ubuntu 24.04 default GCC/G++ | C++ test harness builds. |
| RISC-V GCC | Not required for the current IP-level checks. | Future system-level software tests and benchmarks. |

Co-simulation also needs a checkout of the original CORE-ET RTL. Point
`ORIG_ROOT` at that checkout:

```bash
export ORIG_ROOT=~/ainekko/etcore-soc   # adjust for your machine
```

`ORIG_ROOT` must contain the original `rtl/inc`, `rtl/libs`, and `rtl/shire`
trees. If `ORIG_ROOT` is unset, `mk/rtlcosim.mk` falls back to its default
relative original-RTL checkout path. This original RTL checkout is the reference
model for co-simulation.

### 2. Run Core Checks

```bash
# Run all IP unit tests
make test

# Lint all RTL
make lint

# Run all RTL co-simulation tests
make -C dv/rtlcosim test

# Generate line/branch/toggle LCOV summaries
make coverage-report

# Generate the interactive coverage dashboard
make coverage-html
```

### 3. Run Focused Checks

```bash
# Run one IP's tests
make -C hw/ip/prim_fifo_sync/dv test

# Run one IP's tests with VCD tracing
make -C hw/ip/prim_fifo_sync/dv test-trace

# Run one co-simulation
make -C dv/rtlcosim/prim_fifo test
```

### 4. Run FPGA Example Flows

```bash
# iCE40 synthesis
make -C fpga/fifo_example/ice40 synth

# Xilinx Ultrascale+ synthesis
make -C fpga/fifo_example/xilinx synth

# Resource report
make -C fpga/fifo_example/ice40 report

# Verilator simulation of the project head
make -C fpga/fifo_example/verilator test
```

## Repository Structure

| Path | Description |
|------|-------------|
| `hw/ip/<block>/` | Per-IP blocks. Each translated block carries RTL, DV, and a README at the IP root. |
| `hw/ip/tech_generic/` | Behavioral technology primitives used by default for simulation. |
| `hw/ip/tech_ice40/` | iCE40 implementations of technology primitives. |
| `hw/ip/tech_ecp5/` | ECP5 implementations of technology primitives. |
| `hw/ip/tech_xilinx/` | Xilinx FPGA implementations of technology primitives. |
| `hw/top/` | Top-level chip integration area, reserved for larger system assemblies. |
| `fpga/<project>/` | FPGA-oriented projects with backend heads for synthesis and simulation. |
| `dv/common/` | Shared C++ DV utilities, including `sim_ctrl.h`. |
| `dv/rtlcosim/` | RTL co-simulation tests comparing translated modules against original source RTL. |
| `dv/rtlcosim/coverage/` | Checked-in co-simulation LCOV `.info` files used by CI. |
| `dv/coverview/` | Coverage dashboard frontend and embedding scripts. |
| `mk/` | Shared Make infrastructure for Verilator, co-simulation, primitive selection, and Yosys synthesis. |
| `syn/` | Synthesis scripts and shared synthesis collateral. |
| `sw/` | Firmware and software area. |
| `vendor/` | Third-party IP area. |
| `docs/` | Project-wide translation, coding-style, onboarding, and architecture references. |

## Configuration

Translated IP uses explicit SystemVerilog parameters and package-level types or
constants instead of preprocessor-controlled feature selection. Shared
configuration packages live beside the relevant IP under `hw/ip/<name>/rtl/`
or as dedicated package IPs such as `hw/ip/dft`, `hw/ip/ram_cfg`, and protocol
packages.

For integration work, configurable behavior should be introduced as documented
parameters or package configuration, then described in the affected IP README.
When a translated block must intentionally diverge from the original behavior,
that decision must be documented in both the IP README and the relevant
`BUGS.md` entry when it relates to an original repository bug.

## Verification Model

ETASP verification is built around reproducible, open-source checks:

| Check | Description |
|-------|-------------|
| Unit tests | Per-IP C++ tests under `hw/ip/<block>/dv/`, run with Verilator and explicit `check()` assertions. |
| RTL co-simulation | Per-module tests under `dv/rtlcosim/<module>/` that instantiate original and translated RTL side by side and compare outputs cycle by cycle. |
| Lint | Verilator lint must pass without blanket warning suppression. |
| Coverage | Verilator line, branch, and toggle coverage is generated as LCOV `.info` data and rendered through Coverview. |
| Synthesis smoke tests | Example FPGA projects are synthesized through Yosys with the slang frontend. |

Current simulation support is Verilator. Additional commercial simulator flows
can be added as integration needs require. System-level RISC-V architecture
tests and benchmarks such as CoreMark or Dhrystone are expected to live in the
system/software verification flow as ETASP grows from IP catalog to integrated
CPU subsystem.

## Technology Abstraction

Technology-dependent structures are isolated behind primitives with stable
module contracts. Functional RTL instantiates technology primitives such as
clock gates, reset synchronizers, RAM wrappers, phase/latch composites, and CDC
FIFOs. The build selects the implementation through `mk/prim.mk` and the
`TECH` variable.

This keeps translated IP usable across:

| Target | Implementation path |
|--------|---------------------|
| Simulation / generic | `hw/ip/tech_generic/` |
| iCE40 FPGA | `hw/ip/tech_ice40/` |
| ECP5 FPGA | `hw/ip/tech_ecp5/` |
| Xilinx FPGA | `hw/ip/tech_xilinx/` |
| ASIC | Future foundry-specific technology tree |

## Documentation

Project-wide policies and references:

| Document | Purpose |
|----------|---------|
| [docs/onboarding.md](docs/onboarding.md) | Practical guide for navigating the repository and translation flow. |
| [docs/translation.md](docs/translation.md) | Maintained CORE-ET-to-ETASP translation reference. |
| [docs/minion_vpu_standalone_plan.md](docs/minion_vpu_standalone_plan.md) | Completed dependency map/history for closing the real-VPU Minion path through original-equivalent `standalone_minion`. |
| [docs/coding_style.md](docs/coding_style.md) | SystemVerilog style and tool-compatibility rules. |
| [docs/future_asic_notes.md](docs/future_asic_notes.md) | Notes for ASIC-oriented infrastructure and future technology hooks. |

Each IP block should also carry its own `README.md` describing the module,
parameters, ports, integration notes, constraints, and any intentional
differences from the original source module.

## Governance

ETASP is licensed under [Apache-2.0](LICENSE). Contributions are governed by
[CONTRIBUTING.md](CONTRIBUTING.md), including the Eclipse Contributor Agreement
requirement and the policy for human-reviewed use of AI-assisted development
tools. Project behavior expectations are documented in
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).
