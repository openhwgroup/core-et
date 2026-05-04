/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    // Barrier used (8 to 11)
    uint64_t barrier_o_t = (3 << 5) + 8;
    //Define The Pitches of the each Tensor 
    uint64_t volatile pitchWeight1 = 4;
    //defining clobber  temp_reg0 to reg x31;
    //defining clobber  temp_reg1 to reg x30;
    //defining clobber  loopIDim2 to reg x1;
    //defining clobber  loopIDim1 to reg x3;
    //defining clobber  tw_prefetch_id to reg x4;
    //defining clobber  b_prefetch_first_time to reg x6;
    uint64_t  prefetch_b_pitch = 320;
    // 4 x reuse in the weight to get 64 GB/sec
    // 32 megs each group of 4 shires
    uint64_t  bRowInit = ( shire_id >> 2) * 32 * 1024 * 1024; /*BRow*/;
    uint64_t  bColInitMinion = ((minion_id - 16) & 3) * 16 /*BCol*/;
    uint64_t  bColInit = bColInitMinion /*BCol*/;
    // Weights are in memory
    uint64_t  tensor_b_init = MEM_WEIGHTS + bRowInit + bColInit * pitchWeight1;
    //defining clobber  tensor_b_dyn to reg x8;

    // Weight minions use L2 scp entries
    // 4 minions active, each one jumping 16 cachelines per prefetch (divided by 4 because 2 bits are ignored)
    uint64_t  l2_scp_dest_entry = ((uint64_t) ((minion_id - 16) * 16 / 4) + L2_SCP_WEIGHT_MEM_OFFSET / 4) << 48;

     __asm__ __volatile__ (
        // Generate prefetch command for matrix B
        "or /*tensor_b_dyn*/ x8, %[l2_scp_dest_entry], %[tensor_b_init] \n"
        "addi /*tensor_b_dyn*/ x8, /*tensor_b_dyn*/ x8, 0xf /*15*/  \n"
        // TensorWait ID
        "li /*tw_prefetch_id*/ x4, 0x4 /*4*/  \n"
        // Prefetches to wait before doing a tensor wait
        "li /*skip_tw*/ x5, 0x2 /*2*/  \n"
        // Open loop for dimension 1
        // Reset Loop counter of Dim:  1
        "li /*loopIDim1*/ x3, 0x7 /*7*/  \n"
        "thw_loop_dim_1: \n"

            // Open loop for dimension 2
            // Reset Loop counter of Dim:  2
            "li /*loopIDim2*/ x1, 0x4 /*4*/  \n"
            "thw_loop_dim_2: \n"
                // Checks if tensor wait needed
                "bne zero, x5, skip_tensor_wait\n"
                    //
                    // Checks for tensorwait
                    //

                    // Switches to another barrier to prevent races between threads reusing
                    // same barrier before the last one is done. Done here to improve perf
                    "addi %[barrier_o_t], %[barrier_o_t], 1 \n"
                    "andi %[barrier_o_t], %[barrier_o_t], -5 \n"
                    "li x31, %[compute_threads]\n"
                    "li x30, 0x803400C8\n"
                    "li x28, %[helper_weights_threads]\n"
                    "li x27, 0x803400D0\n"

                    // Actual tensorwait
                    "csrw 0x830, /*tw_prefetch_id*/ x4 \n"

                    // Barrier and FCC1 to thread0 and FCC0 to itself
                    "csrrw x29, 0x820, %[barrier_o_t] \n"
                    "beq x29, zero, thw_skip_ipi \n"
                    "sd x31, 0x0 (x30)\n"
                    "sd x28, 0x0 (x27)\n"
                    "thw_skip_ipi: \n"

                    // Swaps ID of the tensor wait
                    "xori /*tw_prefetch_id*/ x4, /*tw_prefetch_id*/ x4, 0x1 /*1*/  \n"

                    "j  thw_do_prefetch\n"
                
                // Decrements skip tensor wait counter
                "skip_tensor_wait:\n"
                    "addi /*skip_tw*/ x5, x5, -1 \n"

                // Execute B Prefetch
                "thw_do_prefetch:\n"
                "addi x31, %[prefetch_b_pitch], 0x0 /*0*/  \n"

                // To prevent weight threads to be skewed and have issues with FLBs
                // consume an FCC0
                "csrw 0x821, zero \n"

                "csrw 0x85F, /*tensor_b_dyn*/ x8 \n"
                // Swap the B Prefetch ID
                "xori %[prefetch_b_pitch], %[prefetch_b_pitch], 0x1 /*1*/  \n"
                // Advance L2 SCP dest (4 minions, 16 each lines each, 2 LSB ignored)
                "li x31, (4 * 16 / 4) << 48 \n"
                "add /*tensor_b_dyn*/ x8, /*tensor_b_dyn*/ x8, x31 \n"
                // End of loop for dimension 2
                "addi /*loopIDim2*/ x1, /*loopIDim2*/ x1, 0xffffffffffffffff /*-1*/  \n"
                "beq /*loopIDim2*/ x1, zero, htw_exit_loop_dim_2 \n"
                // Advance weights
                "li x31, 0x1400 /*5120*/  \n"
                "add /*tensor_b_dyn*/ x8, /*tensor_b_dyn*/ x8, x31 \n"
                "j thw_loop_dim_2 \n"

            // Outter loop iteration
            "htw_exit_loop_dim_2: \n"
            // End of loop for dimension 2
            "addi /*loopIDim2*/ x3, /*loopIDim2*/ x3, 0xffffffffffffffff /*-1*/  \n"
            "beq /*loopIDim2*/ x3, zero, htw_exit_loop_dim_1 \n"
            // Advance weights
            "li x31, 0x1400 /*5120*/  \n"
            "add /*tensor_b_dyn*/ x8, /*tensor_b_dyn*/ x8, x31 \n"
            // Loop to inner
            "j thw_loop_dim_1 \n"

        // End of loops
        "htw_exit_loop_dim_1:\n"

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done. Done here to improve perf
        "addi %[barrier_o_t], %[barrier_o_t], 1 \n"
        "andi %[barrier_o_t], %[barrier_o_t], -5 \n"
        "li x31, %[compute_threads]\n"
        "li x30, 0x803400C8\n"
        "li x28, %[helper_weights_threads]\n"
        "li x27, 0x803400D0\n"
        "csrw 0x830, /*tw_prefetch_id*/ x4 \n"
        "csrrw x29, 0x820, %[barrier_o_t] \n"
        "beq x29, zero, skip_ipi_ot_helper_last \n"
        "sd x31, 0x0 (x30)\n"
        "sd x28, 0x0 (x27)\n"
        "skip_ipi_ot_helper_last: \n"

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done. Done here to improve perf
        "xori /*tw_prefetch_id*/ x4, /*tw_prefetch_id*/ x4, 0x1 /*1*/  \n"
        "addi %[barrier_o_t], %[barrier_o_t], 1 \n"
        "andi %[barrier_o_t], %[barrier_o_t], -5 \n"
        "li x31, %[compute_threads]\n"
        "li x30, 0x803400C8\n"
        "li x28, %[helper_weights_threads]\n"
        "li x27, 0x803400D0\n"
        "csrw 0x830, /*tw_prefetch_id*/ x4 \n"
        "csrrw x29, 0x820, %[barrier_o_t] \n"
        "beq x29, zero, skip_ipi_ot_helper_last2 \n"
        "sd x31, 0x0 (x30)\n"
        "sd x28, 0x0 (x27)\n"
        "skip_ipi_ot_helper_last2: \n"
      : 
        [prefetch_b_pitch] "+&r" (prefetch_b_pitch)
      : 
        [compute_threads] "i" (COMPUTE_THREADS),
        [helper_weights_threads] "i" (HELPER_WEIGHTS_THREADS),
        [barrier_o_t] "r" (barrier_o_t),
        [l2_scp_dest_entry] "r" (l2_scp_dest_entry),
        [tensor_b_init] "r" (tensor_b_init)
      : 
       /*temp_reg0*/"x31",
       /*temp_reg1*/"x30",
       /*temp_reg1*/"x29",
       /*temp_reg1*/"x28",
       /*temp_reg1*/"x27",
       /*loopIDim2*/"x1",
       /*loopIDim1*/"x3",
       /*tw_prefetch_id*/"x4",
       /*tensor_b_dyn*/"x8" 
    );
