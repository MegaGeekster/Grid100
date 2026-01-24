#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFormLayout>
#include <QMessageBox>

SettingsDialog::SettingsDialog(const int currentGridSize, const bool gameStarted, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , currentGridSize(currentGridSize)
    , selectedGridSize(currentGridSize)
    , gameStarted(gameStarted)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/Game.ico"));
    this->setWindowTitle("Settings");

    windowSetup();
}

void SettingsDialog::windowSetup()
{
    // Setup
    QFormLayout* form = new QFormLayout();
    form->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    form->setLabelAlignment(Qt::AlignLeft);
    // ======= Display grid size options =======
    // Create dropdown
    gridSizeCombo = new QComboBox(this);
    for(const auto &option : gridSizeOptions)
    {
        gridSizeCombo->addItem(option.first, option.second);
    }
    // Set default to current size
    int index = gridSizeCombo->findData(currentGridSize);
    if(index != -1)
    {
        gridSizeCombo->setCurrentIndex(index);
    }
    // Create row
    form->addRow("Grid size:", gridSizeCombo);

    ui->verticalLayout->insertLayout(0, form);
    adjustSize();
}

void SettingsDialog::accept()
{
    selectedGridSize = gridSizeCombo->currentData().toInt();
    if(currentGridSize != selectedGridSize && gameStarted)
    {
        QMessageBox msg(this);
        msg.setWindowTitle("Settings");
        msg.setText("Changing the window size will stop the current game.\nAre you sure you want to restart the game?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.setWindowIcon(QIcon(":/icons/Game.ico"));

        if(msg.exec() == QMessageBox::No)
        {
            selectedGridSize = currentGridSize;
        }
    }
    QDialog::accept();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
