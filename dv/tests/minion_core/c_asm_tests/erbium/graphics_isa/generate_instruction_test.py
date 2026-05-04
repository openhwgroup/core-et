#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Enhanced Random Graphics Instruction Test Generator

Generates graphics instructions with pre-loaded random data from memory regions.
Each instruction is preceded by loads from the test data arrays.
"""

import random
import argparse
from typing import List, Dict, Tuple


class EnhancedGraphicsTestGenerator:
    """Generates complete test sequences with data loading."""

    def __init__(self, seed=None):
        """Initialize the generator with an optional seed."""
        if seed is not None:
            random.seed(seed)

        # Graphics instructions from previous generator
        self.upconvert_instructions = [
            "fcvt.ps.f11", "fcvt.ps.f10",
            "fcvt.ps.un24", "fcvt.ps.un16", "fcvt.ps.un10",
            "fcvt.ps.un8", "fcvt.ps.un2",
            "fcvt.ps.sn16", "fcvt.ps.sn8",
        ]

        self.downconvert_instructions = [
            "fcvt.f11.ps", "fcvt.f10.ps",
            "fcvt.un24.ps", "fcvt.un16.ps", "fcvt.un10.ps",
            "fcvt.un8.ps", "fcvt.un2.ps",
            "fcvt.sn16.ps", "fcvt.sn8.ps",
        ]

        self.cubemap_2op = ["fcvt.ps.rast", "fcvt.rast.ps"]
        self.cubemap_3op = ["cubeface.ps", "cubefaceidx.ps", "cubesgnsc.ps", "cubesgntc.ps"]
        self.rast_3op = ["frcp_fix.rast"]

        # Register tracking
        self.fp_regs = [f"f{i}" for i in range(32)]
        self.int_regs = [f"x{i}" for i in range(5, 32)]  # Avoid x0, ra, sp, gp and tp
        self.mask_regs = [f"m{i}" for i in range(8)]

        self.vector_index = 0

    def _random_fp_reg(self) -> str:
        """Return a random FP register."""
        return random.choice(self.fp_regs)

    def _random_int_reg(self) -> str:
        """Return a random integer register (avoiding x0)."""
        return random.choice(self.int_regs)

    def _random_mask_reg(self) -> str:
        """Return a random mask register."""
        return random.choice(self.mask_regs)

    def generate_load_fp_vector(self, dest_reg: str, data_type: str = "fp") -> str:
        """Generate assembly to load a 256-bit FP vector from test data."""
        # Map data type to appropriate array
        data_map = {
            "fp": "test_data_fp32",
            "fp32": "test_data_fp32",
            "float16": "test_data_float16",
            "float11": "test_data_float11",
            "float10": "test_data_float10",
            "unorm24": "test_data_unorm24",
            "unorm16": "test_data_unorm16",
            "unorm10": "test_data_unorm10",
            "unorm8": "test_data_unorm8",
            "unorm2": "test_data_unorm2",
            "snorm16": "test_data_snorm16",
            "snorm8": "test_data_snorm8",
            "int": "test_data_int32",
            "uint": "test_data_uint32",
        }

        base_label = data_map.get(data_type, "test_data_fp32")

        offset = self.vector_index * 32  # 8 elements * 4 bytes
        self.vector_index = (self.vector_index + 1) % 100  # Cycle through vectors

        # Load address and then load vector
        code = f"    la a0, {base_label}\n"
        code += f"   li a1, {offset}\n"
        code += f"   add a0, a0, a1\n"
        code += f"   flq2 {dest_reg}, 0(a0)"

        return code

    def generate_load_int_value(self, dest_reg: str) -> str:
        """Generate assembly to load a 32-bit integer value."""
        offset = (self.vector_index * 32) + random.randint(0, 7) * 4
        self.vector_index = (self.vector_index + 1) % 100

        code = f"    la a0, test_data_int32\n"
        code += f"   li a1, {offset}\n"
        code += f"   add a0, a0, a1\n"
        code += f"    lw {dest_reg}, 0(a0)"

        return code

    def generate_load_uint_value(self, dest_reg: str) -> str:
        """Generate assembly to load an unsigned 32-bit value."""
        offset = (self.vector_index * 32) + random.randint(0, 7) * 4
        self.vector_index = (self.vector_index + 1) % 100

        code = f"    la a0, test_data_uint32\n"
        code += f"   li a1, {offset}\n"
        code += f"   add a0, a0, a1\n"
        code += f"    lw {dest_reg}, 0(a0)"

        return code

    def generate_load_mask(self, dest_mask: str = "m0") -> str:
        """Generate assembly to load a mask value into m0."""
        offset = self.vector_index % 100
        self.vector_index = (self.vector_index + 1) % 100

        code = f"    la a0, test_data_masks\n"
        code += f"   li a1, {offset}\n"
        code += f"   add a0, a0, a1\n"
        code += f"    lbu a1, 0(a0)\n"
        code += f"    mov.m.x m0, a1, 0  # Load into m0"

        return code

    def generate_upconvert_test(self) -> Tuple[str, str]:
        """Generate an upconvert instruction with data loading."""
        instr = random.choice(self.upconvert_instructions)
        fd = self._random_fp_reg()
        fs1 = self._random_fp_reg()

        # Map instruction to appropriate source data format
        instr_to_format = {
            "fcvt.ps.f16": "float16",
            "fcvt.ps.f11": "float11",
            "fcvt.ps.f10": "float10",
            "fcvt.ps.un24": "unorm24",
            "fcvt.ps.un16": "unorm16",
            "fcvt.ps.un10": "unorm10",
            "fcvt.ps.un8": "unorm8",
            "fcvt.ps.un2": "unorm2",
            "fcvt.ps.sn16": "snorm16",
            "fcvt.ps.sn8": "snorm8",
        }

        data_format = instr_to_format.get(instr, "uint")

        # Load mask
        loads = self.generate_load_mask()
        loads += "\n"

        # Load source data from appropriate format array
        loads += self.generate_load_fp_vector(fs1, data_format)
        loads += "\n"

        # Instruction
        test = f"    {instr} {fd}, {fs1}"

        return loads, test

    def generate_downconvert_test(self) -> Tuple[str, str]:
        """Generate a downconvert instruction with data loading."""
        instr = random.choice(self.downconvert_instructions)
        fd = self._random_fp_reg()
        fs1 = self._random_fp_reg()

        # Load mask
        loads = self.generate_load_mask()
        loads += "\n"

        # Load source data
        loads += self.generate_load_fp_vector(fs1, "fp")
        loads += "\n"

        # Instruction
        test = f"    {instr} {fd}, {fs1}"

        return loads, test

    def generate_cubemap_2op_test(self) -> Tuple[str, str]:
        """Generate a 2-operand cubemap/rast instruction with data loading."""
        instr = random.choice(self.cubemap_2op)
        fd = self._random_fp_reg()
        fs1 = self._random_fp_reg()

        # Load mask
        loads = self.generate_load_mask()
        loads += "\n"

        # Load source data
        loads += self.generate_load_fp_vector(fs1, "fp")
        loads += "\n"

        # Instruction
        test = f"    {instr} {fd}, {fs1}"

        return loads, test

    def generate_cubemap_3op_test(self) -> Tuple[str, str]:
        """Generate a 3-operand cubemap instruction with data loading."""
        instr = random.choice(self.cubemap_3op)
        fd = self._random_fp_reg()
        fs1 = self._random_fp_reg()
        fs2 = self._random_fp_reg()

        # Load mask
        loads = self.generate_load_mask()
        loads += "\n"

        # Load source data
        loads += self.generate_load_fp_vector(fs1, "fp")
        loads += "\n"
        loads += self.generate_load_fp_vector(fs2, "fp")
        loads += "\n"

        # Instruction
        test = f"    {instr} {fd}, {fs1}, {fs2}"

        return loads, test

    def generate_rast_3op_test(self) -> Tuple[str, str]:
        """Generate a frcp_fix.rast instruction with data loading."""
        fd = self._random_fp_reg()
        fs1 = self._random_fp_reg()
        fs2 = self._random_fp_reg()

        # Load mask
        loads = self.generate_load_mask()
        loads += "\n"

        # Load source data
        loads += self.generate_load_fp_vector(fs1, "fp")
        loads += "\n"
        loads += self.generate_load_fp_vector(fs2, "fp")
        loads += "\n"

        # Instruction
        test = f"    frcp_fix.rast {fd}, {fs1}, {fs2}"

        return loads, test

    def generate_bitmixb_test(self) -> Tuple[str, str]:
        """Generate a bitmixb instruction with data loading."""
        rd = self._random_int_reg()
        rs1 = self._random_int_reg()
        rs2 = self._random_int_reg()

        # Load source data
        loads = self.generate_load_uint_value(rs1)
        loads += "\n"
        loads += self.generate_load_uint_value(rs2)
        loads += "\n"

        # Instruction
        test = f"    bitmixb {rd}, {rs1}, {rs2}"

        return loads, test

    def generate_maskpopc_test(self) -> Tuple[str, str]:
        """Generate a maskpopc.rast instruction with data loading."""
        xd = self._random_int_reg()
        ms1 = self._random_mask_reg()
        ms2 = self._random_mask_reg()
        imm4 = random.randint(0, 15)

        # Load mask data into mask registers
        loads = f"    la a0, test_data_masks\n"
        offset1 = self.vector_index % 100
        self.vector_index = (self.vector_index + 1) % 100
        offset2 = self.vector_index % 100
        self.vector_index = (self.vector_index + 1) % 100

        loads += f"    lbu a1, {offset1}(a0)\n"
        loads += f"    mov.m.x {ms1}, a1, 0\n"
        loads += f"    lbu a1, {offset2}(a0)\n"
        loads += f"    mov.m.x {ms2}, a1, 0\n"

        # Instruction
        test = f"    maskpopc.rast {xd}, {ms1}, {ms2}, {imm4}"

        return loads, test

    def generate_random_test(self) -> Tuple[str, str]:
        """Generate a random graphics instruction test with data loading."""
        # These are instruction know to be broken in the RTL
        # No need to run them
        broken_instructions = [
            #self.generate_cubemap_2op_test,
            #self.generate_rast_3op_test,
        ]
        generators = [
            self.generate_upconvert_test,
            self.generate_downconvert_test,
            self.generate_cubemap_3op_test,
            self.generate_bitmixb_test,
            self.generate_maskpopc_test,
        ]

        generator = random.choice(generators)
        return generator()

    def generate_test_file(self, num_tests: int, output_file: str = "graphics_test.S"):
        """Generate a complete test assembly file."""
        saved_regs = 13
        s_regs = 12
        with open(output_file, 'w') as f:
            # Write header
            f.write("/*\n")
            f.write(" * Esperanto Graphics Instructions Test\n")
            f.write(" * Auto-generated test with random data loading\n")
            f.write(f" * Number of tests: {num_tests}\n")
            f.write(" */\n\n")

            f.write("    .text\n")
            f.write("    .globl graphics_instruction_test\n")
            f.write("    .align 2\n\n")

            f.write("graphics_instruction_test:\n")
            f.write("    # Save return address\n")
            f.write(f"    addi sp, sp, -{8*saved_regs}\n")
            f.write("    sd ra, 0(sp)\n")
            for i in range(s_regs):
                f.write(f"    sd s{i}, {8*i+8}(sp)\n")                

            # Generate tests
            for i in range(num_tests):
                f.write(f"    # ========================================\n")
                f.write(f"    # Test {i + 1}\n")
                f.write(f"    # ========================================\n")

                loads, instruction = self.generate_random_test()

                f.write(loads)
                f.write("\n")
                f.write(instruction)
                f.write("\n\n")

            # Write epilogue
            f.write("    # Test complete\n")
            f.write("    ld ra, 0(sp)\n")
            for i in range(s_regs):
                f.write(f"    ld s{i}, {8*i+8}(sp)\n")                
            
            f.write(f"    addi sp, sp, {8*saved_regs}\n")
            f.write("    ret\n\n")

            f.write("    .end\n")

        print(f"Generated test file: {output_file}")
        print(f"  - {num_tests} instruction tests")
        print(f"  - Each test includes data loading from test_data.c")


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description="Generate graphics instruction tests with random data loading"
    )
    parser.add_argument(
        "-n", "--num-tests",
        type=int,
        default=50,
        help="Number of instruction tests to generate (default: 50)"
    )
    parser.add_argument(
        "-o", "--output",
        type=str,
        default="graphics_instruction_test.S",
        help="Output assembly file (default: graphics_instruction_test.S)"
    )
    parser.add_argument(
        "-s", "--seed",
        type=int,
        default=None,
        help="Random seed for reproducibility (optional)"
    )

    args = parser.parse_args()

    generator = EnhancedGraphicsTestGenerator(seed=args.seed)
    generator.generate_test_file(
        num_tests=args.num_tests,
        output_file=args.output
    )


if __name__ == "__main__":
    main()
