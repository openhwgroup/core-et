#include "mm_minion_cold_reset_sequence.h"

typedef struct dvfs_data {
   uint64_t rand_freq; // snp_info[0]
   uint64_t ignore_0;  // snp_info[1]
   uint64_t ignore_1;  // snp_info[2]
   uint64_t ignore_2;  // snp_info[3]
   uint64_t ignore_3;  // snp_info[4]
   uint64_t ignore_4;  // snp_info[5]
   uint64_t ignore_5;  // snp_info[6]
   uint64_t ignore_6;  // snp_info[7]
};

#define NO_WAIT_FOR_LOCK 0
#define WAIT_FOR_LOCK 1

extern "C" int sting_init_dvfs();

int sting_init_dvfs() {
   pll_normalize_and_turn_of_normalization();
   update_minion_pll_freq(750, NO_WAIT_FOR_LOCK);
   while(!(read_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
   return 1;
}

extern "C" int sting_update_minion_pll_freq(dvfs_data* snippet_info);

int sting_update_minion_pll_freq(dvfs_data* snippet_info) {
   uint64_t new_freq = snippet_info->rand_freq;
   update_minion_pll_freq(new_freq, NO_WAIT_FOR_LOCK);
   while(!(read_esr(PP_MACHINE, 0, REGION_OTHER, SHIRE_PLL_READ_DATA) & 0x20000));
   return 1;
}
