# Snake Game in C

A classic Snake game implementation in C using the Raylib library.

## Description

This is a simple Snake game where you control a snake that grows longer as it eats food. The game features:
- Grid-based movement system
- Automatic snake movement with arrow key controls
- Random food spawning
- Collision detection (walls and self-collision)
- Score tracking
- Game over and restart functionality

## Prerequisites

You need to have Raylib installed on your system to build and run this game.

### Installing Raylib

**macOS:**
```bash
brew install raylib
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install libraylib-dev
```

**Fedora:**
```bash
sudo dnf install raylib-devel
```

**Arch Linux:**
```bash
sudo pacman -S raylib
```

For other platforms or manual installation, visit: https://github.com/raysan5/raylib

## Build Instructions

To build the game, simply run:
```bash
make
```

This will compile the source code and create an executable named `snake`.

To clean build artifacts:
```bash
make clean
```

## Run Instructions

After building, run the game with:
```bash
./snake
```

## How to Play

- Use **Arrow Keys** to control the snake's direction
- Eat the red food to grow longer and increase your score
- Avoid hitting the walls or your own body
- Press **SPACE** to restart after game over
- Close the window or press **ESC** to quit

## Controls

- `↑` - Move Up
- `→` - Move Right
- `↓` - Move Down
- `←` - Move Left
- `SPACE` - Restart (after game over)
- `ESC` - Quit game

## License

This project is open source and available for educational purposes.
