#include "sounds.h"
#include <windows.h>

void playMoveSound(bool isAI) {
    Beep(isAI ? 480 : 650, 70);
}

void playInvalidSound() {
    Beep(200, 180);
}

void playWinSound() {
    Beep(523, 100);
    Beep(659, 100);
    Beep(784, 150);
    Beep(1046, 250);
}

void playLoseSound() {
    Beep(400, 150);
    Beep(350, 150);
    Beep(300, 300);
}

void playDrawSound() {
    Beep(440, 120);
    Sleep(40);
    Beep(440, 120);
}

void playUltimateIntroSound() {
    Beep(330, 100);
    Beep(440, 100);
    Beep(554, 120);
    Beep(659, 220);
}

void playBattleMoveSound(bool isAI) {
    Beep(isAI ? 380 : 740, 55);
    Beep(isAI ? 320 : 880, 45);
}

void playBoardCaptureSound() {
    Beep(587, 90);
    Beep(740, 90);
    Beep(988, 180);
}
