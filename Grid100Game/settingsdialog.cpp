#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFormLayout>
#include <QMessageBox>
#include "Resources.h"
#include "Styles.h"

SettingsDialog::SettingsDialog(const int currentGridSize, const bool gameStarted, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , currentGridSize(currentGridSize)
    , selectedGridSize(currentGridSize)
    , gameStarted(gameStarted)
    , minGridSize(5)
    , maxGridSize(25)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(Resources::Icon::game));
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

    // ======= Display mode/theme options =======
    themeCombo = new QComboBox(this);
    for(const auto &option : themeOptions)
    {
        themeCombo->addItem(option.first, option.second);
    }

    // Set default to current theme
    index = themeCombo->findData(Styles::getMode());
    if(index != -1)
    {
        themeCombo->setCurrentIndex(index);
    }

    // Add row
    form->addRow("Theme:", themeCombo);

    // ===== Finalize layout =====
    ui->verticalLayout->insertLayout(0, form);
    adjustSize();
}

void SettingsDialog::handleGridSize()
{
    // Get the chosen grid size
    selectedGridSize = gridSizeCombo->currentData().toInt();

    // Handle custom size
    if(selectedGridSize == -1)
    {
        int const value = customSizeBox->value();
        // Validate custom size
        if(value < minGridSize || value > maxGridSize)
        {
            QMessageBox::warning(
                this,
                "Invalid Size",
                QString("Grid size must be between %1 and %2.")
                    .arg(minGridSize)
                    .arg(maxGridSize)
                );
            return; // Keep dialog open
        }

        selectedGridSize = value;
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
        msg.setWindowIcon(QIcon(Resources::Icon::game));

        // Cancle the change
        if(msg.exec() == QMessageBox::No)
        {
            selectedGridSize = currentGridSize;
        }
    }
}

void SettingsDialog::handleTheme()
{
    // Get the chosen theme
    selectedTheme = static_cast<Styles::Mode>(themeCombo->currentData().toInt());
}

void SettingsDialog::accept()
{
    handleGridSize();
    handleTheme();
    QDialog::accept();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
