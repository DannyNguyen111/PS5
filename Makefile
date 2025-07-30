# Compiler and flags
CC = g++
CFLAGS = -std=c++20 -Wall -Werror -pedantic
BOOST_LIBS = -lboost_unit_test_framework

# Source files
SOURCES = RandWriter.cpp TextWriter.cpp
TEST_SOURCE = test.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJ = $(TEST_SOURCE:.cpp=.o)

# Executables and library
EXE = TextWriter
LIB = TextWriter.a
TEST_EXE = test

.PHONY: all clean lint

# Default target builds everything
all: $(EXE) $(TEST_EXE)

# Build static library from RandWriter.o
$(LIB): RandWriter.o
	ar rcs $@ $^

# Build executable TextWriter
$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Build unit test executable
$(TEST_EXE): $(TEST_OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(BOOST_LIBS)

# Generic compile rule
%.o: %.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c $< -o $@

# Lint target
lint:
	cpplint *.cpp *.hpp

# Clean up
clean:
	rm -f *.o $(EXE) $(LIB) $(TEST_EXE)

