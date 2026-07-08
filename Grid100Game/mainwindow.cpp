#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QTextStream>
#include <QTextBrowser>
#include <QSettings>
#include "settingsdialog.h"
#include "Resources.h"
#include "Styles.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Get saved theme
    QSettings settings;
    Styles::setMode(static_cast<Styles::Mode>(settings.value("Theme", static_cast<int>(Styles::Mode::LIGHT)).toInt()));
    ui->setupUi(this);

    // Configure the window
    this->setWindowIcon(QIcon(Resources::Icon::game));
    this->setWindowTitle("Grid100");
    this->setStyleSheet(Styles::getBackgroundStyle());

    gameState.reset();
    gridSetup();
    updateGridUI();
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
    settingsButton = new QPushButton(this);

    // Connect settings button
    connect(settingsButton, &QPushButton::clicked, this,  &MainWindow::openSettings);

    // ============ Create instructions button ============
    instructionsButton = new QPushButton(this);

    // Connect instructions button
    connect(instructionsButton, &QPushButton::clicked, this, &MainWindow::showInstructions);

    // ============ Create restart button ============
    restartButton = new QPushButton(this);

    // Connect reset Button
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::handleRestart);

    // ============ Create undo button ============
    undoButton = new QPushButton(this);

    // Connect undo button
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::undoLastMove);

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

    // Set icons
    setButtonIcons();

    // ============ Create a grid layout ============
    boardWidget = new QWidget(ui->centralwidget);
    gridLayout = new QGridLayout(boardWidget);

    // Small spaces between cells
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(boardWidget, 0, Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    createGridButtons();
}

void MainWindow::setButtonIcons()
{
    // Undo
    undoButton->setIcon(QIcon(Styles::getUndoIcon()));
    undoButton->setIconSize(QSize(24,24));
    // Instructions
    instructionsButton->setIcon(QIcon(Styles::getInstructionsIcon()));
    instructionsButton->setIconSize(QSize(24, 24));
    // Settings
    settingsButton->setIcon(QIcon(Styles::getSettingsIcon()));
    settingsButton->setIconSize(QSize(24, 24));
    // Restart
    restartButton->setIcon(QIcon(Styles::getRestartIcon()));
    restartButton->setIconSize(QSize(24,24));
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
    cells.clear();
    cells.resize(gridSize, std::vector<Cell>(gridSize, Cell{}));

    // Create cells
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            // Create cell
            QPushButton *button = new QPushButton(this);

            // Set size and style
            button->setFixedSize(32, 32);
            Styles::setButtonStyle(button, Styles::EMPTY_CELL);

            // Connect button
            connect(button, &QPushButton::clicked, this, [=]() {
                onCellClicked(row, col);
            });

            cells[row][col].button = button;

            // Add cell to grid
            gridLayout->addWidget(button, row, col);
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
    std::vector<QPoint> legalMoves = gameState.getLegalMoves(cells);

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

    gameState.placeNumber(pos, &cells[pos.x()][pos.y()]);
    updateGridUI();
    moveHistory.push_back(pos);
    checkGameOver();
}

void MainWindow::checkGameOver()
{
    if(!gameState.hasStarted)
    {
        return;
    }

    std::vector<QPoint> legalMoves = gameState.getLegalMoves(cells);

    if(!legalMoves.empty())
    {
        return;
    }
    gameState.gameOver = true;
    std::pair<QString, QString> message = gameState.getGameOverMessage();

    // Set game over dialog
    QMessageBox msg(this);
    msg.setWindowTitle(message.first);
    msg.setText(message.second);
    msg.setWindowIcon(QIcon(Resources::Icon::game));

    QPushButton *undoBtn = msg.addButton("Undo Last Move", QMessageBox::AcceptRole);
    QPushButton *restartBtn = msg.addButton("New Game", QMessageBox::AcceptRole);
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
    std::vector<QPoint> legalMoves = gameState.getLegalMoves(cells);
    for(const auto move : legalMoves)
    {
        int r = move.x();
        int c = move.y();
        Styles::setButtonStyle(cells[r][c].button, Styles::StyleType::LEGAL_CELL);
    }
}

void MainWindow::handleRestart()
{
    if(!gameState.hasStarted)
    {
        return;
    }
    QMessageBox msg(this);
    msg.setWindowTitle("Restart Game");
    msg.setText("Are you sure you want to restart the game?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setWindowIcon(QIcon(Resources::Icon::game));

    if (msg.exec() == QMessageBox::Yes) {
        restartGame();
    }
}

void MainWindow::restartGame()
{
    gameState.reset();
    moveHistory ={};
    // Reset cells
    for(auto &cellRow : cells)
    {
        for(auto &cell : cellRow)
        {
            cell.number = 0U;
            cell.styleType = Styles::StyleType::EMPTY_CELL;
        }
    }
    updateGridUI();
}

void MainWindow::set0GridButton(const QPoint point)
{
    cells[point.x()][point.y()].button->setText("");
    Styles::setButtonStyle(cells[point.x()][point.y()].button, Styles::StyleType::EMPTY_CELL);
}

void MainWindow::updateGridUI()
{
    // Iterate over grid
    for (int row = 0; row < gameState.getGridSize(); row++)
    {
        for (int col = 0; col < gameState.getGridSize(); col++)
        {
            int value = cells[row][col].number;

            if (value == 0)
            {
                set0GridButton(QPoint{row, col});
            }
            else
            {
                cells[row][col].button->setText(QString::number(value));
                Styles::setButtonStyle(cells[row][col].button, Styles::StyleType::OCCUPIED_CELL);
            }
        }
    }

    // Highlight current cell
    if(!gameState.hasStarted)
    {
        return;
    }
    QPoint currentCell = gameState.getCurrentPos();
    Styles::setButtonStyle(cells[currentCell.x()][currentCell.y()].button, Styles::StyleType::CURRENT_CELL);

    highlightLegalMoves();
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
    msg.setWindowIcon(QIcon(Resources::Icon::game));

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

    // If no history, we are back at start of game
    if(moveHistory.empty())
    {
        restartGame();
        return;
    }
    // Get new current position
    QPoint newPos = moveHistory.empty() ? QPoint{-1, -1} : moveHistory.back();

    // Handle game state undo
    gameState.undoLastMove(newPos, &cells[lastPos.x()][lastPos.y()]);

    // Reset grid
    updateGridUI();
}

void MainWindow::showInstructions()
{
    // Create the instructions window
    QDialog dialog(this);
    dialog.setWindowTitle("Instructions");
    dialog.setWindowIcon(QIcon(Resources::Icon::game));
    dialog.setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Text browser
    QTextBrowser* browser = new QTextBrowser;
    browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Load instructions from file
    QString text = loadInstructions(Styles::getInstructionsFile());
    browser->setHtml(text);

    // Close button
    QPushButton* closeButton = new QPushButton("Close", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Add to layout
    layout->addWidget(browser);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Open at 70% of screen size
    QSize screenSize = dialog.screen()->availableGeometry().size();
    dialog.resize(screenSize.width() * 0.95,
                  screenSize.height() * 0.95);

    dialog.exec();
}

void MainWindow::openSettings()
{
    SettingsDialog dlg(gameState.getGridSize(), gameState.hasStarted, this);
    if(dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    // Handle grid size
    int selectedGridSize = dlg.getSelecteGridSize();
    handleGridSize(selectedGridSize);

    // Handle theme
    Styles::Mode selectedTheme = dlg.getSelectedTheme();
    handleTheme(selectedTheme);
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

void MainWindow::handleTheme(Styles::Mode const selectedTheme)
{
    if(selectedTheme == Styles::getMode())
    {
        return;
    }

    // Change theme
    Styles::setMode(selectedTheme);
    updateGridUI(); // Update button colors
    this->setStyleSheet(Styles::getBackgroundStyle()); // Update background color
    setButtonIcons();

    // Save new theme
    QSettings settings;
    settings.setValue("Theme", static_cast<int>(Styles::getMode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
