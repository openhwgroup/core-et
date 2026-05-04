#!/eng/dvtools/release/python/dv_py3/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# Utility to convert an elf file to ZeBu hex files to preload SP RAM (inc. parity) or ROM
#
# The 1MB of SP RAM is organized into 4 contiguous 256KB blocks
# Each 256KB block is organized into 4 panels that store 18 bytes each
# (16 byte of data + 2 bytes of parity)
# A 64-byte cache line is spread across 4 panels: bytes 0-15 in 0, 16-31 in 1, etc.
# addr[3:0] byte in panel
# addr[5:4] panel
# addr[17:6] line
# addr[19:18] block
#
# The 64BK of ROM is organized into one contiguous block
# A 64-byte cache line is spread across 8 panels: bytes 0-7 in panel 0, 8-15 in 1, etc.
# addr[2:0] byte in panel
# addr[4:2] panel
#
# DRAM is spread across 16 memory controllers in 8 memory shires
# Each memory controller deals with 64-byte cache line sized transactions
# addr[5:0] byte in panel
# addr[8:6] memshire
# addr[9] controller (0=even, 1= odd)

import sys, os
from elftools.elf.elffile import ELFFile

if len(sys.argv) < 3:
    print("Usage:", sys.argv[0], "ROM|SP_RAM|PU_RAM|DRAM infile [outfile]")
    sys.exit(-1)

try:
    inFile = open(sys.argv[2], 'rb')
    elfFile = ELFFile(inFile)
except:
    print("Could not open", sys.argv[2])
    sys.exit(-1)

outFiles = []

def open_output_files(prefix, suffix, files):
    if len(sys.argv) >= 3:
        (name,ext) = os.path.splitext(sys.argv[3]) # Use outfile arg for output filename
    else:
        (name,ext) = os.path.splitext(sys.argv[2]) # Use infile arg for output filename

    try:
        for i in range(files):
            outFiles.append(open("%s_%s%d%s.hex" % (name, prefix, i, suffix), 'w'))
    except:
        print("Could not open output files")
        sys.exit(-1)

    return

# graphics.stanford.edu/~seander/bithacks.html#ParityParallel
def even_parity(x):
    if not 0 <= x <= 255:
        print("illegal byte value", x)
        sys.exit(-1)
    x ^= x >> 4
    x &= 0xf
    return (0x6996 >> x) & 1

# Given a list of 8 bytes, calculates a parity bit for each byte
# and packs the 8 parity bits into a new byte where:
# lsb = parity of bytes[0]
# msb = parity of bytes[7]
def calc_parity_byte(x):
    if not len(x) == 8:
        print("illegal list length", x)
        sys.exit(-1)
    parityByte = 0
    for i in range(8):
        if even_parity(x[i]) : parityByte |= (1 << i)
    return parityByte

def write_seg(baseAddress, addr, size, inputBytesPerPanel, outputBytesPerPanel, panelsPerLine, parity):
    inputBytesPerLine = inputBytesPerPanel * panelsPerLine

    # For each output row, set all panels including parity even if input data isn't available
    for offset in range(0, size, inputBytesPerLine):
        for panel in range(panelsPerLine):
            # ZeBu style address. Address 0 is at base of memory, address 1 is the next line, etc.
            wl = "@%010x " % ((addr + offset - baseAddress) // inputBytesPerLine)

            # For SP RAM, bytes 0-15 for panel 0, 16-31 for panel 1, etc.
            # For SP ROM, bytes 0-7 for panel 0, 8-15 for panel 1, etc.
            # If we've run out of input data, substitute 0.
            outputBytes = [seg.data()[offset + x] if (offset + x) < size else 0 for x in range(panel*inputBytesPerPanel, (panel+1)*inputBytesPerPanel)]

            if parity:
                outputBytes.append(calc_parity_byte(outputBytes[0:8])) # parity for lower 8 bytes
                outputBytes.append(calc_parity_byte(outputBytes[8:16])) # parity for upper 8 bytes

            # Append outputBytes in reverse order, parity first then MSByte to LSByte
            for byte in reversed(outputBytes):
                wl += ("%02x" % byte)

            wl += "\n"
            outFiles[panel].write(wl)
    return

def write_seg_mem(baseAddress, addr, size, inputBytesPerPanel, outputBytesPerPanel, panelsPerLine, parity):
    inputBytesPerLine = inputBytesPerPanel * panelsPerLine

    # For each output row, set all panels including parity even if input data isn't available
    for offset in range(0, size, inputBytesPerLine):
        for panel in range(panelsPerLine):
            # ZeBu style address. Address 0 is at base of memory, address 1 is the next line, etc.
            wl = "@%010x " % (addr + (offset // inputBytesPerLine))

            # For SP RAM, bytes 0-15 for panel 0, 16-31 for panel 1, etc.
            # For SP ROM, bytes 0-7 for panel 0, 8-15 for panel 1, etc.
            # If we've run out of input data, substitute 0.
            outputBytes = [seg.data()[offset + x] if (offset + x) < size else 0 for x in range(panel*inputBytesPerPanel, (panel+1)*inputBytesPerPanel)]

            if parity:
                outputBytes.append(calc_parity_byte(outputBytes[0:8])) # parity for lower 8 bytes
                outputBytes.append(calc_parity_byte(outputBytes[8:16])) # parity for upper 8 bytes

            # Append outputBytes in reverse order, parity first then MSByte to LSByte
            for byte in reversed(outputBytes):
                wl += ("%02x" % byte)

            wl += "\n"
            outFiles[panel].write(wl)
    return
    
def sp_rom_write_seg(addr, size):
    open_output_files("ROM", "", 8)
    write_seg(0x40000000, addr, size, 8, 8, 8, False)
    return

def sp_ram_write_seg(addr, size):
    open_output_files("SP_RAM", "", 4)
    write_seg(0x40400000, addr, size, 16, 18, 4, True)
    return

def sp_mem_write_seg(addr, size):
    open_output_files("MEM", "", 1)
    write_seg_mem(0x40000000, addr, size, 16, 18, 1, True)
    return
    
def dram_write_seg(addr, size):
    open_output_files("DRAM_W_", "_even", 4) # memshire 0-3 "west 0-3" addr[9] = 0 "even"
    open_output_files("DRAM_E_", "_even", 4) # memshire 4-7 "east 0-3" addr[9] = 0 "even"
    open_output_files("DRAM_W_", "_odd", 4)  # memshire 0-3 "west 0-3" addr[9] = 1 "odd"
    open_output_files("DRAM_E_", "_odd", 4)  # memshire 4-7 "east 0-3" addr[9] = 1 "odd"
    write_seg(0x8000000000, addr, size, 64, 64, 16, False)
    return

for seg in elfFile.iter_segments():
    addr = seg['p_paddr']
    size = seg['p_filesz']
    print("Segment start: %x size: %x" % (addr, size))

    
    if (sys.argv[1]) == "ROM":
        sp_rom_write_seg(addr, size)
    elif (sys.argv[1]) == "SP_RAM":
        sp_ram_write_seg(addr, size)
    elif (sys.argv[1]) == "PU_RAM":
        print("PU_RAM support not yet implemented")
        sys.exit(-1)
    elif (sys.argv[1]) == "DRAM":
        dram_write_seg(addr, size)
    elif (sys.argv[1]) == "MEM":
        sp_mem_write_seg(addr, size)
    else:
        print("Unsupported argument", sys.argv[1])
        sys.exit(-1)

