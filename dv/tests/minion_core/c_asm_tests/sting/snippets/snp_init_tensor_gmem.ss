import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource

; Evict SW
unum_t  evict_csr_val  = snp_randnum()

num_t loop_index = 0
num_t top_loop_index = 0
unum_t rand_val = 0
unum_t cpuid = snp_cpu_id()
axreg_t rx1, rx2, rx3

gmemory_t  mem_snp[BOARD_CONF.NUM_CPUS][16384]@64:ET_SEQ_MEM_SET

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder

snip_init
@cpu: *
   la rx1, mem_snp[cpuid]

init_et_sting_tensor_shared_mem:
   for top_loop_index in (0, 16, 1):
      for loop_index in (0, 1024, 8):
         rand_val = snp_randnum()
         li rx2, rand_val
         sd rx2, loop_index(rx1)
      endfor
   addi rx1, rx1, 1024
   endfor
end_init_et_sting_tensor_shared_mem:

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
