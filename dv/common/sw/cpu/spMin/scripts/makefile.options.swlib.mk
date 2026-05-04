### GNU CC riscV Tools selection
AS          := riscv64-unknown-elf-gcc  ### quick, dirty fix for current macro.h
CC          := riscv64-unknown-elf-gcc
AR          := riscv64-unknown-elf-ar
LN          := riscv64-unknown-elf-ld
DUMP        := riscv64-unknown-elf-objdump
CPY         := riscv64-unknown-elf-objcopy

## Processor Selection
PROC  =  -march=rv64imaf
PRINT_OPT =

## RVASM Options  RDJ: Split Include path and defines
RVASM_FLAGS  += ${PROC}
RVASM_FLAGS  += -c
#RVASM_FLAGS  += --defsym __IOSHIRESW__=1
RVASM_FLAGS  += -mabi=lp64f


## RiscV GCC
RVCC_FLAGS  += ${PROC}
RVCC_FLAGS  += -g3
RVCC_FLAGS  += -O0
RVCC_FLAGS  += -D __IOSHIRESW__
RVCC_FLAGS  += -nostdlib
RVCC_FLAGS  += -mcmodel=medany
RVCC_FLAGS  += -mabi=lp64f
RVCC_FLAGS  += -Wa,-march=rv64imaf,-mabi=lp64f

RVCC_FLAGS  += -std=gnu11
RVCC_FLAGS  += --specs=nano.specs
RVCC_FLAGS  += -flto
RVCC_FLAGS  += -ffunction-sections
RVCC_FLAGS  += -fdata-sections
RVCC_FLAGS  += -Wall
RVCC_FLAGS  += -Wextra
RVCC_FLAGS  += -Werror
RVCC_FLAGS  += -Wdouble-promotion
RVCC_FLAGS  += -Wformat
RVCC_FLAGS  += -Wnull-dereference
RVCC_FLAGS  += -Wduplicated-branches
RVCC_FLAGS  += -Wduplicated-cond
RVCC_FLAGS  += -Wshadow
RVCC_FLAGS  += -Wpointer-arith
RVCC_FLAGS  += -Wundef
RVCC_FLAGS  += -Wbad-function-cast
RVCC_FLAGS  += -Wcast-qual
RVCC_FLAGS  += -Wcast-align
RVCC_FLAGS  += -Wconversion
RVCC_FLAGS  += -Wlogical-op
RVCC_FLAGS  += -Wstrict-prototypes
RVCC_FLAGS  += -Wmissing-prototypes
RVCC_FLAGS  += -Wmissing-declarations
RVCC_FLAGS  += -ffat-lto-objects

override DEFINES     += ${PRINT_OPT}



## RiscV LINK
RVLINK_FLAGS += -nostartfiles
RVLINK_FLAGS += -nostdlib
RVLINK_FLAGS += --gc-sections
RVLINK_FLAGS += --entry=Reset_Handler
RVLINK_FLAGS += -T $(scatter_file)
RVLINK_FLAGS += -L $(RISCV)/riscv64-unknown-elf/lib/
RVLINK_FLAGS += --use-linker-plugin

## RiscV OBJDUMP
RVOBJDUMP_FLAGS += -D

## RiscV OBJCOPY
RVOBJCOPY_FLAGS += -O ihex

