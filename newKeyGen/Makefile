CXX = g++
CXXFLAGS = -Wall -O3 -march=$(ARCH) # $(DEBUG) $(PROFILE)
ARCH = i386
DEBUG = -g
PROFILE = -pg

INCLUDE = -I/usr/include -Iinclude -Isource/hashing
LIB = -L/usr/lib -Llib -lboost_date_time-mt -lboost_thread-mt
DEST = bin/gcc/$(ARCH)

OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/SSE2Path.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/Director.o

MKDIR = mkdir -p
RM = rm -f

ifneq ($(ARCH), x86-64)
	CXXFLAGS += -m32 -msse -msse2 -mfpmath=sse
else
	CXXFLAGS += -m64
endif

lavernasbrute: $(OBJECTS)
	$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(LIB) $? -o $(DEST)/$@

$(DEST)/%.o:
	$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c source/$*.cpp -o $@

clean:
	$(RM) $(DEST)/lavernasbrute $(DEST)/gmon.out $(OBJECTS)

.PHONY: clean
