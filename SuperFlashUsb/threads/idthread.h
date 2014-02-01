#ifndef IDTHREAD_H
#define IDTHREAD_H

#include <QThread>
#include "pc/main.h"
#include "pc/sflash.h"

class IDThread : public QThread
{
    Q_OBJECT
public:
    IDThread();

    void specialStart(int dowhat);
    virtual void run();
    void GetLockBits();
    void GetChipID();
    void ReadIDCodes();

    bool end;
    int dowhat;
    //QFile file;

public slots:
    void canceled(void);

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
};

#endif // IDTHREAD_H
