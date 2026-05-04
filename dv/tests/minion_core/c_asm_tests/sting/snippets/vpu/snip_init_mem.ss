resource
cpu_t       x
axreg_t     rx1, rx2, rx3, rx4

; The memory block below can be accessed in all snippets using
; its name since its a global memory
; The size and alignment are parameters initialized in the conf file

gmemory_t   snp_mem_tgt[STR_MEM.MEM_SIZE]@STR_MEM.MEM_ALIGN
unum_t      var
unum_t      index

snip_setup
    ; Empty

snip_init
@cpu: x

    la rx2, snp_mem_tgt
    for index in (0, STR_MEM.MEM_ELEM_CNT, 1):
        if STR_MEM.MEM_RND_FLAG == 1:
            var = snp_randnum()             ; snippet api which returns
            li  rx1,  var                   ; random number
        else:
            li  rx1, STR_MEM.MEM_VAL[index]
        endif
        sw    rx1, 0(rx2)
        addi  rx2, rx2, 0x4
    endfor

snip_run
    ; Empty

snip_check
    ; Empty


