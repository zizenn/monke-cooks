CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude/ -Iinclude/external/
TARGET   := game
BUILD    := build

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))
SRCS     := $(call rwildcard,src,*.cpp)
OBJS     := $(SRCS:src/%.cpp=$(BUILD)/%.o)

ifeq ($(OS),Windows_NT)
LDFLAGS := -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS := -Llib/linux -l:libraylib.a -lX11 -lGL -lm -lpthread -ldl -lrt
endif
ifeq ($(UNAME_S),Darwin)
LDFLAGS := -Llib/macos -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
endif

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) $(TARGET)
