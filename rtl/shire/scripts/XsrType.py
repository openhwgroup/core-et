# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re

from XsrFileIO import Indent
from XsrSheet import XsrSheet
from ParseDefines import ParseDefines


class XsrType:
    """ xsr register type: construct with data from google sheets """
    XLEN = 64
    instances = {}

    def __new__(cls, *p):
        name = p[0]
        if name not in XsrType.instances:
            XsrType.instances[name] = super(XsrType, cls).__new__(cls)
        return XsrType.instances[name]

    def __init__ (self, name):
        # if already created, no need to create again
        if hasattr(self, 'name'):
            return
        
        self.name = name
        self.shortName = re.sub(r'_t$','', re.sub(r'^.sr_','', name))
        self.writeUsed = False
        self.writeNonReservedUsed = False
        self.writeShadowUsed = {}
        self.readShadowUsed = {}
        self.data = {}
        self.reserved = []
        self.generic = False
        self.RO = []
        self.ROValue = []
        
        try:
            v = XsrSheet.getInstance().get(name)
        except:
            self.generic = True
            print ("type %s not defined in spreadsheet.. no custom type conversions and definitions will be used" % name)
            return
        

        for row in v:
            if len(row) > 0 and row[0] != None:
                self.data[row[0]] = row[:0:-1] # remove first column, and reverse


        if not 'fields' in self.data:
            raise Exception("fields row not found for type %s" % name)

        #all fields in lowercase for simplicity
        self.data['fields'] = [ self.composeFieldName(f) for f in self.data['fields'] ]
        
        nrFields = len(self.fields())

        if not 'width' in self.data:
            raise Exception("width row not found for type %s" % name)

        if len(self.width()) < nrFields:
            raise Exception("width row length is less than nrFields for %s" % name)

        if 'RO' in self.data:
            self.RO = [ ro == 'YES' for ro in self.data['RO'] ]
        else:
            self.RO = [False] * nrFields

        # pad entries with no RO field set
        self.RO = ([False] * (nrFields - len(self.RO))) + self.RO

        if 'ROValue' in self.data:
            ROvals = self.data['ROValue']
        else:
            ROvals = []
            
        # pad entries with no ROvals
        ROvals = ([None] * (nrFields - len(ROvals))) + ROvals

        self.ROValue = [ None if v == '' else v for v in ROvals]
        
         
        # convert width to int, and check its sum is less than XLEN
        ctResolv = ParseDefines.getInstance()
        self.data['intWidth'] = list(map(lambda x: ctResolv.getValue(x), self.data['width']))
        if sum(self.data['intWidth']) > XsrType.XLEN:
               raise Exception("type %s, total width should be less than %d" % (self.name, XsrType.XLEN))

        # rename reserved fields
        resCnt = 0;
        for i,f in enumerate(self.fields()):
            [ res, resName] = self.isReserved(f)
            self.reserved.append(res)
            if res:
                if resName != None:
                    self.fields()[i] = '%s_reserved' % (resName )
                else:
                    self.fields()[i] = 'reserved%d' % resCnt
                    resCnt +=1

    @classmethod
    def knownType(XsrType, name):
        return name in XsrType.instances

    def composeFieldName(self, f):
        # to lower case, and replacing spaces and dashes with '_'
        f = re.sub(r' +','', f.lower())
        return re.sub(r'(-| )+','_', f)
    
    def conversionUsed(self):
        return self.writeUsed or len(self.writeShadowUsed) > 0 or len(self.readShadowUsed) > 0 or self.writeNonReservedUsed

    def isReserved(self, field_name):
        reservedNames = [ 'WIRI', 'WPRI', 'ZERO']
        if field_name.upper() in reservedNames:
            return [True, None]
        else:
            m = re.match(r'(.*)_zero$', field_name)
            if m != None:
                return [True, m.group(1)]
            else:
                return [False, None]

    def hasReserved(self):
        return sum(self.reserved) > 0

    def hasRO(self):
        return sum(self.RO) > 0

    def totalWidth(self):
            return '$bits(%s)' % self.name

    def hasField(self, field_name):
        return field_name in self.fields()

    def getItems(self, item, lsbFirst = True):
        if lsbFirst:
            return self.data[item]
        else:
            return self.data[item][::-1]

    def fields(self, lsbFirst=True):
        return self.getItems('fields', lsbFirst)

    def width(self, lsbFirst=True):
        return self.getItems('width', lsbFirst)

    def definition(self, indentLevel = 1):
        struct = [ Indent * indentLevel + 'typedef struct packed {']

        for f,w in zip(self.fields(False), self.width(False)):
            field = Indent * (indentLevel+1) + 'logic '
            if w.isdigit():
                w = int (w)
                if w != 1:
                    field += '[%d:0] ' % (w-1)
            else:
                field += '[%s-1:0] '% w
            field += "%s;" % f
            struct.append(field)

        struct.append(Indent*indentLevel + '} ' + self.name + ';')
        return '\n'.join(struct)

    def wdata(self, def_v = "wb_wdata"):
        w = self.totalWidth()
        if w==64:
            d = def_v
        elif isinstance(w, int):
            d = def_v + ("[%d:0]" % (w-1))
        else:
            d = def_v + ('[%s-1:0]' % w)
        
        if not self.hasReserved() and not self.hasRO():
            return d
        else:
            self.writeUsed = True
            return 'write_%s(%s)'%(self.shortName, d)

    def writeNonReservedName(self):
        if self.hasReserved():
            self.writeNonReservedUsed = True
            return 'write_%s_non_reserved' % self.shortName
        else:
            return "%s'" % self.name

    def writeNonReserved(self, indentLevel):
        w = [ 'function automatic %s %s;' % (self.name, self.writeNonReservedName()),
              'input %s d;' % self.name,
              '%s ret;' % self.name,
              'begin']

        for f,r,ro,rov in zip( self.fields(), self.reserved, self.RO, self.ROValue):
            if r: 
                w.append( Indent + "ret.%s = '0; " % f )
            else:
                w.append( Indent + "ret.%s = d.%s;" % (f,f))

        w += [ Indent + 'return ret;',
              'end',
              'endfunction\n']

        w = [ Indent*indentLevel + l for l in w]
        return '\n'.join(w)        


    def write(self, indentLevel = 1):
        if not self.hasReserved() and not self.hasRO():
            return ''
        
        w = [ 'function automatic %s write_%s;' % (self.name, self.shortName),
              'input [$bits(%s)-1:0] d;' % self.name,
              '%s dd;' % self.name,
              '%s ret;' % self.name,
              'begin',
              Indent + "dd = %s'(d);" % self.name]

        for f,r,ro,rov in zip( self.fields(), self.reserved, self.RO, self.ROValue):
            if r: 
                w.append( Indent + "ret.%s = '0; " % f )
            elif not ro:
                w.append( Indent + "ret.%s = dd.%s;" % (f,f))
            elif rov != None:
                w.append( Indent + "ret.%s = %s; // RO field" % (f, rov))
            else:
                w.append( Indent + "ret.%s = 'x; // RO field... needs to be set manually if used" % f)

        w += [ Indent + 'return ret;',
              'end',
              'endfunction\n']

        w = [ Indent*indentLevel + l for l in w]
        return '\n'.join(w)

    def wdataShadow(self, srcType):
        self.writeShadowUsed[srcType.name] = srcType
        return 'write_%s_as_%s' % (self.shortName, srcType.shortName)

    def rdataShadow(self, srcType):
        self.readShadowUsed[srcType.name] = srcType
        return 'read_%s_as_%s' % (self.shortName, srcType.shortName)

    def writeShadow(self, srcType, indentLevel = 1):
        """ write non-reserved fields of register srcType into the same fields of this register"""
        fname = 'write_%s_as_%s' % (self.shortName, srcType.shortName)
        w = ['function automatic %s %s;' % (self.name, fname),
             'input %s orig;' % self.name,
             'input %s d;' % (srcType.name),
             '%s ret;' % self.name,
             'begin',
             Indent + "ret = orig;" ]

        for f,r in zip( srcType.fields(), srcType.reserved):
            if not r: 
                if not self.hasField(f):
                    raise Exception("%s has no field %s required in %s:\n %s" % (self.name, f, fname, '\n'.join(w)))
                else:
                    idx = self.fields().index(f)
                    if self.reserved[idx]:
                        raise Exception("%s has field %s required in %s, but is reserved" % (self.name, f, fname))
                    if not self.RO[idx]:
                        w.append( Indent + "ret.%s = d.%s;" % (f,f))
                    else:
                        w.append( Indent + " // ret.%s = RO;" % f)

        w += [ Indent + 'return ret;',
               'end',
               'endfunction']

        w = [ Indent*indentLevel + l for l in w]
        return '\n'.join(w)
    
    def readShadow(self, dstType, indentLevel = 1):
        fname = 'read_%s_as_%s' % (self.shortName, dstType.shortName)
        w = ['function automatic %s %s;' % (dstType.name, fname),
             'input %s orig;' % self.name,
             '%s ret;' % dstType.name,
             'begin']
        if dstType.hasReserved():
            w.append( Indent + "ret = '0;")

        for f,r in zip( dstType.fields(), dstType.reserved):
            if not r: 
                if not self.hasField(f):
                    raise Exception("%s has no field %s required in %s" % (self.name, f, fname))
                else:
                    w.append( Indent + "ret.%s = orig.%s;" % (f,f))

        w += [ Indent + 'return ret;',
               'end',
               'endfunction']

        w = [ Indent*indentLevel + l for l in w]
        return '\n'.join(w)
    
    @classmethod
    def allDefinitions(XsrType, indentLevel = 1):
        defs = []
        keys = sorted(XsrType.instances.keys())
        for k in keys:
            t = XsrType.instances[k]
            if not t.generic:
                defs.append(t.definition(indentLevel))
        return '\n\n'.join(defs)

    @classmethod
    def allConversions(XsrType, indentLevel = 1):
        conv = []
        keys = sorted(XsrType.instances.keys())
        for k in keys:
            t = XsrType.instances[k]
            if t.conversionUsed():
                conv.append(Indent * indentLevel + '//conversion functions for %s' % t.name)
                if t.writeUsed:
                    conv.append( t.write(indentLevel) )
                if t.writeNonReservedUsed:
                    conv.append(t.writeNonReserved(indentLevel))
                for src in sorted(t.writeShadowUsed.values(), key = lambda x : x.shortName):
                    conv.append('\n'+ t.writeShadow(src, indentLevel))
                for dst in sorted(t.readShadowUsed.values(),  key = lambda x : x.shortName):
                    conv.append('\n'+ t.readShadow(dst) + '\n')
        return '\n'.join(conv)

    @classmethod
    def resetConversionUsed(XsrType, indentLevel = 1):
        conv = []
        keys = sorted(XsrType.instances.keys())
        for k in keys:
            t = XsrType.instances[k]
            t.writeUsed = False
            t.writeNonReservedUsed = False
            t.writeShadowUsed = {}
            t.readShadowUsed = {}
