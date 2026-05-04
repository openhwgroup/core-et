
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

pb_start tensor_store_snip_run(num_ts_csr_val, num_x31_val, memx, addr_off, csr_regx, regx1, regx2, regx3, is_coop, ten_wait)

if (((is_coop == 1) & (ET_TEST_ENV.COOP_MODE_ENABLE == 1)) | ((is_coop == 0) & (cpu_id%2) == 0)):
   reset_tensor_error()
endif

   ; Save x31
   addi regx3, x31, 0

   ; [x31] Set stride (bit [47:4])
   ; We do not mask because the other fields are ignored
   li  x31, num_x31_val
  
   ; [CSR] Set the register to random values 
   li  csr_regx, num_ts_csr_val

   ; [CSR] Mask the VA and merge it with the CSR value
   la  regx1, memx
if (addr_off != 0):
   li  regx2, addr_off
   add regx1, regx1, regx2
endif
   li  regx2, 0x0000fffffffffff0
   and regx1, regx1, regx2
   or  csr_regx, csr_regx, regx1

   ; Write to the CSR to trigger the Tensor Load Operation 
   csrrw regx1, tensor_store, csr_regx
if (((is_coop == 1) & (ET_TEST_ENV.COOP_MODE_ENABLE == 1) & ((cpu_id%2) == 0)) | ((is_coop == 0) & (cpu_id%2) == 0)):
   beq    x0, regx1, ts_csr_return_is_0

   ; 3 instructions
   test_fail(regz = regx1)

ts_csr_return_is_0:
   ; Restore x31
   addi x31, regx3, 0

if (ten_wait == 1):
   ; Wait until the operation is done
   tensor_store_wait()
   ; Tensor Error Self Check
   check_tensor_store_error(num_rand_csr = num_ts_csr_val, from_scp = 0, rega = regx1, regb = regx2, regc = regx3, pass_label = pass_ts_tensor_error, fail_label = fail_ts_tensor_error)
fail_ts_tensor_error:
   test_fail(regz = regx1)

pass_ts_tensor_error:
   reset_tensor_error()
endif
else :
   ; 3 instructions
   test_fail(regz = regx1)
endif
pb_end



pb_start tensor_store_from_scp_snip_run(num_ts_csr_val, num_x31_val, memx, addr_off, csr_regx, regx1, regx2, regx3, ten_wait)
if ((cpu_id%2) == 0):
   reset_tensor_error()
endif

   ; Save x31
   addi regx3, x31, 0

   ; [x31] Set stride (bit [47:6])
   ; We do not mask because the other fields are ignored
   li  x31, num_x31_val

   ; [CSR] Set the register to random values and keep the ones we need
   li  csr_regx, num_ts_csr_val

   ; Set the bit 48 to 1
   li  regx2, 0x0001000000000000
   or  csr_regx, csr_regx, regx2

   ; [CSR] Mask the VA and merge it with the CSR value
   la  regx1, memx
if (addr_off > 0):
   li  regx2, addr_off
   add regx1, regx1, regx2
endif
   li  regx2, 0x0000ffffffffffc0
   and regx1, regx1, regx2
   or  csr_regx, csr_regx, regx1

   ; Write to the CSR to trigger the Tensor Load Operation
   csrrw regx1, tensor_store, csr_regx
if ((cpu_id%2) == 0):
   beq    x0, regx1, ts_scp_csr_return_is_0

   test_fail(regz = regx1)

ts_scp_csr_return_is_0:
   ; Restore x31
   addi x31, regx3, 0

if (ten_wait == 1):
   ; Tensor Wait
   tensor_store_wait()
   ; Tensor Error Self Check
   check_tensor_store_error(num_rand_csr = num_ts_csr_val, from_scp = 1, rega = regx1, regb = regx2, regc = regx3, pass_label = pass_ts_scp_tensor_error, fail_label = fail_ts_scp_tensor_error)

fail_ts_scp_tensor_error:
   test_fail(regz = regx1)

pass_ts_scp_tensor_error:
   reset_tensor_error()
endif
else:
   ; Thread 1 must trap if writes to tensor_store
   ; if trapped, mret will skip test_fail()
   test_fail(regz = regx1)
endif
pb_end
