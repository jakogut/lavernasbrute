CXX = g++ -pipe
CXXFLAGS = -c -Wall -O3 -msse2 -msse3 -fomit-frame-pointer -march=$(ARCH) -mfpmath=sse
INCLUDE = -I include/
LIB = lib/libboost_date_time-*-mt-1_38.a lib/libboost_thread-*-mt-1_38.a -lpthread
OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/Director.o
ARCH = k8
PLATFORM = GCC
DEST = bin/$(PLATFORM)

all: lavernasbrute

lavernasbrute: $(OBJECTS)	
	$(CXX) $(OBJECTS) $(LIB) -o $(DEST)/lavernasbrute

$(DEST)/Main.o: source/Main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/Main.cpp -o $(DEST)/Main.o

$(DEST)/CPUPath.o: source/CPUPath.cpp	
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/CPUPath.cpp -o $(DEST)/CPUPath.o

$(DEST)/MasterThread.o: source/MasterThread.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/MasterThread.cpp -o $(DEST)/MasterThread.o

$(DEST)/ProcessingPath.o: source/ProcessingPath.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/ProcessingPath.cpp -o $(DEST)/ProcessingPath.o

$(DEST)/Director.o: source/Director.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) source/Director.cpp -o $(DEST)/Director.o
