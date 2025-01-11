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
  const float MAX_VELOCITY = 3.4f;       // Maximum velocity
  const float BASE_ACCELERATION = .60f;  // BASE_ACCELERATION
  const float BOOST_ACCELERATION = 3.5f; // BOOST_ACCELERATION
  const float DECELERATION = .9930;      // DECELERATION
  float ACCELERATION = .78f;
  // TEXTURES
  SDL_Rect playerRect;
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
  Player(int x, int y);
  Player(int x, int y, int width, int height);
  ~Player();
  // Core functions
  void renderPlayer(SDL_Renderer *renderer);
  // input and pos
  void handlePlayerInputAndPosition(const Uint8 *keyState);
  void handleBoundsAndUpdatePosition(float nextX, float nextY);
  bool loadTexture(const char *path, SDL_Renderer *renderer);
  void cleanup(); // Texture
  bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);

  const Weapon &getWeapon() const;
  Weapon &getWeapon();
  std::pair<int, int> getPosition() const;

  // get sizes
  int getWidth() const { return rectWidth; }
  int getHeight() const { return rectHeight; }
};
