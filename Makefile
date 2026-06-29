# ==============================================================================
# Optimized Cross-Platform C++23 Makefile with Speed Patches
# ==============================================================================

# Compiler & Flags
CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude/ -Iinclude/external/

# Output Binary Name
TARGET := game

# Target Directories
SRC_DIR   := src
BUILD_DIR := build

# 1. Recursive Wildcard Function
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))

# SPEED FIX: Use immediate assignment (:=) to execute the scan EXACTLY ONCE
SRCS := $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Extract unique directories into a single space-separated list
UNIQUE_DIRS := $(sort $(dir $(OBJS)))

# 3. OS Auto-Detection & Library Mapping
ifeq ($(OS),Windows_NT)
    OS_NAME := windows
    LDFLAGS := -L$(CURDIR)/lib/windows -lraylib -lopengl32 -lgdi32 -lwinmm
    EXE     := .exe

    SHELL        := pwsh.exe
    .SHELLFLAGS  := -NoProfile -Command

    # SPEED FIX: Group directory paths to minimize shell invocation overhead
    MKDIR = if (!(Test-Path '$(1)')) { New-Item -ItemType Directory -Path '$(1)' -Force | Out-Null }
    RM    = if (Test-Path '$(1)') { Remove-Item -Path '$(1)' -Force }
    RMDIR = if (Test-Path '$(1)') { Remove-Item -Path '$(1)' -Recurse -Force }
else
    UNAME_S := $(shell uname -s)
    MKDIR   = mkdir -p $(1)
    RM      = rm -f $(1)
    RMDIR   = rm -rf $(1)

    ifeq ($(UNAME_S),Linux)
        OS_NAME := linux
        LDFLAGS := -Llib/linux -l:libraylib.a -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lX11 -lGL -lm -lpthread -ldl -lrt
        EXE     :=
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_NAME := macos
        LDFLAGS := -Llib/macos -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        EXE     :=
    endif
endif

OUTPUT := $(TARGET)$(EXE)

.PHONY: all clean compiledb directories

# Default Target
all: directories $(OUTPUT)

# SPEED FIX: Passes the whole array to PowerShell instantly instead of a loop
directories:
	@$(call MKDIR,$(BUILD_DIR))
	@$(if $(UNIQUE_DIRS),$(call MKDIR,$(UNIQUE_DIRS)))

# Link Object Files into Final Binary
$(OUTPUT): $(OBJS)
	@echo "Linking binary for $(OS_NAME): $(OUTPUT)"
	$(CXX) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

# Compile C++ Source Files into Objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# SPEED FIX: Run compiledb with -n (dry-run mode) so it generates the JSON
# instantly without actually executing the slow compiler link steps twice!
compiledb: clean
	@echo "Generating compile_commands.json instantly..."
	compiledb make all -n

# Clean Target to reset workspace
clean:
	@echo "Cleaning up build artifacts..."
	@$(call RMDIR,$(BUILD_DIR))
	@$(call RM,$(OUTPUT))
	@$(call RM,compile_commands.json)
