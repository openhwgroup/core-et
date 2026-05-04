
import "$MINION_DIAGS/sting/snippets/common.pb"

;
; Param Block: Reset Tensor Error
;

pb_start reset_tensor_error()
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   csrw tensor_error, x0
endif
pb_end

pb_start write_tensor_mask(value, rega)
   li   rega, value
   csrw tensor_mask, rega
pb_end

pb_start write_tensor_conv_size_ctrl(size, ctrl)
   csrw tensor_conv_size, size
   csrw tensor_conv_ctrl, ctrl
pb_end

pb_start write_tensor_conv(rxa)
    SELECT 1 {
        1 : csrw tensor_conv_ctrl, rxa
        1 : csrw tensor_conv_size, rxa
    }
pb_end

;
; Param Block: Set Tensor Mask
;
; value : Value to write into the tensor mask

pb_start set_tensor_mask(value, rega)
   li   rega, value
   SELECT 1 {
       12 : csrw tensor_mask, rega
        1 : write_tensor_conv(rxa = rega)
   }
pb_end

pb_start tensor_store_wait()
    csrwi tensor_wait, 8
pb_end

pb_start tensor_load_l2_wait(x31_reg_value)
if ((x31_reg_value%2) == 0):
   ; Bit[0] == 0
   csrwi tensor_wait, 2
else :
   ; Bit[0] == 1
   csrwi tensor_wait, 3
endif
pb_end

pb_start tensor_quant_wait()
    csrwi tensor_wait, 10
pb_end

;
; Param Block: Tensor Wait
;
; x31_reg_value : Register x31 for reading the Tensor ID (bit[0])

pb_start tensor_load_wait(x31_reg_value)
if ((x31_reg_value%2) == 0):
   ; Bit[0] == 0
   csrwi tensor_wait, 0
else :
   ; Bit[0] == 1
   csrwi tensor_wait, 1
endif
pb_end

pb_start cmp_and_jmp(reg_to_cmp, num_expected, num_mask, reg_tmp1, reg_tmp2, tag_fail)
   ; Load the mask
   li reg_tmp1, num_mask
   ; Mask the register to consider only certain bits
   and reg_tmp1, reg_to_cmp, reg_tmp1
   li reg_tmp2, num_expected
   bne reg_tmp1, reg_tmp2, tag_fail
pb_end

;
; Param Block: Check Tensor Error Bit9
; (Illegal TensorReduceSend/TensorReduceRecv partner specified)
;
; regc1 : Register containing the Tensor Error CSR value

pb_start check_tensor_error_bit9(rega1, regb1, regc1, is_error, fail)
if (is_error == 1):
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x200, num_mask = 0x200, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
else :
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x000, num_mask = 0x200, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
endif
pb_end

;
; Param Block: Check Tensor Error Bit8
; (Invalid combination of TensorStore cooperation and size)
;
; isCoop : Tmp register
; regb1 : Register containg the Tensor Error CSR value

pb_start check_tensor_error_bit8(num_csr_value, rega1, regb1, regc1, fail)
if   ((0x0186000000000000 & num_csr_value) == 0x0100000000000000):
   ; [50:49] = 0 / [56:55] = 2
   ; Must report Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x100, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0006000000000000 & num_csr_value) == 0x0000000000000000):
   ; [50:49] = 0 / [56:55] != 2
   ; Must report no Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x000, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0186000000000000 & num_csr_value) == 0x0102000000000000):
   ; [50:49] = 1 / [56:55] = 2
   ; Must report Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x100, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0186000000000000 & num_csr_value) == 0x0182000000000000):
   ; [50:49] = 1 / [56:55] = 3
   ; Must report Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x100, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0006000000000000 & num_csr_value) == 0x0002000000000000):
   ; [50:49] = 1 / [56:55] != 2 && [56:55] != 3
   ; Must report no Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x000, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0186000000000000 & num_csr_value) == 0x0006000000000000):
   ; [50:49] = 3 / [56:55] = 0
   ; Must report no Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x000, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
elif ((0x0006000000000000 & num_csr_value) == 0x0006000000000000):
   ; [50:49] = 3 / [56:55] != 0
   ; Must report Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x100, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
else:
   ; [50:49] = 2
   ; Must report Tensor error
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x100, num_mask = 0x100, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
endif
pb_end

;
; Param Block: Check Tensor Error Bit7
; ( TensorLoad, TensorStore, or CacheOp instruction generated a memory-related fault)
; 
; rega1 : Tmp register
; regb1 : Register containg the Tensor Error CSR value

pb_start check_tensor_error_bit7(rega1, regb1, fail)
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   ; Load the mask to consider the bits we are interested in only
   li   rega1, 0x80
   and  rega1, rega1, regb1
   bne  rega1, x0, fail
endif
pb_end

; Param Block: Check Tensor Error Bit4
; (TensorLoad configured with a non-compatible byte offset and transform field)
; 
; num_l1_scp_en : 1 if SCP Enable, 0 otherwise
; rega1 : Tmp register
; regb1 : Tmp register
; regc1 : Register containg the Tensor Error CSR value

pb_start check_tensor_error_bit4(rega1, regb1, regc1, fail)
   ; Load the mask to consider the bits we are interested in only
   li   regb1, 0x10
   and  rega1, regb1, regc1
if (ET_TEST_ENV.SCP_ENABLE == 1):
   ; SCP enabled
   bne  rega1 ,   x0, fail
else:
   ; SCP not enabled
   bne  rega1, regb1, fail
endif
pb_end

; Param Block: Check Tensor Error Bit3
; FCC Overflow
;
pb_start check_tensor_error_bit3(rega1, regb1, regc1, fail, is_error)
   ; Load the mask to consider the bits we are interested in only
   li   regb1, 0x08
   and  rega1, regb1, regc1
if (is_error == 1):
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x08, num_mask = 0x08, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
else :
   cmp_and_jmp(reg_to_cmp = regc1, num_expected = 0x00, num_mask = 0x08, reg_tmp1 = rega1, reg_tmp2 = regb1, tag_fail = fail)
endif
pb_end

; Param Block: Check Tensor Error Bit1
; (Tensor Load with wrong transform ID)
; 
; num_csr_value : Initial random value written into the Tensor Load CSR
; rega1 : Tmp register
; regb1 : Tmp register
; regc1 : Register containg the Tensor Error CSR value

pb_start check_tensor_error_bit1(num_csr_value, rega1, regb1, regc1, fail)
   ; Only check the transformation if it is not a ten_b
if   ((num_csr_value & 0x0010000000000000) == 0x0000000000000000):
   ; Load the mask to consider the bits we are interested in only
   li   regb1, 0x2
   and  rega1, regb1, regc1
   ; Check for bits[61:59] == 0b100 (tensor_error[1])
if   ((num_csr_value & 0x3800000000000000) == 0x2000000000000000):
   bne  rega1, regb1, fail
elif ((num_csr_value & 0x3800000000000000) == 0x1800000000000000):
   bne  rega1, regb1, fail
else:
   bne  rega1 ,   x0, fail
endif
endif
pb_end

pb_start check_tensor_load_error(num_tload_value, num_scp_en, rega, regb, reg_csr_val)
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   ; Read Tensor Error CSR
   csrrs reg_csr_val, tensor_error, x0

   ; Mem access fault
   ;check_tensor_error_bit7(rega1 = rega, regb1 = reg_csr_val, fail = fail_tensor_load)
   ; SCP Disabled
   check_tensor_error_bit4(rega1 = rega, regb1 = regb, regc1 = reg_csr_val, fail = fail_tensor_load)
if (ET_TEST_ENV.SCP_ENABLE == 1):
   ;Wrong Transformation ID
   check_tensor_error_bit1(num_csr_value = num_tload_value, rega1 = rega, regb1 = regb, regc1 = reg_csr_val, fail = fail_tensor_load)
endif

   j pass_tensor_load_error

fail_tensor_load:
   test_fail(regz = rega)

pass_tensor_load_error:
   reset_tensor_error()
endif
pb_end

pb_start check_tensor_store_error(num_rand_csr, from_scp, rega, regb, regc, pass_label, fail_label)
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   ; Read Tensor Error CSR
   csrrs regc, tensor_error, x0

if (from_scp == 1):
   ; SCP Disabled
   check_tensor_error_bit4(rega1 = rega, regb1 = regb, regc1 = regc, fail = fail_label)
else:
   ; Invalid combination of TensorStore Coop and size
   check_tensor_error_bit8(num_csr_value = num_rand_csr, rega1 = rega, regb1 = regb, regc1 = regc, fail = fail_label)
endif

endif
   j pass_label
pb_end

pb_start check_tensor_reduce_error(rega, regb, regc, same_minion, fail_reduce, pass_reduce)
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   ; Read Tensor Error CSR
   csrrs regc, tensor_error, x0
if (same_minion == 1):
   ; A reduce from the same minion should produce an error
   check_tensor_error_bit9(rega1 = rega, regb1 = regb, regc1 = regc, is_error = 1, fail = fail_reduce)
else :
   check_tensor_error_bit9(rega1 = rega, regb1 = regb, regc1 = regc, is_error = 0, fail = pass_reduce)
endif

endif
   j pass_reduce
pb_end

pb_start tensor_mhandler(rega, regb)
   ; HART1 will always generate an illegal instruction exception
   ; This type of exception is always handled in Machine mode
   csrrs rega, mcause, x0

if ((cpu_id%2) == 1):
   ; HART1 cannot execute Tensor Operations, so it is normal to trap
   li    regb, 0x2 ;Illegal Instruction
   beq   rega, regb, pass_check_tensor_trap
endif

   ; We received an unexpected trap, we go to fail
   test_fail(regz = rega)

pass_check_tensor_trap:
pb_end

pb_start tensor_coop_mhandler(regx1, regx2, is_coop)
   ; HART1 will always generate an illegal instruction exception
   ; This type of exception is always handled in Machine mode
   csrrs rx1, mcause, x0

if (((is_coop == 1) & (ET_TEST_ENV.COOP_MODE_ENABLE == 0)) | ((cpu_id%2) == 1)):
   ; HART1 cannot execute Tensor Operations, so it is normal to trap
   li    rx2, 0x2 ;Illegal Instruction
   beq   rx1, rx2, pass_check_tensor_trap
endif

   ; We received an unexpected trap, we go to fail
   test_fail(regz = rx1)

pass_check_tensor_trap:
pb_end

pb_start tensor_reduce_wait()
   csrwi tensor_wait, 9
pb_end

pb_start tensor_fma_wait()
   csrwi tensor_wait, 7
pb_end

pb_start all_random_tensor_op(top, regxs, retreg)
  csrw top, regxs
  
  csrr regxs,tensor_mask
  
  csrr retreg,top
  
pb_end

pb_start check_fcc_overflow(rega, regb, regc, fail_label)
   ; Read Tensor Error CSR
   csrrs regc, tensor_error, x0
   check_tensor_error_bit3(rega1 = rega, regb1 = regb, regc1 = regc, fail = fail_label, is_error = 1)
pb_end
