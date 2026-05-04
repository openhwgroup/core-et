
#include "trace.h"
#include "sting_lib.h"
#include <stdint.h>
#include <stdio.h>
#include "crc_ccitt.h"

//CRC CCITT uses init value 0xffff and polynomial 0x1021
inline uint16_t process_byte_crc_ccitt(uint16_t crc, int8_t c) {
    uint16_t masked_c  = 0x00ff & (uint16_t) c;
    uint16_t idx = (crc >> 8) ^ masked_c;
    crc = (crc << 8) ^ crc_ccitt_table[idx];
    return crc;
}

uint16_t gen_crc_ccitt(uint8_t* ptr, int size) {
   uint16_t crc_val = 0xffff;
   for (int i = 0; i < size; i++) {
       crc_val = process_byte_crc_ccitt(crc_val, *ptr);
       ptr++;
   }
   return crc_val;
}

typedef struct gen_sign_data {
   uint64_t head;    // snp_info[0]
   uint64_t tail;    // snp_info[8]
} gen_sign_data;

extern "C" uint32_t et_gen_crc16(gen_sign_data* snp_info);

uint32_t et_gen_crc16(gen_sign_data* snp_info) {

   const int length = (snp_info->tail - snp_info->head) / sizeof(int8_t);
   uint64_t* pre_data = (uint64_t *) snp_info->head;  //INT RF content
   return gen_crc_ccitt((uint8_t*) pre_data, length);
}
