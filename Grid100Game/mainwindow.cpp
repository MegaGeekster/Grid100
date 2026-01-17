#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>

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

    // ============ Create instructions button ============
    QPushButton *instructionsButton = new QPushButton(this);
    // Set Icon
    instructionsButton->setIcon(QIcon(":/icons/Instructions.png"));
    instructionsButton->setIconSize(QSize(24, 24));

    // Connect instructions button
    connect(instructionsButton, &QPushButton::clicked, this, [=](){
        QString text = loadInstructions(":/data/Instructions.txt");
        QMessageBox::information(this, "Game Instructions", text);
    });

    // ============ Create restart button ============
    QPushButton *restartButton = new QPushButton(this);
    // Set Icon
    restartButton->setIcon(QIcon(":/icons/Restart.png"));
    restartButton->setIconSize(QSize(24,24));

    // Connect reset Button
    connect(restartButton, &QPushButton::clicked, this, [=](){
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

    // ============ Top bar layout ============
    QHBoxLayout *topBarLayout = new QHBoxLayout();

    // Push buttons to the right
    topBarLayout->addStretch();

    // Add buttons
    topBarLayout->addWidget(restartButton);
    topBarLayout->addWidget(instructionsButton);

    // Small spacing between buttons
    topBarLayout->setSpacing(6);

    // Add top bar to main layout
    mainLayout->addLayout(topBarLayout);

    // ============ Create a grid layout ============
    // Initialize cells list
    cellButtons.resize(10, std::vector<QPushButton*>(10, nullptr));

    QWidget *boardWidget = new QWidget(ui->centralwidget);
    QGridLayout *gridLayout = new QGridLayout(boardWidget);

    // Samll spaces between cells
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(boardWidget, 0, Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create 10×10 cells
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
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

    // Set window size to grid size
    this->adjustSize();
    this->setFixedSize(this->size());
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
    checkGameOver();
}

void MainWindow::checkGameOver()
{
    if(!gameState.hasStarted)
    {
        return;
    }

    std::pair<QString, QString> text = {"", ""};

    // If won the game
    if(gameState.getCurrentNumber() > 100)
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

    QPushButton *restartBtn = msg.addButton("Restart", QMessageBox::AcceptRole);
    QPushButton *exitBtn = msg.addButton("Exit", QMessageBox::RejectRole);

    msg.setDefaultButton(restartBtn);

    msg.exec();

    if (msg.clickedButton() == restartBtn)
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

void MainWindow::updateGridUI()
{
    // Iterate over grid
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int value = gameState.grid[row][col];

            if (value == 0)
            {
                cellButtons[row][col]->setText("");
                cellButtons[row][col]->setStyleSheet(
                    "QPushButton {"
                    "background-color: none;"
                    "border: 1px solid #555;"
                    "}"
                    );
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

MainWindow::~MainWindow()
{
    delete ui;
}
