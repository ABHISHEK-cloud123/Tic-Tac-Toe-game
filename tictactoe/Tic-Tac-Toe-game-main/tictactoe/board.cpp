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

static std::string subBoardRow(const std::vector<char>& mainBoard,
                               const std::vector<std::vector<char>>& subBoards,
                               int bIdx, int subRow, int activeBoard) {
    char owner = mainBoard[bIdx];
    if (owner == 'X') {
        const char* art[3] = {" \\ / ", "  X  ", " / \\ "};
        return std::string(" ") + CYAN + art[subRow] + RESET + " ";
    }
    if (owner == 'O') {
        const char* art[3] = {" /-\\ ", " | | ", " \\-/ "};
        return std::string(" ") + YELLOW + art[subRow] + RESET + " ";
    }
    if (owner == 'D') {
        const char* art[3] = {" --- ", " DRW ", " --- "};
        return std::string(" ") + RED + art[subRow] + RESET + " ";
    }

    bool isActive = (activeBoard == -1 || activeBoard == bIdx);
    std::string out = " ";
    for (int col = 0; col < 3; col++) {
        char c = subBoards[bIdx][subRow * 3 + col];
        if (c == 'X' || c == 'O') {
            out += colorize(c);
        } else if (isActive) {
            out += std::string(GREEN) + c + RESET;
        } else {
            out += c;
        }
        if (col < 2) out += ' ';
    }
    return out + " ";
}

void drawUltimateBoard(const std::vector<char>& mainBoard,
                       const std::vector<std::vector<char>>& subBoards,
                       int activeBoard) {
    clearScreen();
    std::cout << "\n    === ULTIMATE TIC-TAC-TOE ===\n\n";

    for (int mainRow = 0; mainRow < 3; mainRow++) {
        for (int subRow = 0; subRow < 3; subRow++) {
            std::cout << "     ";
            for (int mainCol = 0; mainCol < 3; mainCol++) {
                int bIdx = mainRow * 3 + mainCol;
                std::cout << subBoardRow(mainBoard, subBoards, bIdx, subRow, activeBoard);
                if (mainCol < 2) std::cout << "|";
            }
            std::cout << "\n";
        }
        if (mainRow < 2)
            std::cout << "     -------+-------+-------\n";
    }

    std::cout << "\n  Active Board: ";
    if (activeBoard == -1) {
        std::cout << GREEN << "ANY OPEN BOARD (1-9)" << RESET << "\n\n";
    } else {
        std::cout << GREEN << "#" << (activeBoard + 1) << RESET << "\n\n";
    }
}
