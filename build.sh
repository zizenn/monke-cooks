#!/bin/sh

# 1. Configure the build directory (creates it if it doesn't exist)
cmake -B build

# 2. Compile the game
cmake --build build

# 3. If compilation succeeded, run the game
if [ $? -eq 0 ]; then
    ./build/game
else
    echo "Build failed. Game will not launch."
    exit 1
fi

