#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import argparse

def splitmix64(x: int) -> int:
    x = (x + 0x9E3779B97F4A7C15) & 0xFFFFFFFFFFFFFFFF
    x = ((x ^ (x >> 30)) * 0xBF58476D1CE4E5B9) & 0xFFFFFFFFFFFFFFFF
    x = ((x ^ (x >> 27)) * 0x94D049BB133111EB) & 0xFFFFFFFFFFFFFFFF
    x = (x ^ (x >> 31)) & 0xFFFFFFFFFFFFFFFF
    return x

def undo_xor_shift_right(x: int, shift: int) -> int:
    i = shift
    while i < 64:
        x ^= x >> i
        i *= 2
    return x & 0xFFFFFFFFFFFFFFFF

def splitmix64_inverse(y: int) -> int:
    y = undo_xor_shift_right(y, 31)
    y = (y * 0x319642B2D24D8EC3) & 0xFFFFFFFFFFFFFFFF
    y = undo_xor_shift_right(y, 27)
    y = (y * 0x96DE1B173F119089) & 0xFFFFFFFFFFFFFFFF
    y = undo_xor_shift_right(y, 30)
    y = (y - 0x9E3779B97F4A7C15) & 0xFFFFFFFFFFFFFFFF
    return y

def parse_args():
    parser = argparse.ArgumentParser(description='Get hashed value based on MRAM address or MRAM address based on the value')
    parser.add_argument('-a', '--address', type=lambda s: int(s, 0))
    parser.add_argument('-v', '--value', type=lambda s: int(s, 0))
    return parser.parse_args()

def main(args):
    if args.address:
        val = splitmix64(args.address)
        print(f"Value: {hex(val)} ({val})")
    else:
        addr = splitmix64_inverse(args.value)
        print(f"Address: {hex(addr)} ({addr})")

if __name__ == "__main__":
    args = parse_args()
    if not(args.address or args.value):
        raise ValueError("-a or -v is required")
    main(args)
