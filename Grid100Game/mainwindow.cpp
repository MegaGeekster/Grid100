#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Configure the window
    this->setWindowIcon(QIcon(":/icons/Game.ico"));
    this->setWindowTitle("Grid100");

    gameState.reset();
    gridSetup();
    updateGridUI();
    highlightLegalMoves();
}

QString MainWindow::loadInstructions(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "Oops, we can't seem to find the instructions, you'll just have to figure it out";
    }

    QTextStream in(&file);
    return in.readAll();
}

void MainWindow::gridSetup()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);

    // ============ Configure settings button ============
    QPushButton *settingsButton = new QPushButton(this);
    // Set icon
    settingsButton->setIcon(QIcon(":/icons/Settings.png"));
    settingsButton->setIconSize(QSize(24, 24));

    // Connect settings button
    connect(settingsButton, &QPushButton::clicked, this,  &MainWindow::openSettings);

    // ============ Create instructions button ============
    QPushButton *instructionsButton = new QPushButton(this);
    // Set icon
    instructionsButton->setIcon(QIcon(":/icons/Instructions.png"));
    instructionsButton->setIconSize(QSize(24, 24));

    // Connect instructions button
    connect(instructionsButton, &QPushButton::clicked, this, [=](){
        QString text = loadInstructions(":/data/Instructions.txt");
        QMessageBox::information(this, "Game Instructions", text);
    });

    // ============ Create restart button ============
    QPushButton *restartButton = new QPushButton(this);
    // Set icon
    restartButton->setIcon(QIcon(":/icons/Restart.png"));
    restartButton->setIconSize(QSize(24,24));

    // Connect reset Button
    connect(restartButton, &QPushButton::clicked, this, [=](){
        if(!gameState.hasStarted)
        {
            return;
        }
        QMessageBox msg(this);
        msg.setWindowTitle("Restart Game");
        msg.setText("Are you sure you want to restart the game?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.setWindowIcon(QIcon(":/icons/Game.ico"));

        if (msg.exec() == QMessageBox::Yes) {
            restartGame();
        }
    });

    // ============ Create undo button ============
    QPushButton *undoButton = new QPushButton(this);
    // Set icon
    undoButton->setIcon(QIcon(":/icons/Undo.png"));
    undoButton->setIconSize(QSize(24,24));

    // Connect undo button
    connect(undoButton, &QPushButton::clicked, this, [=](){
        undoLastMove();
    });

    // ============ Top bar layout ============
    QHBoxLayout *topBarLayout = new QHBoxLayout();

    // Add Settings button on the top left
    topBarLayout->addWidget(settingsButton);

    // Push buttons to the right
    topBarLayout->addStretch();

    // Add buttons
    topBarLayout->addWidget(undoButton);
    topBarLayout->addWidget(restartButton);
    topBarLayout->addWidget(instructionsButton);

    // Small spacing between buttons
    topBarLayout->setSpacing(6);

    // Add top bar to main layout
    mainLayout->addLayout(topBarLayout);

    // ============ Create a grid layout ============
    boardWidget = new QWidget(ui->centralwidget);
    gridLayout = new QGridLayout(boardWidget);

    // Samll spaces between cells
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(boardWidget, 0, Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    createGridButtons();
}

void MainWindow::createGridButtons()
{
    // Delete old buttons
    if(gridLayout)
    {
        QLayoutItem* child;
        while ((child = gridLayout->takeAt(0)) != nullptr)
        {
            if(QWidget* widget = child->widget()) {
                widget->deleteLater();  // safely delete old buttons
            }
            delete child;
        }
    }

    // Initialize cells list
    const int gridSize = gameState.getGridSize();
    cellButtons.clear();
    cellButtons.resize(gridSize, std::vector<QPushButton*>(gridSize, nullptr));

    // Create cells
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            // Create cell
            QPushButton *cell = new QPushButton(this);
            cellButtons[row][col] = cell;

            // Set size and style
            cell->setFixedSize(32, 32);
            // cell->setAlignment(Qt::AlignCenter);
            cell->setStyleSheet(
                "QPushButton {"
                "border: 1px solid #555;"
                "margin: 0px;"
                "padding: 0px;"
                "}"
                );

            // Connect button
            connect(cell, &QPushButton::clicked, this, [=]() {
                onCellClicked(row, col);
            });

            // Add cell to grid
            gridLayout->addWidget(cell, row, col);
        }
    }
    ui->centralwidget->updateGeometry();
    boardWidget->updateGeometry();
    this->adjustSize();
}

void MainWindow::onCellClicked(int row, int col)
{
    if (gameState.gameOver)
    {
        return;
    }

    QPoint pos(row, col);

    // Get Legal moves
    std::vector<QPoint> legalMoves = gameState.getLegalMoves();

    // Check if cell is a legal move
    bool isLegal = false;
    for(const auto move : legalMoves)
    {
        if(move.x() == row && move.y() == col)
        {
            isLegal = true;
            break;
        }
    }

    if(!isLegal && gameState.getCurrentNumber() != 1)
    {
        return;
    }

    gameState.placeNumber(pos);
    updateGridUI();
    highlightLegalMoves();
    moveHistory.push_back(pos);
    checkGameOver();
}

void MainWindow::checkGameOver()
{
    if(!gameState.hasStarted)
    {
        return;
    }

    std::pair<QString, QString> text = {"", ""};
    int finalNumber = gameState.getGridSize() * gameState.getGridSize();

    // If won the game
    if(gameState.getCurrentNumber() > finalNumber)
    {
        gameState.gameOver = true;
        // Title
        text.first = "You Won!";
        // Message
        text.second = "You did it!.\nUnbelievable!";
    }

    std::vector<QPoint> legalMoves = gameState.getLegalMoves();
    // If no more legal moves
    if(legalMoves.empty())
    {
        gameState.gameOver = true;
        // Title
        text.first = "Game Over";
        // Message
        text.second = text.second = QString("You reached <b>%1</b>!<br>No more legal moves.</br><br>Game over!</br>")
                                        .arg(gameState.getCurrentNumber() - 1);;
    }

    // If game is not over, do nothing
    if(!gameState.gameOver)
    {
        return;
    }

    // Set game over dialog
    QMessageBox msg(this);
    msg.setWindowTitle(text.first);
    msg.setText(text.second);
    msg.setWindowIcon(QIcon(":/icons/Game.ico"));

    QPushButton *undoBtn = msg.addButton("Undo Last Move", QMessageBox::AcceptRole);
    QPushButton *restartBtn = msg.addButton("Restart", QMessageBox::AcceptRole);
    QPushButton *exitBtn = msg.addButton("Exit", QMessageBox::RejectRole);

    msg.setDefaultButton(undoBtn);

    msg.exec();

    if(msg.clickedButton() == undoBtn)
    {
        gameState.gameOver = false;
        undoLastMove();
    }
    else if (msg.clickedButton() == restartBtn)
    {
        restartGame();
    }
    else if (msg.clickedButton() == exitBtn)
    {
        forceExit = true;
        this->close();
    }
}

void MainWindow::highlightLegalMoves()
{
    std::vector<QPoint> legalMoves = gameState.getLegalMoves();
    for(const auto move : legalMoves)
    {
        int r = move.x();
        int c = move.y();
        cellButtons[r][c]->setStyleSheet(
            "QPushButton {"
            "background-color: #ADD8E6;"
            "border: 1px solid #555;"
            "}"
            );
    }
}

void MainWindow::restartGame()
{
    gameState.reset();
    updateGridUI();
}

void MainWindow::set0GridButton(const QPoint point)
{
    cellButtons[point.x()][point.y()]->setText("");
    cellButtons[point.x()][point.y()]->setStyleSheet(
        "QPushButton {"
        "background-color: none;"
        "border: 1px solid #555;"
        "}"
        );
}

void MainWindow::updateGridUI()
{
    // Iterate over grid
    for (int row = 0; row < gameState.getGridSize(); row++)
    {
        for (int col = 0; col < gameState.getGridSize(); col++)
        {
            int value = gameState.grid[row][col];

            if (value == 0)
            {
                set0GridButton(QPoint{row, col});
            }
            else
            {
                cellButtons[row][col]->setText(QString::number(value));
                cellButtons[row][col]->setStyleSheet(
                    "QPushButton {"
                    "background-color: #D3D3D3;"
                    "border: 1px solid #555;"
                    "}"
                    );
            }
        }
    }

    // Highlight current cell
    if(!gameState.hasStarted)
    {
        return;
    }
    QPoint currentCell = gameState.getCurrentPos();
    cellButtons[currentCell.x()][currentCell.y()]->setStyleSheet(
        "QPushButton {"
        "background-color: #D5FFFF;"
        "border: 1px solid #555;"
        "}"
        );
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // If game over, don't ask for conformation
    if (forceExit)
    {
        event->accept();
        return;
    }

    // Set dialog
    QMessageBox msg(this);
    msg.setWindowTitle("Exit Game");
    msg.setText("Are you sure you want to exit?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setWindowIcon(QIcon(":/icons/Game.ico"));

    if (msg.exec() == QMessageBox::Yes)
    {
        event->accept();   // allow closing
    }
    else
    {
        event->ignore();   // cancel closing
    }
}

void MainWindow::undoLastMove()
{
    if(!gameState.hasStarted || moveHistory.empty())
    {
        return;
    }

    // Get last move
    QPoint lastPos = moveHistory.back();
    moveHistory.pop_back();

    // Get new current position
    QPoint newPos = moveHistory.empty() ? QPoint{-1, -1} : moveHistory.back();

    // Handle game state undo
    gameState.undoLastMove(lastPos, newPos);

    // Reset grid
    updateGridUI();

    // Highlight legal moves for new position
    highlightLegalMoves();
}

void MainWindow::openSettings()
{
    SettingsDialog dlg(gameState.getGridSize(), this);
    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    // Handle grid size
    int selectedGridSize = dlg.getSelecteGridSize();
    handleGridSize(selectedGridSize);
}

void MainWindow::handleGridSize(const int selectedGridSize)
{
    if(selectedGridSize == gameState.getGridSize())
    {
        return;
    }

    gameState.setGridSize(selectedGridSize);
    createGridButtons();
    restartGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}
