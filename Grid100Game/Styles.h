#ifndef STYLES_H
#define STYLES_H

#include <QMap>

class Styles
{
public:
    enum Mode
    {
        LIGHT,
        DARK
    };
    inline static Mode mode = LIGHT;

    enum Target
    {
        EMPTY_CELL,
        OCCUPIED_CELL,
        LEGAL_CELL,
        CURRENT_CELL,
        BACKGROUND
    };

    static QString getStyle(Target const target)
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

    inline static QString const cellSetup = "QPushButton {"
                               "border: 1px solid #000000;"
                               "margin: 0px;"
                               "padding: 0px;"
                               "}";
private:
    struct Colors
    {
        inline static QMap<Target, QString> const light{
            {EMPTY_CELL, "none"},
            {OCCUPIED_CELL, "#D3D3D3"},
            {LEGAL_CELL, "#ADD8E6"},
            {CURRENT_CELL, "#D5FFFF"},
            {BACKGROUND, "none"}
        };

        inline static QMap<Target, QString> const dark{
            {EMPTY_CELL, "#868482"},
            {OCCUPIED_CELL, "#A0A0A0"},
            {LEGAL_CELL, "#598482"},
            {CURRENT_CELL, "#56D4D2"},
            {BACKGROUND, "#868482"}
        };
    };

    inline static QString const cellStyle = "QPushButton {"
                                      "background-color: %1;"
                                      "border: 1px solid #555;"
                                      "}";

};

#endif // STYLES_H
