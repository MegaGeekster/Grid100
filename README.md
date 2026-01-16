# Grid100
Grid100 is a fun 10×10 number puzzle game built with C++ and Qt. The goal is to fill the grid consecutively with numbers from 1 to 100 following specific movement rules.

# Features
10×10 interactive grid with clickable cells
Highlights legal moves for the next number
Restart button with confirmation dialog
Instructions button showing gameplay rules
Game-over detection with user-friendly message
Windows-compatible release with embedded icons and resources

# Gameplay
1. The game starts with number 1.
2. You can place the next number in a cell that is a legal move (highlighted in light blue).
3. Continue until you reach 100 or no legal moves remain.
4. Use the Restart button to start a new game.
5. Click Instructions for a reminder of the rules.

# Building from Source
Clone the repository:
```
git clone https://github.com/<yourusername>/Grid100.git
cd Grid100
```

Open the project in Qt Creator.
Build in Release mode.
Use windeployqt on the release EXE to prepare a distributable folder.

# License
This project is MIT licensed — free to use and modify.
