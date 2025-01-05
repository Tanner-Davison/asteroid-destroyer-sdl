#include "SDL_render.h"
#include "SDL_timer.h"
#define SDL_MAIN_HANDLED
#include "Player.hpp"
#include "createwindow.hpp"

void checkCollision(Player &player1, Player &player2) {
  // Get positions and dimensions of both players
  auto [p1X, p1Y] = player1.getPosition();
  int p1Width = player1.getWidth();
  int p1Height = player1.getHeight();

  auto [p2X, p2Y] = player2.getPosition();
  int p2Width = player2.getWidth();
  int p2Height = player2.getHeight();

  // Check for collision using axis-aligned bounding boxes (AABB)
  bool xOverlap = (p1X < p2X + p2Width) && (p1X + p1Width > p2X);
  bool yOverlap = (p1Y < p2Y + p2Height) && (p1Y + p1Height > p2Y);

  if (xOverlap && yOverlap) {
    // Resolve collision
    if (p1X >= SCREEN_WIDTH - p1Width) {
      player2.setPlayerPos(p1X - p2Width - 5, p2Y); // Push left
    } else {
      player2.setPlayerPos(p1X + p1Width + 5, p2Y); // Push right
    }

    // Ensure player2 respects bounds
    player2.handleBounds(player2.getPosition().first,
                         player2.getPosition().second);
  }
}

int main(int argc, char *args[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
    return 1;
  }

  Player player;
  Player player2((player.getPosition().first * 0.5f),
                 (player.getPosition().second));
  Player player3((player2.getPosition().first * 0.82f),
                 (player2.getPosition().second * -0.5f), 20, 20);

  SDL_Event e;
  bool quit = false;

  // Fixed timestep variables
  const float FIXED_TIME_STEP = 1.0f / 120.0f; // Match your 120 FPS
  float accumulator = 0.0f;
  Uint32 lastTime = SDL_GetTicks();

  // Your existing frame rate cap
  const float FPS = 120.0f;
  const float frameDelay = 1000.0f / FPS;

  while (!quit) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;

    // Cap maximum frame time to prevent spiral of death
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

    // Render
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    player.renderPlayer(gRenderer);
    player2.renderPlayer(gRenderer);
    player3.renderPlayer(gRenderer);
    SDL_RenderPresent(gRenderer);

    // check collision
    // checkCollision(player, player2);

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
