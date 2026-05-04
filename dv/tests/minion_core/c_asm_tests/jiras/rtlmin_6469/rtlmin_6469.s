.equ X31_ALU, 0
.equ X31_DCACHE_MISS, 1
.equ X31_DCACHE_HIT, 2
.equ X31_DIV_MUL, 3
.equ X31_FP, 4
.equ X31_DCACHE_MISS_WA, 5
.equ X31_DIV_MUL_WA, 6

################################################################################        
# helper macros
################################################################################
# sets x31
.macro set_x31 x31_src, x31_val, x31_ptr
    # set x31
    .if \x31_src == X31_DCACHE_HIT ||  \x31_src == X31_DCACHE_MISS
        ld x31, 0(\x31_ptr)
    .elseif \x31_src == X31_ALU
        addi x31, \x31_val, 0
    .elseif \x31_src == X31_DIV_MUL
        mul x31, \x31_val, t0
    .elseif \x31_src == X31_FP
        fmv.x.s x31, ft0
    .elseif \x31_src == X31_DCACHE_MISS_WA
        ld x31, 0(\x31_ptr)
        addi x31, x31, 0 # work around
    .elseif \x31_src == X31_DIV_MUL_WA
        mul x31, \x31_val, t0
        nop # work around
    .else
        .err # bad x31 src
    .endif
        
.endm

#  prepares env to properly set x31 depending on the source (e.g. x31 hit/miss, VPU reg...)
.macro prepare_x31  x31_src, x31_val, x31_ptr, x31_bad
    .if \x31_src == X31_DCACHE_MISS
        li t0, 1ULL << 58            # evict to L2
        or t0, t0, \x31_ptr       #set VA
        andi t0, t0, ~0x3F  # just the one line
        csrw evict_va, t0
        csrwi tensor_wait, 6
    .endif

    addi x31, \x31_bad, 0 # init x31 with a value that would make the test fail
    li t0, 1        
    fmv.s.x ft0, \x31_val
        
.endm

# repeats nops N times
.macro nops count
    .rept \count
        nop
    .endr

.endm

        
################################################################################
# amocmpswap test
################################################################################
.macro amocmpswap dst, src, ptr, l_g, w_d, nr_nops, x31_src, x31_val, x31_ptr, x31_bad
    prepare_x31  \x31_src, \x31_val, \x31_ptr, \x31_bad
    .p2align 6 # align to CL
    set_x31 \x31_src, \x31_val, \x31_ptr
    nops \nr_nops

    # the amocmpswap
    .if (\l_g && \w_d )
        amocmpswapg.d \dst, \src, (\ptr)
    .endif              
    .if (\l_g && !\w_d )
        amocmpswapg.w \dst, \src, (\ptr)
    .endif              
    .if (!\l_g && \w_d )
        amocmpswapl.d \dst, \src, (\ptr)
    .endif              
    .if (!\l_g && !\w_d )
        amocmpswapl.w \dst, \src, (\ptr)
    .endif

    # set x31 to a bad value immediately after the instruction under test
    addi x31, \x31_bad, 0 # init x31 with a value that would make the test fail
        
.endm    


################################################################################
# cacheops test
################################################################################
.equ EVICT, 0
.equ FLUSH, 1
.equ PREFETCH, 2
.equ LOCK, 3

.macro cacheops op, csrconf, nr_nops, x31_src, x31_val, x31_ptr, x31_bad
    prepare_x31  \x31_src, \x31_val, \x31_ptr, \x31_bad
    .p2align 6 # align to CL
    set_x31 \x31_src, \x31_val, \x31_ptr
    nops \nr_nops

    #the cacheop
    .if \op == EVICT
        csrw evict_va, \csrconf
    .elseif \op == FLUSH
        csrw flush_va, \csrconf
    .elseif \op == PREFETCH
        csrw prefetch_va, \csrconf
    .elseif \op == LOCK
        csrw lock_va, \csrconf
     .else
        .err # bad  cacheop
    .endif
        
    # set x31 to a bad value immediately after the instruction under test
    addi x31, \x31_bad, 0 # init x31 with a value that would make the test fail

    # tensor wait
    .if \op == PREFETCH
        csrwi tensor_wait, 6
        fence
    .else
        csrwi tensor_wait, 6
    .endif
 .endm


################################################################################
# tensors test
################################################################################
.equ TL, 0
.equ TL_TENB, 1
.equ TL_INT8, 2
.equ TL_INT16, 3
.equ TL_TRANS8, 4
.equ TL_TRANS16, 5
.equ TL_TRANS32, 6
.equ TL_L2SCP, 7
.equ TS, 8
.equ TS_FROM_SCP, 9

.macro tensorops op, csrconf, csrconf_fma, nr_nops, x31_src, x31_val, x31_ptr, x31_bad
    prepare_x31  \x31_src, \x31_val, \x31_ptr, \x31_bad
    .p2align 6 # align to CL
    set_x31 \x31_src, \x31_val, \x31_ptr
    nops \nr_nops

    #the tensorop
    .if \op == TL
        csrw tensor_load, \csrconf
    .elseif \op == TL_TENB
        csrw tensor_load, \csrconf
        csrw tensor_fma,  \csrconf_fma
    .elseif \op == TL_INT8
        csrw tensor_load, \csrconf
    .elseif \op == TL_INT16
        csrw tensor_load, \csrconf
    .elseif \op == TL_TRANS8
        csrw tensor_load, \csrconf
    .elseif \op == TL_TRANS16
        csrw tensor_load, \csrconf
    .elseif \op == TL_TRANS32
        csrw tensor_load, \csrconf
    .elseif \op == TL_L2SCP
        csrw tensor_load_l2, \csrconf
    .elseif \op == TS
        csrw tensor_store, \csrconf
    .elseif \op == TS_FROM_SCP
        csrw tensor_store, \csrconf
    .else
        .err # bad tensor op
    .endif

    # set x31 to a bad value immediately after the instruction under test
    addi x31, \x31_bad, 0 # init x31 with a value that would make the test fail

    # tensor wait
    .if \op == TL
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_TENB
        csrwi tensor_wait, 7
    .elseif \op == TL_INT8
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_INT16
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_TRANS8
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_TRANS16
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_TRANS32
        csrwi tensor_wait, 0
        csrwi tensor_wait, 1
    .elseif \op == TL_L2SCP
        csrwi tensor_wait, 2
        csrwi tensor_wait, 3
    .elseif \op == TS
        csrwi tensor_wait, 8
    .elseif \op == TS_FROM_SCP
        csrwi tensor_wait, 8
    .endif
.endm
