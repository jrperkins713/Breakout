Dependencies (SFML):
 - sudo apt-get install libsfml-dev

1. make main.exe
 - g++ -Wall -Wextra -pedantic -std=c++17 -g main.cpp model.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

2. execute main.exe
 - A window should pop up immediatly, and the game should begin after a short countdown

If the window does not appear, try to run the following commands. This sometimes occured while testing on WSL2.
 - export DISPLAY=:0
 - export LIBGL_ALWAYS_INDIRECT=0