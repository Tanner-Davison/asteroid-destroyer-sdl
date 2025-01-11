# SDL Window Program

## Project notes for self

# Steps Of Rendering Something

#### please email me if you know a better way <ins>tanner.davison95@gmail.com<ins>

## Step 1: Create Surface

```cpp
cppCopySDL_Surface* surface = TTF_RenderText_Solid(font, "Text", color);
// or
SDL_Surface* surface = IMG_Load("image.png");
```

## Step 2: Create Texture from Surface

```cpp
cppCopySDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
```

## Define the Destination Rectangle (where to render)

```cpp
cppCopySDL_Rect destRect = { x, y, width, height };
```

## Step 3: Render to Screen

```cpp
cppCopySDL_RenderCopy(renderer, texture, NULL, &destRect);
```

## Cleanup to Prevent Memory Leaks

```cpp
cppCopySDL_FreeSurface(surface);   // Free surface first
SDL_DestroyTexture(texture);  // Then destroy texture
```

## POSITION VARS

x: Players x-coordinets
y: Players y-coordinates

velocityX: Players speed along the x-axis
velocityY: Players speed along the y-axis

P(x) = centerX + (i - (n/2)) _ spacing
P(y) = bottomY - |i - (n/2)| _ vertical_offset
Where:

**n** is the total count of players
**i** is the current player index (0 to n-1)
| | represents absolute value

This is essentially a linear function for x-coordinates combined with an absolute value function for y-coordinates,
centered around the middle index. It creates a symmetric V shape because the absolute value function creates the same y-offset for positions equidistant from the center,
while the x-coordinates spread outward linearly.

## MOVEMENT

speed: the base speed of the player

acceleration: How quick the player can change speed

maxSpeed: the max speed the player can reach

isMoving: A boolean variable to track whether the player is currently moving

facingDirection: A variable to track the direction the player is facing, can be an angle, a vector, or a enum for representing

directions like "Up" , "Down", "Left" and "Right".

angle: current rotation angle in degrees

rotationSpeed: degrees per frame

# Used with Neovim

This project is a basic C++ game setup using SDL2 for graphics, input handling, and window management. The project is configured with CMake for building and Visual Studio for compiling on Windows.

# Compilation Notes for Self

clang looks for compilation databases in this order:

1. compile_commands.json
2. compile_flags.txt
3. .clangd\ configuration file

## Prerequisites

Before you can build and run this project, you need to have the following installed:

- **SDL2** library:
  - Download SDL2 from [SDL2 Downloads](https://www.libsdl.org/download-2.0.php) or use a package manager to install it (e.g., vcpkg, or brew on macOS).
  - SDL2.dll must be placed in the `bin` directory for the project to run properly.

## How

## Setup

### Clone the Repository

First, clone the repository to your local machine:

```bash
git clone https://github.com/Tanner-Davison/sdl-first-game.git
cd sdl-first-game
Install SDL2
```

Make sure that you have the SDL2 library installed. You can download it from the official SDL2 website. [or here](https://github.com/libsdl-org/SDL/releases)

If you are using vcpkg to manage dependencies, you can install SDL2 with the following command:

```bash
vcpkg install sdl2
Make sure to link the SDL2 include and lib directories properly in your project if you're not using a package manager.
```

Build the Project
Generate CMake build files:

Open the project folder in your terminal and run:

```bash
cmake -S . -B build
```

Build the project:

After generating the build files, run:

```bash
cmake --build build
```

This will compile your project using Visual Studio's build system.

Running the Project
After building the project, you can run the executable. Depending on your setup, run the following command in your terminal:

On Windows (using Visual Studio):

```bash
.\build\Debug\sdl-first-game.exe
```

```bash
# On Linux/macOS (or a Unix-like system):
./build/sdl-first-game
```

Ensure that the SDL2.dll file is in the same folder as the executable on Windows, or ensure that the necessary SDL2 shared libraries are available on Unix-like systems.

Code Structure

CMakeLists.txt: CMake build configuration.

src/main.cpp: The main C++ entry point of the application.

include/: Folder where header files (if any) are placed.

lib/: Folder containing the SDL2 libraries.

bin/: Folder where SDL2.dll is placed for runtime.

Notes
The project is configured to use SDL2 with the C++ standard set to C++23.
The #define SDL_MAIN_HANDLED is used to prevent SDL2 from overriding the main function.

License
MIT
