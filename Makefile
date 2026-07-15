CXX      := clang++
CXXFLAGS := -std=c++23 -Wall -Wextra -O2 -Iinclude -Iinclude/external
TARGET   := game

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))
SRCS     := $(call rwildcard,src,*.cpp)
OBJS     := $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
CXXFLAGS += -target x86_64-pc-windows-gnu
LDFLAGS  := -Llib/windows -lraylib -lopengl32 -lgdi32 -lwinmm -target x86_64-pc-windows-gnu
RM       := rm -f
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS := -Llib/linux -l:libraylib.a -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon -lX11 -lGL -lm -lpthread -ldl -lrt
else ifeq ($(UNAME_S),Darwin)
LDFLAGS := -Llib/macos -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
endif

.PHONY: all clean run

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$<

clean:
	$(RM) $(OBJS) $(TARGET)
