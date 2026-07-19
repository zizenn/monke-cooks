cmake_minimum_required(VERSION 3.25)
project(RaylibGame LANGUAGES CXX)

# Force C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Neovim LSP support (Generates compile_commands.json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global Compile Flags
add_compile_options(-Wall -Wextra -O2)

# Include Directories
include_directories(include include/external)

# Find all .cpp files recursively inside src/
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Define the executable target
add_executable(game ${SOURCES})

# --- Platform Specific Linking ---

if(WIN32)
    # Windows linking
    target_link_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/windows)
    target_link_libraries(game PRIVATE raylib opengl32 gdi32 winmm)
    
elseif(APPLE)
    # macOS linking
    target_link_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/macos)
    target_link_libraries(game PRIVATE raylib "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    
elseif(LINUX)
    # Linux linking (Preserving your local static setup)
    target_link_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/linux)
    target_link_libraries(game PRIVATE :libraylib.a wayland-client wayland-cursor wayland-egl xkbcommon X11 GL m pthread dl rt)
endif()

# --- Neovim Tooling ---

# Automagically link compile_commands.json to root for clangd
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(lsp_symlink ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${CMAKE_BINARY_DIR}/compile_commands.json" 
            "${CMAKE_SOURCE_DIR}/compile_commands.json"
        COMMENT "Copying compile_commands.json for Neovim/clangd..."
    )
endif()
