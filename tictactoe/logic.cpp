#include "logic.h"
#include <cstdlib>

bool isOccupied(const std::vector<char>& board, int pos) {
    return board[pos] == 'X' || board[pos] == 'O';
}

bool checkWin(const std::vector<char>& board, char player) {
    for (auto& line : WIN_LINES) {
        if (board[line[0]] == player &&
            board[line[1]] == player &&
            board[line[2]] == player)
            return true;
    }
    return false;
}

int findWinningMove(const std::vector<char>& board, char player) {
    for (auto& line : WIN_LINES) {
        int count = 0, empty = -1;

        for (int j = 0; j < 3; j++) {
            if (board[line[j]] == player)
                count++;
            else if (!isOccupied(board, line[j]))
                empty = line[j];
        }
        if (count == 2 && empty != -1)
            return empty;
    }
    return -1;
}

int getAIMove(const std::vector<char>& board, char ai, char human) {
    // try to win
    int move = findWinningMove(board, ai);
    if (move != -1) return move;

    // block opponent
    move = findWinningMove(board, human);
    if (move != -1) return move;

    // grab center
    if (!isOccupied(board, 4)) return 4;

    // random open corner
    int corners[] = {0, 2, 6, 8};
    std::vector<int> open;
    for (int c : corners)
        if (!isOccupied(board, c)) open.push_back(c);
    if (!open.empty())
        return open[std::rand() % open.size()];

    // whatever's left
    std::vector<int> remaining;
    for (int i = 0; i < 9; i++)
        if (!isOccupied(board, i)) remaining.push_back(i);
    return remaining[std::rand() % remaining.size()];
}
