
import "$MINION_DIAGS/sting/snippets/common.pb"


; This is a resource block to will use in TEST_END option sting.
; The purpose of this snippet is consuming the tensor loaded in B by TFMA .
; It avoids test failing by Co-sim errors by instruction pendings
resource

axreg_t reg0, reg1, reg2, reg3

; Max number of lines we can load is 16 (16 lines * 64 bytes per line * 2(MAX_STRIDE) = 2048)
memory_t  mem1[2048]@64:ET_MINION_SHIRE_MEM_SET

snip_setup
;Empty

snip_init
;Empty

snip_run
@cpu: 0,2 .. BOARD_CONF.NUM_CPUS
 ;Empty

  li reg2, 0xFFFFFFFFFF80
  la reg1, mem1
  and reg2, reg2, reg1
  li reg1, 0x10000000000000
  or reg1, reg2, reg1

  ; Tensor Wait for any previous tensor load
  csrwi tensor_wait, 0
  csrwi tensor_wait, 1

  ;save x31
  addi reg3, x31, 0

  li   x31, 0 
  ; load tensor b
  csrrw reg2, tensor_load, reg1
  ;  write TFMA
  li reg3, 0x180000000700006
  csrrw reg1, tensor_fma, reg3
  ; wait for tfma end
  li reg0, 0x7
  csrrw reg0, tensor_wait, reg0

  ;restore x31
  addi x31, reg3, 0

snip_check
;Empty
