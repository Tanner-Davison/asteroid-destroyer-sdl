// creatingwindow.cpp
#include "createwindow.hpp"
#include "SDL_render.h"
#include "SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <direct.h>
#include <stdio.h>
// Define constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;

// Define global variables
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;
SDL_Renderer *gRenderer = NULL;
std::random_device GameRNG::rd;
std::mt19937 GameRNG::gen(GameRNG::rd());

bool init() {
  // Initialization flag
  bool success = true;
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  }

  // Initialize SDL_image
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not initialize! SDL_image Error: %s\n",
           IMG_GetError());
    success = false;
  }

  if (success) {
    gWindow = SDL_CreateWindow("Tanners game", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      }
    }
  }

  return success;
}
bool loadMedia(const char *path) {
  bool success = true;

  // Load image using SDL_image instead of SDL_LoadBMP
  SDL_Surface *loadedSurface = IMG_Load(path);
  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL_image Error: %s\n", path,
           IMG_GetError());
    success = false;
  } else {
    // Convert surface to texture
    SDL_Texture *newTexture =
        SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path,
             SDL_GetError());
      success = false;
    }
    // Free the loaded surface as we don't need it anymore
    SDL_FreeSurface(loadedSurface);
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
  IMG_Quit();
  SDL_Quit();
}
