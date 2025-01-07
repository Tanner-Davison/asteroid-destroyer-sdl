#pragma once
struct SDL_Renderer;

class Asteroid {
private:
  float rectXf;
  float rectYf;
  // render position
  int rectX;
  int rectY;
  // player size
  int rectWidth;
  int rectHeight;

  // movement
  double angle;
  double rotationSpeed;
  float velocityX;
  float velocityY;
  const float MAX_VELOCITY = 3.1f; // Fixed maximum velocity
  const float ACCELERATION = .55f; // Fixed acceleration rate
  const float DECELERATION = .992; // Consistent deceleration

public:
  Asteroid();
  ~Asteroid();
  void renderAsteroid(SDL_Renderer *renderer);
  void update();
};
