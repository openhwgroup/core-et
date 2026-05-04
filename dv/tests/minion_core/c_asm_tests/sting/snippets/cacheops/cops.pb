; Param Block: Check Tensor Error Lock SW
; rega1 : Temporary register
; should_fail  : Indicates whether the test is expected to fail
; on_error     : Label to jump to if error found

pb_start check_tensor_error_lock_sw(rega1, should_fail, on_error)
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   wait_cacheops()
   csrr rega1, tensor_error
   andi rega1, rega1, 0x20 ; TensorError[5] indicates LockSW errors
   bne rega1, x0, on_error   ; Jump to label if error is set
else:
   if (should_fail == 1):
      j on_error ; Always jump to label if we expect this to fail
   endif ; should_fail
endif ; SELF_CHECK_ENABLE
pb_end


; Param Block: Check Tensor Error Lock SW
; rega1, regb1 : Temporary registers
; should_fail  : Indicates whether the test is expected to fail
; on_error     : Label to jump to if error found

pb_start check_tensor_error_cacheop(rega1, should_fail, on_error)
if (ET_TEST_ENV.SELF_CHECK_ENABLE == 1):
   wait_cacheops()
   csrr rega1, tensor_error
   andi rega1, rega1, 0x80   ; TensorError[7] indicates CacheOP errors
   bne rega1, x0, on_error  ; Jump to label if error is set
else:
   if (should_fail == 1):
      j on_error ; Always jump to label if we expect this to fail
   endif ; should_fail
endif ; SELF_CHECK_ENABLE
pb_end

pb_start unlock_all_sw(regx1, regx2, regx3, regx4, num_sets, num_ways)
   li regx3, 0x40

   li regx1, num_ways
LOOP_WAY:
   beq regx1, x0, UNLOCK_ALL_SW_DONE
   addi regx1, regx1, -1
   slli regx4, regx1, 55

   li regx2, num_sets
LOOP_SET:
   beq regx2, x0, LOOP_WAY
   addi regx2, regx2, -1
   add regx4, regx4, regx3
   csrrw x0, unlock_sw, regx4
   j LOOP_SET

UNLOCK_ALL_SW_DONE:
   wait_cacheops()
pb_end
