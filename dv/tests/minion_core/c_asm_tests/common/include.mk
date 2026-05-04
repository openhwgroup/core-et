#

#
# This is to be included in a Makefile to build the Minion tests
# This will build an elf for each .cc or .S file in the directory
#
# Before including this file, one can optionally define MANUAL_DEP to build additional stuff
# EXTRA_CFLAGS, EXTRA_LDFLAGS and EXTRA_CLEAN can also be defined
#

BOOT_ADDR?=0x40000000
STACK_TOP?=0x40FC0000
MINION_MARCH?=rv64imf

COMPILE_OPT ?= -O2

BOOT=$(MINION_DIAGS)/common/boot.S

AS = riscv64-unknown-elf-as
CC = riscv64-unknown-elf-gcc
OD = riscv64-unknown-elf-objdump
# Do not remove -fno-delete-null-pointer-checks since is needed for peakrdl-c-headers
# the compiler will optimize away memory accesses otherwise
CFLAGS = -Wall -Werror -Wextra -Wpedantic \
	-I$(MINION_DIAGS)/include \
	-I$(MINION_DIAGS)/vpu/compare \
	-I$(MINION_DIAGS)/vpu/computational \
	-I$(MINION_DIAGS)/micro_kernels \
	-I$(MINION_DIAGS)/vpu/trans \
	-I$(MINION_DIAGS)/vm_boundary/common \
	-I$(MINION_DIAGS)/ultrasoc \
	-I$(RTLROOT)/shire/esr/scripts/ \
	 $(COMPILE_OPT) -g -mcmodel=medany $(EXTRA_CFLAGS) $(EXTRA_FULLCHIP_CFLAGS) $(EXTRA_PRINT_DBG_CFLAGS)\
        -fno-delete-null-pointer-checks \
	-march=$(MINION_MARCH) -mabi=lp64f -Wa,-march=$(MINION_MARCH),-mabi=lp64f

LDFLAGS = $(EXTRA_LDFLAGS)
ifndef FULL_CHIP_TB
LDFLAGS +=  -T $(MINION_DIAGS)/common/erbium.ld
else 
LDFLAGS +=  -T $(IOSHIRE_DIAGS)/scripts/msMin/riscV.ld
endif

USE_STDLIB ?=0

ifdef STACK_BASE
CFLAGS+= -DSTACK_BASE=$(STACK_BASE)
endif

ifdef STACK_SIZE_LOG2
CFLAGS+= -DSTACK_SIZE_LOG2=$(STACK_SIZE_LOG2)
endif

CFLAGS+=${IOSHIRE_INCLUDE} 

# By default stdlib is not used, in case certain tests need stdlib then the USE_STDLIB has to be set
ifeq ($(USE_STDLIB),0)
    LDFLAGS += -nostdlib -nostartfiles
    CRT = $(MINION_DIAGS)/common/crt.S
else
    CRT = $(MINION_DIAGS)/common/crt.S
    LDFLAGS += -nostartfiles
endif

C_BIN_SRC = $(filter-out $(EXTRA_SRC),$(wildcard *.c))
CC_BIN_SRC = $(filter-out $(EXTRA_SRC),$(wildcard *.cc))
S_BIN_SRC  = $(filter-out $(EXTRA_SRC),$(wildcard *.S))

C_BIN = $(patsubst %.c,%,$(C_BIN_SRC))
CC_BIN = $(patsubst %.cc,%,$(CC_BIN_SRC))
S_BIN  = $(patsubst %.S,%,$(S_BIN_SRC))

C_DASM = $(patsubst %.c,%.dasm,$(C_BIN_SRC))
CC_DASM = $(patsubst %.cc,%.dasm,$(CC_BIN_SRC))
S_DASM  = $(patsubst %.S,%.dasm,$(S_BIN_SRC))

all: $(S_BIN) $(CC_BIN) $(C_BIN) $(S_DASM) $(CC_DASM) $(C_DASM)
	@echo "$(CFLAGS)";
%: %.c $(MANUAL_DEP) $(EXTRA_SRC)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_INCLUDE) $(CRT) $(LDFLAGS) -o $@ $(BOOT) $< $(EXTRA_SRC)

%: %.cc $(MANUAL_DEP) $(EXTRA_SRC)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_INCLUDE) $(CRT) $(LDFLAGS) -o $@ $(BOOT) $< $(EXTRA_SRC)

%: %.S $(MANUAL_DEP) $(EXTRA_SRC)
	$(CC) $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) $(SLDFLAGS) -o $@ $(BOOT) $< $(EXTRA_SRC)

%.o: %.cc $(MANUAL_DEP) $(EXTRA_SRC)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_INCLUDE) $(LDFLAGS) -c $< $(EXTRA_SRC)

%.dasm: %
	$(OD) -xSd -M numeric,no-aliases $< > $@

clean: $(EXTRA_CLEAN)
	rm -f *.o $(S_BIN) $(CC_BIN) $(C_BIN) $(CC_DASM) $(S_DASM) $(C_DASM)

test:
	@echo "$(CFLAGS)";
.PHONY: all clean $(EXTRA_CLEAN)
