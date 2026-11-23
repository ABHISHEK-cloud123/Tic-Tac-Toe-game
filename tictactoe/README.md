# 🎮 Tic-Tac-Toe (CLI)

A terminal-based Tic-Tac-Toe game written in C++ with colorful output, an AI opponent, and a fun game-over screen.

![Language](https://img.shields.io/badge/Language-C++-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

## Features

- **Player vs Player** — play against a friend on the same machine
- **Player vs AI** — the computer fights back using a basic strategy (center → corners → block → win)
- Color-coded tokens (X in cyan, O in yellow)
- Flashing game-over animation with ASCII art
- Random programmer joke at the end of each game

## How to Build

You need **g++** (MinGW) installed. Then open Command Prompt and run:

```
cd D:\tictactoe
build.bat
```

Or compile manually:

```
g++ -std=c++17 -Wall -o tictactoe.exe main.cpp board.cpp logic.cpp display.cpp
```

## How to Play

After building, just run:

```
play.bat
```

Or directly:

```
tictactoe.exe
```

You'll be asked to pick a game mode, then enter numbers 1–9 to place your token:

```
     1 | 2 | 3
    ---+---+---
     4 | 5 | 6
    ---+---+---
     7 | 8 | 9
```

## Project Structure

```
tictactoe/
├── main.cpp        → game loop and entry point
├── board.h/.cpp    → board rendering and screen utilities
├── logic.h/.cpp    → win detection and AI move selection
├── display.h/.cpp  → end-game effects and typewriter animation
├── colors.h        → ANSI terminal color codes
├── build.bat       → one-click compile script
├── play.bat        → one-click run script
└── README.md       → you are here
```

## Requirements

- Windows 10/11
- g++ (MinGW) — any version that supports C++17
- A terminal that supports ANSI colors (CMD, PowerShell, Windows Terminal)
