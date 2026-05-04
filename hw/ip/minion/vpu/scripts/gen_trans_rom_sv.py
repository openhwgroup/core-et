# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""Generate original-equivalent trans ROM SystemVerilog modules from packed hex.

This generator intentionally preserves the original module granularity from the
CORE-ET `trans_unit` tree. Each input `*.packed.hex` file becomes one output
module whose name is derived from the file stem, for example:

  trans_rcp_rom_case_c0.packed.hex -> trans_rcp_rom_case_c0.sv

The packed word format is the canonical output produced by
`extract_trans_rom.py`:

    {exc, mask[6:0], taylor, c2[16:0], c0[31:0]}

The generated modules are combinational and expose the same logical boundary as
the original ROM helper modules, but with repo-native port naming and package
types.
"""

from __future__ import annotations

import argparse
import math
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path


PACKED_WIDTH = 58
REPO_ROOT = Path(__file__).resolve().parents[5]
MODULE_STEM_RE = re.compile(r"^(?P<name>.+?)\.packed$")
HEX_DIGIT_RE = re.compile(r"^[0-9a-fA-F]+$")


@dataclass(frozen=True)
class RomImage:
    module_name: str
    addr_width: int
    words: list[int]


def infer_module_name(path: Path) -> str:
    stem_match = MODULE_STEM_RE.fullmatch(path.stem)
    if stem_match is not None:
        return stem_match.group("name")
    return path.stem


def infer_addr_width(depth: int) -> int:
    if depth <= 0 or (depth & (depth - 1)) != 0:
        raise ValueError(f"ROM depth must be a power of two, found {depth}")
    return int(math.log2(depth))


def load_hex_image(path: Path, module_name: str | None, addr_width: int | None) -> RomImage:
    lines = [line.strip() for line in path.read_text(encoding="utf-8").splitlines() if line.strip()]
    if not lines:
        raise ValueError(f"{path}: empty ROM image")

    words: list[int] = []
    for lineno, line in enumerate(lines, start=1):
        if not HEX_DIGIT_RE.fullmatch(line):
            raise ValueError(f"{path}:{lineno}: non-hex ROM line: {line}")
        words.append(int(line, 16))

    inferred_addr_width = infer_addr_width(len(words))
    resolved_addr_width = inferred_addr_width if addr_width is None else addr_width
    if resolved_addr_width != inferred_addr_width:
        expected_depth = 1 << resolved_addr_width
        raise ValueError(
            f"{path}: explicit addr width {resolved_addr_width} implies depth {expected_depth}, "
            f"but image has {len(words)} entries"
        )

    max_word = max(words)
    if max_word >= (1 << PACKED_WIDTH):
        raise ValueError(f"{path}: word exceeds packed width {PACKED_WIDTH}")

    resolved_name = infer_module_name(path) if module_name is None else module_name
    return RomImage(module_name=resolved_name, addr_width=resolved_addr_width, words=words)


def emit_module(rom: RomImage, source_path: Path) -> str:
    hex_width = (PACKED_WIDTH + 3) // 4
    try:
        display_source = source_path.resolve().relative_to(REPO_ROOT)
    except ValueError:
        display_source = source_path.resolve()
    case_lines = []
    for index, word in enumerate(rom.words):
        case_lines.append(
            f"      {rom.addr_width}'h{index:x}: data_packed = {PACKED_WIDTH}'h{word:0{hex_width}x};"
        )
    case_body = "\n".join(case_lines)

    return f"""// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module {rom.module_name}
  import vpu_pkg::*;
(
  /* verilator lint_off UNUSEDSIGNAL */  // The repo-native trans_rom_index_t stays 8 bits wide; smaller ROMs intentionally ignore the unused upper address bits.
  input  trans_rom_index_t         index_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output trans_fma_coefficients_t  data_o
);

  // Generated from packed coefficients extracted out of the original
  // CORE-ET trans-unit ROM helper at:
  //   {display_source}
  logic [{PACKED_WIDTH - 1}:0]      data_packed;

  always_comb begin
    unique case (index_i[{rom.addr_width - 1}:0])
{case_body}
      default: data_packed = {PACKED_WIDTH}'h0;
    endcase

    data_o = '0;
    data_o.exc    = data_packed[57];
    data_o.mask   = data_packed[56:50];
    data_o.taylor = data_packed[49];
    data_o.c2     = data_packed[48:32];
    data_o.c0     = data_packed[31:0];
  end

endmodule
"""


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Generate original-equivalent trans ROM modules from packed hex dumps."
    )
    parser.add_argument(
        "--hex",
        action="append",
        type=Path,
        help="Packed hex file(s) to convert. May be passed multiple times.",
    )
    parser.add_argument(
        "--hex-dir",
        type=Path,
        help="Directory containing *.packed.hex images to convert.",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        required=True,
        help="Directory for generated SystemVerilog files.",
    )
    parser.add_argument(
        "--module-name",
        help="Override the emitted module name. Only valid with a single --hex input.",
    )
    parser.add_argument(
        "--addr-width",
        type=int,
        help="Override the ROM address width. Only valid with a single --hex input.",
    )
    return parser


def collect_inputs(args: argparse.Namespace) -> list[Path]:
    paths: list[Path] = []
    if args.hex is not None:
        paths.extend(args.hex)
    if args.hex_dir is not None:
        paths.extend(sorted(args.hex_dir.glob("*.packed.hex")))
    deduped = []
    seen: set[Path] = set()
    for path in paths:
        resolved = path.resolve()
        if resolved not in seen:
            deduped.append(path)
            seen.add(resolved)
    return deduped


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    inputs = collect_inputs(args)
    if not inputs:
        parser.error("at least one --hex or --hex-dir input is required")
    if (args.module_name is not None or args.addr_width is not None) and len(inputs) != 1:
        parser.error("--module-name and --addr-width require exactly one input image")

    args.out_dir.mkdir(parents=True, exist_ok=True)

    generated = []
    for hex_path in inputs:
        rom = load_hex_image(hex_path, args.module_name, args.addr_width)
        out_path = args.out_dir / f"{rom.module_name}.sv"
        out_path.write_text(emit_module(rom, hex_path.resolve()), encoding="utf-8")
        generated.append((rom.module_name, out_path))

    print(f"Generated {len(generated)} trans ROM module(s) into {args.out_dir}")
    for module_name, out_path in generated:
        print(f"  - {module_name}: {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
