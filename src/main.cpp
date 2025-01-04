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
  float velocityX = 0.0f;
  float velocityY = 0.0f;
  float acceleration = 0.9f;
  float deceleration = 0.8f;
  float maxSpeed = 20.0f;
  int rectX = 100;
  int rectY = 100;
  const int rectWidth = 50;
  const int rectHeight = 50;

  SDL_Event e;
  bool quit = false;
  Uint32 lastTime = SDL_GetTicks();
  const float FPS = 120.0f;
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
      velocityX += (acceleration);
    } else if (keyState[SDL_SCANCODE_LEFT]) {
      velocityX -= acceleration;
    } else {
      velocityX *= deceleration;
    }

    if (keyState[SDL_SCANCODE_UP]) {
      velocityY -= acceleration;
    } else if (keyState[SDL_SCANCODE_DOWN]) {
      velocityY += acceleration;
    } else {
      velocityY *= deceleration;
    }

    // Clamp velocity to max speed
    velocityX = std::max(-maxSpeed, std::min(maxSpeed, velocityX));
    velocityY = std::max(-maxSpeed, std::min(maxSpeed, velocityY));

    // Calculate next position
    float nextX = rectX + velocityX;
    float nextY = rectY + velocityY;

    // Handle bounds and update position
    if (nextX <= 0) {
      rectX = 0;
      velocityX = 0;
    } else if (nextX >= SCREEN_WIDTH - rectWidth) {
      rectX = SCREEN_WIDTH - rectWidth;
      velocityX = 0;
    } else {
      rectX = static_cast<int>(nextX);
    }

    if (nextY <= 0) {
      rectY = 0;
      velocityY = 0;
    } else if (nextY >= SCREEN_HEIGHT - rectHeight) {
      rectY = SCREEN_HEIGHT - rectHeight;
      velocityY = 0;
    } else {
      rectY = static_cast<int>(nextY);
    }

    // Render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

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
