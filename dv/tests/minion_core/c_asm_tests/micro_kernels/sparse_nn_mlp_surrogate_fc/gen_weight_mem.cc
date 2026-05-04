/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <math.h>
#include <random>
#include <cmath>
#include <x86intrin.h>

int main()
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d{0,0.02};
 
    FILE * file = fopen("weights.mem", "wb");

    // Generate 1024 x 1025 weight matrix    
    for(uint64_t i = 0; i < (1024 * 1025); i++)
    {
        float f32 = d(gen);
        unsigned short f16 = _cvtss_sh(f32, 0);
        fwrite(&f16, sizeof(f16), 1, file);
    }
    fclose(file);
}

