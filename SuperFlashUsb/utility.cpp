#include "utility.h"



QString RomRamSizeByteLUT[] =
{
    "NO RAM",   // 0
    "2 KB",     // 1
    "4 KB",     // 2
    "8 KB",
    "16 KB",
    "32 KB",
    "64 KB",
    "128 KB",
    "256 KB",
    "4 Mb",
    "8 Mb",
    "16 Mb",
    "32 Mb",
    "48 Mb",
    "64 Mb"     // 14
};

int dothedo(ROM &rom)
{
    if (!rom.filename.isEmpty())
    {
        if (rom.file)
        {
            rom.file->close();
            delete rom.file;
        }
        rom.file = new QFile(rom.filename);
        if (!rom.file->open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Could not open file"));
            return -1;
        }


        if (rom.file->size() < 0x8000)
        {
            QMessageBox::warning(NULL, QObject::tr("Error"), rom.file->fileName() + QObject::tr(" is not large enough a file"));
            return -1;
        }
        // something was chosen, do stuff
        if (rom.setup() < 0)
        {
            return -1;
        }

        rom.finalString = QString("<b>")+QString("%1").arg(rom.num)+QString(") </b>")+QString(rom.RomTitle)+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>ROM</b>: ")+QString(RomRamSizeByteLUT[rom.RomSizeByte])+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>SRAM</b>: ")+QString(RomRamSizeByteLUT[rom.SramSizeByte]);
        if (rom.isHeadered())
        {
            rom.finalString += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EMU-Header Present");
        }

    }

    return 0;
}

int AllASCII(unsigned char *b, int size)
{
        int i;
        for (i = 0; i < size; i++)
        {
                if (b[i] < 32 || b[i] > 126)
                {
                        return 0;
                }
        }
        return 1;
}

int ScoreHiROM(unsigned char *data)
{
        int score = 0;

        if ((data[0xFFDC] + data[0xFFDD]*256 + data[0xFFDE] + data[0xFFDF]*256) == 0xFFFF)
        {
                score += 2;
        }

        if (data[0xFFDA] == 0x33)
        {
                score += 2;
        }
        if ((data[0xFFD5] & 0xf) < 4)
        {
                score += 2;
        }
        if (!(data[0xFFFD] & 0x80))
        {
                score -= 4;
        }
        if ((1 << (data[0xFFD7] - 7)) > 48)
        {
                score -= 1;
        }
        if (!AllASCII(&data[0xFFB0], 6))
        {
                score -= 1;
        }
        if (!AllASCII(&data[0xFFC0], 20))
        {
                score -= 1;
        }

        return (score);
}

int ScoreLoROM(unsigned char *data)
{
        int score = 0;

        if ((data[0x7FDC] + data[0x7FDD]*256 + data[0x7FDE] + data[0x7FDF]*256) == 0xFFFF)
        {
                score += 2;
        }
        if (data[0x7FDA] == 0x33)
        {
                score += 2;
        }
        if ((data[0x7FD5] & 0xf) < 4)
        {
                score += 2;
        }
        if (!(data[0x7FFD] & 0x80))
        {
                score -= 4;
        }
        if ((1 << (data[0x7FD7] - 7)) > 48)
        {
                score -= 1;
        }
        if (!AllASCII(&data[0x7FB0], 6))
        {
                score -= 1;
        }
        if (!AllASCII(&data[0x7FC0], 20))
        {
                score -= 1;
        }

        return (score);
}

bool is_headered(QFile &file)
{
    if (file.size() % 1024 == 512)
    {
        return true;
    }

    return false;
}

bool isHirom(uchar *data)
{
    bool hirom;
    int hiscore, loscore;

    hiscore = ScoreHiROM(data);
    loscore = ScoreLoROM(data);
    if (hiscore>loscore)
    {
        hirom = true;
    }
    else
    {
        hirom = false;
    }
    return hirom;
}
