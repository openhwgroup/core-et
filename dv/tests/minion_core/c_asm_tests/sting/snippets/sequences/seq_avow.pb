pb_start rand_csr_write(csr_reg)
   SELECT 1 {
      1 : csrrw x0, csr_reg, x0
      1 : csrrw x0, csr_reg, x16
      1 : csrrw x0, csr_reg, x17
      1 : csrrw x0, csr_reg, x18
      1 : csrrw x0, csr_reg, x19
      1 : csrrw x0, csr_reg, x20
      1 : csrrw x0, csr_reg, x21
      1 : csrrw x0, csr_reg, x22
      1 : csrrw x0, csr_reg, x23
      1 : csrrw x0, csr_reg, x24
   }
pb_end

pb_start rand_twait()
   SELECT 1 {
      1 : csrrwi x0, tensor_wait, 0
      1 : csrrwi x0, tensor_wait, 1
      1 : csrrwi x0, tensor_wait, 2
      1 : csrrwi x0, tensor_wait, 3
      1 : csrrwi x0, tensor_wait, 4
      1 : csrrwi x0, tensor_wait, 5
      1 : csrrwi x0, tensor_wait, 6
      1 : csrrwi x0, tensor_wait, 7
      1 : csrrwi x0, tensor_wait, 8
      1 : csrrwi x0, tensor_wait, 9
      1 : csrrwi x0, tensor_wait, 10
   }
pb_end

pb_start rand_terror()
   SELECT 1 {
      1 : csrrwi x0, tensor_error, 0
   }
pb_end

pb_start rand_tcoop()
   rand_csr_write(csr_reg = tensor_cooperation)
pb_end

pb_start rand_tmask()
   csrrs x0, tensor_mask, x0
   rand_csr_write(csr_reg = tensor_mask)
pb_end

pb_start avow_cov_op()
   SELECT 1 {
      1 : rand_twait()
      1 : rand_terror()
      1 : rand_tcoop()
      1 : rand_tmask()
   }
   SELECT 1 {
      10 : NO_INST()
      1  : rand_twait()
      1  : rand_terror()
      1  : rand_tcoop()
      1  : rand_tmask()
   }
pb_end
