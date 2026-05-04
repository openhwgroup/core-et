#!/usr/bin/env python3
"""
XSR (ESR/CSR) SystemVerilog register generator.

ESR = ET System Registers (APB-accessed peripheral registers)
CSR = Control/Status Registers (RISC-V CSR instruction-accessed)

Reads register definitions from CSV (either ESR or CSR format) and generates
clean SystemVerilog register blocks following Ainekko conventions:
  - Active-low async reset
  - prim_clk_gate for latch clock gating (replacing et_clk_gate)
  - No macros, no `include, no `define
  - lowRISC naming (_i/_o, _d/_q)

The script auto-detects the CSV format from the header row:
  - ESR format: Mod, PP, Address, Name, Access, ...
  - CSR format: Address, Name, Privilege, Access, ...

Both formats share the same generation engine (mirroring the original
XsrFile/XsrRegister base class that handles both ESR and CSR).

Usage:
  # ESR mode (APB register block):
  python3 gen_esr.py --csv esr.csv --mod rbox --out rbox_esr.sv

  # CSR mode (register sections for include):
  # --out supplies the section-file basename; the generator emits
  # intpipe_csr_file_auto_<section>.svh files, not a monolithic include.
  python3 gen_esr.py --csv csr.csv --out intpipe_csr_file_auto.svh

  # CSR types package:
  python3 gen_esr.py --csv csr.csv --types-dir csr_types/ --pkg-out intpipe_csr_pkg.sv
"""

import argparse
import csv
import sys
import os
import re
from dataclasses import dataclass, field
from typing import List, Optional, Dict, Tuple


# ── Constants ──────────────────────────────────────────────────────────

NR_THREADS = 2  # CORE_NR_THREADS


# ── Type definition parsing (for CSR typed registers) ─────────────────

@dataclass
class TypeField:
    name: str
    width: int
    is_reserved: bool
    is_ro: bool
    ro_value: str


@dataclass
class XsrType:
    name: str
    fields: List[TypeField]
    total_bits: int

    def has_reserved(self) -> bool:
        return any(f.is_reserved for f in self.fields)


def parse_type_csv(csv_path: str) -> Optional[XsrType]:
    """Parse a type CSV file."""
    type_name = os.path.basename(csv_path).replace('.csv', '')
    with open(csv_path, newline='', encoding='utf-8') as f:
        rows = list(csv.reader(f))
    if len(rows) < 2:
        return None

    field_names = [c.strip() for c in rows[0][1:]]
    widths_raw = [c.strip() for c in rows[1][1:]]
    ro_flags = [c.strip().upper() for c in rows[2][1:]] if len(rows) > 2 else []
    ro_values = [c.strip() for c in rows[3][1:]] if len(rows) > 3 else []

    fields = []
    for i, name in enumerate(field_names):
        if not name:
            continue
        try:
            width = int(widths_raw[i]) if i < len(widths_raw) and widths_raw[i] else 1
        except ValueError:
            width = 1
        uname = name.upper()
        is_reserved = (uname in ('WPRI', 'WIRI', 'ZERO') or
                       'reserved' in name.lower() or name.endswith('-ZERO'))
        is_ro = (i < len(ro_flags) and ro_flags[i] == 'YES')
        ro_value = ro_values[i] if i < len(ro_values) else ''
        sv_name = name.lower().replace(' ', '_').replace('-', '_')
        if is_reserved:
            sv_name = f'reserved{i}' if 'reserved' not in sv_name else sv_name
        fields.append(TypeField(name=sv_name, width=width,
                                is_reserved=is_reserved, is_ro=is_ro,
                                ro_value=ro_value))

    return XsrType(name=type_name, fields=fields,
                   total_bits=sum(f.width for f in fields))


def load_all_types(types_dir: str) -> Dict[str, XsrType]:
    """Load all type CSVs from a directory."""
    types = {}
    if not types_dir or not os.path.isdir(types_dir):
        return types
    for fname in sorted(os.listdir(types_dir)):
        if fname.endswith('.csv'):
            t = parse_type_csv(os.path.join(types_dir, fname))
            if t:
                types[t.name] = t
    return types


# ── Unified register model ────────────────────────────────────────────

@dataclass
class XsrRegister:
    """One register parsed from CSV. Works for both ESR and CSR formats."""
    # Common fields (present in both ESR and CSR)
    address: int
    name: str
    access: str          # RW, RO, RW-HWIRED, RO-HWIRED
    description: str
    reg_type: str        # custom type name, 'MANUAL', or empty
    width: str           # bit width as string
    rdata: str
    wdata: str
    shadow: str
    reset_val: str
    reset: str           # GLOBAL, DEBUG, NON-DEBUG
    trigger_cnt: str
    latch: bool
    implemented: bool
    ifdef: str
    comment: str

    # ESR-specific
    mod: str             # module filter name
    pp: int              # privilege protection bits
    connect_to_port: bool

    # CSR-specific
    privilege: str       # user, supervisor, machine
    shared: bool         # True = one instance; False = per-thread
    sign_ext_rdata: bool
    aon: bool            # always-on clock domain
    trap_if: str

    # Metadata
    row_num: int
    format: str          # 'esr' or 'csr'

    def writeable(self) -> bool:
        return self.access == 'RW'

    def is_manual(self) -> bool:
        return self.reg_type == 'MANUAL'

    def is_typed(self) -> bool:
        return bool(self.reg_type) and self.reg_type != 'MANUAL'

    def is_hwired(self) -> bool:
        return 'HWIRED' in self.access

    def is_ro(self) -> bool:
        return self.access.startswith('RO')

    def port_width(self) -> int:
        if not self.width:
            return 64
        try:
            return int(self.width)
        except ValueError:
            return 64

    def data_range(self) -> str:
        w = self.port_width()
        return '' if w == 1 else f'[{w-1}:0]'

    def short_name(self) -> str:
        """Strip module prefix for ESR port naming."""
        prefix = self.mod + '_'
        return self.name[len(prefix):] if self.name.startswith(prefix) else self.name


# ── CSV parsing (auto-detects ESR vs CSR from header) ─────────────────

def parse_bool(val: str) -> bool:
    return val.strip().upper() == 'YES' if val else False


def parse_csv(csv_path: str, mod_filter: str = None) -> Tuple[List[XsrRegister], str]:
    """Parse CSV file. Returns (registers, format) where format is 'esr' or 'csr'."""
    regs = []
    with open(csv_path, newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        header = [h.strip() for h in next(reader)]

    # Auto-detect format
    if header[0] == 'Mod':
        fmt = 'esr'
    elif header[0] == 'Address':
        fmt = 'csr'
    else:
        print(f'Error: unrecognized CSV header: {header[0]}', file=sys.stderr)
        sys.exit(1)

    # Build column index map
    col = {h: i for i, h in enumerate(header)}

    def get(row, field, default=''):
        i = col.get(field)
        if i is None or i >= len(row):
            return default
        return row[i].strip() if row[i] else default

    with open(csv_path, newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader)  # skip header

        for row_num, row in enumerate(reader, start=2):
            # Skip short rows / section headers
            addr_field = 'Address'
            addr_str = get(row, addr_field)
            name = get(row, 'Name')
            if not addr_str or not name:
                continue
            try:
                address = int(addr_str, 16)
            except ValueError:
                continue

            # ESR filter by Mod
            mod = get(row, 'Mod', '')
            if fmt == 'esr' and mod_filter and mod != mod_filter:
                continue

            # Parse common fields
            access = get(row, 'Access', 'RW').upper()
            impl_str = get(row, 'implemented', 'YES')
            implemented = impl_str.upper() != 'NO' if impl_str else True
            if not implemented:
                continue

            reg = XsrRegister(
                address=address,
                name=name,
                access=access,
                description=get(row, 'Description'),
                reg_type=get(row, 'Type'),
                width=get(row, 'width'),
                rdata=get(row, 'rdata'),
                wdata=get(row, 'wdata'),
                shadow=get(row, 'shadow'),
                reset_val=get(row, 'resetVal'),
                reset=get(row, 'reset', 'NON-DEBUG'),
                trigger_cnt=get(row, 'triggerCnt'),
                latch=parse_bool(get(row, 'latch')),
                implemented=implemented,
                ifdef=get(row, 'ifdef'),
                comment=get(row, 'comment'),
                # ESR-specific
                mod=mod,
                pp=int(get(row, 'PP', '0') or '0'),
                connect_to_port=parse_bool(get(row, 'connect_to_port')),
                # CSR-specific
                privilege=get(row, 'Privilege', '').lower(),
                shared=parse_bool(get(row, 'shared')) if fmt == 'csr' else False,
                sign_ext_rdata=parse_bool(get(row, 'sign_ext_rdata')),
                aon=parse_bool(get(row, 'aon')),
                trap_if=get(row, 'trap_if'),
                # Metadata
                row_num=row_num,
                format=fmt,
            )
            regs.append(reg)

    return regs, fmt


# ══════════════════════════════════════════════════════════════════════
#  ESR output generation (APB register blocks) — existing functionality
# ══════════════════════════════════════════════════════════════════════

def generate_esr_sv(mod: str, regs: List[XsrRegister], addr_width: int = 16,
                    reset_name: str = 'rst_ni') -> str:
    """Generate SystemVerilog APB register block (ESR mode)."""

    port_regs = [r for r in regs if r.connect_to_port]
    rw_regs = [r for r in port_regs if r.writeable()]
    ro_regs = [r for r in port_regs if not r.writeable()]
    max_addr = max(r.address for r in regs) if regs else 0

    lines = []
    def emit(s=''):
        lines.append(s)

    # Header
    emit(f'// Copyright (c) 2026 Ainekko')
    emit(f'// SPDX-License-Identifier: Apache-2.0')
    emit(f'//')
    emit(f'// ET System Register block for {mod.upper()}.')
    emit(f'//')
    emit(f'// Auto-generated from esr.csv by gen_esr.py.')
    emit(f'// Do not edit manually.')
    emit(f'//')
    emit(f'// Registers:')
    for r in port_regs:
        rw = "RW" if r.writeable() else "RO"
        emit(f'//   0x{r.address:03x}  {r.short_name():30s}  {rw}  {r.port_width()}-bit  {r.description}')
    emit(f'//')
    emit(f'// APB protocol: 2-phase (setup + access).')
    emit(f'// Write enable pulses (_en_o) are asserted for one cycle after a write.')
    emit()

    emit(f'module {mod}_esr')
    emit(f'  import apb_pkg::*;')
    emit(f'(')
    emit(f'  input  logic       clk_i,')
    emit(f'  input  logic       {reset_name},')
    emit(f'')
    emit(f'  // APB interface')
    emit(f'  input  apb_pkg::req_t  apb_req_i,')
    emit(f'  output apb_pkg::rsp_t  apb_rsp_o,')

    if rw_regs or ro_regs:
        emit(f'')
        emit(f'  // Register ports')
    for r in rw_regs:
        emit(f'  output logic {r.data_range():10s} {r.short_name()}_o,')
        emit(f'  output logic            {r.short_name()}_en_o,')
    for r in ro_regs:
        emit(f'  input  logic {r.data_range():10s} {r.short_name()}_i,')

    for i in range(len(lines) - 1, -1, -1):
        if lines[i].rstrip().endswith(','):
            lines[i] = lines[i].rstrip()[:-1]
            break

    emit(f');')
    emit()

    # Address constants
    emit(f'  // ── Address constants ────────────────────────────────')
    for r in port_regs:
        cname = f'Addr{r.short_name().replace("_", " ").title().replace(" ", "")}'
        emit(f'  localparam logic [{addr_width-1}:0] {cname} = {addr_width}\'h{r.address:x};')
    emit()

    # Register storage
    emit(f'  // ── Register storage ─────────────────────────────────')
    for r in rw_regs:
        emit(f'  logic {r.data_range():10s} reg_{r.short_name()}_q;')
    emit()

    # Write enable delay
    emit(f'  // ── Write enable pulses (delayed one cycle) ──────────')
    emit(f'  // Internal enables (gate register writes):')
    for r in rw_regs:
        emit(f'  logic en_{r.short_name()}_d;')
    emit(f'  // Output enables (wdata-gated, match original CUSTOM STATEMENTS):')
    for r in rw_regs:
        emit(f'  logic out_en_{r.short_name()}_d;')
        emit(f'  logic out_en_{r.short_name()}_q;')
    emit()

    # APB decode signals
    emit(f'  // ── APB decode ───────────────────────────────────────')
    emit(f'  logic s0_setup;')
    emit(f'  logic s1_access;')
    emit(f'  logic [{addr_width-1}:0] s0_addr;')
    emit(f'  logic s0_write;')
    emit(f'  logic s0_addr_valid;')
    data_width = max((r.port_width() for r in port_regs), default=64)
    emit(f'  logic [{data_width - 1}:0] s0_rdata;')
    emit()

    # Combinational decode
    emit(f'  always_comb begin')
    emit(f'    s0_setup = apb_req_i.psel && !apb_req_i.penable;')
    emit(f'    s1_access = apb_req_i.psel && apb_req_i.penable;')
    emit(f'    s0_addr = apb_req_i.paddr;')
    emit(f'    s0_write = apb_req_i.pwrite;')
    emit(f'    s0_addr_valid = 1\'b0;')
    emit(f'    s0_rdata = \'0;')
    emit(f'')
    emit(f'    if (s0_setup) begin')
    emit(f'      unique case (s0_addr)')
    for r in port_regs:
        cname = f'Addr{r.short_name().replace("_", " ").title().replace(" ", "")}'
        if r.writeable():
            emit(f'        {cname}: begin s0_addr_valid = 1\'b1; s0_rdata = {data_width}\'(reg_{r.short_name()}_q); end')
        else:
            emit(f'        {cname}: begin s0_addr_valid = 1\'b1; s0_rdata = {data_width}\'({r.short_name()}_i); end')
    emit(f'        default: s0_addr_valid = 1\'b0;')
    emit(f'      endcase')
    emit(f'    end')
    emit()

    for r in rw_regs:
        cname = f'Addr{r.short_name().replace("_", " ").title().replace(" ", "")}'
        emit(f'    en_{r.short_name()}_d = s1_access && s0_write && (s0_addr == {cname});')
    emit()
    for r in rw_regs:
        cname = f'Addr{r.short_name().replace("_", " ").title().replace(" ", "")}'
        emit(f'    out_en_{r.short_name()}_d = s1_access && (|s1_wdata_q) && (s0_addr == {cname});')
    emit(f'  end')
    emit()

    # Pipeline registers
    emit(f'  // ── Pipeline registers ───────────────────────────────')
    emit(f'  logic [{data_width-1}:0] s1_rdata_q;')
    emit(f'  logic s1_addr_valid_q;')
    emit(f'  logic [{data_width-1}:0] s1_wdata_q;')
    emit()
    emit(f'  always_ff @(posedge clk_i or negedge {reset_name}) begin')
    emit(f'    if (!{reset_name})')
    emit(f'      s1_addr_valid_q <= 1\'b0;')
    emit(f'    else if (s0_setup)')
    emit(f'      s1_addr_valid_q <= s0_addr_valid;')
    emit(f'  end')
    emit()
    emit(f'  always_ff @(posedge clk_i) begin')
    emit(f'    if (s0_setup && !s0_write)')
    emit(f'      s1_rdata_q <= s0_rdata;')
    emit(f'    if (s0_setup && s0_write)')
    emit(f'      s1_wdata_q <= apb_req_i.pwdata;')
    emit(f'  end')
    emit()

    # APB outputs
    emit(f'  // ── APB outputs ──────────────────────────────────────')
    emit(f'  assign apb_rsp_o.prdata  = s1_rdata_q;')
    emit(f'  assign apb_rsp_o.pslverr = !s1_addr_valid_q;')
    emit(f'  assign apb_rsp_o.pready  = 1\'b1;')
    emit()

    # Register write + reset
    emit(f'  // ── Register write + reset ───────────────────────────')
    emit(f'  always_ff @(posedge clk_i or negedge {reset_name}) begin')
    emit(f'    if (!{reset_name}) begin')
    for r in rw_regs:
        emit(f'      reg_{r.short_name()}_q <= {r.port_width()}\'({r.reset_val});')
    for r in rw_regs:
        emit(f'      out_en_{r.short_name()}_q <= 1\'b0;')
    emit(f'    end else begin')
    for r in rw_regs:
        emit(f'      out_en_{r.short_name()}_q <= out_en_{r.short_name()}_d;')
    for r in rw_regs:
        w = r.port_width()
        if w < data_width:
            emit(f'      if (en_{r.short_name()}_d) reg_{r.short_name()}_q <= s1_wdata_q[{w-1}:0];')
        else:
            emit(f'      if (en_{r.short_name()}_d) reg_{r.short_name()}_q <= s1_wdata_q;')
    emit(f'    end')
    emit(f'  end')
    emit()

    # Output assignments
    emit(f'  // ── Output assignments ───────────────────────────────')
    for r in rw_regs:
        emit(f'  assign {r.short_name()}_o    = reg_{r.short_name()}_q;')
        emit(f'  assign {r.short_name()}_en_o = out_en_{r.short_name()}_q;')
    emit()

    emit(f'endmodule')
    return '\n'.join(lines) + '\n'


# ══════════════════════════════════════════════════════════════════════
#  CSR output generation (register sections for intpipe_csr_file)
# ══════════════════════════════════════════════════════════════════════

# Mapping from Verilog `defines to SystemVerilog package parameters.
# Used to replace width/resetVal references in generated CSR code.
DEFINE_TO_PARAM = {
    '`CSR_NR_COUNTERS':   'CsrNrCounters',
    '`CSR_NR_EVENTS_L':   'CsrNrEventsL',
    '`VA_SIZE':            'VaSize',
    '`VA_SIZE_EXT':        'VaSizeExt',
    '`PA_SIZE':            'PaSize',
    '`PC_SIZE_EXT':        'PcSizeExt',
    '`MSTATUS_RESET_VAL':  'MstatusResetVal',
    '`DCSR_RESET_VAL':     'DcsrResetVal',
    '`TDATA1_RESET_VAL':   'Tdata1ResetVal',
    '`CSR_UCACHE_CTRL_SZ': 'CsrUcacheCtrlSz',
    '`CSR_MCACHE_CTRL_SZ': 'CsrMcacheCtrlSz',
    '`AMOFENCE_CTRL_RESET_VAL': 'AmofenceCtrlResetVal',
    '`CSR_TRIGGER_INFO':   'CsrTriggerInfo',
    '`CSR_VENDOR_ID':      'CsrVendorId',
    '`CSR_ARCH_ID':        'CsrArchId',
    '`CSR_IMP_ID':         'CsrImpId',
    '`CSR_ISA_MAX':        'CsrIsaMax',
}


def map_defines(s: str) -> str:
    """Replace Verilog `defines with package parameters.
    Sort by length (longest first) to avoid partial matches."""
    for old, new in sorted(DEFINE_TO_PARAM.items(), key=lambda x: -len(x[0])):
        s = s.replace(old, new)
    return s


def ifdef_wrap(lines: List[str], ifdef: str) -> List[str]:
    if not ifdef:
        return lines
    return [f'`ifdef {ifdef}'] + lines + [f'`endif']


def gen_csr_declarations(regs: List[XsrRegister]) -> str:
    """DECLARATIONS section."""
    lines = []
    for r in regs:
        if r.is_hwired() or (r.is_ro() and not r.writeable()):
            continue

        dl = []
        is_dec_only = r.is_manual() or (r.shadow and r.writeable())

        if is_dec_only:
            if r.shared:
                dl.append(f'  logic reg_{r.name}_en, reg_{r.name}_en_next;')
            else:
                dl.append(f'  logic [{NR_THREADS}-1:0] reg_{r.name}_en, reg_{r.name}_en_next;')
        elif r.is_typed():
            if r.shared:
                dl.append(f'  {r.reg_type} reg_{r.name}, reg_{r.name}_pre, reg_{r.name}_next;')
                dl.append(f'  logic reg_{r.name}_en, reg_{r.name}_en_next;')
            else:
                dl.append(f'  {r.reg_type} [{NR_THREADS}-1:0] reg_{r.name}, reg_{r.name}_pre, reg_{r.name}_next;')
                dl.append(f'  logic [{NR_THREADS}-1:0] reg_{r.name}_en, reg_{r.name}_en_next;')
            if r.latch:
                if r.shared:
                    dl.append(f'  logic reg_{r.name}_latch_clk_en;')
                else:
                    dl.append(f'  logic [{NR_THREADS}-1:0] reg_{r.name}_latch_clk_en;')
        else:
            w = r.width if r.width else '64'
            if r.shared:
                dl.append(f'  logic [{w}-1:0] reg_{r.name}, reg_{r.name}_next;')
                dl.append(f'  logic reg_{r.name}_en, reg_{r.name}_en_next;')
            else:
                dl.append(f'  logic [{NR_THREADS}-1:0][{w}-1:0] reg_{r.name}, reg_{r.name}_next;')
                dl.append(f'  logic [{NR_THREADS}-1:0] reg_{r.name}_en, reg_{r.name}_en_next;')
            if r.latch:
                if r.shared:
                    dl.append(f'  logic reg_{r.name}_latch_clk_en;')
                else:
                    dl.append(f'  logic [{NR_THREADS}-1:0] reg_{r.name}_latch_clk_en;')

        lines.extend(ifdef_wrap(dl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_decode_defaults(regs: List[XsrRegister]) -> str:
    """READ_AND_DECODE_COMB_DEFAULT section."""
    lines = []
    for r in regs:
        if r.is_hwired() or (r.is_ro() and not r.writeable()):
            continue
        dl = []
        if r.shared:
            dl.append(f'      reg_{r.name}_en_next = 1\'b0;')
        else:
            for i in range(NR_THREADS):
                dl.append(f'      reg_{r.name}_en_next[{i}] = 1\'b0;')
        lines.extend(ifdef_wrap(dl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_decode_cases(regs: List[XsrRegister], reg_map: Dict[str, XsrRegister]) -> str:
    """READ_AND_DECODE_COMB_CASES section."""
    lines = []

    def build_read_expr(r: XsrRegister) -> str:
        """Build the CSR readback expression with per-thread indexing and 64-bit shaping."""
        if r.rdata:
            raw_expr = r.rdata
            is_constant = bool(re.match(r"^\d+'[hbdo]", raw_expr))
            if not r.shared and not is_constant and '[mem_thread_id]' not in raw_expr:
                raw_expr = f'{raw_expr}[mem_thread_id]'
        elif r.shadow:
            dest = reg_map.get(r.shadow)
            if dest and not dest.shared:
                raw_expr = f'reg_{r.shadow}[mem_thread_id]'
            else:
                raw_expr = f'reg_{r.shadow}'
            if (r.is_typed() and dest and dest.is_typed() and
                    r.reg_type != dest.reg_type):
                d_short = dest.reg_type.removeprefix('csr_').removesuffix('_t')
                s_short = r.reg_type.removeprefix('csr_').removesuffix('_t')
                raw_expr = f'read_{d_short}_as_{s_short}({raw_expr})'
        elif r.shared:
            raw_expr = f'reg_{r.name}'
        else:
            raw_expr = f'reg_{r.name}[mem_thread_id]'

        if r.sign_ext_rdata:
            return f"64'($signed({raw_expr}))"
        return f"64'({raw_expr})"

    for r in regs:
        ad = f'csr_ad_{r.name.upper()}'
        trap = r.trap_if
        cl = []

        if r.is_hwired():
            # Hardwired register — constant read, no write effect
            rval = r.rdata if r.rdata else '64\'h0'
            cl.append(f'        {ad}: begin')
            cl.append(f'          mem_addr_valid = 1\'b1;')
            cl.append(f'          io_rw_wb_rdata_next = {rval};')
            cl.append(f'        end')

        elif r.is_ro():
            # Read-only
            read_expr = build_read_expr(r)
            cl.append(f'        {ad}: begin')
            if trap:
                cl.append(f'          mem_addr_valid = !{trap};')
                cl.append(f'          mem_feature_xcpt = {trap};')
            else:
                cl.append(f'          mem_addr_valid = 1\'b1;')
            cl.append(f'          io_rw_wb_rdata_next = {read_expr};')
            cl.append(f'        end')

        elif r.writeable():
            # Read-write
            read_expr = build_read_expr(r)

            cl.append(f'        {ad}: begin')
            if trap:
                cl.append(f'          mem_addr_valid = !{trap};')
                if r.shared:
                    cl.append(f'          reg_{r.name}_en_next = mem_wen && !{trap};')
                else:
                    cl.append(f'          reg_{r.name}_en_next = {{mem_wen && mem_thread_id, mem_wen && !mem_thread_id}} & {{{NR_THREADS}{{!{trap}}}}};')
                cl.append(f'          mem_feature_xcpt = {trap};')
            else:
                cl.append(f'          mem_addr_valid = 1\'b1;')
                if r.shared:
                    cl.append(f'          reg_{r.name}_en_next = mem_wen;')
                else:
                    cl.append(f'          reg_{r.name}_en_next = {{mem_wen && mem_thread_id, mem_wen && !mem_thread_id}};')
            cl.append(f'          io_rw_wb_rdata_next = {read_expr};')
            cl.append(f'        end')

        lines.extend(ifdef_wrap(cl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_decode_seq(regs: List[XsrRegister]) -> str:
    """READ_AND_DECODE_SEQ section."""
    lines = []
    for r in regs:
        if r.is_hwired() or (r.is_ro() and not r.writeable()):
            continue
        sl = []
        if r.shared:
            sl.append(f'  always_ff @(posedge clock_dec) begin')
            sl.append(f'    if (reset_c) reg_{r.name}_en <= \'0;')
            sl.append(f'    else if (reg_{r.name}_en ^ reg_{r.name}_en_next)')
            sl.append(f'      reg_{r.name}_en <= reg_{r.name}_en_next;')
            sl.append(f'  end')
        else:
            sl.append(f'  always_ff @(posedge clock_dec) begin')
            sl.append(f'    if (reset_c) reg_{r.name}_en <= \'0;')
            sl.append(f'    else if (reg_{r.name}_en != reg_{r.name}_en_next)')
            sl.append(f'      reg_{r.name}_en <= reg_{r.name}_en_next;')
            sl.append(f'  end')
        lines.extend(ifdef_wrap(sl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_write_defaults(regs: List[XsrRegister]) -> str:
    """WRITE_COMB_DEFAULT section."""
    lines = []
    for r in regs:
        if not r.writeable() or r.is_hwired() or r.is_manual() or r.shadow:
            continue
        dl = []
        if r.latch:
            dl.append(f'      reg_{r.name}_latch_clk_en = \'0;')
        else:
            dl.append(f'      reg_{r.name}_next = reg_{r.name};')
        lines.extend(ifdef_wrap(dl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_write_comb(regs: List[XsrRegister], reg_map: Dict[str, XsrRegister],
                       types: Dict[str, XsrType] = {}) -> str:
    """WRITE_CORE_COMB section."""
    lines = []

    # Shadows first — use merge function to update only shadow fields in dest
    for r in regs:
        if not r.writeable() or r.is_hwired() or r.is_manual() or not r.shadow:
            continue
        dest = reg_map.get(r.shadow)
        if not dest:
            continue

        # Determine write data for the shadow register
        if r.wdata:
            raw_wdata = r.wdata
        elif r.is_typed():
            t = types.get(r.reg_type)
            if t and t.has_reserved():
                short = r.reg_type.removeprefix('csr_').removesuffix('_t')
                raw_wdata = f"write_{short}(wb_wdata[$bits({r.reg_type})-1:0])"
            else:
                raw_wdata = f"wb_wdata[$bits({r.reg_type})-1:0]"
        else:
            raw_wdata = 'wb_wdata'

        # Determine merge expression — typed shadow uses write_DEST_as_SHADOW()
        d_short = dest.reg_type.removeprefix('csr_').removesuffix('_t') if dest.is_typed() else ''
        s_short = r.reg_type.removeprefix('csr_').removesuffix('_t') if r.is_typed() else ''
        use_merge = (dest.is_typed() and r.is_typed() and dest.reg_type != r.reg_type
                     and d_short and s_short)

        wl = []
        if r.shared:
            if use_merge:
                wl.append(f'      if (reg_{r.name}_en) reg_{r.shadow}_next = write_{d_short}_as_{s_short}(reg_{r.shadow}_next, {raw_wdata});')
            else:
                wl.append(f'      if (reg_{r.name}_en) reg_{r.shadow}_next = {raw_wdata};')
        else:
            for i in range(NR_THREADS):
                if use_merge:
                    wl.append(f'      if (reg_{r.name}_en[{i}]) reg_{r.shadow}_next[{i}] = write_{d_short}_as_{s_short}(reg_{r.shadow}_next[{i}], {raw_wdata});')
                else:
                    wl.append(f'      if (reg_{r.name}_en[{i}]) reg_{r.shadow}_next[{i}] = {raw_wdata};')
        if dest.latch:
            wl.append(f'      reg_{r.shadow}_latch_clk_en |= reg_{r.name}_en;')
        lines.extend(ifdef_wrap(wl, r.ifdef))

    # Then regular writes
    for r in regs:
        if not r.writeable() or r.is_hwired() or r.is_manual() or r.shadow:
            continue
        if r.wdata:
            wdata = r.wdata
        elif r.is_typed():
            base = 'wb_wdata_1p' if r.latch else 'wb_wdata'
            t = types.get(r.reg_type)
            if t and t.has_reserved():
                # Type has reserved fields: use write_TYPE() conversion
                short = r.reg_type.removeprefix('csr_').removesuffix('_t')
                wdata = f"write_{short}({base}[$bits({r.reg_type})-1:0])"
            else:
                # Generic type: simple cast
                wdata = f"{r.reg_type}'({base}[$bits({r.reg_type})-1:0])"
        elif r.width:
            w = map_defines(r.width) if r.width.startswith('`') else r.width
            base = 'wb_wdata_1p' if r.latch else 'wb_wdata'
            wdata = f'{base}[{w}-1:0]'
        else:
            wdata = 'wb_wdata_1p' if r.latch else 'wb_wdata'
        wl = []
        if r.latch:
            if r.shared:
                wl.append(f'      reg_{r.name}_next = {wdata};')
                wl.append(f'      reg_{r.name}_latch_clk_en |= reg_{r.name}_en;')
            else:
                for i in range(NR_THREADS):
                    wl.append(f'      reg_{r.name}_next[{i}] = {wdata};')
                wl.append(f'      reg_{r.name}_latch_clk_en |= reg_{r.name}_en;')
        else:
            if r.shared:
                wl.append(f'      if (reg_{r.name}_en) reg_{r.name}_next = {wdata};')
            else:
                for i in range(NR_THREADS):
                    wl.append(f'      if (reg_{r.name}_en[{i}]) reg_{r.name}_next[{i}] = {wdata};')
        lines.extend(ifdef_wrap(wl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_write_seq(regs: List[XsrRegister], reg_map: Dict[str, XsrRegister]) -> str:
    """WRITE_SEQ section."""
    lines = []
    for r in regs:
        if not r.writeable() or r.is_hwired() or r.is_manual() or r.shadow:
            continue
        clk = 'clock' if r.aon else 'clock_wb'
        rst_map = {'GLOBAL': 'reset_c', 'DEBUG': 'reset_d', 'NON-DEBUG': 'reset_w'}
        rst = rst_map.get(r.reset, 'reset_c')
        has_reset = r.reset_val and r.reset_val != "'x"
        dst = f'reg_{r.name}_pre' if r.is_typed() else f'reg_{r.name}'
        sl = []

        if r.latch:
            # Behavior-named write-commit primitive. Generic preserves the
            # original helper-clock write semantics directly; FPGA targets may
            # map the same write-commit role to an edge-triggered implementation.
            if r.shared:
                if has_reset:
                    sl.append(f'  prim_write_commit_rst_en #(')
                    sl.append(f'    .Width ($bits({dst})),')
                    sl.append(f'    .ResetValue ({r.reset_val})')
                    sl.append(f'  ) u_reg_{r.name} (')
                    sl.append(f'    .clk_i ({clk}),')
                    sl.append(f'    .rst_i ({rst}_reg),')
                    sl.append(f'    .en_i  (reg_{r.name}_latch_clk_en),')
                    sl.append(f'    .d_i   (reg_{r.name}_next),')
                    sl.append(f'    .q_o   ({dst})')
                    sl.append(f'  );')
                else:
                    sl.append(f'  prim_write_commit_en #(')
                    sl.append(f'    .Width ($bits({dst}))')
                    sl.append(f'  ) u_reg_{r.name} (')
                    sl.append(f'    .clk_i ({clk}),')
                    sl.append(f'    .en_i  (reg_{r.name}_latch_clk_en),')
                    sl.append(f'    .d_i   (reg_{r.name}_next),')
                    sl.append(f'    .q_o   ({dst})')
                    sl.append(f'  );')
            else:
                if has_reset:
                    for i in range(NR_THREADS):
                        sl.append(f'  prim_write_commit_rst_en #(')
                        sl.append(f'    .Width ($bits({dst}[{i}])),')
                        sl.append(f'    .ResetValue ({r.reset_val})')
                        sl.append(f'  ) u_reg_{r.name}{i} (')
                        sl.append(f'    .clk_i ({clk}),')
                        sl.append(f'    .rst_i ({rst}_reg),')
                        sl.append(f'    .en_i  (reg_{r.name}_latch_clk_en[{i}]),')
                        sl.append(f'    .d_i   (reg_{r.name}_next[{i}]),')
                        sl.append(f'    .q_o   ({dst}[{i}])')
                        sl.append(f'  );')
                else:
                    for i in range(NR_THREADS):
                        sl.append(f'  prim_write_commit_en #(')
                        sl.append(f'    .Width ($bits({dst}[{i}]))')
                        sl.append(f'  ) u_reg_{r.name}{i} (')
                        sl.append(f'    .clk_i ({clk}),')
                        sl.append(f'    .en_i  (reg_{r.name}_latch_clk_en[{i}]),')
                        sl.append(f'    .d_i   (reg_{r.name}_next[{i}]),')
                        sl.append(f'    .q_o   ({dst}[{i}])')
                        sl.append(f'  );')
        else:
            # FF
            if r.shared:
                if has_reset:
                    sl.append(f'  always_ff @(posedge {clk}) begin')
                    sl.append(f'    if ({rst}) {dst} <= {r.reset_val};')
                    sl.append(f'    else {dst} <= reg_{r.name}_next;')
                    sl.append(f'  end')
                else:
                    sl.append(f'  always_ff @(posedge {clk})')
                    sl.append(f'    {dst} <= reg_{r.name}_next;')
            else:
                sl.append(f'  always_ff @(posedge {clk}) begin')
                for i in range(NR_THREADS):
                    if has_reset:
                        sl.append(f'    if ({rst}) {dst}[{i}] <= {r.reset_val};')
                        sl.append(f'    else {dst}[{i}] <= reg_{r.name}_next[{i}];')
                    else:
                        sl.append(f'    {dst}[{i}] <= reg_{r.name}_next[{i}];')
                sl.append(f'  end')

        lines.extend(ifdef_wrap(sl, r.ifdef))
    return '\n'.join(lines)


def gen_csr_type_conversions(types: Dict[str, XsrType], regs: List[XsrRegister]) -> str:
    """TYPE_CONVERSIONS section."""
    lines = []
    used = set()
    for r in regs:
        if r.is_typed() and r.reg_type in types:
            used.add(r.reg_type)

    for tname in sorted(used):
        t = types[tname]
        short = tname.removeprefix('csr_').removesuffix('_t')

        # write_SHORT(d) — from raw bits
        lines.append(f'  function automatic {tname} write_{short};')
        lines.append(f'    input [$bits({tname})-1:0] d;')
        lines.append(f'    {tname} dd, ret;')
        lines.append(f'  begin')
        lines.append(f'    dd = {tname}\'(d);')
        for f in t.fields:
            if f.is_reserved:
                lines.append(f"    ret.{f.name} = '0;")
            elif f.is_ro and f.ro_value:
                lines.append(f"    ret.{f.name} = {f.ro_value};")
            elif f.is_ro:
                lines.append(f"    ret.{f.name} = 'x;")
            else:
                lines.append(f'    ret.{f.name} = dd.{f.name};')
        lines.append(f'    return ret;')
        lines.append(f'  end')
        lines.append(f'  endfunction')
        lines.append('')

        # write_SHORT_non_reserved(d) — from typed value
        lines.append(f'  function automatic {tname} write_{short}_non_reserved;')
        lines.append(f'    input {tname} d;')
        lines.append(f'    {tname} ret;')
        lines.append(f'  begin')
        for f in t.fields:
            if f.is_reserved:
                lines.append(f"    ret.{f.name} = '0;")
            else:
                lines.append(f'    ret.{f.name} = d.{f.name};')
        lines.append(f'    return ret;')
        lines.append(f'  end')
        lines.append(f'  endfunction')
        lines.append('')

    # ── Shadow cross-type functions ──────────────────────────────────
    # For each shadow register pair where both are typed and types differ,
    # generate: read_DEST_as_SHADOW(orig) and write_DEST_as_SHADOW(orig, d)
    reg_map = {r.name: r for r in regs}
    seen_shadow_pairs = set()
    for r in regs:
        if not r.shadow or not r.is_typed():
            continue
        dest = reg_map.get(r.shadow)
        if not dest or not dest.is_typed():
            continue
        if dest.reg_type == r.reg_type:
            continue
        pair_key = (dest.reg_type, r.reg_type)
        if pair_key in seen_shadow_pairs:
            continue
        seen_shadow_pairs.add(pair_key)

        dt = types.get(dest.reg_type)
        st = types.get(r.reg_type)
        if not dt or not st:
            continue

        d_short = dest.reg_type.removeprefix('csr_').removesuffix('_t')
        s_short = r.reg_type.removeprefix('csr_').removesuffix('_t')

        # Shared field names (non-reserved fields present in both types)
        s_field_names = {f.name for f in st.fields if not f.is_reserved}
        d_field_names = {f.name for f in dt.fields}
        common = s_field_names & d_field_names

        # read_DEST_as_SHADOW(orig) — extract shadow fields from dest
        lines.append(f'  function automatic {r.reg_type} read_{d_short}_as_{s_short};')
        lines.append(f'    input {dest.reg_type} orig;')
        lines.append(f'    {r.reg_type} ret;')
        lines.append(f'  begin')
        lines.append(f"    ret = '0;")
        for f in st.fields:
            if not f.is_reserved and f.name in d_field_names:
                lines.append(f'    ret.{f.name} = orig.{f.name};')
        lines.append(f'    return ret;')
        lines.append(f'  end')
        lines.append(f'  endfunction')
        lines.append('')

        # write_DEST_as_SHADOW(orig, d) — merge shadow fields into dest
        lines.append(f'  function automatic {dest.reg_type} write_{d_short}_as_{s_short};')
        lines.append(f'    input {dest.reg_type} orig;')
        lines.append(f'    input {r.reg_type} d;')
        lines.append(f'    {dest.reg_type} ret;')
        lines.append(f'  begin')
        lines.append(f'    ret = orig;')
        # Build set of RO field names from the DEST (parent) type
        dest_ro_fields = {f.name for f in dt.fields if f.is_ro and not f.is_reserved}
        for f in st.fields:
            if f.is_reserved:
                continue
            if f.name not in d_field_names:
                continue
            if f.name in dest_ro_fields:
                lines.append(f'    // ret.{f.name} = RO;')
            else:
                lines.append(f'    ret.{f.name} = d.{f.name};')
        lines.append(f'    return ret;')
        lines.append(f'  end')
        lines.append(f'  endfunction')
        lines.append('')

    return '\n'.join(lines)


def generate_csr_svh(regs: List[XsrRegister], types: Dict[str, XsrType]) -> Dict[str, str]:
    """Generate CSR auto include files (one per section).
    Returns dict of filename_suffix → content."""
    reg_map = {r.name: r for r in regs}
    header = '// Auto-generated by gen_esr.py (CSR mode) — do not edit manually.\n\n'

    named_sections = [
        ('declarations',    lambda: gen_csr_declarations(regs)),
        ('decode_default',  lambda: gen_csr_decode_defaults(regs)),
        ('decode_cases',    lambda: gen_csr_decode_cases(regs, reg_map)),
        ('decode_seq',      lambda: gen_csr_decode_seq(regs)),
        ('write_default',   lambda: gen_csr_write_defaults(regs)),
        ('write_comb',      lambda: gen_csr_write_comb(regs, reg_map, types)),
        ('write_seq',       lambda: gen_csr_write_seq(regs, reg_map)),
        ('type_conversions', lambda: gen_csr_type_conversions(types, regs)),
    ]

    files = {}
    for name, fn in named_sections:
        content = header + fn() + '\n'
        files[name] = map_defines(content)
    return files


def generate_csr_pkg(regs: List[XsrRegister], types: Dict[str, XsrType]) -> str:
    """Generate CSR types package."""
    lines = []
    lines.append('// Copyright (c) 2026 Ainekko')
    lines.append('// SPDX-License-Identifier: Apache-2.0')
    lines.append('//')
    lines.append('// CSR type definitions and address enum.')
    lines.append('// Auto-generated by gen_esr.py — do not edit manually.')
    lines.append('')
    lines.append('package intpipe_csr_pkg;')
    lines.append('')

    for tname in sorted(types.keys()):
        t = types[tname]
        lines.append(f'  typedef struct packed {{')
        for f in t.fields:
            if f.width == 1:
                lines.append(f'    logic {f.name};')
            else:
                lines.append(f'    logic [{f.width-1}:0] {f.name};')
        lines.append(f'  }} {t.name};')
        lines.append('')

    # Address enum (ifdef-guarded entries wrapped in their ifdef)
    lines.append('  typedef enum logic [11:0] {')
    enum_lines = []
    for r in regs:
        entry = f"    csr_ad_{r.name.upper()} = 12'h{r.address:03x}"
        if r.ifdef:
            enum_lines.append(f'`ifdef {r.ifdef}')
            enum_lines.append(entry + ',')
            enum_lines.append(f'`endif')
        else:
            enum_lines.append(entry + ',')
    # Remove trailing comma from last non-endif line
    for i in range(len(enum_lines) - 1, -1, -1):
        if enum_lines[i].rstrip().endswith(','):
            enum_lines[i] = enum_lines[i].rstrip()[:-1]
            break
    lines.extend(enum_lines)
    lines.append('  } csr_ad_t;')
    lines.append('')
    lines.append('endpackage')
    return '\n'.join(lines) + '\n'


# ══════════════════════════════════════════════════════════════════════
#  Main
# ══════════════════════════════════════════════════════════════════════

def main():
    parser = argparse.ArgumentParser(
        description='Generate ESR/CSR SystemVerilog from CSV')
    parser.add_argument('--csv', required=True, help='Path to esr.csv or csr.csv')
    parser.add_argument('--mod', default=None,
                        help='Module name filter (ESR mode, e.g. rbox)')
    parser.add_argument('--out', required=True,
                        help='Output .sv/.svh path (CSR mode uses only the basename and emits section files)')
    parser.add_argument('--addr-width', type=int, default=16,
                        help='APB address width (ESR mode, default: 16)')
    parser.add_argument('--reset-name', type=str, default='rst_ni',
                        help='Reset port name (ESR mode, default: rst_ni)')
    parser.add_argument('--types-dir', default=None,
                        help='Directory with type CSV files (CSR mode)')
    parser.add_argument('--pkg-out', default=None,
                        help='Output package .sv file (CSR mode)')
    args = parser.parse_args()

    regs, fmt = parse_csv(args.csv, args.mod)
    if not regs:
        print(f'Error: no registers found', file=sys.stderr)
        sys.exit(1)

    types = load_all_types(args.types_dir)

    print(f'Format: {fmt.upper()}, {len(regs)} registers, {len(types)} types')

    if fmt == 'esr':
        sv = generate_esr_sv(args.mod, regs, args.addr_width, args.reset_name)
        os.makedirs(os.path.dirname(args.out) or '.', exist_ok=True)
        with open(args.out, 'w') as f:
            f.write(sv)
        print(f'Generated {args.out}')
    else:
        section_files = generate_csr_svh(regs, types)
        out_dir = os.path.dirname(args.out) or '.'
        os.makedirs(out_dir, exist_ok=True)
        base = os.path.splitext(os.path.basename(args.out))[0]
        for name, content in section_files.items():
            path = os.path.join(out_dir, f'{base}_{name}.svh')
            with open(path, 'w') as f:
                f.write(content)
            print(f'Generated {path}')

    if fmt == 'csr' and args.pkg_out:
        pkg = generate_csr_pkg(regs, types)
        os.makedirs(os.path.dirname(args.pkg_out) or '.', exist_ok=True)
        with open(args.pkg_out, 'w') as f:
            f.write(pkg)
        print(f'Generated {args.pkg_out}')


if __name__ == '__main__':
    main()
