#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# COnvert IntelHex to ET hex
import sys
import os
#import intelhex
import struct

from intelhex import IntelHex


if __name__ == '__main__':

    print( "\nConvert inteHex to ET hex\n" )
    if len(sys.argv) > 1 :
        print( "File Name: ", sys.argv[1] )
    print( )

    output_name = "zebu_" + sys.argv[1]
    f = open( output_name , "w" )

    ih = IntelHex()


    ih.loadhex(  sys.argv[1] )

    #ih.dump()

    print( "    minAddr: %x" % ih.minaddr() )
    print( "    maxAddr: %x" % ih.maxaddr() )

    segments = ih.segments()
    index=0
    for seg in segments :
        startAddr = seg[0]
        stopAddr  = seg[1]
        print( "    Start: %x   Stop: %x" % ( startAddr, stopAddr ) )
        if index == 0:
          for addr in  range ( startAddr, stopAddr, 64 ) :
              wl = "@%04X " % ((addr - startAddr) >>6)
              for i in range ( 0, 64 ) :
                  wl += ( "%02X" % ih[addr + 63 - i] )
              f.write( wl )
              f.write("\n")
        index+=1

    f.close
