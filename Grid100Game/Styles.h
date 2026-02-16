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

    enum CellType
    {
        EMPTY,
        OCCUPIED,
        LEGAL,
        CURRENT
    };

    static QString getCellStyle(CellType const cellType)
    {
        QString style = cellStyle;
        switch(mode)
        {
        case DARK:
        case LIGHT:
        {
            style = cellStyle.arg(Colors::light.value(cellType, "none"));
            break;
        }
        }

        return style;
    }

    inline static QString const cellSetup = "QPushButton {"
                               "border: 1px solid #555;"
                               "margin: 0px;"
                               "padding: 0px;"
                               "}";
private:
    struct Colors
    {
        inline static QMap<CellType, QString> const light{
            {EMPTY, "none"},
            {OCCUPIED, "#D3D3D3"},
            {LEGAL, "#ADD8E6"},
            {CURRENT, "#D5FFFF"}
        };
    };

    inline static QString cellStyle = "QPushButton {"
                                      "background-color: %1;"
                                      "border: 1px solid #555;"
                                      "}";

};

#endif // STYLES_H
