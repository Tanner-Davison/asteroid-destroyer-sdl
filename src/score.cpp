#include "score.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

Score::Score()
    : score(0), textWidth(0), textHeight(0), x(50), y(50) {
    messageToRender = "Score: 0";
}

Score::~Score() {}

void Score::setScore(int newScore) {
    score += newScore;
    messageToRender = "Score: " + std::to_string(score);
}

void Score::renderScore(SDL_Renderer* renderer) {
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, messageToRender.c_str(), 0, textColor);
    if (!textSurface) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Unable to create texture from text! SDL Error: %s\n", SDL_GetError());
        SDL_DestroySurface(textSurface);
        return;
    }

    textWidth = textSurface->w;
    textHeight = textSurface->h;
    SDL_DestroySurface(textSurface);

    SDL_FRect renderQuad = {
        static_cast<float>(x), static_cast<float>(y),
        static_cast<float>(textWidth), static_cast<float>(textHeight)
    };

    SDL_RenderTexture(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}
