# SDL Window Program

# Used with Neovim

This project is a basic C++ game setup using SDL2 for graphics, input handling, and window management. The project is configured with CMake for building and Visual Studio for compiling on Windows.

## Prerequisites

Before you can build and run this project, you need to have the following installed:

- **SDL2** library:
  - Download SDL2 from [SDL2 Downloads](https://www.libsdl.org/download-2.0.php) or use a package manager to install it (e.g., vcpkg, or brew on macOS).
  - SDL2.dll must be placed in the `bin` directory for the project to run properly.

## Setup

### Clone the Repository

First, clone the repository to your local machine:

```bash
git clone https://github.com/your-username/sdl-first-game.git
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

