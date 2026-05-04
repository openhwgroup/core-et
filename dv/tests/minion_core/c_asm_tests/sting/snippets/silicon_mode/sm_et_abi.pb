
pb_start jump_to_c_funct(c_arg_addr, c_funct)

   ; Pass the argument in the register a0, AKA x10
   la x10, c_arg_addr
   ; Call the external C function
   la x1, c_funct
   jalr x1, x1, 0
pb_end

pb_start selfcheck_common(pb_et_info_addr, pb_c_routine)
if (ET_TEST_ENV.SCP_ENABLE == 1):

   ; Sting ABI != GCC ABI
   ; So we have to backup the registers
   rv_save_abi_xregs()
   jump_to_c_funct(c_arg_addr = pb_et_info_addr, c_funct = pb_c_routine)
   ; C function return value is set on x10
   beq x10, x0, pass_selfcheck
   test_fail(regz = x31)
pass_selfcheck:
   ; Restore back the registers
   rv_restore_abi_xregs()
endif
pb_end

; Tensor Load
pb_start sm_set_tl_info(regx1, regx2, et_info_addr, csr_enc_reg, x31_reg, pre_data_addr, post_data_addr)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   csrrs regx2, tensor_mask, x0  ;  [8] | TEN_MASK  |
   sd regx2, 8(regx1)            ;      |           |
                                 ;      -------------
   sd x31_reg, 16(regx1)         ; [16] | x31       |
                                 ;      -------------
   la regx2, pre_data_addr       ; [24] | PRE_DATA  |
   sd regx2, 24(regx1)           ;      |           |
                                 ;      -------------
   la regx2, post_data_addr      ; [32] | POST_DATA |
   sd regx2, 32(regx1)           ;      |           |
                                 ;      -------------
pb_end

pb_start selfcheck_tl(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = tl_selfchecking)
pb_end

; Tensor FMA
pb_start sm_set_tfma_info(regx1, regx2, et_info_addr, csr_enc_reg, x31_reg, pre_data_addr, pre_vpurf_addr, tl_tenb_csr_reg, post_data_addr, pre_tenc_addr, post_tenc_addr, phy_mid)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   csrrs regx2, tensor_mask, x0  ;  [8] | TEN_MASK  |
   sd regx2, 8(regx1)            ;      |           |
                                 ;      -------------
   sd x31_reg, 16(regx1)         ; [16] | x31       |
                                 ;      -------------
   la regx2, pre_data_addr       ; [24] | PRE_DATA  |
   sd regx2, 24(regx1)           ;      |           |
                                 ;      -------------
   la regx2, post_data_addr      ; [32] | POST_DATA |
   sd regx2, 32(regx1)           ;      |           |
                                 ;      -------------
   la regx2, pre_vpurf_addr      ; [40] | PRE_VPURF |
   sd regx2, 40(regx1)           ;      |           |
                                 ;      -------------
   sd tl_tenb_csr_reg, 48(regx1) ; [48] | TL_TENB   |
                                 ;      -------------
   li regx2, phy_mid             ; [56] | MINION_ID |
   sd regx2, 56(regx1)           ;      |           |
                                 ;      -------------
   la regx2, pre_tenc_addr       ; [64] | PRE_TENC  |
   sd regx2, 64(regx1)           ;      |           |
                                 ;      -------------
   la regx2, post_tenc_addr      ; [72] | POST_TENC |
   sd regx2, 72(regx1)           ;      |           |
                                 ;      -------------
pb_end

pb_start selfcheck_tfma(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = tfma_selfchecking)
pb_end

; Tensor Load L2
pb_start sm_set_tl_l2_info(regx1, regx2, et_info_addr, csr_enc_reg, x31_reg)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   csrrs regx2, tensor_mask, x0  ;  [8] | TEN_MASK  |
   sd regx2, 8(regx1)            ;      |           |
                                 ;      -------------
   sd x31_reg, 16(regx1)         ; [16] | x31       |
                                 ;      -------------
pb_end

pb_start selfcheck_tl_l2(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = tl_l2_selfchecking)
pb_end

; Tensor Reduce
pb_start sm_set_tr_info(regx1, regx2, et_info_addr, csr_enc_reg, partnr_tr_info_addr, pre_vpurf_addr, post_vpurf_addr, phy_mid)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   la regx2, partnr_tr_info_addr ; [24] | PARTNER   |
   sd regx2, 24(regx1)           ;      | CSR_ENC   |
                                 ;      -------------
   la regx2, post_vpurf_addr     ; [32] | POST_DATA |
   sd regx2, 32(regx1)           ;      |           |
                                 ;      -------------
   la regx2, pre_vpurf_addr      ; [40] | PRE_VPURF |
   sd regx2, 40(regx1)           ;      |           |
                                 ;      -------------
   li regx2, phy_mid             ; [56] | MINION_ID |
   sd regx2, 56(regx1)           ;      |           |
                                 ;      -------------
   li regx2, 0xc00000000000000
   or regx2, regx1, regx1
   csrrw x0, evict_va, regx2
   csrwi tensor_wait, 6
pb_end

pb_start selfcheck_tr(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = tr_selfchecking)
pb_end

; Tensor Quant
pb_start sm_set_tquant_info(regx1, regx2, et_info_addr, csr_enc_reg, pre_data_addr, pre_vpurf_addr, post_data_addr, phy_mid)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   la regx2, pre_data_addr       ; [24] | PRE_DATA  |
   sd regx2, 24(regx1)           ;      |           |
                                 ;      -------------
   la regx2, post_data_addr      ; [32] | POST_DATA |
   sd regx2, 32(regx1)           ;      |           |
                                 ;      -------------
   la regx2, pre_vpurf_addr      ; [40] | PRE_VPURF |
   sd regx2, 40(regx1)           ;      |           |
                                 ;      -------------
   li regx2, phy_mid             ; [56] | MINION_ID |
   sd regx2, 56(regx1)           ;      |           |
                                 ;      -------------
pb_end

pb_start selfcheck_tquant(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = tquant_selfchecking)
pb_end

pb_start sm_set_ts_info(regx1, regx2, et_info_addr, csr_enc_reg, x31_reg, pre_vpurf_addr, post_data_addr, phy_mid)
                                 ;      -------------
   la regx1, et_info_addr        ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)      ;      |           |
                                 ;      -------------
   sd x31_reg, 16(regx1)         ; [16] | x31       |
                                 ;      -------------
   la regx2, post_data_addr      ; [32] | POST_DATA |
   sd regx2, 32(regx1)           ;      |           |
                                 ;      -------------
   la regx2, pre_vpurf_addr      ; [40] | PRE_VPURF |
   sd regx2, 40(regx1)           ;      |           |
                                 ;      -------------
   li regx2, phy_mid             ; [56] | MINION_ID |
   sd regx2, 56(regx1)           ;      |           |
                                 ;      -------------
pb_end

pb_start sm_set_cts_info(regx1, regx2, et_info_addr, csr_enc_reg, x31_reg, pre_vpurf_addr, post_data_addr, post_data_off, phy_mid)
                                       ;      -------------
   la regx1, et_info_addr              ;  [0] | CSR_ENC   |
   sd csr_enc_reg, 0(regx1)            ;      |           |
                                       ;      -------------
   sd x31_reg, 16(regx1)               ; [16] | x31       |
                                       ;      -------------
   la regx2, post_data_addr            ; [32] | POST_DATA |
   if (post_data_off != 0):            ;      |           |
      addi regx2, regx2, post_data_off ;      |           |
   endif                               ;      |           |
   sd regx2, 32(regx1)                 ;      |           |
                                       ;      -------------
   la regx2, pre_vpurf_addr            ; [40] | PRE_VPURF |
   sd regx2, 40(regx1)                 ;      |           |
                                       ;      -------------
   li regx2, phy_mid                   ; [56] | MINION_ID |
   sd regx2, 56(regx1)                 ;      |           |
                                       ;      -------------
pb_end

pb_start selfcheck_tstore(et_info_addr)
   selfcheck_common(pb_et_info_addr = et_info_addr, pb_c_routine = ts_selfchecking)
pb_end

; Checkpoint signature generation
pb_start sm_set_checkpoint_head_info(regx1, et_info_addr)
   la regx1, et_info_addr
                                 ;      -------------
   sd x2, 0(regx1)               ;  [0] |   HEAD    |
                                 ;      -------------
pb_end

pb_start sm_set_checkpoint_tail_info(regx1, et_info_addr)
   la regx1, et_info_addr
                                 ;      -------------
   sd x2, 8(regx1)               ;  [8] |   TAIL    |
                                 ;      -------------
pb_end
