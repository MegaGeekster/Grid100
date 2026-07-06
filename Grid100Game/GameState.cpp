#include "Cell.h"
#include "GameState.h"

GameState::GameState()
    : currentNumber{1}
    , gridSize{10}
{
}

std::vector<QPoint> GameState::getLegalMoves(const std::vector<std::vector<Cell>> &grid)
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
        if(x >= 0 && y >= 0 && x < gridSize && y < gridSize && grid[x][y].number == 0)
        {
            nextMoves.push_back(QPoint{x, y});
        }
    }

    return nextMoves;
}

void GameState::placeNumber(const QPoint &pos, Cell *cell)
{
    if (!hasStarted)
    {
        hasStarted = true;
    }
    // Set and update number
    cell->number = currentNumber;
    currentNumber++;

    currentPos = pos;
}

bool GameState::isGameOver(const std::vector<std::vector<Cell>> &grid)
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

    std::vector<QPoint> legalMoves = getLegalMoves(grid);
    if(legalMoves.empty())
    {
        return true;
    }

    return false;
}

void GameState::reset()
{
    currentNumber = 1;
    hasStarted = false;
    gameOver = false;
}

void GameState::undoLastMove(const QPoint &newPosition, Cell *const lastCell)
{
    lastCell->number = 0;
    // Set current number to last number
    currentNumber--;
    // Set current position to previous position
    currentPos = newPosition;
}

int GameState::calculatePercentage()
{
    int total = gridSize * gridSize;
    int percentage = ((currentNumber - 1) * 100) / total;
    return percentage;
}

std::pair<QString, QString> GameState::getGameOverMessage()
{
    std::pair<QString, QString> message = {"", ""};
    int percentage = calculatePercentage();

    // If won game
    if(percentage == 100)
    {
        message.first = "You Win!";
        message.second = "Unbelievable! You won!\nCongratulations!";
        return message;
    }

    QString textToAppend = "";
    // Get message for percentage range
    if(percentage < 50)
    {
        textToAppend = "Better luck next time...";
    }
    else if(percentage < 70)
    {
        textToAppend = "Getting there...";
    }
    else if(percentage < 85)
    {
        textToAppend = "Nice Job!";
    }
    else if(percentage < 95)
    {
        textToAppend = "Amazing!";
    }
    else if(percentage < 100)
    {
        textToAppend = "Incredible! You are really good at this!";
    }

    // Build message
    message.first = "Game Over";
    message.second = QString("No more moves left.<br>You reached <b>%1</b> (%2%).<br>").arg(currentNumber - 1).arg(percentage) + textToAppend;

    return message;
}

