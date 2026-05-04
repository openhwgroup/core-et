
pb_start dump_l1scp(pb_rx1, pb_rx2, pb_rx3, pb_mem_addr)
if (ET_TEST_ENV.SCP_ENABLE == 1):
   tensor_load_wait(x31_reg_value = 0)
   tensor_load_wait(x31_reg_value = 1)

   li pb_rx1, 0x0079000000000000
   la pb_rx2, pb_mem_addr
   add pb_rx1, pb_rx1, pb_rx2
   add pb_rx3, x31, x0
   li x31, 0x40

   ; SCP Lines [0..15]
   csrrw x0, tensor_store, pb_rx1

   li pb_rx2, 0x1000000000000000

   ; SCP Lines [16..31]
   add pb_rx1, pb_rx1, pb_rx2
   addi pb_rx1, pb_rx1, 1024
   csrrw x0, tensor_store, pb_rx1

   ; SCP Lines [32..48]
   add pb_rx1, pb_rx1, pb_rx2
   addi pb_rx1, pb_rx1, 1024
   csrrw x0, tensor_store, pb_rx1
   add x31, pb_rx3, x0

   tensor_store_wait()
endif
pb_end

pb_start dump_tl_l1scp_lines(pb_rx1, pb_rx2, pb_mem_addr, pb_ts_csr_val, pb_ts_start_line)
if (ET_TEST_ENV.SCP_ENABLE == 1):
   tensor_load_wait(x31_reg_value = 0)
   tensor_load_wait(x31_reg_value = 1)

   ; Load Address
   la pb_rx2, pb_mem_addr
   ; Set the proper memory offset
   li pb_rx1, pb_ts_start_line
   slli pb_rx1, pb_rx1, 6
   add pb_rx2, pb_rx2, pb_rx1
   ; Set the mem address in the csr value
   li pb_rx1, pb_ts_csr_val
   add pb_rx1, pb_rx1, pb_rx2
   ; Backup x31
   add pb_rx2, x31, x0
   li x31, 0x40

   csrrw x0, tensor_store, pb_rx1

   ; Restore x31
   add x31, x0, pb_rx2
   tensor_store_wait()
endif
pb_end

pb_start dump_vpu_rf(_pb_rx1, _pb_mem_addr)
   la _pb_rx1, _pb_mem_addr

   tensor_fma_wait()
   tensor_store_wait()
   tensor_reduce_wait()
   tensor_quant_wait()

   fsq2  f0,   0(_pb_rx1)
   fsq2  f1,  32(_pb_rx1)
   fsq2  f2,  64(_pb_rx1)
   fsq2  f3,  96(_pb_rx1)
   fsq2  f4, 128(_pb_rx1)
   fsq2  f5, 160(_pb_rx1)
   fsq2  f6, 192(_pb_rx1)
   fsq2  f7, 224(_pb_rx1)
   fsq2  f8, 256(_pb_rx1)
   fsq2  f9, 288(_pb_rx1)
   fsq2 f10, 320(_pb_rx1)
   fsq2 f11, 352(_pb_rx1)
   fsq2 f12, 384(_pb_rx1)
   fsq2 f13, 416(_pb_rx1)
   fsq2 f14, 448(_pb_rx1)
   fsq2 f15, 480(_pb_rx1)
   fsq2 f16, 512(_pb_rx1)
   fsq2 f17, 544(_pb_rx1)
   fsq2 f18, 576(_pb_rx1)
   fsq2 f19, 608(_pb_rx1)
   fsq2 f20, 640(_pb_rx1)
   fsq2 f21, 672(_pb_rx1)
   fsq2 f22, 704(_pb_rx1)
   fsq2 f23, 736(_pb_rx1)
   fsq2 f24, 768(_pb_rx1)
   fsq2 f25, 800(_pb_rx1)
   fsq2 f26, 832(_pb_rx1)
   fsq2 f27, 864(_pb_rx1)
   fsq2 f28, 896(_pb_rx1)
   fsq2 f29, 928(_pb_rx1)
   fsq2 f30, 960(_pb_rx1)
   fsq2 f31, 992(_pb_rx1)

pb_end

pb_start dump_vpu_rf_tenc(pb_rx1, pb_rx2, pb_vpu_rf_mem_addr, pb_vpu_tenc_mem_addr)
   ; Dump VPU RF
   dump_vpu_rf(_pb_rx1 = pb_rx1, _pb_mem_addr = pb_vpu_rf_mem_addr)

   ; Dump TenC
   la pb_rx1, pb_vpu_tenc_mem_addr
   csrrs pb_rx2, tensor_mask, x0 ; Backup tensor_mask

   ; Do a TFMA to dump TenC to VPU RF
   csrrw x0, tensor_mask, x0
   li pb_rx1, 0x81f8000000800006
   csrrw x0, tensor_fma, pb_rx1
   csrwi tensor_wait, 7
   ; Dump VPU RF content (tenC) to memory
   dump_vpu_rf(_pb_rx1 = pb_rx1, _pb_mem_addr = pb_vpu_tenc_mem_addr)

   ; Restore VPU RF previous state
   la pb_rx1, pb_vpu_rf_mem_addr

   flq2  f0,   0(pb_rx1)
   flq2  f1,  32(pb_rx1)
   flq2  f2,  64(pb_rx1)
   flq2  f3,  96(pb_rx1)
   flq2  f4, 128(pb_rx1)
   flq2  f5, 160(pb_rx1)
   flq2  f6, 192(pb_rx1)
   flq2  f7, 224(pb_rx1)
   flq2  f8, 256(pb_rx1)
   flq2  f9, 288(pb_rx1)
   flq2 f10, 320(pb_rx1)
   flq2 f11, 352(pb_rx1)
   flq2 f12, 384(pb_rx1)
   flq2 f13, 416(pb_rx1)
   flq2 f14, 448(pb_rx1)
   flq2 f15, 480(pb_rx1)
   flq2 f16, 512(pb_rx1)
   flq2 f17, 544(pb_rx1)
   flq2 f18, 576(pb_rx1)
   flq2 f19, 608(pb_rx1)
   flq2 f20, 640(pb_rx1)
   flq2 f21, 672(pb_rx1)
   flq2 f22, 704(pb_rx1)
   flq2 f23, 736(pb_rx1)
   flq2 f24, 768(pb_rx1)
   flq2 f25, 800(pb_rx1)
   flq2 f26, 832(pb_rx1)
   flq2 f27, 864(pb_rx1)
   flq2 f28, 896(pb_rx1)
   flq2 f29, 928(pb_rx1)
   flq2 f30, 960(pb_rx1)
   flq2 f31, 992(pb_rx1)

   ; Restore previous tensor_mask value
   csrrs x0, tensor_mask, pb_rx2
pb_end

pb_start flush_vpu_rf(pb_rx1, pb_rx2, pb_mem_addr)
   ; Setup Flush CSR Enc
   la pb_rx1, pb_mem_addr
   li pb_rx2, 0xc0000000000000f
   or pb_rx2, pb_rx2, pb_rx1

   ; Backup X31
   add pb_rx1, x0, x31

   ; Set X31 (stride)
   li x31, 64

   ; Write to Flush CSR
   csrrw x0, flush_va, pb_rx2

   ; Restore X31
   add x31, x0, pb_rx1
pb_end

pb_start flush_et_info(pb_rx1, pb_rx2, pb_mem_addr)
   ; Setup Flush CSR Enc
   la pb_rx1, pb_mem_addr
   li pb_rx2, 0xc00000000000000
   or pb_rx2, pb_rx2, pb_rx1

   ; Write to Flush CSR
   csrrw x0, flush_va, pb_rx2
pb_end

pb_start sm_sign_hint(_pb_rx1)
   slti x0, _pb_rx1, 0x516
pb_end

pb_start backup_tensorCSR2stack()
   ; x2 is the StackPointer
   if ((cpuid%2) == 0):
      addi x2, x2, -40
      csrrs x1, tensor_mask, x0
      sd    x1, 0(x2)
      csrrs x1, tensor_conv_size, x0
      sd    x1, 8(x2)
      csrrs x1, tensor_conv_ctrl, x0
      sd    x1, 16(x2)
      csrrs x1, tensor_cooperation, x0
      sd    x1, 24(x2)
      csrrs x1, tensor_error, x0
      sd    x1, 32(x2)
   endif
pb_end

pb_start restore_tensorCSR2stack()
   ; x2 is the StackPointer
   if ((cpuid%2) == 0):
      ; Only the tensor_mask must be restored
      ld x1, 0(x2)
      csrrw x0, tensor_mask, x1
      addi x2, x2, 40
   endif
pb_end

pb_start backup_maskRF2stack()
   ; x2 is the StackPointer
   addi     x2, x2, -8
   mova.x.m x1
   sd       x1, 0(x2)
pb_end

pb_start restore_maskRF2stack()
   ; x2 is the StackPointer
   addi     x2, x2, 8
pb_end

pb_start backup_SIMDstate2stack()
   ; x2 is the StackPointer
   addi x2, x2, -24
   csrrs x1, gsc_progress, x0
   sd x1, 0(x2)
   csrrs x1, fcsr, x0
   sd x1, 8(x2)
   csrrs x1, fflags, x0
   sd x1, 16(x2)
pb_end

pb_start restore_SIMDstate2stack()
   ; x2 is the StackPointer
   ld x1, 0(x2)
   csrrw x0, gsc_progress, x1
   ld x1, 8(x2)
   csrrw x0, fcsr, x1
   ld x1, 16(x2)
   csrrw x0, fflags, x1
   addi x2, x2, 24
pb_end

pb_start backup_intRF2stack()
   ; x2 is the StackPointer
   ; 8 bytes-per-register * 32 registers = 256
   addi x2, x2, -256
   sd  x0,   0(x2)
   sd  x1,   8(x2)
   sd  x2,  16(x2)
   sd  x3,  24(x2)
   sd  x4,  32(x2)
   sd  x5,  40(x2)
   sd  x6,  48(x2)
   sd  x7,  56(x2)
   sd  x8,  64(x2)
   sd  x9,  72(x2)
   sd x10,  80(x2)
   sd x11,  88(x2)
   sd x12,  96(x2)
   sd x13, 104(x2)
   sd x14, 112(x2)
   sd x15, 120(x2)
   sd x16, 128(x2)
   sd x17, 136(x2)
   sd x18, 144(x2)
   sd x19, 152(x2)
   sd x20, 160(x2)
   sd x21, 168(x2)
   sd x22, 176(x2)
   sd x23, 184(x2)
   sd x24, 192(x2)
   sd x25, 200(x2)
   sd x26, 208(x2)
   sd x27, 216(x2)
   sd x28, 224(x2)
   sd x29, 232(x2)
   sd x30, 240(x2)
   sd x31, 248(x2)
pb_end

pb_start restore_intRF2stack()
   ; x2 is the StackPointer
   ld  x0,   0(x2)
   ld  x1,   8(x2)
   ld  x2,  16(x2)
   ld  x3,  24(x2)
   ld  x4,  32(x2)
   ld  x5,  40(x2)
   ld  x6,  48(x2)
   ld  x7,  56(x2)
   ld  x8,  64(x2)
   ld  x9,  72(x2)
   ld x10,  80(x2)
   ld x11,  88(x2)
   ld x12,  96(x2)
   ld x13, 104(x2)
   ld x14, 112(x2)
   ld x15, 120(x2)
   ld x16, 128(x2)
   ld x17, 136(x2)
   ld x18, 144(x2)
   ld x19, 152(x2)
   ld x20, 160(x2)
   ld x21, 168(x2)
   ld x22, 176(x2)
   ld x23, 184(x2)
   ld x24, 192(x2)
   ld x25, 200(x2)
   ld x26, 208(x2)
   ld x27, 216(x2)
   ld x28, 224(x2)
   ld x29, 232(x2)
   ld x30, 240(x2)
   ld x31, 248(x2)
   ; 8 bytes-per-register * 32 registers = 256
   addi x2, x2, 256
pb_end

pb_start backup_fpRF2stack()
   ; x2 is the StackPointer
   ; 4 bytes-per-register * 8 lanes * 32 registers = 1024
   addi x2, x2, -1024
   fsq2  f0,   0(x2)
   fsq2  f1,  32(x2)
   fsq2  f2,  64(x2)
   fsq2  f3,  96(x2)
   fsq2  f4, 128(x2)
   fsq2  f5, 160(x2)
   fsq2  f6, 192(x2)
   fsq2  f7, 224(x2)
   fsq2  f8, 256(x2)
   fsq2  f9, 288(x2)
   fsq2 f10, 320(x2)
   fsq2 f11, 352(x2)
   fsq2 f12, 384(x2)
   fsq2 f13, 416(x2)
   fsq2 f14, 448(x2)
   fsq2 f15, 480(x2)
   fsq2 f16, 512(x2)
   fsq2 f17, 544(x2)
   fsq2 f18, 576(x2)
   fsq2 f19, 608(x2)
   fsq2 f20, 640(x2)
   fsq2 f21, 672(x2)
   fsq2 f22, 704(x2)
   fsq2 f23, 736(x2)
   fsq2 f24, 768(x2)
   fsq2 f25, 800(x2)
   fsq2 f26, 832(x2)
   fsq2 f27, 864(x2)
   fsq2 f28, 896(x2)
   fsq2 f29, 928(x2)
   fsq2 f30, 960(x2)
   fsq2 f31, 992(x2)
pb_end

pb_start restore_fpRF2stack()
   ; x2 is the StackPointer
   flq2  f0,   0(x2)
   flq2  f1,  32(x2)
   flq2  f2,  64(x2)
   flq2  f3,  96(x2)
   flq2  f4, 128(x2)
   flq2  f5, 160(x2)
   flq2  f6, 192(x2)
   flq2  f7, 224(x2)
   flq2  f8, 256(x2)
   flq2  f9, 288(x2)
   flq2 f10, 320(x2)
   flq2 f11, 352(x2)
   flq2 f12, 384(x2)
   flq2 f13, 416(x2)
   flq2 f14, 448(x2)
   flq2 f15, 480(x2)
   flq2 f16, 512(x2)
   flq2 f17, 544(x2)
   flq2 f18, 576(x2)
   flq2 f19, 608(x2)
   flq2 f20, 640(x2)
   flq2 f21, 672(x2)
   flq2 f22, 704(x2)
   flq2 f23, 736(x2)
   flq2 f24, 768(x2)
   flq2 f25, 800(x2)
   flq2 f26, 832(x2)
   flq2 f27, 864(x2)
   flq2 f28, 896(x2)
   flq2 f29, 928(x2)
   flq2 f30, 960(x2)
   flq2 f31, 992(x2)
   ; 4 bytes-per-register * 8 lanes * 32 registers = 1024
   addi x2, x2, 1024
pb_end

pb_start backup_tenc2stack()
if ((cpuid%2) == 0):
if (ET_TEST_ENV.SCP_ENABLE == 1):
   csrrw x0, tensor_mask, x0
   ; Uses tensor_mask
   ; Write result to the VPU RF
   ; Config is TensorIMA8A32
   ; Max a_rows and b_cols value
   li x1, 0x81f8000000800006
   csrrw x0, tensor_fma, x1
   csrwi tensor_wait, 7
   backup_fpRF2stack()
endif
endif
pb_end

pb_start restore_tenc2stack()
if ((cpuid%2) == 0):
if (ET_TEST_ENV.SCP_ENABLE == 1):
   ; backup_fpRF2stack() increases the SP by 1024
   ; we will not restore the data but we want to
   ; restore the SP position
   addi x2, x2, 1024
endif
endif
pb_end

pb_start generate_signature(pb_rx1, pb_rx2, pb_et_info_addr, pb_sign_addr)
   sm_set_checkpoint_tail_info(regx1 = pb_rx1, et_info_addr = pb_et_info_addr)

   backup_intRF2stack()
   backup_fpRF2stack()
   backup_tensorCSR2stack()
   backup_maskRF2stack()
   backup_SIMDstate2stack()
   backup_tenc2stack()

   sm_set_checkpoint_head_info(regx1 = pb_rx1, et_info_addr = pb_et_info_addr)

   ; Sting ABI != GCC ABI
   ; So we have to backup the registers
   rv_save_abi_xregs()
   jump_to_c_funct(c_arg_addr = pb_et_info_addr, c_funct = et_gen_crc16)
   ; C function return value is set on x10
   ; In this case, the CRC16 checksum is on x10
   sm_sign_hint(_pb_rx1 = x10)

   add pb_rx1, x0, x10
   la  pb_rx2, pb_sign_addr
   sh  pb_rx1, 0(pb_rx2)

   ; Restore back the registers
   rv_restore_abi_xregs()

   restore_tenc2stack()
   restore_SIMDstate2stack()
   restore_maskRF2stack()
   restore_tensorCSR2stack()
   restore_fpRF2stack()
   restore_intRF2stack()
pb_end
