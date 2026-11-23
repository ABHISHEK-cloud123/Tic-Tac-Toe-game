#include "board.h"
#include "colors.h"
#include <iostream>

void clearScreen() {
    std::cout << "\033[H\033[2J" << std::flush;
}

std::string colorize(char token) {
    if (token == 'X') return std::string(CYAN) + "X" + RESET;
    if (token == 'O') return std::string(YELLOW) + "O" + RESET;
    return std::string(1, token);
}

void drawBoard(const std::vector<char>& board) {
    clearScreen();
    std::cout << "\n  === TIC-TAC-TOE ===\n\n";

    for (int row = 0; row < 3; row++) {
        int i = row * 3;
        std::cout << "     " << colorize(board[i]) << " | "
                  << colorize(board[i + 1]) << " | "
                  << colorize(board[i + 2]) << "\n";
        if (row < 2)
            std::cout << "    ---+---+---\n";
    }
    std::cout << "\n";
}
