/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


__asm__ __volatile__(

   /* evict_coalescing_buffer */

   /* li */
   "li x25, 3122523575941063663\n"


   /* flb0 */
   "csrrw x25, flb0, x25\n"

   "beq x0, x25, main_tensor_store_1094_evict_coalescing_buffer_follower_code_185\n"
   "main_tensor_store_1094_evict_coalescing_buffer_lead_code_185:\n"

   /* li */
   "li x26, 2561\n"


   /* li */

   /* mtg_load_addr */
   "li x14, 0x80F1E030 /*ESR_SC_IDX_COP_SM_CTL_BCAST[1][8]@8*/\n"



   /* sd */
   "sd x26, 0(x14)\n"


   /* fence */
   "fence iorw, iorw\n"


   /* wait_shire_cache_cop */
   "main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_0_744:\n"

   /* li */

   /* mtg_load_addr */
   "li x14, 0x80F00030 /*ESR_SC_IDX_COP_SM_CTL_B0[1][8]@8*/\n"



   /* ld */
   "ld x26, 0(x14)\n"


   /* srli */
   "srli x26, x26, 24\n"


   /* li */
   "li x14, 255\n"


   /* and */
   "and x26, x26, x14\n"


   /* li */
   "li x14, 8\n"

   "beq x26, x14, main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_0_744\n"


   /* wait_shire_cache_cop */
   "main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_1_745:\n"

   /* li */

   /* mtg_load_addr */
   "li x14, 0x80F02030 /*ESR_SC_IDX_COP_SM_CTL_B1[1][8]@8*/\n"



   /* ld */
   "ld x26, 0(x14)\n"


   /* srli */
   "srli x26, x26, 24\n"


   /* li */
   "li x14, 255\n"


   /* and */
   "and x26, x26, x14\n"


   /* li */
   "li x14, 8\n"

   "beq x26, x14, main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_1_745\n"


   /* wait_shire_cache_cop */
   "main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_2_746:\n"

   /* li */

   /* mtg_load_addr */
   "li x14, 0x80F04030 /*ESR_SC_IDX_COP_SM_CTL_B2[1][8]@8*/\n"



   /* ld */
   "ld x26, 0(x14)\n"


   /* srli */
   "srli x26, x26, 24\n"


   /* li */
   "li x14, 255\n"


   /* and */
   "and x26, x26, x14\n"


   /* li */
   "li x14, 8\n"

   "beq x26, x14, main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_2_746\n"


   /* wait_shire_cache_cop */
   "main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_3_747:\n"

   /* li */

   /* mtg_load_addr */
   "li x14, 0x80F06030 /*ESR_SC_IDX_COP_SM_CTL_B3[1][8]@8*/\n"



   /* ld */
   "ld x26, 0(x14)\n"


   /* srli */
   "srli x26, x26, 24\n"


   /* li */
   "li x14, 255\n"


   /* and */
   "and x26, x26, x14\n"


   /* li */
   "li x14, 8\n"

   "beq x26, x14, main_tensor_store_1094_wait_shire_cache_cop_wait_shire_cache_cops_3_747\n"


   /* li */
   "li x25, 5367922880\n"


   /* li */
   "li x26, 4294967295\n"


   /* send_fcc */

   /* sd */
   "sd x26, 0(x25)\n"


   "main_tensor_store_1094_evict_coalescing_buffer_follower_code_185:\n"
   "main_tensor_store_1094_evict_coalescing_buffer_wait_for_fcc_credit_185:\n"

   /* fcc */
   "csrrwi x0, fcc, 0\n"
);
