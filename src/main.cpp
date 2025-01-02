#include "SDL_surface.h"
#define SDL_MAIN_HANDLED
#include "createwindow.hpp"

int main(int argc, char *args[]) {
  // Start up SDL and create window
  std::string imgPath = "build/Debug/hello_world.bmp";
  std::string imgPath2 = "build/Debug/hello_worl.bmp";
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    // Load media
    if (!loadMedia(imgPath.c_str())) {
      printf("Failed to load media!\n");
    } else {
      // Apply the image
      SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
      // Update the surface
      SDL_UpdateWindowSurface(gWindow);
      // Wait two seconds
    }
  }

  SDL_Delay(2000);
  if (!loadMedia(imgPath2.c_str())) {
    printf("Failed to load media!\n");
  } else {
    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
    // update surface
    SDL_UpdateWindowSurface(gWindow);
  }
  // Hack to get window to stay up
  SDL_Event e;
  bool quit = false;
  while (quit == false) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}
