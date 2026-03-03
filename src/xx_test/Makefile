# Compiler and flags
CC = clang++
CFLAGS = -Wall -std=c++17
LDFLAGS = 

# Target executable name
TARGET = test

# Source file
SRC = test.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean run rebuild