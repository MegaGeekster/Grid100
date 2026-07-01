#include "Styles.h"

QString Styles::getStyle(Target const target)
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

    switch(target)
    {
    case BACKGROUND:
    {
        return QString("background-color: %1;").arg(colorTheme.backgrounds.value(target, "none"));
    }
    default:
        return cellStyle.arg(colorTheme.backgrounds.value(target, "none"), colorTheme.text, colorTheme.border);
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

