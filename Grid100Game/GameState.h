#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include <QPoint>
#include <QPushButton>
#include "Cell.h"

class GameState
{
public:
    void setCurrentNumber(const int val) {currentNumber = val;}
    int getCurrentNumber() {return currentNumber;}

    void setCurrentPos(const QPoint &pos) {currentPos = pos;}
    QPoint getCurrentPos() {return currentPos;}

    void setGridSize(const int size){gridSize = size;}
    int getGridSize() {return gridSize;}

    GameState();

    std::vector<QPoint> getLegalMoves(const std::vector<std::vector<Cell>> &grid);
    void placeNumber(const QPoint &pos, Cell *cell);
    bool isGameOver(const std::vector<std::vector<Cell>> &grid);
    void reset();

    void undoLastMove(const QPoint &newPosition, Cell *const lastCell);
    int calculatePercentage();
    std::pair<QString, QString> getGameOverMessage();

    bool gameOver = false;
    bool hasStarted;

private:
    int currentNumber; // Next number to place
    QPoint currentPos; // Last placed number
    const std::vector<QPoint> moves = {
        {3,0}, {-3,0}, {0,3}, {0,-3},
        {2,2}, {2,-2}, {-2,2}, {-2,-2}
    };
    int gridSize;
};

#endif // GAMESTATE_H
