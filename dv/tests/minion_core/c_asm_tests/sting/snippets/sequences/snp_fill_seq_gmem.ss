
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

resource

num_t   index = 0
unum_t  x31_value   = 0x40 ; x31[47:6] = Stride (64 bytes) | x31[0] = ID
unum_t  ten_wait_en = 1
unum_t  addr_offset = 0
unum_t  cpuid = snp_cpu_id()

unum_t first_scp  = 0x0079000000000000
unum_t second_scp = 0x1079000000000000
unum_t third_scp  = 0x2079000000000000

axreg_t first_ts_scp
axreg_t second_ts_scp
axreg_t third_ts_scp
axreg_t scp_size

axreg_t mem_reg

unum_t l_mid = snp_rshift(cpuid, 1)
gmemory_t op_mem[ET_TEST_ENV.NUM_MINIONS][41216]@64:ET_SEQ_MEM_SET

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init
   ; Empty

snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
; ET_TEST_ENV.SCP_ENABLE is defined in et.conf
if (ET_TEST_ENV.SCP_ENABLE == 1):
   pre_reading_l1scp()
   backup_x31()

   li scp_size, 3072

   ; Load Global Read Only memory
   la mem_reg, op_mem[l_mid]

   li first_ts_scp, first_scp
   add first_ts_scp, first_ts_scp, mem_reg
   addi mem_reg, mem_reg, 1024

   li second_ts_scp, second_scp
   add second_ts_scp, second_ts_scp, mem_reg
   addi mem_reg, mem_reg, 1024

   li third_ts_scp, third_scp
   add third_ts_scp, third_ts_scp, mem_reg
   addi mem_reg, mem_reg, 1024

   li  x31, x31_value

   for index in (0, 39936, 3072):
      csrrw x0, tensor_store, first_ts_scp
      add first_ts_scp, first_ts_scp, scp_size
      csrrw x0, tensor_store, second_ts_scp
      add second_ts_scp, second_ts_scp, scp_size
      csrrw x0, tensor_store, third_ts_scp
      add third_ts_scp, third_ts_scp, scp_size
   endfor

   tensor_store_wait()
   restore_x31()
endif

snip_check
   ; Empty
