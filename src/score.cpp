#include "score.hpp"

Score::Score()
    : score(0), textWidth(0), textHeight(0), x(50),
      y(50) // Position wherever you want it
{
  messageToRender = "Score: 0";
}

Score::~Score() {
  // No need to cleanup string
}

void Score::setScore(int newScore) {
  score += newScore;
  messageToRender = "Score: " + std::to_string(score);
}

void Score::renderScore(SDL_Renderer *renderer) {
  SDL_Color textColor = {255, 255, 255, 255}; // White text

  // Create surface
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(font, messageToRender.c_str(), textColor);
  if (textSurface == nullptr) {
    printf("Unable to render text surface! SDL_ttf Error: %s\n",
           TTF_GetError());
    return;
  }
  // Create texture from surface
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == nullptr) {
    printf("Unable to create texture from rendered text! SDL Error: %s\n",
           SDL_GetError());
    SDL_FreeSurface(textSurface);
    return;
  }

  textWidth = textSurface->w;
  textHeight = textSurface->h;

  // Setup rendering Rect
  SDL_Rect renderQuad = {x, y, textWidth, textHeight};

  // Render
  SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

  // Clean up
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}
