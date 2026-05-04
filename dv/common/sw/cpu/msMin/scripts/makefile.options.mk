### GNU CC riscV Tools selection
AS          := riscv64-unknown-elf-gcc  ### quick, dirty fix for current macro.h
CC          := riscv64-unknown-elf-gcc
AR          := riscv64-unknown-elf-ar
LN          := riscv64-unknown-elf-ld
DUMP        := riscv64-unknown-elf-objdump
CPY         := elf2hex
MSMIN_CODE  := 1

## Processor Selection
PROC  =  -march=rv64imaf


## RVASM Options  RDJ: Split Include path and defines
RVASM_FLAGS  += ${PROC}
RVASM_FLAGS  += -c
#RVASM_FLAGS  += --defsym __IOSHIRESW__=1
RVASM_FLAGS  += -mabi=lp64f


## RiscV GCC
RVCC_FLAGS  += ${PROC}
RVCC_FLAGS  += -g
RVCC_FLAGS  += -O0
RVCC_FLAGS  += -D __IOSHIRESW__
RVCC_FLAGS  += -nostdlib
RVCC_FLAGS  += -mcmodel=medany
RVCC_FLAGS  += -mabi=lp64f
RVCC_FLAGS  += -Wa,-march=rv64imaf,-mabi=lp64f

override DEFINES     += ${PRINT_OPT}


## RiscV LINK
RVLINK_FLAGS += -nostartfiles
RVLINK_FLAGS += -nostdlib
RVLINK_FLAGS += --gc-sections
RVLINK_FLAGS += --entry=Reset_Handler
RVLINK_FLAGS += -T $(scatter_file)
RVLINK_FLAGS += -L $(RISCV)/riscv64-unknown-elf/lib/

## RiscV OBJDUMP
RVOBJDUMP_FLAGS += -D

## RiscV OBJCOPY
RVOBJCOPY_FLAGS += -O ihex

