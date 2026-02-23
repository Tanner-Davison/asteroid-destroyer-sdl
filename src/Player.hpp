#pragma once
#include "weapon.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <utility>

struct SDL_Renderer;

class Player {
private:
    float rectXf;
    float rectYf;
    int rectX;
    int rectY;
    int rectWidth = 50;
    int rectHeight = 50;

    float velocityX;
    float velocityY;

#ifdef _WIN32
    const float MAX_VELOCITY = 5.4f;
    const float BASE_ACCELERATION = 0.70f;
    const float BOOST_ACCELERATION = 3.5f;
    const float DECELERATION = 0.9830f;
    float ACCELERATION = 0.05f;
#else
    const float MAX_VELOCITY = 3.4f;
    const float BASE_ACCELERATION = 0.40f;
    const float BOOST_ACCELERATION = 3.5f;
    const float DECELERATION = 0.9730f;
    float ACCELERATION = 0.05f;
#endif

    SDL_FRect playerRect;
    SDL_Texture* mTexture;
    int textureWidth;
    int textureHeight;

    bool isMovingUp;
    bool isMovingDown;
    bool isMovingLeft;
    bool isMovingRight;
    bool shooting;
    bool boost;

    Weapon weapon;

public:
    Player();
    Player(int x, int y);
    Player(int x, int y, int width, int height);
    ~Player();

    void renderPlayer(SDL_Renderer* renderer);
    void handlePlayerInputAndPosition(const bool* keyState);
    void handleBoundsAndUpdatePosition(float nextX, float nextY);
    bool loadTexture(const char* path, SDL_Renderer* renderer);
    void cleanup();
    bool checkCollision(const SDL_FRect& a, const SDL_FRect& b);

    static std::vector<std::unique_ptr<Player>> createPlayers(int count);
    const Weapon& getWeapon() const;
    Weapon& getWeapon();
    std::pair<int, int> getPosition() const;

    int getWidth() const { return rectWidth; }
    int getHeight() const { return rectHeight; }
};
