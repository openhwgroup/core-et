.macro TIMER_START
        slti x0, x0, 0x600
.endm

.macro TIMER_END
        slti x0, x0, 0x601
.endm
