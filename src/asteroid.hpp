#pragma once
#include "Player.hpp"
#include "SDL_render.h"
#include <memory>
#include <vector>
struct SDL_Renderer;

class Asteroid {
private:
    float rectXf;
    float rectYf;
    // render position
    int rectX;
    int rectY;
    // player size
    int rectWidth;
    int rectHeight;
    // TEXTURES
    SDL_Texture* mTexture;
    int textureWidth;
    int textureHeight;
    // movement
    double angle;
    double rotationSpeed;
    float velocityX;
    float velocityY;
    const float MAX_VELOCITY = 3.1f; // Fixed maximum velocity
    const float ACCELERATION = .55f; // Fixed acceleration rate
    const float DECELERATION = .992; // Consistent deceleration
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
    bool checkStartCollision(const SDL_Rect& a, const SDL_Rect& b);
};
