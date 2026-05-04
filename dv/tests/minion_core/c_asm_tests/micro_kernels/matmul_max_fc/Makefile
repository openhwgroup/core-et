TARGET = $(notdir $(CURDIR))

# Compiler flags
CC=riscv64-unknown-elf-gcc
CPPFLAGS += -mcmodel=medany -O3 -I$(MINION_DIAGS)/include -I$(RTLROOT)/shire/esr/scripts
CPPFLAGS += -D N_SHIRES_COMPUTE=32

# Gets the source and related obj
SRCC := $(shell find src/ -name "*.cc")
SRCH := $(shell find src/ -name "*.h")
SRCC := $(filter-out $(SRCH),$(SRCC))
SRCH := $(shell find src/ -name *.swp)
SRCC := $(filter-out $(SRCH),$(SRCC))
SRCC := $(filter-out src/,$(SRCC))
OBJF := $(addprefix build/, $(notdir $(SRCC:.cc=.o)))

all: build/net_test build/net_test.S net_desc.mem $(TARGET)

build/net_test:	build firmware.mem $(OBJF)
	$(CC) build/*.o -o build/net_test -nostartfiles -nostdlib -T./src/linker.ld

build/net_test.S: build/net_test
	riscv64-unknown-elf-objdump -S -x build/net_test > build/net_test.S

net_desc.mem: build/net_test
	./build_net_desc.pl

$(TARGET):
	ln -s build/net_test $(TARGET)
	./gen_weight_mem.pl
	./gen_scale_offset.pl

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

