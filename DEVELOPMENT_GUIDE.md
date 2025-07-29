# SDL2 Asteroid Destroyer - Development Guide
please reach out to Tanner Davison / tanner.davison95@gmail.com for collaboration on this repo and to request changes to this guide.
## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture Design](#architecture-design)
3. [Setup Instructions](#setup-instructions)
4. [Development Environment](#development-environment)
5. [Code Organization](#code-organization)
6. [Game Systems](#game-systems)
7. [Best Practices](#best-practices)
8. [Troubleshooting](#troubleshooting)
9. [Cross-Platform Considerations](#cross-platform-considerations)

## Project Overview

The Asteroid Destroyer is a 2D space shooter game built with SDL2, featuring:
- **Multi-player support** with V-formation positioning
- **Physics-based movement** with acceleration and deceleration
- **Weapon system** with bullet collision detection
- **Level progression** with increasing asteroid counts
- **Score system** with visual display
- **Cross-platform compatibility** (Windows & macOS)

### Key Features
- **Fixed timestep physics** (120 FPS simulation, 120 FPS rendering)
- **Smooth player movement** with velocity-based physics
- **Collision detection** between bullets, asteroids, and players
- **Texture loading** and rendering system
- **Font rendering** with TTF support

## Architecture Design

### Core Design Principles

1. **Separation of Concerns**: Each class handles a specific game component
2. **Component-Based Architecture**: Game objects are composed of multiple systems
3. **Fixed Timestep**: Physics updates at 120 FPS regardless of frame rate
4. **Resource Management**: Proper cleanup of SDL resources

### Class Hierarchy

```
Game Engine
├── Window Management (createwindow.cpp/hpp)
├── Player System (Player.cpp/hpp)
│   └── Weapon System (weapon.cpp/hpp)
├── Asteroid System (asteroid.cpp/hpp)
├── Score System (score.cpp/hpp)
└── Main Game Loop (main.cpp)
```

### Data Flow

```
Input → Player Movement → Physics Update → Collision Detection → Rendering
  ↓           ↓              ↓                ↓              ↓
Keyboard → Velocity → Position Update → Hit Detection → Screen Display
```

## Setup Instructions

### Prerequisites

#### macOS
```bash
# Install SDL2 via Homebrew
brew install sdl2 sdl2_image sdl2_ttf cmake

# Verify installation
pkg-config --modversion sdl2
```

#### Windows
1. Download SDL2 development libraries from [SDL2 Downloads](https://www.libsdl.org/download-2.0.php)
2. Extract to project directories:
   - `include/SDL2/` - Header files
   - `lib/x64/` - Library files
   - `bin/` - Runtime DLLs

### Project Structure
```
asteroid-destroyer-sdl/
├── src/                    # Source files
│   ├── main.cpp           # Game loop and initialization
│   ├── Player.cpp/hpp     # Player movement and rendering
│   ├── weapon.cpp/hpp     # Bullet system
│   ├── asteroid.cpp/hpp   # Asteroid physics and rendering
│   ├── score.cpp/hpp      # Score display system
│   ├── createwindow.cpp/hpp # SDL window management
│   └── *.png              # Game assets
├── include/SDL2/          # SDL2 headers
├── lib/x64/              # Windows libraries
├── bin/                  # Windows DLLs
├── assets/fonts/         # Font files
├── CMakeLists.txt        # Build configuration
├── .clangd              # Language server config
├── .clang-format        # Code formatting rules
└── compile_commands.json # Compilation database
```

### Building the Project

#### macOS
```bash
# Create build directory
mkdir cmake-build-debug
cd cmake-build-debug

# Configure and build
cmake ..
make

# Run the game
./asteroid-destroyer-sdl
```

#### Windows
```bash
# Using Visual Studio
cmake -S . -B build
cmake --build build --config Release

# Run the game
.\build\Release\asteroid-destroyer-sdl.exe
```

## Development Environment

### IDE Configuration

#### Neovim Setup
```lua
-- Required plugins
require('lspconfig').clangd.setup({
    cmd = { "clangd", "--background-index" },
    filetypes = { "c", "cpp", "objc", "objcpp" },
})

-- Format on save
vim.api.nvim_create_autocmd("BufWritePre", {
    pattern = { "*.cpp", "*.hpp", "*.c", "*.h" },
    callback = function(args)
        require("conform").format({ bufnr = args.buf })
    end,
})
```

#### Configuration Files

**`.clangd`** - Language server configuration:
```yaml
CompileFlags:
  Add: 
    - "-I${workspaceFolder}/include"
    - "-I${workspaceFolder}/include/SDL2"
    - "-I/opt/homebrew/Cellar/sdl2/2.30.11/include/SDL2"
    - "-std=c++20"
    - "-D__APPLE__"
    - "-DSDL_MAIN_HANDLED"
```

**`.clang-format`** - Code formatting rules:
```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 80
Standard: Cpp17
```

### Essential SDL2 Headers

```cpp
// Always include SDL_MAIN_HANDLED first
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

// Core SDL2 headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
```

## Code Organization

### Header File Structure

#### Standard Include Order
```cpp
// 1. SDL_MAIN_HANDLED definition
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

// 2. Standard library headers
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// 3. SDL2 headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

// 4. Project headers
#include "createwindow.hpp"
#include "Player.hpp"
#include "asteroid.hpp"
#include "score.hpp"
```

### Class Design Patterns

#### Resource Management
```cpp
class Player {
private:
    SDL_Texture* mTexture;  // Raw pointer for SDL resource
    
public:
    ~Player() {
        if (mTexture) {
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
        }
    }
    
    bool loadTexture(const char* path, SDL_Renderer* renderer) {
        // Clean up existing texture
        if (mTexture) {
            SDL_DestroyTexture(mTexture);
            mTexture = nullptr;
        }
        
        // Load new texture
        SDL_Surface* surface = IMG_Load(path);
        if (!surface) return false;
        
        mTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        
        return mTexture != nullptr;
    }
};
```

#### Physics System
```cpp
class Player {
private:
    // Smooth movement with float precision
    float rectXf, rectYf;
    int rectX, rectY;  // Integer position for rendering
    
    // Physics properties
    float velocityX, velocityY;
    const float MAX_VELOCITY = 3.4f;
    const float BASE_ACCELERATION = 0.40f;
    const float DECELERATION = 0.9730f;
    
public:
    void handlePlayerInputAndPosition(const Uint8* keyState) {
        // Update velocity based on input
        if (keyState[SDL_SCANCODE_RIGHT]) {
            velocityX = std::min(velocityX + ACCELERATION, MAX_VELOCITY);
        } else if (keyState[SDL_SCANCODE_LEFT]) {
            velocityX = std::max(velocityX - ACCELERATION, -MAX_VELOCITY);
        } else {
            velocityX *= DECELERATION;  // Apply friction
        }
        
        // Update position
        float nextX = rectXf + velocityX;
        float nextY = rectYf + velocityY;
        handleBoundsAndUpdatePosition(nextX, nextY);
    }
};
```

## Game Systems

### 1. Fixed Timestep Physics

```cpp
const float FIXED_TIME_STEP = 1.0F / 120.0F;  // 8.33ms
float accumulator = 0.0F;

while (!quit) {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    
    accumulator += deltaTime;
    
    // Physics updates at fixed timestep
    while (accumulator >= FIXED_TIME_STEP) {
        updatePhysics();
        accumulator -= FIXED_TIME_STEP;
    }
    
    // Rendering happens every frame
    render();
}
```

### 2. Collision Detection

```cpp
// AABB (Axis-Aligned Bounding Box) collision
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

// Bullet collision with asteroids
std::optional<size_t> checkBulletCollision(const SDL_Rect& target) {
    for (size_t i = 0; i < bullets.size(); i++) {
        SDL_Rect bulletRect = bullets[i].getHitBox();
        if (SDL_HasIntersection(&bulletRect, &target)) {
            return i;
        }
    }
    return std::nullopt;
}
```

### 3. Multi-Player V-Formation

```cpp
std::vector<std::unique_ptr<Player>> createPlayers(int count) {
    int centerX = static_cast<int>(SCREEN_WIDTH / 2);
    int bottomY = static_cast<int>(SCREEN_HEIGHT - 100);
    const int PLAYER_SPACING = 70;
    const int VERTICAL_OFFSET = 50;
    
    std::vector<std::unique_ptr<Player>> players;
    
    if (count == 1) {
        players.push_back(std::make_unique<Player>(centerX, bottomY));
    } else {
        int middleIndex = count / 2;
        for (int i = 0; i < count; ++i) {
            int offsetX = (i - middleIndex) * PLAYER_SPACING;
            int offsetY = std::abs(i - middleIndex) * VERTICAL_OFFSET;
            players.push_back(
                std::make_unique<Player>(centerX + offsetX, bottomY - offsetY));
        }
    }
    
    return players;
}
```

### 4. Weapon System

```cpp
class Weapon {
private:
    std::vector<Bullet> bullets;
    float cooldown = 350.0f;
    Uint32 lastShotTime;
    float bulletSpeed = 600.0f;
    
public:
    void shoot() {
        if (!canShoot()) return;
        
        float velX = bulletSpeed * cos(angle);
        float velY = bulletSpeed * sin(angle);
        bullets.emplace_back(x, y, velX, velY);
        lastShotTime = SDL_GetTicks();
    }
    
    bool canShoot() {
        return (SDL_GetTicks() - lastShotTime >= cooldown);
    }
};
```

## Best Practices

### 1. SDL2 Resource Management

```cpp
// Always check for null pointers
SDL_Surface* surface = IMG_Load(path);
if (!surface) {
    printf("Failed to load image: %s\n", IMG_GetError());
    return false;
}

// Create texture and immediately free surface
SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
SDL_FreeSurface(surface);  // Free immediately after use

if (!texture) {
    printf("Failed to create texture: %s\n", SDL_GetError());
    return false;
}
```

### 2. Error Handling

```cpp
// Initialize SDL subsystems
if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
}

// Initialize SDL_image
int imgFlags = IMG_INIT_PNG;
if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    return false;
}
```

### 3. Performance Optimization

```cpp
// Use const references for large objects
void updateAsteroids(const std::vector<std::unique_ptr<Player>>& players) {
    // Avoid copying large vectors
}

// Pre-allocate vectors when size is known
std::vector<Asteroid> asteroids;
asteroids.reserve(asteroidCount);  // Avoid reallocations

// Use range-based for loops
for (auto& asteroid : asteroids) {
    asteroid.update();
}
```

### 4. Memory Management

```cpp
// Use smart pointers for ownership
std::vector<std::unique_ptr<Player>> players;

// Use RAII for resource management
class TextureManager {
private:
    SDL_Texture* texture;
    
public:
    TextureManager(SDL_Renderer* renderer, const char* path) {
        texture = loadTexture(renderer, path);
    }
    
    ~TextureManager() {
        if (texture) SDL_DestroyTexture(texture);
    }
    
    // Delete copy constructor and assignment
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
};
```

## Troubleshooting

### Common Issues

#### 1. SDL_MAIN_HANDLED Redefinition
```cpp
// Solution: Add guard at the very top
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif
```

#### 2. Missing SDL2 Headers
```bash
# Check if SDL2 is properly installed
pkg-config --cflags --libs sdl2

# Verify include paths in .clangd
CompileFlags:
  Add: 
    - "-I/opt/homebrew/include/SDL2"
```

#### 3. Runtime DLL Errors (Windows)
- Ensure SDL2.dll is in the same directory as the executable
- Check that all required DLLs are present: SDL2.dll, SDL2_image.dll, SDL2_ttf.dll

#### 4. Compilation Database Issues
```bash
# Regenerate compile_commands.json
rm -rf cmake-build-debug
mkdir cmake-build-debug
cd cmake-build-debug
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### Debugging Tips

#### 1. Enable SDL Error Logging
```cpp
SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
SDL_Log("Debug message: %s", SDL_GetError());
```

#### 2. Visual Debugging
```cpp
// Draw collision boxes
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderDrawRect(renderer, &collisionRect);
```

#### 3. Performance Monitoring
```cpp
Uint32 frameStart = SDL_GetTicks();
// ... render code ...
Uint32 frameTime = SDL_GetTicks() - frameStart;
if (frameTime < 16) {  // Cap at ~60 FPS
    SDL_Delay(16 - frameTime);
}
```

## Cross-Platform Considerations

### Platform-Specific Code

```cpp
#ifdef _WIN32
    const float MAX_VELOCITY = 5.4f;
    const float BASE_ACCELERATION = 0.70f;
#else
    const float MAX_VELOCITY = 3.4f;
    const float BASE_ACCELERATION = 0.40f;
#endif
```

### CMake Platform Detection

```cmake
if(APPLE)
    # macOS specific settings
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
elseif(WIN32)
    # Windows specific settings
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
    )
endif()
```

### File Paths

```cpp
// Use forward slashes for cross-platform compatibility
const char* texturePath = "src/spaceship.png";

// Or use platform-specific paths
#ifdef _WIN32
    const char* fontPath = "assets\\fonts\\FiraCode-Regular.ttf";
#else
    const char* fontPath = "assets/fonts/FiraCode-Regular.ttf";
#endif
```

---

## Quick Start Checklist

- [ ] Install SDL2 development libraries
- [ ] Set up CMake build system
- [ ] Configure IDE with clangd
- [ ] Set up code formatting with clang-format
- [ ] Test compilation on target platform
- [ ] Verify asset loading
- [ ] Test game functionality
- [ ] Set up version control

This guide provides a comprehensive foundation for building and maintaining your SDL2 game project. The architecture is designed to be scalable, maintainable, and cross-platform compatible. 
