#include "Styles.h"

QString Styles::getStyle(Target const target)
{
    QMap<Target, QString> colorMap;
    switch(mode)
    {
    case DARK:
        colorMap = Colors::dark;
        break;
    case LIGHT:
    {
        colorMap = Colors::light;
        break;
    }
    }

    switch(target)
    {
    case BACKGROUND:
    {
        return QString("background-color: %1;").arg(colorMap.value(target, "none"));
    }
    default:
        return cellStyle.arg(colorMap.value(target, "none"));
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

