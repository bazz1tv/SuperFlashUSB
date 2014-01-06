#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#define BOOTLOADER_BASE_ADDRESS 0x7f8000

#define FLAG1_OFFSET 0x1d
#define FLAG2_OFFSET 0x1e

class BootLoaderEntry
{
public:
    BootLoaderEntry();

    // IMPORTANT
    bool VerifyBootLoaderRomEntry();
    void DownloadBootLoaderRomEntry(int num);


    // DONT CARE
    bool isFlag1Verified();
    bool isFlag2Verified();
    bool isBootLoaderRomTitleVerified();

    // DATA
    QByteArray bootLoaderEntry;
    uchar *entry;
};

#endif // BOOTLOADER_H




