/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "../data_small/indices.h"
#include "../data_small/lengths.h"
#include "../data_small/weights.h"
#include "../data_small/data.h"
#include "find_vectors_small.h"

using namespace data_small;

// Function that finds the small vectors based on shire id
void find_vectors_small(volatile int32_t ** lengths, volatile int64_t ** indices, volatile float ** weights, 
                        volatile uint8_t ** data, uint64_t * out, uint32_t shire_id, uint32_t outTensorRowSize)
{
    // Compute based on position
    uint64_t out_ptr  = 0x8210000000ULL + ((shire_id >> 1) * 128 * outTensorRowSize * 2); // Batch 128, out row size by 2 just in case
    * out  = out_ptr;

    // Find index for this minion
    if(shire_id < 2)
    {
        * lengths = lengths_0;
        * indices = indices_0;
        * weights = weights_0;
        * data = data_0;
    }
    else if(shire_id < 4)
    {
        * lengths = lengths_1;
        * indices = indices_1;
        * weights = weights_1;
        * data = data_1;
    }
    else if(shire_id < 6)
    {
        * lengths = lengths_2;
        * indices = indices_2;
        * weights = weights_2;
        * data = data_2;
    }
    else if(shire_id < 8)
    {
        * lengths = lengths_3;
        * indices = indices_3;
        * weights = weights_3;
        * data = data_3;
    }
    else if(shire_id < 10)
    {
        * lengths = lengths_4;
        * indices = indices_4;
        * weights = weights_4;
        * data = data_4;
    }
    else if(shire_id < 12)
    {
        * lengths = lengths_5;
        * indices = indices_5;
        * weights = weights_5;
        * data = data_5;
    }
    else if(shire_id < 14)
    {
        * lengths = lengths_6;
        * indices = indices_6;
        * weights = weights_6;
        * data = data_6;
    }
    else if(shire_id < 16)
    {
        * lengths = lengths_7;
        * indices = indices_7;
        * weights = weights_7;
        * data = data_7;
    }
    else if(shire_id < 18)
    {
        * lengths = lengths_8;
        * indices = indices_8;
        * weights = weights_8;
        * data = data_8;
    }
    else if(shire_id < 20)
    {
        * lengths = lengths_9;
        * indices = indices_9;
        * weights = weights_9;
        * data = data_9;
    }
}
