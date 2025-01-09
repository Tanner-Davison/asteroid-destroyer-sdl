#pragma once
#include "SDL_rect.h"
#include "SDL_stdinc.h"
#include <math.h>
#include <optional>
#include <vector>
struct SDL_Renderer;

class Bullet {
private:
  float x, y;
  float velocityX;
  float velocityY;
  static const int WIDTH = 2;
  static const int HEIGHT = 45;
  bool active = true;

public:
  Bullet(float startX, float startY, float velX, float velY);
  void update(float deltaTime);
  void render(SDL_Renderer *renderer);
  bool isOffScreen() const;
  bool isActive() const;
  SDL_Rect getHitBox() const;
  void deactivate();
};

class Weapon {
  std::vector<Bullet> bullets;
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
  const std::vector<Bullet> &getBullets() const { return bullets; };
  void shoot();
  bool canShoot();
  void destroyBullet(size_t index);
  std::optional<size_t> checkBulletCollision(const SDL_Rect &target);
};
