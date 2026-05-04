# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from __future__ import print_function
from googleapiclient.discovery import build
from httplib2 import Http
from oauth2client import file, client, tools
import sys
from collections import defaultdict
import io
import re
import os

from XsrFileIO import Indent
import XsrFile

################################################################################
# CSR REGISTER
################################################################################
class CsrRegister (XsrFile.XsrRegister):
    """CSR register"""
    header = ['Address', 'Name', 'Privilege', 'Access', 'Description','shared','Type',
              'width','rdata','sign_ext_rdata', 'wdata','shadow', 'resetVal', 'aon','reset',
              'triggerCnt', 'latch','implemented', 'ifdef', 'trap_if','comment']
    boolFields = ['shared', "aon", 'implemented',"sign_ext_rdata"]
    __allRegs = {}

    def __init__ (self, values, rowNr):
        XsrFile.XsrRegister.__init__(self, values, rowNr, CsrRegister.header, CsrRegister.boolFields, "clock_wb", "clock")
        
    @classmethod
    def adWidth(CsrRegister):
        return "`CSR_REG_NR_SZ"

    @classmethod
    def allRegs(CsrRegister):
        return CsrRegister.__allRegs
        
    @classmethod
    def updateTools(CsrRegister, filename):
        defines_end = "// END OF CSR AD DEFINITIONS\n";
        declar_end = "// END OF CSR DECLARATIONS\n"

        # get map of all addresses
        regs = {}
        for name,reg in CsrRegister.__allRegs.items():
            regs[name] = {'ad':reg.conf['Address'], 'defined': False, 'declared': False }

        # read file contents
        old_contents = open(filename, "r").readlines()
        new_contents = []
        defines_done = False
        declar_done = False
        for i, line in enumerate(old_contents):
            m = re.match(r"#define CSR_(.*) (.*)\n$", line)
            if m != None:
                if defines_done:
                    raise Exception("found csr defintion in %s after %s => %s" % (filename, defines_end, line))
                else:
                    reg = m.group(1).lower()
                    ad  = m.group(2)
                    if reg in regs:
                        regs[reg]['defined'] = True
                        if (regs[reg]['ad'] != int(ad,16)):
                            line = "#define CSR_%s 0x%x\n" % (reg.upper(), regs[reg]['ad'])
                            print ("changed ad for %s => from %s to %x" % (reg, ad, regs[reg]['ad']))

            m = re.match(r"^\s*DECLARE_CSR\(\s*([^\s]+)\s*,", line)
            if m != None:
                if declar_done:
                    raise Exception("found csr declarations in %s after %s => %s" % (filename, declar_end, line))
                else:
                    reg = m.group(1)
                    if reg in regs:
                        regs[reg]['declared'] = True

            if line == defines_end:
                # add missing defines
                new_defs = ''
                for name,reg in sorted(regs.items()):
                    if not reg['defined']:
                        new_defs += "#define CSR_%s 0x%x\n" % (name.upper(), reg['ad'])
                        print ("adding address definition for %s in %s" % (name, filename))
                line = new_defs + line
                defines_done = True
            if line == declar_end:
                # add missing declarations
                new_decl = ''
                for name, reg in sorted(regs.items()):
                    if not reg['declared']:
                        new_decl += "DECLARE_CSR(%s, CSR_%s)\n" % (name, name.upper())
                        print ("adding declaration for %s in %s" % (name, filename))
                line = new_decl + line
                declar_done = True
            new_contents.append(line)

        with open(filename, "w") as f:
            f.write("".join(new_contents))

    def stamp(CsrRegister):
        return 'csr'

    def wdataSignal(self):
        return 'wb_wdata'

    @classmethod
    def checks(CsrRegister):
        XsrFile.XsrRegister.checks(CsrRegister.__allRegs)
        

        
################################################################################
# CSR REGISTER FILE
################################################################################

class CsrFile (XsrFile.XsrFile):
    """CSR file"""
    CSR_SPREADSHEET_ID = '1oFrVRSxR8GB0JkCfAMMhcB_oiWGJM8ASzjqWIN_Y5J0'
    REGS_CSV_FILE = "csr" # without .csv exception

    def __init__ (self, fromGdocs = False):
        if fromGdocs:
            conf = XsrFile.XsrFileSheetConfiguration (
                min_cols = 5,
                gdocsConf = XsrFile.XsrFileGdocsConfiguration(sheetId = CsrFile.CSR_SPREADSHEET_ID,
                                                              mainSheet = 'CsrFile',
                                                              download_filename = 'Minion CSR Registers.xlsx')
            )

        else:
            
            base_dir = os.environ['RTLROOT'] + '/shire/minion/intpipe/csr/scripts/'
            conf = XsrFile.XsrFileSheetConfiguration (
                min_cols = 5,
                csvConf = XsrFile.XsrFileCsvConfiguration(mainFile = CsrFile.REGS_CSV_FILE,
                                                          base_dir = base_dir)
            )

        XsrFile.XsrFile.__init__(self, 
                                 stamp= 'csr', 
                                 expHeader= CsrRegister.header,
                                 regType = CsrRegister,
                                 conf = conf)

    
    def updateTools(self, filename):
        CsrRegister.updateTools(filename)

    @classmethod
    def getCsrDv(self):
        regs = {}
        for name,reg in CsrRegister.allRegs().items():
            regs[name] = {'address':reg.conf['Address'], 'access':reg.conf['Access']}
        return regs             

