#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create a grid layout on the central widget
    QWidget *boardWidget = new QWidget(ui->centralwidget);
    QGridLayout *gridLayout = new QGridLayout(boardWidget);

    // Samll spaces between cells
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
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
