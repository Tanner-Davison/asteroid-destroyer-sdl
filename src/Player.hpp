#pragma once
#include "weapon.hpp"
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
  const int rectWidth = 35;
  const int rectHeight = 35;

  // Physics Properties
  float velocityX;
  float velocityY;
  const float MAX_VELOCITY = 8.0f;  // Fixed maximum velocity
  const float ACCELERATION = 1.0f;  // Fixed acceleration rate
  const float DECELERATION = 0.82f; // Consistent deceleration

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
  ~Player();

  // Core functions
  void renderPlayer(SDL_Renderer *renderer);
  void handleInput(bool up, bool down, bool left, bool right, bool shooting);

  // Position management
  void setPlayerPos(int x, int y);
  void updatePlayerPos();
  void handleBounds(float nextX, float nextY);
  std::pair<int, int> getPosition() const;

  // Getters for size (if needed)
  int getWidth() const { return rectWidth; }
  int getHeight() const { return rectHeight; }
};
