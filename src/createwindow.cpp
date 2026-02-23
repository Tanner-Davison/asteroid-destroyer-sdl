#include "createwindow.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <print>
#include <unistd.h>

#ifdef _WIN32
const int SCREEN_WIDTH = 1900;
const int SCREEN_HEIGHT = 1200;
#else
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 900;
#endif

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* font = nullptr;

std::random_device GameRNG::rd;
std::mt19937 GameRNG::gen(GameRNG::rd());

bool init() {
    // Set working directory to the executable's location so assets are always
    // found
    const char* basePath = SDL_GetBasePath();
    if (basePath) {
        chdir(basePath);
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::print("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        std::print("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                   SDL_GetError());
        return false;
    }

    font = TTF_OpenFont("FiraCode-Regular.ttf", 24);
    if (!font) {
        std::print("Failed to load font! SDL_ttf Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Asteroid Destroyer", SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        std::print("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, nullptr);
    if (!gRenderer) {
        std::print("Renderer could not be created! SDL Error: %s\n",
                   SDL_GetError());
        return false;
    }

    return true;
}

bool loadMedia(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::print("Unable to load image %s! SDL_image Error: %s\n", path,
                   SDL_GetError());
        return false;
    }

    SDL_Texture* newTexture =
        SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (!newTexture) {
        std::print("Unable to create texture from %s! SDL Error: %s\n", path,
                   SDL_GetError());
        SDL_DestroySurface(loadedSurface);
        return false;
    }

    SDL_DestroySurface(loadedSurface);
    SDL_DestroyTexture(newTexture);
    return true;
}

void close() {
    TTF_CloseFont(font);
    font = nullptr;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    TTF_Quit();
    SDL_Quit();
}
