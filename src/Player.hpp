#pragma once
#include <SDL2/SDL_rect.h>
#include "weapon.hpp"

#include <SDL2/SDL_image.h>

#include <utility>

struct SDL_Renderer;

class Player {
private:
    // smooth movement float points
    float rectXf;
    float rectYf;

    // Rendering position
    int rectX;
    int rectY;

    // PLAYER SIZE
    int rectWidth = 50;
    int rectHeight = 50;

    // Physics properties
    float velocityX;
    float velocityY;
#ifdef _WIN32
    const float MAX_VELOCITY = 5.4f;       // Maximum velocity
    const float BASE_ACCELERATION = .70f;  // BASE_ACCELERATION
    const float BOOST_ACCELERATION = 3.5f; // BOOST_ACCELERATION
    const float DECELERATION = .9830;      // DECELERATION
    float ACCELERATION = .05f;
#else
    const float MAX_VELOCITY = 3.4f;       // Maximum velocity
    const float BASE_ACCELERATION = .40f;  // BASE_ACCELERATION
    const float BOOST_ACCELERATION = 3.5f; // BOOST_ACCELERATION
    const float DECELERATION = .9730;      // DECELERATION
    float ACCELERATION = .05f;

#endif
    // Textures
    SDL_Rect playerRect;
    SDL_Texture* mTexture;
    int textureWidth;
    int textureHeight;
    // Movement state
    bool isMovingUp;
    bool isMovingDown;
    bool isMovingLeft;
    bool isMovingRight;
    bool shooting;
    bool boost;
    // Weapon system
    Weapon weapon;

public:
    Player();
    Player(int x, int y);
    Player(int x, int y, int width, int height);
    ~Player();
    // Core functions
    void renderPlayer(SDL_Renderer* renderer);
    // input and pos
    void handlePlayerInputAndPosition(const Uint8* keyState);
    void handleBoundsAndUpdatePosition(float nextX, float nextY);
    bool loadTexture(const char* path, SDL_Renderer* renderer);
    void cleanup(); // Texture
    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);

    const Weapon& getWeapon() const;
    Weapon& getWeapon();
    std::pair<int, int> getPosition() const;

    // get sizes
    int getWidth() const {
        return rectWidth;
    }
    int getHeight() const {
        return rectHeight;
    }
};
