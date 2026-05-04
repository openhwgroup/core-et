#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import sys
from struct import *

sp_otp_size = 8192
physical_bit_index_adjustment = 16218
starting_addr = 3931
starting_bank = 3

def convert_bin_to_zebu(zebu_path_prefix):
    try:
        binary_file = open(sys.argv[1], "rb")
    except:
        print("Unable to open binary file '{:s}' for reading!".format("blank_otp.bin"))
        sys.exit(-1)

    try:
        binary_data = binary_file.read()
    except:
        print("Error reading binary file!")
        sys.exit(-1)
    
    if len(binary_data) != (sp_otp_size // 8):
        print("Invalid binary file size!")
        sys.exit(-1)
    
    physical_pages = [ bytearray(512), bytearray(512), bytearray(512) ]

    bit_value = []
        
    for virtual_word_index in range(256):
        virtual_word_value = unpack_from("<I", binary_data, 4 * virtual_word_index)[0]
        for virtual_bit_in_word_offset in range(32):
            bit_value.append((virtual_word_value>>virtual_bit_in_word_offset) & 1)

    num_loaded = 0
    for bank_index in range(6):
        filename = "efuse_bank_" + str(bank_index) + ".txt"
        f = open(filename,"w")
        for addr_val in range(4096):
            if addr_val<16:
                if (addr_val<starting_addr and bank_index==starting_bank) or bank_index<starting_bank or num_loaded>=sp_otp_size:
                    f.write("@0%1x00 1\n" % (addr_val))
                    f.write("@1%1x00 1\n" % (addr_val))
                else:
                    f.write("@0%1x00 %1d\n" % (addr_val, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    f.write("@1%1x00 %1d\n" % (addr_val, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    num_loaded += 1
            elif addr_val<256:
                if (addr_val<starting_addr and bank_index==starting_bank) or bank_index<starting_bank or num_loaded>=sp_otp_size:
                    f.write("@0%1x0%1x 1\n" % (addr_val%16, (addr_val>>4)%16))
                    f.write("@1%1x0%1x 1\n" % (addr_val%16, (addr_val>>4)%16))
                else:
                    f.write("@0%1x0%1x %1d\n" % (addr_val%16, (addr_val>>4)%16, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    f.write("@1%1x0%1x %1d\n" % (addr_val%16, (addr_val>>4)%16, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    num_loaded += 1
            else:
                if (addr_val<starting_addr and bank_index==starting_bank) or bank_index<starting_bank or num_loaded>=sp_otp_size:
                    f.write("@0%1x%2x 1\n" % (addr_val%16, (addr_val>>4)%256))
                    f.write("@1%1x%1x 1\n" % (addr_val%16, (addr_val>>4)%256))
                else:
                    f.write("@0%1x%2x %1d\n" % (addr_val%16, (addr_val>>4)%256, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    f.write("@1%1x%1x %1d\n" % (addr_val%16, (addr_val>>4)%256, 0 if bit_value[sp_otp_size-1-num_loaded] else 1))
                    num_loaded += 1
        f.close()

convert_bin_to_zebu("efuse_bank")
