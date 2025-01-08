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
  const float MAX_VELOCITY = 3.4f;  // Fixed maximum velocity
  const float ACCELERATION = .78f;  // Fixed acceleration rate
  const float DECELERATION = .9930; // Consistent deceleration
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

  // Weapon system
  Weapon weapon;

public:
  Player();
  Player(float x, float y);
  Player(float x, float y, int width, int height);
  ~Player();
  // Player input
  void handlePlayerInput(const Uint8 *keyState);
  // Core functions
  void renderPlayer(SDL_Renderer *renderer);
  void handleInput(bool up, bool down, bool left, bool right, bool shooting);

  // Position management
  void setPlayerPos(int x, int y);
  void updatePlayerPos();
  void updatePlayerPos(int x, int y);
  void handleBounds(float nextX, float nextY);
  bool loadTexture(const char *path, SDL_Renderer *renderer);
  void cleanup(); // texture;
  bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);

  std::pair<int, int> getPosition() const;

  // Getters for size (if needed)
  int getWidth() const { return rectWidth; }
  int getHeight() const { return rectHeight; }
};
