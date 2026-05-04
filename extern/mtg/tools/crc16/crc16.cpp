
#include <stdint.h>
#include <stdio.h>
#include "crc_ccitt.h"
#include "../mtg_printf/mtg_printf.h"

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

uint64_t read_uint16t(uint16_t* ptr) {
   uint64_t ptr_val = (uint64_t) ptr;
   uint8_t aligned = (uint8_t) (ptr_val & 0x3);
   if (aligned != 0) {
      ptr = (uint16_t*) ((ptr_val >> 2) << 2);
   }
   uint32_t val;
#ifdef GEN_CHECKPOINT
   __asm__("amoorg.w %[val], x0, (%[ptr])" : [val] "=r" (val) : [ptr] "r" (ptr) : );
#else
   __asm__("lhu %[val], 0(%[ptr])" : [val] "=r" (val) : [ptr] "r" (ptr) : );
#endif
   if (aligned == 0) {
      return (uint16_t) (val & 0xffff);
   }
   else {
      return (uint16_t) (val >> 16);
   }
}

#ifdef UART_PRINTS
extern "C" uint16_t et_gen_crc16(gen_sign_data* snp_info, uint16_t* golden_value);
uint16_t et_gen_crc16(gen_sign_data* snp_info, uint16_t* golden_value) {
#else
extern "C" uint16_t et_gen_crc16(gen_sign_data* snp_info);
uint16_t et_gen_crc16(gen_sign_data* snp_info) {
#endif

   const int length = (snp_info->tail - snp_info->head) / sizeof(int8_t);
   uint64_t* pre_data = (uint64_t *) snp_info->head;  //INT RF content
   uint16_t crc16 = gen_crc_ccitt((uint8_t*) pre_data, length);
#ifdef UART_PRINTS
   uint16_t gval = read_uint16t(golden_value);
   mtg_printf("RTL:0x%x SYS:0x%x\n", crc16, gval);
#endif
   return crc16;
}

#ifdef UART_PRINTS
uint16_t dbg_gen_crc_ccitt(uint8_t* ptr, int size) {
   uint16_t crc_val = 0xffff;
   uint64_t* print_ptr = (uint64_t*) ptr;
   uint8_t cntr = 0;
   const int mask = 0x7;
   int pre_size = size & (~mask);
   for (int i = 0; i < pre_size; i++) {
       crc_val = process_byte_crc_ccitt(crc_val, *ptr);
       ptr++;
       cntr++;
       if (cntr == 8) {
          mtg_printf("RTL:MEM64[0x%lx] = 0x%lx\n", (uint64_t) print_ptr, *print_ptr);
          cntr = 0;
          print_ptr = (uint64_t*) ptr;
       }
   }

   int post_size = size%8;
   for (int i = 0; i < post_size; i++) {
       crc_val = process_byte_crc_ccitt(crc_val, *ptr);
       ptr++;
       mtg_printf("RTL:MEM8[0x%lx] = 0x%lx\n", (uint64_t) ptr, *ptr);
   }
   return crc_val;
}

extern "C" uint16_t dbg_et_gen_crc16(gen_sign_data* snp_info, uint16_t* golden_value);
uint16_t dbg_et_gen_crc16(gen_sign_data* snp_info, uint16_t* golden_value) {
   const int length = (snp_info->tail - snp_info->head) / sizeof(int8_t);
   uint64_t* pre_data = (uint64_t *) snp_info->head;  //INT RF content
   uint16_t crc16 = dbg_gen_crc_ccitt((uint8_t*) pre_data, length);
   uint16_t gval = read_uint16t(golden_value);
   mtg_printf("RTL:0x%x SYS:0x%x\n", crc16, gval);
   return crc16;
}
#endif
