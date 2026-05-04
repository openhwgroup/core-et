BIN_NAME=basic

C_SOURCES=basic.cc
ASM_SOURCES=trap_handler.S

CXX=riscv64-unknown-elf-g++
CXXFLAGS=-Wall -Werror -mcmodel=medany
OBJDUMP=riscv64-unknown-elf-objdump
DEPDIR = .depend
DEPFLAGS = -MMD -MF $(DEPDIR)/$*.d
INCDIR=-I$(MINION_DIAGS)/include

OBJECTS=$(C_SOURCES:%.cc=%.o) $(ASM_SOURCES:%.S=%.o)

$(shell mkdir -p $(DEPDIR))

all: $(BIN_NAME) $(BIN_NAME).dasm 

$(BIN_NAME): $(OBJECTS)
	$(CXX) $(INCDIR) $(OBJECTS) -o $(BIN_NAME) 


$(BIN_NAME).dasm: $(BIN_NAME)
	$(OBJDUMP) -x -S $< > $@


%.o: %.cc
	$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@ -g $(DEPFLAGS)

%.o: %.S
	$(CXX) $(INCDIR) $(CXXFLAGS) -c $< -o $@ -g 

-include $(patsubst %.cc,$(DEPDIR)/%.d, $(C_SOURCES))


clean:
	rm $(OBJECTS) $(BIN_NAME) $(BIN_NAME).dasm $(OBJECTS) \
     $(DEPDIR) bootrom.mem mem_desc.txt tbox_test.mem -rf

.PHONY: clean all
