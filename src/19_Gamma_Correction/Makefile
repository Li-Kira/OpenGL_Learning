# Directory paths
SRC_DIR = .
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
INCLUDE_DIR = ../../include
EXTERNAL_DIR = ../../external
SHADER_DIR = ../../resource/shader
TEXTURE_DIR = ../../resource/texture
MODEL_DIR = ../../resource/model
TARGET = Gamma_Correction

# Source files
APP_SRC = Gamma_Correction.cpp
GLAD_SRC = ../glad.c
STD_SRC = ../stb_image.cpp
APP_OBJ = $(OBJ_DIR)/Gamma_Correction.o
GLAD_OBJ = $(OBJ_DIR)/glad.o
STD_OBJ = $(OBJ_DIR)/stb_image.o
IMGUI_SRCS = $(EXTERNAL_DIR)/imgui/imgui.cpp \
             $(EXTERNAL_DIR)/imgui/imgui_demo.cpp \
             $(EXTERNAL_DIR)/imgui/imgui_draw.cpp \
             $(EXTERNAL_DIR)/imgui/imgui_tables.cpp \
             $(EXTERNAL_DIR)/imgui/imgui_widgets.cpp \
             $(EXTERNAL_DIR)/imgui/backends/imgui_impl_glfw.cpp \
             $(EXTERNAL_DIR)/imgui/backends/imgui_impl_opengl3.cpp

# Debug mode configuration
ifdef DEBUG
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
		 -I$(EXTERNAL_DIR)/imgui \
		 -I$(EXTERNAL_DIR)/imgui/backends

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

# Copy shader files
copy_shaders: | $(BUILD_DIR)
	@echo "Copying shaders..."
	@if [ -d "$(SHADER_DIR)" ]; then \
		mkdir -p $(BUILD_DIR)/res/shader/; \
		cp -r $(SHADER_DIR)/* $(BUILD_DIR)/res/shader/ 2>/dev/null || true; \
	else \
		echo "Warning: Shader directory does not exist"; \
	fi

# Copy texture files
copy_textures: | $(BUILD_DIR)
	@echo "Copying textures..."
	@if [ -d "$(TEXTURE_DIR)" ]; then \
		mkdir -p $(BUILD_DIR)/res/texture/; \
		cp -r $(TEXTURE_DIR)/* $(BUILD_DIR)/res/texture/ 2>/dev/null || true; \
	else \
		echo "Warning: Texture directory does not exist"; \
	fi

# Link executable
$(BUILD_DIR)/$(TARGET): $(APP_OBJ) $(GLAD_OBJ) $(STD_OBJ) $(IMGUI_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Default target
all: $(BUILD_DIR)/$(TARGET) copy_shaders copy_textures

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
	mkdir -p $(BUILD_DIR)/res/shader

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean run init rebuild copy_shader copy_textures