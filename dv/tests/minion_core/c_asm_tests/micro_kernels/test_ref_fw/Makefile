# Compiler flags
CC=riscv64-unknown-elf-gcc
CPPFLAGS += -mcmodel=medany -O3 -I$(MINION_DIAGS)/include -I$(RTLROOT)/shire/esr/scripts

# Gets the source and related obj
SRCC := $(shell find src -name "*.cc")
OBJF := $(addprefix build/, $(notdir $(SRCC:.cc=.o)))

all: build/net_test build/net_test.S

build/net_test:	build firmware.mem $(OBJF)
	$(CC) build/*.o -o build/net_test -nostartfiles -nostdlib -T./src/linker.ld

build/net_test.S: build/net_test
	riscv64-unknown-elf-objdump -S -x build/net_test > build/net_test.S

firmware.mem:
	pushd $(FAKEFW)/ > /dev/null ; make ; popd > /dev/null ; cp $(FAKEFW)/obj/firmware.mem .

# c source
build/%.o: src/%.cc
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build firmware.mem
