#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameState.h"
#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>
#include <QGridLayout>
#include "Cell.h"
#include "Styles.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void undoLastMove();
    ~MainWindow();

private:
    void gridSetup();
    QString loadInstructions(const QString &filePath);
    void onCellClicked(int row, int col);
    void updateGridUI();
    void highlightLegalMoves();
    bool isGameOver();
    void handleGameOver();
    void handleRestart();
    void restartGame();
    void closeEvent(QCloseEvent *event) override;
    void set0GridButton(const QPoint point);
    void resetGridButton(const QPoint point);
    void openSettings();
    void handleGridSize(const int selectedGridSize);
    void handleTheme(const Styles::Mode slectedTheme);
    void createGridButtons();
    void showInstructions();

    Ui::MainWindow *ui;
    GameState gameState;
    std::vector<std::vector<Cell>> cells;
    bool forceExit = false;
    QGridLayout *gridLayout;
    QWidget *boardWidget;

    std::vector<QPoint> moveHistory;

    // Buttons
    QPushButton *settingsButton;
    QPushButton *instructionsButton;
    QPushButton *restartButton;
    QPushButton *undoButton;
    void setButtonIcons();
};
#endif // MAINWINDOW_H
