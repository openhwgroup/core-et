TARGET = $(notdir $(CURDIR))

# Compiler flags
CC=riscv64-unknown-elf-gcc
CPPFLAGS += -mcmodel=medany -O3 -I$(MINION_DIAGS)/include -I$(RTLROOT)/shire/esr/scripts
CPPFLAGS += -D SIM_SHIRES=32

# Gets the source and related obj
SRCC := $(shell find src/ -name "*.cc")
SRCH := $(shell find src/ -name "*.h")
SRCH := $(shell find src/ -name *.swp)
OBJF := $(addprefix build/, $(notdir $(SRCC:.cc=.o)))

all: build/net_test build/net_test.S net_desc.mem $(TARGET) build/gen_weights

build/net_test:	build firmware.mem $(OBJF)
	$(CC) build/*.o -o build/net_test -nostartfiles -nostdlib -T./src/linker.ld

build/net_test.S: build/net_test
	riscv64-unknown-elf-objdump -S -x build/net_test > build/net_test.S

net_desc.mem: build/net_test
	./build_net_desc.pl

$(TARGET):
	ln -s build/net_test $(TARGET)

build/gen_weights: gen_weights.cc
	gcc gen_weights.cc -o build/gen_weights

firmware.mem:
	pushd $(FAKEFW)/ ; make ; popd ; cp $(FAKEFW)/obj/firmware.mem .

# c source
build/%.o: src/%.cc src/test_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	rm -rf build net_desc.mem $(TARGET) memImage.hex weights.mem offsets.mem scales.mem firmware.mem

print-% : ; @echo $* = $($*)

