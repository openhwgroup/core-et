import "$MINION_DIAGS/sting/snippets/test_loop/test_loop.pb"

resource


axreg_t rx1, rx2
unum_t  watermark = snp_randnum_glbl()
unum_t  cpuid = snp_cpu_id()

gmemory_t start_marker[BOARD_CONF.NUM_CPUS][64]@64:DFLT_MEM_SET
gmemory_t loop_count[BOARD_CONF.NUM_CPUS][64]@64:DFLT_MEM_SET


snip_setup
   ; Empty
snip_init
   ; Empty


snip_run
@cpu: *

   ; Watermark that ends with 0xBABE
   loop_end(random_num = watermark, rega = rx1, regb = rx2)

   la   rx2, loop_count[cpuid]
   ld   rx1, 0(rx2)
   addi rx1, rx1, -1
   beq  rx1, x0, test_loop_done

   sd   rx1, 0(rx2)

   la   rx2, start_marker[cpuid]
   ld   rx1, 0 (rx2)
   jalr rx1

test_loop_done:

snip_check
    ; Empty
