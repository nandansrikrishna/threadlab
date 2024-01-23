CC=g++ -g -Wall -std=c++17

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC+=-D_XOPEN_SOURCE
    LIBTHREAD=libthread_macos.o
else
    LIBTHREAD=libthread.o
endif

# List of source files for your disk scheduler
LAB_SOURCES=threadlab.cpp

# Generate the names of the disk scheduler's object files
LAB_OBJS=${LAB_SOURCES:.cpp=.o}

all: threadlab

# Compile the disk scheduler and tag this compilation
threadlab: ${LAB_OBJS} ${LIBTHREAD}
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${LAB_OBJS} threadlab
