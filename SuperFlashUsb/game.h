#ifndef GAME_H
#define GAME_H

#include "bootloaderentry.h"
#include "rom_t.h"

#include <QtWidgets>

class Game
{
public:
    Game();

    int load(QString filename);
    void setNum(int num);

    BootLoaderEntry bootloaderentry;
    ROM_t rom;

    int num;
};

#endif // TOTOTEKGAME_H
