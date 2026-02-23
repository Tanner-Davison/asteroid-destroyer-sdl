#include "Player.hpp"
#include "createwindow.hpp"
#include <SDL3/SDL.h>

Player::Player(int rectX, int rectY, int width, int height)
    : isMovingUp(false),
      isMovingDown(false),
      isMovingLeft(false),
      isMovingRight(false),
      shooting(false),
      boost(false),
      rectXf(static_cast<float>(rectX)),
      rectYf(static_cast<float>(rectY)),
      ACCELERATION(BASE_ACCELERATION),
      rectX(rectX),
      rectY(rectY),
      velocityX(0.0f),
      velocityY(0.0f),
      rectWidth(width),
      rectHeight(height),
      mTexture(nullptr),
      textureWidth(0),
      textureHeight(0) {
    playerRect = {
        static_cast<float>(rectX), static_cast<float>(rectY),
        static_cast<float>(width), static_cast<float>(height)
    };
}

Player::Player(int rectX, int rectY) : Player(rectX, rectY, 50, 50) {}
Player::Player() : Player(100, 100) {}

void Player::renderPlayer(SDL_Renderer* renderer) {
    SDL_FRect rect = {
        static_cast<float>(rectX), static_cast<float>(rectY),
        static_cast<float>(rectWidth), static_cast<float>(rectHeight)
    };
    if (mTexture != nullptr) {
        SDL_RenderTexture(renderer, mTexture, nullptr, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    weapon.update((rectX - 2) + static_cast<float>(rectWidth) / 2, rectY - 6);
    weapon.render(renderer);
}

void Player::handleBoundsAndUpdatePosition(float nextX, float nextY) {
    const bool hitLeftWall = nextX <= 0;
    const bool hitRightWall = nextX >= SCREEN_WIDTH - rectWidth;
    const bool hitTopWall = nextY <= 0;
    const bool hitBottomWall = nextY >= SCREEN_HEIGHT - rectHeight;

    if (hitLeftWall) {
        rectXf = 0.0f;
        velocityX = -velocityX * 1.8f;
    } else if (hitRightWall) {
        rectXf = SCREEN_WIDTH - rectWidth;
        velocityX = -velocityX * 1.8f;
    } else {
        rectXf = nextX;
    }

    if (hitTopWall) {
        rectYf = 0.0f;
        velocityY = -velocityY * 1.8f;
    } else if (hitBottomWall) {
        rectYf = SCREEN_HEIGHT - rectHeight;
        velocityY = -velocityY * 1.8f;
    } else {
        rectYf = nextY;
    }

    rectX = static_cast<int>(rectXf);
    rectY = static_cast<int>(rectYf);
}

bool Player::loadTexture(const char* path, SDL_Renderer* renderer) {
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

void Player::handlePlayerInputAndPosition(const bool* keyState) {
    const bool up    = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP];
    const bool down  = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN];
    const bool left  = keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT];
    const bool right = keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT];
    const bool boosting   = keyState[SDL_SCANCODE_LSHIFT];
    const bool isShooting = keyState[SDL_SCANCODE_SPACE];

    ACCELERATION = boosting ? BOOST_ACCELERATION : BASE_ACCELERATION;
    float currentMaxVelocity = boosting ? MAX_VELOCITY * 1.5f : MAX_VELOCITY;

    if (right) {
        velocityX = std::min(velocityX + ACCELERATION, currentMaxVelocity);
    } else if (left) {
        velocityX = std::max(velocityX - ACCELERATION, -currentMaxVelocity);
    } else {
        velocityX *= DECELERATION;
        if (std::abs(velocityX) < 0.3f) velocityX = 0;
    }

    if (down) {
        velocityY = std::min(velocityY + ACCELERATION, currentMaxVelocity);
    } else if (up) {
        velocityY = std::max(velocityY - ACCELERATION, -currentMaxVelocity);
    } else {
        velocityY *= DECELERATION;
        if (std::abs(velocityY) < 0.3f) velocityY = 0;
    }

    if (isShooting) weapon.shoot();

    handleBoundsAndUpdatePosition(rectXf + velocityX, rectYf + velocityY);
}

std::pair<int, int> Player::getPosition() const {
    return {rectX, rectY};
}

bool Player::checkCollision(const SDL_FRect& a, const SDL_FRect& b) {
    SDL_FRect result;
    return SDL_GetRectIntersectionFloat(&a, &b, &result);
}

const Weapon& Player::getWeapon() const { return weapon; }
Weapon& Player::getWeapon() { return weapon; }

void Player::cleanup() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}

std::vector<std::unique_ptr<Player>> Player::createPlayers(int count) {
    int centerX = SCREEN_WIDTH / 2;
    int bottomY = SCREEN_HEIGHT - 100;
    const int PLAYER_SPACING = 70;
    const int VERTICAL_OFFSET = 50;

    std::vector<std::unique_ptr<Player>> players;
    if (count <= 0) return players;

    if (count == 1) {
        players.push_back(std::make_unique<Player>(centerX, bottomY));
    } else {
        int middleIndex = count / 2;
        for (int i = 0; i < count; ++i) {
            int offsetX = (i - middleIndex) * PLAYER_SPACING;
            int offsetY = std::abs(i - middleIndex) * VERTICAL_OFFSET;
            players.emplace_back(std::make_unique<Player>(centerX + offsetX, bottomY - offsetY));
        }
    }
    return players;
}

Player::~Player() { cleanup(); }
