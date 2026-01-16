#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameState.h"
#include <QMainWindow>
#include <QPushButton>

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
    ~MainWindow();

private:
    void gridSetup();
    QString loadInstructions(const QString &filePath);
    void onCellClicked(int row, int col);
    void updateGridUI();
    void highlightLegalMoves();
    void checkGameOver();
    void restartGame();
    Ui::MainWindow *ui;
    GameState gameState;
    std::vector<std::vector<QPushButton*>> cellButtons;
};
#endif // MAINWINDOW_H
