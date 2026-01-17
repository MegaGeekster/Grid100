#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameState.h"
#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>


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
    void checkGameOver();
    void restartGame();
    void closeEvent(QCloseEvent *event) override;
    void set0GridButton(const QPoint point);
    void resetGridButton(const QPoint point);

    Ui::MainWindow *ui;
    GameState gameState;
    std::vector<std::vector<QPushButton*>> cellButtons;
    bool forceExit = false;

    std::vector<QPoint> moveHistory;
};
#endif // MAINWINDOW_H
