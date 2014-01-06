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

    int open();
    bool isHeadered();

    int setup();


    void clear();




    QString romHeaderToString();

    void setString(QString str="ABCD", uchar mod=NOTHING);


    bool isValid();


    bool isTypical();

    //bool VerifyBootLoaderRomEntry(uchar *entry);
    uchar* DownloadBootLoaderRomEntry();

    int QueryUSBRomHeader();


    int DoTheDo();


    bool deformed_header;
    qint64 romsizeinbytes;
    qint64 sramsizeinbytes;
    bool isAlreadyOnCart;
    bool hirom;
    uchar *data;
    ushort offset;
    bool headered;
    QString RomTitle;
    u_int8_t RomSizeByte;
    u_int8_t SramSizeByte;
    u_int8_t CartTypeByte;
    QString CartTypeStr;
    QFile *file;
    QString filename;
    int startaddr;
    quint8 num;
    QString finalString;
    QString bootLoaderEntryRomTitle;
    bool exists;
    QByteArray bootLoaderEntry;
};

#endif // ROM_T_H
