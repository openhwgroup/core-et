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

from XsrFileIO import Indent
from XsrSheet import XsrSheet
from XsrType import XsrType

################################################################################
# XSR REGISTER
################################################################################
class XsrRegister:
    """XSR register"""

    def __init__ (self, values, rowNr, header, boolFields, clock_sec = 'clock', clock_aon = 'clock_aon'):
        self.clock_sec = clock_sec
        self.clock_aon = clock_aon
        # add empty values
        self.rowNr = rowNr
        while len(values) < len(header):
            values.append(None)

        # save configuration
        self.conf = dict ( zip(header,values) )

        # translate some fields from string to int
        try:
            self.conf['Address'] = self.composeAddress()
        except:
            raise Exception("Error converting ad to hex for row %d" % rowNr);

        # and from string to Bool
        for f in boolFields:
            if self.conf[f] == None or self.conf[f] == '' or self.conf[f].upper() == 'NO':
                self.conf[f] = False
            elif self.conf[f].upper() == 'YES':
                self.conf[f] = True
            else:
                raise Exception("%s, row %d, bad value in field %s: %s", self.conf['Name'], rowNr, f, self.conf[f])

        # create type
        t = self.getField('Type',None)
        if t != None and t != 'MANUAL':
            self.type = XsrType(t)
        else:
            self.type = None


    def composeAddress(self):
        return int(self.conf['Address'], 16)
        
    @classmethod
    def adWidth(XsrRegister):
        raise NonImplementedError()

    def wdataSignal(self):
        raise NonImplementedError()

    def resetMacroName(self):
        r = self.getField('reset', 'GLOBAL')
        resetMacros = { 'GLOBAL': "", 'DEBUG': "_D", 'NON-DEBUG': "_ND"}
        if r not in resetMacros:
            raise Exception ("bad custom reset: %s" % r);
        return resetMacros[r]

    def resetSignalName(self):
        r = self.getField('reset', 'GLOBAL')
        resetSignals = { 'GLOBAL': "reset_c", 'DEBUG': "reset_d", 'NON-DEBUG': "reset_w"}
        if r not in resetSignals:
            raise Exception ("bad custom reset: %s" % r);
        return resetSignals[r]

    def latchResetDeclaration(self):
        macro = "%s_LATCH_RESET_DECLARATION" % self.stamp_uc()
        return self.macro(macro, self.resetSignalName(), indentLevel=0, newLine=False)
    
    @classmethod
    def updateTools(XsrRegister, filename):
        raise NonImplementedError()

    @classmethod
    def allRegs(XsrRegister):
        raise NonImplementedError()

    def stamp(self):
        raise NonImplementedError()

    def stamp_uc(self):
        return self.stamp().upper()

    def fullStamp(self): # this is used for ESR... stamp="esr", fullStamp="esr_subBlock"
        return self.stamp()

    @classmethod
    def checks(XsrRegister, allRegs):
        knownAds = {}
        for name,reg in allRegs.items():
            ad_str = '%s' % (str(reg.conf['Address']))
            if reg.ifdef():
                ad_str+="(with ifdef %s)" % reg.ifdef()
            # check that the address has not already been used
            if ad_str in knownAds:
                raise Exception ("ad 0x%x for %s already used\n" % (ad_str, name))
            knownAds[ad_str] = 1

            # check shadow is allowed (either typed, or regs with the exact same length)
            if reg.shadowing() and reg.type == None and ( reg.width() != reg.shadow().width() or reg.shadow().type != None):
                raise Exception("%s row %d: shadowing only for typed registers or registers with exact same length" % (name, reg.rowNr))

            # cannot specify both type and width
            if reg.getField('width',None) != None and reg.type != None:
                raise Exception("%s: cannot specify both type and width (row %d)" % (name, reg.rowNr))
                
            # can't be a latch and...
            if reg.latch():
                if reg.getField('triggerCnt'):
                    raise Exception("%s: counters cannot be latches (row %d)" % (name , reg.rowNr))
                if reg.getField('wdata'):
                    print ("Warning => %s: has custom wdata and using latches (row %d).. check there is no logic between core wdata and data written into the reg (other than enables) " % (name , reg.rowNr))
                if reg.manual():
                    raise Exception("%s: MANUAL Xsrs cannot be latches (row %d)" % (name , reg.rowNr))

            if reg.shadowing() and (reg.getField('latch', '') == 'YES') !=  reg.shadow().latch():
                raise Exception ("%s: Xsrs with is a shadow of another register and they have different latch options (row %d)" % (name , reg.rowNr))
            
            if reg.shadowing() and reg.getField('latch', '') == 'YES':
                raise Exception ("%s: Xsrs shadows can only be FF based (row %d)" % (name , reg.rowNr))

    def getField(self, field, default_v = None):
        if not field in self.conf or self.conf[field] == None or self.conf[field] == '' :
            return default_v
        else:
            return self.conf[field]

    def trapCondition(self):
        return self.getField('trap_if',None)

    def hasTrap(self):
        return self.trapCondition() != None
    
    def writeable(self):
        return self.conf['Access'] == 'RW' 

    def sign_ext_str(self):
        if 'sign_ext_rdata' in self.conf and self.conf['sign_ext_rdata']:
            return '_SX'
        else:
            return ''
    
    def shared(self):
        if 'shared' in self.conf:
            return self.conf['shared']
        else:
            return True

    def implemented(self):
        return self.conf['implemented']

    def shared_macro_str(self):
        return "_SHARED" if self.shared() else "_NON_SHARED"

    def ifdef (self):
        return self.getField('ifdef', None)

    def latch(self):
        if self.shadowing():
            return self.shadow().latch()
        else:
            return self.getField('latch', 'NO') == 'YES'

    def manual(self):
        t = self.getField('Type', None)
        return t == 'MANUAL'

    def typed(self):
        return self.type != None

    def width(self):
        if self.typed():
            return '$bits(%s)' % self.type.name
        else:
            return self.getField('width', '64')

    def msb(self):
        w = self.width()
        if w.isdigit():
            return str(int(w)-1)
        else:
            return w + '-1'

    def portName(self):
        p = "%s_%s" % (self.stamp(), self.name())
        if self.writeable():
            return p + "_op"
        else:
            return p + "_ip"

    def counterLSBbits(self):
        w = self.width()
        if w.isdigit():
            return str(int(w).bit_length())
        else:
            return '$clog2(%s)'%w

    def name (self):
        return self.conf['Name']

    def shadowing(self):
        return self.getField('shadow', None) != None

    def shadow(self):
        reg = self.getField('shadow', None)
        if reg not in self.allRegs():
            raise Exception("invalid shadow %s for %s" % (reg, self.name()))
        return self.allRegs()[reg]

    def clock (self):
        if 'aon' in self.conf:
            return self.clock_aon if self.conf['aon'] else self.clock_sec
        else:
            return self.clock_sec

    def rdata(self):
        if not self.shadowing():
            if self.getField("connect_to_port", False) and  not self.writeable():
                def_v = self.portName()
            else:
                def_v = "reg_" + self.name()
        else:
            def_v = self.shadow().rdata()
        return self.getField('rdata', def_v )

    def rdataConstant(self):
        d = self.rdata()
        if d.isdigit():
            return True
        if re.match("$`%s" % self.stamp_uc(), d) != None: #uses a `XSR_XXX macro
            return True
        if re.match("\d*'(b|h|d)?[a-fA-F0-9]+", d) != None: #matches 'b0
            return True
        return False

    def rdataOverridesShadow(self):
        return self.shadowing() and len(self.getField('rdata', '')) > 0 # rdata provided in spreadsheet... use it even if shadowing
        
    def rdataWithShadowConv(self):
        if self.rdataOverridesShadow():
            return False
        else:
            return self.shadowing() and ( self.typed() and self.shadow().type.name != self.type.name )
        
    def dataRange(self, ignore64=False):
        w = self.width()
        if w == '64' and ignore64:
            return ''

        if w.isdigit():
            return '[%d:0]' % (int(w)-1)
        else:
            return '[%s-1:0]' % w

    def wdata(self):
        def_v = self.wdataSignal()
        if self.latch():
            def_v+='_1p'

        if self.typed():
            def_v = self.type.wdata(def_v)
            if self.type.generic:
                def_v = "%s'(%s)" % (self.type.name, def_v)
        else:
            def_v += self.dataRange(ignore64 = True)
            
        return self.getField('wdata', def_v)

    def macro(self, macro, params, indentLevel=1, newLine=True):
        # conver params to list, if it is a single value
        if not isinstance(params, list):
            params = [params]
        # and convert any numerical params to string
        params = [ str(i) for i in params]

        ret = Indent*indentLevel + ('`%s(' % macro) + ', '.join(params) + ')'
        ifdef_indent = Indent*indentLevel + ('`ifdef')
        endif_indent = Indent*indentLevel + ('`endif')

        if self.ifdef():
            ret="%s %s\n%s\n%s" % ( ifdef_indent, self.ifdef(), ret, endif_indent)

        if newLine:
            ret+='\n'

        return ret

    def addressDefinition(self):
        ret = Indent + "%s_ad_%s = %s," % ( self.fullStamp(), self.name().upper(), 
                                            self.addressWithWidth())
        if self.ifdef():
            ret = "`ifdef %s\n%s\n`endif" % (self.ifdef(), ret)
        return ret

    def addressWithWidth(self):
        return ("%s'h%x" % (self.adWidth(), self.conf['Address']))
    
    def declarations(self, indentLevel = 0):
        if self.writeable():
            params = [self.name()]
            macro = '%s_DECLARATION%s' % (self.stamp_uc(), self.shared_macro_str())
            if self.shadowing() or self.manual():
                macro+='_DEC_ONLY'
            else:
                if self.latch():
                    macro+="_LATCH"
                if self.typed():
                    macro+='_WITH_TYPE'
                    params.append(self.type.name)
                else:
                    params.append(self.width())

            return self.macro(macro, params, indentLevel, newLine=False)

    def readAndDecodeProcDefault(self, indentLevel = 2):
        if self.writeable():
            macro = '%s_DECODE_DEFAULT%s' % (self.stamp_uc(), self.shared_macro_str())
            return self.macro(macro, self.name(), indentLevel)
        else:
            return ''

    def readAndDecodeCase(self, indentLevel = 4):
        if self.writeable():
            macro = '%s_DECODE_CASE%s%s' % (self.stamp_uc(), self.shared_macro_str(), self.sign_ext_str())
            macroParams = ['%s_ad_' % self.fullStamp() + self.name().upper(), self.name(), self.rdata()]
            if self.rdataConstant():
                macro += '_RD_CONSTANT'
        else:
            macro = '%s_DECODE_CASE_RO%s'  % (self.stamp_uc(), self.shared_macro_str())
            macroParams = [ '%s_ad_' % self.fullStamp() + self.name().upper(), self.rdata()]

        if self.rdataOverridesShadow():
            # do not use, but generate the conversion function in the _auto file
            self.shadow().type.rdataShadow(self.type)
        if self.rdataWithShadowConv():
            macroParams.append( self.shadow().type.rdataShadow(self.type) )
            macro+='_SHADOW'

        if self.hasTrap():
            macro+="_WITH_TRAP"
            macroParams.append(self.trapCondition())
        return self.macro(macro, macroParams, indentLevel)

    def readAndDecodeSeq(self, indentLevel = 0):
        if self.writeable():
            return self.macro('%s_DECODE_SEQ%s' % (self.stamp_uc(), self.shared_macro_str()), self.name(), indentLevel)          
        else:
            return ''

    def writeCombDefault(self, indentLevel = 2):
        cnt = self.getField('triggerCnt')
        macro=None
        if cnt:
            lsb='0+:%s' % self.counterLSBbits()
            msb='%s:%s' % (self.msb(), self.counterLSBbits())
            macro = '%s_WRITE_COMB_DEFAULT%s_CNT' % (self.stamp_uc(), self.shared_macro_str())
            macroParams = [self.name(), msb, lsb, cnt]
        elif self.writeable() and not self.shadowing() and not self.manual():
            if not self.latch():
                macro = '%s_WRITE_COMB_DEFAULT' % self.stamp_uc()
            else:
                macro = '%s_WRITE_LATCH_CLK_EN_ZERO' % self.stamp_uc()
            macroParams = self.name()

        if macro:
            return self.macro(macro, macroParams, indentLevel)
        else:
            return ''
 
    def writeCoreComb(self, indentLevel = 2):
        if self.writeable() and not self.manual():
            latch_macro = '_LATCH' if self.latch() else ''
            macro = '%s_WRITE_COMB%s%s' % (self.stamp_uc(), latch_macro, self.shared_macro_str())
            macroParams = [ self.name(), self.wdata()]
            if self.shadowing():
                if self.shadow().typed():
                    macro += "_SHADOW"
                    macroParams+=[self.shadow().name(), self.shadow().type.wdataShadow(self.type)]
                else:
                    if self.shadow().width() != self.width():
                        raise Exception("Non typed shadow and different widths %s -> %s " % (self.name(), self.shadow().name()))
                    macro += "_UNTYPED_SHADOW"
                    macroParams+=[self.shadow().name()]
            return self.macro(macro, macroParams, indentLevel)
        else:
            return ''

    def writeSeq(self, indentLevel=0):
        if self.shadowing() or not self.writeable() or self.manual():
            return ''
        rv = self.getField("resetVal")
        latch_or_ff = "LATCH" if  self.latch() else "SEQ"
        macro = '%s_WRITE_%s%s' % (self.stamp_uc(), latch_or_ff, self.shared_macro_str())

        if self.latch():
            if self.typed():
                params= [self.clock(), self.name(), self.name() + '_pre', self.name() + '_next']
            else:
                params= [self.clock(), self.name(), self.name(), self.name() + '_next']
        else:
            if self.typed():
                params = [self.clock(), self.name() + '_pre', self.name() + '_next']
            else:
                params = [self.clock(), self.name(), self.name() + '_next']

        if rv:
            macro+='_RST' + self.resetMacroName()
            params.append(rv)

        write = self.macro ( macro, params, indentLevel)

        if self.typed():
            macro = "%s_WRITE_TYPE_WITH_RESERVED%s" % (self.stamp_uc(), self.shared_macro_str())
            write_res = self.macro(macro, [self.name(), self.type.writeNonReservedName()], indentLevel)
            return write + write_res
        else:
            return write


         
################################################################################
# XSR REGISTER GROUP
################################################################################
class XsrRegisterGroup:
    """Group of XSR registers"""
    def __init__ (self, name, regType):
        self.name = name
        self.regType = regType
        self.regs = []

    def regs (self):
        return regs

    def add ( self, data, rowNr, filt = None):
        r = self.regType(data, rowNr)
        if filt:
            for f in filt:
                if r.getField(f, None) != filt[f]:
                    return
        
        # and add to all registers list
        if r.implemented():
            if r.name() in r.allRegs():
                raise Exception("register with name %s already existing" % r.name())
            self.regs.append(r)
            r.allRegs()[r.name()] = r

    def empty(self):
        return len(self.regs) == 0
        
    def addressDefinitions(self):
        ret = []
        for r in self.regs:
            ret.append(r.addressDefinition())
        if len(ret) > 0:
            ret[0] = Indent + ('// %s addresses\n' % self.name) + ret[0]
        return ret

    def cppAddressDefinitions(self, stamp = None):
        ret = []
        for r in self.regs:
            ret.append(r.cppAddressDefinition(stamp))
        return ret

    def comment(self, indentLevel=1, newLine=True):
        c = Indent*indentLevel + '// ' + self.name
        if newLine: c+='\n'
        return c

    def declarations (self):
        ret = []
        for r in self.regs:
            d = r.declarations()
            if d: ret.append(d)
        if len(ret) > 0:
            ret.insert(0, self.comment(indentLevel = 0, newLine=False))
        return ret


################################################################################
# XSR REGISTER FILE
################################################################################
class XsrFileGdocsConfiguration:
    
    def __init__ (self, sheetId, mainSheet, download_filename):
        self.sheetId = sheetId
        self.mainSheet = mainSheet
        self.download_filename = download_filename

class XsrFileCsvConfiguration:
    def __init__ (self, mainFile, base_dir):
        self.regsFile = mainFile
        self.base_dir = base_dir


class XsrFileSheetConfiguration:
    def __init__ (self, min_cols = 1, csvConf = None, gdocsConf = None):
        self.csvConf = csvConf
        self.gdocsConf = gdocsConf
        self.min_cols = min_cols
    
            
class XsrFile:
    """XSR file"""
    def __init__ (self, stamp, regType, expHeader, conf, filt = None):
        self.stamp = stamp
        self.regType = regType
        self.groups = []

        if conf.csvConf != None:
            self.sheet = XsrSheet.getInstance(conf)
        elif conf.gdocsConf != None:
            self.sheet = XsrSheet.getInstance(conf)
        else:
            raise Exception("Either CSV or Google Spreadsheet configuration needs to be provided")
        
        self.loadSpreadSheet(conf, expHeader, filt)
        
    def loadSpreadSheet (self, conf, expHeader, filt):
        values = self.sheet.getMainSheet()

        header = values.pop(0)
        if header != expHeader:
            raise Exception ('header in sheet does not match this script implementation:\nrec: %s\nexp: %s' % (header, expHeader))

        max_cols = len(expHeader)

        currentGroup = None
        knownGroups = {}
        cnt = 2
        for row in values:
            if len(row) == 1 :
                groupName = row[0]
                if not groupName in knownGroups:
                    currentGroup = XsrRegisterGroup(groupName, self.regType)
                    self.groups.append(currentGroup)
                    knownGroups[groupName] = 1

            elif len(row) <= max_cols and len(row) >=conf.min_cols:
                if currentGroup != None :
                    currentGroup.add(row, cnt, filt)
                else:
                    raise Exception ("invalid XSR group for row %d: %s" % (cnt, row))
            elif len(row) > 0:
                sys.stderr.write("Invalid row %d with %s (len: %d, expected in range [%d,%d]). Will be ignored\n"
                                 % ( cnt, row, len(row), conf.min_cols, max_cols ))

            cnt+=1
        self.regType.checks()


    def addressDefinitions (self, extraBits = 0):
        """ returns Xsr addresses as enum (for Xsr_defines.vh)"""
        ads = []
        extraBits-=1
        
        for g in self.groups:
            ads+=g.addressDefinitions()
        w = self.regType.adWidth()
        if extraBits != 0:
            if w.isdigit():
                wmP = str(int(w) + extraBits)
            elif extraBits > 0:
                wmP = "%s+%d" % (w, extraBits) # add plus sign
            else:
                wmP = "%s%d" % (w, extraBits) # no need to add - sign, as number is already negative

        if len(ads) > 0:
            ads[-1] = re.sub(r',$','', ads[-1]) #remove trailing ,
        return 'typedef enum logic [%s:0]\n{\n' % (wmP) +  \
            ("\n".join(ads)) + '\n} %s_ad_t;' % self.stamp


    def cppAddressDefinitions (self, stamp = None):
        """ returns Xsr addresses as c++ definitions"""
        ads = []
        
        for g in self.groups:
            ads+=g.cppAddressDefinitions(stamp)
        return "\n".join(ads)


    
    def declarations (self):
        """returns declaration of signals"""
        c = []
        for g in self.groups:
            c+=g.declarations()

        # add declarations for latch reset signals and ph1 wdata if applicable
        hasLatch = False
        resets = {}
        for g in self.groups:
            for r in g.regs:
                if r.getField("resetVal") != None and r.latch():
                    resets[r.latchResetDeclaration()] = 1
                if r.latch():
                    hasLatch = True
                    
        for r in resets:
            c+= [ r ]
            
        if self.requiresLatchPH1() and hasLatch:
            c+= [ self.LatchPH1Declaration() ]
        c+='\n'
            
        return "\n".join(c)
    
    def requiresLatchPH1(self):
        return False
    
    def LatchPH1Declaration(self):
        return None
    
    def readAndDecodeProc(self, backslash=[False, False, False]):
        default = ''
        for g in self.groups:
            gr=''
            for r in g.regs:
                gr+=r.readAndDecodeProcDefault()
            if len(gr)>0:
                default+=g.comment(2)
                default+=gr

        cases = ''
        for g in self.groups:
            gr=''
            for r in g.regs:
                gr+=r.readAndDecodeCase()
            if len(gr)>0:
                cases+=g.comment(4)
                cases+=gr

        seq = ''
        for g in self.groups:
            gr = ''
            for r in g.regs:
                gr+=r.readAndDecodeSeq()
            if len(gr)>0:
                seq+=g.comment(0)
                seq+=gr

        if backslash[0]:
            default = re.sub( "\n", " \\\n", default)
        if backslash[1]:
            cases = re.sub( "\n", " \\\n", cases)
        if backslash[2]:
            seq = re.sub( "\n", " \\\n", seq) 

        return [default, cases, seq]

    def writeCombDefault(self, backslash=False): 
        ret=''
        regs = []
        for g in self.groups:
            gr = ''
            for r in g.regs:
                gr+=r.writeCombDefault()
            if len(gr)>0:
                ret+=g.comment(2)
                ret+=gr
        if backslash:
            ret = re.sub( "\n", " \\\n", ret)
        return ret

    def writeCoreComb(self, backslash=False):  
        ret=''
        # first shadows, then the others
        # write shadows use write_x_as_y ( x_next, wb_data)
        # instead of (x,wb_data) => in case x_next was updated before calling write_comb

        shadowing = [ True, False]
        for s in shadowing:
            if s:
                ret += '\n      // WRITE_COMB for regs that shadow some other reg\n'
            else:
                ret += '\n      // WRITE_COMB for the other regs\n'
            for g in self.groups:
                gr =''
                for r in g.regs:
                    if r.shadowing() == s:
                        gr+=r.writeCoreComb()
                if len(gr)>0:
                    ret+=g.comment(2)
                    ret+=gr

        if backslash:
            ret = re.sub( "\n", " \\\n", ret)
        return ret
   
    def writeSeq(self):
        ret=''
        for g in self.groups:
            gr = ''
            for r in g.regs:
                gr+=r.writeSeq()
            if len(gr) > 0:
                ret+=g.comment()
                ret+=gr
        return ret

    def maxAd(self):
        m = 0
        for g in self.groups:
            for r in g.regs:
                if r.conf['Address'] > m:
                    m = r.conf['Address']
        return m

    def log2MaxAd(self):
        return self.maxAd().bit_length()


