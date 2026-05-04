pb_start tensor_load_snip_run(num_x31_val, csr_val, mem, addr_off, trega, tregb, tregc)
   ; [x31] Set Tensor Op ID (bit [0]) and stride (bit [47:6])
   ; We do not mask because the other fields are ignored
   li  x31, num_x31_val
  
   ; [CSR] Set the register to random values and keep the ones we need 
   li  csr_reg, csr_val

   ; [CSR] Mask the VA and merge it with the CSR value
   la  trega, mem
if (addr_off > 0):
   li  tregb, addr_off
   add trega, trega, tregb
endif
   li  tregb, 0x0000ffffffffffc0
   and trega, trega, tregb
   or  csr_reg, csr_reg, trega

   ; Write to the CSR to trigger the Tensor Load Operation 
   csrrw trega, tensor_load, csr_reg
if ((cpu_id%2) == 0):
   beq    x0, trega, tl_csr_return_is_0

   test_fail(regz = trega)

tl_csr_return_is_0:
   ; Restore x31
   addi x31, tregc, 0

if (ten_wait_en == 1):
   ; Tensor Wait
   tensor_load_wait(x31_reg_value = num_x31_val)
   ; Tensor Error Self Check
   check_tensor_load_error(num_tload_value = csr_val, rega = trega, regb = tregb, reg_csr_val = tregc)
endif
else:
   ; Thread 1 must trap if writes to tensor_store
   ; if trapped, mret will skip test_fail()
   test_fail(regz = trega)
endif
pb_end
