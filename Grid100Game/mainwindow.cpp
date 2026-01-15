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
    gridSetup();
}

QString MainWindow::loadInstructions(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Oops, we can't seem to find the instructions, you'll just have to figure it out";

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

    // Place instructions button
    mainLayout->addWidget(instructionsButton, 0, Qt::AlignRight);

    // Connect instructions button
    connect(instructionsButton, &QPushButton::clicked, this, [=](){
        QString text = loadInstructions(":/data/Instructions.txt");
        QMessageBox::information(this, "Game Instructions", text);
    });

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

    ui->centralwidget->setLayout(gridLayout);
    // Set window size to grid size
    this->adjustSize();
    this->setFixedSize(this->size());
}

void MainWindow::onCellClicked(int row, int col)
{
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
    // resetHighlightedCells();
    // highlightLegalMoves();
    // checkGameOver();
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


MainWindow::~MainWindow()
{
    delete ui;
}
