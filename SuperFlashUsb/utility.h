#ifndef UTILITY_H
#define UTILITY_H

#include <QtWidgets>
#include "pc/main.h"
#include "pc/sflash.h"
//#include "rom_t.h"


extern QString RomRamSizeByteLUT[];
extern QMap<int,QString> CartTypeMap;
extern qint64 totalRomSizeInBytes;




/* Snes9x Hi/LoROM autodetect code */

int AllASCII(unsigned char *b, int size);

int ScoreHiROM(unsigned char *data);
int ScoreLoROM(unsigned char *data);

int ScoreRomHeader(unsigned char *data);



bool is_headered(QFile &file);

bool isHirom(uchar *data);
bool isHirom2(uchar *loromdata, uchar *hiromdata);




//void WriteMultiRomEntry(ROM_t *rom, QString &myBootLoader);
//int dothedo(ROM_t &rom);

class SRAM_t
{
public:
    SRAM_t()
    {

        file = NULL;
        filename = "";
        finalString="";

    }

    ~SRAM_t()
    {
        if (file)
        {
            file->close();
            delete file;
        }
    }




    int setup()
    {
        if (!filename.isEmpty())
        {
            if (file)
            {
                file->close();
                delete file;
            }
            file = new QFile(filename);
            if (!file->open(QIODevice::ReadOnly))
            {
                QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Could not open file"));
                return -1;
            }


            if (file->size() > 0x20000)
            {
                QMessageBox::warning(NULL, QObject::tr("Error"), file->fileName() + QObject::tr("\nFile is too big"));
                return -1;
            }


            // This might only work for Linux/Unix based OS
            finalString = filename;//filename.mid(filename.lastIndexOf('/')+1);

            data = file->map(0, file->size());
        }


        return 0;
    }

    QFile *file;
    QString filename;
    QString finalString;
    uchar *data;
};

#endif // UTILITY_H
