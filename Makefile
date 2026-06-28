# ==============================================================================
# Cross-Platform C++23 Makefile with Build Directory & Bear Support
# ==============================================================================

# Compiler & Flags
CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude/ -Iinclude/external/

# Output Binary Name
TARGET := game

# Target Directories
SRC_DIR   := src
BUILD_DIR := build

# 1. Recursive Wildcard Function to find all .cpp files in src/ and subdirectories
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))

# Locate all source files dynamically
SRCS := $(call rwildcard,$(SRC_DIR),*.cpp)
# Map source files to object files in the build directory
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# 2. OS Auto-Detection & Library Mapping
ifeq ($(OS),Windows_NT)
    # Windows Host
    OS_NAME := windows
    LDFLAGS := -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm
    EXE     := .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_NAME := linux
        # Optimized for your Hyprland/Wayland static build
        LDFLAGS := -Llib/linux -l:libraylib.a -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lX11 -lGL -lm -lpthread -ldl -lrt
        EXE     :=
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_NAME := macos
        LDFLAGS := -Llib/macos -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        EXE     :=
    endif
endif

# Final output binary path
OUTPUT := $(TARGET)$(EXE)

.PHONY: all clean bear directories

# Default Target
all: directories $(OUTPUT)

# 3. Create Build Tree Structure Automatically
directories:
	@mkdir -p $(BUILD_DIR)
	@$(foreach dir,$(sort $(dir $(OBJS))),mkdir -p $(dir);)

# Link Object Files into Final Binary
$(OUTPUT): $(OBJS)
	@echo "Linking binary for $(OS_NAME): $(OUTPUT)"
	$(CXX) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

# Compile C++ Source Files into Objects inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Bear Target for LSP (Neovim/Clangd)
bear: clean
	@echo "Generating compile_commands.json via Bear..."
	bear -- make all

# Clean Target to reset workspace
clean:
	@echo "Cleaning up build artifacts..."
	rm -rf $(BUILD_DIR) $(OUTPUT) compile_commands.json
