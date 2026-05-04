#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import sys

if len(sys.argv) != 4:
    print("Usage: {:s} <output_hex_file> <input_binary_file> <address>".format(sys.argv[0]))
    sys.exit(-1)

try:
    address = int(sys.argv[3],0)
except:
    print("Invalid address argument '{:s}'!".format(sys.argv[3]))
    sys.exit(-1)

try:
    binary_file = open(sys.argv[2], "rb")
except:
    print("Unable to open binary file '{:s}' for reading!".format(sys.argv[2]))
    sys.exit(-1)

try:
    binary_data = binary_file.read()
except:
    print("Error reading binary file!")
    sys.exit(-1)

try:
    hex_file = open(sys.argv[1], "w")
    print("Opened hex file '{:s}' for writing!".format(sys.argv[1]))
except:
    print("Unable to open hex file '{:s}' for writing!".format(sys.argv[1]))
    sys.exit(-1)

print("@{0:08x}".format(address), file = hex_file)
for offset in range(0, len(binary_data), 4):
    print("{0:02x}{1:02x}{2:02x}{3:02x}".format(binary_data[offset+3], binary_data[offset+2], binary_data[offset+1], binary_data[offset+0]), file = hex_file)

hex_file.close()

print("Converted binary file '{:s}' to hex file '{:s}".format(sys.argv[2], sys.argv[1]))

sys.exit(0)
