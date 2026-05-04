/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    //
    // Send credits to prefetcher
    // Gets the IPI address to thread0
    // Barrier used (0 to 7)
    uint64_t barrier_o_t = (31 << 5);
    //Define The Pitches of the each Tensor
    uint64_t volatile pitchInput0 = 320;
    uint64_t volatile pitchInput1 = 1;
    uint64_t volatile pitchOutput0 = 64;
    uint64_t volatile pitchOutput1 = 1;
    uint64_t volatile pitchWeight0 = 320;
    uint64_t volatile pitchWeight1 = 4;
    uint64_t volatile pitchBias0 = 4;
    //defining clobber  temp_reg0 to reg x31;
    //defining clobber  temp_reg1 to reg x30;
    //defining clobber  loopIDim2 to reg x1;
    //defining clobber  loopIDim1 to reg x3;
    uint64_t  coopMaskA = ((/*CoopID*/ ((minion_id >>2)& 0x1) + 0) | (/*coopMaskMinion*/  (( (0xf << ( minion_id & 0x1c)) >> ( minion_id & 0x18 )) & 0xFF)  << 8 ) | ((/*coopMaskNeigh*/ (0x1<<(minion_id >> 3)) & 0xff) << 16 ));
    uint64_t  coopMaskB = ((/*CoopID*/ (minion_id & 0x3) + 2) | (/*coopMaskMinion*/  (( (0x11111111 << ( minion_id & 0x3)) >> ( minion_id & 0x18 )) & 0xFF)  << 8 ) | ((/*coopMaskNeigh*/ (0xF) & 0xff) << 16 ));
    uint64_t  aRowInitMinion = (((( minion_id  >> 2 )& 7) * 98)) /*ARow*/;
#if N_SHIRES_COMPUTE == 32
    uint64_t  aRowInit = (((( shire_id  >> 0 )& 31) * 784)) + aRowInitMinion /*ARow*/;
#else
    uint64_t  aRowInit = aRowInitMinion /*ARow*/;
#endif
    uint64_t  bColInitMinion = (((( minion_id  >> 0 )& 3) * 16)) /*BCol*/;
    uint64_t  bColInit = (0) + bColInitMinion /*BCol*/;

    // Input activations  are stored in L2 SCP using the "offset" region
    // Shire_id to access local L2 SCP
    // Offset where the L2 activations are prefetched
    // Offset depending where the section of each minion is prefetched
#ifdef  UPF
    uint64_t  tensor_a_init = (uint64_t volatile) 0x0080000000UL + (2 << 23) + (L2_SCP_ACTIVATION_PREF_OFFSET << 6) + (((minion_id >> 2) * 512) << 6);
#else
    uint64_t  tensor_a_init = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_ACTIVATION_PREF_OFFSET << 6) + (((minion_id >> 2) * 512) << 6);
#endif
    // Weights are stored in L2 SCP using the "offset" region
    // Shire_id to access local L2 SCP
    // Offset where the L2 weights are prefetched
    // Offset depending where the section of each minion is prefetched
    uint64_t  tensor_b_init = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_WEIGHT_PREF_OFFSET << 6) + ((minion_id & 0x3) * 16 * 64);

    // Result is stored in the L2 SCP using the "linear" region
#if N_SHIRES_COMPUTE == 32
    uint64_t  tensor_c_dyn = (uint64_t volatile) 0x00C0000000UL + (L2_SCP_RESULT_OFFSET << 11) + aRowInit * pitchOutput0  + bColInit * pitchOutput1;
#else
    #ifdef UPF
        uint64_t  tensor_c_dyn = (uint64_t volatile) 0x0080000000UL + (2 << 23) + (L2_SCP_RESULT_OFFSET << 6) + aRowInit * pitchOutput0  + bColInit * pitchOutput1;
    #else 
        uint64_t  tensor_c_dyn = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_RESULT_OFFSET << 6) + aRowInit * pitchOutput0  + bColInit * pitchOutput1;
    #endif
#endif
    // Defining clobber  tensor_fma to reg x6;
    uint64_t  tensor_e_init = (uint64_t volatile) MEM_OFFSET + bColInit * pitchBias0;
    uint64_t  tensor_d_init = (uint64_t volatile) MEM_SCALE;

    __asm__ __volatile__ (
        // As there's a single scale, load it in the beginning
        // Use tload id 1 for this as well for bias. Bias twait
        // will make sure this is done as well
        "li x30, 0x420000000000000 /*297237575406452736*/  \n"
        "add x30, x30, %[tensor_d_init] \n"
        "addi x31, zero, 1\n"
        "csrwi 0x804, 0x0\n"
        "csrw 0x83F, x30 \n"
        // TensorLoad of bias to L1 Scp
        "li x30, 0x400000000000000 /*288230376151711744*/  \n"
        "add x30, x30, %[tensor_e_init] \n"
        "addi x31, zero, 1\n"
        "csrw 0x83F, x30 \n"

        "li /*tensor_a_dyn*/ x4, 0x400000000000000d /*4611686018427387917*/  \n"
        "li /*tensor_b_dyn*/ x5, 0x421000000000000f /*4760304806130614287*/  \n"
        "add /*tensor_b_dyn*/ x5, /*tensor_b_dyn*/ x5, %[tensor_b_init] \n"
        "add /*tensor_a_dyn*/ x4, /*tensor_a_dyn*/ x4, %[tensor_a_init] \n"
        "li /*tensor_fma*/ x6, 0x7ef800000310007 /*571816415190908935*/  \n"
        // Open loop for dimension 1
        // Reset Loop counter of Dim:  1
        "li /*loopIDim1*/ x3, 0x7 /*7*/  \n"
        // Open loop for dimension 2
        // Reset Loop counter of Dim:  2
        "li /*loopIDim2*/ x1, 0x4 /*4*/  \n"
        "loop_dim_2: \n"
        "li /*temp_reg1*/ x30, 0x4 /*4*/  \n"
        // Sends an FCC to the other thread

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done
        "addi %[barrier_o_t], %[barrier_o_t], 0x1 /*1*/  \n"
        "andi %[barrier_o_t], %[barrier_o_t], -9  \n"

        // Set A TensorLoad Pitch and coop
        "li x31, 0x40\n"
        "csrw 0x804, %[coopMaskA] \n"
        // Waits for the other thread to send the prefetch signal
        "csrw 0x821, zero \n"
        // Execute A TensorLoad
        "csrw 0x83F, /*tensor_a_dyn*/ x4 \n"

        // Set B TensorLoad Pitch and coop
        "csrw 0x804, %[coopMaskB] \n"
        // Waits for the other thread to send the prefetch signal
        "csrwi 0x821, 1\n"
        // Execute B TensorLoad
        "csrw 0x83F, /*tensor_b_dyn*/ x5 \n"

        "compute_tile_result_loop: \n"
        // As now there are cycles where we wait for the TLoad A to finish, it is the
        // best place to send the FCC to helper act thread. Now we are consuming the data, so
        // it is as well from producer/consumer point of view
        "csrrw x31, 0x820, %[barrier_o_t] \n"
        "beq x31, zero, skip_ipi_ot_helper_main \n"
            "li x31, %[helper_activation_threads]\n"
            "li x30, 0x803400D0\n"
            "sd x31, 0x0 /*0*/ (x30)\n"
        "skip_ipi_ot_helper_main: \n"
        "li x31, 0x0 /*0*/  \n"
        "add x31, x31, /*loopIDim2*/ x1 \n"
        "addi x31, x31, 0xffffffffffffffff /*-1*/  \n"
        "bne x31, zero, write_rf_skip \n"
        // Set what?
        "li x31, 0x800000 /*8388608*/  \n"
        "or /*tensor_fma*/ x6, /*tensor_fma*/ x6, x31 \n"
        "write_rf_skip: \n"

        //Wait for Tload A to be done
        "csrw 0x830, 0x0 /*0*/  \n"
        // Start TensorFMA
        "csrw 0x801, /*tensor_fma*/ x6 \n"
        // Switch registers for tensorload A
        "li x31, 0x200000000000000 /*144115188075855872*/  \n"
        "xor /*tensor_a_dyn*/ x4, /*tensor_a_dyn*/ x4, x31 \n"
        // Switch registers for tensorFMA
        "li x31, 0x30100 /*196864*/  \n"
        "xor /*tensor_fma*/ x6, /*tensor_fma*/ x6, x31 \n"
        // Clear the Start flag
        "andi /*tensor_fma*/ x6, /*tensor_fma*/ x6, 0xfffffffffffffffe /*-2*/  \n"
        // End of loop for dimension 2
        "addi /*loopIDim2*/ x1, /*loopIDim2*/ x1, 0xffffffffffffffff /*-1*/  \n"
        "beq /*loopIDim2*/ x1, zero, exit_loop_dim_2 \n"
        // Advance the same amount as the activation prefetch
        "addi /*tensor_a_dyn*/ x4, /*tensor_a_dyn*/ x4, 16 * 64 \n"
        // Advance the same amount as the weight prefetch
        "li x31, 4 * 16 * 64\n"
        "add   x5, x5, x31\n"
        "j loop_dim_2 \n"
        "exit_loop_dim_2: \n"
        // Check if last iteration (don't start tloads for next iteration as there's not)
        "addi /*loopIDim1*/ x3, /*loopIDim1*/ x3, 0xffffffffffffffff /*-1*/  \n"
        "beq /*loopIDim1*/ x3, zero, write_results \n"

        // Load new bias (stupid for this test, but let's keep it for perf checking
        // reasons)
        "li    x30, 0x400000000000000 /*288230376151711744*/  \n"
        "add   x30, x30, %[tensor_e_init] \n"
        "li    x31, 1\n"
        "csrwi 0x804, 0x0\n"
        "csrw  0x83F, x30 \n"

        // Set A TensorLoad Pitch and coop mask. Read consecutive cachelines from L2 scp
        "li x31, 0x40\n"
        "csrw 0x804, %[coopMaskA] \n"
        // Waits for the other thread to send the prefetch signal
        "csrw 0x821, zero \n"
        // Execute A TensorLoad
        "csrw 0x83F, /*tensor_a_dyn*/ x4 \n"
        // Execute B TensorLoad
        "csrw 0x804, %[coopMaskB] \n"
        // Waits for the other thread to send the prefetch signal. Read consecutive cachelines from L2 scp
        "csrwi 0x821, 1\n"
        "csrw 0x83F, /*tensor_b_dyn*/ x5 \n"
        // Mark the next tensorFMA as 1st one, so it must clear the registers
        "ori /*tensor_fma*/ x6, /*tensor_fma*/ x6, 0x1 /*1*/  \n"

        //
        // Result write section
        //
        "write_results: \n"

        // Waits for data required by tensorquant to be in L1 Scp
        "csrwi 0x830, 0x1 /*0*/  \n"

        // TensorQuant
        "li x31, 0x1ec000000a92614 /*138485688552728084*/  \n"
        "csrw 0x806, x31 \n"

        // Tensor Store (stride is 64)
        "li x30, 0x406e000000000008 /*4640959416005296128*/  \n"
        "add x30, x30, %[tensor_c_dyn] \n"
        "li x31, 0x40 /*64*/  \n"
        "csrw 0x87F, x30 \n"

        //If last result, we are done
        "beq /*loopIDim1*/ x3, zero, compute_done_a_rows \n"

        // Updates TensorFMA (clear last pass bit)
        "li x31, 0xffffffffff7fffff /*-8388609*/  \n"
        "and /*tensor_fma*/ x6, /*tensor_fma*/ x6, x31 \n"
         //Reset Loop counter of Dim:  2
        "li /*loopIDim2*/ x1, 0x4 /*4*/  \n"
        "addi %[tensor_c_dyn], %[tensor_c_dyn], 0x380 /*896*/  \n"
         //jump in the middle of the first loop, because some tensor load has already been done
        "j compute_tile_result_loop \n"
        "compute_done_a_rows: \n"
      :
        [coopMaskA] "+&r" (coopMaskA),
        [coopMaskB] "+&r" (coopMaskB),
        [tensor_c_dyn] "+&r" (tensor_c_dyn)
      :
        [helper_activation_threads] "i" (HELPER_ACTIVATION_THREADS),
        [barrier_o_t] "r" (barrier_o_t),
        [tensor_a_init] "r" (tensor_a_init),
        [tensor_b_init] "r" (tensor_b_init),
        [tensor_e_init] "r" (tensor_e_init),
        [tensor_d_init] "r" (tensor_d_init)
      :
       /*temp_reg0*/"x31",
       /*temp_reg1*/"x30",
       /*loopIDim2*/"x1",
       /*loopIDim1*/"x3",
       /*tensor_a_dyn*/"x4",
       /*tensor_b_dyn*/"x5",
       /*tensor_fma*/"x6"
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

