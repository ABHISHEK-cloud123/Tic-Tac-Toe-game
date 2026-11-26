#pragma once

#include <vector>
#include <string>

void clearScreen();
std::string colorize(char token);
void drawBoard(const std::vector<char>& board);
void drawUltimateBoard(const std::vector<char>& mainBoard,
                       const std::vector<std::vector<char>>& subBoards,
                       int activeBoard);
