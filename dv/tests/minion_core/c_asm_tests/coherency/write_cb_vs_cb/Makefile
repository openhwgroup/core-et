# Compiler flags
CC=riscv64-unknown-elf-gcc
CPPFLAGS += -mcmodel=medany -O2 -I$(MINION_DIAGS)/include -I$(RTLROOT)/shire/esr/scripts/

# Gets the source and related obj
SRCC := $(shell find src/ | grep "\.cc" | grep -v swp)
OBJF := $(addprefix build/, $(notdir $(SRCC:.cc=.o)))

all: build build/net_test firmware build/net_test.S net_desc.mem test

build:
	mkdir build

build/net_test:	$(OBJF)
	$(CC) build/*.o -o build/net_test -nostartfiles -nostdlib -T./linker.ld

# c source
build/%.o: src/%.cc
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

firmware:
	pushd $(FAKEFW)/ > /dev/null ; make ; popd > /dev/null ; cp $(FAKEFW)/obj/firmware.mem .

build/net_test.S: build/net_test
	riscv64-unknown-elf-objdump -S -x build/net_test > build/net_test.S

net_desc.mem: build/net_test
	./build_net_desc.pl

test:
	ln -s build/net_test test

clean:
	rm -rf build net_desc.mem test memImage.hex weights.mem firmware.mem

