#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
gen_plic_rdl.py — Generate SystemRDL for a RISC-V PLIC

Author: Claude

Configuration
─────────────
  NUM_SOURCES  = 10   (interrupt sources, IDs 1..10; ID 0 is reserved)
  NUM_CONTEXTS = 32   (2 per hart: M-mode and S-mode, 16 harts total)
  BASE_ADDR    = 0xA000_0000

PLIC Memory Map (offsets from BASE_ADDR — per RISC-V PLIC spec)
───────────────────────────────────────────────────────────────
  0x000004 + (src-1)*4          Priority register, source 1..N    (RO = 1)
  0x001000                      Interrupt pending bits             (RO, hw-managed)
  0x002000 + C*0x0080           Interrupt enable bits, context C  (RW)
  0x200000 + C*0x1000 + 0x000   Priority threshold, context C     (RO = 0)
  0x200000 + C*0x1000 + 0x004   Claim / Complete, context C       (RW, hw-managed)
"""

import math
import sys

# ── Configuration ──────────────────────────────────────────────────────────────
NUM_SOURCES  = 6
NUM_CONTEXTS = 32
BASE_ADDR    = 0xA000_0000
OUTPUT_FILE  = "plic.rdl"

# Derived
ID_WIDTH = math.ceil(math.log2(NUM_SOURCES + 1))   # 4 bits covers IDs 0..6

# ── Output helpers ─────────────────────────────────────────────────────────────
_lines: list[str] = []

def w(s: str = "") -> None:
    _lines.append(s)

def section(title: str, addr_hint: str = "") -> None:
    label = f" {title} "
    if addr_hint:
        label += f"({addr_hint}) "
    bar_len = max(0, 65 - len(label))
    w(f"    // ──{label}{'─' * bar_len}")

# ── File header ────────────────────────────────────────────────────────────────
w("// =============================================================")
w("// plic.rdl — RISC-V PLIC SystemRDL Definition")
w("//")
w(f"//   Base address      : 0x{BASE_ADDR:08X}")
w(f"//   Interrupt sources : {NUM_SOURCES}  (IDs 1..{NUM_SOURCES}; ID 0 is reserved/no-interrupt)")
w(f"//   Contexts          : {NUM_CONTEXTS}  (M-mode + S-mode per hart, {NUM_CONTEXTS // 2} harts)")
w("//   Priorities        : NONE (priority regs RO=1, threshold RO=0)")
w("//")
w("// =============================================================")
w()
w()

# ══════════════════════════════════════════════════════════════════════════════
# Register type definitions
# ══════════════════════════════════════════════════════════════════════════════

# ── 1. Priority register ───────────────────────────────────────────────────────
w("// ─────────────────────────────────────────────────────────────────────────")
w("// plic_priority_t")
w("// Read-only, hardwired to 1.")
w("// No priority levels are implemented; every active source has equal")
w("// priority and will be forwarded to any context whose threshold == 0.")
w("// ─────────────────────────────────────────────────────────────────────────")
w("reg plic_priority_t {")
w('    desc = "Interrupt source priority. Hardwired to 1 — no priority levels '
  'are implemented.";')
w("    default hw = r;")
w("    default sw = r;")
w("    field {")
w('        desc = "Priority level (always 1)";')
w("        reset = 1'h1;")
w("    } PRIORITY[0:0] = 1'h1;")
w("};")
w()

# ── 2. Pending register ────────────────────────────────────────────────────────
w("// ─────────────────────────────────────────────────────────────────────────")
w("// plic_pending_t")
w("// One bit per interrupt source, read-only.  Bit 0 is reserved (no source 0).")
w(f"// Bits {NUM_SOURCES}:1 correspond to sources 1..{NUM_SOURCES}.")
w("// Set by the gateway on the leading edge of the source signal.")
w("// Cleared atomically when a hart successfully claims the interrupt.")
w("// ─────────────────────────────────────────────────────────────────────────")
w("reg plic_pending_t {")
w('    desc = "Interrupt pending bits. Hardware-managed; read-only for software.";')
w("    field {")
w('        desc = "Reserved — source 0 does not exist";')
w("        hw = r; sw = r;")
w("        reset = 1'h0;")
w("    } RESERVED[0:0] = 1'h0;")
w("    field {")
w(f'        desc = "Pending flags for interrupt sources 1..{NUM_SOURCES}";')
w("        hw = rw; sw = r;")
w(f"        reset = {NUM_SOURCES}'h0;")
w(f"    }} PENDING[{NUM_SOURCES}:1] = {NUM_SOURCES}'h0;")
w("};")
w()

# ── 3. Enable register ─────────────────────────────────────────────────────────
w("// ─────────────────────────────────────────────────────────────────────────")
w("// plic_enable_t  (one instance per context)")
w("// Read/write by software.  Bit 0 is reserved (no source 0).")
w(f"// Bits {NUM_SOURCES}:1 individually gate each source for the associated context.")
w("// ─────────────────────────────────────────────────────────────────────────")
w("reg plic_enable_t {")
w('    desc = "Interrupt enable bits for a context. Software read/write.";')
w("    field {")
w('        desc = "Reserved — source 0 does not exist";')
w("        hw = r; sw = r;")
w("        reset = 1'h0;")
w("    } RESERVED[0:0] = 1'h0;")
w("    field {")
w(f'        desc = "Enable bits for interrupt sources 1..{NUM_SOURCES}";')
w("        hw = r; sw = rw;")
w(f"        reset = {NUM_SOURCES}'h0;")
w(f"    }} ENABLE[{NUM_SOURCES}:1] = {NUM_SOURCES}'h0;")
w("};")
w()

# ── 4. Threshold register ──────────────────────────────────────────────────────
w("// ─────────────────────────────────────────────────────────────────────────")
w("// plic_threshold_t  (one instance per context)")
w("// Read-only, hardwired to 0.")
w("// Threshold = 0 means every pending+enabled interrupt (priority >= 1)")
w("// is forwarded to the context — i.e. all interrupts pass.")
w("// ─────────────────────────────────────────────────────────────────────────")
w("reg plic_threshold_t {")
w('    desc = "Priority threshold. Hardwired to 0 — all enabled pending '
  'interrupts are forwarded to the hart.";')
w("    default hw = r;")
w("    default sw = r;")
w("    field {")
w('        desc = "Threshold (always 0)";')
w("        reset = 1'h0;")
w("    } THRESHOLD[0:0] = 1'h0;")
w("};")
w()

# ── 5. Claim / Complete register ───────────────────────────────────────────────
w("// ─────────────────────────────────────────────────────────────────────────")
w("// plic_claim_complete_t  (one instance per context)")
w("//")
w("//   READ  (claim)    Returns the ID of the highest-priority pending interrupt")
w("//                    that is enabled for this context, and atomically clears")
w("//                    its pending bit.  Returns 0 if no interrupt is pending.")
w("//")
w("//   WRITE (complete) Signals that the hart has serviced the interrupt with")
w("//                    the written ID.  The gateway may re-assert the pending")
w("//                    bit if the source is still active.")
w("//")
w(f"//   Field width : {ID_WIDTH} bits  (covers IDs 0..{2**ID_WIDTH - 1}; valid range 0..{NUM_SOURCES})")
w("// ─────────────────────────────────────────────────────────────────────────")
w("reg plic_claim_complete_t {")
w('    desc = "Claim (read) / Complete (write) register. '
  f'A read atomically claims the highest-priority pending interrupt for this '
  f'context (returns 0 if none). Writing the claimed ID signals completion.";')
w("    field {")
w(f'        desc = "Interrupt ID: 0 = no interrupt pending, '
  f'1..{NUM_SOURCES} = valid interrupt source";')
w("        hw = rw; sw = rw;")
w(f"        reset = {ID_WIDTH}'h0;")
w(f"    }} ID[{ID_WIDTH - 1}:0] = {ID_WIDTH}'h0;")
w("};")
w()
w()

# ══════════════════════════════════════════════════════════════════════════════
# Address map (relative offsets; instantiated at BASE_ADDR in the SOC wrapper)
# ══════════════════════════════════════════════════════════════════════════════
w("// =============================================================")
w("// plic_addrmap — main PLIC register map (base-relative offsets)")
w("// =============================================================")
w("addrmap PLIC_cpu {")
w('    desc = "RISC-V Platform-Level Interrupt Controller register map.";')
w()

# ── Priority registers ─────────────────────────────────────────────────────────
last_prio_offset = NUM_SOURCES * 4
section("Priority Registers", f"0x{4:06X} .. 0x{last_prio_offset:06X}")
w(f"    //   plic_priority_src01 @ 0x000004  ..  plic_priority_src{NUM_SOURCES:02d} @ 0x{last_prio_offset:06X}")
w(f"    //   {NUM_SOURCES} × 32-bit registers, RO = 1, stride = 4 bytes")
w()
for src in range(1, NUM_SOURCES + 1):
    offset = src * 4
    w(f"    plic_priority_t priority_src{src:02d} @ 0x{offset:08X};")
w()

# ── Pending register ───────────────────────────────────────────────────────────
section("Interrupt Pending Register", "0x001000")
w(f"    //   1 × 32-bit register; bits[{NUM_SOURCES}:1] valid, bit[0] reserved")
w()
w("    plic_pending_t pending @ 0x00001000;")
w()

# ── Enable registers ───────────────────────────────────────────────────────────
enable_last = 0x2000 + (NUM_CONTEXTS - 1) * 0x80
section("Interrupt Enable Registers", f"0x002000 .. 0x{enable_last:06X}  stride=0x80")
w(f"    //   {NUM_CONTEXTS} contexts × 1 × 32-bit register; bits[{NUM_SOURCES}:1] valid, bit[0] reserved")
w()
for ctx in range(NUM_CONTEXTS):
    offset = 0x002000 + ctx * 0x80
    hart   = ctx // 2
    mode   = "M" if ctx % 2 == 0 else "S"
    mode_long = "Machine" if mode == "M" else "Supervisor"
    inst   = f"enable_ctx{ctx:02d}"
    w(f"    plic_enable_t {inst} @ 0x{offset:08X};")
    w(f'    {inst}->name = "Interrupt Enable – Hart {hart}, {mode}-mode";')
    w(f'    {inst}->desc = "Interrupt enable bits for context {ctx} '
      f'(hart {hart}, {mode_long} mode). '
      f'Bit N enables source N for this context.";')
    w()
w()

# ── Per-context threshold + claim/complete ─────────────────────────────────────
ctx_last_base = 0x200000 + (NUM_CONTEXTS - 1) * 0x1000
section("Context Registers  [threshold + claim/complete]",
        f"0x200000 .. 0x{ctx_last_base + 4:06X}  stride=0x1000")
w("    //   Layout per context: +0x000 threshold (RO=0)  +0x004 claim/complete (RW)")
w()
for ctx in range(NUM_CONTEXTS):
    base = 0x200000 + ctx * 0x1000
    hart = ctx // 2
    mode = "M" if ctx % 2 == 0 else "S"
    mode_long = "Machine" if mode == "M" else "Supervisor"
    thr_inst = f"threshold_ctx{ctx:02d}"
    cc_inst  = f"claim_complete_ctx{ctx:02d}"
    w(f"    // Context {ctx:2d}  (hart {hart:2d}, {mode}-mode)")
    w(f"    plic_threshold_t      {thr_inst}     @ 0x{base:08X};")
    w(f'    {thr_inst}->name = "Priority Threshold – Hart {hart}, {mode}-mode";')
    w(f'    {thr_inst}->desc = "Priority threshold for context {ctx} '
      f'(hart {hart}, {mode_long} mode). Hardwired to 0 — all enabled pending '
      f'interrupts are forwarded to this hart.";')
    w(f"    plic_claim_complete_t {cc_inst} @ 0x{base + 4:08X};")
    w(f'    {cc_inst}->name = "Claim/Complete – Hart {hart}, {mode}-mode";')
    w(f'    {cc_inst}->desc = "Claim/Complete for context {ctx} '
      f'(hart {hart}, {mode_long} mode). '
      f'Read to claim the highest-priority pending interrupt (returns 0 if none); '
      f'write the claimed ID to signal completion.";')
    w()

w("};")
w()
w()


# ── Write output ───────────────────────────────────────────────────────────────
content = "\n".join(_lines) + "\n"
with open(OUTPUT_FILE, "w") as fh:
    fh.write(content)

total_regs = NUM_SOURCES + 1 + NUM_CONTEXTS + NUM_CONTEXTS * 2

print(f"✓ Generated '{OUTPUT_FILE}'")
print(f"  Lines              : {len(_lines)}")
print(f"  Bytes              : {len(content)}")
print()
print(f"  Register instances :")
print(f"    Priority         : {NUM_SOURCES}  (sources 1..{NUM_SOURCES})")
print(f"    Pending          :  1")
print(f"    Enable           : {NUM_CONTEXTS}  (one per context)")
print(f"    Threshold        : {NUM_CONTEXTS}  (one per context)")
print(f"    Claim/Complete   : {NUM_CONTEXTS}  (one per context)")
print(f"    ─────────────────────────────")
print(f"    Total            : {total_regs}")
