#ifndef UTILITY_H
#define UTILITY_H

#include <QtWidgets>

extern QString RomRamSizeByteLUT[];

/* Snes9x Hi/LoROM autodetect code */

int AllASCII(unsigned char *b, int size);

int ScoreHiROM(unsigned char *data);

int ScoreLoROM(unsigned char *data);

bool is_headered(QFile &file);

bool isHirom(uchar *data);



class ROM
{
public:
    ROM()
    {
        offset=0;
        hirom = false;
        headered=false;
        for (uchar i=0; i < 22; i++)
        {
            RomTitle[i]=0;
        }
        RomSizeByte=0;
        SramSizeByte=0;
        file = NULL;
        filename = "";
        finalString="";
        num=1;
    }

    ~ROM()
    {
        if (file)
        {
            file->close();
            delete file;
        }
    }

    bool isHeadered()
    {
        return headered;
    }

    int setup()
    {
        offset = 0;
        // How to find which item was selected?

        // Autodetect the HiROM/LoROM state
        //return;

        if ((headered = is_headered(*file)))
        {
            //QMessageBox::warning(this, tr("Error"), file->fileName() + tr(" is headered"));
            //file.unmap(data);
            offset = 0x200;
        }

        data = file->map(offset, file->size()-offset);
        hirom = isHirom(data);
        // stored in hirom
        if (hirom)
            offset = 0x8000;
        else
            offset = 0;



        for (uchar i=0; i < 21; i++)
        {
            RomTitle[i] = data[0x7fc0+offset+i];
        }
        //RomTitle[21] = 0;
        // ROM Title All set

        // Get ROM Size
        RomSizeByte = data[0x7fd7+offset];
        // Get SRAM Size
        SramSizeByte = data[0x7fd8+offset];
        // Headered

        if (RomSizeByte > 14 || SramSizeByte > 14)
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Something's Wrong. Did you load the correct file?"));
            //qApp->quit();
            return -1;
        }


        return 0;
    }

    bool hirom;
    uchar *data;
    ushort offset;
    bool headered;
    char RomTitle[21+1];
    u_int8_t RomSizeByte;
    u_int8_t SramSizeByte;
    QFile *file;
    QString filename;

public:
    quint8 num;
    QString finalString;
};

int dothedo(ROM &rom);

#endif // UTILITY_H
