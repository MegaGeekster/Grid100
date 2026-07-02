# Grid100
Grid100 is a fun 10×10 number puzzle game built with C++ and Qt. The goal is to fill the grid consecutively with numbers from 1 to 100 following specific movement rules.

<img width="258" height="325" alt="image" src="https://github.com/user-attachments/assets/fe6fffa2-3af9-4c01-964a-76ce528b731e" />

# Features
- 10×10 interactive grid with clickable cells
- Highlights legal moves for the next number
- Restart button with confirmation dialog
- Instructions button showing gameplay rules
- Settings dialog with personalization options
- Game-over detection with user-friendly message
- Windows-compatible release with embedded icons and resources

# Gameplay
1. The game starts with number 1.
2. You can place the next number in a cell that is a legal move (highlighted in light blue)
   - Skipping two cells streight:
   <img width="258" alt="streight_move" src="https://github.com/user-attachments/assets/c48be8ef-fd28-4a5c-a243-5d199060ec8d" />
   
   - Skipping one cell diagnolly:
   <img width="258" alt="diagonal_move" src="https://github.com/user-attachments/assets/b08b8518-1f61-45a5-9d42-7dbc88714132" />
   
4. Continue until you reach 100 or no legal moves remain.
5. Use the Restart button to start a new game.
6. Click Instructions for a reminder of the rules.
7. Use the settings dialog to chose your prefered grid size and theme.
 
   <img width="215" height="203" alt="image" src="https://github.com/user-attachments/assets/189e597b-b4e4-4dd8-b2ef-afec5289daac" />


# Building from Source
Clone the repository:
```
git clone https://github.com/<yourusername>/Grid100.git
cd Grid100
```

Open the project in Qt Creator.
Build in Release mode.
Use windeployqt on the release EXE to prepare a distributable folder.
