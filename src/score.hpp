#pragma once
#include "createwindow.hpp"
#include <SDL2/SDL_ttf.h>
#include <string> // Add this for string handling

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
    int getScore() const {
        return score;
    }
};
