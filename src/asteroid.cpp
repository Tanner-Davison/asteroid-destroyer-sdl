#include "asteroid.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include "createwindow.hpp"
#include <SDL2/SDL.h>
#include <random>

class Player;
Asteroid::Asteroid(const std::vector<std::unique_ptr<Player>>& players)
    : rectXf(0), rectYf(0), rectWidth(100), rectHeight(100), mTexture(nullptr) {
    std::uniform_int_distribution<> disX(0, SCREEN_WIDTH - rectWidth);
    std::uniform_int_distribution<> disY(0, SCREEN_HEIGHT - rectHeight);

    bool validPosition = false;
    while (!validPosition) {
        this->rectX = disX(GameRNG::gen);
        this->rectY = disY(GameRNG::gen);
        this->rectXf = static_cast<float>(rectX);
        this->rectYf = static_cast<float>(rectY);

        // Check if asteroid collides with any player
        validPosition = true;
        for (const auto& player : players) {
            SDL_Rect playerRect = {player->getPosition().first, player->getPosition().second, player->getWidth(),
                                   player->getHeight()};
            SDL_Rect asteroidRect = {rectX, rectY, rectWidth, rectHeight};

            // Check for collision (this assumes you have a checkCollision function or
            // use SDL to compare rects)
            if (checkStartCollision(asteroidRect, playerRect)) {
                validPosition = false; // Re-generate if collision is detected
                break;
            }
        }
    }

    // After finding a valid position, handle movement and other initialization
    std::uniform_real_distribution<float> velDis(-2.0f, 2.0f);
    velocityX = velDis(GameRNG::gen);
    velocityY = velDis(GameRNG::gen);
    angle = 0.0;
    std::uniform_real_distribution<> rotDis(-2.0, 2.0);
    rotationSpeed = rotDis(GameRNG::gen);
}

Asteroid::Asteroid(const Asteroid& other)
    : rectXf(other.rectXf),
      rectYf(other.rectYf),
      rectX(other.rectX),
      rectY(other.rectY),
      rectWidth(other.rectWidth),
      rectHeight(other.rectHeight),
      velocityX(other.velocityX),
      velocityY(other.velocityY),
      angle(other.angle),
      rotationSpeed(other.rotationSpeed),
      destroyed(other.destroyed),
      mTexture(other.mTexture) {}

// Copy assignment operator
Asteroid& Asteroid::operator=(const Asteroid& other) {
    if (this != &other) { // Protect against self-assignment
        rectXf = other.rectXf;
        rectYf = other.rectYf;
        rectX = other.rectX;
        rectY = other.rectY;
        rectWidth = other.rectWidth;
        rectHeight = other.rectHeight;
        velocityX = other.velocityX;
        velocityY = other.velocityY;
        angle = other.angle;
        rotationSpeed = other.rotationSpeed;
        destroyed = other.destroyed;
        mTexture = other.mTexture;
    }
    return *this;
}
Asteroid::~Asteroid() = default;
void Asteroid::renderAsteroid(SDL_Renderer* renderer) {
    SDL_Rect rect = {rectX, rectY, rectWidth, rectHeight};
    if (mTexture != nullptr) {
        SDL_RenderCopy(renderer, mTexture, NULL, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Medium gray
        SDL_RenderFillRect(renderer, &rect);
    }
}
bool Asteroid::loadTexture(const char* path, SDL_Renderer* renderer) {
    if (this->mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
    // load Texture
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error %s\n", path, IMG_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (mTexture == nullptr) {
        printf("Unable to create Texture %s! SDL Error: %s\n", path, SDL_GetError());
        return false;
    }
    // main implementation
    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return true;
}
void Asteroid::update() {
    // Update position
    rectXf += velocityX;
    rectYf += velocityY;

    // Wrap around screen edges
    if (rectXf < -rectWidth)
        rectXf = SCREEN_WIDTH;
    if (rectXf > SCREEN_WIDTH)
        rectXf = -rectWidth;
    if (rectYf < -rectHeight)
        rectYf = SCREEN_HEIGHT;
    if (rectYf > SCREEN_HEIGHT)
        rectYf = -rectHeight;

    // Update integer coordinates
    rectX = static_cast<int>(rectXf);
    rectY = static_cast<int>(rectYf);

    // Update rotation
    angle += rotationSpeed;
    if (angle > 360)
        angle -= 360;
    if (angle < 0)
        angle += 360;
}

int Asteroid::getRectX() const {
    return this->rectX;
}
int Asteroid::getRectY() const {
    return this->rectY;
}
int Asteroid::getRectWidth() const {
    return this->rectWidth;
}
int Asteroid::getRectHeight() const {
    return this->rectHeight;
}
void Asteroid::destroy() {
    destroyed = true;
}
bool Asteroid::isDestroyed() const {
    return destroyed;
}
bool Asteroid::checkStartCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return SDL_HasIntersection(&a, &b) != SDL_FALSE;
}
