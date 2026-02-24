# Vendor Dependencies

This directory contains third-party libraries and dependencies.

## Expected Dependencies

- SDL2 - Window management, input, audio
- OpenGL/GLEW - Graphics rendering
- pthreads - Threading (POSIX systems)

## Installation

Dependencies are managed via system package managers:

### macOS (Homebrew)
```bash
brew install sdl2 glew
```

### Linux (apt)
```bash
sudo apt-get install libsdl2-dev libglew-dev
```

### Windows
Download SDL2 and GLEW development libraries and place in vendor/
