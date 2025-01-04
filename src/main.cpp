#include "SDL_render.h"
#include "SDL_surface.h"
#define SDL_MAIN_HANDLED
#include "createwindow.hpp"

int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return 1;
  }

  // Rectangle position variables
  int rectX = 100; // Starting X position
  int rectY = 100; // Y position
  const int rectWidth = 50;
  const int rectHeight = 50;
  const int SPEED = 5; // Pixels per frame

  SDL_Event e;
  bool quit = false;
  Uint32 lastTime = SDL_GetTicks();
  const float FPS = 60.0f;
  const float frameDelay = 1000.0f / FPS;

  while (!quit) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;

    // Handle events
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    // Handle keyboard state
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_RIGHT]) {
      rectX += SPEED; // Move right
    }
    if (keyState[SDL_SCANCODE_LEFT]) {
      rectX -= SPEED;
    }
    if (keyState[SDL_SCANCODE_UP]) {
      rectY -= SPEED;
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
      rectY += SPEED;
    }

    // Render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    // Draw rectangle at updated position
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_Rect rect = {rectX, rectY, rectWidth, rectHeight};
    SDL_RenderFillRect(gRenderer, &rect);

    SDL_RenderPresent(gRenderer);

    // Cap frame rate
    int frameTime = SDL_GetTicks() - currentTime;
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }

    lastTime = currentTime;
  }

  close();
  return 0;
}
