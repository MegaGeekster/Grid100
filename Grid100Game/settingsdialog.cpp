#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFormLayout>
#include <QComboBox>

SettingsDialog::SettingsDialog(const int currentGridSize, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , currentGridSize(currentGridSize)
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
    QComboBox *gridSizeCombo = new QComboBox(this);
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
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
