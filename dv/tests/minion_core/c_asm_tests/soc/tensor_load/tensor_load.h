/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


void do_tensor_load_common(et_tensor_load_conf_t * load_conf_matrix, void* ptr, bool coop) {
   //
   // Tensor Load configuration
   //
   load_conf_matrix->rd_l2scp       = 0x0U;                   
   load_conf_matrix->use_tmask      = false;                  // xs[   63]: Not used currently
   load_conf_matrix->use_coop       = coop;                   // xs[   62]: 1: cooperation, 0: no cooperation
   load_conf_matrix->transformation = 0x0UL;                  // xs[61:59]: Tensor Load Op dependant

   load_conf_matrix->addr           = (uint64_t) ptr;         // xs[47: 6]: FROM: Full Virtual Address
   load_conf_matrix->num_lines      = et_get_rand_word(0,15); // xs[ 3: 0]: num_cache_lines - 1 (Max Lines = 16)

   //load_conf_matrix->dst_start      = et_get_rand_word(0,47); // xs[58:53]: TO: SCP Cache line (0..47)
   load_conf_matrix->dst_start      = et_get_rand_word(0,47-load_conf_matrix->num_lines); // xs[58:53]: TO: SCP Cache line (0..47)

   load_conf_matrix->offset         = et_get_rand_word(0,3);  // xs[ 5: 4]
   load_conf_matrix->use_tenb       = et_get_rand_word(0,1);  // xs[   52]: Tensor Load Op dependant

   load_conf_matrix->stride         = 64;                     // x31[47:6]: Stride in Bytes (typically a full cache line, 64 bytes)
   load_conf_matrix->id             = 0;                      // x31[0]: Tensor Operation ID
}

void do_tensor_load(et_tensor_load_conf_t * load_conf_matrix) {
   load_conf_matrix->transformation = 0x0UL; // xs[61:59]: In a Tensor Load must be 0
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   et_tensor_load (load_conf_matrix);
}

void setup_tensor_load_B(et_tensor_load_conf_t * load_conf_matrix) {
   load_conf_matrix->transformation = et_get_rand_word(0,8); // WARL(0) 
   load_conf_matrix->use_tenb       = 1;     // xs[52]
   et_tensor_load (load_conf_matrix);
}

void do_tensor_load_interleave8(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b001; // xs[61:59]: In a Tensor Load interleave8 must be 0b001
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   et_tensor_load (load_conf_matrix);
}

void do_tensor_load_interleave16(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b010; // xs[61:59]: In a Tensor Load interleave8 must be 0b001
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   load_conf_matrix->offset         = load_conf_matrix->offset & 0b10; //xs[4]: WARL(0)
   et_tensor_load (load_conf_matrix);
}

void do_tensor_load_transpose8(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b101; // xs[61:59]: In a Tensor Load transpose8 must be 0b101
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   et_tensor_load (load_conf_matrix);
}

void do_tensor_load_transpose16(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b110; // xs[61:59]: In a Tensor Load transpose16 must be 0b110
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   load_conf_matrix->offset         = load_conf_matrix->offset & 0b10; //xs[4]: WARL(0)
   et_tensor_load (load_conf_matrix);
}

void do_tensor_load_transpose32(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b111; // xs[61:59]: In a Tensor Load transpose32 must be 0b111
   load_conf_matrix->use_tenb       = 0;     // xs[52]
   et_tensor_load (load_conf_matrix);
}

unsigned long do_tensor_load_illegal_config_0(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b011; // xs[61:59]: Illegal config
   et_tensor_load (load_conf_matrix);
   unsigned long ten_err = get_tensor_error();
   return ten_err;
}

unsigned long  do_tensor_load_illegal_config_1(et_tensor_load_conf_t* load_conf_matrix) {
   load_conf_matrix->transformation = 0b100; // xs[61:59]: Illegal config
   et_tensor_load (load_conf_matrix);
   unsigned long ten_err = get_tensor_error();
   return ten_err;
}

void do_tensorFMA(et_tensor_load_conf_t* tenA, et_tensor_load_conf_t* tenB){
    // The main purpose of this function is to consume the values of a given TenB in
    // order to be able to check it. For this reason, some of the values are hardcoded
    uint64_t tenA_cols = tenB->num_lines;
    uint64_t tenA_rows = tenA->num_lines;

    uint64_t tenB_cols = 16;
    //TenB_rows is equal to tenA_cols

    //         use_tmask,       b_num_col, a_num_rows, a_num_cols, offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b,       scp_loc_a, opcode, first_pass
    tensor_fma(        0, (tenB_cols/4)-1,  tenA_rows,  tenA_cols,      0,        0,             1,             1,        1,         0, tenA->dst_start,  0b011,          1);
}
