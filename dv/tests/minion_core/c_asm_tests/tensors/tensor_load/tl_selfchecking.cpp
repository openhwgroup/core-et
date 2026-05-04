
#define PASS 0
#define NOT_PASS 1
#define L1SCP_LINE_SIZE_BYTES 64
#define L1SCP_NUM_LINES 48

#define TLOAD_TENA  0
#define TLOAD_TENB  0
#define TLOAD_INT8  1
#define TLOAD_INT16 2
#define TLOAD_TRA8  5
#define TLOAD_TRA16 6
#define TLOAD_TRA32 7

#define CHECK 1
#define EXECUTE 2
#define TL_ACTION_WRAPPER(ACTION, RTL_DATA, GOLDEN_DATA) \
   if ((ACTION) == CHECK) { \
      if ((RTL_DATA) != (GOLDEN_DATA)) return NOT_PASS; \
   } \
   else if ((ACTION) == EXECUTE) { \
      (RTL_DATA) = (GOLDEN_DATA); \
   }\
   else { \
      return NOT_PASS; \
   } \

inline uint_fast64_t sm_get_bits(uint64_t value, int head, int tail) {
   // we want to extract k-bits
   uint_fast64_t k = (head - tail) + 1;
   uint_fast64_t mask = 1;
   mask = mask << k;
   mask = mask - 1;
   uint_fast64_t sh_val = value;
   sh_val = sh_val  >> tail;
   return (mask & sh_val);
}

int compare_l1scp(uint64_t* l1scp_a, uint64_t* l1scp_b) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint64_t);
   uint64_t (*cmp_l1scp_a)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) l1scp_a;
   uint64_t (*cmp_l1scp_b)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) l1scp_b;

   for (int i = 0; i < L1SCP_NUM_LINES; i++) {
      for (int j = 0; j < elems_per_scp_line; j++) {
         if (cmp_l1scp_a[i][j] != cmp_l1scp_b[i][j]) return NOT_PASS;
      }
   }

   return PASS;
}

int do_tl(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint64_t);
   uint64_t (*sec_l1scp)[elems_per_scp_line] = (uint64_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t VA     = (sm_get_bits(csr_enc, 47,  6) * 64);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   for (int i = 0; i < count; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         uint64_t * golden_data = (uint64_t*) (VA + (i*stride));
         uint64_t * rtl_data = sec_l1scp[(dst + i) % L1SCP_NUM_LINES];
         for (int elem = 0; elem < elems_per_scp_line; elem++) {
            TL_ACTION_WRAPPER(action, rtl_data[elem], golden_data[elem])
         }
      }
   }
   return PASS;
}

int do_tl_int8(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint8_t);
   uint8_t (*sec_l1scp)[elems_per_scp_line] = (uint8_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t base   = (sm_get_bits(csr_enc, 47,  6) * 64);
   const uint64_t offset = (sm_get_bits(csr_enc,  5,  4) * 16);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   const uint64_t VA = base + offset;
   for (int i = 0; i < count; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         for (int r = 0; r < 4; r++) {
            uint8_t * golden_data = (uint8_t*) (VA + (i * 4 + r) * stride);
            uint8_t * rtl_data = (uint8_t*) sec_l1scp[(dst + i) % L1SCP_NUM_LINES];
            for (int c = 0; c < 16; c++) {
               TL_ACTION_WRAPPER(action, rtl_data[c*4 + r], golden_data[c])
            }
         }
      }
   }
   return PASS;
}

int do_tl_int16(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint16_t);
   uint16_t (*sec_l1scp)[elems_per_scp_line] = (uint16_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t base   = (sm_get_bits(csr_enc, 47,  6) * 64);
   const uint64_t offset = (sm_get_bits(csr_enc,  5,  5) * 32);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   const uint64_t VA = base + offset;

   for (int i = 0; i < count; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         for (int r = 0; r < 2; r++) {
            uint16_t * golden_data = (uint16_t*) (VA + (i * 2 + r) * stride);
            uint16_t * rtl_data = (uint16_t*) sec_l1scp[(dst+i) % L1SCP_NUM_LINES];
            for (int c = 0; c < 16; c++) {
               TL_ACTION_WRAPPER(action, rtl_data[c*2 + r], golden_data[c])
            }
         }
      }
   }
   return PASS;
}

int do_tl_tra8(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint8_t);
   uint8_t (*sec_l1scp)[elems_per_scp_line] = (uint8_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t base   = (sm_get_bits(csr_enc, 47,  6) * 64);
   const uint64_t offset = (sm_get_bits(csr_enc,  5,  4) * 16);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   const uint64_t VA = base + offset;
   for (int i = 0; i < count ; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         uint8_t * rtl_data = (uint8_t*) sec_l1scp[(dst + i) % L1SCP_NUM_LINES];
         for (int r = 0; r < 64; r++) {
            uint8_t * golden_data = (uint8_t*) (VA + r*stride + i);
            TL_ACTION_WRAPPER(action, rtl_data[r], golden_data[0])
         }
      }
   }
   return PASS;
}

int do_tl_tra16(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint16_t);
   uint16_t (*sec_l1scp)[elems_per_scp_line] = (uint16_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t base   = (sm_get_bits(csr_enc, 47,  6) * 64);
   const uint64_t offset = (sm_get_bits(csr_enc,  5,  5) * 32);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   const uint64_t VA = base + offset;
   for (int i = 0; i < count ; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         uint16_t * rtl_data = (uint16_t*) sec_l1scp[(dst + i) % L1SCP_NUM_LINES];
         for (int r = 0; r < 32; r++) {
            uint16_t * golden_data = (uint16_t*) (VA + r*stride + i*2);
            TL_ACTION_WRAPPER(action, rtl_data[r], golden_data[0])
         }
      }
   }
   return PASS;
}

int do_tl_tra32(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const int elems_per_scp_line = L1SCP_LINE_SIZE_BYTES / sizeof(uint32_t);
   uint32_t (*sec_l1scp)[elems_per_scp_line] = (uint32_t (*)[elems_per_scp_line]) l1scp;

   const uint64_t usemsk = sm_get_bits(csr_enc, 63, 63);
   const uint64_t dst    = sm_get_bits(csr_enc, 58, 53);
   const uint64_t base   = (sm_get_bits(csr_enc, 47,  6) * 64);
   const      int count  = (sm_get_bits(csr_enc,  3,  0) + 1);
   const uint64_t stride = (sm_get_bits(x31, 47, 6) * 64);

   const uint64_t VA = base;
   for (int i = 0; i < count ; i++) {
      if (!usemsk || (sm_get_bits(tensor_mask, i, i) == 1)) {
         uint32_t * rtl_data = (uint32_t*) sec_l1scp[(dst + i) % L1SCP_NUM_LINES];
         for (int r = 0; r < 16; r++) {
            uint32_t * golden_data = (uint32_t*) (VA + r*stride + i*4);
            TL_ACTION_WRAPPER(action, rtl_data[r], golden_data[0])
         }
      }
   }
   return PASS;
}

int action_tl_wrapper(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* l1scp, int action) {
   const uint64_t config = sm_get_bits(csr_enc, 61, 59);
   switch (config) {
      case TLOAD_TENA:
         return do_tl      (csr_enc, x31, tensor_mask, l1scp, action);
      case TLOAD_INT8:
         return do_tl_int8 (csr_enc, x31, tensor_mask, l1scp, action);
      case TLOAD_INT16:
         return do_tl_int16(csr_enc, x31, tensor_mask, l1scp, action);
      case TLOAD_TRA8:
         return do_tl_tra8 (csr_enc, x31, tensor_mask, l1scp, action);
      case TLOAD_TRA16:
         return do_tl_tra16(csr_enc, x31, tensor_mask, l1scp, action);
      case TLOAD_TRA32:
         return do_tl_tra32(csr_enc, x31, tensor_mask, l1scp, action);
   }
   return PASS;
}

void execute_tl(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp) {
   action_tl_wrapper(csr_enc, x31, tensor_mask, pre_l1scp, EXECUTE);
}

void check_tl(uint64_t csr_enc, uint64_t x31, uint64_t tensor_mask, uint64_t* pre_l1scp) {
   action_tl_wrapper(csr_enc, x31, tensor_mask, pre_l1scp, CHECK);
}

int tl_selfchecking(et_tensor_load_conf_t * conf, uint64_t tmask, uint64_t* pre_data, uint64_t* post_data) {
   uint64_t csr_enc, x31_val;

   csr_enc = (((uint64_t)conf->use_tmask & 1) << 63) |
             (((uint64_t)conf->use_coop & 1)  << 62) |
             ((conf->transformation & 0x7) << 59)    |
             ((conf->dst_start & 0x3F) << 53)        |
             (((uint64_t)conf->use_tenb & 0x1) << 52)|
             ((conf->addr & 0xFFFFFFFFFFC0ULL))      |
             ((conf->offset & 0x3) << 4)             |
             ((conf->num_lines & 0xF));

   x31_val = (conf->stride & 0xFFFFFFFFFFC0ULL) | (conf->id & 0x1);

   // Check the whole L1SCP content
   execute_tl(csr_enc, x31_val, tmask, pre_data);
   return compare_l1scp(pre_data, post_data);
}

