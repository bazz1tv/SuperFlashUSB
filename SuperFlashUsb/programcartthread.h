#ifndef PROGRAMCARTTHREAD_H
#define PROGRAMCARTTHREAD_H

#include "utility.h"

#include <QThread>

class ProgramCartThread : public QThread
{
    Q_OBJECT
public:
    ProgramCartThread()
    {
        rom1=NULL;
        rom2=NULL;
        rom3=NULL;
        rom4=NULL;
    }
    ~ProgramCartThread()
    {

    }

    virtual void run();
    void specialStart(ROM_t *rom1, ROM_t *rom2, ROM_t *rom3, ROM_t *rom4);

    bool end;
    ROM_t *rom1,*rom2,*rom3,*rom4;

public slots:
    void canceled(void)
    {
        end = true;
    }

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
};

#endif // PROGRAMCARTTHREAD_H
