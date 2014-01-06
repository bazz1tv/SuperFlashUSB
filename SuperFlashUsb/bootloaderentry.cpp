#include "bootloaderentry.h"

BootLoaderEntry::BootLoaderEntry()
{
    entry = NULL;
    //this->num = num;
}

void BootLoaderEntry::DownloadBootLoaderRomEntry()
{
    int sflash_rom_entry_startaddr = (0x7f8000+0x4000+(0x20*(num-1)));

    fprintf (stderr, "BootLoader Rom Entry @ %x\n", sflash_rom_entry_startaddr);

    ::startaddr = sflash_rom_entry_startaddr;
    ::aal = ::startaddr&0xff;
    ::aah = (::startaddr&0xff00)>>8;
    ::aab = (::startaddr&0xff0000)>>16;
    ::numbytes = 0x20;

    ::InitRead();
    ::ReadDataToBuffer(&bootLoaderEntry);


    entry = (uchar*)bootLoaderEntry.data();
}


bool BootLoaderEntry::VerifyBootLoaderRomEntry()
{
    if (!isBootLoaderRomTitleVerified())
    {
        fprintf(stderr, "Rom%d BootLoader Entry: RomTitle Failed Verification\n", num);
        return false;
    }

    if (!isFlag1Verified())
    {
        fprintf(stderr, "Rom%d BootLoader Entry: Flag1 Failed Verification\n", num);
        return false;
    }

    if (!isFlag2Verified())
    {
        fprintf(stderr, "Rom%d BootLoader Entry: Flag1 Failed Verification\n", num);
        return false;
    }

    return true;
}



///////// DONT CARE

bool BootLoaderEntry::isBootLoaderRomTitleVerified()
{
    // TEST ROM TITLE
    for (int i=1; i < 0x1d; i++)
    {
        QChar c = entry[i];
        if (!c.isPrint())
        {
            return false;
        }
    }

    return true;
}

bool BootLoaderEntry::isFlag1Verified()
{
    uchar valid_flag1[] = {0x0,0x40,0x20,0x60, 0x1, 0x41, 0x21, 0x61, 0x10, 0x50, 0x30, 0x70, 0xFF};
    // 0xff is termination value

    uchar flag1 = entry[FLAG1_OFFSET];

    int i=0;
    while(valid_flag1[i] != 0xFF)
    {
        if (flag1 == valid_flag1[i++])
        {
            return true;
        }

    }

    return false;
}

bool BootLoaderEntry::isFlag2Verified()
{
    uchar flag2 = entry[FLAG2_OFFSET];
    uchar valid_flag2[] = {0x0, 0x10, 0x20, 0x30, 0x40, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xFF};
    // 0xff is termination value

    int i=0;
    while(valid_flag2[i] != 0xFF)
    {
        if (flag2 == valid_flag2[i++])
        {
            return true;
        }

    }

    return false;
}
