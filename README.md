# Tetris Console Game

A simple Tetris game implemented in C++ using the console for display and interaction.

## Features
- Classic Tetris gameplay with seven Tetrominoes (I, O, T, S, Z, J, L).
- Smooth movement and rotation.
- Automatic downward movement with increasing speed.
- Line clearing and scoring system.
- Game over and restart functionality.
- Simple ASCII-based rendering in the console.

## Controls
- **Left Arrow (←)**: Move piece left
- **Right Arrow (→)**: Move piece right
- **Down Arrow (↓)**: Move piece down faster
- **Up Arrow (↑)**: Rotate piece
- **X**: Restart game after Game Over
- **Q**: Quit game

## How to Run
1. Ensure you have a C++ compiler (such as MinGW for Windows).
2. Copy the source code into a `.cpp` file (e.g., `tetris.cpp`).
3. Open a terminal/command prompt and navigate to the file location.
4. Compile the code:
   ```sh
   g++ -o tetris tetris.cpp -static-libgcc -static-libstdc++
   ```
5. Run the game:
   ```sh
   tetris
   ```
   A working version of the exe file has been uploaded in the releases section.

## Dependencies
- Uses `<conio.h>` and `<windows.h>` (Windows only).
- If using Linux/Mac, you may need to replace `_kbhit()`, `_getch()`, and Windows-specific console functions with platform-independent alternatives.
