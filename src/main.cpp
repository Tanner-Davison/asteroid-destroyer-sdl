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

  int imgFlags = IMG_INIT_PNG;
  // Initializing SDL_Image w/ png
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not initialize! SDL_image Error: %s\n",
           IMG_GetError());
    return 1;
  }
  float centerX = SCREEN_WIDTH / 2.0f;
  float bottomY = SCREEN_HEIGHT - 100.0f;
  const int PLAYER_SPACING = 50;

  Player player(centerX - static_cast<float>(PLAYER_SPACING) / 2, bottomY);
  Player player2(centerX + static_cast<float>(PLAYER_SPACING) / 2, bottomY);

  Player player3(centerX, bottomY - static_cast<float>(PLAYER_SPACING));

  std::vector<Player *> players{&player, &player2, &player3};
  for (auto *player : players) {
    if (!player->loadTexture("src/spaceship.png", gRenderer)) {
      printf("Failed to load player3 texture\n");
      return 1;
    }
  }
  std::vector<Asteroid> asteroids;
  for (int i = 0; i < 3; i++) {
    asteroids.emplace_back();
  }

  SDL_Event e;
  bool quit = false;

  // timestep var
  const float FIXED_TIME_STEP = 1.0f / 120.0f; //(8.33ms)
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

      for (auto &asteroid : asteroids) {
        SDL_Rect asteroidRect = {asteroid.getRectX(), asteroid.getRectY(),
                                 asteroid.getRectWidth(),
                                 asteroid.getRectHeight()};

        // Check bullet collisions for each player
        for (Player *playa : players) {
          if (playa->getWeapon().checkBulletCollision(asteroidRect)) {
            printf("Bullet hit asteroid!\n"); // Debug print
            asteroid.destroy();
            break; // Stop checking other players once asteroid is hit
          }
        }
      }
      // Update asteroids and check bullet collisions
      std::vector<size_t> asteroidsToRemove;

      // Mark asteroids for removal
      for (size_t i = 0; i < asteroids.size(); i++) {
        if (asteroids[i].isDestroyed()) {
          asteroidsToRemove.push_back(i);
        }
      }

      // Remove from back to front to maintain valid indices
      for (auto it = asteroidsToRemove.rbegin(); it != asteroidsToRemove.rend();
           ++it) {
        if (*it < asteroids.size()) { // Safety check
          asteroids.erase(asteroids.begin() + *it);
        }
      }
      accumulator -= FIXED_TIME_STEP;
    }

    for (auto &asteroid : asteroids) {
      asteroid.update();
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    for (Player *playa : players) {
      playa->renderPlayer(gRenderer);
    }
    for (auto &asteroid : asteroids) {
      asteroid.renderAsteroid(gRenderer);
    }

    SDL_RenderPresent(gRenderer); // FINAL FRAME
                                  // collison detection_______
    std::vector<Player *> playersToRemove;
    for (Player *player : players) { // Note: use -> with pointers
      SDL_Rect playerRect = {player->getPosition().first, // Use -> instead of .
                             player->getPosition().second, player->getWidth(),
                             player->getHeight()};

      for (const auto &asteroid : asteroids) {
        SDL_Rect asteroidRect = {asteroid.getRectX(), asteroid.getRectY(),
                                 asteroid.getRectWidth(),
                                 asteroid.getRectHeight()};

        if (player->checkCollision(playerRect,
                                   asteroidRect)) { // Use -> instead of .
          if (players.size() == 1) {
            quit = true;
          }
          playersToRemove.push_back(player);
          break;
        }
      }
    }
    players.erase(std::remove_if(players.begin(), players.end(),
                                 [&playersToRemove](Player *player) {
                                   return std::find(playersToRemove.begin(),
                                                    playersToRemove.end(),
                                                    player) !=
                                          playersToRemove.end();
                                 }),
                  players.end());
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
