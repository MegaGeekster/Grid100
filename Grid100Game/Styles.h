#ifndef STYLES_H
#define STYLES_H

#include <QMap>
#include "Resources.h"

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

    static QString getStyle(Target const target);

    inline static QString const cellSetup = "QPushButton {"
                               "border: 1px solid #000000;"
                               "margin: 0px;"
                               "padding: 0px;"
                               "}";

    // Get resources
    inline static QString getGameIcon()
    {
        return getModePath(Resources::Icon::iconsPath, Resources::Icon::game);
    }

    inline static QString getUndoIcon()
    {
        return getModePath(Resources::Icon::iconsPath, Resources::Icon::undo);
    }

    inline static QString getRestartIcon()
    {
        return getModePath(Resources::Icon::iconsPath, Resources::Icon::restart);
    }

    inline static QString getSettingsIcon()
    {
        return getModePath(Resources::Icon::iconsPath, Resources::Icon::settings);
    }

    inline static QString getInstructionsIcon()
    {
        return getModePath(Resources::Icon::iconsPath, Resources::Icon::instructions);
    }

    inline static QString getInstructionsFile()
    {
        return getModePath(Resources::dataPath, Resources::Instructions::instructionsFile);
    }

private:
    static QString getModePath(QString const& path, QString const& image);

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
            {OCCUPIED_CELL, "#AAAAAA"},
            {LEGAL_CELL, "#598482"},
            {CURRENT_CELL, "#56D4D2"},
            {BACKGROUND, "#868482"}
        };
    };

    inline static QString const cellStyle = "QPushButton {"
                                      "background-color: %1;"
                                      "border: 1px solid #000000;"
                                      "}";

};

#endif // STYLES_H
