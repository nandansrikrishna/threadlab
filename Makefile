CC=g++ -g3 -Wall -std=c++17 -I./

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC+=-D_XOPEN_SOURCE
    LIBTHREAD=libthread_macos.o
else
    LIBTHREAD=libthread.o
endif

# List of source files
LAB_SOURCES=TODO.cpp
EXE_NAME=${LAB_SOURCES:.cpp=}
LAB_SOLUTION_SOURCES=solutions/threadlab.cpp

# Generate the names of the object files
LAB_OBJS=${LAB_SOURCES:.cpp=.o}
LAB_SOLUTION_OBJS=${LAB_SOLUTION_SOURCES:.cpp=.o}

all: ${EXE_NAME} threadlab_solution

${EXE_NAME}: ${LAB_OBJS} ${LIBTHREAD}
	${CC} -o $@ $^ -ldl -pthread
threadlab_solution: ${LAB_SOLUTION_OBJS} ${LIBTHREAD}
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -o $@ -c $<
%.o: %.cc
	${CC} -o $@ -c $<

clean:
	rm -f ${EXE_NAME} ${LAB_OBJS} ${LAB_SOLUTION_OBJS} threadlab_solution
