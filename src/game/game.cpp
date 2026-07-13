#include "scenes.hpp"
#include <iostream>

void scene::LoadGame() { std::cout << "Loading Game" << std::endl; }

void scene::UpdateGame() { std::cout << "Updating Game" << std::endl; }

void scene::DrawGame() { std::cout << "Drawing Game" << std::endl; }

void scene::UnloadGame() { std::cout << "Unloading Game" << std::endl; }
