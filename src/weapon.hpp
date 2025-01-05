#pragma once
#include "SDL_stdinc.h"
#include <math.h>
#include <vector>
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
  void update(float deltaTime);
  void render(SDL_Renderer *renderer);
  bool isOffScreen() const;
};

class Weapon {
  std::vector<Bullet> bullets;
  float x, y;
  float cooldown = 250.0f;
  Uint32 lastShotTime = 0; // Ticks from SDL
  float bulletSpeed = 600.0f;
  float angle = (-M_PI / 2);

public:
  Weapon();
  void update(float playerX, float playerY);
  void render(SDL_Renderer *renderer);
  void shoot();
  bool canShoot();
};
