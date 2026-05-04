/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    // Barrier used (16 to 23)
    // 8 minion barrier, starting at 16th barrier
    uint64_t barrier_o_t = (7 << 5) + 16;

    // Define The Pitches of the each Tensor 
    uint64_t volatile pitchInput0 = 320;
    uint64_t volatile pitchInput1 = 1;
    //defining clobber  temp_reg0 to reg x31;
    //defining clobber  temp_reg1 to reg x30;
    //defining clobber  loopIDim2 to reg x1;
    //defining clobber  loopIDim1 to reg x3;
    //defining clobber  tw_prefetch_id to reg x4;
    //defining clobber  skip_tw to reg x5;

    // Shape and distribution of the activation
    uint64_t  prefetch_a_pitch = 320;
    uint64_t  aRowInitMinion = (minion_id) * 98 /*ARow*/;
#if N_SHIRES_COMPUTE == 32
    uint64_t  aRowInit = (shire_id * 784) + aRowInitMinion /*ARow*/;
#else
    uint64_t  aRowInit = aRowInitMinion /*ARow*/;
#endif

    // Activation is stored in the L2 SCP using the "linear" region
#if N_SHIRES_COMPUTE == 32
    uint64_t  tensor_a_init = (uint64_t volatile) 0x00C0000000ULL + (L2_SCP_ACTIVATION_SOURCE_OFFSET << 11) + aRowInit * pitchInput0;
#else
    uint64_t  tensor_a_init = (uint64_t volatile) 0x0080000000ULL + (shire_id << 23) + (L2_SCP_ACTIVATION_SOURCE_OFFSET << 6 ) + aRowInit * pitchInput0;
#endif
    // Activation minions use 4096 L2 scp entries
    // 512 entries each minion (divided by 4 because destination bits [1:0] are in xs[5:4] which are ignored
    uint64_t  l2_scp_dest_entry = ((uint64_t) (minion_id * 512 / 4) + L2_SCP_ACTIVATION_PREF_OFFSET) << 48;

    // Waits for sync minions to report that data is ready to be consumed
    fcc(FCC_1);

    __asm__ __volatile__ (
        // Generate prefetch command for matrix A
        "or /*tensor_a_dyn*/ x7, %[l2_scp_dest_entry], %[tensor_a_init] \n"
        "addi /*tensor_a_dyn*/ x7, /*tensor_a_dyn*/ x7, 0xd /*13*/  \n"
        "li /*tw_prefetch_id*/ x4, 0x4 /*4*/  \n"
        "li /*skip_tw*/ x5, 0x2 /*2*/  \n"
        "addi /*skip_tw*/ x5, /*skip_tw*/ x5, -1 \n"
        // Open loop for dimension 1
        // Reset Loop counter of Dim:  1
        "li /*loopIDim1*/ x3, 16  \n"
            "loop_dim_1: \n"
            // Open loop for dimension 2
            // Reset Loop counter of Dim:  2
            "li /*loopIDim2*/ x1, 0x1 \n"
                "loop_dim_2: \n"
                "li /*temp_reg1*/ x30, 0x4 /*4*/  \n"

                // Execute A Prefetch and consumes credit
                "addi x31, %[prefetch_a_pitch], 0x0 /*0*/  \n"
                "csrw 0x821, zero \n"
                "csrw 0x85F, /*tensor_a_dyn*/ x7 \n"

                // Swap the A prefetch ID
                "xori %[prefetch_a_pitch], %[prefetch_a_pitch], 0x1 /*1*/  \n"

                // Checks if needs to do a tensor wait
                "bne /*skip_tw*/ x5, zero, tw_minus_prefetch_0 \n"

                    // Switches to another barrier to prevent races between threads reusing
                    // same barrier before the last one is done
                    "addi %[barrier_o_t], %[barrier_o_t], 1\n"
                    "andi %[barrier_o_t], %[barrier_o_t], -9\n"

                    // If so, does tensor wait and switches ID to wait for
                    "csrw 0x830, /*tw_prefetch_id*/ x4 \n"
                    "xori /*tw_prefetch_id*/ x4, /*tw_prefetch_id*/ x4, 0x1 /*1*/  \n"

                    // Access to barrier, if last sends FCC0 to compute minions
                    "csrrw x31, 0x820, %[barrier_o_t] \n"
                    "beq x31, zero, skip_tw_prefetch_0 \n"
                        "li x31, %[compute_threads]\n"
                        "li x30, 0x803400C0\n"
                        "sd x31, 0x0 /*0*/ (x30)\n"
                    "j skip_tw_prefetch_0 \n"

                // Decrements iterations to skip tensor wait
                "tw_minus_prefetch_0: \n"
                    "addi /*skip_tw*/ x5, /*skip_tw*/ x5, 0xffffffffffffffff /*-1*/  \n"

                // Done prefetch
                "skip_tw_prefetch_0: \n"
                "compute_tile_result_loop: \n"
                // Increments 16 positions the destination in L2 Scp
                "li   x31, (16 / 4) << 48\n"
                "add  /*tensor_a_dyn*/ x7, /*tensor_a_dyn*/ x7, x31  \n"
                // Increments inner loop count and checks if end
                "addi /*loopIDim2*/ x1, /*loopIDim2*/ x1, 0xffffffffffffffff /*-1*/  \n"
                "beq /*loopIDim2*/ x1, zero, exit_loop_dim_2 \n"
                // Advance inner loop pointer (as activation matrix, advance 64 bytes)
                "addi /*tensor_a_dyn*/ x7, /*tensor_a_dyn*/ x7, 0x40 /*64*/  \n"
                "j loop_dim_2 \n"

            // End of inner loop, moving to outter
            "exit_loop_dim_2: \n"
            // Increments outter loop count and checks if end
            "addi /*loopIDim1*/ x3, /*loopIDim1*/ x3, 0xffffffffffffffff /*-1*/  \n"
            "beq /*loopIDim1*/ x3, zero, write_results \n"

            // Update data for next iteration of the loop
            // Advance a tensor a full row, less the previously advanced
            "li x31, 0x10c0 /*4288*/  \n"
            "add /*tensor_a_dyn*/ x7, /*tensor_a_dyn*/ x7, x31 \n"
            "j loop_dim_1\n"

        "write_results: \n"

        // Sends an FCC to the other thread
        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done
        "addi %[barrier_o_t], %[barrier_o_t], 1 \n"
        "andi %[barrier_o_t], %[barrier_o_t], -9 \n"
        "csrw 0x830, /*tw_prefetch_id*/ x4 \n"
        "csrrw x31, 0x820, %[barrier_o_t] \n"
        "beq x31, zero, skip_ipi_ot_helper_last \n"
        "li x31, %[compute_threads]\n"
        "li x30, 0x803400C0\n"
        "sd x31, 0x0 /*0*/ (x30)\n"
        "skip_ipi_ot_helper_last: \n"
      : 
        [prefetch_a_pitch] "+&r" (prefetch_a_pitch) 
      : 
        [compute_threads] "i" (COMPUTE_THREADS),
        [barrier_o_t] "r" (barrier_o_t),
        [tensor_a_init] "r" (tensor_a_init),
        [l2_scp_dest_entry] "r" (l2_scp_dest_entry)
      : 
       /*temp_reg0*/"x31",
       /*temp_reg1*/"x30",
       /*loopIDim2*/"x1",
       /*loopIDim1*/"x3",
       /*tw_prefetch_id*/"x4",
       /*skip_tw*/"x5",
       /*tensor_a_dyn*/"x7"
    );

