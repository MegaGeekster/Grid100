#ifndef RESOURCES_H
#define RESOURCES_H

#include <QString>

struct Resources
{
    inline static QString const resourcesPath = ":/resources/";
    inline static QString const imagesPath = resourcesPath + "images/";
    inline static QString const dataPath = resourcesPath + "data/";

    struct Icon
    {
        inline static QString const iconsPath = resourcesPath + "icons/";

        inline static QString const game = "Game.ico";
        inline static QString const instructions = "Instructions.png";
        inline static QString const restart = "Restart.png";
        inline static QString const settings = "Settings.png";
        inline static QString const undo = "Undo.png";
    };

    struct Instructions
    {
        inline static QString const instructionsFile = "Instructions.html";
        struct images
        {
            inline static QString const diagonalMove = "diagonal_move.png";
            inline static QString const streightMove = "streight_move.png";
            inline static QString const secondMove = "second_move.png";
        };
    };
};

#endif // RESOURCES_H
