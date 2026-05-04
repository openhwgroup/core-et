import "$MINION_DIAGS/sting/snippets/test_loop/test_loop.pb"

resource


axreg_t rx1, rx2
unum_t  watermark = snp_randnum_glbl()
unum_t  test_loop_size = ET_TEST_ENV.TEST_LOOP_SIZE
unum_t  cpuid = snp_cpu_id()

gmemory_t start_marker[BOARD_CONF.NUM_CPUS][64]@64:DFLT_MEM_SET
gmemory_t loop_count[BOARD_CONF.NUM_CPUS][64]@64:DFLT_MEM_SET
gmemory_t reg_bak[BOARD_CONF.NUM_CPUS][256]@64:DFLT_MEM_SET


snip_setup
   ; Empty
snip_init
@cpu: *

   la   rx1, test_loop_start
   la   rx2, start_marker[cpuid]
   sd   rx1, 0(rx2)
   
   li   rx1, test_loop_size
   la   rx2, loop_count[cpuid]
   sd   rx1, 0(rx2)

if (ET_TEST_ENV.TEST_LOOP_SIZE > 1):
   backup_registers(bak_mem = reg_bak[cpuid], rega = rx1)
   j test_loop_watermark

endif
test_loop_start:
if (ET_TEST_ENV.TEST_LOOP_SIZE > 1):
   restore_registers(bak_mem = reg_bak[cpuid], rega = rx1)
endif
test_loop_watermark:
   ; Watermark that ends with 0xF00D
   loop_start(random_num = watermark, rega = rx1, regb = rx2)

snip_run
   ; Empty

snip_check
    ; Empty
