pb_start tensor_fma_snip_run(csr_regx, regxa, regxb, rand_csr_val, ten_mask_val, ten_wait, backup_mem, hartid)

if ((cpu_id%2) == 0):
   backup_fp_regs(memory = backup_mem[hartid], rega = regxa)
   reset_tensor_error()
endif
   ; Save x31
   addi regxa, x31, 0

if ((rand_csr_val & 0x8000000000000000) != 0):
   ; [Setup] For TensorFMA, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = ten_mask_val, rega = regxb)
endif

   tensor_fma_base_snip_run(csr_reg_a = csr_regx, regx_a = regxa, regx_b = regxb, csr_val = rand_csr_val, tfma_wait = ten_wait)
if ((cpu_id%2) == 0):
   restore_fp_regs(memory = backup_mem[hartid], rega = regxa)
endif
pb_end

pb_start tensor_fma_base_snip_run(csr_reg_a, regx_a, regx_b, csr_val, tfma_wait)

   ; Tensor Wait for any previous tensor load
   csrwi tensor_wait, 0
   csrwi tensor_wait, 1

   li csr_reg_a, csr_val
   ; Write to the CSR to trigger the Tensor Load Operation 
   csrrw regx_b, tensor_fma, csr_reg_a

if ((cpu_id%2) == 0):
   beq    x0, regx_b, tfma_csr_return_is_0

   test_fail(regz = regx_b)

tfma_csr_return_is_0:
   ; Restore x31
   addi x31, regx_a, 0

if (tfma_wait == 1):
   tensor_fma_wait()
   ; FUTURE: Also Self Check
endif
else:
   ; Thread 1 must trap if writes to tensor_store
   ; if trapped, mret will skip test_fail()
   test_fail(regz = regx_a)
endif
pb_end
