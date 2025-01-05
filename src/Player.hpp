#pragma once
struct SDL_Renderer;

class Player {
private:
  // position (Floating Point for smoother movement)
  float rectXf;
  float rectYf;
  // integer position (for rendering)
  int rectX;
  int rectY;
  // player size
  const int rectWidth = 50;
  const int rectHeight = 50;
  // physics Properties
  float velocityX;
  float velocityY;
  const float MAX_VELOCITY = 8.0f;  // Fixed maximum velocity
  const float ACCELERATION = 1.0f;  // Fixed acceleration rate
  const float DECELERATION = 0.92f; // Consistent deceleration
  // player movement
  bool isMovingUp;
  bool isMovingDown;
  bool isMovingLeft;
  bool isMovingRight;

public:
  Player();
  ~Player();
  void renderPlayer(SDL_Renderer *renderer);
  void setPlayerPos(int x, int y);
  void updatePlayerPos();
  void handleBounds(float nextX, float nextY);
  void handleInput(bool up, bool down, bool left, bool right);
};
