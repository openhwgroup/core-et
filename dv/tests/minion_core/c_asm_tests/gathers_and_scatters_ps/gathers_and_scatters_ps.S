#include "macros.h"

#define ITERATIONS 256
#define UNTAG 12
#define UNSET 6
#define WAYS 4
#define TICKETER 0xFFF00000
    
.text
.global _start
_start: 
    ASM_TEST_START

    //enable lanes
    li x2, 0xff
    mova.m.x x2

    la a0, __data

    // load
    flw.ps fs0, 0(a0)
    call delay

    // gather
    call fgw_test
    call delay
    call fgh_test
    call delay
    call fgb_test
    call delay

    // store
    fsw.ps fs0, 0x300(a0)
    call delay

    // scatter
    call fscw_test
    call delay
    call fsch_test
    call delay
    call fscb_test

end:
    ASM_TEST_PASS
end_loop:
    j end_loop


delay:
    li t0, 64
loop_delay:
    add t0, t0, -1
    bne t0, zero, loop_delay
    ret
    
fgw_test:
    // no miss
    fbci.pi fs1, 0x000
    fgw.ps fs2, fs1(a0)

    // miss first
    fbci.pi fs1, 0x040
    fgw.ps fs2, fs1(a0)
    fcvt.ps.f16 fs3, fs2
   
    // miss last
    li x2, 0x3f
    mova.m.x x2
    fbci.pi fs1, 0x040
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x080
    li x2, 0xff
    mova.m.x x2
    
    fgw.ps fs2, fs1(a0)
    fcvt.ps.f16 fs3, fs2
    
    //miss all
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x0c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x100
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x140
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x180
    li x2, 0xff
    mova.m.x x2
    
    fgw.ps fs2, fs1(a0)
    fcvt.ps.f16 fs3, fs2

    ret

    // -------------------------------------

fgh_test:
    // no miss
    fbci.pi fs1, 0x000
    fgh.ps fs2, fs1(a0)

    //miss all, two consecutive instructions
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x1c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x200
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x240
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x280
    li x2, 0xff
    mova.m.x x2

    fgh.ps fs2, fs1(a0)
    fgh.ps fs3, fs1(a0)

    ret

    // -------------------------------------

fgb_test:
    // no miss
    fbci.pi fs1, 0x000
    fgb.ps fs2, fs1(a0)

    //mixture of instructions
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x1c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x200
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x240
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x280
    li x2, 0xff
    mova.m.x x2

    fgb.ps fs2, fs1(a0)
    fgh.ps fs3, fs1(a0)
    fgw.ps fs2, fs1(a0)
    fgh.ps fs2, fs1(a0)
    fgb.ps fs3, fs1(a0)

    ret

    // -------------------------------------

fscw_test:
    // no miss
    fbci.pi fs1, 0x300
    fscw.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    // miss first
    fbci.pi fs1, 0x400
    fscw.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    // miss last
    li x2, 0x3f
    mova.m.x x2
    fbci.pi fs1, 0x400
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x440
    li x2, 0xff
    mova.m.x x2
    
    fscw.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    // miss all
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x4c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x500
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x540
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x580
    li x2, 0xff
    mova.m.x x2

    fscw.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    ret

    // -------------------------------------

fsch_test:
    // no miss
    fbci.pi fs1, 0x300
    fsch.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)
    
    // miss all, consecutive instructions
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x5c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x600
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x640
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x680
    li x2, 0xff
    mova.m.x x2

    fsch.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)
    fsch.ps fs0, fs1(a0)
    fsch.ps fs2, fs1(a0)
    fgw.ps fs3, fs1(a0)

    ret

    // -------------------------------------

fscb_test:
    // no miss
    fbci.pi fs1, 0x300
    fscb.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    // mixture of instructions
    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x6c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x700
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x740
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x780
    li x2, 0xff
    mova.m.x x2

    fscb.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)
    fsch.ps fs2, fs1(a0)
    fgw.ps fs3, fs1(a0)
    fscw.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)

    li x2, 0x03
    mova.m.x x2
    fbci.pi fs1, 0x7c0
    li x2, 0x0c
    mova.m.x x2
    fbci.pi fs1, 0x800
    li x2, 0x30
    mova.m.x x2
    fbci.pi fs1, 0x840
    li x2, 0xc0
    mova.m.x x2
    fbci.pi fs1, 0x880
    li x2, 0xff
    mova.m.x x2

    fscw.ps fs3, fs1(a0)
    fsch.ps fs0, fs1(a0)
    fgw.ps fs2, fs1(a0)
    fscb.ps fs2, fs1(a0)
    fgw.ps fs3, fs1(a0)

    ret


.data
.global __data
.align 8
__data: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
