/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Global
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/test_compute.h"

int main(int argc, char * argv[])
{
    FILE * fd;

    fd = fopen("weights.mem", "wb");

    uint64_t total_bytes = 4ULL * 1024ULL * 1024ULL * 1024ULL;

    // For all the rows
    for(int i = 0; i < (total_bytes / DATA_TENSOR_ROW_SIZE); i++)
    {
        // UINT8 Data
        uint8_t data[DATA_TENSOR_ROW_ELEMS];
        for(int j = 0; j < DATA_TENSOR_ROW_ELEMS; j++)
            data[j] = (uint8_t) rand();

        fwrite(data, DATA_TENSOR_ROW_ELEMS, 1, fd);

        // Scale and bias
        float scale = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        fwrite(&scale, 4, 1, fd);
        float bias = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        fwrite(&bias, 4, 1, fd);

        // Padding
        uint8_t padding[DATA_TENSOR_ROW_SIZE];
        for(int j = (DATA_TENSOR_ROW_ELEMS + 8); j < DATA_TENSOR_ROW_SIZE; j++)
        {
            padding[j] = 0;
        }
        fwrite(&padding[DATA_TENSOR_ROW_ELEMS + 8], DATA_TENSOR_ROW_SIZE - DATA_TENSOR_ROW_ELEMS - 8, 1, fd);
    }
    fclose(fd);

    return 0;
}

