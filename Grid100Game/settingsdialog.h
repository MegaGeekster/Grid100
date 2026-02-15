#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <unordered_map>
#include <QComboBox>
#include <QSpinBox>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const int currentGridSize, const bool gameStarted, QWidget *parent = nullptr);
    int getSelecteGridSize() {return selectedGridSize;}
    ~SettingsDialog();

private:
    void windowSetup();
    void accept() override;

    QComboBox* gridSizeCombo;
    std::unordered_map<QString, int> gridSizeOptions{
        {"8x8", 8},
        {"9x9", 9},
        {"10x10", 10},
        {"15x15", 15},
        {"20x20", 20},
        {"Custom Size", -1}
    };
    QSpinBox* customSizeBox;
    int const minGridSize;
    int const maxGridSize;

    int currentGridSize;
    int selectedGridSize;
    bool gameStarted;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
