#include "SDL_render.h"
#include "SDL_timer.h"
#define SDL_MAIN_HANDLED
#include "Player.hpp"
#include "asteroid.hpp"
#include "createwindow.hpp"
#include <vector>
int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return 1;
  }

  Player player;
  Player player2((player.getPosition().first * 0.5f),
                 (player.getPosition().second));
  Player player3((player2.getPosition().first * 0.74f),
                 (player2.getPosition().second - 9.0f));
  std::vector<Asteroid> asteroids{3};
  SDL_Event e;
  bool quit = false;

  // timestep var
  const float FIXED_TIME_STEP = 1.0f / 120.0f; // Match your 120 FPS
  float accumulator = 0.0f;
  Uint32 lastTime = SDL_GetTicks();

  // frame rate cap
  const float FPS = 120.0f;
  const float frameDelay = 1000.0f / FPS;

  while (!quit) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;

    // Cap max frame time to prevent spiral of death
    if (deltaTime > 0.25f) {
      deltaTime = 0.25f;
    }

    accumulator += deltaTime;

    // e handle
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    // timestep update
    while (accumulator >= FIXED_TIME_STEP) {
      player.handlePlayerInput(keyState);
      player2.handlePlayerInput(keyState);
      player3.handlePlayerInput(keyState);

      accumulator -= FIXED_TIME_STEP;
    }

    for (auto &asteroid : asteroids) {
      asteroid.update();
    }
    // Render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    player.renderPlayer(gRenderer);
    player2.renderPlayer(gRenderer);
    player3.renderPlayer(gRenderer);
    for (auto &asteroid : asteroids) {
      asteroid.renderAsteroid(gRenderer);
    }

    SDL_RenderPresent(gRenderer); // FINAL FRAME

    // check collision
    // checkCollision(player, player2);

    SDL_Rect playerRect = {player.getPosition().first,
                           player.getPosition().second, player.getWidth(),
                           player.getHeight()};

    for (const auto &asteroid : asteroids) {
      SDL_Rect asteroidRect = {asteroid.getRectX(), asteroid.getRectY(),
                               asteroid.getRectWidth(),
                               asteroid.getRectHeight()};

      if (player.checkCollision(playerRect, asteroidRect)) {
        // Handle collision
        // IDEAS player loses life, game over, etc.
        quit = true;
      }
    }
    // Frame rate capping
    Uint32 endTime = SDL_GetTicks();
    float elapsedMS = endTime - currentTime;
    if (frameDelay > elapsedMS) {
      SDL_Delay(frameDelay - elapsedMS);
    }

    lastTime = currentTime;
  }

  close();
  return 0;
}
