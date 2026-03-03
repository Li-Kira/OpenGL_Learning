# Directory paths
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = ../../include
EXTERNAL_DIR = ../../external
TEXTURE_DIR = ../../resource/texture
MODEL_DIR = ../../resource/model
TARGET = Lighting

# Source files
APP_SRC = Lighting.cpp
GLAD_SRC = ../glad.c
STD_SRC = ../stb_image.cpp
APP_OBJ = $(OBJ_DIR)/Lighting.o
GLAD_OBJ = $(OBJ_DIR)/glad.o
STD_OBJ = $(OBJ_DIR)/stb_image.o
STD_OBJ = $(OBJ_DIR)/stb_image.o

# Lighting mode configuration
ifdef Lighting
DBG_OPT_FLAGS = -g
else
DBG_OPT_FLAGS = -O2
endif

# Address sanitizer configuration
ifdef ASAN
ASAN_FLAGS = -fsanitize=address
else
ASAN_FLAGS =
endif

# Compiler and flags
CC = clang++
CFLAGS = -Wall -std=c++17 \
         -I$(INCLUDE_DIR) \
         -I$(EXTERNAL_DIR)/include \
         -I/opt/homebrew/include \
         $(DBG_OPT_FLAGS) \
         $(ASAN_FLAGS) \

LDFLAGS = -L/opt/homebrew/lib \
          -lglfw \
		  -lassimp \
          -framework OpenGL \
          -framework Cocoa \
          -framework IOKit \
          -framework CoreVideo

# Ensure build directory exists
$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

# Compile C++ files
$(APP_OBJ): $(APP_SRC) | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Compile C files (for glad)
$(GLAD_OBJ): $(GLAD_SRC) | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -I$(EXTERNAL_DIR)/include $< -o $@

# Compile Cpp files (for std)
$(STD_OBJ): $(STD_SRC) | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -I$(EXTERNAL_DIR)/include $< -o $@

# Link executable
$(BUILD_DIR)/$(TARGET): $(APP_OBJ) $(GLAD_OBJ) $(STD_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Run target
run: all
	cd $(BUILD_DIR) && ./$(TARGET)

# Clean rule
clean:
	rm -rf $(BUILD_DIR)

# Create project structure
init:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean run init rebuild