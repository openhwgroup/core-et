
import "$MINION_DIAGS/sting/snippets/tensor/tensor.pb"
import "$MINION_DIAGS/sting/snippets/sequences/sequences.pb"

resource

num_t   index = 0
unum_t  csr_value   = 0xf  ; xs[3:0] = NumLines (minus 1)
unum_t  x31_value   = 0x40 ; x31[47:6] = Stride (64 bytes) | x31[0] = ID
unum_t  ten_wait_en = 1
unum_t  addr_offset = 0
unum_t  cpuid = snp_cpu_id()

axreg_t csr_reg, rx1, rx2, rx3, mem_reg

unum_t l_mid = snp_rshift(cpuid, 1)
gmemory_t  global_ro_mem[ET_TEST_ENV.NUM_MINIONS][1024]@64:ET_DRAM_REGION

snip_setup
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init
   ; Empty

snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
; ET_TEST_ENV.SCP_ENABLE is defined in et.conf
if (ET_TEST_ENV.SCP_ENABLE == 1):
   pre_writting_l1scp()
   backup_x31()

   ; Load Global Read Only memory
   la mem_reg, global_ro_mem[l_mid]

   ; [CSR] Set the register to random values and keep the ones we need
   li  csr_reg, csr_value
   li  rx2, 0x0000ffffffffffc0
   and mem_reg, mem_reg, rx2
   or  csr_reg, csr_reg, mem_reg

   ; Increment TL SCP destination reg
   li rx1, 0x00200000000000000

   ; [x31] Set Tensor Op ID (bit [0]) and stride (bit [47:6])
   li  x31, x31_value

start_fill_l1scp:
; We have to do it 3 times in order to fill all the L1 SCP
for index in (0, 48, 16):
   ; Write to the CSR to trigger the Tensor Load Operation
   csrrw x0, tensor_load, csr_reg
   add csr_reg, csr_reg, rx1
endfor
end_fill_l1scp:
   ; Tensor Wait
   tensor_load_wait(x31_reg_value = x31_value)

   restore_x31()
   post_writting_l1scp()
endif

snip_check
   ; Empty
