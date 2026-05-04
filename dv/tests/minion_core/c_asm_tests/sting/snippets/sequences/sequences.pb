import "$MINION_DIAGS/sting/snippets/sequences/seq_avow.pb"

pb_start NO_INST()
   RANDINST 0:0
pb_end

pb_start seq_rand_inst()
   SELECT 1 {
      10 : NO_INST()
       5 : RANDINST 1:5
       1 : RANDINST 5:10
   }
pb_end

pb_start seq_rand_inst_wrapper(pb_rx_x31, pb_uses_x31)
   if (ET_TEST_ENV.AVOW_MODE == 1):
      SELECT 1 {
         1 : avow_cov_op()
         5 : NO_INST()
      }
   endif
pb_end

pb_start pre_reading_l1scp()
   tensor_load_wait(x31_reg_value = 0)
   tensor_load_wait(x31_reg_value = 1)
pb_end

pb_start pre_writting_l1scp()
   tensor_fma_wait()
   tensor_quant_wait()
   tensor_store_wait() ; Tensor Store From SCP
   pre_reading_l1scp() ; Required for WAW dependencies
pb_end

pb_start post_writting_l1scp()
   tensor_load_wait(x31_reg_value = 0)
   tensor_load_wait(x31_reg_value = 1)
pb_end

pb_start pre_reading_vpurf()
   tensor_reduce_wait()
   tensor_fma_wait()
   tensor_quant_wait()
pb_end

pb_start pre_writting_vpurf()
   tensor_reduce_wait()
   tensor_store_wait() ; Tensor Store
pb_end

pb_start seq_ts(_cpu_id, _rx1, _ts_csr, _ts_x31, _self_check)
      ; Tensor Store
      add x31, _ts_x31, x0
   if (_self_check == 0) :
      csrrw x0, tensor_store, _ts_csr
   else :
      csrrw _ts_x31, tensor_store, _ts_csr
   endif
   if ((_cpu_id%2) == 1):
      test_fail(regz = _rx1)
   endif
pb_end

pb_start seq_tl(_cpu_id, _rx1, _tl_csr, _tl_x31, _self_check)
      ; Tensor Load
      add x31, _tl_x31, x0
      if (_self_check == 0) :
         csrrw x0, tensor_load, _tl_csr
      else :
         csrrw _tl_x31, tensor_load, _tl_csr
      endif
   if ((_cpu_id%2) == 1):
      test_fail(regz = _rx1)
   endif
pb_end

pb_start seq_tfma(_cpu_id, _rx1, _tfma_csr, _self_check)
      ; Tensor FMA
   if (_self_check == 0) :
      csrrw x0, tensor_fma, _tfma_csr
   else :
      csrrw _tfma_csr, tensor_fma, _tfma_csr
   endif
   if ((_cpu_id%2) == 1):
      test_fail(regz = _rx1)
   endif
pb_end

pb_start seq_tensor_coop(_cpu_id, _rx1, _coop_mask_val, _self_check)
   li _rx1, _coop_mask_val
   csrw tensor_cooperation, _rx1
   if ((_cpu_id%2) == 1):
      test_fail(regz = _rx1)
   endif
pb_end

pb_start ts_tlb_tfma(pb_cpu_id, pb_rx1, pb_ts_x31, pb_ts_csr, pb_tl_x31, pb_tl_csr, pb_tfma_csr)
   ; Tensor Store
   seq_ts  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _ts_csr   = pb_ts_csr, _ts_x31 = pb_ts_x31, _self_check = 1)
   ; Tensor Load TenB
   seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 1)
   ; Tensor FMA
   seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 1)
pb_end

pb_start mix_ts_tlb_tfma(pb_cpu_id, pb_rx1, pb_ts_x31, pb_ts_csr, pb_tl_x31, pb_tl_csr, pb_tfma_csr)
   SELECT 1 {
      9 : seq_ts  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _ts_csr   = pb_ts_csr, _ts_x31 = pb_ts_x31, _self_check = 0)
      2 : seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
      2 : seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 0)
   }
   SELECT 1 {
      2 : seq_ts  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _ts_csr   = pb_ts_csr, _ts_x31 = pb_ts_x31, _self_check = 0)
      9 : seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
      2 : seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 0)
   }
   SELECT 1 {
      2 : seq_ts  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _ts_csr   = pb_ts_csr, _ts_x31 = pb_ts_x31, _self_check = 0)
      2 : seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
      9 : seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 0)
   }
pb_end

pb_start clean_tlb_tfma(pb_cpu_id, pb_rx1, pb_tl_x31, pb_tl_csr, pb_tfma_csr)
   seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 1)
   seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 1)
pb_end

pb_start conv_tl(pb_cpu_id, pb_rx1, pb_tl_x31, pb_tl_csr, pb_conv_size_csr, pb_conv_ctrl_csr)
   add x31, pb_tl_x31, x0
   write_tensor_conv_size_ctrl(size = pb_conv_size_csr, ctrl = pb_conv_ctrl_csr)
   csrrw pb_tl_x31, tensor_load, pb_tl_csr
   if ((pb_cpu_id%2) == 1):
      test_fail(regz = pb_rx1)
   endif
pb_end

pb_start mix_conv_tl(pb_cpu_id, pb_rx1, pb_tl_x31, pb_tl_csr, pb_conv_size_csr, pb_conv_ctrl_csr)
   SELECT 1 {
      9 : csrw tensor_conv_size, pb_conv_size_csr
      2 : csrw tensor_conv_ctrl, pb_conv_ctrl_csr
      2 : seq_tl(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
   }
   seq_rand_inst()
   SELECT 1 {
      2 : csrw tensor_conv_size, pb_conv_size_csr
      9 : csrw tensor_conv_ctrl, pb_conv_ctrl_csr
      2 : seq_tl(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
   }
   seq_rand_inst()
   SELECT 1 {
      2 : csrw tensor_conv_size, pb_conv_size_csr
      2 : csrw tensor_conv_ctrl, pb_conv_ctrl_csr
      9 : seq_tl(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
   }
pb_end

pb_start tl_tfma(pb_cpu_id, pb_rx1, pb_tl_x31, pb_tl_csr, pb_tfma_csr)
   ; Tensor Load
   seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 1)
   ; Tensor FMA
   seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 1)
pb_end

pb_start mix_tl_tfma(pb_cpu_id, pb_rx1, pb_tl_x31, pb_tl_csr, pb_tfma_csr)
   SELECT 1 {
      1 : seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
      1 : seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 0)
   }
   SELECT 1 {
      1 : seq_tl  (_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tl_csr   = pb_tl_csr, _tl_x31 = pb_tl_x31, _self_check = 0)
      1 : seq_tfma(_cpu_id = pb_cpu_id, _rx1 = pb_rx1, _tfma_csr = pb_tfma_csr, _self_check = 0)
   }
pb_end

pb_start prepare_tensor_csr(pb_csr, pb_csr_val, pb_op_type, pb_rx_mem, pb_rx1, pb_mem_off)
   if (pb_op_type == ET_SEQ_OP.CSR_TLOAD || pb_op_type == ET_SEQ_OP.CSR_TLOAD_TENB || pb_op_type == ET_SEQ_OP.CSR_TLOAD_L2):
      li  pb_csr, pb_csr_val
      li  pb_rx1, 0x0000ffffffffffc0
      and pb_rx1, pb_rx_mem, pb_rx1
      or  pb_csr, pb_csr, pb_rx1
      if (pb_mem_off != 0):
         li rx1, pb_mem_off
         add pb_rx_mem, pb_rx_mem, rx1
      endif
   elif (pb_op_type == ET_SEQ_OP.CSR_TSTORE || pb_op_type == ET_SEQ_OP.CSR_TSTORE_SCP):
      SELECT 1 {
         1 : NO_INST()
         1 : addi pb_rx_mem, pb_rx_mem, 0x10
         1 : addi pb_rx_mem, pb_rx_mem, 0x20
         1 : addi pb_rx_mem, pb_rx_mem, 0x30
      }
      li  pb_csr, pb_csr_val
      li  pb_rx1, 0x0000fffffffffff0
      and pb_rx1, pb_rx_mem, pb_rx1
      or  pb_csr, pb_csr, pb_rx1
      if (pb_mem_off != 0):
         li rx1, pb_mem_off
         add pb_rx_mem, pb_rx_mem, rx1
      endif
   elif (pb_op_type == ET_SEQ_OP.CSR_TFMA || pb_op_type == ET_SEQ_OP.CSR_TFMA_TENB || pb_op_type == ET_SEQ_OP.CSR_TQUANT || pb_op_type == ET_SEQ_OP.CSR_TREDUCE || pb_op_type == ET_SEQ_OP.CSR_TCONV_SIZE || pb_op_type == ET_SEQ_OP.CSR_TCONV_CTRL):
      li pb_csr, pb_csr_val
   else:
      addi x0, x0, pb_op_type
      test_fail(regz = pb_csr)
   endif
pb_end

pb_start write_tensor_csr(pb_csr, pb_op_type)
   if (pb_op_type == ET_SEQ_OP.CSR_TLOAD || pb_op_type == ET_SEQ_OP.CSR_TLOAD_TENB):
      csrrw x0, tensor_load, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TFMA || pb_op_type == ET_SEQ_OP.CSR_TFMA_TENB):
      csrrw x0, tensor_fma, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TQUANT):
      csrrw x0, tensor_quant, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TSTORE || pb_op_type == ET_SEQ_OP.CSR_TSTORE_SCP):
      csrrw x0, tensor_store, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TREDUCE):
      csrrw x0, tensor_reduce, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TCONV_SIZE):
      csrrw x0, tensor_conv_size, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TCONV_CTRL):
      csrrw x0, tensor_conv_ctrl, pb_csr
   elif (pb_op_type == ET_SEQ_OP.CSR_TLOAD_L2):
      csrrw x0, tensor_load_l2scp, pb_csr
   else:
      addi x0, x0, pb_op_type
      test_fail(regz = pb_csr)
   endif
pb_end

pb_start write_tensor_wait(pb_wait_type)

   if ((pb_wait_type & 0x001) != 0):
      csrrwi x0, tensor_wait, 0
   endif
   if ((pb_wait_type & 0x002) != 0):
      csrrwi x0, tensor_wait, 1
   endif
   if ((pb_wait_type & 0x004) != 0):
      csrrwi x0, tensor_wait, 2
   endif
   if ((pb_wait_type & 0x008) != 0):
      csrrwi x0, tensor_wait, 3
   endif
   if ((pb_wait_type & 0x010) != 0):
      csrrwi x0, tensor_wait, 4
   endif
   if ((pb_wait_type & 0x020) != 0):
      csrrwi x0, tensor_wait, 5
   endif
   if ((pb_wait_type & 0x040) != 0):
      csrrwi x0, tensor_wait, 6
   endif
   if ((pb_wait_type & 0x080) != 0):
      csrrwi x0, tensor_wait, 7
   endif
   if ((pb_wait_type & 0x100) != 0):
      csrrwi x0, tensor_wait, 8
   endif
   if ((pb_wait_type & 0x200) != 0):
      csrrwi x0, tensor_wait, 9
   endif
   if ((pb_wait_type & 0x400) != 0):
      csrrwi x0, tensor_wait, 10
   endif

pb_end

pb_start inline_inst(inst_val)
if (ET_TEST_ENV.AVOW_MODE == 1):
   SELECT 1 {
      1 : avow_cov_op()
      5 : NO_INST()
   }
endif

   .word inst_val

if (ET_TEST_ENV.AVOW_MODE == 1):
   SELECT 1 {
      1 : avow_cov_op()
      5 : NO_INST()
   }
endif
pb_end

pb_start inline_comb_inst(pb_inst0_val, pb_inst1_val)
   SELECT 2 {
      1 : NO_INST()
      3 : inline_inst(inst_val = pb_inst0_val)
      3 : inline_inst(inst_val = pb_inst1_val)
      if (ET_TEST_ENV.AVOW_MODE == 1):
         2 : avow_cov_op()
      endif
   }
pb_end




pb_start avow_write_tensor_csr(glbl_bias, avow_csr, avow_op_type)
   SELECT 1 {
      3  : avow_cov_op()
      11 : NO_INST()
   }

   write_tensor_csr(pb_csr = avow_csr, pb_op_type = avow_op_type)

   SELECT 1 {
      3  : avow_cov_op()
      11 : NO_INST()
   }
pb_end

pb_start avow_inline_ins(pb_ins0, pb_ins1, pb_ins2, pb_ins3, pb_ins4)
   SELECT 1 {
      4 : inline_inst(inst_val = pb_ins0)
      4 : inline_inst(inst_val = pb_ins1)
      4 : inline_inst(inst_val = pb_ins2)
      4 : inline_inst(inst_val = pb_ins3)
      4 : inline_inst(inst_val = pb_ins4)
      1 : NO_INST()
   }

   SELECT 1 {
      8 : NO_INST()
      1 : inline_inst(inst_val = pb_ins0)
      1 : inline_inst(inst_val = pb_ins1)
      1 : inline_inst(inst_val = pb_ins2)
      1 : inline_inst(inst_val = pb_ins3)
      1 : inline_inst(inst_val = pb_ins4)
   }
pb_end

pb_start clean_amo_rd(pb_rn0, pb_rn1)
   li x13, pb_rn0
   li x14, pb_rn1
   xor x24, x13, x14
   slli x25, x13, 1
   xor x30, x14, x25

   ; Also set a predictable value to 'fd' used by atomic operations
   fcmovm.ps f28, f12, f27
   fcmovm.ps f29, f3, f12

pb_end

pb_start ow_unpred_fregs(fregs_mask)
   if ET_TEST_ENV.SILICON_MODE == 1:
      if ((fregs_mask & 0x1) != 0):
         fadd.s f0, f12, f12
      endif
      if ((fregs_mask & 0x2) != 0):
         fadd.s f1, f13, f13
      endif
      if ((fregs_mask & 0x4) != 0):
         fadd.s f2, f14, f14
      endif
      if ((fregs_mask & 0x8) != 0):
         fadd.s f3, f15, f15
      endif
      if ((fregs_mask & 0x10) != 0):
         fadd.s f4, f16, f16
      endif
      if ((fregs_mask & 0x20) != 0):
         fadd.s f5, f17, f17
      endif
      if ((fregs_mask & 0x40) != 0):
         fadd.s f6, f18, f18
      endif
      if ((fregs_mask & 0x80) != 0):
         fadd.s f7, f19, f19
      endif
      if ((fregs_mask & 0x100) != 0):
         fadd.s f8, f20, f20
      endif
      if ((fregs_mask & 0x200) != 0):
         fadd.s f9, f21, f21
      endif
      if ((fregs_mask & 0x400) != 0):
         fadd.s f10, f22, f22
      endif
      if ((fregs_mask & 0x800) != 0):
         fadd.s f11, f23, f23
      endif
      if ((fregs_mask & 0x1000) != 0):
         fadd.s f28, f24, f24
      endif
      if ((fregs_mask & 0x2000) != 0):
         fadd.s f29, f25, f25
      endif
   endif
pb_end
