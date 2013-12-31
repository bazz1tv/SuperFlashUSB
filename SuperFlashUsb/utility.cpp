#include "utility.h"

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

void WriteMultiRomEntry(ROM_t *rom, QString &myBootLoader)
{
    int n, uses_DSP;
    unsigned char name[0x1c], flags1, flags2;
    static int slot = 0;

    // SKIP SDP STUFF FOR NOW DSP
    /*uses_DSP = snes_header.rom_type == 3 || snes_header.rom_type == 5 ||
               snes_header.rom_type == 0xf6;*/

    /*fseek (destfile, 0x4000 + (file_no - 1) * 0x20, SEEK_SET);
    fputc (0xff, destfile);                       // 0x0 = 0xff*/

    //memcpy (name, rominfo->name, 0x1c);
    int i=0x4000 + (rom->num-1) * 0x20;
    int x=0;

    myBootLoader[i++] = 0xff;
    for (x=0; x < 21; x++)
    {
        myBootLoader[i++] = rom->RomTitle[x];
    }
    for (x; x <= 0x1c; x++)
    {
        myBootLoader[i++] = ' ';
    }

    //fwrite (name, 1, 0x1c, destfile);             // 0x1 - 0x1c = name

    if (rom->sramsizeinbytes)
      {
        if (rom->sramsizeinbytes == 2 * 1024)
          flags2 = 0x00;
        else if (rom->sramsizeinbytes == 8 * 1024)
          flags2 = 0x10;
        else if (rom->sramsizeinbytes == 32 * 1024)
          flags2 = 0x20;
        else // if (snes_sramsize == 128 * 1024)  // Default to 1024 kbit SRAM
          flags2 = 0x30;
      }
    else
      flags2 = 0x40;

    if (rom->romsizeinbytes > 0x400000)            // Enable Extended Map for >32 Mbit ROMs
      flags2 |= 0x80;

    flags1 = rom->hirom ? 0x10 : 0x00;

    /*if (!rom->hirom && uses_DSP)                  // Set LoROM DSP flag if necessary
      flags1 |= 0x01;*/

    if (rom->startaddr == 0)
      flags1 |= 0x00;
    else if (rom->startaddr == 0x200000)
      flags1 |= 0x40;
    else if (rom->startaddr == 0x400000)
      flags1 |= 0x20;
    else if (rom->startaddr == 0x600000)
      flags1 |= 0x60;

    //slot += (size + 16 * MBIT - 1) & ~(16 * MBIT - 1);

    //fputc (flags1, destfile);                     // 0x1d = mapping flags
    //fputc (flags2, destfile);                     // 0x1e = SRAM flags
    //fputc (size / 0x8000, destfile);              // 0x1f = ROM size (not used by loader)
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


