#pragma once
#include "SDL_rect.h"
#include "weapon.hpp"
#include <SDL_image.h>
#include <utility>

struct SDL_Renderer;

class Player {
private:
  // Position (Floating Point for smoother movement)
  float rectXf;
  float rectYf;

  // Integer position (for rendering)
  int rectX;
  int rectY;

  // Player size
  int rectWidth = 45;
  int rectHeight = 45;

  // Physics Properties
  float velocityX;
  float velocityY;
  const float MAX_VELOCITY = 3.4f; // Fixed maximum velocity
  const float BASE_ACCELERATION = .60f;
  const float BOOST_ACCELERATION = 1.5f;
  const float DECELERATION = .9930; // Consistent deceleration
  float ACCELERATION = .78f;
  //
  // TEXTURES
  SDL_Texture *mTexture;
  int textureWidth;
  int textureHeight;
  //

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

  std::pair<int, int> getPosition() const;

  // Getters for size (if needed)
  int getWidth() const { return rectWidth; }
  int getHeight() const { return rectHeight; }
};
