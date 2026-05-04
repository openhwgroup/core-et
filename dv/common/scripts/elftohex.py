#!/usr/bin/env python3.6
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
# addr[5:3] panel
#
# DRAM is spread across 16 memory controllers in 8 memory shires
# Each memory controller deals with 64-byte cache line sized transactions
# addr[5:0] byte in panel
# addr[8:6] memshire
# addr[9] controller (0=even, 1=odd)
#
# If using DDR DRAM models, additional address bit swizzling is implemented:
# Mesh  Synopsys DDR    Comment
# addr  dword addr
# bit   bit
# 0     -               Byte address bit 0
# 1     -               Byte address bit 1
# 2     -               Byte address bit 2
# 3     0               Byte address bit 3 (double-word address bit 0)
# 4     1               Byte address bit 4 (double-word address bit 1)
# 5     24              DDR bank address bit 0
# 6     -               Memshire number (bit 0) 000 = dwrow[0], 100 = derow[0]
# 7     -               Memshire number (bit 1) 001 = dwrow[1], 101 = derow[1]
# 8     -               Memshire number (bit 2) 010 = dwrow[2], 110 = derow[2]
# 9     even/odd        Memory controller
# 10    25              DDR bank address bit 1
# 11    2
# 12    3
# 13    4
# 14    26              DDR bank address bit 2
# 15    8
# 16    9
# 17    10
# 18    11
# 19    12
# 20    13
# 21    14
# 22    15
# 23    16
# 24    17
# 25    18
# 26    19
# 27    20
# 28    21
# 29    5
# 30    6
# 31    7
# 32    22
# 33    23

import sys, os

try:
    from elftools.elf.elffile import ELFFile
except ImportError as error:
        print("You don't have pyelftools installed. pip3 install pyelftools")

try:
    from bitstring import BitArray
except ImportError as error:
        print("You don't have bitstring installed. pip3 install bitstring")

if len(sys.argv) < 3:
    print("Usage:", sys.argv[0], "ROM|SP_RAM|PU_RAM|DRAM|DDR|MAX_BOOTRAM infile [outfile]")
    sys.exit(-1)

try:
    inFile = open(sys.argv[2], 'rb')
    elfFile = ELFFile(inFile)
except:
    print("Could not open", sys.argv[2])
    sys.exit(-1)

outFiles = []

def open_output_files(prefix, suffix, files):
    if len(sys.argv) > 3:
        (name,ext) = os.path.splitext(sys.argv[3]) # Use outfile arg for output filename
    else:
        (name,ext) = os.path.splitext(sys.argv[2]) # Use infile arg for output filename

    try:
        for i in range(files):
            outFiles.append(open("%s%s%d%s.hex" % (name, prefix, i, suffix), 'w'))
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

# basic operation on a bitfield
# bitfield - an array of 18 bytes
# offset - which bit (0-143) in the bitfield should be set to 1
def set_bit_in_bitfield(bfield, offset):
    if not len(bfield) == 18:
        print("set_bit_in_bitfield: illegal list length", x)
        sys.exit(-1)
    if offset < 0 or 144 <= offset:
        print("set_bit_in_bitfield: illegal offset", offset)
        sys.exit(-1)

    byte_index = int(offset / 8)
    bit_index = offset % 8

    bfield[byte_index] |= (1 << bit_index)

# basic operation on a bitfield
# bitfield - an array of 18 bytes
# offset - bit offset (0-137) where the 8-bit byte value should be written to the bitfield
# val - value to be written
# the function assumes that the bitfield was initially set to ALL ZEROS!  It does NOT clear any bits, only sets bits to 1.
def set_byte_in_bitfield(bfield, offset, val):
    if not len(bfield) == 18:
        print("set_byte_in_bitfield: illegal list length", x)
        sys.exit(-1)
    if offset < 0 or (144-7) <= offset:
        print("set_byte_in_bitfield: illegal offset", offset)
        sys.exit(-1)

    for bit_index in range(8):
        bit_val = val & (1 << bit_index)
        if 0 != bit_val:
            set_bit_in_bitfield(bfield, offset + bit_index)

# adds parity bits to an array of bytes
# x - array of 16 bytes
# return value - array of 18 bytes where each original 8 bits has a 1 parity bit added
def add_parity_bytes(x):
    if not len(x) == 16:
        print("add_parity: illegal list length", x)
        sys.exit(-1)

    result = [0] * 18
    offset = 0
    for i in x:
        set_byte_in_bitfield(result, offset, i)
        if even_parity(i):
            set_bit_in_bitfield(result, offset + 8)
        offset += 9

    return result

def write_hex(baseAddress, bytesPerZebuRow, inputBytesPerPanel, outputBytesPerPanel, panelsPerLine, parity, ddr, max_ram=False, pu_sram=False):
    inputBytesPerLine = inputBytesPerPanel * panelsPerLine
    bytes = []
    firstSegment = True

    # Accumulate segments on contiguous lines. When a gap is detected, write out lines for accumulated segments.
    for seg in elfFile.iter_segments():
        segAddr = seg['p_paddr']
        segSize = seg['p_filesz']
         
        segEndAddress = segAddr + segSize
        print("Segment start: %x size: %x" % (segAddr, segSize))

        if (segAddr < baseAddress and pu_sram):
            print("PU_SRAM: Segment is not eligible, since its address is smaller than base address!")
            continue

        if (segAddr > 2*baseAddress and max_ram): 
            # this should be sufficient to catch some weird cases with 
            # different memory regions in an ELF      
            print("MAX_RAM: Segment is not eligible, too far from the base address!")
            continue
             

        # If this segment starts on a later line than the previous segment ended on,
        # write out the lines for the previous segment(s) and start a new group of lines
        if (not firstSegment and ((segAddr // inputBytesPerLine) > (prevSegEndAddress // inputBytesPerLine))):
            write_lines(bytes, baseAddress, lineAddress, bytesPerZebuRow, inputBytesPerPanel, panelsPerLine, parity, ddr)
            bytes = []
            firstSegment = True

        # The first segment may start unaligned: zero-pad the lineAddress to segAddr gap, if any.
        if (firstSegment):
            lineAddress = (segAddr // inputBytesPerLine) * inputBytesPerLine
            bytes.extend([0] * (segAddr - lineAddress))
            firstSegment = False

        # Subsequent segments that start on the same line aren't necessarily contiguous: zero-pad the gap, if any.
        elif (segAddr > prevSegEndAddress):
            bytes.extend([0] * (segAddr - prevSegEndAddress))

        bytes.extend(seg.data())
        prevSegEndAddress = segEndAddress

    # write out lines after last segment
    write_lines(bytes, baseAddress, lineAddress, bytesPerZebuRow, inputBytesPerPanel, panelsPerLine, parity, ddr)

# requires address is aligned to the beginning of a line - will not zero pad before
# will zero pad the end of a line
def write_lines(bytes, baseAddress, address, bytesPerZebuRow, inputBytesPerPanel, panelsPerLine, parity, ddr):
    inputBytesPerLine = inputBytesPerPanel * panelsPerLine

    print("Writing lines from %08x to %08x" % (address, address + len(bytes)))

    # For each line, set all panel bytes including parity even if input data isn't available
    for lineIndex in range(0, len(bytes), inputBytesPerLine):

        # For each panel in the line, generate the .hex file output lines
        for panel in range(0, panelsPerLine):
            panelIndex = panel * inputBytesPerPanel

            # Some panels require multiple ZeBu output rows per line, e.g. when using DDR DRAM each memory
            # controller is a 64 byte panel but the memory init hex file requires 8 bytes per row.
            for rowIndex in range(0, inputBytesPerPanel, bytesPerZebuRow):
                # ZeBu style address. Address 0 is at base of memory
                if (ddr):
                    # DDR DRAM models require swizzled address bits
                    writeAddress = mesh_addr_to_synopsys_ddr_addr(address + lineIndex + rowIndex - baseAddress)

                else:
                    writeAddress = (address + lineIndex + rowIndex - baseAddress) // inputBytesPerLine

                wl = "@%010x " % (writeAddress)

                startIndex = lineIndex + panelIndex + rowIndex
                stopIndex = startIndex + bytesPerZebuRow

                # For SP RAM, bytes 0-15 for panel 0, 16-31 for panel 1, etc.
                # For SP ROM, bytes 0-7 for panel 0, 8-15 for panel 1, etc.
                # If we've run out of input data, substitute 0.
                outputBytes = [bytes[x] if (x) < len(bytes) else 0 for x in range(startIndex, stopIndex)]

                if parity:
                    outputBytes = add_parity_bytes(outputBytes)

                # Append outputBytes in reverse order MSByte to LSByte
                for byte in reversed(outputBytes):
                    wl += ("%02x" % byte)

                wl += "\n"
                outFiles[panel].write(wl)
    return

def mesh_addr_to_synopsys_ddr_addr(addr):
    mesh_addr = BitArray(uint=addr, length=36)
    ddr_addr = BitArray(uint=0, length=36)

    # BitArray insists on indexing the MSB as 0, so reverse for natural indexing
    mesh_addr.reverse()

    # python slice indices are [m:n+1] for bits [m:n]
    ddr_addr[0:2]   = mesh_addr[3:5]
    ddr_addr[2:5]   = mesh_addr[11:14]
    ddr_addr[5:8]   = mesh_addr[29:32]
    ddr_addr[8:10]  = mesh_addr[15:17]
    ddr_addr[10:22] = mesh_addr[17:29]
    ddr_addr[22:24] = mesh_addr[32:34]
    ddr_addr[24]    = mesh_addr[5]
    ddr_addr[25]    = mesh_addr[10]
    ddr_addr[26]    = mesh_addr[14]

    # Undo previous reverse before converting back to int
    ddr_addr.reverse()

    return ddr_addr.int

def sp_rom_write_hex():
    open_output_files("", "", 8)
    write_hex(0x40000000, 8, 8, 8, 8, False, False)
    return

def sp_ram_write_hex():
    open_output_files("SP_RAM", "", 4)
    write_hex(0x40400000, 16, 16, 18, 4, True, False)
    return

def pu_sram_write_hex():
    output_dir=""
    
    if "-t" in sys.argv: 
        for index,value in enumerate(sys.argv):
             if value == "-t":
                output_dir=sys.argv[index+1]
    

    if (output_dir !=""):
        try:
            os.makedirs(output_dir)
        except FileExistsError:
            pass   

    outFiles.append(open("{}PU_SRAM0.hex".format(output_dir), 'w'))
    outFiles.append(open("{}PU_SRAM1.hex".format(output_dir), 'w'))
    outFiles.append(open("{}PU_SRAM2.hex".format(output_dir), 'w'))
    outFiles.append(open("{}PU_SRAM3.hex".format(output_dir), 'w'))
    write_hex(0x20000000, 16, 16, 18, 4, True, False, pu_sram=True)


def dram_write_hex(ddr):
    open_output_files("_dwrow", "_even", 4) # memshire 0-3 "west 0-3" addr[9] = 0 "even"
    open_output_files("_derow", "_even", 4) # memshire 4-7 "east 0-3" addr[9] = 0 "even"
    open_output_files("_dwrow", "_odd", 4)  # memshire 0-3 "west 0-3" addr[9] = 1 "odd"
    open_output_files("_derow", "_odd", 4)  # memshire 4-7 "east 0-3" addr[9] = 1 "odd"

    if (ddr):
        # DDR models with address swizzling and 8-bytes per ZeBu hex line
        write_hex(0x8000000000,  8, 64, 64, 16, False, True)
    else:
        # AXI models with 64-bytes per ZeBu hex line
        write_hex(0x8000000000, 64, 64, 64, 16, False, False)
    return

def max_bootram_write_hex():
    output_dir=""
    
    if "-t" in sys.argv: 
        for index,value in enumerate(sys.argv):
             if value == "-t":
                output_dir=sys.argv[index+1]
    

    if (output_dir !=""):
        try:
            os.makedirs(output_dir)
        except FileExistsError:
            pass   
  
    outFiles.append(open("{}BootRAM.hex".format(output_dir), 'w')) ##hacky but lets not reinvent the wheel 
    write_hex(0x10000,4,4,4,1,False, False, max_ram=True)

if (sys.argv[1]) == "ROM":
    sp_rom_write_hex()
elif (sys.argv[1]) == "SP_RAM":
    sp_ram_write_hex()
elif (sys.argv[1]) == "PU_RAM":
    pu_sram_write_hex()
elif (sys.argv[1]) == "DRAM":
    dram_write_hex(False)
elif (sys.argv[1]) == "DDR":
    dram_write_hex(True)
elif (sys.argv[1]) == "MAX_BOOTRAM":
    max_bootram_write_hex()
else:
    print("Unsupported argument", sys.argv[1])
    sys.exit(-1)
