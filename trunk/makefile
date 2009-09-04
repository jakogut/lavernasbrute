CXX = g++ -pipe
CXXFLAGS = -c -Wall -O3 -msse2 -msse3 -fomit-frame-pointer -march=$(ARCH) -mfpmath=sse
INCLUDE = -I include/
LIB = lib/libboost_date_time-mgw44-mt-1_38.lib lib/libboost_thread-mgw44-mt-1_38.lib -lpthread
PLATFORM = GCC
ARCH = k8
OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o
DEST = bin/$(PLATFORM)

all: lavernasbrute

lavernasbrute: $(OBJECTS)	
	mkdir $(DEST) && $(CXX) $(OBJECTS) $(LIB) -o $(DEST)/lavernasbrute

$(DEST)/Main.o: source/Main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/Main.cpp -o $(DEST)/Main.o

$(DEST)/CPUPath.o: source/CPUPath.cpp	
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/CPUPath.cpp -o $(DEST)/CPUPath.o

$(DEST)/MasterThread.o: source/MasterThread.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/MasterThread.cpp -o $(DEST)/MasterThread.o

$(DEST)/ProcessingPath.o: source/ProcessingPath.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/ProcessingPath.cpp -o $(DEST)/ProcessingPath.o
