#include "GameState.h"

GameState::GameState()
    : currentNumber(1)
    , gridSize(10)
{
    // Initialize grid
    grid.resize(gridSize);
    for (auto &row : grid) {
        row.resize(gridSize, 0);
    }
}

std::vector<QPoint> GameState::getLegalMoves()
{
    if(currentNumber == 1)
    {
        return {};
    }

    // Return moves
    std::vector<QPoint> nextMoves;

    for(const auto &move : moves)
    {
        // Calculate move
        int32_t x = currentPos.x() + move.x();
        int32_t y = currentPos.y() + move.y();

        // Validate bounds
        if(x >= 0 && y >= 0 && x < grid.size() && y < grid.size() && grid[x][y] == 0)
        {
            nextMoves.push_back(QPoint{x, y});
        }
    }

    return nextMoves;
}

bool GameState::placeNumber(const QPoint &pos)
{
    if (!hasStarted)
    {
        hasStarted = true;
    }
    // Set and update number
    grid[pos.x()][pos.y()] = currentNumber;
    currentNumber++;

    currentPos = pos;
    return true;
}

bool GameState::isGameOver()
{
    int finalNumber = gridSize * gridSize;
    if(currentNumber > finalNumber)
    {
        return true;
    }

    if(currentNumber == 1)
    {
        return false;
    }

    std::vector<QPoint> legalMoves = getLegalMoves();
    if(legalMoves.empty())
    {
        return true;
    }

    return false;
}

void GameState::reset()
{
    grid.assign(gridSize, std::vector<int>(gridSize, 0));
    currentNumber = 1;
    hasStarted = false;
    gameOver = false;
}

void GameState::undoLastMove(const QPoint &lastPosition, const QPoint &newPosition)
{
    grid[lastPosition.x()][lastPosition.y()] = 0;
    // Set current number to last number
    currentNumber--;
    // Set current position to previous position
    currentPos = newPosition;
}
