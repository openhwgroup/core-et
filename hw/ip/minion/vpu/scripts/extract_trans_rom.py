# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""Extract original VPU transcendental ROM contents from Verilog case tables.

This script reconstructs the original `trans_*_rom_case_*.v` contents from the
CORE-ET tree and emits both:

1. raw `rom_data` words, exactly as written in the original case tables
2. packed `trans_fma_coefficients` words, matching the original module outputs

The packed word layout is the original packed-struct order from
`trans_types.vh`:

    {exc, mask[6:0], taylor, c2[16:0], c0[31:0]}

Only the ROM-local outputs are recreated here. In the original ROM modules the
`exc` and `taylor` fields remain zero because each module starts with `data = '0`
and only overwrites `mask`, `c2`, and `c0`.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Callable


REPO_ROOT = Path(__file__).resolve().parents[5]
DEFAULT_OUT_DIR = REPO_ROOT / "build" / "trans_rom_extract"

TRANS_ROM_DIR = Path("rtl/shire/minion/vpu/trans_unit")
TRANS_DEFINES = Path("rtl/inc/trans_defines.vh")
TRANS_TYPES = Path("rtl/inc/trans_types.vh")

CASE_RE = re.compile(
    r"^\s*(default|[0-9]+\'[hdb][0-9a-fA-F_xXzZ?]+)\s*:\s*"
    r"rom_data\s*=\s*([0-9]+\'[hdb][0-9a-fA-F_xXzZ?]+)\s*;\s*$"
)
ROM_WIDTH_RE = re.compile(r"logic\s*\[\s*(\d+)\s*:\s*0\s*\]\s*rom_data\s*;")
DEFINE_RE = re.compile(r"^\s*`define\s+(\w+)\s+(.+?)\s*$")


@dataclass(frozen=True)
class PackedCoefficients:
    exc: int
    mask: int
    taylor: int
    c2: int
    c0: int

    def pack(self) -> int:
        value = 0
        value = (value << 1) | self.exc
        value = (value << 7) | self.mask
        value = (value << 1) | self.taylor
        value = (value << 17) | self.c2
        value = (value << 32) | self.c0
        return value


@dataclass(frozen=True)
class ModuleSpec:
    name: str
    addr_size_macro: str
    raw_width: int
    packer: Callable[[dict[str, int], int, int], PackedCoefficients]


def bits(value: int, msb: int, lsb: int) -> int:
    width = msb - lsb + 1
    return (value >> lsb) & ((1 << width) - 1)


def concat(parts: list[tuple[int, int]], width: int) -> int:
    value = 0
    for part_value, part_width in parts:
        value = (value << part_width) | (part_value & ((1 << part_width) - 1))
    return value & ((1 << width) - 1)


def parse_sv_int(token: str) -> tuple[int, int]:
    match = re.fullmatch(r"(\d+)\'([hdb])([0-9a-fA-F_xXzZ?]+)", token)
    if not match:
        raise ValueError(f"unsupported SystemVerilog literal: {token}")
    width = int(match.group(1))
    base = match.group(2)
    digits = match.group(3).replace("_", "").lower().replace("x", "0").replace("z", "0").replace("?", "0")
    if base == "h":
        value = int(digits, 16)
    elif base == "d":
        value = int(digits, 10)
    elif base == "b":
        value = int(digits, 2)
    else:
        raise ValueError(f"unsupported literal base in {token}")
    return width, value


def parse_numeric_defines(path: Path, wanted: set[str]) -> dict[str, int]:
    values: dict[str, int] = {}
    remaining = set(wanted)
    raw_exprs: dict[str, str] = {}
    for line in path.read_text(encoding="utf-8").splitlines():
        match = DEFINE_RE.match(line.split("//", 1)[0])
        if not match:
            continue
        name, expr = match.groups()
        if name in remaining:
            raw_exprs[name] = expr.strip()

    unresolved = True
    while unresolved:
        unresolved = False
        for name in list(remaining):
            expr = raw_exprs.get(name)
            if expr is None:
                continue
            range_match = re.fullmatch(r"(\d+)\s*:\s*(\d+)", expr)
            if range_match:
                msb = int(range_match.group(1))
                lsb = int(range_match.group(2))
                values[name] = (msb << 8) | lsb
                remaining.remove(name)
                unresolved = True
                continue

            rewritten = expr
            for other_name, other_value in values.items():
                rewritten = rewritten.replace(f"`{other_name}", str(other_value))
            if "`" in rewritten:
                continue
            if not re.fullmatch(r"[0-9()\s+\-*/]+", rewritten):
                continue
            values[name] = int(eval(rewritten, {"__builtins__": {}}, {}))
            remaining.remove(name)
            unresolved = True

    if remaining:
        missing = ", ".join(sorted(remaining))
        raise ValueError(f"failed to parse required defines from {path}: {missing}")
    return values


def unpack_range(encoded_range: int) -> tuple[int, int]:
    return (encoded_range >> 8, encoded_range & 0xFF)


def load_module_words(module_path: Path, addr_count: int, raw_width: int) -> list[int]:
    text = module_path.read_text(encoding="utf-8")
    width_match = ROM_WIDTH_RE.search(text)
    if width_match is None:
        raise ValueError(f"failed to find rom_data width in {module_path}")
    file_raw_width = int(width_match.group(1)) + 1
    if file_raw_width != raw_width:
        raise ValueError(
            f"{module_path.name}: expected rom_data width {raw_width}, found {file_raw_width}"
        )

    words: dict[int, int] = {}
    default_word: int | None = None
    for line in text.splitlines():
        match = CASE_RE.match(line)
        if not match:
            continue
        label, literal = match.groups()
        literal_width, literal_value = parse_sv_int(literal)
        if literal_width != raw_width:
            raise ValueError(
                f"{module_path.name}: expected literal width {raw_width}, found {literal_width}"
            )
        if label == "default":
            default_word = literal_value
        else:
            _, addr = parse_sv_int(label)
            if addr >= addr_count:
                raise ValueError(
                    f"{module_path.name}: address {addr:#x} exceeds depth {addr_count}"
                )
            words[addr] = literal_value

    if default_word is None:
        raise ValueError(f"{module_path.name}: missing default rom_data assignment")

    return [words.get(addr, default_word) for addr in range(addr_count)]


def pack_rcp_c0(_: dict[str, int], __: int, raw: int) -> PackedCoefficients:
    return PackedCoefficients(
        exc=0,
        mask=0,
        taylor=0,
        c2=0,
        c0=concat([(0, 6), (1, 1), (raw, 25)], 32),
    )


def pack_rcp_c1c2(consts: dict[str, int], _: int, raw: int) -> PackedCoefficients:
    rcp_c2_msb, rcp_c2_lsb = unpack_range(consts["TRANS_RCP_C2_RANGE"])
    rcp_c1_msb, rcp_c1_lsb = unpack_range(consts["TRANS_RCP_C1_RANGE"])
    return PackedCoefficients(
        exc=0,
        mask=0,
        taylor=0,
        c2=concat([(bits(raw, rcp_c2_msb, rcp_c2_lsb), rcp_c2_msb - rcp_c2_lsb + 1), (0, 7)], 17),
        c0=concat([(0x7, 3), (bits(raw, rcp_c1_msb, rcp_c1_lsb), rcp_c1_msb - rcp_c1_lsb + 1), (0, 5), (0, 8)], 32),
    )


def pack_rsqrt_c0(_: dict[str, int], index: int, raw: int) -> PackedCoefficients:
    if index & 0x80:
        c0 = concat([(0, 5), (1, 1), (raw, 25), (0, 1)], 32)
    else:
        c0 = concat([(0, 5), (1, 1), (0, 1), (raw, 25)], 32)
    return PackedCoefficients(exc=0, mask=0, taylor=0, c2=0, c0=c0)


def pack_rsqrt_c1c2(consts: dict[str, int], _: int, raw: int) -> PackedCoefficients:
    rsqrt_c2_msb, rsqrt_c2_lsb = unpack_range(consts["TRANS_RSQRT_C2_RANGE"])
    rsqrt_c1_msb, rsqrt_c1_lsb = unpack_range(consts["TRANS_RSQRT_C1_RANGE"])
    return PackedCoefficients(
        exc=0,
        mask=0,
        taylor=0,
        c2=concat([(bits(raw, rsqrt_c2_msb, rsqrt_c2_lsb), rsqrt_c2_msb - rsqrt_c2_lsb + 1), (0, 8)], 17),
        c0=concat([(0x7, 3), (bits(raw, rsqrt_c1_msb, rsqrt_c1_lsb), rsqrt_c1_msb - rsqrt_c1_lsb + 1), (0, 6), (0, 8)], 32),
    )


def pack_log_c0(_: dict[str, int], __: int, raw: int) -> PackedCoefficients:
    return PackedCoefficients(exc=0, mask=1, taylor=0, c2=0, c0=concat([(0, 5), (raw, 26), (0, 1)], 32))


def pack_log_c1c2(consts: dict[str, int], _: int, raw: int) -> PackedCoefficients:
    log_c2_msb, log_c2_lsb = unpack_range(consts["TRANS_LOG_C2_RANGE"])
    log_c1_msb, log_c1_lsb = unpack_range(consts["TRANS_LOG_C1_RANGE"])
    return PackedCoefficients(
        exc=0,
        mask=1,
        taylor=0,
        c2=concat([(bits(raw, log_c2_msb, log_c2_lsb), log_c2_msb - log_c2_lsb + 1), (0, 7)], 17),
        c0=concat([(0x3, 2), (bits(raw, log_c1_msb, log_c1_lsb), log_c1_msb - log_c1_lsb + 1), (0, 6), (0, 8)], 32),
    )


def pack_exp_c0(_: dict[str, int], __: int, raw: int) -> PackedCoefficients:
    return PackedCoefficients(exc=0, mask=0x3, taylor=0, c2=0, c0=concat([(0, 3), (raw, 26), (0, 3)], 32))


def pack_exp_c1c2(consts: dict[str, int], _: int, raw: int) -> PackedCoefficients:
    exp_c2_msb, exp_c2_lsb = unpack_range(consts["TRANS_EXP_C2_RANGE"])
    exp_c1_msb, exp_c1_lsb = unpack_range(consts["TRANS_EXP_C1_RANGE"])
    return PackedCoefficients(
        exc=0,
        mask=0x3,
        taylor=0,
        c2=concat([(bits(raw, exp_c2_msb, exp_c2_lsb), exp_c2_msb - exp_c2_lsb + 1), (0, 6)], 17),
        c0=concat([(0, 2), (bits(raw, exp_c1_msb, exp_c1_lsb), exp_c1_msb - exp_c1_lsb + 1), (0, 6), (0, 8)], 32),
    )


def pack_sin_c0(_: dict[str, int], index: int, raw: int) -> PackedCoefficients:
    if index & 0x20:
        c0 = concat([(0, 5), (raw, 27)], 32)
        mask = 0x3
    elif index & 0x10:
        c0 = concat([(0, 4), (raw, 27), (0, 1)], 32)
        mask = 0x7
    elif index & 0x08:
        c0 = concat([(0, 3), (raw, 27), (0, 2)], 32)
        mask = 0xF
    elif index & 0x02:
        c0 = concat([(0, 1), (raw, 27), (0, 4)], 32)
        mask = 0x3F
    elif index == 0:
        c0 = concat([(raw, 27), (0, 5)], 32)
        mask = 0x7F
    else:
        c0 = 0
        mask = 0
    return PackedCoefficients(exc=0, mask=mask, taylor=0, c2=0, c0=c0)


def pack_sin_c1c2(consts: dict[str, int], index: int, raw: int) -> PackedCoefficients:
    sin_c2_msb, sin_c2_lsb = unpack_range(consts["TRANS_SIN_C2_RANGE"])
    sin_c1_msb, sin_c1_lsb = unpack_range(consts["TRANS_SIN_C1_RANGE"])
    c2_bits = bits(raw, sin_c2_msb, sin_c2_lsb)
    c1_bits = bits(raw, sin_c1_msb, sin_c1_lsb)

    if index & 0x20:
        c2 = concat([(c2_bits, 15), (0, 2)], 17)
        c0 = concat([(0, 4), (c1_bits, 20), (0, 8)], 32)
        mask = 0x3
    elif index & 0x10:
        c2 = concat([(0, 1), (c2_bits, 15), (0, 1)], 17)
        c0 = concat([(0, 4), (c1_bits, 20), (0, 8)], 32)
        mask = 0x7
    elif index & 0x08:
        c2 = concat([(0, 2), (c2_bits, 15)], 17)
        c0 = concat([(0, 4), (c1_bits, 20), (0, 8)], 32)
        mask = 0xF
    elif index & 0x02:
        c2 = concat([(0, 1), (c2_bits, 15), (0, 1)], 17)
        c0 = concat([(0, 1), (c1_bits, 20), (0, 3), (0, 8)], 32)
        mask = 0x3F
    elif index == 0:
        c2 = concat([(0, 2), (c2_bits, 15)], 17)
        c0 = concat([(0, 1), (c1_bits, 20), (0, 3), (0, 8)], 32)
        mask = 0x7F
    else:
        c2 = 0
        c0 = 0
        mask = 0

    return PackedCoefficients(exc=0, mask=mask, taylor=0, c2=c2, c0=c0)


MODULE_SPECS = [
    ModuleSpec("trans_rcp_rom_case_c0", "TRANS_RCP_ADDR_SIZE", 25, pack_rcp_c0),
    ModuleSpec("trans_rcp_rom_case_c1c2", "TRANS_RCP_ADDR_SIZE", 26, pack_rcp_c1c2),
    ModuleSpec("trans_rsqrt_rom_case_c0", "TRANS_RSQRT_ADDR_SIZE", 25, pack_rsqrt_c0),
    ModuleSpec("trans_rsqrt_rom_case_c1c2", "TRANS_RSQRT_ADDR_SIZE", 24, pack_rsqrt_c1c2),
    ModuleSpec("trans_log_rom_case_c0", "TRANS_LOG_ADDR_SIZE", 26, pack_log_c0),
    ModuleSpec("trans_log_rom_case_c1c2", "TRANS_LOG_ADDR_SIZE", 26, pack_log_c1c2),
    ModuleSpec("trans_exp_rom_case_c0", "TRANS_EXP_ADDR_SIZE", 26, pack_exp_c0),
    ModuleSpec("trans_exp_rom_case_c1c2", "TRANS_EXP_ADDR_SIZE", 29, pack_exp_c1c2),
    ModuleSpec("trans_sin_rom_case_c0", "TRANS_SIN_ADDR_SIZE", 27, pack_sin_c0),
    ModuleSpec("trans_sin_rom_case_c1c2", "TRANS_SIN_ADDR_SIZE", 35, pack_sin_c1c2),
]
MODULE_SPECS_BY_NAME = {spec.name: spec for spec in MODULE_SPECS}


def hex_width(bits: int) -> int:
    return (bits + 3) // 4


def format_words_hex(words: list[int], width_bits: int) -> str:
    width_nibbles = hex_width(width_bits)
    return "\n".join(f"{word:0{width_nibbles}x}" for word in words) + "\n"


def format_words_bin(words: list[int], width_bits: int) -> str:
    return "\n".join(f"{word:0{width_bits}b}" for word in words) + "\n"


def write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Recreate original VPU transcendental ROM contents from Verilog case tables."
    )
    parser.add_argument(
        "--orig-root",
        type=Path,
        default=Path(os.environ["ORIG_ROOT"]) if "ORIG_ROOT" in os.environ else None,
        help="Path to the original CORE-ET tree. Defaults to $ORIG_ROOT if set.",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=DEFAULT_OUT_DIR,
        help=f"Output directory for extracted ROM dumps (default: {DEFAULT_OUT_DIR}).",
    )
    parser.add_argument(
        "--module",
        action="append",
        choices=sorted(MODULE_SPECS_BY_NAME.keys()),
        help="Specific ROM module(s) to extract. Default: all trans ROM modules.",
    )
    parser.add_argument(
        "--view",
        choices=["raw", "packed", "all"],
        default="all",
        help="Emit raw rom_data words, packed coefficient words, or both.",
    )
    parser.add_argument(
        "--format",
        choices=["hex", "bin", "json", "all"],
        default="all",
        help="Emit hex, binary, JSON, or all formats.",
    )
    return parser


def main() -> int:
    parser = build_arg_parser()
    args = parser.parse_args()

    if args.orig_root is None:
        parser.error("--orig-root is required unless ORIG_ROOT is set")

    orig_root = args.orig_root.expanduser().resolve()
    trans_defines_path = orig_root / TRANS_DEFINES
    trans_types_path = orig_root / TRANS_TYPES
    trans_rom_root = orig_root / TRANS_ROM_DIR

    for needed_path in (trans_defines_path, trans_types_path, trans_rom_root):
        if not needed_path.exists():
            parser.error(f"missing required original path: {needed_path}")

    wanted_defines = {
        "TRANS_RCP_ADDR_SIZE",
        "TRANS_RCP_C2_RANGE",
        "TRANS_RCP_C1_RANGE",
        "TRANS_RSQRT_ADDR_SIZE",
        "TRANS_RSQRT_C2_RANGE",
        "TRANS_RSQRT_C1_RANGE",
        "TRANS_LOG_ADDR_SIZE",
        "TRANS_LOG_C2_RANGE",
        "TRANS_LOG_C1_RANGE",
        "TRANS_EXP_ADDR_SIZE",
        "TRANS_EXP_C2_RANGE",
        "TRANS_EXP_C1_RANGE",
        "TRANS_SIN_ADDR_SIZE",
        "TRANS_SIN_C2_RANGE",
        "TRANS_SIN_C1_RANGE",
    }
    consts = parse_numeric_defines(trans_defines_path, wanted_defines)

    selected = MODULE_SPECS if args.module is None else [MODULE_SPECS_BY_NAME[name] for name in args.module]
    want_raw = args.view in ("raw", "all")
    want_packed = args.view in ("packed", "all")
    formats = ["hex", "bin", "json"] if args.format == "all" else [args.format]

    manifest: dict[str, object] = {
        "packed_layout": {
            "fields_msb_to_lsb": ["exc", "mask", "taylor", "c2", "c0"],
            "widths": {"exc": 1, "mask": 7, "taylor": 1, "c2": 17, "c0": 32},
            "total_bits": 58,
        },
        "modules": {},
    }

    for spec in selected:
        module_path = trans_rom_root / f"{spec.name}.v"
        addr_width = consts[spec.addr_size_macro]
        addr_count = 1 << addr_width
        raw_words = load_module_words(module_path, addr_count, spec.raw_width)
        packed_words = [spec.packer(consts, index, raw).pack() for index, raw in enumerate(raw_words)]

        module_json = {
            "module": spec.name,
            "source": str(module_path),
            "addr_width": addr_width,
            "depth": addr_count,
            "raw_width": spec.raw_width,
            "packed_width": 58,
            "entries": [],
        }
        for index, (raw_word, packed_word) in enumerate(zip(raw_words, packed_words)):
            coeff = spec.packer(consts, index, raw_word)
            module_json["entries"].append(
                {
                    "address": index,
                    "raw": f"0x{raw_word:0{hex_width(spec.raw_width)}x}",
                    "fields": {
                        "exc": coeff.exc,
                        "mask": f"0x{coeff.mask:02x}",
                        "taylor": coeff.taylor,
                        "c2": f"0x{coeff.c2:05x}",
                        "c0": f"0x{coeff.c0:08x}",
                    },
                    "packed": f"0x{packed_word:0{hex_width(58)}x}",
                }
            )
        manifest["modules"][spec.name] = module_json

        if want_raw and "hex" in formats:
            write_text(args.out_dir / f"{spec.name}.raw.hex", format_words_hex(raw_words, spec.raw_width))
        if want_raw and "bin" in formats:
            write_text(args.out_dir / f"{spec.name}.raw.bin", format_words_bin(raw_words, spec.raw_width))
        if want_packed and "hex" in formats:
            write_text(args.out_dir / f"{spec.name}.packed.hex", format_words_hex(packed_words, 58))
        if want_packed and "bin" in formats:
            write_text(args.out_dir / f"{spec.name}.packed.bin", format_words_bin(packed_words, 58))
        if "json" in formats:
            write_text(
                args.out_dir / f"{spec.name}.json",
                json.dumps(module_json, indent=2) + "\n",
            )

    if "json" in formats:
        write_text(args.out_dir / "manifest.json", json.dumps(manifest, indent=2) + "\n")

    print(f"Extracted {len(selected)} trans ROM module(s) into {args.out_dir}")
    for spec in selected:
        print(f"  - {spec.name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
