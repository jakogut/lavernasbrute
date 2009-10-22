CXX = g++
CXXFLAGS = -Wall -O3 -pg

INCLUDE = -I/usr/include -Iinclude
LIB = -L/usr/lib -Llib -lpthread -lboost_date_time-mt -lboost_thread-mt
DEST = bin/GCC

OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/Director.o

lavernasbrute: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LIB) $? -o $(DEST)/$@

$(DEST)/%.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c source/$*.cpp -o $@
