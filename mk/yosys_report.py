#!/usr/bin/env python3
"""Parse a Yosys synthesis report and print a clean resource summary."""

import sys
import re
from collections import defaultdict


def parse_module_cells(lines, top_name):
    """Extract cell counts from the requested top module statistics block."""
    cells = defaultdict(int)
    in_module = False
    skip_words = {'wires', 'wire', 'bits', 'public', 'ports', 'port',
                  'cells', 'submodules', 'memories', 'processes',
                  'connections'}
    top_names = {top_name, f'\\{top_name}'}
    for line in lines:
        line = line.rstrip()

        if line.strip().startswith('=== ') and line.strip().endswith(' ==='):
            name = line.strip()[4:-4]
            if in_module:
                break
            in_module = name in top_names
            continue

        if not in_module:
            continue

        m = re.match(r'\s+(\S+)\s+(\d+)', line)
        if m:
            name = m.group(1)
            count = int(m.group(2))
            if name in skip_words:
                continue
            if 'bit' in name or 'wire' in name or 'port' in name:
                continue
            if name.startswith('$scopeinfo'):
                continue
            cells[name] += count

    return dict(cells)


def parse_design_hierarchy_cells(lines):
    """Extract flattened design totals from the design hierarchy summary."""
    cells = defaultdict(int)
    in_hierarchy = False
    in_summary = False
    skip_words = {'wires', 'wire', 'bits', 'public', 'ports', 'port',
                  'cells', 'submodules', 'memories', 'processes',
                  'connections'}

    for line in lines:
        line = line.rstrip()

        if line.strip() == '=== design hierarchy ===':
            in_hierarchy = True
            in_summary = False
            continue

        if not in_hierarchy:
            continue

        if line.strip().startswith('=== ') and line.strip().endswith(' ==='):
            break

        if not in_summary and line.strip().startswith('Number of wires:'):
            in_summary = True

        if not in_summary:
            continue

        m = re.match(r'\s+(\S+)\s+(\d+)', line)
        if m:
            name = m.group(1)
            count = int(m.group(2))
            if name in skip_words:
                continue
            if 'bit' in name or 'wire' in name or 'port' in name:
                continue
            if name.startswith('$scopeinfo'):
                continue
            cells[name] += count

    return dict(cells)


def classify_ice40(cells):
    luts = cells.get('SB_LUT4', 0)
    carry = cells.get('SB_CARRY', 0)
    bram = cells.get('SB_RAM40_4K', 0)
    ffs = sum(v for k, v in cells.items() if k.startswith('SB_DFF'))
    return {'LUTs (SB_LUT4)': luts, 'FFs (SB_DFF*)': ffs,
            'BRAM (SB_RAM40_4K)': bram, 'Carry (SB_CARRY)': carry}


def classify_xilinx(cells):
    luts = sum(v for k, v in cells.items() if k.startswith('LUT'))
    ffs = sum(v for k, v in cells.items() if k.startswith('FD'))
    bram36 = sum(v for k, v in cells.items() if 'RAMB36' in k)
    bram18 = sum(v for k, v in cells.items() if 'RAMB18' in k)
    carry = sum(v for k, v in cells.items() if k.startswith('CARRY'))
    dsp = sum(v for k, v in cells.items() if 'DSP' in k)
    return {'LUTs (LUT1-6)': luts, 'FFs (FD*)': ffs,
            'BRAM36': bram36, 'BRAM18': bram18,
            'Carry (CARRY*)': carry, 'DSP': dsp}


def main():
    if len(sys.argv) < 3:
        print(f'Usage: {sys.argv[0]} <report.rpt> <top_module>', file=sys.stderr)
        sys.exit(1)

    rpt_path = sys.argv[1]
    top_name = sys.argv[2]

    with open(rpt_path) as f:
        lines = [line.rstrip('\n') for line in f]

    stats_start = next((i for i, line in enumerate(lines)
                        if 'Printing statistics' in line), None)
    if stats_start is None:
        print(f'No statistics found in {rpt_path}', file=sys.stderr)
        sys.exit(1)

    stats_lines = lines[stats_start + 1:]

    cells = parse_design_hierarchy_cells(stats_lines)
    if not cells:
        cells = parse_module_cells(stats_lines, top_name)
    if not cells:
        print(f'No cells found in {rpt_path}', file=sys.stderr)
        sys.exit(1)

    # Auto-detect technology
    if any(k.startswith('SB_') for k in cells):
        summary = classify_ice40(cells)
    elif any(k.startswith('LUT') or k.startswith('FD') for k in cells):
        summary = classify_xilinx(cells)
    else:
        summary = None

    print(f'=== {top_name} resource summary ===\n')

    if summary:
        max_label = max(len(k) for k in summary)
        for label, count in summary.items():
            if count > 0:
                print(f'  {label:{max_label}s}  {count:>10,}')
    print()

    # Raw breakdown (skip zero counts and Yosys internals)
    hw_cells = {k: v for k, v in cells.items()
                if v > 0 and not k.startswith('$')}
    if hw_cells:
        print('  Cell breakdown:')
        for name, count in sorted(hw_cells.items(), key=lambda x: -x[1]):
            print(f'    {name:30s} {count:>10,}')


if __name__ == '__main__':
    main()
