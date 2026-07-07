#include "Styles.h"

QString Styles::getBackgroundStyle()
{
    Theme colorTheme;
    switch(mode)
    {
    case DARK:
        colorTheme = dark;
        break;
    case LIGHT:
    {
        colorTheme = light;
        break;
    }
    }

    return QString("background-color: %1;").arg(colorTheme.backgrounds.value(StyleType::BACKGROUND, "none"));
}

void Styles::setButtonStyle(QPushButton *const button, StyleType styleType)
{
    Theme colorTheme;
    switch(mode)
    {
    case DARK:
    {
        colorTheme = dark;
        break;
    }
    case LIGHT:
    {
        colorTheme = light;
        break;
    }
    }

    button->setStyleSheet(cellStyle.arg(colorTheme.backgrounds.value(styleType, "none"), colorTheme.text, colorTheme.border));
}

QString Styles::getModePath(QString const& path, QString const& image)
{
    QString folderName = "";
    switch(mode)
    {
    case DARK:
        folderName = "dark/";
        break;
    case LIGHT:
        folderName = "light/";
        break;
    }
    return path + folderName + image;
}

