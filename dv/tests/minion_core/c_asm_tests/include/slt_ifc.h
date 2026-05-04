/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __SLT_IFC__
#define __SLT_IFC__

typedef struct{
    uint32_t active_shires;
    uint64_t shire_mask;

    uint32_t active_minions_per_shire;
    uint64_t minion_mask;

    uint32_t active_threads_per_minion;
    uint32_t lead_hart_id;
} _SLT_INFO;

typedef struct{
    _SLT_INFO info;
    uint8_t padding[64-sizeof(_SLT_INFO)];
} SLT_INFO;

SLT_INFO __attribute__((section ("slt_info"))) slt_test;
/*
SLT_INFO __attribute__((section ("slt_info"))) slt_test {
    {
        2,   //active shires
        0x3, //shire mask

        32,         //active minions per shire
        0xffffffff, //minions mask

        2,   //active threads per minion
        0   //lead hart id
    },
    {0}  //padding
};
*/

inline uint32_t amo_read_w(uint32_t *addr) {
    uint32_t read_data;
    __asm__ __volatile__(
       "amoorg.w %[read_data], x0, (%[addr])\n"
       : [read_data] "=r" (read_data)
       : [addr] "r" (addr)
       :
    );
    return read_data;
}

inline uint32_t amo_read_d(uint64_t *addr) {
    uint64_t read_data;
    __asm__ __volatile__(
       "amoorg.d %[read_data], x0, (%[addr])\n"
       : [read_data] "=r" (read_data)
       : [addr] "r" (addr)
       :
    );
    return read_data;
}

uint32_t get_number_active_shires() {
    return amo_read_w(&slt_test.info.active_shires);
}

uint64_t get_shire_mask() {
    return amo_read_d(&slt_test.info.shire_mask);
}

uint32_t get_number_active_minions_per_shires() {
    return amo_read_w(&slt_test.info.active_minions_per_shire);
}

uint64_t get_minion_mask() {
    return amo_read_d(&slt_test.info.minion_mask);
}

uint32_t get_number_active_threads_per_minion() {
    return amo_read_w(&slt_test.info.active_threads_per_minion);
}

uint32_t get_number_active_harts_per_shire() {
    return get_number_active_minions_per_shires()*get_number_active_threads_per_minion();
}

uint32_t get_lead_hart_id() {
    return amo_read_w(&slt_test.info.lead_hart_id);
}
#endif
