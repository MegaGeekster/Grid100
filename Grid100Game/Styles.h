#ifndef STYLES_H
#define STYLES_H

#include <QMap>
#include <QPushButton>
#include "Resources.h"

class Styles
{
public:
    enum Mode
    {
        LIGHT,
        DARK
    };

    enum StyleType
    {
        EMPTY_CELL,
        OCCUPIED_CELL,
        LEGAL_CELL,
        CURRENT_CELL,
        BACKGROUND
    };

    static QString getBackgroundStyle();
    static void setButtonStyle(QPushButton *const button, StyleType styleType);

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

    inline static Mode getMode()
    {
        return mode;
    }

    inline static void setMode(const Mode newMode)
    {
        mode = newMode;
    }

private:
    static QString getModePath(QString const& path, QString const& image);

    struct Theme
    {
        QMap<StyleType, QString> backgrounds;
        QString text;
        QString border;
    };

    inline static Theme const light
    {
        {
            {EMPTY_CELL, "none"},
            {OCCUPIED_CELL, "#D3D3D3"},
            {LEGAL_CELL, "#ADD8E6"},
            {CURRENT_CELL, "#D5FFFF"},
            {BACKGROUND, "none"}
        },
        "#000000", // text
        "#000000"  // border
    };

    inline static Theme const dark{
        {
            {EMPTY_CELL, "757575"},
            {OCCUPIED_CELL, "#696969"},
            {LEGAL_CELL, "#78AAA8"},
            {CURRENT_CELL, "#56D4D2"},
            {BACKGROUND, "#808080"}
        },
        "#FFFFFF", // text
        "#F0F0F0"  // border
    };

    inline static QString const cellStyle = "QPushButton {"
                                      "background-color: %1;"
                                      "color: %2;"
                                      "border: 1px solid %3;"
                                      "}";

    inline static Mode mode = LIGHT;


};

#endif // STYLES_H
