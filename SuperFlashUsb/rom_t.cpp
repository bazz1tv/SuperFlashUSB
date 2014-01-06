

#include "rom_t.h"

qint64 totalRomSizeInBytes;

ROM_t::ROM_t()
{
    initialized=false;
    //bootLoaderEntryRomTitle.resize(0x20);
    fileoffset=0;
    //hirom = false;
    //headered=false;
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
    isAlreadyOnCart = false;
    //deformed_header = false;
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

bool ROM_t::isHirom()
{
    return ::isHirom(data);
}

int ROM_t::openFile(QIODevice::OpenModeFlag flags, QString filename/*=this->filename*/)
{
    if (filename == "")
    {
        filename = this->filename;
    }
    else
    {
        this->filename = filename;
    }

    if (!filename.isEmpty())
    {
        if (file)
        {
            file->close();
            delete file;
            file = NULL;
        }

        file = new QFile(filename);

        if (!file->open(flags))
        {
            QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Could not open file"));
            return -1;
        }

        return 1;
    }

    return -1;
}

bool ROM_t::isHeadered()
{
    if (file->size() % 1024 == 512)
    {
        return true;
    }

    return false;

}

bool ROM_t::deformedHeader()
{
    return (RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0);
}

uchar* ROM_t::mapFile()
{
    fileoffset = 0;
    // How to find which item was selected?

    // Autodetect the HiROM/LoROM state

    if (isHeadered())
    {
        fileoffset = 0x200;
    }

    return data = file->map(fileoffset, file->size()-fileoffset);
}

void ROM_t::loadRomTitleFromMap()
{
    for (uchar i=0; i < 21; i++)
    {
        RomTitle[i] = data[0x7fc0+romheaderoffset+i];

        if (!RomTitle[i].isPrint())
            RomTitle[i] = '.';
    }

    RomTitle = RomTitle.toUpper();
}

//
int ROM_t::setup()
{
    // This part assumes *data
    int romheaderoffset=0;          // LoRom
    if (isHirom())
        romheaderoffset = 0x8000;   // HiRom



    loadRomTitleFromMap();

    // ROM Title All set
    CartTypeByte = data[0x7fd6+romheaderoffset];
    if (CartTypeMap.contains(CartTypeByte))
            CartTypeStr = QString(CartTypeMap[CartTypeByte]);
        else CartTypeStr = QString("Unknown Cart Type");

    // Get ROM Size
    RomSizeByte = data[0x7fd7+romheaderoffset];


    // Get SRAM Size
    SramSizeByte = data[0x7fd8+romheaderoffset];
    // Headered

    if (deformedHeader())
    {
        //QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Something's Wrong. Did you load the correct file?"));
        if (QMessageBox::warning(NULL, QObject::tr("Error"), QObject::tr("ROM Header is deformed. Load anyways?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes )
        {
            romsizeinbytes = file->size()-fileoffset;
        }

        else return -1;
    }
    else
    {
        romsizeinbytes = 2048<<(RomSizeByte-1);
        qint64 romfilesize = file->size();
        if (isHeadered())
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

        ::totalRomSizeInBytes += romsizeinbytes;

        if (::totalRomSizeInBytes > 0x7f8000)
        {
            QMessageBox::critical(NULL, "Too Much Data!", "There is not enough room for this ROM!");
            clear();
        }

    }

    isAlreadyOnCart = false;
    initialized = true;

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
        //this->startaddr = 0x200000;
        str = QString(RomTitle);

        if (isAlreadyOnCart)
        {
            str += QString("<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ROM-ON-CART");
            str += QString("<p>Start Address: $%1").arg(startaddr,6,16,QChar('0'));
        }

        str += QString("<p>Cart Type: ")+CartTypeStr+
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
        //else
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
    if (!(RomSizeByte > 14 || SramSizeByte > 14 || RomSizeByte == 0 || !CartTypeMap.contains(CartTypeByte)))
        return true;
    else return false;
}




int ROM_t::load()
{
    if (openFile(QIODevice::ReadOnly) < 0)
    {
       fprintf(stderr, "ROM%d: openFile() failed\n", num);
       return -1;
    }


    if (file->size() < 0x8000)
    {
        QMessageBox::warning(NULL, QObject::tr("Error"), file->fileName() + QObject::tr(" is not large enough a file"));
        return -1;
    }

    // Load our uchar *data pointer with raw data of the file
    mapFile();


    if (setup() < 0)
    {
        fprintf(stderr, "ROM%d: setup() failed\n", num);
        return -1;
    }

    setString();

    initialized = true;

    return 0;
}
