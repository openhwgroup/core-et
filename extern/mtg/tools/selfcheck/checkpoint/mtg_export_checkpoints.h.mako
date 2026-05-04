
__attribute__((noinline)) void print_checkpoints() {
#ifdef GEN_CHECKPOINT
   __asm__ __volatile__(
      "add x0, x0, x0\n"
   );
#else
   __asm__ __volatile__(
      "jalr zero, 0(ra)"
   );
#endif
   uint16_t* crc16_mem = (uint16_t*) ${hex(addr)};
   int num_chkpts = ${size}/sizeof(uint16_t);

   _mtg_printf("CRC16: /* CRC16 signatures extracted from sys_emu */\n");
   _mtg_printf("CRC16: uint16_t __attribute__((section (\"mtg_${CRC_REGION}\"))) ${CRC_REGION}[%d] = {\n", num_chkpts);

   for (int i = 0; i < (num_chkpts-1); i++) {
      _mtg_printf("CRC16:    0x%lx,\n", *crc16_mem);
      crc16_mem++;
   }
   _mtg_printf("CRC16:    0x%lx\n", *crc16_mem);
   _mtg_printf("CRC16: };\n", num_chkpts);
}
