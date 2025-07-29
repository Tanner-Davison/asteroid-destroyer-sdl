// creatingwindow.hpp
// This is where my window and its resources are being
// handled. No classes or Structs just good old declaration
//
#pragma once
#include <SDL2/SDL.h>
#include <random>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
//     Function Declarations
bool init();                      // Starts Up SDL and creates window
bool loadMedia(const char* path); // Loads Media
void close();                     // Frees Media and Shuts Down SDL
// Global variables declared as extern;
extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gHelloWorld;
extern SDL_Renderer* gRenderer;
// static RNG members (random generated numbers)
namespace GameRNG {
extern std::random_device rd;
extern std::mt19937 gen;
} // namespace GameRNG
