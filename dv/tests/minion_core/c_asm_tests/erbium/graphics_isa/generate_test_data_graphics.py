#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Enhanced Random Test Data Generator for Esperanto Graphics Instructions

Generates random test data including graphics-specific formats:
- float16, float11, float10
- unorm (2, 8, 10, 16, 24 bit)
- snorm (8, 16 bit)
- Standard float32, int32, uint32
"""

import struct
import random
import argparse
from typing import List, Tuple


class GraphicsDataGenerator:
    """Generates random test data in various graphics formats."""
    
    def __init__(self, seed=None):
        """Initialize with optional seed for reproducibility."""
        if seed is not None:
            random.seed(seed)
    
    def float_to_hex(self, value: float) -> str:
        """Convert float to hex string representation."""
        return "0x{:08x}".format(struct.unpack('I', struct.pack('f', value))[0])
    
    def float32_to_float16(self, f32: float) -> int:
        """Convert float32 to float16 bits (IEEE 754 half precision)."""
        bits = struct.unpack('I', struct.pack('f', f32))[0]
        
        sign = (bits >> 16) & 0x8000
        exponent = ((bits >> 23) & 0xff) - 127 + 15
        mantissa = (bits >> 13) & 0x3ff
        
        # Handle special cases
        if exponent <= 0:
            return sign  # Zero or denormal -> zero
        elif exponent >= 31:
            return sign | 0x7c00  # Inf or NaN -> inf
        
        return sign | (exponent << 10) | mantissa
    
    def float32_to_float11(self, f32: float) -> int:
        """Convert float32 to float11 bits (unsigned, 5 exp, 6 mantissa)."""
        if f32 < 0.0:
            f32 = 0.0  # float11 is unsigned
        
        bits = struct.unpack('I', struct.pack('f', f32))[0]
        
        exponent = ((bits >> 23) & 0xff) - 127 + 15
        mantissa = (bits >> 17) & 0x3f
        
        # Handle special cases
        if exponent <= 0:
            return 0  # Zero or too small
        elif exponent >= 31:
            return 0x7c0  # Inf (11111 00000)
        
        return (exponent << 6) | mantissa
    
    def float32_to_float10(self, f32: float) -> int:
        """Convert float32 to float10 bits (unsigned, 5 exp, 5 mantissa)."""
        if f32 < 0.0:
            f32 = 0.0  # float10 is unsigned
        
        bits = struct.unpack('I', struct.pack('f', f32))[0]
        
        exponent = ((bits >> 23) & 0xff) - 127 + 15
        mantissa = (bits >> 18) & 0x1f
        
        # Handle special cases
        if exponent <= 0:
            return 0  # Zero or too small
        elif exponent >= 31:
            return 0x3e0  # Inf (11111 00000)
        
        return (exponent << 5) | mantissa
    
    def float_to_unorm(self, f32: float, bits: int) -> int:
        """Convert float to unsigned normalized integer."""
        # Clamp to [0.0, 1.0]
        clamped = max(0.0, min(1.0, f32))
        max_val = (1 << bits) - 1
        return int(clamped * max_val + 0.5)
    
    def float_to_snorm(self, f32: float, bits: int) -> int:
        """Convert float to signed normalized integer."""
        # Clamp to [-1.0, 1.0]
        clamped = max(-1.0, min(1.0, f32))
        max_val = (1 << (bits - 1)) - 1
        
        if clamped >= 0:
            return int(clamped * max_val + 0.5)
        else:
            return int(clamped * (max_val + 1) - 0.5)
    
    def generate_random_float32(self) -> float:
        """Generate a random float32 value."""
        choice = random.random()
        
        if choice < 0.4:  # 40% normal values
            return random.uniform(-1000.0, 1000.0)
        elif choice < 0.5:  # 10% small values
            return random.uniform(-1.0, 1.0)
        elif choice < 0.6:  # 10% denormals and near-zero
            sign = random.choice([-1, 1])
            return sign * random.uniform(1e-40, 1e-38)
        elif choice < 0.65:  # 5% special values
            return random.choice([0.0, -0.0, 1.0, -1.0])
        elif choice < 0.70:  # 5% large values
            return random.uniform(1e30, 1e38)
        elif choice < 0.75:  # 5% inf/nan
            return random.choice([float('inf'), float('-inf'), float('nan')])
        elif choice < 0.85:  # 10% graphics-relevant normalized values (0-1)
            return random.uniform(0.0, 1.0)
        else:  # 15% graphics-relevant signed normalized values (-1 to 1)
            return random.uniform(-1.0, 1.0)
    
    def pack_float16_in_32bit(self, f32: float) -> int:
        """Pack float16 in upper 16 bits of 32-bit word."""
        f16 = self.float32_to_float16(f32)
        return (f16 << 16) | 0x0000  # float16 in bits [31:16]
    
    def pack_float11_in_32bit(self, f32: float) -> int:
        """Pack float11 in upper 11 bits of 32-bit word."""
        f11 = self.float32_to_float11(f32)
        return (f11 << 21) | 0x0000  # float11 in bits [31:21]
    
    def pack_float10_in_32bit(self, f32: float) -> int:
        """Pack float10 in upper 10 bits of 32-bit word."""
        f10 = self.float32_to_float10(f32)
        return (f10 << 22) | 0x0000  # float10 in bits [31:22]
    
    def pack_unorm24_in_32bit(self, f32: float) -> int:
        """Pack unorm24 in upper 24 bits of 32-bit word."""
        unorm = self.float_to_unorm(f32, 24)
        return (unorm << 8) | 0x00  # unorm24 in bits [31:8]
    
    def pack_unorm16_in_32bit(self, f32: float) -> int:
        """Pack unorm16 in upper 16 bits of 32-bit word."""
        unorm = self.float_to_unorm(f32, 16)
        return (unorm << 16) | 0x0000  # unorm16 in bits [31:16]
    
    def pack_unorm10_in_32bit(self, f32: float) -> int:
        """Pack unorm10 in upper 10 bits of 32-bit word."""
        unorm = self.float_to_unorm(f32, 10)
        return (unorm << 22) | 0x0000  # unorm10 in bits [31:22]
    
    def pack_unorm8_in_32bit(self, f32: float) -> int:
        """Pack unorm8 in upper 8 bits of 32-bit word."""
        unorm = self.float_to_unorm(f32, 8)
        return (unorm << 24) | 0x000000  # unorm8 in bits [31:24]
    
    def pack_unorm2_in_32bit(self, f32: float) -> int:
        """Pack unorm2 in upper 2 bits of 32-bit word."""
        unorm = self.float_to_unorm(f32, 2)
        return (unorm << 30) | 0x00000000  # unorm2 in bits [31:30]
    
    def pack_snorm16_in_32bit(self, f32: float) -> int:
        """Pack snorm16 in upper 16 bits of 32-bit word."""
        snorm = self.float_to_snorm(f32, 16)
        return ((snorm & 0xFFFF) << 16) | 0x0000  # snorm16 in bits [31:16]
    
    def pack_snorm8_in_32bit(self, f32: float) -> int:
        """Pack snorm8 in upper 8 bits of 32-bit word."""
        snorm = self.float_to_snorm(f32, 8)
        return ((snorm & 0xFF) << 24) | 0x000000  # snorm8 in bits [31:24]
    
    def generate_random_int32(self) -> int:
        """Generate a random 32-bit signed integer."""
        choice = random.random()
        
        if choice < 0.6:
            return random.randint(-1000000, 1000000)
        elif choice < 0.8:
            return random.randint(-2147483648, 2147483647)
        elif choice < 0.9:
            return random.randint(-100, 100)
        else:
            return random.choice([0, -1, 1, -2147483648, 2147483647])
    
    def generate_random_uint32(self) -> int:
        """Generate a random 32-bit unsigned integer."""
        choice = random.random()
        
        if choice < 0.6:
            return random.randint(0, 1000000)
        elif choice < 0.8:
            return random.randint(0, 4294967295)
        elif choice < 0.9:
            return random.randint(0, 255)
        else:
            return random.choice([0, 1, 255, 65535, 0xFFFFFFFF])
    
    def generate_mask_value(self) -> int:
        """Generate an 8-bit mask value for m0 register."""
        choice = random.random()
        
        if choice < 0.5:
            return 0xFF
        elif choice < 0.7:
            return 0x00
        elif choice < 0.85:
            return 1 << random.randint(0, 7)
        else:
            return random.randint(0, 255)
    
    def generate_c_data_file(self, num_vectors: int, output_file: str = "test_data.c"):
        """Generate a C file with test data arrays including graphics formats."""
        
        with open(output_file, 'w') as f:
            # Write header
            f.write("/*\n")
            f.write(" * Esperanto Graphics Instructions Test Data\n")
            f.write(" * Auto-generated random test data with graphics formats\n")
            f.write(f" * Number of test vectors: {num_vectors}\n")
            f.write(" */\n\n")
            
            f.write("#include <stdint.h>\n\n")
            
            # Generate standard float32 data
            f.write("/* Standard floating-point test data (32-bit) */\n")
            f.write(f"__attribute__((aligned(32))) float test_data_fp32[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: self.float_to_hex(self.generate_random_float32()))
            f.write("};\n\n")
            
            # Generate float16 data (packed in 32-bit)
            f.write("/* Float16 test data (packed in 32-bit, bits [31:16]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_float16[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_float16_in_32bit(self.generate_random_float32())))
            f.write("};\n\n")
            
            # Generate float11 data (packed in 32-bit)
            f.write("/* Float11 test data (packed in 32-bit, bits [31:21]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_float11[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_float11_in_32bit(self.generate_random_float32())))
            f.write("};\n\n")
            
            # Generate float10 data (packed in 32-bit)
            f.write("/* Float10 test data (packed in 32-bit, bits [31:22]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_float10[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_float10_in_32bit(self.generate_random_float32())))
            f.write("};\n\n")
            
            # Generate unorm24 data
            f.write("/* Unorm24 test data (packed in 32-bit, bits [31:8]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_unorm24[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_unorm24_in_32bit(random.uniform(0.0, 1.0))))
            f.write("};\n\n")
            
            # Generate unorm16 data
            f.write("/* Unorm16 test data (packed in 32-bit, bits [31:16]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_unorm16[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_unorm16_in_32bit(random.uniform(0.0, 1.0))))
            f.write("};\n\n")
            
            # Generate unorm10 data
            f.write("/* Unorm10 test data (packed in 32-bit, bits [31:22]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_unorm10[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_unorm10_in_32bit(random.uniform(0.0, 1.0))))
            f.write("};\n\n")
            
            # Generate unorm8 data
            f.write("/* Unorm8 test data (packed in 32-bit, bits [31:24]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_unorm8[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_unorm8_in_32bit(random.uniform(0.0, 1.0))))
            f.write("};\n\n")
            
            # Generate unorm2 data
            f.write("/* Unorm2 test data (packed in 32-bit, bits [31:30]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_unorm2[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_unorm2_in_32bit(random.uniform(0.0, 1.0))))
            f.write("};\n\n")
            
            # Generate snorm16 data
            f.write("/* Snorm16 test data (packed in 32-bit, bits [31:16]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_snorm16[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_snorm16_in_32bit(random.uniform(-1.0, 1.0))))
            f.write("};\n\n")
            
            # Generate snorm8 data
            f.write("/* Snorm8 test data (packed in 32-bit, bits [31:24]) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_snorm8[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.pack_snorm8_in_32bit(random.uniform(-1.0, 1.0))))
            f.write("};\n\n")
            
            # Generate integer data
            f.write("/* Integer test data (32-bit signed) */\n")
            f.write(f"__attribute__((aligned(32))) int32_t test_data_int32[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.generate_random_int32() & 0xFFFFFFFF))
            f.write("};\n\n")
            
            # Generate unsigned integer data
            f.write("/* Unsigned integer test data (32-bit) */\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_data_uint32[{num_vectors * 8}] = {{\n")
            self._write_array_data(f, num_vectors, lambda: "0x{:08x}".format(self.generate_random_uint32()))
            f.write("};\n\n")
            
            # Generate mask values
            f.write("/* Mask register test data (8-bit) */\n")
            f.write(f"__attribute__((aligned(8))) uint8_t test_data_masks[{num_vectors}] = {{\n    ")
            mask_values = [self.generate_mask_value() for _ in range(num_vectors)]
            hex_values = ["0x{:02x}".format(v) for v in mask_values]
            
            for i in range(0, len(hex_values), 16):
                chunk = hex_values[i:i+16]
                f.write(", ".join(chunk))
                if i + 16 < len(hex_values):
                    f.write(",\n    ")
            
            f.write("\n};\n\n")
            
            # Generate output buffers
            f.write("/* Output buffer for results */\n")
            f.write(f"__attribute__((aligned(32))) float test_output_fp32[{num_vectors * 8}];\n")
            f.write(f"__attribute__((aligned(32))) uint32_t test_output_uint32[{num_vectors * 8}];\n\n")
            
            # Generate data structure
            f.write("/* Data structure for test access */\n")
            f.write("typedef struct {\n")
            f.write("    float *fp32_data;\n")
            f.write("    uint32_t *float16_data;\n")
            f.write("    uint32_t *float11_data;\n")
            f.write("    uint32_t *float10_data;\n")
            f.write("    uint32_t *unorm24_data;\n")
            f.write("    uint32_t *unorm16_data;\n")
            f.write("    uint32_t *unorm10_data;\n")
            f.write("    uint32_t *unorm8_data;\n")
            f.write("    uint32_t *unorm2_data;\n")
            f.write("    uint32_t *snorm16_data;\n")
            f.write("    uint32_t *snorm8_data;\n")
            f.write("    int32_t *int_data;\n")
            f.write("    uint32_t *uint_data;\n")
            f.write("    uint8_t *mask_data;\n")
            f.write("    float *fp_output;\n")
            f.write("    uint32_t *uint_output;\n")
            f.write(f"    int num_vectors;\n")
            f.write("} test_data_t;\n\n")
            
            f.write("test_data_t test_data = {\n")
            f.write("    .fp32_data = test_data_fp32,\n")
            f.write("    .float16_data = test_data_float16,\n")
            f.write("    .float11_data = test_data_float11,\n")
            f.write("    .float10_data = test_data_float10,\n")
            f.write("    .unorm24_data = test_data_unorm24,\n")
            f.write("    .unorm16_data = test_data_unorm16,\n")
            f.write("    .unorm10_data = test_data_unorm10,\n")
            f.write("    .unorm8_data = test_data_unorm8,\n")
            f.write("    .unorm2_data = test_data_unorm2,\n")
            f.write("    .snorm16_data = test_data_snorm16,\n")
            f.write("    .snorm8_data = test_data_snorm8,\n")
            f.write("    .int_data = test_data_int32,\n")
            f.write("    .uint_data = test_data_uint32,\n")
            f.write("    .mask_data = test_data_masks,\n")
            f.write("    .fp_output = test_output_fp32,\n")
            f.write("    .uint_output = test_output_uint32,\n")
            f.write(f"    .num_vectors = {num_vectors}\n")
            f.write("};\n")
        
        print(f"Generated C test data file: {output_file}")
        print(f"  - {num_vectors} test vectors per format")
        print(f"  - Formats: FP32, F16, F11, F10, UNORM (2,8,10,16,24), SNORM (8,16), INT32, UINT32")
        print(f"  - {num_vectors * 8} values per format")
        print(f"  - {num_vectors} mask values")
    
    def _write_array_data(self, f, num_vectors: int, value_generator):
        """Helper to write array data with formatting."""
        for vec in range(num_vectors):
            f.write("    /* Vector {} */\n    ".format(vec))
            values = [value_generator() for _ in range(8)]
            f.write(", ".join(values))
            if vec < num_vectors - 1:
                f.write(",\n")
            else:
                f.write("\n")


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description="Generate random test data for Esperanto graphics instructions (with graphics formats)"
    )
    parser.add_argument(
        "-n", "--num-vectors",
        type=int,
        default=100,
        help="Number of test vectors to generate (default: 100)"
    )
    parser.add_argument(
        "-o", "--output",
        type=str,
        default="test_data_graphics.c",
        help="Output C file (default: test_data_graphics.c)"
    )
    parser.add_argument(
        "-s", "--seed",
        type=int,
        default=None,
        help="Random seed for reproducibility (optional)"
    )
    
    args = parser.parse_args()
    
    generator = GraphicsDataGenerator(seed=args.seed)
    generator.generate_c_data_file(
        num_vectors=args.num_vectors,
        output_file=args.output
    )


if __name__ == "__main__":
    main()
