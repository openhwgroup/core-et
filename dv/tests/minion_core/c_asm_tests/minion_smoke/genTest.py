#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import argparse
import configparser
import random
import enum
import itertools
import functools
import re

class Parameter(enum.Enum):
    """ Enum to describe parameters of an instruction"""
    INT = 0
    FP = 1
    MASK = 2
    IMM3 = 3
    IMM8 = 4
    ONEHOT8 = 5

class RF:
    """ class to get next random RF operands"""
    def __init__ (self, prefix, start, stop, uniform, geom_p):
        self.__regs = [ f"{prefix}{i}" for i in range(start, stop)]
        count = len(self.__regs)
        self.__indexes = range(count)
        self.__uniform = uniform
        # create weights
        assert geom_p >0  and geom_p <=1
        self.__weights = [1 - (1-geom_p)**(i+1) for i in range(count)]
        # and initial shuffle
        random.shuffle(self.__regs)

        # list of functions to call when a reg changes
        self.__cb = []

    def next(self, writing, forced = None):
        if forced == None:
            if random.randint(0,99) < self.__uniform:
                sel = random.choice(self.__indexes)
            else:
                sel = random.choices(self.__indexes, cum_weights = self.__weights)[0]
        else:
            sel = self.__regs.index(forced)

        r = self.__regs.pop(sel)
        self.__regs = [r] + self.__regs

        if writing:
            for i in self.__cb:
                i(r)
        return r
        
    def addCb(self, f):
        self.__cb.append(f)

class MaskRF(RF):
    """ class to get next random mask register... similar to RF, but if writing, give prio to m0"""
    def __init__ (self, uniform, geom_p, m0):
        super().__init__("m", 0, 8, uniform, geom_p)
        self.__m0 = m0

    def next(self, writing):
        if writing and random.randint(0,99) < self.__m0:
            return super().next(writing, forced = "m0")
        else:
            return super().next(writing)

class Snippet:
    """ code generator base class"""
    TDATA2_ALLOWED=False
    COMMENT_COLUMN=60
    def __init__(self, name, test):
        self._name = name
        w = test.config.getint("probs", "weight_" + name)
        test.generators.append(self)
        test.weights.append(w)
        self._getId = test.getId
    
    def code(self):
        """ Function to implement for derived classes"""
        raise NotImplementedError()

    def getParam(self, paramType, writing):
        if paramType == Parameter.INT:
            return Test.intRF.next(writing)
        elif paramType == Parameter.FP:
            return Test.fpRF.next(writing)
        elif paramType == Parameter.MASK:
            return Test.maskRF.next(writing)
        elif paramType == Parameter.IMM3:
            return str(self.getImm(3))
        elif paramType == Parameter.IMM8:
            return str(self.getImm(8))
        elif paramType == Parameter.ONEHOT8:
            return str(1 << random.randint(0,7))
        else:
            return str(paramType)

    def getImm(self, nbits, signed = False, symmetric = False):
        if not signed:
            r = range(0, 1<<nbits)
        else:
            if symmetric:
                r = range( -(1<<(nbits-1)) + 1, 1<<(nbits-1))
            else:
                r = range( -(1<<(nbits-1)), 1<<(nbits-1))
        return random.choice(r)
        
    def instruction(self, name, *args, writing = True, comment = None):
        payload = [self.getParam(t, i == 0 and writing) for i,t in enumerate(args)]
        line = name + " " + ", ".join(payload)
        sId = self._getId()
        cmt_prefix = f"{sId}: {self._name}"
        comment = cmt_prefix if not comment else cmt_prefix + " " + comment
        pad = Snippet.COMMENT_COLUMN - len(line)
        assert pad > 0, "increment indent for comment"
        return line + " " * pad + "# " + comment
    
    def _write_tdata2(self, reg, comment):
        """tdata2 is only allowed to be written in m-mode... since this test may run in u or s mode,
        copy the data to be written in validation3, and do an ecall => tdata2 will be written in the trap handler"""
        if Snippet.TDATA2_ALLOWED:
            return self.instruction("csrw", "tdata2", reg, writing = False, comment = comment)
        else:
            return (self.instruction("csrw", "validation3", reg, writing = False, comment = comment) 
                    + "\n" + self.instruction("ecall", writing = False, comment = comment) )
    
################################################################################
# specializations of code generation base class
################################################################################
class IntArith(Snippet):
    insns = ("add", "sub", "srl", "sra","sll",
             "addw", "subw", "srlw", "sraw","sllw", 
             "and", "or", "xor", "slt", "sltu")
    parameters = (Parameter.INT, Parameter.INT, Parameter.INT)
    def __init__(self, test):
        super().__init__("int_arith", test)

    def code(self):
        return self.instruction(random.choice(IntArith.insns), *IntArith.parameters)

class Mem(Snippet):
    avAds = {} # key: int registers, value = a valid address they have in storage
    debug_csr = 0

    #xcpts
    PMA_XCPT = 0
    DEBUG_XCPT = 1
    NO_XCPT = 2

    MAX_OFFSET = (1<<11) -1
    
    def __init__(self, test, name, loads, stores, paramType, canCache):
        super().__init__(name, test)
        self._scratch_size = test._scratch_size
        self._reuse = test.config.getint("mem", "reuse_ad")

        self.__xcpt_weights = list(itertools.accumulate([test.config.getint("mem", "pma_xcpt"), test.config.getint("mem", "debug_xcpt")]))
        assert self.__xcpt_weights[-1] <= 100, "sum % of xcpts cannot be larger than 100"
        self.__xcpt_weights.append(100)
        Test.intRF.addCb(self.intRegUpdated)
        
        self.__insns = loads + stores
        self.__loads = loads
        self.__paramType = paramType
        self._canCache = canCache


    def xcpt(self):
        return random.choices( (Mem.PMA_XCPT, Mem.DEBUG_XCPT, Mem.NO_XCPT), cum_weights = self.__xcpt_weights)[0]

    def reuse(self, xcpt):
        if len(Mem.avAds) == 0 :
            return None,None,None

        if xcpt == Mem.PMA_XCPT:
            # if we want a PMA exception, we can only reuse if ad + MAX_OFFSET > scratch_size
            # and valid offsets are the ones where ad + offset > scratch_size
            valid  = [ (r,a) for r,a in Mem.avAds.items() if  a + Mem.MAX_OFFSET >= self._scratch_size]
            if len(valid) > 0:
                reg,ad = random.choice(valid)
                offset = random.choice(range ( self._scratch_size - ad, Mem.MAX_OFFSET + 1))
                return reg, ad, offset
            else:
                return None,None,None

        elif xcpt == Mem.DEBUG_XCPT:
            # try to reuse an address that can match the debug register
            valid = [ (r,a) for r,a in Mem.avAds.items() if (a  - Mem.MAX_OFFSET <= Mem.debug_csr and 
                                                             Mem.debug_csr <= a + Mem.MAX_OFFSET) ]
            if len(valid) > 0:
                reg,ad = random.choice(valid)
                offset = Mem.debug_csr - ad
                return reg, ad, offset

            valid = list(Mem.avAds.items()) # any of the existing addresses will do, but we'll have to write tdata2
        else:
            # no xcpts => any address that won't cause a PMA exception
            valid  = [ (r,a) for r,a in Mem.avAds.items() if  a - Mem.MAX_OFFSET < self._scratch_size - 32]
            if len(valid) == 0:
                return None,None,None

        #valid offsets of 12  bits, and careful to to go beyong the scratch area
        reg,ad = random.choice(valid)
        offset = random.randint(max(-ad, -Mem.MAX_OFFSET), min(self._scratch_size -32 - ad, Mem.MAX_OFFSET))
        return reg, ad, offset 

    def code(self):
        c = []
        xcpt = self.xcpt()
        reg = None
        # try to reuse any of the existing addresses
        if random.randint(0, 99) < self._reuse:
            reg,ad,offset = self.reuse(xcpt)

        if reg == None: # not reusing, choose a new random address and offset
            if xcpt == Mem.PMA_XCPT: # choose so that: ad + offset > scratch_size
                ad = random.randint(self._scratch_size - Mem.MAX_OFFSET, self._scratch_size)
                offset = random.choice(range ( self._scratch_size - ad, Mem.MAX_OFFSET + 1 ))
            else:
                ad = random.randint(0, self._scratch_size - 32)
                offset = self.getImm(12, signed = True, symmetric = True)
                ad-=offset

            # load in register 'reg' with and address
            reg = self.getParam(Parameter.INT, True)
            c.append(self.instruction("la", reg,f"__scratch_area {ad:+#x}", comment = f"(prepare address)"))

        # if xcpt == debug, set debug csr
        if xcpt == Mem.DEBUG_XCPT and Mem.debug_csr != ad + offset:
            Mem.debug_csr = ad + offset
            c+= [
                self.instruction("addi", reg, reg, offset, writing = False, comment = f"(prepare debug trigger)"),
                self._write_tdata2(reg,comment = f"(prepare debug trigger)"),
                self.instruction("addi", reg, reg, -offset, writing = False, comment = f"(prepare debug trigger)")
                ]
        
        # clear debug csr if we don't want a debug scpt
        if xcpt != Mem.DEBUG_XCPT and Mem.debug_csr == ad + offset:
            Mem.debug_csr = 0
            c.append(self._write_tdata2("x0", comment = f"(clear debug trigger)"))

        if self._canCache():
            Mem.avAds[reg] = ad

        # and finally, the Memory access
        xcpt_comment = ( " (with PMA xcpt)", " (with debug xcpt)", None)

        c.append(self.memInstruction(f"{offset:#x}({reg})", comment = xcpt_comment[xcpt]))

        assert offset + ad >= 0 
        assert offset + ad <= self._scratch_size -32 or xcpt != Mem.NO_XCPT
        assert offset <= Mem.MAX_OFFSET and offset >= -Mem.MAX_OFFSET
        return "\n".join(c)

    
    def memInstruction(self, address_params, comment):
        opcode = random.choice(self.__insns)
        writing =  opcode in self.__loads # set writing to 1 for loads
        return self.instruction(opcode, self.__paramType, address_params, writing = writing, comment = comment)

    def intRegUpdated(self, reg):
        #if register has been overwritten => no longer having a valid address anymore
        if reg in Mem.avAds:
            del Mem.avAds[reg]


class IntDiv(Snippet):
    insns = ("mul", "mulh", "mulhsu", "mulhu",
             "div", "divu", "rem", "remu",
             "mulw", "divw", "remw", "remuw")
    parameters = (Parameter.INT, Parameter.INT, Parameter.INT)
    def __init__(self, test):
        super().__init__("int_div", test)
    def code(self):
        return self.instruction(random.choice(IntDiv.insns), *IntDiv.parameters)

class FPArith(Snippet):
    insns = (("fadd.ps",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fsub.ps",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmul.ps",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmin.ps",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmax.ps",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmadd.ps",  Parameter.FP, Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmsub.ps",  Parameter.FP, Parameter.FP, Parameter.FP, Parameter.FP),
             ("fnmadd.ps", Parameter.FP, Parameter.FP, Parameter.FP, Parameter.FP),
             ("fnmsub.ps", Parameter.FP, Parameter.FP, Parameter.FP, Parameter.FP),
             ("fadd.pi",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fsub.pi",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmul.pi",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmin.pi",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("fmax.pi",   Parameter.FP, Parameter.FP, Parameter.FP),
             ("flt.pi",    Parameter.FP, Parameter.FP, Parameter.FP)


            )

    def __init__(self, test):
        super().__init__("fp_arith", test)
    def code(self):
        return self.instruction( *random.choice(FPArith.insns) )

class Trans(Snippet):
    insns = ("fexp.ps", "flog.ps", "frcp.ps")
    def __init__(self, test):
        super().__init__("trans", test)
    def code(self):
        return self.instruction( random.choice(Trans.insns), Parameter.FP, Parameter.FP)

class Fp2Int(Snippet):
    insns = (("fmv.x.s",    Parameter.INT, Parameter.FP), 
              ("fclass.s",  Parameter.INT, Parameter.FP),
              ("fcvt.w.s",  Parameter.INT, Parameter.FP),
              ("fcvt.wu.s", Parameter.INT, Parameter.FP),
              ("flt.s",     Parameter.INT, Parameter.FP,Parameter.FP),
              ("fle.s",     Parameter.INT, Parameter.FP, Parameter.FP),
              ("fmvz.x.ps", Parameter.INT, Parameter.FP, Parameter.IMM3),
              ("fmvs.x.ps", Parameter.INT, Parameter.FP, Parameter.IMM3)
           )
    def __init__(self, test):
        super().__init__("fp2int", test)
    def code(self):
        return self.instruction( *random.choice(Fp2Int.insns) )

class Int2fp(Snippet):
    insns = ("fmv.s.x", "fcvt.s.w", "fcvt.s.wu", "fbcx.ps")
    parameters = (Parameter.FP, Parameter.INT)
    def __init__(self, test):
        super().__init__("int2fp", test)
    def code(self):
        return self.instruction(random.choice(Int2fp.insns), *Int2fp.parameters)

class GscProgress(Snippet):
    def __init__(self, test):
        super().__init__("gsc_progress", test)
    def code(self):
        return self.instruction("csrwi", "gsc_progress", Parameter.IMM3, writing = False )

class Evict(Snippet):
    def __init__(self, test):
        super().__init__("evict", test)
    def code(self):
        reg = self.getParam(Parameter.INT, True)
        return (self.instruction("li", reg, " (1<<58) | 15", comment = "evict (prepare conf val)") + "\n" +
                self.instruction("csrw", "evict_sw", reg))
        
class Illegal(Snippet):
    insns = ( (".word", 0x0), #illegal at ID
              ("csrwi", "dscratch0", "0") # illegal at WB
          )
    def __init__(self, test):
        super().__init__("illegal", test)
    def code(self):
        return self.instruction( *random.choice(Illegal.insns) )

class Branch(Snippet):
    def __init__(self, test):
        super().__init__("branch", test)
        self.__unconditional = test.config.getint("branch", "unconditional")
        self.__condRange = self.parseRange(test, "conditional_length_range")
        self.__uncondRange = self.parseRange(test, "unconditional_length_range")
        self.__current = {}
        self.__available = []

    def parseRange(self, test, optName):
        opt = test.config.get("branch", optName)
        optL = [int(x.strip()) for x in opt.split(',')]
        assert len(optL) == 2 and optL[0]>0 and optL[0]<=optL[1]
        return range(optL[0], optL[1] + 1)
        
    def code(self):
        #note that all branches are forward (to avoid tests not moving forward)
        label = self.nextLabel()
        if random.randint(0, 99) < self.__unconditional:
            code = self.instruction("j", f"{label}f", writing = False)
            length = random.choice(self.__uncondRange)
        else:
            code = self.instruction("blt", Parameter.INT, Parameter.INT, f"{label}f", writing = False)
            length = random.choice(self.__condRange)

        assert label not in self.__current
        self.__current[label] = length
        return code
            

    def nextLabel(self):
        if len(self.__available) == 0:
            return len(self.__current) + 1
        else:
            return self.__available.pop(-1)

    def step(self):
        # check if we have to put the label
        removed = []
        for label in self.__current:
            self.__current[label]-=1
            if self.__current[label] == 0:
                removed.append(label)
        if len(removed) == 0:
            return []
        else:
            for r in removed:
                del self.__current[r]
            self.__available+=removed
            return [f"{i}:" for i in sorted(removed)]

    def empty(self):
        return len(self.__current) == 0

    def end(self):
        return [f"{i}:" for i in sorted(self.__current)]
        self.__current = {}
        self.__available = []

class Mask(Snippet):
    insns = ( ("maskand",  Parameter.MASK, Parameter.MASK, Parameter.MASK),
              ("maskor",   Parameter.MASK, Parameter.MASK, Parameter.MASK),
              ("maskxor",  Parameter.MASK, Parameter.MASK, Parameter.MASK),
              ("masknot",  Parameter.MASK, Parameter.MASK),
              ("mov.m.x",  Parameter.MASK, Parameter.INT, Parameter.IMM8),
              ("mov.m.x",  Parameter.MASK, "x0", "0"),
              ("mov.m.x",  Parameter.MASK, "x0", "0xFF"),
              ("mov.m.x",  Parameter.MASK, Parameter.INT, Parameter.ONEHOT8),
              ("flem.ps",  Parameter.MASK, Parameter.FP, Parameter.FP),
              ("fltm.ps",  Parameter.MASK, Parameter.FP, Parameter.FP),
              ("fsetm.pi", Parameter.MASK, Parameter.FP)
    )
    def __init__(self, test):
        super().__init__("mask", test)
        self._force_nonzero_m0 = test.config.getboolean("bugs", "force_nonzero_m0")

    def code(self):
        if self._force_nonzero_m0 == False:
            return self.instruction( *random.choice(Mask.insns) )
        else:
            m = Test.maskRF.next(writing = True)
            insn = random.choice(Mask.insns)
            c = self.instruction( insn[0], m, *insn[2:])
            if m == "m0":
                c+="\n" + self.instruction("mov.m.x", m, "x0", 1<< random.randint(0,7), comment = f"m0 forced to non-zero")
            return c

class Mask2int(Snippet):
    insns = ( ("mova.x.m", Parameter.INT),
              ("maskpopc", Parameter.INT, Parameter.MASK),
              ("maskpopcz", Parameter.INT, Parameter.MASK)
        )
    def __init__(self, test):
        super().__init__("mask2int", test)
    def code(self):
        return self.instruction( *random.choice(Mask2int.insns) )

class Fcsr(Snippet):
    insns = ( {"code": ("csrwi", "frm"),   "usesReg": False, "rm": 0,    "flags": False},
              {"code": ("csrw", "fflags"), "usesReg": True,  "rm": None, "flags": True},
              {"code": ("csrw", "fcsr"),   "usesReg": True,  "rm": 5,    "flags": True},
          )
    def __init__(self, test):
        super().__init__("fcsr", test)
        self.__invalidRM = test.config.getint("probs", "prob_invalid_rm")

    def code(self):
        c = []
        i = random.choice(Fcsr.insns)

        # compute value to write
        val = 0
        if i["rm"] != None:
            val |=  self.getRM() << i["rm"]
        if i["flags"]:
            val |= random.getrandbits(5) | ( 1 << random.randint(0,1) << 31)
        
        # and add code:
        if i["usesReg"]:
            r = Test.intRF.next(writing  = True)
            return (self.instruction("li", r, f"{val:#x}") + "\n" +
                    self.instruction( *i["code"], r))
        else:
            return self.instruction( *i["code"], val)

    def getRM(self):
        if random.randint(0,99) < self.__invalidRM:
            return random.randint(5,7)
        else: 
            return random.randint(0,4)

class IntMem(Mem):
    loads = ("ld", "lw", "lh", "lb", "lwu", "lhu", "lbu")
    stores = ("sd", "sw", "sh", "sb")
    def __init__(self, test, canCache):
        super().__init__(test, "int_mem", IntMem.loads, IntMem.stores, Parameter.INT, canCache)

class FPMem(Mem):
    loads_fxx_ps = ("flw.ps",)
    stores_fxx_ps = ("fsw.ps",)
    loads = ("flw", "flq2", "fbc.ps")
    stores = ("fsw", "fsq2")
    def __init__(self, test, canCache):
        # RTLMIN-6474 workarround
        enable_fxx_ps_inst = test.config.getboolean("bugs","enable_fxx_ps_inst")
        loads = FPMem.loads + FPMem.loads_fxx_ps if enable_fxx_ps_inst else FPMem.loads
        stores = FPMem.stores + FPMem.stores_fxx_ps if enable_fxx_ps_inst else FPMem.stores
        super().__init__(test, "fp_mem", loads, stores, Parameter.FP, canCache)

class GSC(Mem):
    loads = ("fgw.ps", "fgh.ps", "fgb.ps")
    stores = ("fscw.ps", "fsch.ps", "fscb.ps")

    def __init__(self, test, canCache):
        super().__init__(test, "gsc", GSC.loads, GSC.stores, Parameter.FP, canCache)
        Test.fpRF.addCb(self.fpRegUpdated)
        self._avOffsets = {} # registers holding a valid offset
        self._loadOffsets = [] # all the offsets to load

    def hasPMAxcpt(self, ad, offsets):
        for i in offsets:
            if i + ad + 3 >=self._scratch_size:
                return True
        return False

    def reuse(self, xcpt):
        pma = (xcpt == Mem.PMA_XCPT)
        valid = []
        # find valid ad+offset combinations
        for reg,ad in Mem.avAds.items():
            for reg_offsets, o in self._avOffsets.items():
                if self.hasPMAxcpt(ad,o) == pma and all( i + ad >= 0 for i in o):
                    valid.append( (reg, ad, reg_offsets, o))
        if len(valid) == 0:
            return None, None, None, None
        else:
            return random.choice(valid)
        

    def fpRegUpdated(self, reg):
        if reg in self._avOffsets:
            del self._avOffsets[reg]

    def code(self):
        c = []
        comment = None
        xcpt = self.xcpt()

        # try to reuse any of the existing addresses
        reg = None
        offsets_reg = None
        if random.randint(0, 99) < self._reuse:
            reg,ad, offsets_reg, offsets = self.reuse(xcpt)

        if reg == None:
            reg = self.getParam(Parameter.INT, True)
            offsets_reg = self.getParam(Parameter.FP, True)
            #randomize address
            ad = random.randint(0, self._scratch_size - 32)

            # randomize offsets
            offsets = [ random.randint(-ad, self._scratch_size - 32 -ad) for i in range(8) ]

            # force PMA xcpt
            if xcpt == Mem.PMA_XCPT:
                lane = random.randint(0,7)
                offsets[0] =  random.randint(self._scratch_size - ad , 2*self._scratch_size)

            # write the offsets
            
            c+= [ self.instruction("la", reg, f"__gsc_offsets {len(self._loadOffsets)*32:+#x}",
                                   writing = False, comment  = f"(ad where to load offsets from)"),
                  self.instruction("flq2", offsets_reg, f"0({reg})", comment = f"(load offsets)") 
              ]
            self._loadOffsets.append((self._getId(), offsets))

            # write the address
            if xcpt != Mem.DEBUG_XCPT: # for debug, it is written later
                c.append(self.instruction("la", reg,f"__scratch_area {ad:+#x}", comment = f"(prepare address)"))



        # setup debug xcpt if needed
        if xcpt == Mem.DEBUG_XCPT:
            lane = random.randint(0,7)
            comment = f"(debug xcpt lane {lane})"
            c+=[ self.instruction("la", reg,f"__scratch_area {ad:+#x} {offsets[lane]:+#}", comment = f"(prepare debug trigger)"),
                 self._write_tdata2(reg,comment = f"(prepare debug trigger)"),                 
                 self.instruction("la", reg,f"__scratch_area {ad:+#x}", comment = f"(prepare address)")
            ]
        elif xcpt == Mem.PMA_XCPT:
            for i in range(8):
                if ad + offsets[i] >= self._scratch_size:
                    comment = f"(PMA xcpt lane {i})"
                    break

        if self._canCache():
            Mem.avAds[reg] = ad
            self._avOffsets[offsets_reg] = offsets

        # and finally, the instruction
        c.append(self.memInstruction(f"{offsets_reg}, {reg}", comment = comment))
        #return "\n".join(c)
        return "\n"

    def printOffsets(self):
        offsets= ['# GSC offsets section',
                  '.section .gsc_offsets, "aw"',
                  '__gsc_offsets:']
        for o in self._loadOffsets:
            gscId, vals = o
            offsets.append( f".word {vals[0] & 0xFFFFFFFF: #x} # offsets for gsc (id={gscId})" )
            offsets +=[ f".word {i & 0xFFFFFFFF: #x}" for i in vals[1:]]
        return offsets
################################################################################
# Main class
################################################################################
class Test:
    def __init__(self, configFile, tdata2_allowed):
        Snippet.TDATA2_ALLOWED = tdata2_allowed
        #init configuration file
        self.config = configparser.ConfigParser()
        self.config.read(configFile)
        self.__count = self.config.getint("general", "length")
        self._scratch_size = self.__getScratchSize( self.config.get("general", "scratch_size") )
        # init register allocation
        uniform = self.config.getint("rf", "uniform")
        geom_p = self.config.getfloat("rf", "geometric_param")
        m0writes= self.config.getfloat("rf", "m0_writes")

        Test.intRF = RF("x", 1, 32, uniform, geom_p) #skipping x0
        Test.fpRF = RF("f", 0 ,32, uniform, geom_p)
        Test.maskRF = MaskRF(uniform, geom_p, m0writes)


        # init snippets
        self.generators = []
        self.weights = []
        self.__branches = Branch(self)
        IntArith(self)
        IntMem(self, self.__branches.empty)
        IntDiv(self)
        FPArith(self)
        FPMem(self, self.__branches.empty)
        Trans(self)
        self.__gsc = GSC(self, self.__branches.empty)
        Fp2Int(self)
        Int2fp(self)
        Illegal(self)
        Mask(self)
        Mask2int(self)
        Evict(self)
        GscProgress(self)
        Fcsr(self)


    def createAsm(self, outfile):
        num_threads = 2
        code=['\n\n# START OF AUTOMATICALLY GENERATED TEST',
              '.section .test, "ax"',
              'test_start:',
              'csrr t0, hartid',
              'bne t0, zero, __test_thread1\n']

        for self.__threadId in range(num_threads):
            # clean 'cached' addresses in offsets since we are changing thread
            Mem.avAds = {}
            self.__gsc._avOffsets = {}

            # generate snippets
            code.append(f"__test_thread{self.__threadId}:")
            for self.__snippetId, i in enumerate(random.choices(self.generators, self.weights, k=self.__count)):
                code.append(i.code())
                code+=self.__branches.step() # label of branches after 'len' snippets
  
            # labels for branches than don't have destination
            code+=self.__branches.end() #add labels of branches before ending the code generation

            if self.__threadId < num_threads -1:
                code+=[ f"j __test_end\n",
                        ".p2align  6"
                ]

        # end of test
        code+=["__test_end:",
               "# END OF AUTOMATICALLY GENERATED TEST\n"]

        # and gsc offsets to load
        offsets = self.__gsc.printOffsets()
        outfile.write("\n".join(offsets))
        outfile.write("\n".join(code))

    def getId(self):
        return self.__snippetId + self.__count * self.__threadId

    def createDataRaw(self, outfile):
        d=random.getrandbits(8*self._scratch_size).to_bytes(self._scratch_size,'big')
        outfile.write(d)

    def __getScratchSize(self, size):
        # get size in bytes from a string such as '1M'
        factor = {'M': 1<<20, 'K': 1 << 10, '': 1 }
        r = re.compile(r'(\d*\.?\d+)(.?)$')
        m = r.match(size)

        if not m:
            raise Exception(f"Bad format for data.raw size {size}. Use something like 1M")
        if m.group(2) not in factor:
            raise Exception(f"Bad format for data.raw size {size}. Use something like 1M")

        return int(float(m.group(1)) * factor[m.group(2)])

################################################################################
# deal with random seed
################################################################################
def init_seed(cmdline):
    seed = 0
    if cmdline != None:
        seed = cmdline
    else:
        #read from ../SEED
        try:
            with open("../SEED", "r") as f:
                seed= int(f.readline())
        except:
            pass

    random.seed(seed)
    print(f"Using random seed {seed}")

################################################################################
# entry point
################################################################################
if __name__ == "__main__":
    # parse command line options
    parser = argparse.ArgumentParser("Creates scoreboard stress test")
    parser.add_argument("--seed", help="random seed", type=int)
    parser.add_argument("--conf", help="config file (genTest.conf by default)", default="genTest.conf", type=str)
    parser.add_argument("--tdata2-not-allowed", help="use ecall instead of writing tdata2 directly, because of privilege mode", action='store_false')
    parser.add_argument("--output", help="output filename", type=argparse.FileType("w"))
    parser.add_argument("--dataraw", help="output filename", type=argparse.FileType("wb"))
    args = parser.parse_args()

    #init random seed
    init_seed(args.seed)

    # and create test
    gen = Test(args.conf, args.tdata2_not_allowed)
    if args.output:
        gen.createAsm(args.output)

    if args.dataraw:
        gen.createDataRaw(args.dataraw)

    if not args.dataraw and not args.output:
        raise Exception("specify at least one of --output or --dataraw cmdline options")
