; This snippet runs on a random mask of HARTs in the shire.
; Each participant in one shire issue the FLB and then waits FCC credits. 
; The last participant to reach the barrier gives FCC credits to everybody else.

resource
cpu_t x

unum_t  index = 0
axreg_t ax1, ax2, ax3, ax4, x31_bak
unum_t  credinc0_esr = 0x803400C0
unum_t  credinc1_esr = 0x803400C8
unum_t  credinc2_esr = 0x803400D0
unum_t  credinc3_esr = 0x803400D8
unum_t  flb0_esr     = 0x80340100

unum_t  counter_select  = snp_urandrange_glbl(0,2)
unum_t  barrier_select  = snp_urandrange_glbl(0,31)
unum_t  barrier_offset  = snp_mul(barrier_select,0x8)
unum_t  barrier_lock_offset  = snp_mul(barrier_select,0x4)
unum_t  logcpu_mask     = glbl_snp_get_rand_lsb_mask(BOARD_CONF.NUM_CPUS)
unum_t  num_players     = snp_setc(logcpu_mask)
unum_t  base_cpu        = snp_get_first_bit_set(logcpu_mask)
unum_t  phycpu_mask     = snp_get_phycpu_mask(logcpu_mask)
unum_t  odd_mask        = snp_get_odd_bits(phycpu_mask)
unum_t  even_mask       = snp_get_even_bits(phycpu_mask)
unum_t  cpuid           = snp_cpu_id()
unum_t  shireid_mask    = snp_rshift(0xFF,22)  ; shire mask in ESR
unum_t  flb_upper_limit = snp_sub(255,num_players)
unum_t  flb_init_value  = snp_urandrange_glbl(0,flb_upper_limit) ; Should I cover [255-num_players, 255] range?
unum_t  num_players_min1 = snp_sub(num_players, 1)
unum_t  match_value     = snp_add(flb_init_value,num_players_min1)
unum_t  match_mask      = snp_lshift(match_value,5)  ; 
unum_t  flb_arg         = snp_or(match_mask, barrier_select) 

unum_t  do_something    = snp_bit_set_in_mask(cpuid, logcpu_mask)

gmemory_t barrier_locks[64]@4
gnum_t  test_uniq_id[BOARD_CONF.NUM_CPUS]
test_uniq_id[cpuid] = 100

snip_setup
   ; Empty

snip_init
   ; Empty

snip_run

@cpu: *
  add x31_bak, x31, x0
  if (do_something > 0):

   ; Init flb and locks
   if (cpuid == base_cpu):
     la ax4, barrier_locks
     if (test_uniq_id[cpuid] == 100):
       for index in (0, 128, 4):
         addi ax3, ax4, index
         amoswapg.w x0, ax3, x0
       endfor       
     endif

     ;Busy wait until the barrier is free. Lock the barrier by writing the test uniq id in barrier lock address
     addi ax4, ax4, barrier_lock_offset
     li x31, 0x0
     li ax2, test_uniq_id[cpuid]
   master_wait_lock:
       amocmpswapg.w ax3, ax4, ax2
       bne ax3, x0, master_wait_lock

     ; Init flb 
     li ax3, flb_init_value
     ori ax3, ax3, shireid_mask 
     li ax4, flb0_esr
     sd ax3, barrier_offset(ax4)

     fence iorw, iorw

     ; Send fccs to everyone
     li   ax2, even_mask
     li   ax3, odd_mask

     if counter_select == 0:
       li   ax1, credinc0_esr
     else:
       li   ax1, credinc1_esr
     endif

     ori ax1, ax1, shireid_mask 
     sd   ax2, 0  (ax1)
     sd   ax3, 16 (ax1)
   endif ; cpuid == base_cpu

  ; do_something = snp_bit_set_in_mask(cpuid, cpu_mask)
  ; if (do_something > 0):

   csrwi fcc, counter_select
   la ax3, barrier_locks
   addi ax3, ax3, barrier_lock_offset
   li ax2, test_uniq_id[cpuid]
   wait_barrier:
     amoorg.w ax1, ax3, x0
     bne ax1, ax2, wait_barrier


   ; Issue flb
   li ax3, flb_arg
   csrrw ax4, flb0, ax3
   
   ; If not the last on the barrier, jump to the fcc check
   li ax3, 0x1   
   bne ax4, ax3, wait_fcc_label

   ; Unlock barrier
   la ax3, barrier_locks
   addi ax3, ax3, barrier_lock_offset
   li x31, test_uniq_id[cpuid]
   li ax2, 0x0
   amocmpswapg.w x0, ax3, ax2

   ; Send fccs to everyone
   li   ax2, 0xFFFFFFFF
   li   ax3, 0xFFFFFFFF

   if counter_select == 0:
     li   ax1, credinc0_esr
   else:
     li   ax1, credinc1_esr
   endif

   ori ax1, ax1, shireid_mask 
   sd   ax2, 0  (ax1)
   sd   ax3, 16 (ax1)
      
   endif ; do_something

wait_fcc_label:
   csrwi fcc, counter_select

   test_uniq_id[cpuid] = snp_add(test_uniq_id, 1)

   add x31, x31_bak, x0

snip_check
    ; Empty

