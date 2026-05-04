include $(FAKEFW)/firmware.mk

CC  := riscv64-unknown-elf-gcc
LD  := riscv64-unknown-elf-gcc
CFLAGS  := -mcmodel=medany -O3
LDFLAGS  := -T$(FAKEFW)/src/linker.ld -nostartfiles -nostdlib -O3
TARGET := net_test

SRCS := $(wildcard *.cc)
OBJS := $(patsubst %.cc,%.o,$(SRCS)) $(FIRMWARE_OBJS) crt.o

all: $(TARGET)
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
%.o: %.cc
	$(CC) $(CFLAGS) -c $<
clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean
