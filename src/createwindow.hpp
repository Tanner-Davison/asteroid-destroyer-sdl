// creatingwindow.hpp
#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern SDL_Window *gWindow;

//     Function Declarations
bool init();      // Starts Up SDL and creates window
bool loadMedia(); // Loads Media
void close();     // Frees Media and Shuts Down SDL

// Global variables declared as extern;
extern SDL_Window *gWindow;
extern SDL_Surface *gScreenSurface;
extern SDL_Surface *gHelloWorld;
