#include "utility.h"
#include "rom_t.h"

qint64 totalRomSizeInBytes=0;

QMap<int,QString> CartTypeMap;



QString RomRamSizeByteLUT[] =
{
    "NO RAM",   // 0
    "2 KB",     // 1
    "4 KB",     // 2
    "8 KB",     // 3
    "16 KB",    // 4
    "32 KB",    // 5
    "64 KB",    // 6
    "128 KB",   // 7
    "256 KB",   // 8
    "4 Mb",     // 9
    "8 Mb",     // 10
    "16 Mb",    // 11
    "32 Mb",    // 12
    "48 Mb",    // 13
    "64 Mb"     // 14
};




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

int ScoreRomHeader(unsigned char *data)
{
    int score = 0;

    if ((data[0x2c] + data[0x2d]*256 + data[0x2e] + data[0x2f]*256) == 0xFFFF)
    {
        score += 2;
    }

    if (data[0x2A] == 0x33)
    {
        score += 2;
    }
    if ((data[0x25] & 0xf) < 4)
    {
        score += 2;
    }
    if (!(data[0x4d] & 0x80))
    {
        score -= 4;
    }
    if ((1 << (data[0x27] - 7)) > 48)
    {
        score -= 1;
    }
    if (!AllASCII(&data[0x0], 6))
    {
        score -= 1;
    }
    if (!AllASCII(&data[0x10], 20))
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


bool isHirom2(uchar *loromdata, uchar *hiromdata)
{
    bool hirom;
    int hiscore, loscore;

    hiscore = ScoreRomHeader(hiromdata);
    loscore = ScoreRomHeader(loromdata);
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


