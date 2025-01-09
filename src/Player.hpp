#pragma once
#include "SDL_rect.h"
#include "weapon.hpp"
#include <SDL_image.h>
#include <utility>

struct SDL_Renderer;

class Player {
private:
  // FLOATING POINT POSITION FOR SMOOTHER CHANGES
  float rectXf;
  float rectYf;

  // INT POSITION FOR RENDERING
  int rectX;
  int rectY;

  // PLAYER SIZE
  int rectWidth = 45;
  int rectHeight = 45;

  // PHYSICS PROPERTIES
  float velocityX;
  float velocityY;
  const float MAX_VELOCITY = 3.4f;       // Fixed maximum velocity
  const float BASE_ACCELERATION = .60f;  // FIXED
  const float BOOST_ACCELERATION = 1.5f; // FIXED BOOST_ACCELERATION
  const float DECELERATION = .9930;      // FIXED Consistent deceleration
  float ACCELERATION = .78f;
  SDL_Rect playerRect;
  // TEXTURES
  SDL_Texture *mTexture;
  int textureWidth;
  int textureHeight;

  // MOVEMENT STATE
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
  Player(float x, float y);
  Player(float x, float y, int width, int height);
  ~Player();
  // Player input
  void handlePlayerInput(const Uint8 *keyState);
  void handleInput(bool up, bool down, bool left, bool right, bool shooting,
                   bool boost);
  // Core functions
  void renderPlayer(SDL_Renderer *renderer);

  // Position management
  void setPlayerPos(int x, int y);
  void updatePlayerPos();
  void updatePlayerPos(int x, int y);
  void handleBounds(float nextX, float nextY);
  bool loadTexture(const char *path, SDL_Renderer *renderer);
  void cleanup(); // so far texture is my only one needed to clean;
  bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);
  Weapon getWeapon() const;

  std::pair<int, int> getPosition() const;

  // Getters for size (if needed)
  int getWidth() const { return rectWidth; }
  int getHeight() const { return rectHeight; }
};
