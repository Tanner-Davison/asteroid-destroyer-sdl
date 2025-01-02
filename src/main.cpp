#define SDL_MAIN_HANDLED
#include "createwindow.hpp"

int main(int argc, char *args[]) {
  // Start up SDL and create window
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    // Load media
    if (!loadMedia()) {
      printf("Failed to load media!\n");
    } else {
      // Apply the image
      SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
      // Update the surface
      SDL_UpdateWindowSurface(gWindow);
      // Wait two seconds
      SDL_Delay(2000);
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}
