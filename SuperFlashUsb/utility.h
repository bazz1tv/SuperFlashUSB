#ifndef UTILITY_H
#define UTILITY_H

#include <QtWidgets>
#include "pc/main.h"
#include "pc/sflash.h"
//#include "rom_t.h"


extern QString RomRamSizeByteLUT[];
extern QMap<int,QString> CartTypeMap;





/* Snes9x Hi/LoROM autodetect code */

int AllASCII(unsigned char *b, int size);

int ScoreHiROM(unsigned char *data);
int ScoreLoROM(unsigned char *data);

int ScoreRomHeader(unsigned char *data);



//bool is_headered(QFile &file);

bool isHirom(uchar *data);
bool isHirom2(uchar *loromdata, uchar *hiromdata);



#endif // UTILITY_H
