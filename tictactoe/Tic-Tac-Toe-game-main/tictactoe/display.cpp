#include "display.h"
#include "board.h"
#include "colors.h"
#include "sounds.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <windows.h>

void typewriter(const std::string& text, int delayMs) {
    for (char c : text) {
        std::cout << c << std::flush;
        Sleep(delayMs);
    }
    std::cout << "\n";
}

void showEndScreen(char winner, bool aiWon) {
    // flash effect
    for (int i = 0; i < 6; i++) {
        clearScreen();
        std::cout << (i % 2 == 0 ? CYAN : YELLOW)
                  << "\n\n   !!! GAME OVER !!!\n\n" << std::flush;
        Sleep(200);
    }
    std::cout << RESET;
    clearScreen();

    if (aiWon) {
        playLoseSound();
        std::cout << RED
                  << "      _______\n"
                  << "     |       |\n"
                  << "     (|- - -|)\n"
                  << "      |  ^  |\n"
                  << "      | === |\n"
                  << "      |_____|\n\n"
                  << RESET << "The AI (" << colorize(winner)
                  << ") outsmarted you this time!\n";
    } else {
        playWinSound();
        std::cout << GREEN
                  << "       ___________      \n"
                  << "      '._==_==_=_.'     \n"
                  << "      .-\\:      /-.    \n"
                  << "     | (|:.     |) |    \n"
                  << "      '-|:.     |-'     \n"
                  << "        \\::.    /      \n"
                  << "         '::. .'        \n"
                  << "           ) (          \n"
                  << "         _.' '._        \n"
                  << "        `-------`       \n\n"
                  << RESET << "Congrats, Player " << colorize(winner)
                  << "! You won!\n";
    }

    // random joke at the end, why not
    std::cout << "Here's a programmer joke for you:\n\n";

    std::vector<std::pair<std::string, std::string>> jokes = {
        {"Why do programmers wear glasses?", "Because they can't C#!"},
        {"How many programmers to change a light bulb?", "None - hardware problem."},
        {"Programmer's favorite hangout?", "Foo Bar."},
        {"Why did the dev quit?", "Didn't get arrays!"}
    };
    int idx = std::rand() % jokes.size();

    std::cout << YELLOW;
    typewriter(jokes[idx].first, 40);
    Sleep(1200);
    std::cout << MAGENTA;
    typewriter(jokes[idx].second, 50);
    std::cout << RESET << "\n";
}
