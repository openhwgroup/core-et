BIN_NAME=fptoint

CSOURCES=main.cc
ASMSOURCES=crt0.S

CXX=riscv64-unknown-elf-g++
CXXFLAGS=-Wall -Werror -O2 -mcmodel=medany
OBJDUMP=riscv64-unknown-elf-objdump
DEPDIR = .depend
DEPFLAGS = -MMD -MF $(DEPDIR)/$*.d
LINK_FLAGS=-nostartfiles
INCDIR=-I$(MINION_DIAGS)/include


OBJECTS=$(CSOURCES:%.cc=%.o) $(ASMSOURCES:%.S=%.o)

$(shell mkdir -p $(DEPDIR))

all: $(BIN_NAME) $(BIN_NAME).S

$(BIN_NAME): $(OBJECTS)
	$(CXX) $(INCDIR) $(OBJECTS) -o $(BIN_NAME) $(LINK_FLAGS)

$(BIN_NAME).S: $(BIN_NAME)
	$(OBJDUMP) -x -S $< > $@

%.o: %.cc
	$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@ -g $(DEPFLAGS)

%.o: %.S
	$(CXX) $(INCDIR) -c $< -o $@

-include $(patsubst %.cc,$(DEPDIR)/%.d, $(CSOURCES))

main.cc: gen_test.pl
	./gen_test.pl > main.cc

clean:
	rm $(OBJECTS) $(BIN_NAME) $(BIN_NAME).S $(DEPDIR) *.mem -rf main.cc

.PHONY: clean all

