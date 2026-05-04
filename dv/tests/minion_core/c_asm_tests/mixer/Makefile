BOOT_ADDR?=0x8000001000
STACK_TOP?=0x80A1FFC000
MINION_MARCH?=rv64imf

COMPILE_OPT ?= -O2

BOOT=$(MINION_DIAGS)/common/boot.S

AS = riscv64-unknown-elf-as
CC = riscv64-unknown-elf-gcc
OD = riscv64-unknown-elf-objdump
CFLAGS = -Wall -Werror -Wextra -Wpedantic -I$(MINION_DIAGS)/include -I$(MINION_DIAGS)/micro_kernels $(COMPILE_OPT) -g -mcmodel=medany $(EXTRA_CFLAGS) \
	-march=$(MINION_MARCH) -mabi=lp64f -Wa,-march=$(MINION_MARCH),-mabi=lp64f
LDFLAGS = $(EXTRA_LDFLAGS) \
	 -Wl,--section-start=bootrom=$(BOOT_ADDR),--section-start=stack=$(STACK_TOP)
USE_STDLIB ?=0

ifdef STACK_BASE
CFLAGS+= -DSTACK_BASE=$(STACK_BASE)
endif

ifdef STACK_SIZE_LOG2
CFLAGS+= -DSTACK_SIZE_LOG2=$(STACK_SIZE_LOG2)
endif

CFLAGS+=${IOSHIRE_INCLUDE} 

# By default stdlib is not used, in case certain tests need stdlib then the USE_STDLIB has to be set
LDFLAGS += -nostdlib -nostartfiles
CRT = $(MINION_DIAGS)/common/crt.S

SRC_C = mixer.cc mtwister.cc
SRC_H = mixer.h mtwister.h

#OBJS = *.o
OBJS =

ROUND_SRC_C = round.cc mtwister.cc
ROUND_SRC_H = mixer.h mtwister.h
ROUND_INC   = -I$(MINION_DIAGS)/include -I$(MINION_DIAGS)/micro_kernels -I$(FAKEFW)/include


round: $(ROUND_SRC_C) $(ROUND_SRC_H) mixermake.pl
	./mixermake.pl
	gcc -std=gnu++11 $(ROUND_INC) -o $@ $(ROUND_SRC_C)


mixer: $(SRC_C) $(SRC_H) $(CRT) mixermake.pl
	rm -f *.o
	./mixermake.pl
	$(CC) $(CFLAGS) -Wno-write-strings $(LDFLAGS) -o $@ $(SRC_C) $(OBJS) $(CRT)

clean:
	rm *.o mixer


