#include "asteroid.hpp"
#include "createwindow.hpp"
#include <SDL.h>
#include <random>

Asteroid::Asteroid() : rectXf(0), rectYf(0), rectWidth(100), rectHeight(100) {
  std::uniform_int_distribution<> disX(0, SCREEN_WIDTH - rectWidth);
  std::uniform_int_distribution<> disY(0, SCREEN_HEIGHT - rectHeight);

  this->rectX = disX(GameRNG::gen);
  this->rectY = disY(GameRNG::gen);
  this->rectXf = static_cast<float>(rectX);
  this->rectYf = static_cast<float>(rectY);
  // movement handlers on start
  std::uniform_real_distribution<float> velDis(-2.0f, 2.0f);
  velocityX = velDis(GameRNG::gen);
  velocityY = velDis(GameRNG::gen);
  angle = 0.0;
  std::uniform_real_distribution<> rotDis(-2.0, 2.0);
  rotationSpeed = rotDis(GameRNG::gen);
}
Asteroid::Asteroid(const Asteroid &other)
    : rectXf(other.rectXf), rectYf(other.rectYf), rectX(other.rectX),
      rectY(other.rectY), rectWidth(other.rectWidth),
      rectHeight(other.rectHeight), velocityX(other.velocityX),
      velocityY(other.velocityY), angle(other.angle),
      rotationSpeed(other.rotationSpeed), destroyed(other.destroyed) {}

// Copy assignment operator
Asteroid &Asteroid::operator=(const Asteroid &other) {
  if (this != &other) { // Protect against self-assignment
    rectXf = other.rectXf;
    rectYf = other.rectYf;
    rectX = other.rectX;
    rectY = other.rectY;
    rectWidth = other.rectWidth;
    rectHeight = other.rectHeight;
    velocityX = other.velocityX;
    velocityY = other.velocityY;
    angle = other.angle;
    rotationSpeed = other.rotationSpeed;
    destroyed = other.destroyed;
  }
  return *this;
}
Asteroid::~Asteroid() = default;
void Asteroid::renderAsteroid(SDL_Renderer *renderer) {
  SDL_Rect rect = {rectX, rectY, rectWidth, rectHeight};
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Medium gray
  SDL_RenderFillRect(renderer, &rect);
}
void Asteroid::update() {
  // Update position
  rectXf += velocityX;
  rectYf += velocityY;

  // Wrap around screen edges
  if (rectXf < -rectWidth)
    rectXf = SCREEN_WIDTH;
  if (rectXf > SCREEN_WIDTH)
    rectXf = -rectWidth;
  if (rectYf < -rectHeight)
    rectYf = SCREEN_HEIGHT;
  if (rectYf > SCREEN_HEIGHT)
    rectYf = -rectHeight;

  // Update integer coordinates
  rectX = static_cast<int>(rectXf);
  rectY = static_cast<int>(rectYf);

  // Update rotation
  angle += rotationSpeed;
  if (angle > 360)
    angle -= 360;
  if (angle < 0)
    angle += 360;
}

int Asteroid::getRectX() const { return this->rectX; }
int Asteroid::getRectY() const { return this->rectY; }
int Asteroid::getRectWidth() const { return this->rectWidth; }
int Asteroid::getRectHeight() const { return this->rectHeight; }
void Asteroid::destroy() { destroyed = true; }
bool Asteroid::isDestroyed() const { return destroyed; }
