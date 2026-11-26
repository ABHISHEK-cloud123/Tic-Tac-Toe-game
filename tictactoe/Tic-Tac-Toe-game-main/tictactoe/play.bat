@echo off
if not exist tictactoe.exe (
    echo Game not built yet. Run build.bat first.
    pause
    exit
)
tictactoe.exe
pause
