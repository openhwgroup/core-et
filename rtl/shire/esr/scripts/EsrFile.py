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
# ESR REGISTER
################################################################################
class EsrRegister (XsrFile.XsrRegister):
    """ESR register"""
    header = ["Mod", "PP", "Address", "Name", "Access", "Description", "Type", "width", 
              "rdata", "wdata", "shadow", "resetVal", "reset", "triggerCnt", "latch", "connect_to_port",
              "implemented", "ifdef", "comment" ]
    boolFields = [ 'implemented', "connect_to_port"]
    __allRegs = {}
    __modName = ''

    def __init__ (self, values, rowNr):
        XsrFile.XsrRegister.__init__(self, values, rowNr, EsrRegister.header, EsrRegister.boolFields, "clock")

    def shared(self):
        return False

    def shared_macro_str(self):
        return ""
        
    @classmethod
    def adWidth(EsrRegister, prefix = '`' ):
        return prefix + "ESR_%s_APB_AD_WIDTH" % EsrRegister.__modName.upper()

    @classmethod
    def clearRegs(EsrRegister, mod = None):
        EsrRegister.__allRegs = {}
        EsrRegister.__modName = mod

    @classmethod
    def allRegs(EsrRegister):
        return EsrRegister.__allRegs
        
    def stamp(EsrRegister):
        return 'esr'

    def fullStamp(EsrRegister):
        return 'esr_' + EsrRegister.__modName

    def wdataSignal(self):
        return 'apb_pwdata'

    @classmethod
    def checks(EsrRegister):
        XsrFile.XsrRegister.checks(EsrRegister.__allRegs)

    def portDeclaration(self, indentLevel = 0):
        
        if self.getField('connect_to_port', False):
            portName = self.portName()
            direction = 'output'  if self.writeable() else 'input'
            portType = self.type.name if self.typed() else "logic %s" % self.dataRange()
            return Indent*indentLevel + "%s %s %s,\n"  % (direction, portType, portName)
        else:
            return ''

    def portAssignment(self, indentLevel = 0):
        if self.getField('connect_to_port', False) and self.writeable():
            return Indent*indentLevel + "assign %s = reg_%s;\n" % (self.portName(), self.name())
        else:
            return ''

    def composeAddress(self):
        lsb = int(self.conf['Address'], 16)
        msb = int (self.conf['PP'])
        self.regNo = lsb
        if msb <0 or msb > 3:
            raise Exception ("bad PP bits %s(have to be between 0 and 3)\n" % self.conf['PP'])
        return " {2\'d%d, %s'h%x}" % (msb, self.adWidth(), lsb);
        
    def addressWithWidth(self):
        return self.conf['Address']

    def cppAddressDefinition(self, stamp = None):
        if stamp == None:
            stamp = self.fullStamp().upper()
        pref = "%s_%s" % (stamp, self.name().upper())
        prot_names = [ 'PRV_U', 'PRV_S', 'PRV_D', 'PRV_M' ]
        prot = prot_names[int(self.conf['PP'])]
        acc = "esr_access_" + self.conf['Access']
        return """
#define %s_REGNO 0x%x
#define %s_PROT %s
#define %s_ACCESS %s""" % (pref, self.regNo, pref, prot, pref, acc)

    def cppList(self, stamp = None):
        if stamp == None:
            stamp = self.fullStamp().upper()
        return stamp  + "_ADD_TO_LIST(" + self.name().upper() + ");"
    
################################################################################
# ESR REGISTER FILE
################################################################################

class EsrFile (XsrFile.XsrFile):
    """ESR file"""
    ESR_SPREADSHEET_ID = '1HdYfPNeET3YZFI0SN69rizUj2PexYvW0aGiHlD-e1pM'
    REGS_CSV_FILE = "esr" # without .csv extension

    def __init__ (self, fromGdocs = False, mod = None):
        filt = {'Mod': mod} if mod else None
        EsrRegister.clearRegs(mod)
        stamp = 'esr_%s' % mod if mod else 'esr'

        if fromGdocs:
            conf = XsrFile.XsrFileSheetConfiguration (
                min_cols = 4,
                gdocsConf = XsrFile.XsrFileGdocsConfiguration(sheetId = EsrFile.ESR_SPREADSHEET_ID,
                                                              mainSheet = 'EsrFile',
                                                              download_filename = 'Minion ESR Registers.xlsx')
            )
        else:
            base_dir = os.environ['RTLROOT'] + '/shire/esr/scripts/'
            conf = XsrFile.XsrFileSheetConfiguration (
                min_cols = 4,
                csvConf =  XsrFile.XsrFileCsvConfiguration (mainFile = EsrFile.REGS_CSV_FILE,
                                                            base_dir = base_dir)
            )
        
        XsrFile.XsrFile.__init__(self, 
                                 stamp = stamp, 
                                 expHeader = EsrRegister.header, 
                                 regType = EsrRegister,
                                 conf = conf,
                                 filt = filt)
    def requiresLatchPH1(self):
        return True

    def LatchPH1Declaration(self):
        return "`ESR_LATCH_P1_WDATA_DECLARATION"

    
    def portDeclarations(self):
        ret=''
        for g in self.groups:
            gr =''
            for r in g.regs:
                gr+=r.portDeclaration(1)
            if len(gr)>0:
                ret+=g.comment(1)
                ret+=gr
        return ret

    def portAssignments(self):
        ret=''
        for g in self.groups:
            gr =''
            for r in g.regs:
                gr+=r.portAssignment(1)
            if len(gr)>0:
                ret+=g.comment(1)
                ret+=gr
        return ret

    def cppList( self, stamp = None ):
        l = []
        for g in self.groups:
            for r in g.regs:
                l.append(r.cppList(stamp))
        return "\n".join(l)
