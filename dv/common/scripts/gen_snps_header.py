#!/usr/bin/python36
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# /eng/dvtools/release/python/dv_py3/bin/python


#
# Rajko there are two different binaries. IT added to python36 binary as well
#

#ipyxact example. Parses an IP-XACT XML file called generic_example.xml
#and prints out a C header of the register maps found
import argparse
import sys
import time

from ipyxact.ipyxact import Component

def parse_args():
    parser = argparse.ArgumentParser(
            description='Generate a C header from an IP-XACT file')
    parser.add_argument('ipxact_file', help='IP-XACT file to parse')
    parser.add_argument('-o', dest='output_file', help='Write output to file')
    parser.add_argument('-g', '--header-guard', dest='header_guard',
            help='Header guard. If not specified it will be derived from the '
                 'output file name else there will be no guard.')
    return parser.parse_args();

def open_output(output):
    return open(output, 'w') if output else sys.stdout

def get_header_guard(args):
    if args.header_guard:
        return args.header_guard
    elif args.output_file:
        return args.output_file.upper().replace('.', '_').replace('/', '_');
    else:
        return None

def write_prologue(of, args):
    guard = get_header_guard(args);
    of.write('/* Header generated from ipxact ({}) */\n'
            .format(time.strftime("%Y-%m-%d %H:%M:%S")));
    if guard:
        of.write('#ifndef {}\n'.format(guard))
        of.write('#define {}\n\n'.format(guard))
    of.write('#include "{}"\n'.format(args.output_file.replace('.h','_macro.h')))
    of.write('\n')
    of.write('struct {}\n'.format(args.output_file.replace('.h',' {')))

def write_prologue_macro(of, args):
    guard = get_header_guard(args).replace('_H','_MACRO_H');
    of.write('/* Header generated from ipxact ({}) */\n\n\n'
            .format(time.strftime("%Y-%m-%d %H:%M:%S")));
    if guard:
        of.write('#ifndef {}\n'.format(guard))
        of.write('#define {}\n\n'.format(guard))
    of.write('\n\n')

def write_epilogue(of, args):
    of.write('};\n\n')
    guard = get_header_guard(args);
    if guard:
        of.write('#endif /* {} */\n\n'.format(guard))

def write_epilogue_macro(of, args):
    of.write('\n\n')
    guard = get_header_guard(args).replace('_H','_MACRO_H');
    if guard:
        of.write('#endif /* {} */\n\n'.format(guard))

def gen_mask(offset, width):
    mask = 0
    for i in range(offset, offset+width):
        mask += 1<<i
    return mask

def write_enum(field):
    print ("RR: Write Enum.....\n")
    of.write("typedef enum {\n")
    for es in field.enumeratedValues:
        for e in sorted(es.enumeratedValue, key=lambda x: x.value):
            of.write("    {}_{} = {},\n".format(
            field.name.lower(), e.name, e.value<<field.bitOffset))
    of.write("} {}_t;\n\n".format(field.name.lower()))

def write_reg_fields(of, memory_maps, offset=0, name=None):
    for m in memory_maps.memoryMap:
        if name:
            mname=name
        else:
            mname = m.name
        multiblock = len(m.addressBlock) > 1

        for block in m.addressBlock:
            if multiblock:
                bname = mname + '_' + block.name.upper()
            else:
                bname = mname
            for reg in sorted(block.register, key=lambda a: a.addressOffset):
                reg_name = '{}'.format(reg.name).upper()
#def write_reg_fieldsOld(reg, reg_name):
                of.write("/*-------------------------------------------------------------------------------------*/\n")
                for f in sorted(reg.field, key=lambda x: x.bitOffset):
                    of.write("/*  {} : {:<20s}*/\n".format(reg_name, f.name))
                    rnSh = "{}__{}__SHIFT".format(reg_name, f.name.upper())
                    rnWi = "{}__{}__WIDTH".format(reg_name, f.name.upper())
                    rnMs = "{}__{}__MASK".format(reg_name, f.name.upper())
                    of.write("#define {:<64s}0x{:08X}\n".format(rnSh, f.bitOffset))
                    of.write("#define {:<64s}0x{:08X}\n".format(rnWi, f.bitWidth))
                    of.write("#define {:<64s}0x{:08X}\n\n".format(rnMs, gen_mask(f.bitOffset, f.bitWidth)))
                    if f.enumeratedValues:
                        of.write('\n')
#                        write_enum(field)
                of.write('\n')

def write_memory_maps(of, memory_maps, offset=0, name=None):
    for m in memory_maps.memoryMap:
        if name:
            mname=name
        else:
            mname = m.name
        multiblock = len(m.addressBlock) > 1

        for block in m.addressBlock:
            if multiblock:
                bname = mname + '_' + block.name.upper()
            else:
                bname = mname
            structOffset = offset + block.baseAddress
            dummyCount = 0
            for reg in sorted(block.register, key=lambda a: a.addressOffset):
                regOffset = offset + block.baseAddress + reg.addressOffset
                if structOffset != regOffset :
                    count = (regOffset - structOffset)/4
#                    print "RR: dummyCount ",dummyCount,"  Count: ",count
                    of.write("    volatile uint32_t dummy%d[%d];\n" % (dummyCount, count) )
                    dummyCount = dummyCount + 1
                    structOffset = structOffset + count*4
                structOffset = structOffset + reg.size/8
                reg_name = '    volatile uint32_t {};'.format(reg.name)
                reg_addr = '/* 0x{:08X} */'.format(regOffset)
                of.write("{: <60s} {}\n".format(reg_name, reg_addr))

                reg_name = '{}_'.format(reg.name).upper()

if __name__ == '__main__':
    args = parse_args()


#    with open(args.ipxact_file) as transformF:
#        u = transformF.read()
#        w=unicode(u, errors='replace').encode('ascii', 'replace')
#        with open("rr.xml", "w") as tmpF:
#            tmpF.write(w)

    with open(args.ipxact_file) as f:
#    with open('rr.xml') as f:
        name = None
        offset = 0

        component = Component()
        print( type(component) )
        component.load(f)
        print( "RR: " )
        print( component )

        fileMacro = args.output_file.replace('.h','_macro.h')

        with open_output(args.output_file) as of:
            write_prologue(of, args);
            write_memory_maps(of, component.memoryMaps, offset, name)
            write_epilogue(of, args);

        with open_output(fileMacro) as ofM:
            write_prologue_macro(ofM, args);
            write_reg_fields(ofM, component.memoryMaps, offset, name)
            write_epilogue_macro(ofM, args);
