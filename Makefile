CC      := gcc
TARGET  := game
SOURCES := $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJECTS := $(SOURCES:.c=.o)

# Platform Detection & Setup
ifeq ($(OS),Windows_NT)
    EXE           := $(TARGET).exe
    RAYLIB_LIB    := lib/libraylib.a
    PLATFORM_LIBS := -lgdi32 -lwinmm -lopengl32
    CFLAGS        := -DNOGDI -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS
    CLEAN_CMD     := cmd /c "if exist $(EXE) del /f /q $(EXE)"
else
    EXE           := $(TARGET)
    CLEAN_CMD     := rm -f $(EXE) $(OBJECTS)
    UNAME_S       := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin) # macOS
        RAYLIB_LIB    := lib/macos/libraylib.a
        PLATFORM_LIBS := -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    else # Linux assumed
        RAYLIB_LIB    := lib/linux/libraylib.a
        PLATFORM_LIBS := -lm -ldl -lpthread -lGL -lrt -lX11
    endif
endif

CPPFLAGS := -Iinclude -Ilib
CFLAGS   += -std=c11 -Wall -Wextra -Wpedantic

# 2. Build Targets
.PHONY: all run clean lsp

# Making 'all' automatically trigger the 'lsp' definition file update
all: lsp $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $^ $(RAYLIB_LIB) $(PLATFORM_LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Force compiledb to evaluate the clean tool command 'make' directly
lsp:
	@echo "Updating compile_commands.json..."
	-@compiledb -n make --no-print-directory $(EXE)

run: all
	@LIBGL_ALWAYS_SOFTWARE=0 ./$(EXE)

clean:
	@$(CLEAN_CMD)
