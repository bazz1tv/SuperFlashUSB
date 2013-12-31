#ifndef SRAM_T_H
#define SRAM_T_H

#include <QtWidgets>

class SRAM_t
{
public:
    SRAM_t()
    {

        file = NULL;
        filename = "";
        finalString="";

    }

    ~SRAM_t()
    {
        if (file)
        {
            file->close();
            delete file;
        }
    }




    int setup()
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


            if (file->size() > 0x20000)
            {
                QMessageBox::warning(NULL, QObject::tr("Error"), file->fileName() + QObject::tr("\nFile is too big"));
                return -1;
            }


            // This might only work for Linux/Unix based OS
            finalString = filename;//filename.mid(filename.lastIndexOf('/')+1);

            data = file->map(0, file->size());
        }


        return 0;
    }

    QFile *file;
    QString filename;
    QString finalString;
    uchar *data;
};

#endif // SRAM_T_H
