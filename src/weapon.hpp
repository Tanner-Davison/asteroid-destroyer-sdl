#pragma once
struct SDL_Renderer;

class Bullet {
private:
  float x, y;
  float velocityX;
  float velocityY;
  static const int WIDTH = 5;
  static const int HEIGHT = 5;

public:
  Bullet(float startX, float startY, float velX, float velY);
  void update();
  void render(SDL_Renderer *renderer);
  bool isOffScreen();
};
