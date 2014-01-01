

#include "rom_t.h"

qint64 totalRomSizeInBytes;

ROM_t::ROM_t()
{
    bootLoaderEntry.resize(0x20);
    offset=0;
    hirom = false;
    headered=false;
    RomTitle.resize(0x1c+1);
    for (uchar i=0; i < 0x1c+1; i++)
    {
        RomTitle[i]=0;
    }
    RomSizeByte=0;
    SramSizeByte=0;
    file = NULL;
    filename = "";
    finalString="";
    num=1;
    isAlreadyOnCart = true;
    deformed_header = false;
    sramsizeinbytes = 0;
}

ROM_t::~ROM_t()
{
    if (file)
    {
        file->close();
        delete file;
    }
}

int ROM_t::open()
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

bool ROM_t::isHeadered()
{
    return headered;
}

int ROM_t::setup()
{
    offset = 0;
    // How to find which item was selected?

    // Autodetect the HiROM/LoROM state

    if ((headered = is_headered(*file)))
    {
        offset = 0x200;
    }

    data = file->map(offset, file->size()-offset);
    hirom = isHirom(data);
    // stored in hirom
    int romoffset=0;
    if (hirom)
        romoffset = 0x8000;
    else
        romoffset = 0;



    for (uchar i=0; i < 21; i++)
    {
        RomTitle[i] = data[0x7fc0+romoffset+i];
    }

    // ROM Title All set

    CartTypeByte = data[0x7fd6+romoffset];

    // Get ROM Size
    RomSizeByte = data[0x7fd7+romoffset];


    // Get SRAM Size
    SramSizeByte = data[0x7fd8+romoffset];
    // Headered

    if (RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0)
    {
        //QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Something's Wrong. Did you load the correct file?"));
        if (QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("ROM Header is deformed. Load anyways?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes )
        {
            deformed_header = true;
        }

        else return -1;
    }

    // Check for Correlation between RomSizeByte and actual RomFileSize
    // map RomSizeByte to actual number of bytes

    if (deformed_header)
    {
        /*qint64 romfilesize = file->size();
            if (headered)
                romfilesize -= 0x200;*/
        romsizeinbytes = file->size()-offset;
    }
    else
    {
        romsizeinbytes = 2048<<(RomSizeByte-1);
        qint64 romfilesize = file->size();
        if (headered)
            romfilesize -= 0x200;
        if (romsizeinbytes != romfilesize)
        {
            if (QMessageBox::question(NULL, QObject::tr("Hm..."), QObject::tr("This file is not the same size as the ROM header says it should be\n\nLoad it anyways?")) == QMessageBox::No)
            {
                return -1;
            }
            else
            {
                romsizeinbytes = romfilesize;
                // NOT SURE IF I SHOULD DO THIS :D
            }
        }

        sramsizeinbytes = SramSizeByte ? 1 << (SramSizeByte + 10) : 0;

        totalRomSizeInBytes += romsizeinbytes;

        if (totalRomSizeInBytes > 0x7f8000)
        {
            QMessageBox::critical(NULL, "Too Much Data!", "There is not enough room for this ROM!");
            clear();
        }

    }

    isAlreadyOnCart = false;


    return 0;
}

void ROM_t::clear()
{
    finalString = QString("<b>%1)</b> ").arg(num);
}

//#define NOTHING 0
//#define PREPEND 1
//#define APPEND 2

QString ROM_t::romHeaderToString()
{
    QString str("");
    if (isValid())
    {
        str = QString(RomTitle)+
                QString("<p>Cart Type: ")+CartTypeStr+
                QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+
                QString("<b>ROM</b>: ")+QString(RomRamSizeByteLUT[RomSizeByte])+
                QString(" ($%1 bytes)").arg(1 << (RomSizeByte + 10),0,16)+
                QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+
                QString("<b>RAM</b>: ")+
                QString(RomRamSizeByteLUT[SramSizeByte])+
                QString(" ($%1 bytes)").arg(1 << (SramSizeByte + 10),0,16);

        if (isHeadered())
        {
            str += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EMU-Header Present");
        }
    }
    else
    {
        // PROBABLY UNIX-ONLY functional
        str = filename.mid(filename.lastIndexOf('/')+1);
    }

    return str;
}

void ROM_t::setString(QString str/*="ABCD"*/, uchar mod/*=NOTHING*/)
{
    if (str == "ABCD")
    {
        finalString = QString("<b>%1)</b> ").arg(num)+romHeaderToString();
    }
    else
    {
        if (!mod)
            finalString = QString("<b>%1)</b> ").arg(num)+str;
        else if (mod == PREPEND)
        {
            finalString = QString("<b>%1)</b> ").arg(num)+str+QString("<p>")+romHeaderToString();
        }
        else if (mod == APPEND)
        {
            finalString = QString("<b>%1)</b> ").arg(num)+romHeaderToString() + QString("<p>")+str;
        }
    }
}

bool ROM_t::isValid()
{
    if (!(RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0))
        return true;
    else return false;
}

bool ROM_t::isTypical()
{
    if (CartTypeMap.contains(CartTypeByte))
        return true;
    else return false;
}

int ROM_t::QueryUSBRomHeader()
{
    // GET THE ROM HEADER INFO

    int sflash_rom_entry_startaddr = (0x7f8000+0x4000+(0x20*(num-1)));
    //uchar *flags1;

    fprintf (stderr, "%lx\n", sflash_rom_entry_startaddr);

    // WE SHOULD FIRST VALIDATE THE SECTION AT THE 0x7f8000-0x7fffff

    // FOR SAKE OF SPEED - LET'S CHECK SOLELY THE MENU SECTION
    // 0x8a0, 0x3e0 bytes



    // THUS, READ 0X7F8000+0X8A0, 0X3E0 BYTES


    // COMPARE TO BOOTLOADER[0X8A0+]

    //
    // BEFORE WE BEGIN COMPARING VALUES AND SUCH

    ::startaddr = sflash_rom_entry_startaddr;
    aal = ::startaddr&0xff;
    aah = (::startaddr&0xff00)>>8;
    aab = (::startaddr&0xff0000)>>16;
    numbytes = 0x20;
    QByteArray entry;

    ::InitRead();
    ::ReadDataToBuffer(&entry);

    uchar *derp = (uchar*)entry.data();

    uchar flags1 = derp[0x1d];

    for (int i=0; i < 0x20; i++)
    {
        fprintf (stderr, "\\x%2x",derp[i]);
    }


    if (num == 1 && derp[0] != 0xff)
    {
        QMessageBox::warning(NULL, "UH OH", "Super Flash Bootloader Game Entry Table appears invalid. \nLet's Start from Scratch!");
        deformed_header = true;
        isAlreadyOnCart = false;
        return -2;
    }

    if (derp[0] == 0x0 || derp[0] != 0xFF || (derp[0] == 0xFF && ::zero_found) )
    {

        deformed_header = true;
        isAlreadyOnCart = false;

        if (derp[0] == 0x0)
            ::zero_found = true;
        return -1;
    }



    //if ()
    // GET THAT INFO INTO A VARIABLE
    // flags1 = blalalal
    // THEN MODIFY THE BELOW CODE


    if (flags1 & 0x40)
        startaddr = 0x200000;
    else if (flags1 & 0x20)
        startaddr = 0x400000;
    else if (flags1 & 0x60)
        startaddr = 0x600000;
    else startaddr = 0;

    // we need to read the byte

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

    if (hirom)
    {
        selectedheader = &hiromheader;
    }
    else
    {
        selectedheader = &loromheader;
    }



    for (uchar i=0; i < 21; i++)
    {
        RomTitle[i] = selectedheader->at(0x10+i);
        if (!RomTitle[i].isPrint())
            RomTitle[i] = '.';
    }

    RomTitle.toUpper();

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
    }
    else
    {
        setString("Game may be Empty/Corrupt. Or it may have a very unique ROM header. I'll display the ROM info anyways:", PREPEND);
    }

    isAlreadyOnCart = true;

    return 0;
}

int ROM_t::DoTheDo()
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

        setString();

        /*finalString = QString("<b>")+QString("%1").arg(num)+QString(") </b>")+QString(RomTitle)+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>ROM</b>: ")+QString(RomRamSizeByteLUT[RomSizeByte])+QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;")+QString("<b>SRAM</b>: ")+QString(RomRamSizeByteLUT[SramSizeByte]);
            if (isHeadered())
            {
                finalString += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EMU-Header Present");
            }*/

    }

    return 0;
}
