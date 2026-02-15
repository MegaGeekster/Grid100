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

    // Create hidden input box for custom size
    customSizeBox = new QSpinBox(this);
    customSizeBox->setRange(5, 25); //Range of allowed input size
    customSizeBox->setValue(currentGridSize);

    // Set default to current size
    int index = gridSizeCombo->findData(currentGridSize);
    if(index != -1)
    {
        gridSizeCombo->setCurrentIndex(index);
        customSizeBox->hide();
    }

    // Create layout for grid size options
    QHBoxLayout* sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(gridSizeCombo);
    sizeLayout->addWidget(customSizeBox);

    // Create row
    form->addRow("Grid size:", sizeLayout);

    // Connect the hidden box
    connect(gridSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
        int value = gridSizeCombo->currentData().toInt();
        bool isCustom = (value == -1);
        customSizeBox->setVisible(isCustom);
        adjustSize();
    });

    ui->verticalLayout->insertLayout(0, form);
    adjustSize();
}

void SettingsDialog::accept()
{
    // Get the chosen grid size
    selectedGridSize = gridSizeCombo->currentData().toInt();

    if(selectedGridSize == -1)
    {
        selectedGridSize = customSizeBox->value();
    }

    // If grid size is changed and game is in progress, confirm the change
    if(currentGridSize != selectedGridSize && gameStarted)
    {
        // Create confirmation window
        QMessageBox msg(this);
        msg.setWindowTitle("Warning");
        msg.setText("Changing the window size will stop the current game.\nAre you sure you want to restart the game?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.setWindowIcon(QIcon(":/icons/Game.ico"));

        // Cancle the change
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
