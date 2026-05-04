
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource 

; Evict SW
unum_t  cpuid = snp_cpu_id()
unum_t  evict_csr_val  = snp_randnum()
unum_t  l1_scp_csr_val = snp_randnum()

unum_t  hid_mask = snp_gen_harts_mask(BOARD_CONF.NUM_CPUS)
unum_t  odd_mask = snp_get_odd_bits(hid_mask)
unum_t  even_mask = snp_get_even_bits(hid_mask)

unum_t  neigh_chicken_esr = 0x80DF0070

axreg_t csr_reg 
axreg_t rx1 
axreg_t rx2 
axreg_t rx3

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 
   ; Empty

snip_run 
;@cpu: 0,16 .. BOARD_CONF.NUM_CPUS
@cpu: *
if (cpuid == 0):
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   write_ESR_value(rega = rx1, regb = rx2, regc = rx3, shireid = 0, esr_addr = neigh_chicken_esr, esr_value = 0)
   write_ESR_value(rega = rx1, regb = rx2, regc = rx3, shireid = 1, esr_addr = neigh_chicken_esr, esr_value = 0)

   ;writeFCC(credinc = 0, regtemp = rx1, regtemp2 = rx2, valhartmask = even_mask)

   ;writeFCC(credinc = 2, regtemp = rx1, regtemp2 = rx2, valhartmask = odd_mask)

   ;if (cpuid%2) == 0:
   ;  wait_fcc(cnt = 0)
   ;else:
   ;  wait_fcc(cnt = 2)
   ;endif

else: ; Call mhandler
   et_jump_to_m_mode()
endif
;else:

   ;if (cpuid%2) == 0:
   ;  wait_fcc(cnt = 0)
   ;else:
   ;  wait_fcc(cnt = 2)
   ;endif

endif

snip_check 
   ; Empty

snip_handler
mhandler@cpu: *

   write_ESR_value(rega = rx1, regb = rx2, regc = rx3, shireid = 0, esr_addr = neigh_chicken_esr, esr_value = 0)
   write_ESR_value(rega = rx1, regb = rx2, regc = rx3, shireid = 1, esr_addr = neigh_chicken_esr, esr_value = 0)

   ;writeFCC(credinc = 0, regtemp = rx1, regtemp2 = rx2, valhartmask = even_mask)

   ;writeFCC(credinc = 2, regtemp = rx1, regtemp2 = rx2, valhartmask = odd_mask)

   ;if (cpuid%2) == 0:
   ;  wait_fcc(cnt = 0)
   ;else:
   ;  wait_fcc(cnt = 2)
   ;endif

exit_handler:
    ; Skip instruction that trapped
    csrr rx1, mepc
    addi rx1, rx1, 4
    csrw mepc, rx1
