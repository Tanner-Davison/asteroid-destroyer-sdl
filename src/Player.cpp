#include "Player.hpp"
#include "createwindow.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

// Most complete constructor
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
    playerRect = {getPosition().first, getPosition().second, getWidth(),
                  getHeight()};
}
// Position constructor - delegates to full constructor with default dimensions
Player::Player(int rectX, int rectY) : Player(rectX, rectY, 50, 50) {}
// Default constructor - delegates to position constructor with default position
Player::Player() : Player(100, 100) {}

void Player::renderPlayer(SDL_Renderer* renderer) {
    SDL_Rect rect = {rectX, rectY, rectWidth, rectHeight};
    if (mTexture != nullptr) {
        SDL_RenderCopy(renderer, mTexture, nullptr, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    // Render weapon // updates position
    weapon.update((rectX - 2) + static_cast<float>(rectWidth) / 2, rectY - 6);
    weapon.render(renderer);
}

void Player::handleBoundsAndUpdatePosition(float nextX, float nextY) {
    const bool hitLeftWall = nextX <= 0,
               hitRightWall = nextX >= SCREEN_WIDTH - rectWidth,
               hitTopWall = nextY <= 0,
               hitBottomWall = nextY >= SCREEN_HEIGHT - rectHeight;

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

    // update Position
    rectX = static_cast<int>(rectXf);
    rectY = static_cast<int>(rectYf);
}
bool Player::loadTexture(const char* path, SDL_Renderer* renderer) {
    // Free Existing Texture * ERROR HANDLING *
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
    // Load Image
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == nullptr) {
        printf("Unable to laod image %s! SDL_image Error: %s\n", path,
               IMG_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (mTexture == nullptr) {
        printf("Unable to create Texture %s! SDL Error: %s\n", path,
               SDL_GetError());
        return false;
    }
    // Main implementation
    textureWidth = loadedSurface->w;
    textureHeight = loadedSurface->h;
    // Free the survface!!
    SDL_FreeSurface(loadedSurface);
    return true;
}

// PlayerInput
void Player::handlePlayerInputAndPosition(const Uint8* keyState) {
    const bool up = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP],
               down = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN],
               left = keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT],
               right = keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT],
               boost = keyState[SDL_SCANCODE_LSHIFT],
               isShooting = keyState[SDL_SCANCODE_SPACE];

    ACCELERATION = boost ? BOOST_ACCELERATION : BASE_ACCELERATION;
    float CurrentMaxVelocity = boost ? MAX_VELOCITY * 1.5f : MAX_VELOCITY;

    // Movement handling
    if (right) {
        velocityX = std::min(velocityX + ACCELERATION, CurrentMaxVelocity);
    } else if (left) {
        velocityX = std::max(velocityX - ACCELERATION, -CurrentMaxVelocity);
    } else {
        velocityX *= DECELERATION;
        if (abs(velocityX) < 0.3f) {
            velocityX = 0;
        }
    }

    if (down) {
        velocityY = std::min(velocityY + ACCELERATION, CurrentMaxVelocity);
    } else if (up) {
        velocityY = std::max(velocityY - ACCELERATION, -CurrentMaxVelocity);
    } else {
        velocityY *= DECELERATION;
        if (abs(velocityY) < 0.3f) {
            velocityY = 0;
        }
    }

    if (isShooting) {
        weapon.shoot();
    }

    float nextX = rectXf + velocityX;
    float nextY = rectYf + velocityY;
    handleBoundsAndUpdatePosition(nextX, nextY);
}
//
// Not connected to handlePlayerInputAndPosition
std::pair<int, int> Player::getPosition() const {
    return std::make_pair(rectX, rectY);
}

bool Player::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    // edges recA
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    // edges recB
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // Check if any edges don't overlap
    if (bottomA <= topB)
        return false;
    if (topA >= bottomB)
        return false;
    if (rightA <= leftB)
        return false;
    if (leftA >= rightB)
        return false;

    // If none of the above, they overlap
    return true;
}
const Weapon& Player::getWeapon() const {
    return weapon;
}
Weapon& Player::getWeapon() {
    return weapon;
}
void Player::cleanup() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }
}
std::vector<std::unique_ptr<Player>> Player::createPlayers(int count) {
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
            players.emplace_back(
                std::make_unique<Player>(centerX + offsetX, bottomY - offsetY));
        }
    }

    return players;
}
Player::~Player() {
    cleanup();
};
