/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Local
#include "test_common.h"
#include "test_compute.h"

#ifdef USE_SMALL_DATA
    #include "find_vectors_small.h"
#else
    #include "find_vectors_large.h"
#endif

#include "utils.h"

void test_compute(uint32_t shire_id, uint32_t minion_id)
{
     __asm__ __volatile__ (
        "test_compute_start_point:\n"
        ".global test_compute_start_point\n"
    );

    __asm__ __volatile__ ("mov.m.x m0, zero, 0xff /*255*/  \n");

    // Global barrier (harts, flb to use, source shire id, fcc to sync minions)
    global_barrier_starter(32, 0, shire_id, FCC_0);

    // Waits for credit to come back
    fcc(FCC_0);

    //
    // Global distribution
    // We have 15 sparse matmuls, each one with batch=128
    // Each shire works on 64 batches, 2 shires disabled
    // Each minion works on 2 batches
    //

    // Stall threads of disabled shires (30 and 31)
    if(shire_id >= ACTIVE_SHIRES)
    {
        // FLB at barrier 27 and with 32 threads
        uint64_t last = flbarrier(27, 32 - 1);

        // The last one reaching the barrier sends FCC0 to the Coalescing Buffer Drainer threads (24-27)
        // User, own shire, credit0 on thread1
        if(last)
            fcc_send(PRV_U, 32, THREAD_0, FCC_0, 0x1);
    }

    //
    // Operation is pretty simple:
    //  - Length vector specifices the length (non-zero-values) of each element of the batch
    //  - Minion needs to add all the lengths of previous elements of the batch to know where its data starts
    //  - Minion gets its element assigned length to know how many elements to add
    //  - Minion reads weight and index for current pass
    //  - Using index gets 64 byte elements from data (dense) matrix
    //  - Applies dequantize
    //  - Multiplies by weight and accums

    volatile int32_t * lengths; // Pointer to the lengths (non-zero values) of each element of the batch
    volatile int64_t * indices; // Pointer to the indices (row of dense data vector) of each non-zero value
    volatile float   * weights; // Pointer to the weights of the sparse tensor
    volatile uint8_t * data;    // Pointer to the dense data tensor
    uint64_t           out;     // Pointer to the result

    // Gets tensors based on shire
    #ifdef USE_SMALL_DATA
        find_vectors_small(&lengths, &indices, &weights, &data, &out, shire_id, OUT_TENSOR_ROW_SIZE);
    #else
        find_vectors_large(&lengths, &indices, &weights, &data, &out, shire_id, OUT_TENSOR_ROW_SIZE);
    #endif

    int32_t index_sparse_data; // Index to weights and lengths
    int32_t result_pos;        // Which is the position within the batch for first result

    result_pos = (shire_id & 1) * RESULTS_PER_SHIRE + (minion_id * RESULTS_PER_MINION);
    out = out + result_pos * OUT_TENSOR_ROW_SIZE;

    // Add all the non-zero-values for all the results before the one minion will work on
    index_sparse_data = 0;
    for(int32_t i = 0; i < result_pos; i++)
        index_sparse_data += lengths[i];

    // Compute
    // For all the results this minion works on
    for(int32_t batch_result = 0; batch_result < RESULTS_PER_MINION; batch_result++)
    {
        int32_t length_result = lengths[result_pos + batch_result]; // Number of non-zero-values for this result
        volatile int32_t gather_offsets[] = { 0, 1, 2, 3, 4, 5, 6, 7 };      // Gather offsets

        // Clears results
        __asm__ __volatile__ (
            "fxor.pi f0, f0, f0\n"
            "fxor.pi f1, f1, f1\n"
            "fxor.pi f2, f2, f2\n"
            "fxor.pi f3, f3, f3\n"
            "fxor.pi f4, f4, f4\n"
            "fxor.pi f5, f5, f5\n"
            "fxor.pi f6, f6, f6\n"
            "fxor.pi f7, f7, f7\n"
            // Clear to convert from int8 to uint8
            "li      t0, 0xff\n"
            "fbcx.ps f30, t0\n"
            // Loads gather offsets
            "flw.ps  f31, 0x0(%[gather_offsets])\n"
          :
          : [gather_offsets] "r" (gather_offsets)
          : "t0"
        );

        // For all the accumulations needed for this result
        for(int32_t pass = 0; pass < length_result; pass++)
        {
            volatile uint8_t * data_pass  = data + indices[index_sparse_data] * DATA_TENSOR_ROW_SIZE;
            float            * scale_ptr  = (float *) &data_pass[DATA_TENSOR_ROW_ELEMS];
            //float              scale      = * scale_ptr;
            float            * offset_ptr = (float *) &data_pass[DATA_TENSOR_ROW_ELEMS + 4];
            //float              offset     = * offset_ptr;
            float            * weight_ptr = (float *) &weights[index_sparse_data];
            //float              weight     = * weight_ptr;

            // Moves to next element
            index_sparse_data++;

            // For all the B cols of the result
            //for(int32_t out_line_size = 0; out_line_size < OUT_TENSOR_ROW_ELEMS; out_line_size++)
            //{
            //    uint8_t quantized_data = data_pass[out_line_size];
            //    float dequantized_data = ((float) quantized_data * scale) + offset;
            //    results[out_line_size] += weight * dequantized_data;
            //}

            __asm__ __volatile__ (
                //
                //    uint8_t quantized_data = data_pass[out_line_size];
                //

                // Load the weight, scale and offset
                "fbc.ps  f27, 0x0(%[weight_ptr])\n"
                "fbc.ps  f28, 0x0(%[offset_ptr])\n"
                "fbc.ps  f29, 0x0(%[scale_ptr])\n"

                // Load 64 bytes with 8 gathers
                // TODO: replace for fg32b
                "fgb.ps f8, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f9, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f10, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f11, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f12, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f13, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f14, f31, %[data_pass]\n"
                "addi   %[data_pass], %[data_pass], 8\n" // Move to next 8 elements
                "fgb.ps f15, f31, %[data_pass]\n"
                // As fgb does sign extend, remove it as we work with uint_8
                "fand.pi f8,  f8, f30\n"
                "fand.pi f9,  f9, f30\n"
                "fand.pi f10, f10, f30\n"
                "fand.pi f11, f11, f30\n"
                "fand.pi f12, f12, f30\n"
                "fand.pi f13, f13, f30\n"
                "fand.pi f14, f14, f30\n"
                "fand.pi f15, f15, f30\n"

                //
                //    float dequantized_data = ((float) quantized_data * scale) + offset;
                //

                // Convert to float32
                "fcvt.ps.pw f8,  f8\n"
                "fcvt.ps.pw f9,  f9\n"
                "fcvt.ps.pw f10, f10\n"
                "fcvt.ps.pw f11, f11\n"
                "fcvt.ps.pw f12, f12\n"
                "fcvt.ps.pw f13, f13\n"
                "fcvt.ps.pw f14, f14\n"
                "fcvt.ps.pw f15, f15\n"
                // Multiply by scale and add offset
                "fmadd.ps f8,  f8,  f29, f28\n"
                "fmadd.ps f9,  f9,  f29, f28\n"
                "fmadd.ps f10, f10, f29, f28\n"
                "fmadd.ps f11, f11, f29, f28\n"
                "fmadd.ps f12, f12, f29, f28\n"
                "fmadd.ps f13, f13, f29, f28\n"
                "fmadd.ps f14, f14, f29, f28\n"
                "fmadd.ps f15, f15, f29, f28\n"

                //
                //    results[out_line_size] += weight * dequantized_data;
                //

                "fmadd.ps f0, f27, f8,  f0\n"
                "fmadd.ps f1, f27, f9,  f1\n"
                "fmadd.ps f2, f27, f10, f2\n"
                "fmadd.ps f3, f27, f11, f3\n"
                "fmadd.ps f4, f27, f12, f4\n"
                "fmadd.ps f5, f27, f13, f5\n"
                "fmadd.ps f6, f27, f14, f6\n"
                "fmadd.ps f7, f27, f15, f7\n"
              : [data_pass]  "+&r" (data_pass)
              : [offset_ptr] "r"   (offset_ptr),
                [scale_ptr]  "r"   (scale_ptr),
                [weight_ptr] "r"   (weight_ptr)
              : "t0"
            );
        }

        //// Stores the results
        //for(int32_t out_line_size = 0; out_line_size < OUT_TENSOR_ROW_ELEMS; out_line_size++)
        //    out[out_line_size] = results[out_line_size];

        uint64_t tensor_store = (0ULL << 62)   // No register stride
                              | (0ULL << 57)   // Start storing register 0
                              | (3ULL << 55)   // 64 byte row size
                              | (3ULL << 51)   // 4 rows to store a total of 256 bytes
                              | (0ULL << 49)   // No cooperation
                              | (out & ~0x3F); // Destination array

        __asm__ __volatile__(
            "li  x31, 0x40\n"
            "csrw 0x87F, %[tensor_store]\n"
          :
          : [tensor_store] "r" (tensor_store)
          : "x31"
        );
    }

    //
    // Done!!
    //

     __asm__ __volatile__ (
        "test_compute_end_point:\n"
        ".global test_compute_end_point\n"
    );
}
