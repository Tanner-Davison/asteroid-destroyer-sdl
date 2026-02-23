#include "asteroid.hpp"
#include "createwindow.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <random>

Asteroid::Asteroid(const std::vector<std::unique_ptr<Player>>& players)
    : rectXf(0), rectYf(0), rectWidth(100), rectHeight(100), mTexture(nullptr),
      textureWidth(0), textureHeight(0), angle(0.0), rotationSpeed(0.0),
      velocityX(0.0f), velocityY(0.0f), destroyed(false) {

    std::uniform_int_distribution<> disX(0, SCREEN_WIDTH - rectWidth);
    std::uniform_int_distribution<> disY(0, SCREEN_HEIGHT - rectHeight);

    bool validPosition = false;
    while (!validPosition) {
        rectX = disX(GameRNG::gen);
        rectY = disY(GameRNG::gen);
        rectXf = static_cast<float>(rectX);
        rectYf = static_cast<float>(rectY);

        validPosition = true;
        for (const auto& player : players) {
            SDL_FRect playerRect = {
                static_cast<float>(player->getPosition().first),
                static_cast<float>(player->getPosition().second),
                static_cast<float>(player->getWidth()),
                static_cast<float>(player->getHeight())
            };
            SDL_FRect asteroidRect = {
                rectXf, rectYf,
                static_cast<float>(rectWidth), static_cast<float>(rectHeight)
            };
            if (checkStartCollision(asteroidRect, playerRect)) {
                validPosition = false;
                break;
            }
        }
    }

    std::uniform_real_distribution<float> velDis(-2.0f, 2.0f);
    velocityX = velDis(GameRNG::gen);
    velocityY = velDis(GameRNG::gen);

    std::uniform_real_distribution<> rotDis(-2.0, 2.0);
    rotationSpeed = rotDis(GameRNG::gen);
}

Asteroid::Asteroid(const Asteroid& other)
    : rectXf(other.rectXf), rectYf(other.rectYf),
      rectX(other.rectX), rectY(other.rectY),
      rectWidth(other.rectWidth), rectHeight(other.rectHeight),
      velocityX(other.velocityX), velocityY(other.velocityY),
      angle(other.angle), rotationSpeed(other.rotationSpeed),
      destroyed(other.destroyed), mTexture(other.mTexture),
      textureWidth(other.textureWidth), textureHeight(other.textureHeight) {}

Asteroid& Asteroid::operator=(const Asteroid& other) {
    if (this != &other) {
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
        textureWidth = other.textureWidth;
        textureHeight = other.textureHeight;
    }
    return *this;
}

Asteroid::~Asteroid() = default;

void Asteroid::renderAsteroid(SDL_Renderer* renderer) {
    SDL_FRect rect = {rectXf, rectYf, static_cast<float>(rectWidth), static_cast<float>(rectHeight)};
    if (mTexture != nullptr) {
        SDL_RenderTexture(renderer, mTexture, nullptr, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Asteroid::loadTexture(const char* path, SDL_Renderer* renderer) {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!mTexture) {
        printf("Unable to create texture %s! SDL Error: %s\n", path, SDL_GetError());
        SDL_DestroySurface(loadedSurface);
        return false;
    }
    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;
    SDL_DestroySurface(loadedSurface);
    return true;
}

void Asteroid::update() {
    rectXf += velocityX;
    rectYf += velocityY;

    if (rectXf < -rectWidth)  rectXf = static_cast<float>(SCREEN_WIDTH);
    if (rectXf > SCREEN_WIDTH) rectXf = static_cast<float>(-rectWidth);
    if (rectYf < -rectHeight) rectYf = static_cast<float>(SCREEN_HEIGHT);
    if (rectYf > SCREEN_HEIGHT) rectYf = static_cast<float>(-rectHeight);

    rectX = static_cast<int>(rectXf);
    rectY = static_cast<int>(rectYf);

    angle += rotationSpeed;
    if (angle > 360) angle -= 360;
    if (angle < 0)   angle += 360;
}

int Asteroid::getRectX() const { return rectX; }
int Asteroid::getRectY() const { return rectY; }
int Asteroid::getRectWidth() const { return rectWidth; }
int Asteroid::getRectHeight() const { return rectHeight; }
void Asteroid::destroy() { destroyed = true; }
bool Asteroid::isDestroyed() const { return destroyed; }

bool Asteroid::checkStartCollision(const SDL_FRect& a, const SDL_FRect& b) {
    SDL_FRect result;
    return SDL_GetRectIntersectionFloat(&a, &b, &result);
}
