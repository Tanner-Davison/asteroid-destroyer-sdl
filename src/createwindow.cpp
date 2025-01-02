// creatingwindow.cpp
#include "createwindow.hpp"
#include <direct.h>
#include <stdio.h>
// Define constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Define global variables
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }
  return success;
}
bool loadMedia() {
  bool success = true;

  gHelloWorld = SDL_LoadBMP("build/Debug/hello_world.bmp");
  if (gHelloWorld == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n",
           "build/Debug/hello_world.bmp", SDL_GetError());
    success = false;
  }

  return success;
}
void close() {
  // Deallocate surface
  SDL_FreeSurface(gHelloWorld);
  gHelloWorld = NULL;

  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}
