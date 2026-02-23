#pragma once
#include "createwindow.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

extern TTF_Font* font;

class Score {
private:
    int score;
    int x;
    int y;
    int textWidth;
    int textHeight;
    std::string messageToRender;

public:
    Score();
    ~Score();
    void renderScore(SDL_Renderer* renderer);
    void setScore(int newScore);
    int getScore() const { return score; }
};
