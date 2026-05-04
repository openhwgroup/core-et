
import "$MINION_DIAGS/sting/snippets/common.pb"
import "$STING_ROOT/src/snippets/rv/rv_param_blocks.pb"

resource 

cpu_t x

snip_setup 
    ; Empty
    ; For current Sting version, this is a placeholder


snip_init 
   ; Empty

snip_run 
@cpu: x
   exclusive_mode(set_it = 1, previous = x0)
   RANDINST 12:27
   exclusive_mode(set_it = 0, previous = x0)

snip_check 
   ; Empty
