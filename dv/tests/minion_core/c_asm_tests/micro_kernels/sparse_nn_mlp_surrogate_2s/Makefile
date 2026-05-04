# Compiler flags
CC=riscv64-unknown-elf-gcc
CPPFLAGS += -mcmodel=medany -O3 -D N_SHIRES_COMPUTE=1 -D SINGLE_PASS  -I$(MINION_DIAGS)/include -I$(RTLROOT)/shire/esr/scripts

# Gets the source and related obj
SRCC := $(shell find src/ | grep "\.cc" | grep -v swp)
OBJF := $(addprefix build/, $(notdir $(SRCC:.cc=.o)))

all: build/net_test build/net_test.S net_desc.mem sparse_nn_mlp_surrogate_2s weights.mem src/test_act_data.h

build/net_test:	build firmware.mem $(OBJF)
	$(CC) build/*.o -o build/net_test -nostartfiles -nostdlib -T./src/linker.ld

build/net_test.S: build/net_test
	riscv64-unknown-elf-objdump -S -x build/net_test > build/net_test.S

net_desc.mem: build/net_test
	./build_net_desc.pl

sparse_nn_mlp_surrogate_2s:
	ln -s build/net_test sparse_nn_mlp_surrogate_2s

weights.mem: ./build/gen_weight_mem
	./build/gen_weight_mem

src/test_act_data.h: ./build/gen_act_data
	./build/gen_act_data

firmware.mem:
	pushd $(FAKEFW)/ > /dev/null ; make ; popd > /dev/null ; cp $(FAKEFW)/obj/firmware.mem .

# c source
build/%.o: src/%.cc src/*.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/gen_weight_mem: gen_weight_mem.cc
	g++ -std=gnu++11 -mf16c gen_weight_mem.cc -o ./build/gen_weight_mem

build/gen_act_data: gen_act_data.cc
	g++ -std=gnu++11 -mf16c gen_act_data.cc -o ./build/gen_act_data

build:
	mkdir build

clean:
	rm -rf build net_desc.mem sparse_nn_mlp_surrogate_2s memImage.hex weights.mem firmware.mem

