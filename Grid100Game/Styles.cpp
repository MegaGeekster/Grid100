#include "Styles.h"

QString Styles::getStyle(StyleType const type)
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

    switch(type)
    {
    case BACKGROUND:
    {
        return QString("background-color: %1;").arg(colorTheme.backgrounds.value(type, "none"));
    }
    default:
        return cellStyle.arg(colorTheme.backgrounds.value(type, "none"), colorTheme.text, colorTheme.border);
    }
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

