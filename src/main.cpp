/*
 * Copyright © 2023 Tanner Davison
 * SDL3_GAME: Asteroid Destroyer
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <print>
#include <string>
#include <vector>

#include "./Player.hpp"
#include "./asteroid.hpp"
#include "./createwindow.hpp"
#include "./score.hpp"

void displayLevelText(SDL_Renderer* renderer, int level) {
    std::string text = "LEVEL " + std::to_string(level);
    SDL_Surface* textSurface =
        TTF_RenderText_Solid(font, text.c_str(), 0, {255, 255, 255, 255});
    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_DestroySurface(textSurface);

    SDL_FRect textRect = {static_cast<float>((SCREEN_WIDTH - textWidth) / 2),
                          static_cast<float>((SCREEN_HEIGHT - textHeight) / 2),
                          static_cast<float>(textWidth),
                          static_cast<float>(textHeight)};

    const int DISPLAY_FRAMES = 120;
    for (int i = 0; i < DISPLAY_FRAMES; i++) {
        SDL_SetRenderDrawColor(renderer, 10, 14, 33, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(textTexture);
}

std::vector<Asteroid> spawnAsteroids(
    int count, const std::vector<std::unique_ptr<Player>>& players,
    SDL_Renderer* renderer) {
    std::vector<Asteroid> asteroids;
    for (int i = 0; i < count; i++) {
        asteroids.emplace_back(players);
        if (!asteroids.back().loadTexture("asteroid.png", renderer)) {
            std::println("Failed to load asteroid texture for asteroid {}", i);
        }
    }
    return asteroids;
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::println("Failed to initialize!");
        return 1;
    }

    font = TTF_OpenFont("FiraCode-Regular.ttf", 48);
    if (!font) {
        std::println("Failed to load font! Error: {}", SDL_GetError());
        return 1;
    }

    int deathCount = 3;
    Score scoreDisplay;
    auto players = Player::createPlayers(3);

    for (auto& player : players) {
        if (!player->loadTexture("spaceship.png", gRenderer)) {
            std::println("Failed to load player texture.");
            return 1;
        }
    }

    int currentLevel = 1;
    int asteroidsForLevel = 19;
    std::vector<Asteroid> asteroids =
        spawnAsteroids(asteroidsForLevel, players, gRenderer);

    SDL_Event EVENT;
    bool quit = false;

    const float FIXED_TIME_STEP = 1.0f / 120.0f;
    float accumulator = 0.0f;
    Uint64 lastTime = SDL_GetTicks();

    const float FPS = 120.0f;
    const float frameDelay = 1000.0f / FPS;

    while (!quit) {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        deltaTime = std::min(deltaTime, 0.25f);
        accumulator += deltaTime;

        while (SDL_PollEvent(&EVENT)) {
            if (EVENT.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        int numKeys = 0;
        const bool* keyState = SDL_GetKeyboardState(&numKeys);

        while (accumulator >= FIXED_TIME_STEP) {
            for (auto& player : players) {
                player->handlePlayerInputAndPosition(keyState);
            }

            // Bullet <-> Asteroid collision
            for (auto it = asteroids.begin(); it != asteroids.end();) {
                SDL_FRect asteroidRect = {
                    static_cast<float>(it->getRectX()),
                    static_cast<float>(it->getRectY()),
                    static_cast<float>(it->getRectWidth()),
                    static_cast<float>(it->getRectHeight())};

                bool asteroidHit = false;
                for (auto& player : players) {
                    auto bulletIndex =
                        player->getWeapon().checkBulletCollision(asteroidRect);
                    if (bulletIndex.has_value()) {
                        player->getWeapon().destroyBullet(bulletIndex.value());
                        scoreDisplay.setScore(100);
                        it = asteroids.erase(it);
                        asteroidHit = true;
                        break;
                    }
                }
                if (!asteroidHit)
                    ++it;
            }

            // Level advancement
            if (asteroids.empty()) {
                currentLevel++;
                if (currentLevel == 2) {
                    asteroidsForLevel = 80;
                    displayLevelText(gRenderer, currentLevel);
                    asteroids =
                        spawnAsteroids(asteroidsForLevel, players, gRenderer);
                }
            }

            // Player <-> Asteroid collision
            std::vector<Player*> playersToRemove;
            for (const auto& player : players) {
                SDL_FRect playerRect = {
                    static_cast<float>(player->getPosition().first),
                    static_cast<float>(player->getPosition().second),
                    static_cast<float>(player->getWidth()),
                    static_cast<float>(player->getHeight())};

                for (const auto& asteroid : asteroids) {
                    SDL_FRect asteroidRect = {
                        static_cast<float>(asteroid.getRectX()),
                        static_cast<float>(asteroid.getRectY()),
                        static_cast<float>(asteroid.getRectWidth()),
                        static_cast<float>(asteroid.getRectHeight())};

                    SDL_FRect result;
                    if (SDL_GetRectIntersectionFloat(&playerRect, &asteroidRect,
                                                     &result)) {
                        if (players.size() == 1) {
                            players.clear();
                            players = Player::createPlayers(++deathCount);
                            for (auto& p : players) {
                                if (!p->loadTexture("spaceship.png",
                                                    gRenderer)) {
                                    std::print(
                                        "Failed to load player texture\n");
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

            std::erase_if(players, [&playersToRemove](
                                       const std::unique_ptr<Player>& p) {
                return std::find(playersToRemove.begin(), playersToRemove.end(),
                                 p.get()) != playersToRemove.end();
            });

            for (auto& asteroid : asteroids) {
                asteroid.update();
            }

            accumulator -= FIXED_TIME_STEP;
        }

        // Rendering
        SDL_SetRenderDrawColor(gRenderer, 10, 14, 33, 255);
        SDL_RenderClear(gRenderer);

        scoreDisplay.renderScore(gRenderer);
        for (auto& player : players) {
            player->renderPlayer(gRenderer);
        }
        for (auto& asteroid : asteroids) {
            asteroid.renderAsteroid(gRenderer);
        }

        // Level indicator
        std::string levelText = "Level: " + std::to_string(currentLevel);
        SDL_Surface* levelSurface = TTF_RenderText_Solid(
            font, levelText.c_str(), 0, {255, 255, 255, 255});
        SDL_Texture* levelTexture =
            SDL_CreateTextureFromSurface(gRenderer, levelSurface);
        SDL_FRect levelRect = {10.0f, 10.0f,
                               static_cast<float>(levelSurface->w),
                               static_cast<float>(levelSurface->h)};
        SDL_RenderTexture(gRenderer, levelTexture, nullptr, &levelRect);
        SDL_DestroySurface(levelSurface);
        SDL_DestroyTexture(levelTexture);

        SDL_RenderPresent(gRenderer);

        Uint64 endTime = SDL_GetTicks();
        auto elapsedMS = static_cast<float>(endTime - currentTime);
        if (frameDelay > elapsedMS) {
            SDL_Delay(static_cast<Uint32>(frameDelay - elapsedMS));
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    close();
    return 0;
}
