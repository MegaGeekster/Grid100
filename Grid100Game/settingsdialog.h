#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <unordered_map>
#include <QComboBox>
#include <QSpinBox>
#include "Styles.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const int currentGridSize, const bool gameStarted, QWidget *parent = nullptr);
    int getSelecteGridSize() {return selectedGridSize;}
    Styles::Mode getSelectedTheme() {return selectedTheme;}
    ~SettingsDialog();

private:
    void windowSetup();
    void accept() override;

    // ===== Grid size =====
    void handleGridSize();

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

    // ===== Mode/Theme =====
    void handleTheme();
    QComboBox* themeCombo;
    std::unordered_map<QString, Styles::Mode> themeOptions{
        {"Light Theme", Styles::Mode::LIGHT},
        {"Dark Theme", Styles::Mode::DARK}
    };
    Styles::Mode selectedTheme;

    bool gameStarted;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
