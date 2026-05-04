
pb_start loop_start(random_num, rega, regb)
   li   rega, random_num
   slli rega, rega, 16
   li   regb, 0xF00D
   add  rega, rega, regb
   add  rega, x0, x0
pb_end


pb_start loop_end(random_num, rega, regb)
   li   rega, random_num
   slli rega, rega, 16
   li   regb, 0xBABE
   add  rega, rega, regb
   add  rega, x0, x0
pb_end

pb_start backup_registers(bak_mem, rega)
   la rega, bak_mem

   sd  x1,   0(rega)
   sd  x2,   8(rega)
   sd  x3,  16(rega)
   sd  x4,  24(rega)
   sd  x5,  32(rega)
   sd  x6,  40(rega)
   sd  x7,  48(rega)
   sd  x8,  56(rega)
   sd  x9,  64(rega)
   sd x10,  72(rega)
   sd x11,  80(rega)
   sd x12,  88(rega)
   sd x13,  96(rega)
   sd x14, 104(rega)
   sd x15, 112(rega)
   sd x16, 120(rega)
   sd x17, 128(rega)
   sd x18, 136(rega)
   sd x19, 144(rega)
   sd x20, 152(rega)
   sd x21, 160(rega)
   sd x22, 168(rega)
   sd x23, 176(rega)
   sd x24, 184(rega)
   sd x25, 192(rega)
   sd x26, 200(rega)
   sd x27, 208(rega)
   sd x28, 216(rega)
   sd x29, 224(rega)
   sd x30, 232(rega)
   sd x31, 240(rega)
pb_end

pb_start restore_registers(bak_mem, rega)
   la rega, bak_mem

   ld  x1,   0(rega)
   ld  x2,   8(rega)
   ld  x3,  16(rega)
   ld  x4,  24(rega)
   ld  x5,  32(rega)
   ld  x6,  40(rega)
   ld  x7,  48(rega)
   ld  x8,  56(rega)
   ld  x9,  64(rega)
   ld x10,  72(rega)
   ld x11,  80(rega)
   ld x12,  88(rega)
   ld x13,  96(rega)
   ld x14, 104(rega)
   ld x15, 112(rega)
   ld x16, 120(rega)
   ld x17, 128(rega)
   ld x18, 136(rega)
   ld x19, 144(rega)
   ld x20, 152(rega)
   ld x21, 160(rega)
   ld x22, 168(rega)
   ld x23, 176(rega)
   ld x24, 184(rega)
   ld x25, 192(rega)
   ld x26, 200(rega)
   ld x27, 208(rega)
   ld x28, 216(rega)
   ld x29, 224(rega)
   ld x30, 232(rega)
   ld x31, 240(rega)
pb_end
