cmake_minimum_required(VERSION 3.20)

project(monke_cooks
  VERSION 0.0.1
  LANGUAGES C CXX
  DESCRIPTION "A cooking game"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# raylib - use FetchContent with DRM platform on Linux (no X11/Wayland needed)
FetchContent_Declare(raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 6.0
  GIT_SHALLOW 1
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# On Linux, force GLFW Wayland backend (no X11 needed)
if(UNIX AND NOT APPLE)
  set(GLFW_BUILD_WAYLAND ON CACHE BOOL "" FORCE)
  set(GLFW_BUILD_X11 OFF CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(raylib)

# raylib-cpp (header-only C++ bindings)
FetchContent_Declare(raylib_cpp
  GIT_REPOSITORY https://github.com/robloach/raylib-cpp.git
  GIT_TAG v6.0.2
  GIT_SHALLOW 1
)
set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib_cpp)

if(WIN32)
  set(PLATFORM_LIBS winmm)
elseif(APPLE)
  set(PLATFORM_LIBS "-framework Cocoa -framework IOKit")
else()
  find_library(WAYLAND_CLIENT wayland-client)
  find_library(WAYLAND_CURSOR wayland-cursor)
  find_library(WAYLAND_EGL wayland-egl)
  find_library(XKBCOMMON xkbcommon)
  find_library(LIBDRM drm)
  set(PLATFORM_LIBS ${WAYLAND_CLIENT} ${WAYLAND_CURSOR} ${WAYLAND_EGL} ${XKBCOMMON} ${LIBDRM} pthread dl rt m)
endif()

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)
if(NOT SOURCES)
  set(SOURCES src/main.cpp)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Get raylib-cpp include dir and add as SYSTEM to suppress -Wshadow from its headers
get_target_property(_rci raylib_cpp INTERFACE_INCLUDE_DIRECTORIES)
if(_rci AND NOT _rci MATCHES "-NOTFOUND$")
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${_rci})
endif()

# Clear raylib_cpp's include dirs so they don't get added again as -I
set_target_properties(raylib_cpp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "")

target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp ${PLATFORM_LIBS})

if(UNIX AND NOT APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${CMAKE_INSTALL_PREFIX}/lib;/nix/store"
    INSTALL_RPATH "\$ORIGIN/../lib"
  )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
