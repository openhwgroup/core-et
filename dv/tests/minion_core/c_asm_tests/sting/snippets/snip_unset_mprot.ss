; This snippet set Mprot esr address to random value
; It tries to reduce overhead on setting it address inside snippet block run.
; and makes easy to mix with other snippets.

import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"


resource

unum_t x_cpu = snp_urandrange_glbl(0, BOARD_CONF.NUM_CPUS)
unum_t hid_mask = snp_gen_harts_mask(BOARD_CONF.NUM_CPUS)
unum_t odd_mask = snp_get_odd_bits(hid_mask)
unum_t even_mask = snp_get_even_bits(hid_mask)


axreg_t rx1, rx2

snip_setup
   ; Empty
snip_init
   ; Empty

snip_run

@cpu: *
   
   if (cpu_id == x_cpu) :
     li rx1, 0xdeadaaaa
     if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
        addi rx2, x0, 0x004  ; load esr @mprot
        set_random_mprot(rega = rx1, regb = rx2, rand_val = 0x10)
     else:  ; Call mhandler
        et_jump_to_m_mode()
     endif

     writeFCC(credinc = 0, regtemp = rx1, regtemp2 = rx2, valhartmask = even_mask)
     writeFCC(credinc = 2, regtemp = rx1, regtemp2 = rx2, valhartmask = odd_mask)

     if ((cpu_id%2) == 0):
        wait_fcc(cnt = 0)
     else:
        wait_fcc(cnt = 2)
     endif

   else:
     if ((cpu_id%2) == 0):
        wait_fcc(cnt = 0)
     else:
        wait_fcc(cnt = 2)
     endif
   endif

snip_check
   ; Empty

snip_handler

mhandler@cpu: *

   if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
      test_fail(regz = rx1)
   else:
      ;csrrs rx1, mcause, x0  ; I don't mind mcause only expected  8 and 9
      set_random_mprot(rega = rx1, regb = rx2, rand_val = 0x10)
   endif
    
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
  
