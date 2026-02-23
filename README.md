<h1 align="center">Asteroid Destroyer</h1>

<p align="center">A 2D space shooter built from scratch in C++ using SDL3.</p>
<p align="center">Created by <a href="mailto:tanner.davison95@gmail.com">Tanner Davison</a></p>
<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?logo=cplusplus" />
  <img src="https://img.shields.io/badge/SDL-3-brightgreen" />
  <img src="https://img.shields.io/badge/build-CMake-red" />
  <img src="https://img.shields.io/badge/deps-vcpkg-blueviolet" />
</p>

---

## About

Asteroid Destroyer is my first C++ game project: a multi-player space shooter with physics-based movement, a bullet weapon system, level progression, and a live score display. Built entirely with SDL3, CMake, and vcpkg with no bundled libraries.

**Features**
- Multi-player V-formation spawn
- Physics-based movement with acceleration, deceleration, and wall bounce
- Fixed timestep game loop (120Hz physics, 120Hz rendering)
- Bullet weapon system with cooldown and collision detection
- Level progression with increasing asteroid counts
- Live score display with SDL3_ttf font rendering
- Cross-platform — macOS, Linux, Windows

---

## Prerequisites

| Tool | Version |
|------|---------|
| CMake | 3.15+ |
| vcpkg | latest |
| GCC / Clang / MSVC | C++23 support |

### Environment Setup

Before cloning, make sure your system has a C++23 compiler and CMake installed.

**macOS**
```bash
xcode-select --install
brew install cmake
```

**Linux**
```bash
sudo apt install build-essential cmake git
```

**Windows**

Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload. This includes MSVC, CMake, and the Windows SDK.

---

### Install vcpkg

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh   # macOS / Linux
.\bootstrap-vcpkg.bat  # Windows
```

Add to your shell profile (`~/.zshrc`, `~/.bashrc`, or PowerShell `$PROFILE`):

```bash
export VCPKG_ROOT=~/vcpkg
export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

---

## Building

SDL3, SDL3_image, and SDL3_ttf are installed automatically by vcpkg on first configure.

### macOS / Linux

```bash
git clone https://github.com/Tanner-Davison/asteroid-destroyer-sdl.git
cd asteroid-destroyer-sdl
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
make -j$(sysctl -n hw.logicalcpu)   # macOS
make -j$(nproc)                     # Linux
```

### Windows

```powershell
git clone https://github.com/Tanner-Davison/asteroid-destroyer-sdl.git
cd asteroid-destroyer-sdl
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

---

## Running

```bash
./build/asteroid-destroyer      # macOS / Linux
.\build\asteroid-destroyer.exe  # Windows
```

---

## Controls

| Key | Action |
|-----|--------|
| W / ↑ | Move up |
| S / ↓ | Move down |
| A / ← | Move left |
| D / → | Move right |
| Space | Shoot |
| Left Shift | Boost |

---

## Project Structure

```
asteroid-destroyer/
├── src/
│   ├── main.cpp              # Game loop and collision orchestration
│   ├── Player.cpp/hpp        # Movement, input, texture, collision
│   ├── weapon.cpp/hpp        # Bullet pool and shoot cooldown
│   ├── asteroid.cpp/hpp      # Spawn, physics, wrap-around
│   ├── score.cpp/hpp         # Score state and TTF rendering
│   └── createwindow.cpp/hpp  # SDL3 init, window, renderer
├── assets/fonts/             # FiraCode font
├── CMakeLists.txt
└── vcpkg.json
```

---

## License

MIT
