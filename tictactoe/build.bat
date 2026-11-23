@echo off
echo Compiling Tic-Tac-Toe...
g++ -std=c++17 -Wall -o tictactoe.exe main.cpp board.cpp logic.cpp display.cpp

if %errorlevel% == 0 (
    echo Build successful! Run play.bat to start the game.
) else (
    echo Build failed. Make sure g++ is installed and added to PATH.
)
pause
