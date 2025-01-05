// creatingwindow.cpp
#include "createwindow.hpp"
#include "SDL_render.h"
#include "SDL_video.h"
#include <direct.h>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
// Define constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;

// Define global variables
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;
SDL_Renderer *gRenderer = NULL;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("Tanners game", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

      // Get window surface
      // gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }
  return success;
}
bool loadMedia(const char *path) {
  bool success = true;

  gHelloWorld = SDL_LoadBMP(path);
  if (gHelloWorld == NULL) {
    printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
    success = false;
  }

  return success;
}

void close() {
  // Deallocate surface
  SDL_FreeSurface(gHelloWorld);
  gHelloWorld = NULL;

  // Destroy Renderer
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;
  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}
