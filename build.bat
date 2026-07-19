@echo off

:: 1. Configure the build using MinGW Makefiles or Ninja (whichever Scoop installed)
:: We add -DCMAKE_BUILD_TYPE=Debug to keep it consistent
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
if %errorlevel% neq 0 (
    echo CMake configuration failed.
    pause
    exit /b %errorlevel%
)

:: 2. Compile the game
cmake --build build
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b %errorlevel%
)

:: 3. Run the game (Direct path, no extra folders)
".\build\game.exe"

