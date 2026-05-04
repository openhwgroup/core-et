/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    
//
    // Send credits to prefetcher
    // Gets the IPI address to thread0
    // Barrier used (0 to 7)
    uint64_t barrier_o_t = (31 << 5);
    //Define The Pitches of the each Tensor
    uint64_t volatile pitchInput0_T0 = 320;
    uint64_t volatile pitchInput1 = 1;
    uint64_t volatile pitchOutput0 = 320;
    uint64_t volatile pitchOutput1 = 1;
    //defining clobber  temp_reg0 to reg x31;
    //defining clobber  temp_reg1 to reg x30;
    //defining clobber  loopIDim2 to reg x1;
    //defining clobber  loopIDim1 to reg x3;
    // Input activations  are stored in L2 SCP using the "offset" region
    // Shire_id to access local L2 SCP
    // Offset where the L2 activations are prefetched
    // Offset depending where the section of each minion is prefetched
    uint64_t  tensor_a = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_ACTIVATION_SOURCE_OFFSET << 6) + ((minion_id *pitchInput0_T0*4)); //4 iter
    uint64_t  tensor_b = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_ACTIVATION2_SOURCE_OFFSET << 6) +  ((minion_id *pitchInput0_T0*4)); //4 iter



    // Result is stored in the L2 SCP using the "linear" region
#if N_SHIRES_COMPUTE == 32
    uint64_t  tensor_c = (uint64_t volatile) 0x00C0000000UL + (L2_SCP_RESULT_OFFSET << 11) + (minion_id * pitchOutput0 * 4); //always write to another shire l2scp
#else
    uint64_t  tensor_c = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_RESULT_OFFSET << 6) + ((minion_id * pitchOutput0 * 4));
#endif

    __asm__ __volatile__ (

        // gsole fixes:
        //  - moved to 4 different integer registers with what to gather in fg32b
        //    this way we don't need to update base pointer on every pair of fg32b
        //  - moved all fg32b together to hide 4 cycle load to use
        //  - tensorquant is doing upconvert, mul and downconvert when not needed
        //    with coherent scale factors. Removed
        //  - removed tensor load as it is not needed in tensor quant
        //  - results must be in f0..f7 in order to get affected by tensor quant
        //  - fixed clear upper to be 0xff instead of 0xffff
        //  - moved the compactation from code to tensor quant
        //  - moved stuff that is constant out of the loops

        // Prepares mask and clear upper bits
        "li       x31, 0xFF\n"
        "fbcx.ps  f16,  x31\n"
        "mov.m.x  m0, zero, 0xff\n"

        // fg32b immediates for 4 different chunks
        "li       x7,  0x398a418820\n"
        "li       x8,  0x7B9AC5A928\n"
        "li       x9,  0xBDAB49CA30\n"
        "li       x10, 0xFFBBCDEB38\n"

        //Tensor Store
        "li       x29, 0x4018000000000000\n" // 4 rows of 16B each
        "add      x29, x29, %[tensor_c]\n"
        "li       x31, 16\n"                 // stride for store is 16

        //tensor quant, saturate + pack128b
        "li       x30, 0x19c0000000000a9\n"

        // FLB send
        "li       x27, %[helper_activation_threads]\n"
        "li       x26, 0x803400D0\n"

        // Outter loop
        "li       x11, 16 \n"
        "IterateResultsDim1:\n"
            "li       x12, 4\n"
            // Waits for credits from activation threads
            // Each credit is 4 cachelines worth, which is exactly the inner loop size
            "csrw     0x821, zero \n"

            // Inner loop
            "IterateResultsDim2:\n"

                "lb       x0, 64(%[tensor_a])\n"
                "lb       x0, 64(%[tensor_b])\n"
                "fg32b.ps f0, x7,  %[tensor_a]\n"
                "fg32b.ps f1, x7,  %[tensor_b]\n"
                "fg32b.ps f2, x8,  %[tensor_a]\n"
                "fg32b.ps f3, x8,  %[tensor_b]\n"
                "fg32b.ps f4, x9,  %[tensor_a]\n"
                "fg32b.ps f5, x9,  %[tensor_b]\n"
                "fg32b.ps f6, x10, %[tensor_a]\n"
                "fg32b.ps f7, x10, %[tensor_b]\n"
                "fand.pi  f0, f0, f16\n"
                "fand.pi  f1, f1, f16\n"
                "fand.pi  f2, f2, f16\n"
                "fand.pi  f3, f3, f16\n"
                "fand.pi  f4, f4, f16\n"
                "fand.pi  f5, f5, f16\n"
                "fand.pi  f6, f6, f16\n"
                "fand.pi  f7, f7, f16\n"
                "fadd.pi  f0, f0, f1\n"
                "fadd.pi  f1, f2, f3\n"
                "fadd.pi  f2, f4, f5\n"
                "fadd.pi  f3, f6, f7\n"
                "addi     %[tensor_a], %[tensor_a], 32\n"
                "addi     %[tensor_b], %[tensor_b], 32\n"
                "fg32b.ps f8,  x7,  %[tensor_a]\n"
                "fg32b.ps f9,  x7,  %[tensor_b]\n"
                "fg32b.ps f10, x8,  %[tensor_a]\n"
                "fg32b.ps f11, x8,  %[tensor_b]\n"
                "fg32b.ps f12, x9,  %[tensor_a]\n"
                "fg32b.ps f13, x9,  %[tensor_b]\n"
                "fg32b.ps f14, x10, %[tensor_a]\n"
                "fg32b.ps f15, x10, %[tensor_b]\n"
                "fand.pi  f8, f8, f16\n"
                "fand.pi  f9, f9, f16\n"
                "fand.pi  f10, f10, f16\n"
                "fand.pi  f11, f11, f16\n"
                "fand.pi  f12, f12, f16\n"
                "fand.pi  f13, f13, f16\n"
                "fand.pi  f14, f14, f16\n"
                "fand.pi  f15, f15, f16\n"
                "fadd.pi  f4, f9, f9\n"
                "fadd.pi  f5, f10, f11\n"
                "fadd.pi  f6, f12, f13\n"
                "fadd.pi  f7, f14, f15\n"

                //tensor quant, saturate + pack128b
                "csrw     0x806, x30\n"

                // Given that an instruction needs to be between tquant and tstore,
                // we put the FLB access here
                "csrrw    x28, 0x820, %[barrier_o_t]\n"

                // Tensor store and increment result pointer
                "csrw     0x87F, x29\n"
                "addi     x29, x29, 64\n"

                // Switches to another barrier to prevent races between threads reusing
                // same barrier before the last one is done
                "addi     %[barrier_o_t], %[barrier_o_t], 1\n"
                "andi     %[barrier_o_t], %[barrier_o_t], -9\n"

                // Increment sources for next iteration here
                "addi     %[tensor_a], %[tensor_a], 32\n"
                "addi     %[tensor_b], %[tensor_b], 32\n"

                //  End of loop for Dimension 2
                "addi     x12, x12, -1\n"

                // Checks if FLB winner and send credit back
                "beq      x28, zero, skip_ipi_ot_helper_main\n"
                    "sd       x27, 0(x26)\n"
                "skip_ipi_ot_helper_main:\n"
                
                //  End of loop for Dimension 2
                "bne      x12, zero, IterateResultsDim2\n"
            // End of loop for Dimension 1
            "addi     x11, x11, -1\n"
            "beq      x11, zero, IterateResultsExitDim1\n"
            // Assuming no padding right now to ease prefetching, can be fixed in reality
            //"addi     %[tensor_a], %[tensor_a], 64\n" //320 -256
            //"addi     %[tensor_b], %[tensor_b], 64\n"
            //"addi     x29, x29, 64\n"
            "j        IterateResultsDim1\n"
        // End outter loop
        "IterateResultsExitDim1:\n"
      :
        [tensor_c] "+&r" (tensor_c),
        [tensor_a] "+&r" (tensor_a),
        [tensor_b] "+&r" (tensor_b)
      :
        [helper_activation_threads] "i" (HELPER_ACTIVATION_THREADS),
        [barrier_o_t] "r" (barrier_o_t)
      :
       /*temp_reg0*/"x31",
       /*temp_reg1*/"x30",
       /*temp_reg1*/"x29",
       /*temp_reg1*/"x28",
       /*temp_reg1*/"x27",
       /*temp_reg1*/"x26",
       /*fGatherConfReg*/"x7",
       /*fGatherConfReg*/"x8",
       /*fGatherConfReg*/"x9",
       /*fGatherConfReg*/"x10",
       /*loopIDim2*/"x11",
       /*loopIDim1*/"x12"
    );

    // We need a separate barrier for the completely done barrier, as some
    // minions might be done at some point before the other ones. If we use
    // the regular 2 thread 0 barriers, the minion will get boggus data

    // This fence waits for Write Arounds to finish!!
    tensorwait(8);

    // We also fence in case we have other stores
    riscv_fence();

    // FLB at barrier 27 and with 32 threads (FLBs 28-31 are reserved for the FW)
    if(flbarrier(27, 32 - 1))
    {
        // The last one reaching the barrier sends FCC0 to the Coalescing Buffer Drainer threads (24-27)
        // User, own shire, credit0 on thread1
        fcc_send(PRV_U, SHIRE_OWN, THREAD_1, FCC_0, HELPER_DRAIN_THREADS);
    }
