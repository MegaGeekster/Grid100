#include "GameState.h"

GameState::GameState()
    :currentNumber(1)
{
    // Initialize grid
    grid.resize(10);
    for (auto &row : grid) {
        row.resize(10, 0);
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

bool GameState::placeNumber(QPoint pos)
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
    if(currentNumber > 100)
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
    grid.assign(10, std::vector<int>(10, 0));
    currentNumber = 1;
    hasStarted = false;
    gameOver = false;
}
