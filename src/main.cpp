#define SDL_MAIN_HANDLED
// Standard library headers
#include <algorithm>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

// SDL headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

// Game component headers
#include "./Player.hpp"
#include "asteroid.hpp"
#include "createwindow.hpp"
#include "score.hpp"

std::vector<std::unique_ptr<Player>> createPlayers(int count) {
    int centerX = static_cast<int>(SCREEN_WIDTH / 2);
    int bottomY = static_cast<int>(SCREEN_HEIGHT - 100);
    const int PLAYER_SPACING = 70;
    const int VERTICAL_OFFSET = 50;
    std::vector<std::unique_ptr<Player>> players;
    if (count <= 0) {
        return players;
    }
    if (count == 1) {
        // Center on Single
        players.push_back(std::make_unique<Player>(centerX, bottomY));
    } else {
        // V formation logic
        int middleIndex = count / 2;
        for (int i = 0; i < count; ++i) {
            int offsetX = (i - middleIndex) * PLAYER_SPACING;
            int offsetY = std::abs(i - middleIndex) * VERTICAL_OFFSET;
            players.push_back(
                std::make_unique<Player>(centerX + offsetX, bottomY - offsetY));
        }
    }

    return players;
}

// Function to display level text
void displayLevelText(SDL_Renderer* renderer, int level) {
    // Create a texture for the level text
    SDL_Surface* textSurface = TTF_RenderText_Solid(
        font, ("LEVEL " + std::to_string(level)).c_str(), {255, 255, 255, 255});
    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    // Get text dimensions
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    // Position text in center of screen
    SDL_Rect textRect = {(SCREEN_WIDTH - textWidth) / 2,
                         (SCREEN_HEIGHT - textHeight) / 2, textWidth,
                         textHeight};

    // Display the text for a few frames
    const int DISPLAY_FRAMES = 120;  // 2 seconds at 60 FPS
    for (int i = 0; i < DISPLAY_FRAMES; i++) {
        SDL_SetRenderDrawColor(renderer, 10, 14, 33, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);  // Approximately 60 FPS
    }

    SDL_DestroyTexture(textTexture);
}

// Function to spawn asteroids for a level
std::vector<Asteroid> spawnAsteroids(
    int count, const std::vector<std::unique_ptr<Player>>& players,
    SDL_Renderer* renderer) {
    std::vector<Asteroid> asteroids;
    for (int i = 0; i < count; i++) {
        asteroids.emplace_back(players);
        if (!asteroids.back().loadTexture("src/asteroid.png", renderer)) {
            printf("Failed to load asteroid texture for asteroid %d\n", i);
            // Handle error - maybe return empty vector or throw exception
        }
    }
    return asteroids;
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n",
               IMG_GetError());
        return 1;
    }

    // Initialize TTF
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
               TTF_GetError());
        return 1;
    }

    // Load font
    font = TTF_OpenFont("../assets/fonts/FiraCode-Regular.ttf", 48);
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    int deathCount = 3;
    Score scoreDisplay;
    auto players = createPlayers(3);

    for (auto& player : players) {
        if (!player->loadTexture("src/spaceship.png", gRenderer)) {
            printf("Failed to load player texture\n");
            return 1;
        }
    }

    // Game state variables
    int currentLevel = 1;
    int asteroidsForLevel = 19;  // Level 1 starts with 19 asteroids

    // ASTEROIDS
    std::vector<Asteroid> asteroids =
        spawnAsteroids(asteroidsForLevel, players, gRenderer);

    SDL_Event EVENT;
    bool quit = false;

    // MY TIMESTEP VARS
    const float FIXED_TIME_STEP = 1.0F / 120.0F;  // (8.33ms)
    float accumulator = 0.0F;
    Uint32 lastTime = SDL_GetTicks();

    // FRAME RATE CAP
    const float FPS = 120.0F;
    const float frameDelay = 1000.0F / FPS;

    while (!quit) {
        // FRAME TIMING
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.25f);  // Cap max frame time
        accumulator += deltaTime;

        // Handle SDL events
        while (SDL_PollEvent(&EVENT)) {
            if (EVENT.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* keyState = SDL_GetKeyboardState(nullptr);

        // TIMESTEP UPDATE LOOP
        while (accumulator >= FIXED_TIME_STEP) {
            // Update player positions based on input
            for (auto& player : players) {
                player->handlePlayerInputAndPosition(keyState);
            }

            // Update asteroids and handle bullet collisions
            std::vector<size_t> asteroidsToRemove;
            for (auto& asteroid : asteroids) {
                SDL_Rect asteroidRect = {
                    asteroid.getRectX(), asteroid.getRectY(),
                    asteroid.getRectWidth(), asteroid.getRectHeight()};

                bool asteroidHit = false;
                // BULLET <----> ASTEROID DETECTION
                for (auto& player : players) {
                    auto bulletIndex =
                        player->getWeapon().checkBulletCollision(asteroidRect);
                    if (bulletIndex.has_value()) {
                        printf("Bullet %zu hit asteroid at position: %d, %d\n",
                               bulletIndex.value(), asteroid.getRectX(),
                               asteroid.getRectY());
                        player->getWeapon().destroyBullet(bulletIndex.value());
                        asteroid.destroy();
                        scoreDisplay.setScore(100);
                        asteroidHit = true;
                        break;
                    }
                }
                if (asteroidHit)
                    continue;
            }

            // MARK DESTROYED ASTEROIDS
            for (size_t i = 0; i < asteroids.size(); i++) {
                if (asteroids[i].isDestroyed()) {
                    asteroidsToRemove.push_back(i);
                }
            }

            // REMOVE DESTROYED ASTROIDS IN REVERSE
            for (unsigned long& it :
                 std::ranges::reverse_view(asteroidsToRemove)) {
                if (it < asteroids.size()) {
                    asteroids.erase(asteroids.begin() + it);
                }
            }

            // Check if all asteroids are destroyed - advance to next level
            if (asteroids.empty()) {
                currentLevel++;

                // For level 2, spawn 200 asteroids
                if (currentLevel == 2) {
                    asteroidsForLevel = 80;

                    // Display level text
                    displayLevelText(gRenderer, currentLevel);

                    // Spawn new asteroids for level 2
                    asteroids =
                        spawnAsteroids(asteroidsForLevel, players, gRenderer);
                }
                // You could add more levels here with different asteroid counts
            }

            // PLAYER <--> ASTEROID DETECTION
            std::vector<Player*> playersToRemove;
            for (const auto& player : players) {
                SDL_Rect playerRect = {player->getPosition().first,
                                       player->getPosition().second,
                                       player->getWidth(), player->getHeight()};

                for (const auto& asteroid : asteroids) {
                    SDL_Rect asteroidRect = {
                        asteroid.getRectX(), asteroid.getRectY(),
                        asteroid.getRectWidth(), asteroid.getRectHeight()};

                    if (player->checkCollision(playerRect, asteroidRect)) {
                        // TODO: Refactor single player respawn logic into
                        // separate function
                        // TODO: Add proper game over state handling
                        // TODO: Consider adding invulnerability frames after
                        // respawnj
                        if (players.size() == 1) {
                            players.clear();
                            players = std::move(createPlayers(++deathCount));
                            for (auto& player : players) {
                                if (!player->loadTexture("src/spaceship.png",
                                                         gRenderer)) {
                                    printf("Failed to load player texture\n");
                                    return 1;
                                }
                            }
                        }
                        playersToRemove.push_back(player.get());
                        scoreDisplay.setScore(-100);
                        break;
                    }
                }
            }

            // REMOVE DESTROYED PLAYERS
            players.erase(
                std::remove_if(
                    players.begin(), players.end(),
                    [&playersToRemove](const std::unique_ptr<Player>& player) {
                        return std::find(playersToRemove.begin(),
                                         playersToRemove.end(),
                                         player.get()) != playersToRemove.end();
                    }),
                players.end());

            // UPDATE ASTEROID POSITIONS
            for (auto& asteroid : asteroids) {
                asteroid.update();
            }

            accumulator -= FIXED_TIME_STEP;
        }

        // RENDERING (HAPPENS EVERY FRAME)
        SDL_SetRenderDrawColor(gRenderer, 10, 14, 33, 255);
        SDL_RenderClear(gRenderer);

        // RENDER GAME OBJECTS
        scoreDisplay.renderScore(gRenderer);
        for (auto& player : players) {
            player->renderPlayer(gRenderer);
        }
        for (auto& asteroid : asteroids) {
            asteroid.renderAsteroid(gRenderer);
        }

        // Render level indicator
        SDL_Surface* levelSurface = TTF_RenderText_Solid(
            font, ("Level: " + std::to_string(currentLevel)).c_str(),
            {255, 255, 255, 255});
        SDL_Texture* levelTexture =
            SDL_CreateTextureFromSurface(gRenderer, levelSurface);
        SDL_Rect levelRect = {10, 10, levelSurface->w, levelSurface->h};
        SDL_RenderCopy(gRenderer, levelTexture, NULL, &levelRect);
        SDL_FreeSurface(levelSurface);
        SDL_DestroyTexture(levelTexture);

        SDL_RenderPresent(gRenderer);

        // FRAME RATE CAPPING
        Uint32 endTime = SDL_GetTicks();
        float elapsedMS = endTime - currentTime;
        if (frameDelay > elapsedMS) {
            SDL_Delay(frameDelay - elapsedMS);
        }
    }

    // Clean up
    TTF_CloseFont(font);
    TTF_Quit();
    close();
    return 0;
}
// Test comment
