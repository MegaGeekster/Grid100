#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <unordered_map>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(int const currentGridSize, QWidget *parent = nullptr);
    ~SettingsDialog();

private:
    void windowSetup();
    std::unordered_map<QString, int> gridSizeOptions{
        {"8x8", 8},
        {"9x9", 9},
        {"10x10", 10},
        {"15x15", 15},
        {"20x20", 20}
    };

    int currentGridSize;
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
