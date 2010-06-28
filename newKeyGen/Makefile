CXX = g++
CXXFLAGS = -Wall -O3 -march=$(ARCH) # $(PROFILE) # $(DEBUG)
ARCH = i686
BITNESS = 32
DEBUG = -g
PROFILE = -pg

INCLUDE = -I/usr/include -Iinclude -Isource -Isource/hashing
LIB = -L/usr/lib -Llib -lboost_date_time-mt -lboost_thread-mt
DEST = bin/gcc/$(ARCH)

OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/SSEPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/Director.o
MKDIR = mkdir -p
RM = rm -f

ifeq ($(ARCH), x86-64)
	BITNESS = 64
	CXXFLAGS += -DSSE
endif

ifeq ($(BITNESS), 32)
	CXXFLAGS += -m32
else ifeq ($(BITNESS), 64) 
	CXXFLAGS += -m64
endif

lavernasbrute: $(OBJECTS)
	$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(LIB) $? -o $(DEST)/$@

$(DEST)/%.o:
	$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c source/$*.cpp -o $@
	
tests: MD4_UT NTLM_UT NTLM_SSE_UT
	@echo -e "\nRunning tests:"
	@$(DEST)/tests/MD4_UT
	@$(DEST)/tests/NTLM_UT
	@$(DEST)/tests/NTLM_SSE_UT
	
MD4_UT: 
	$(MKDIR) $(DEST)/tests/
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/tests/MD4_UT.cpp -o $(DEST)/tests/MD4_UT

NTLM_UT: 
	$(MKDIR) $(DEST)/tests/
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/tests/NTLM_UT.cpp -o $(DEST)/tests/NTLM_UT
	
NTLM_SSE_UT: NTLM_UT
	$(MKDIR) $(DEST)/tests/
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/tests/NTLM_SSE_UT.cpp -o $(DEST)/tests/NTLM_SSE_UT

clean:
	$(RM) $(DEST)/lavernasbrute $(DEST)/gmon.out $(OBJECTS)

.PHONY: clean
