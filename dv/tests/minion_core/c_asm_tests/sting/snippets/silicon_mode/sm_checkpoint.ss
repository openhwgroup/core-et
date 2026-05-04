
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/silicon_mode.pb"
import "$MINION_DIAGS/sting/snippets/silicon_mode/sm_et_abi.pb"
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

; This is a resource block where different hardware resources used
; by each snippet can be reserved. In this snippet, we are reserving
; a general purpose register. More complex resource notations will be
; presented in the next set of snippets
resource ;## STING PHASE

unum_t cpuid = snp_cpu_id()
num_t index = 0
num_t index1 = 0
unum_t rand_num = snp_randnum()

axreg_t cp_rx

unum_t rn1 = snp_randnum()
unum_t rn2 = snp_randnum()
unum_t rn3 = snp_randnum()
unum_t rn4 = snp_randnum()
unum_t rn5 = snp_randnum()
unum_t rn6 = snp_randnum()
unum_t rn7 = snp_randnum()
axreg_t rx1
axreg_t rx2
axreg_t rx3
axreg_t rx4
axreg_t rx5

gmemory_t et_info_mem[BOARD_CONF.NUM_CPUS][64]@64:ET_DRAM_REGION
memory_t  crc16_signature[BOARD_CONF.NUM_CPUS][64]@64:ET_MINION_SHIRE_MEM_SET ; CRC16 -> 16 bits, but align it to cache line (64 Bytes)

snip_setup ;## STING PHASE
    ; Empty

snip_init ;## STING PHASE
@cpu: *
   la rx1, et_info_mem[cpuid]
   sd x0,  0(rx1)
   sd x0,  8(rx1)
   sd x0, 16(rx1)
   sd x0, 24(rx1)
   sd x0, 32(rx1)
   sd x0, 40(rx1)
   sd x0, 48(rx1)
   sd x0, 56(rx1)


snip_run ;## STING PHASE
@cpu: *

   ; Wait for any long operation to be done (memory accesses, tensors, cacheops...)
   fence iorw, iorw
   csrwi tensor_wait, 0
   csrwi tensor_wait, 1
   csrwi tensor_wait, 2
   csrwi tensor_wait, 3
   csrwi tensor_wait, 4
   csrwi tensor_wait, 5
   csrwi tensor_wait, 6
   csrwi tensor_wait, 7
   csrwi tensor_wait, 8
   csrwi tensor_wait, 9
   csrwi tensor_wait, 10

   generate_signature(pb_rx1 = rx1, pb_rx2 = rx2, pb_et_info_addr = et_info_mem[cpuid], pb_sign_addr = crc16_signature[cpuid])

   li cp_rx, rand_num
   li rx1, rn1
   li rx2, rn2
   li rx3, rn3
   li rx4, rn4
   li rx5, rn5

   clean_amo_rd(pb_rn0 = rn6, pb_rn1 = rn7)
   
snip_check
   ; Empty

