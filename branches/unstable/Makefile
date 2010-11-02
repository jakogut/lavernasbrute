OPTIMIZATION=-O3

CXX = g++
CC = gcc
NVCC = nvcc
CXXFLAGS = $(OPTIMIZATION) -Wall -march=$(ARCH)
CCFLAGS = $(CXXFLAGS)
NVCC_FLAGS = $(CXXFLAGS)

ARCH = i686
BITNESS = 32

SRC = source
SRC_ASM = $(SRC)/asm
INCLUDE = -I/usr/include -Iinclude -Isource
LIB = -L/usr/lib -Llib -lboost_date_time-mt -lboost_thread-mt
DEST = bin/gcc/$(ARCH)

OBJECTS_MAIN = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/SSEPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/BloomFilter.o $(DEST)/CharacterSet.o
OBJECTS_TESTS = $(DEST)/MD4_UT.o $(DEST)/NTLM_UT.o $(DEST)/NTLM_SSE_UT.o

ASM = $(SRC_ASM)/Main.asm $(SRC_ASM)/CPUPath.asm $(SRC_ASM)/SSEPath.asm $(SRC_ASM)/MasterThread.asm $(SRC_ASM)/ProcessingPath.asm $(SRC_ASM)/BloomFilter.asm $(SRC_ASM)/CharacterSet.asm

# If x86-64 is specified, build a 64-bit binary with SSE support.
ifeq ($(ARCH), x86-64)
	BITNESS = 64
	CXXFLAGS += -DSSE
endif

ifeq ($(BITNESS), 32)
	CXXFLAGS += -m32
else ifeq ($(BITNESS), 64) 
	CXXFLAGS += -m64
endif

lavernasbrute: $(OBJECTS_MAIN)
	mkdir -p $(DEST)
	$(CXX) $(CXXFLAGS) $(LIB) $? -o $(DEST)/$@

asm: $(ASM)

# Object build rules
$(DEST)/%.o: $(SRC)/%.cpp
	mkdir -p $(DEST)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/$*.cpp -o $@

$(DEST)/%.o: $(SRC)/%.c
	mkdir -p $(DEST)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $(SRC)/$*.c -o $@

# Assembly build rules
$(SRC_ASM)/%.asm: $(SRC)/%.cpp
	mkdir -p $(SRC_ASM)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -S -masm=intel $(SRC)/$*.cpp -o $@

$(SRC_ASM)/%.asm: $(SRC)/%.c
	mkdir -p $(SRC_ASM)
	$(CC) $(CCFLAGS) $(INCLUDE) -S -masm=intel $(SRC)/$*.c -o $@

# CUDA build rule
$(DEST)/%.o : $(SRC)/%.cu
	mkdir -p $(DEST)
	$(NVCC) $(NVCC_FLAGS) $(INCLUDE) -c $(SRC)/$*.cu -o $@

# tests: MD4_UT NTLM_UT NTLM_SSE_UT
#	@echo -e "\nRunning tests:"
#	@$(DEST)/tests/MD4_UT
#	@$(DEST)/tests/NTLM_UT
#	@$(DEST)/tests/NTLM_SSE_UT
	
clean:
	@rm -rf $(DEST)
	@rm -rf $(SRC_ASM)

.PHONY: clean
