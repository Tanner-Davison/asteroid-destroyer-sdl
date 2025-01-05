#include "SDL_render.h"
#define SDL_MAIN_HANDLED
#include "Player.hpp"
#include "createwindow.hpp"
int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return 1;
  };

  // Create player instance
  Player player;

  // Game loop variables
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

    // Get keyboard state and update player
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    player.handleInput(keyState[SDL_SCANCODE_UP], keyState[SDL_SCANCODE_DOWN],
                       keyState[SDL_SCANCODE_LEFT],
                       keyState[SDL_SCANCODE_RIGHT]);

    // Render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    // Render player
    player.renderPlayer(gRenderer);

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
