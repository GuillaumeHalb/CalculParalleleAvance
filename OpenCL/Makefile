
ifndef CXX
	CXX=g++
endif

CXXFLAGS=-O3 -ffast-math

LIBS = -lm -lOpenCL

COMMON_DIR = ../../CXX_common

INC = -I $(COMMON_DIR)

BIN_OBJS = binomial.o wtime.o
EXEC = binomial

# Check our platform and make sure we define the APPLE variable
# and set up the right compiler flags and libraries
PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM), Darwin)
	CXX = clang++
	CXXFLAGS += -stdlib=libc++
	LIBS = -lm -framework OpenCL
endif

all: $(EXEC)

binomial: $(BIN_OBJS)
	$(CXX) $(BIN_OBJS) $(CXXFLAGS) $(LIBS) -o $(EXEC)

wtime.o: $(COMMON_DIR)/wtime.c
	$(CXX) -c $^ $(CXXFLAGS) -o $@


.c.o:
	$(CXX) -c $< $(CXXFLAGS) -o $@

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS) $(INC) -o $@


clean:
	rm -f $(BIN_OBJS) $(EXEC)
