#include "board.h"
#include "logic.h"
#include "display.h"
#include "colors.h"
#include "sounds.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

static int readChoice(int minVal, int maxVal) {
    int choice = 0;
    while (true) {
        if (std::cin >> choice && choice >= minVal && choice <= maxVal) {
            return choice;
        }
        playInvalidSound();
        std::cout << "Invalid choice. Please enter " << minVal << " or " << maxVal << ": ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

static void playNormalMode(bool vsAI) {
    // slots labeled 1-9 so the player knows what to type
    std::vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char current = 'X';
    int turns = 0;
    bool won = false;

    while (turns < 9 && !won) {
        drawBoard(board);
        int pick = -1;

        if (vsAI && current == 'O') {
            std::cout << "AI (" << colorize('O') << ") is thinking..." << std::flush;
            Sleep(800);
            pick = getAIMove(board, 'O', 'X') + 1;
        } else {
            std::cout << "Player " << colorize(current) << ", pick a spot (1-9): ";
            std::cin >> pick;

            if (pick < 1 || pick > 9 || isOccupied(board, pick - 1)) {
                playInvalidSound();
                std::cout << "Nope, bad move. Hit Enter and try again...";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cin.get();
                continue;
            }
        }

        board[pick - 1] = current;
        playMoveSound(vsAI && current == 'O');
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
        playDrawSound();
        std::cout << "It's a draw! Nobody wins this round.\n\n";
    }
}

static void playUltimateMode(bool vsAI) {
    std::vector<char> mainBoard = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    std::vector<std::vector<char>> subBoards(
        9, {'1', '2', '3', '4', '5', '6', '7', '8', '9'});

    int activeBoard = -1;
    char current = 'X';
    bool won = false;

    while (!won) {
        drawUltimateBoard(mainBoard, subBoards, activeBoard);
        int targetBoard = activeBoard;
        int targetCell = -1;

        if (vsAI && current == 'O') {
            std::cout << "AI (" << colorize('O') << ") is planning a tactical strike..." << std::flush;
            Sleep(800);
            auto move = getUltimateAIMove(mainBoard, subBoards, activeBoard, 'O', 'X');
            targetBoard = move.first;
            targetCell = move.second;
        } else {
            if (targetBoard == -1 || isMainCellResolved(mainBoard, targetBoard)) {
                std::cout << "Player " << colorize(current) << ", pick an open board (1-9): ";
                int bPick = -1;
                std::cin >> bPick;
                if (bPick < 1 || bPick > 9 || isMainCellResolved(mainBoard, bPick - 1)) {
                    playInvalidSound();
                    std::cout << "Invalid or finished board. Hit Enter and try again...";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cin.get();
                    continue;
                }
                targetBoard = bPick - 1;
            }

            std::cout << "Player " << colorize(current) << ", pick a cell in Board #"
                      << (targetBoard + 1) << " (1-9): ";
            int cPick = -1;
            std::cin >> cPick;
            if (cPick < 1 || cPick > 9 || isOccupied(subBoards[targetBoard], cPick - 1)) {
                playInvalidSound();
                std::cout << "Nope, bad move. Hit Enter and try again...";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cin.get();
                continue;
            }
            targetCell = cPick - 1;
        }

        subBoards[targetBoard][targetCell] = current;

        if (checkWin(subBoards[targetBoard], current)) {
            mainBoard[targetBoard] = current;
            playBoardCaptureSound();
        } else if (isBoardFull(subBoards[targetBoard])) {
            mainBoard[targetBoard] = 'D';
            playBattleMoveSound(vsAI && current == 'O');
        } else {
            playBattleMoveSound(vsAI && current == 'O');
        }

        if (checkWin(mainBoard, current)) {
            won = true;
            showEndScreen(current, vsAI && current == 'O');
            break;
        }

        bool allResolved = true;
        for (int i = 0; i < 9; i++) {
            if (!isMainCellResolved(mainBoard, i)) {
                allResolved = false;
                break;
            }
        }
        if (allResolved) {
            break;
        }

        activeBoard = isMainCellResolved(mainBoard, targetCell) ? -1 : targetCell;
        current = (current == 'X') ? 'O' : 'X';
    }

    if (!won) {
        drawUltimateBoard(mainBoard, subBoards, -1);
        playDrawSound();
        std::cout << "Ultimate stalemate! Nobody claimed the main board.\n\n";
    }
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    SetConsoleOutputCP(CP_UTF8);

    clearScreen();
    std::cout << "=== SELECT GAME TYPE ===\n"
              << "1. Normal Mode\n"
              << "2. " << YELLOW << "Ultimate Mode" << RESET
              << " (9-in-1 Tactical Battle!)\n"
              << "Pick (1/2): ";

    int gameType = readChoice(1, 2);
    bool isUltimate = (gameType == 2);

    if (isUltimate) {
        clearScreen();
        playUltimateIntroSound();
        std::cout << CYAN << "\n  *** ULTIMATE MODE UNLOCKED ***\n\n" << YELLOW;
        typewriter("  A true strategist enters the arena! Respect for choosing Ultimate Mode - prepare for battle!", 25);
        std::cout << RESET << "\n";
        Sleep(900);
    }

    std::cout << "=== GAME MODE ===\n"
              << "1. Player vs Player\n"
              << "2. Player vs AI (" << CYAN << "X = you" << RESET
              << ", " << YELLOW << "O = AI" << RESET << ")\n"
              << "Pick (1/2): ";

    int mode = readChoice(1, 2);
    bool vsAI = (mode == 2);

    if (isUltimate) {
        playUltimateMode(vsAI);
    } else {
        playNormalMode(vsAI);
    }

    return 0;
}
