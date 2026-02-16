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

        inline static QString const game = iconsPath + "Game.ico";
        inline static QString const instructions = iconsPath + "Instructions.png";
        inline static QString const restart = iconsPath + "Restart.png";
        inline static QString const settings = iconsPath + "Settings.png";
        inline static QString const undo = iconsPath + "Undo.png";
    };

    struct Instructions
    {
        inline static QString const instructionsFile = dataPath + "Instructions.html";
        struct images
        {
            inline static QString const diagonalMove = imagesPath + "diagonal_move.png";
            inline static QString const streightMove = imagesPath + "streight_move.png";
            inline static QString const secondMove = imagesPath + "second_move.png";
        };
    };
};

#endif // RESOURCES_H
