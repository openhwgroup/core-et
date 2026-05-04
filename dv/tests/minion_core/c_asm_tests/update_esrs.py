#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re
from pathlib import Path

# ------------------------------------------------------------
#  Address translation logic
# ------------------------------------------------------------

def should_translate(addr: int) -> bool:
    """
    Translate only if the highest set bit is bit 32 (0-based).
    That means: addr.bit_length() - 1 == 32
    """
    if addr == 0:
        return False
    return addr.bit_length() - 1 == 32


def translate_address(addr: int) -> int:
    """
    Translation rules:

    - Output bit 31 = 1
    - Output bits 23:22 = input bits 31:30
    - Output bits 21:0 = input bits 21:0
    - SID (input bits 29:22) is discarded
    """
    if not should_translate(addr):
        return addr

    # Extract bits 21:0 (REST)
    rest = addr & ((1 << 22) - 1)   # mask = 0x3FFFFF

    # Extract PP from bits 31:30
    pp = (addr >> 30) & 0x3

    # Build output
    out = 0
    out |= (1 << 31)        # set bit 31
    out |= (pp << 22)       # place PP into bits 23:22
    out |= rest             # keep REST

    return out


# ------------------------------------------------------------
#  Number parsing
# ------------------------------------------------------------

def parse_number(raw: str) -> int:
    """
    Parse hex or decimal safely.
    Leading-zero decimals like '09' must NOT be parsed with base=0.
    """
    if raw.lower().startswith("0x"):
        return int(raw, 16)
    return int(raw, 10)


# ------------------------------------------------------------
#  Recursive file processing
# ------------------------------------------------------------

def process_files(root_dir="."):
    # Match hex (0x...) or decimal numbers
    number_pattern = re.compile(r"0x[0-9A-Fa-f]+|\d+")

    for path in Path(root_dir).rglob("*"):
        if not path.is_file():
            continue

        try:
            text = path.read_text(errors="ignore")
        except Exception:
            continue

        def repl(match):
            raw = match.group(0)

            # Only transform hex literals; decimals are always left unchanged
            if not raw.lower().startswith("0x"):
                return raw

            addr = parse_number(raw)
            new_addr = translate_address(addr)

            if new_addr == addr:
                return raw  # unchanged

            return f"0x{new_addr:X}"

        new_text = number_pattern.sub(repl, text)

        if new_text != text:
            path.write_text(new_text)
            print(f"Updated {path}")


# ------------------------------------------------------------
#  Main
# ------------------------------------------------------------

if __name__ == "__main__":
    process_files(".")
