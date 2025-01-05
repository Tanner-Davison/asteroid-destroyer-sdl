#include "Player.hpp"
#include "createwindow.hpp"
#include <SDL.h>

Player::Player()
    : isMovingUp(false), isMovingDown(false), isMovingLeft(false),
      isMovingRight(false), shooting(false), rectXf(100.0f), rectYf(100.0f),
      rectX(100), rectY(100), velocityX(0.0f), velocityY(0.0f) {};

Player::~Player() = default;

void Player::renderPlayer(SDL_Renderer *renderer) {
  // Render player rectangle
  SDL_Rect rect = {rectX, rectY, rectWidth, rectHeight};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &rect);

  // Render weapon
  weapon.update(rectX + rectWidth / 2,
                rectY + rectHeight /
                            2); // Update weapon position to center of player
  weapon.render(renderer);
}

void Player::setPlayerPos(int x, int y) {
  this->rectX = x;
  this->rectY = y;
  this->rectXf = static_cast<float>(x);
  this->rectYf = static_cast<float>(y);
}

void Player::updatePlayerPos() {
  rectX = static_cast<int>(rectXf);
  rectY = static_cast<int>(rectYf);
}

void Player::handleBounds(float nextX, float nextY) {
  if (nextX <= 0) {
    rectXf = 0.0f;
    velocityX = -velocityX * 1.8f;
  } else if (nextX >= SCREEN_WIDTH - rectWidth) {
    rectXf = SCREEN_WIDTH - rectWidth;
    velocityX = -velocityX * 1.8f;
  } else {
    rectXf = nextX;
  }

  if (nextY <= 0) {
    rectYf = 0.0f;
    velocityY = -velocityY * 1.8f;
  } else if (nextY >= SCREEN_HEIGHT - rectHeight) {
    rectYf = SCREEN_HEIGHT - rectHeight;
    velocityY = -velocityY * 1.8f;
  } else {
    rectYf = nextY;
  }

  updatePlayerPos();
}

void Player::handleInput(bool up, bool down, bool left, bool right,
                         bool isShooting) {
  // Movement handling
  if (right) {
    velocityX = std::min(velocityX + ACCELERATION, MAX_VELOCITY);
  } else if (left) {
    velocityX = std::max(velocityX - ACCELERATION, -MAX_VELOCITY);
  } else {
    velocityX *= DECELERATION;
    if (abs(velocityX) < 0.3f) {
      velocityX = 0;
    }
  }
  if (down) {
    velocityY = std::min(velocityY + ACCELERATION, MAX_VELOCITY);
  } else if (up) {
    velocityY = std::max(velocityY - ACCELERATION, -MAX_VELOCITY);
  } else {
    velocityY *= DECELERATION;
    if (abs(velocityY) < 0.3f) {
      velocityY = 0;
    }
  }
  // hand weapons shooting
  if (isShooting) {
    weapon.shoot();
  }

  // next position
  float nextX = rectXf + velocityX;
  float nextY = rectYf + velocityY;
  handleBounds(nextX, nextY);
}

std::pair<int, int> Player::getPosition() const {
  return std::make_pair(rectX, rectY);
}
