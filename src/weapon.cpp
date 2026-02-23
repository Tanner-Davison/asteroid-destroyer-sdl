#include "weapon.hpp"
#include "createwindow.hpp"
#include <SDL3/SDL.h>

Bullet::Bullet(float startX, float startY, float velX, float velY)
    : x(startX), y(startY), velocityX(velX), velocityY(velY), active(true) {}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_FRect bulletRect = {x, y, static_cast<float>(WIDTH), static_cast<float>(HEIGHT)};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bulletRect);
}

void Bullet::update(float deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
}

bool Bullet::isOffScreen() const {
    return (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT);
}

SDL_FRect Bullet::getHitBox() const {
    return {x, y, static_cast<float>(WIDTH), static_cast<float>(HEIGHT)};
}

bool Bullet::isActive() const { return active; }
void Bullet::deactivate() { active = false; }

// Weapon
Weapon::Weapon()
    : x(0), y(0), cooldown(350.0f), lastShotTime(0), bulletSpeed(600.0f), angle(-M_PI / 2) {}

void Weapon::update(float playerX, float playerY) {
    x = playerX;
    y = playerY;

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.isOffScreen() || !b.isActive(); }),
        bullets.end());

    const float deltaTime = 1.0f / 60.0f;
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }
}

void Weapon::render(SDL_Renderer* renderer) {
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
    SDL_FRect weaponRect = {x, y, 5.0f, 5.0f};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &weaponRect);
}

void Weapon::shoot() {
    if (!canShoot()) return;
    float velX = bulletSpeed * cos(angle);
    float velY = bulletSpeed * sin(angle);
    bullets.emplace_back(x, y, velX, velY);
    lastShotTime = SDL_GetTicks();
}

bool Weapon::canShoot() {
    return (SDL_GetTicks() - lastShotTime >= static_cast<Uint64>(cooldown));
}

std::optional<size_t> Weapon::checkBulletCollision(const SDL_FRect& target) {
    for (size_t i = 0; i < bullets.size(); i++) {
        SDL_FRect bulletRect = bullets[i].getHitBox();
        SDL_FRect result;
        if (SDL_GetRectIntersectionFloat(&bulletRect, &target, &result)) {
            return i;
        }
    }
    return std::nullopt;
}

void Weapon::destroyBullet(size_t index) {
    if (index < bullets.size()) {
        bullets.erase(bullets.begin() + index);
    }
}
