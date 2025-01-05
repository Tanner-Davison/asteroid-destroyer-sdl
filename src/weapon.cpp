#include "weapon.hpp"
#include "createwindow.hpp"
Bullet::Bullet(float startX, float startY, float velX, float velY)
    : x(startX), y(startY), velocityX(velX), velocityY(velY) {};

void Bullet::render(SDL_Renderer *renderer) {
  SDL_Rect bulletRect = {static_cast<int>(x), static_cast<int>(y), WIDTH,
                         HEIGHT};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Color: White
  SDL_RenderFillRect(renderer, &bulletRect);
}
void Bullet::update(float deltaTime) {
  x += velocityX * deltaTime;
  y += velocityY * deltaTime;
};

bool Bullet::isOffScreen() const {
  return (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT);
}
////////////////////////// WEAPON  Impls///////////////////////////////////

Weapon::Weapon() : x(0), y(0), cooldown(250.0f), lastShotTime(0) {};

void Weapon::update(float playerX, float playerY) {
  x = playerX - 4;
  y = playerY - 27;

  // update bullets
  float deltaTime = 1.0f / 60.0f;

  bullets.erase(
      std::remove_if(bullets.begin(), bullets.end(),
                     [](const Bullet &bullet) { return bullet.isOffScreen(); }),
      bullets.end());

  for (auto &bullet : bullets) {
    bullet.update(deltaTime);
  }
}
void Weapon::render(SDL_Renderer *renderer) {
  // render bullets
  for (auto &bullet : bullets) {
    bullet.render(renderer);
  }

  // Optionally render the weapon itself
  SDL_Rect weaponRect = {static_cast<int>(x), static_cast<int>(y), 10, 10};
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
  SDL_RenderFillRect(renderer, &weaponRect);
}

void Weapon::shoot() {
  if (!canShoot())
    return;

  float velX = bulletSpeed * cos(angle);
  float velY = bulletSpeed * sin(angle);

  bullets.emplace_back(x, y, velX, velY);
  lastShotTime = SDL_GetTicks();
}
bool Weapon::canShoot() { return (SDL_GetTicks() - lastShotTime >= cooldown); }
