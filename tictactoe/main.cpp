#include "board.h"
#include "logic.h"
#include "display.h"
#include "colors.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    clearScreen();
    std::cout << "=== GAME MODE ===\n"
              << "1. Player vs Player\n"
              << "2. Player vs AI (" << CYAN << "X = you" << RESET
              << ", " << YELLOW << "O = AI" << RESET << ")\n"
              << "Pick (1/2): ";

    int mode;
    std::cin >> mode;
    bool vsAI = (mode == 2);

    // slots labeled 1-9 so the player knows what to type
    std::vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char current = 'X';
    int turns = 0;
    bool won = false;

    while (turns < 9 && !won) {
        drawBoard(board);
        int pick = -1;

        if (vsAI && current == 'O') {
            // AI turn
            std::cout << "AI (" << colorize('O') << ") is thinking..." << std::flush;
            Sleep(800);
            pick = getAIMove(board, 'O', 'X') + 1;
        } else {
            std::cout << "Player " << colorize(current) << ", pick a spot (1-9): ";
            std::cin >> pick;

            if (pick < 1 || pick > 9 || isOccupied(board, pick - 1)) {
                std::cout << "Nope, bad move. Hit Enter and try again...";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cin.get();
                continue;
            }
        }

        board[pick - 1] = current;
        turns++;

        if (checkWin(board, current)) {
            won = true;
            showEndScreen(current, vsAI && current == 'O');
            break;
        }

        current = (current == 'X') ? 'O' : 'X';
    }

    if (!won) {
        drawBoard(board);
        std::cout << "It's a draw! Nobody wins this round.\n\n";
    }

    return 0;
}
