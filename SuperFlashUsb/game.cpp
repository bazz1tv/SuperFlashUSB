#include "game.h"

Game::Game()
{
}

int Game::load(QString filename)
{
    return rom.load(filename, QIODevice::ReadOnly);
}

void Game::setNum(int num)
{
    this->num = num;
    rom.num = num;
    bootloaderentry.num = num;
}
