#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
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

    // Create instructions button
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


    // Create a grid layout on the central widget
    QWidget *boardWidget = new QWidget(ui->centralwidget);
    QGridLayout *gridLayout = new QGridLayout(boardWidget);

    // Samll spaces between cells
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(boardWidget, 0, Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create 10×10 cells
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            // Create cell
            QLabel *cell = new QLabel(this);
            // Set size and style
            cell->setFixedSize(32, 32);
            cell->setAlignment(Qt::AlignCenter);
            cell->setStyleSheet(
                "QLabel {"
                "border: 1px solid #555;"
                "margin: 0px;"
                "padding: 0px;"
                "}"
                );

            // Add cell to grid
            gridLayout->addWidget(cell, row, col);
        }
    }

    ui->centralwidget->setLayout(gridLayout);
    // Set window size to grid size
    this->adjustSize();
    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}
