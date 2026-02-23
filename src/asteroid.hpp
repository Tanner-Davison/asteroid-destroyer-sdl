#pragma once
#include "Player.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

struct SDL_Renderer;

class Asteroid {
private:
    float rectXf;
    float rectYf;
    int rectX;
    int rectY;
    int rectWidth;
    int rectHeight;

    SDL_Texture* mTexture;
    int textureWidth;
    int textureHeight;

    double angle;
    double rotationSpeed;
    float velocityX;
    float velocityY;

    const float MAX_VELOCITY = 3.1f;
    const float ACCELERATION = 0.55f;
    const float DECELERATION = 0.992f;
    bool destroyed = false;

public:
    Asteroid(const std::vector<std::unique_ptr<Player>>& players);
    Asteroid(const Asteroid& other);
    Asteroid& operator=(const Asteroid& other);
    ~Asteroid();

    void renderAsteroid(SDL_Renderer* renderer);
    bool loadTexture(const char* path, SDL_Renderer* renderer);
    void update();

    int getRectX() const;
    int getRectY() const;
    int getRectWidth() const;
    int getRectHeight() const;
    void destroy();
    bool isDestroyed() const;
    bool checkStartCollision(const SDL_FRect& a, const SDL_FRect& b);
};
