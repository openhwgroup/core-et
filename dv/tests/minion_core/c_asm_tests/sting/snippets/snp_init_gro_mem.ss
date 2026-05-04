import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource

; Evict SW
unum_t  evict_csr_val  = snp_randnum()

num_t loop_index = 0
unum_t rand_val = 0
unum_t cpuid = snp_cpu_id()
axreg_t rx1, rx2, rx3

unum_t l_mid = snp_rshift(cpuid, 1)
gmemory_t  global_ro_mem[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder

snip_init
@cpu: 0,2..BOARD_CONF.NUM_CPUS
   la rx1, global_ro_mem[l_mid]

init_et_sting_read_only_mem:
   for loop_index in (0, 1024, 8):
      rand_val = snp_randnum()
      li rx2, rand_val
      sd rx2, loop_index(rx1)
   endfor
end_init_et_sting_read_only_mem:

   fence iorw, iorw
if (TEST_PROPS.TEST_MODE == RV_PARAMS.RV_MODE.MACHINE):
   evict_all_l1(value = evict_csr_val, rega = rx1, regb = rx2, regc = rx3)
else: ; Call mhandler
   et_jump_to_m_mode()
endif

snip_run
   ; Empty


snip_check
   ; Empty


snip_handler
mhandler@cpu: *
   evict_all_l1(value = evict_csr_val, rega = rx1, regb = rx2, regc = rx3)
exit_handler:
   ; Skip instruction that trapped
   csrr rx1, mepc
   addi rx1, rx1, 4
   csrw mepc, rx1
