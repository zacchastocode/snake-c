# Snake Game in C with Raylib

A classic Snake game implementation in C using the Raylib library. Control the snake with arrow keys, eat food to grow, and avoid hitting walls or yourself!

## Game Features

- Grid-based movement system
- Arrow key controls for direction changes
- Automatic snake movement
- Random food spawning
- Collision detection (walls and self-collision)
- Score tracking
- Game over and restart functionality
- Visual grid display

## Prerequisites

Before building the game, you need to install the Raylib library on your system.

### macOS

Using Homebrew:
```bash
brew install raylib
```

### Ubuntu/Debian

Using apt package manager:
```bash
sudo apt update
sudo apt install libraylib-dev
```

### Fedora/RHEL

Using dnf package manager:
```bash
sudo dnf install raylib-devel
```

### Other Systems

For other operating systems or manual installation, please refer to the [official Raylib installation guide](https://github.com/raysan5/raylib#build-and-installation).

## Build Instructions

To build the game, simply run:
```bash
make
```

This will compile `main.c` and create an executable named `snake`.

To clean up build artifacts:
```bash
make clean
```

## Run Instructions

After building, run the game with:
```bash
./snake
```

## How to Play

- **Arrow Keys**: Change the snake's direction (Up, Down, Left, Right)
- **Objective**: Eat the red food to grow your snake and increase your score
- **Avoid**: Don't hit the walls or your own body, or it's game over!
- **Restart**: Press SPACE or ENTER after game over to play again

## Controls

- `↑` Up Arrow - Move up
- `↓` Down Arrow - Move down
- `←` Left Arrow - Move left
- `→` Right Arrow - Move right
- `SPACE` or `ENTER` - Restart after game over

## Game Rules

1. The snake starts in the center of the screen with a length of 3 segments
2. The snake moves automatically in the current direction
3. Eating food (red squares) increases your score by 10 points and grows the snake by 1 segment
4. Hitting a wall ends the game
5. Hitting your own body ends the game
6. The game can be restarted after a game over

## Technical Details

- **Screen Resolution**: 800x600 pixels
- **Grid Size**: 20x20 pixels per cell
- **Programming Language**: C (C99 standard)
- **Graphics Library**: Raylib
- **Target FPS**: 60 FPS

## License

This is a simple educational project. Feel free to use and modify as needed.
