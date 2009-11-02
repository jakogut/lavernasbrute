CXX = g++
CXXFLAGS = -Wall -O3 # $(DEBUG) $(PROFILE)
DEBUG = -g
PROFILE = -pg

INCLUDE = -I/usr/include -Iinclude
LIB = -L/usr/lib -Llib -lpthread -lboost_date_time-mt -lboost_thread-mt
DEST = bin/GCC

OBJECTS = $(DEST)/Main.o $(DEST)/CPUPath.o $(DEST)/MasterThread.o $(DEST)/ProcessingPath.o $(DEST)/Director.o

MKDIR = mkdir -p
RM = rm -f

lavernasbrute: $(OBJECTS)
	@$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(LIB) $? -o $(DEST)/$@

$(DEST)/%.o:
	@$(MKDIR) $(DEST)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c source/$*.cpp -o $@

clean:
	$(RM) $(DEST)/lavernasbrute $(DEST)/gmon.out $(OBJECTS)

.PHONY: clean
