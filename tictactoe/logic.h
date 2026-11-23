#pragma once

#include <vector>

// all eight ways to win — rows, columns, diagonals
static const int WIN_LINES[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
    {0, 4, 8}, {2, 4, 6}
};

bool isOccupied(const std::vector<char>& board, int pos);
bool checkWin(const std::vector<char>& board, char player);
int findWinningMove(const std::vector<char>& board, char player);
int getAIMove(const std::vector<char>& board, char ai, char human);
