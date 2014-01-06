#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <QtWidgets>
#include "pc/main.h"
#include "pc/sflash.h"

#define BOOTLOADER_BASE_ADDRESS 0x7f8000

#define FLAG1_OFFSET 0x1d
#define FLAG2_OFFSET 0x1e

class BootLoaderEntry
{
public:
    BootLoaderEntry();
    //BootLoaderEntry (int num);

    // IMPORTANT
    bool VerifyBootLoaderRomEntry();
    void DownloadBootLoaderRomEntry();


    // DONT CARE
    bool isFlag1Verified();
    bool isFlag2Verified();
    bool isBootLoaderRomTitleVerified();

    // DATA
    QByteArray bootLoaderEntry;
    uchar *entry;
    int num;
};

#endif // BOOTLOADER_H




