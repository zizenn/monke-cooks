CC ?= gcc
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
SOURCES := $(call rwildcard,src/,*.c)
TARGET := game
RAYLIB_HEADERS := raylib.h raymath.h rlgl.h

ifeq ($(OS),Windows_NT)
TARGET_OS := windows
else
UNAME_S := $(shell uname -s)
ifneq (,$(findstring MINGW,$(UNAME_S)))
TARGET_OS := windows
else ifneq (,$(findstring MSYS,$(UNAME_S)))
TARGET_OS := windows
else ifeq ($(UNAME_S),Darwin)
TARGET_OS := macos
else ifeq ($(UNAME_S),Linux)
TARGET_OS := linux
else
$(error Unsupported platform: $(UNAME_S))
endif
endif

ifeq ($(TARGET_OS),windows)
EXE := $(TARGET).exe
RAYLIB_LIB := lib/libraylib.a
RAYLIB_INCLUDE := lib
PLATFORM_LIBS := -lgdi32 -lwinmm -lopengl32
RUN_CMD := ./$(EXE)
ifeq ($(wildcard $(RAYLIB_LIB)),)
$(error Missing $(RAYLIB_LIB). Add Windows raylib binaries under lib/)
endif
else ifeq ($(TARGET_OS),linux)
EXE := $(TARGET)
RAYLIB_LIB := lib/linux/libraylib.a
RAYLIB_INCLUDE := lib
PLATFORM_LIBS := -lm -ldl -lpthread -lGL -lrt -lX11
RUN_CMD := ./$(EXE)
ifeq ($(wildcard $(RAYLIB_LIB)),)
$(error Missing $(RAYLIB_LIB). Add Linux raylib binaries under lib/linux/)
endif
else ifeq ($(TARGET_OS),macos)
EXE := $(TARGET)
RAYLIB_LIB := lib/macos/libraylib.a
RAYLIB_INCLUDE := lib
PLATFORM_LIBS := -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
RUN_CMD := ./$(EXE)
ifeq ($(wildcard $(RAYLIB_LIB)),)
$(error Missing $(RAYLIB_LIB). Add macOS raylib binaries under lib/macos/)
endif
endif

CPPFLAGS := -Iinclude -I$(RAYLIB_INCLUDE)

.PHONY: default all run clean

default: all

all: $(RAYLIB_LIB)
	$(CC) -o $(EXE) $(SOURCES) $(CPPFLAGS) $(RAYLIB_LIB) $(PLATFORM_LIBS)

ifeq ($(TARGET_OS),windows)
$(RAYLIB_LIB):
	@echo "Using local Windows raylib at $(RAYLIB_LIB)"
else ifeq ($(TARGET_OS),linux)
$(RAYLIB_LIB):
	@echo "Using local Linux raylib at $(RAYLIB_LIB)"
else ifeq ($(TARGET_OS),macos)
$(RAYLIB_LIB):
	@echo "Using local macOS raylib at $(RAYLIB_LIB)"
endif

run: all
	$(RUN_CMD)

clean:
ifeq ($(TARGET_OS),windows)
	@cmd /c "if exist game.exe del /f /q game.exe & if exist game del /f /q game"
else
	@rm -f game game.exe
endif
