#
# Clement Poh
#
# Makefile for assignment two of COMP20007 2016.
#
# Replace '<STUDENT-ID>' with your Student ID e.g. 269508
STUDENTID = 736901
SUBMIT	= hash.c types.c extra.c report.pdf

# Define the C compiler
CC      = gcc

# Define the compiler flags
#  -Wall to turn on most of the warnings
#  -Wextra to turn on even more warnings
#  -Wpedantic to turn on even more warnings
#  -ansi to ensure ansi compliance
#  -std=c99 to ensure c99 compliance
#  -m32 to compile for 32 bit architectures
#  -O0 to turn off optimisations
#  -g  to include debugging symbols
#  -pg for profiling
CFLAGS  = -Wall -Wextra -Wpedantic -O0 -g # -ansi -std=c99 -m32 -pg

# Define any libraries to be linked
LIB		=

# Define the source, header and object files
HDR		= src/array.h src/list.h src/hashtable.h src/hash.h src/types.h src/extra.h
SRC		= $(HDR:.h=.c) src/main.c
OBJ   = $(SRC:.c=.o)

# The executable name
TARGET	= bin/hashing

all: compile run

compile: $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIB)

run:
	./$(TARGET) -t s -p -f test/some-fruit.txt

clean:
	rm -f $(OBJ)

usage: $(TARGET)
	./$(TARGET)

tags:
	ctags -w $(SRC)

ints: $(TARGET)
	./$(TARGET) -h n -m test/ints.in -p

test: $(TARGET)
	./$(TARGET) -h u -m -t s test/str.in -f test/search.in -p

collide: $(TARGET)
	./$(TARGET) -h u -t s -c 1 test/str.in

other:
	./$(TARGET) -t s -c "$1" -s "$2" 1element.txt > out.txt
	./$(TARGET) -t s -s "$2" -h u -f out.txt out.txt

$(OBJ): $(HDR)
