#ifndef ROM_T_H
#define ROM_T_H

#include "utility.h"

extern QString RomRamSizeByteLUT[];
extern QMap<int,QString> CartTypeMap;
extern qint64 totalRomSizeInBytes;

#define NOTHING 0
#define PREPEND 1
#define APPEND 2

class ROM_t
{
public:
    ROM_t();
    ~ROM_t();

    uchar* mapFile();
    int openFile(QString filename, QIODevice::OpenModeFlag flags);
    bool isHeadered();

    int setup();
    void clear();

    QString romHeaderToString();

    QString setString(QString str="ABCD", uchar mod=NOTHING);

    void loadRomTitleFromMap();

    bool isHirom();
    bool isValid();

    //int QueryUSBRomHeader();


    int load(QString filename, QIODevice::OpenModeFlag flags);

    bool deformedHeader();

    qint64 romsizeinbytes;
    qint64 sramsizeinbytes;
    bool isAlreadyOnCart;
    //bool hirom;
    uchar *data;
    qint64 fileoffset;
    //bool headered;
    QString RomTitle;

    //
    u_int8_t RomSizeByte;
    u_int8_t SramSizeByte;
    u_int8_t CartTypeByte;
    //
    QString CartTypeStr;
    //
    QFile *file;
    QString filename;
    //
    int startaddr;
    quint8 num;
    //
    QString finalString;
    bool initialized;

};

#endif // ROM_T_H
