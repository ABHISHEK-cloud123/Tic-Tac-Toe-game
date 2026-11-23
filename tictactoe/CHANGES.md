# Tic-Tac-Toe — Code Breakdown & Changes

## Project Structure

```
tictactoe/
├── main.cpp       — entry point, game loop
├── board.h/.cpp   — screen clearing, token coloring, board rendering
├── logic.h/.cpp   — win detection, AI move selection
├── display.h/.cpp — typewriter effect, end-of-game screen
├── colors.h       — ANSI color code definitions
├── Makefile        — build instructions
└── CHANGES.md     — this file
```

---

## File-by-File Breakdown

### `colors.h`
Holds the six ANSI escape code macros (`RESET`, `CYAN`, `YELLOW`, `GREEN`, `RED`, `MAGENTA`).
Pulled out of the main file so any source that needs colors just does `#include "colors.h"` —
no duplication, one place to tweak if you want to change a shade.

### `board.h` / `board.cpp`
Three functions that handle everything visual about the board:

| Function | What it does |
|---|---|
| `clearScreen()` | Sends the ANSI escape to wipe the terminal and reset the cursor. |
| `colorize(token)` | Returns the token character wrapped in its color — `X` in cyan, `O` in yellow, numbers as-is. |
| `drawBoard(board)` | Clears the screen and prints the 3×3 grid using a loop instead of three hardcoded rows. |

### `logic.h` / `logic.cpp`
All the game rules and the AI brain:

| Function | What it does |
|---|---|
| `isOccupied(board, pos)` | Returns `true` if a cell already has an `X` or `O`. Replaces the repeated `board[x] != 'X' && board[x] != 'O'` checks from the original. |
| `checkWin(board, player)` | Loops the 8 win lines (rows, columns, diagonals) to see if a player has three in a row. |
| `findWinningMove(board, player)` | Scans win lines for a line where the player has 2 out of 3 filled and the third is empty — returns that empty cell index, or -1. |
| `getAIMove(board, ai, human)` | Priority chain: win → block → center → random corner → anything open. |

The `WIN_LINES` array lives in the header as a `static const` so both `checkWin` and `findWinningMove` reference one copy instead of each declaring their own (which the original did).

### `display.h` / `display.cpp`
The fun stuff — effects and the game-over sequence:

| Function | What it does |
|---|---|
| `typewriter(text, delayMs)` | Prints text one character at a time with a delay between each. |
| `showEndScreen(winner, aiWon)` | Flashes "GAME OVER", shows the ASCII art (robot face if AI won, trophy if human won), then prints a random programmer joke with the typewriter effect. |

### `main.cpp`
The entry point — handles mode selection, the turn loop, input validation, and calling the other modules. Kept lean: ~55 lines, no game logic mixed in.

---

## What Changed (and Why)

### 1. Split into multiple files
**Before:** Everything in one ~170-line file.
**After:** 8 files, each under 70 lines.
**Why:** Easier to navigate, test, and modify individual parts. Also looks like how someone would naturally grow a project — you start with one file, then split when it gets unwieldy.

### 2. Killed the over-commenting
**Before:** Every function had a sentence-long comment above it explaining exactly what it does:
```cpp
// Function to simulate typing text character by character
// Renders the Tic-Tac-Toe board matrix cleanly with colored tokens
// Verifies if there is a winner
```
**After:** Comments only where the *why* isn't obvious. Stuff like `// flash effect` or `// block opponent` — short, to the point. The function name should do most of the talking.
**Why:** Excessive "this function does X" comments on every function is a dead giveaway of AI-generated code. Human devs comment the tricky or non-obvious parts.

### 3. Removed the `/**** ... ****/` header block
**Why:** Big decorative comment borders at the top of a file are another AI pattern. Real projects either have a license header or nothing.

### 4. Renamed functions to be shorter/more natural
| Original | New | Reason |
|---|---|---|
| `formatToken()` | `colorize()` | Shorter, more direct — it colorizes a token |
| `typewriterPrint()` | `typewriter()` | The "print" is implied |
| `triggerSurprise()` | `showEndScreen()` | Says what it actually does |

### 5. Extracted `isOccupied()` helper
**Before:** The check `board[x] != 'X' && board[x] != 'O'` was copy-pasted in `findWinningMove()`, `getAIMove()`, and `main()`.
**After:** One `isOccupied()` function used everywhere.
**Why:** DRY principle. Also makes the intent clearer when reading the code.

### 6. Deduplicated `WIN_LINES`
**Before:** The 8-element win conditions array was declared separately inside both `checkWin()` and `findWinningMove()`.
**After:** Declared once in `logic.h` as `static const`, shared by both.
**Why:** One source of truth. If you ever change the board size, you update one place.

### 7. Used a loop for board rendering
**Before:** Three manually written rows of `cout` statements.
**After:** A `for` loop over rows 0-2.
**Why:** Less repetition, easier to modify if the board changes. A human writing this iteratively would likely start hardcoded and then loop-ify it.

### 8. Toned down the in-game text
| Before | After |
|---|---|
| `"Invalid move! Press Enter to try again..."` | `"Nope, bad move. Hit Enter and try again..."` |
| `"You conquered the code!"` | `"You won!"` |
| `"It's a tie game! Software engineering wins this round."` | `"It's a draw! Nobody wins this round."` |
| `"Enter choice (1 or 2): "` | `"Pick (1/2): "` |

**Why:** Overly polished, formal phrasing in a hobby project feels generated. Casual language fits better.

### 9. Chained `cout` statements
**Before:** Each line of ASCII art was a separate `std::cout << ... << "\n";` statement.
**After:** Chained with `<<` into one expression.
**Why:** More natural C++ style and reduces visual noise.

### 10. Added a Makefile
**Why:** Any C++ project with more than one source file needs a build system. A simple Makefile is the most natural choice for a small project like this.

---

## Building & Running

```bash
# with make
make
./tictactoe

# or manually
g++ -std=c++17 -Wall -o tictactoe main.cpp board.cpp logic.cpp display.cpp
./tictactoe

# on Windows with g++
g++ -std=c++17 -Wall -o tictactoe.exe main.cpp board.cpp logic.cpp display.cpp
tictactoe.exe
```
