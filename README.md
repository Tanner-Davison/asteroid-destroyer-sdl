<h1 align="center"> Asteroids Destroyer an SDL Game. </h1>

<p align="center"> Welcome, This is my first Game Project using SDL's Subsystem and my best first attempt at creating a 2D game! </p>
<p align="center">Created By: Tanner Davison</p>
<p align="center" >tanner.davison95@gmail.com</p>
<p align="center"><em>Now compatible on Mac and or Windows Desktop!</em></p>
<br/>
<p>Please email me if you know any patterns that could assist in this project or if you would like to share any tips and tricks I could use to improve my SDL compatibility</p>
</br>

# Steps Of Rendering In Game Objects

#### _or even better send a pr with your changes!_

### Step 1: Create Surface

```cpp
cppCopySDL_Surface* surface = TTF_RenderText_Solid(font, "Text", color);
// or
SDL_Surface* surface = IMG_Load("image.png");
```

### Step 2: Create Texture from Surface

```cpp
cppCopySDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
```

### Define the Destination Rectangle (where to render)

```cpp
cppCopySDL_Rect destRect = { x, y, width, height };
```

### Step 3: Render to Screen

```cpp
cppCopySDL_RenderCopy(renderer, texture, NULL, &destRect);
```

### Cleanup to Prevent Memory Leaks

```cpp
SDL_DestroyTexture(texture);      // Then destroy texture
```

## Updating Player Position functions(quite a lot)

```cpp
//main() ->
    //...

    while (accumulator >= FIXED_TIME_STEP) {
      for (auto &player : players) {
        player->handlePlayerInputAndPosition(keyState); //HERE!
      }

    //...
```

### handlePlayerInputAndPosition

````cpp
void Player::handlePlayerInputAndPosition(const Uint8 *keyState) {
  handleInput(keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP],
              keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN],
              keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT],
              keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT],
              keyState[SDL_SCANCODE_SPACE], keyState[SDL_SCANCODE_LSHIFT]);
}

```cpp
void Player::handlePlayerInputAndPosition(const Uint8 *keyState) {
  const bool up = keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP],
             down = keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN],
             left = keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT],
             right = keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT],
             boost = keyState[SDL_SCANCODE_LSHIFT],
             isShooting = keyState[SDL_SCANCODE_SPACE];

  ACCELERATION = boost ? BOOST_ACCELERATION : BASE_ACCELERATION;
  float CurrentMaxVelocity = boost ? MAX_VELOCITY * 1.5f : MAX_VELOCITY;

  // Movement handling
  if (right) {
    velocityX = std::min(velocityX + ACCELERATION, CurrentMaxVelocity);
  } else if (left) {
    velocityX = std::max(velocityX - ACCELERATION, -CurrentMaxVelocity);
  } else {
    velocityX *= DECELERATION;
    if (abs(velocityX) < 0.3f) {
      velocityX = 0;
    }
  }

  if (down) {
    velocityY = std::min(velocityY + ACCELERATION, CurrentMaxVelocity);
  } else if (up) {
    velocityY = std::max(velocityY - ACCELERATION, -CurrentMaxVelocity);
  } else {
    velocityY *= DECELERATION;
    if (abs(velocityY) < 0.3f) {
      velocityY = 0;
    }
  }

  if (isShooting) {
    weapon.shoot();
  }

  float nextX = rectXf + velocityX;
  float nextY = rectYf + velocityY;
    //THIS GETS CALLED NEXT
  handleBoundsAndUpdatePosition(nextX, nextY);
}
````

### handleBoundsAndUpdatePosition(nextX, nextY);

```cpp
void Player::handleBoundsAndUpdatePosition(float nextX, float nextY) {
  const bool hitLeftWall = nextX <= 0,
             hitRightWall = nextX >= SCREEN_WIDTH - rectWidth,
             hitTopWall = nextY <= 0,
             hitBottomWall = nextY >= SCREEN_HEIGHT - rectHeight;

  if (hitLeftWall) {
    rectXf = 0.0f;
    velocityX = -velocityX * 1.8f;

  } else if (hitRightWall) {
    rectXf = SCREEN_WIDTH - rectWidth;
    velocityX = -velocityX * 1.8f;
  } else {
    rectXf = nextX;
  }

  if (hitTopWall) {
    rectYf = 0.0f;
    velocityY = -velocityY * 1.8f;

  } else if (hitBottomWall) {
    rectYf = SCREEN_HEIGHT - rectHeight;
    velocityY = -velocityY * 1.8f;

  } else {
    rectYf = nextY;
  }

  // update Position
  rectX = static_cast<int>(rectXf);
  rectY = static_cast<int>(rectYf);
}
```

## Specific vars description to position

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

This project is a basic C++ game setup using SDL2 for graphics, input handling, and window management. The project is configured with CMake for building and g++ for compiling on Windows.

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

**CMakeLists.txt:** CMake build configuration.

**src/main.cpp:** The main C++ entry point of the application.

**include/:** Folder where header files (if any) are placed.

**lib/:** Folder containing the SDL2 libraries.

**bin/:** Folder where SDL2.dll is placed for runtime.

The project is configured to use SDL2 with the C++ standard set to C++23.
The #define **SDL_MAIN_HANDLED** is used to prevent SDL2 from overriding the main function.
PLEASE PUT THIS ON TOP TO NOT DRIVE YOURSELF CRAZY DURING COMPILATION
License
MIT
