#pragma once
#include "SDL_rect.h"
#include "SDL_stdinc.h"
#include <math.h>
#include <vector>
struct SDL_Renderer;

class Bullet {
private:
  float x, y;
  float velocityX;
  float velocityY;
  static const int WIDTH = 3;
  static const int HEIGHT = 3;
  bool active;

public:
  Bullet(float startX, float startY, float velX, float velY);
  void update(float deltaTime);
  void render(SDL_Renderer *renderer);
  bool isOffScreen() const;
  bool isActive();
  SDL_Rect getHitBox() const;
  void deactivate();
};

class Weapon {
  std::vector<Bullet> bullets;
  const std::vector<Bullet> &getBullets() const { return bullets; };
  float x;
  float y;
  float cooldown;
  Uint32 lastShotTime;
  float bulletSpeed;
  float angle;

public:
  Weapon();
  void update(float playerX, float playerY);
  void render(SDL_Renderer *renderer);
  void shoot();
  bool canShoot();
  bool checkBulletCollision(const SDL_Rect &target);
};
