#include "weapon.hpp"
#include "createwindow.hpp"
Bullet::Bullet(float startX, float startY, float velX, float velY)
    : x(startX), y(startY), velocityX(velX), velocityY(velY), active(true) {};

void Bullet::render(SDL_Renderer* renderer) {
    SDL_Rect bulletRect = {static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Color: White
    SDL_RenderFillRect(renderer, &bulletRect);
}
void Bullet::update(float deltaTime) {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
};

bool Bullet::isOffScreen() const {
    return (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT);
}
SDL_Rect Bullet::getHitBox() const {
    return {static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT};
}
bool Bullet::isActive() const {
    return active;
}
void Bullet::deactivate() {
    active = false;
};

////////////////////////// WEAPON  Impls///////////////////////////////////
Weapon::Weapon() : x(0), y(0), cooldown(350.0f), lastShotTime(300), bulletSpeed(600.0f), angle(-M_PI / 2) {}

void Weapon::update(float playerX, float playerY) {
    this->x = playerX;
    this->y = playerY;

    auto oldSize = bullets.size();
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                            [](const Bullet& bullet) {
                                bool should_remove = bullet.isOffScreen() || !bullet.isActive();
                                // printf("Checking bullet - OffScreen: %d, "
                                //        "Active: %d, Should Remove: %d\n",
                                //        bullet.isOffScreen(),
                                //        bullet.isActive(), should_remove);
                                return should_remove;
                            }),
                  bullets.end());

    float deltaTime = 1.0f / 60.0f;
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }
}
void Weapon::render(SDL_Renderer* renderer) {
    // render bullets
    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }

    // Optionally render the weapon itself
    SDL_Rect weaponRect = {static_cast<int>(x), static_cast<int>(y), 5, 5};
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
bool Weapon::canShoot() {
    return (SDL_GetTicks() - lastShotTime >= cooldown);
}
std::optional<size_t> Weapon::checkBulletCollision(const SDL_Rect& target) {
    for (size_t i = 0; i < bullets.size(); i++) {
        SDL_Rect bulletRect = bullets[i].getHitBox();
        if (SDL_HasIntersection(&bulletRect, &target)) {
            return i; // Return the index as an optional
        }
    }
    return std::nullopt; // Return empty optional instead of -1
}
void Weapon::destroyBullet(size_t index) {
    if (index < bullets.size()) {
        bullets.erase(bullets.begin() + index); // Directly erase instead of deactivating
    }
}
