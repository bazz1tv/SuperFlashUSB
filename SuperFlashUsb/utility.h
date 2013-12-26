#ifndef UTILITY_H
#define UTILITY_H

#include <QtWidgets>
#include "pc/main.h"
#include "pc/sflash.h"

extern QString RomRamSizeByteLUT[];
extern QMap<int,QString> CartTypeMap;


/* Snes9x Hi/LoROM autodetect code */

int AllASCII(unsigned char *b, int size);

int ScoreHiROM(unsigned char *data);
int ScoreLoROM(unsigned char *data);

int ScoreRomHeader(unsigned char *data);



bool is_headered(QFile &file);

bool isHirom(uchar *data);
bool isHirom2(uchar *loromdata, uchar *hiromdata);

int OpenFiles();
int OpenForReadBin(const char *filename);
int OpenForWriteBin(const char *filename);

//extern uchar MajorCommand,MinorCommand;



class ROM_t
{
public:
    ROM_t()
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

    ~ROM_t()
    {
        if (file)
        {
            file->close();
            delete file;
        }
    }

    int open()
    {
        if (file)
        {
            file->close();
            delete file;
        }

        file = new QFile(filename);

        if (!file->open(QIODevice::ReadWrite))
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Could not open file"));
            return -1;
        }

        return 1;
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

        CartTypeByte = data[0x7fd6+offset];

        // Get ROM Size
        RomSizeByte = data[0x7fd7+offset];


        // Get SRAM Size
        SramSizeByte = data[0x7fd8+offset];
        // Headered

        if (RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0)
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Something's Wrong. Did you load the correct file?"));
            //qApp->quit();
            return -1;
        }

        // Check for Correlation between RomSizeByte and actual RomFileSize
        // map RomSizeByte to actual number of bytes
        qint64 targetbytesize = 2048<<(RomSizeByte-1);
        qint64 romfilesize = file->size();
        if (headered)
            romfilesize -= 0x200;
        if (targetbytesize != romfilesize)
        {
            //QMessageBox::question()zzzz
            if (QMessageBox::question(NULL, QObject::tr("Hm..."), QObject::tr("This file is not the same size as the ROM header says it should be\n\nLoad it anyways?")) == QMessageBox::No)
            {
                return -1;
            }
            //QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("The file is not the same size as the ROM header says it should be"));
            //qApp->quit();

        }


        return 0;
    }

#define NOTHING 0
#define PREPEND 1
#define APPEND 2
    QString romHeaderToString()
    {
        QString str("");
        if (isValid())
        {
             str = QString("<b>")+QString("%1").arg(num)+QString(") </b>")+QString(RomTitle)+
                    QString("<p>Cart Type: ")+CartTypeStr+
                    QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>ROM</b>: ")+QString(RomRamSizeByteLUT[RomSizeByte])+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>RAM</b>: ")+QString(RomRamSizeByteLUT[SramSizeByte]);

            if (isHeadered())
            {
                str += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EMU-Header Present");
            }
        }
        //else str="";

        return str;
    }

    void setString(QString str="ABCD", uchar mod=NOTHING)
    {
        if (str == "ABCD")
        {
            finalString = romHeaderToString();
        }
        else
        {
            if (!mod)
                finalString = QString("<b>%1)</b> ").arg(num)+str;
            else if (mod == PREPEND)
            {
                finalString = str+QString("<p>")+romHeaderToString();
            }
            else if (mod == APPEND)
            {
                finalString = romHeaderToString() + QString("<p>")+str;
            }
        }
    }

    bool isValid()
    {
        if (!(RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0))
            return true;
        else return false;
    }

    bool isTypical()
    {
        if (CartTypeMap.contains(CartTypeByte))
            return true;
        else return false;
    }

    void QueryUSBRomHeader()
    {
        int header_startaddr = 0x7fb0+startaddr;    // startaddr local to ROM_t

        // This refers to those pesky global variables I have floating around from PC-commandline-version
        aal = header_startaddr&0xff;
        aah = (header_startaddr&0xff00)>>8;
        aab = (header_startaddr&0xff0000)>>16;

        numbytes = 0x50;    // Header Size from 0x7fb0-0x7fff

        QByteArray loromheader,hiromheader, *selectedheader;
        ReadHeader(&loromheader);

        header_startaddr = 0xffb0+startaddr;
        aal = header_startaddr&0xff;
        aah = (header_startaddr&0xff00)>>8;
        aab = (header_startaddr&0xff0000)>>16;

        numbytes = 0x50;
        ReadHeader(&hiromheader);

        bool hirom= isHirom2((uchar*)loromheader.data(), (uchar*)hiromheader.data());
        //int offset;
        if (hirom)
        {
            //offset = 1;
            selectedheader = &hiromheader;
            //QMessageBox::critical(this, "Derp", "HiRom");
        }
        else
        {
            selectedheader = &loromheader;
            //QMessageBox::critical(this, "Derp", "LoRom");
        }



        for (uchar i=0; i < 21; i++)
        {
            RomTitle[i] = selectedheader->at(0x10+i);
        }
        //RomTitle[21] = 0;
        // ROM Title All set
        CartTypeByte = selectedheader->at(0x16+0x10);
        // CartTypeByteToStr
        if (CartTypeMap.contains(CartTypeByte))
            CartTypeStr = QString(CartTypeMap[CartTypeByte]);
        else CartTypeStr = QString("Unknown Cart Type");

        // Get ROM Size
        RomSizeByte = selectedheader->at(0x17+0x10);


        // Get SRAM Size
        SramSizeByte = selectedheader->at(0x18+0x10);

        if (isValid() && isTypical())
        {
            setString();
            //ui->textEdit1->setHtml(ui->textEdit1->rom.finalString);
        }
        else
        {
            setString("Game may be Empty/Corrupt. Or it may have a very unique ROM header. I'll display the ROM info anyways:", PREPEND);
        }
    }

    int DoTheDo()
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


            if (file->size() < 0x8000)
            {
                QMessageBox::warning(NULL, QObject::tr("Error"), file->fileName() + QObject::tr(" is not large enough a file"));
                return -1;
            }
            // something was chosen, do stuff
            if (setup() < 0)
            {
                return -1;
            }

            finalString = QString("<b>")+QString("%1").arg(num)+QString(") </b>")+QString(RomTitle)+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>ROM</b>: ")+QString(RomRamSizeByteLUT[RomSizeByte])+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>SRAM</b>: ")+QString(RomRamSizeByteLUT[SramSizeByte]);
            if (isHeadered())
            {
                finalString += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EMU-Header Present");
            }

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
    u_int8_t CartTypeByte;
    QString CartTypeStr;
    QFile *file;
    QString filename;
    int startaddr;

public:
    quint8 num;
    QString finalString;
};



int dothedo(ROM_t &rom);

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
