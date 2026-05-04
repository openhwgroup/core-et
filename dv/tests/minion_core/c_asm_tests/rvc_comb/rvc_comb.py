#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import argparse
import random
import itertools
import json

class Operand:
    valid_kinds = [ "reg", "short_reg", "imm"]
    def __init__(self, kind, **kwargs):
        """ kind: one of reg, short_reg, imm, fixed
        **kwargs: extra options.

        Extra options are:
        for regs:
          forbidden (default []) => registers that won't be used. 
        only for imm:
          size (required)
          signed (default True)
          offset (default True) (some instructions have LSB fixed to 0)
          inc (increment combinations by..., default=1)
          mask (default none) => ands imm with mask if specified
          forbidden: values that won't be used (before offset)
        only for fixed:
           value (required)
        """

        # generate combinations
        forbidden = kwargs['forbidden'] if 'forbidden' in kwargs else []
        if kind == "fixed":
            self.__values = [str(kwargs['value'])]
        elif kind == "reg":
            self.__values = [ f'x{i}' for i in range(32) if i not in forbidden ]
        elif kind == "short_reg":
            self.__values = [ f'x{i}' for i in range(8,16) if i not in forbidden]
        elif kind == "imm":
            signed = kwargs['signed'] if 'signed' in kwargs else True
            size = kwargs['size']
            offset = kwargs['offset'] if 'offset' in kwargs else 0
            inc = kwargs['inc'] if 'inc' in kwargs else 1
            mask  = f" & {kwargs['mask']}" if 'mask' in kwargs and kwargs['mask'] != None  else ""
            if signed == False:
                self.__values = [ f"{i << offset}{mask}" for i in range(0, 1<<size, inc) if i not in forbidden]
            else:
                self.__values = [ f"{i << offset}{mask}" for i in range(-(1<<(size-1)), 1 << (size-1), inc) if i not in forbidden]
        else:
            raise Exception(f"Invalid operand kind {kind}")

    def __iter__(self):
        return iter(self.__values)

class Immediate(Operand):
    def __init__(self, size, inc = 1, signed = True, offset=0, allow_zero = True, mask = None):
        forbidden = [] if allow_zero else [0]
        super().__init__("imm", size=size, signed = signed, offset=offset, inc=inc, forbidden = forbidden, mask = mask)
        
class Fixed(Operand):
    def __init__(self, value):
        super().__init__("fixed", value = value)
    
class Arith_Instruction:
    
    def __init__(self, name, *operands):
        Snippets.add([ f"{name} {', '.join(i)}" for i in itertools.product(*operands)])

class Branch_Jump:

    def _snippet(self, insn, max_offset, prefix=None):
        "offset in multiples of 2B, or nr compressed instructions"
        code = [f'//jump/branch test with {insn}']
        if prefix:
            code.append(prefix)
        #assert max_offset == 1 << (max_offset-1).bit_length(), "max_offset to be power of 2"
        count = (max_offset >> 1)
        bw_label_offset = 10000
        exit_label = 1+ bw_label_offset + count
        code+=[f'{i+1}: {insn} {i+1+bw_label_offset}f' for i in range(count)]
        code+=[f'{count+1}: c.j {exit_label}f']
        code+=[f'{bw_label_offset + i}: {insn} {i+1}b' for i in range(count,0,-1)]
        code+=[f"{exit_label}:"]
        return ('\n    '.join(code))
            
        
class Branches(Branch_Jump):
    def __init__(self, name, registers, reg_val):
        for reg in registers:
            Snippets.add(self._snippet(f"{name} {reg},", max_offset=128, prefix= f'li {reg}, {reg_val}'))

class Jump(Branch_Jump):
    def __init__(self):
        Snippets.add(self._snippet('c.j', max_offset=1024))

class JumpRegister:
    def __init__(self, name, registers):
        for reg in registers:
            code=[f'la {reg}, 1f', f'{name} {reg}', '1:']
            Snippets.add('\n    '.join(code))
            
class Load_Store:
    def __init__(self, name, reg1, reg2, offset, load = False):

        for r2 in reg2:
            dest = [ r for r in reg1  if r != r2 or not load]
            code=[f"{name} {r1}, {o}({r2})" for r1,o in itertools.product(dest, offset)]
            random.shuffle(code)
            Snippets.add(f"la {r2}, __data\n    " +  "\n    ".join(code))

            # and now r2 with r2 if load
            if load and r2 in reg1:
                for o in offset:
                    Snippets.add(f"la {r2}, __data\n    {name} {r2}, {o}({r2})")

class Load_Store_Stack:
    def __init__(self, name, reg, offset, load = False):
        dest = [ r for r in reg if r != "x2" or not load]
        code= [f"{name} {r}, {o}(sp)" for r,o in itertools.product(dest, offset)]
        random.shuffle(code)
        Snippets.add("la sp, __data\n    " + "\n    ".join(code))

        # and now loads with sp destination
        if load and "x2" in reg:
            for o in offset:
                Snippets.add(f"la sp, __data\n    {name} sp, {o}(sp)")
class Reserved:
    def __init__(self, encoding, variable_mask = 0, avoid = {}):
        self.__variable_positions = [ i for i in range(32) if ((variable_mask >> i)&1) == 1]
        count = 1<<len(self.__variable_positions)
        encodings = [encoding | self.__get_variable(i) for i in range(count)]
        code=[f'.half {i:#x}' for i in encodings if f"{i:x}" not in avoid]
        Snippets.add(code)

    def __get_variable(self, v):
        res = 0
        for i in self.__variable_positions:
            if v & 1 == 1:
                res|= 1<<i
            v >>=1
        assert v==0, "variable too big!"
        return res
        
        
class Snippets:
    __snippets = []
    @classmethod
    def add(cls, code):
        if isinstance(code, str):
            cls.__snippets.append(code)
        else:
            cls.__snippets+=code

    def create(cls, code, data, nr_harts = 16):
        snippets = cls.__snippets
        random.shuffle(snippets)
        per_hart = int ( (len(snippets) + nr_harts -1 )/nr_harts)
        with open(code, "w") as f:
            for h in range(nr_harts):
                f.write(f"\n\ntest_hart_{h:x}:\n")
                code = '\n    '.join(snippets[h*per_hart: (h+1)*per_hart]).replace('__data', f'__data_{h:x}')
                f.write(code)
                f.write("\n    ASM_TEST_PASS\n")


    def __init__ (self):
        # Define operands
        short_reg = Operand("short_reg")
        short_reg_nz = Operand("short_reg", forbidden=[0])
        long_reg = Operand("reg")
        long_reg_nz = Operand("reg", forbidden=[0])
        long_reg_nz_nsp = Operand("reg", forbidden=[0,2])
        long_reg_nsp = Operand("reg", forbidden=[2])        
        SP = Operand("fixed", value="sp")

        # Define instructions
        Arith_Instruction("C.ADDI4SPN", short_reg, SP, Immediate(8, offset = 2, allow_zero = False, signed = False))
        Arith_Instruction("C.ADDI", long_reg, Immediate(6,allow_zero = True))
        Arith_Instruction("C.ADDIW", long_reg_nz, Immediate(6))
        Arith_Instruction("C.ADDI16SP", SP, Immediate(6, allow_zero = False, offset=4))
        Arith_Instruction("C.ANDI", short_reg, Immediate(6))
        Arith_Instruction("C.SUB", short_reg, short_reg)
        Arith_Instruction("C.XOR", short_reg, short_reg)
        Arith_Instruction("C.OR", short_reg, short_reg)
        Arith_Instruction("C.AND", short_reg, short_reg)
        Arith_Instruction("C.SUBW", short_reg, short_reg)
        Arith_Instruction("C.ADDW", short_reg, short_reg)
        Arith_Instruction("C.SLLI", long_reg, Immediate(6, signed = False, allow_zero = False))
        Arith_Instruction("C.SLLI64", long_reg)
        Arith_Instruction("C.SRLI", short_reg, Immediate(6, signed = False, allow_zero = False))
        Arith_Instruction("C.SRLI64", short_reg)
        Arith_Instruction("C.SRAI", short_reg, Immediate(6, signed = False, allow_zero = False))
        Arith_Instruction("C.SRAI64", short_reg)
        Arith_Instruction("C.LI", long_reg, Immediate(6))
        Arith_Instruction("C.LUI", long_reg_nsp, Immediate(6, allow_zero = False, mask="0xFFFFF"))
        Arith_Instruction("C.ADD", long_reg, long_reg_nz)
        Arith_Instruction("C.MV", long_reg, long_reg_nz)
        Arith_Instruction("C.NOP")
        Branches("c.BEQZ", short_reg, 0)
        Branches("c.BNEZ", short_reg, 1)
        Jump()
        JumpRegister('C.JR',long_reg_nz)
        JumpRegister('C.JALR',long_reg_nz)
        Load_Store("C.SW", short_reg, short_reg, Immediate(5, signed = False, offset= 2))
        Load_Store("C.SD", short_reg, short_reg, Immediate(5, signed = False, offset=3))
        Load_Store("C.LW", short_reg, short_reg, Immediate(5, signed = False, offset= 2), load = True)
        Load_Store("C.LD", short_reg, short_reg, Immediate(5, signed = False, offset= 3), load = True)
        Load_Store_Stack("C.LWSP", long_reg_nz, Immediate(6, signed = False, offset=2), load = True)
        Load_Store_Stack("C.LDSP", long_reg_nz, Immediate(6, signed = False, offset=3), load = True)
        Load_Store_Stack("C.SWSP", long_reg, Immediate(6, signed = False, offset=2))
        Load_Store_Stack("C.SDSP", long_reg, Immediate(6, signed = False, offset=3))



        #load reserved encodings to avoid
        with open('extract_reserved/reserved.json') as f:
            avoid = json.load(f)
        Reserved(0,0)

        # Reserved in RVC Quadrant 0 (Opcode[1:0] == 0b00) with:
        # C.ADDI4SPN (nzuimm = 0)
        Reserved(0x0000, 0x001c, avoid)
        # C.FLD (RV32/64)
        Reserved(0x2000, 0x1ffc, avoid)
        # Opcode[15:13] == 0b100
        Reserved(0x8000, 0x1ffc, avoid)
        # C.FSD (RV32/64)
        Reserved(0xa000, 0x1ffc, avoid)

        # Reserved in RVC Quadrant 1 (Opcode[1:0] == 0b01) with:
        # C.NOP (nzimm != 0)
        Reserved(0x0001, 0)
        # C.ADDIW (RV 64/128 rd = 0)
        Reserved(0x2001, 0x107c, avoid)
        # C.LI (rd = 0)
        Reserved(0x4001, 0x17c0, avoid)
        # C.LUI (rd = 0)
        Reserved(0x6001, 0, avoid)
        # C.ADDI16SP (nzimm = 0)
        Reserved(0x6101, 0, avoid)
        # Opcode[15:13] == 0b100, Opcode[12] == 0b1, Opcode[11:10] = 0b11, Opcode[6:5] == 0b10/0b11
        Reserved(0x9c41, 0x039c, avoid)
        Reserved(0x9c61, 0x039c, avoid)

        # Reserved in RVC Quadrant 2 (Opcode[1:0] == 0b10) with:
        # C.FLDSP (RV32/64)
        Reserved(0x2002, 0x1ffc, avoid)
        # C.LWSP
        Reserved(0x4002, 0x107c, avoid)
        # C.LDSP (RV64/128)
        Reserved(0x6002, 0x107c, avoid)
        # C.FSDSP (RV32/64)
        Reserved(0xa002, 0x1ffc, avoid)
        # C.JR (rs1 = 0)
        Reserved(0x8002, 0x0000, avoid)

        #TODO: missing instructions:
        #    C.FLD (RV32/64)
        #    C.FSD (RV32/64)
        #    C.FLDSP (RV32/64)
        #    C.EBREAK
        #    C.FSDSP (RV32/64)

def createData(fname, size, nr_harts = 16):
    with open(fname, 'w') as f:
        for h in range(nr_harts):
            f.write(f"__data_{h:x}:\n")
            for i in range(0, size, 4):
                f.write(f".word {random.getrandbits(32):#x}\n")

if __name__== "__main__" :
    CODE_FILE = 'code.s'
    DATA_FILE = 'data.s'
    DATA_SIZE = 1<<19 # 512Kib per hart
    # parse cmd line arguments
    parser = argparse.ArgumentParser("generate RVC code for coverage")
    parser.add_argument("-c","--code", help=f"Generate {CODE_FILE}", action='store_true' )
    parser.add_argument("-d","--data", help=f"Generate {DATA_FILE}", action='store_true' )
    parser.add_argument("-s", "--seed", help="Random seed (Default=0)", default = 0, type=int)
    args = parser.parse_args()

    random.seed(args.seed)

    if args.code:
        s = Snippets()
        s.create(CODE_FILE, DATA_FILE)
    if args.data:
        createData(DATA_FILE, DATA_SIZE)
        
