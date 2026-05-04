
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"

resource

num_t   index = 0
unum_t cpuid = snp_cpu_id()
axreg_t rx_ba, rx1
unum_t SIZE_OF_VPU_RX_EIGHT_LANES = 32 ;(32 bits per lane) * 8 = 32 bytes

unum_t l_mid = snp_rshift(cpuid, 1)
gmemory_t  global_ro_mem[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION
gmemory_t  mem_fp[BOARD_CONF.NUM_CPUS][128]@64:ET_DRAM_REGION

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init
   ; Empty

snip_run
@cpu: *

   backup_fp_regs(memory = mem_fp[cpuid], rega = rx1)

   la rx_ba, global_ro_mem[l_mid]
start_fill_vpu_rf:
for index in (0, 32, 1):
   load_mem_to_vpu_lanes(reg_mem = rx_ba, imm_fp = index)
   addi rx_ba, rx_ba, SIZE_OF_VPU_RX_EIGHT_LANES
endfor
end_fill_vpu_rf:

   restore_fp_regs(memory = mem_fp[cpuid], rega = rx1)

snip_check
   ; Empty
