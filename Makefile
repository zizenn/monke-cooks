CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude -Iinclude/external
TARGET   := game

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))
SRCS     := $(call rwildcard,src,*.cpp)
OBJS     := $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
LDFLAGS := -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS := -Llib/linux -l:libraylib.a -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lX11 -lGL -lm -lpthread -ldl -lrt
endif
ifeq ($(UNAME_S),Darwin)
LDFLAGS := -Llib/macos -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
endif

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
