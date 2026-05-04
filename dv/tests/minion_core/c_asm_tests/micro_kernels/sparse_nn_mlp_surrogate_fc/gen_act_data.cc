/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <math.h>
#include <random>
#include <cmath>
#include <x86intrin.h>

int main()
{
    FILE * file = fopen("src/test_act_data.h", "w");

    // Header
    fprintf(file, "#ifndef _TEST_ACT_DATA_H_\n");
    fprintf(file, "#define _TEST_ACT_DATA_H_\n");
    fprintf(file, "\n");
    fprintf(file, "uint8_t act_data[] = {\n");

    // Starts generating randoms
    std::random_device rd{};
    std::mt19937 gen{rd()};
 
    // Values near the mean are the most likely
    // Standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d{0,0.02};
 
    // Generate 512 values
    for(uint64_t i = 0; i < 512; i++)
    {
        float f32 = d(gen);
        // Activations come from ReLU
        if(f32 < 0.0f) f32 = 0.0f;
        unsigned short f16 = _cvtss_sh(f32, 0);
        uint8_t * u8 = (uint8_t *) &f16;
        fprintf(file, "    %u,\n", * u8);
        u8++;
        if(i != 511) fprintf(file, "    %u,\n", * u8);
        else         fprintf(file, "    %u\n", * u8);
    }

    // Footer
    fprintf(file, "};\n");
    fprintf(file, "\n");
    fprintf(file, "#endif\n");

    // Closes the file
    fclose(file);
}

