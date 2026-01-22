#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/Game.ico"));
    this->setWindowTitle("Settings");
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
