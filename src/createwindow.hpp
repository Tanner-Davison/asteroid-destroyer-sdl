#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <random>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

bool init();
bool loadMedia(const char* path);
void close();

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* font;

namespace GameRNG {
extern std::random_device rd;
extern std::mt19937 gen;
}
