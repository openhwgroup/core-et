import "$MINION_DIAGS/sting/snippets/tensor/tensor_load.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor_store.pb"

pb_start tensor_load_mix(tl_value, tl_mask_value, x31_value, mem_value, mem_off_value, reg1, reg2, reg3, reg4)
if ((cpu_id%2) == 0):
   reset_tensor_error()
endif
   ; Save x31
   addi reg4, x31, 0

if ((tl_value & 0x8000000000000000) != 0):
   ; [Setup] For Tensor Loads, if bit 63 is set, we are using Tensor Mask (PRM-10)
   set_tensor_mask(value = tl_mask_value, rega = reg1)
endif
   tensor_load_snip_run(num_x31_val = x31_value, csr_val = tl_value, mem = mem_value, addr_off = mem_off_value, trega = reg2, tregb = reg3, tregc = reg4)
pb_end
