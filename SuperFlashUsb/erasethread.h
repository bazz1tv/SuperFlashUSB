#ifndef ERASETHREAD_H
#define ERASETHREAD_H

#include <QFile>
#include <QThread>
#include "pc/main.h"
#include "utility.h"

class EraseThread : public QThread
{
    Q_OBJECT
public:
    EraseThread()
    {
    }
    ~EraseThread()
    {

    }

    virtual void run();
    void specialStart(int blocknum_start, int blocknum_end);

    void Erase();
    int VerifyErase();
    void EraseBlocks(int blocknum_start, int blocknum_end);
    //void EraseBlockUsingAddress(unsigned long block_address);
    void EraseBlock(int blocknum);






    bool end;
    int blocknum_start, blocknum_end;
    //QFile *file;

public slots:
    void canceled (void)
    {
      end = true;
    }

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
};


#endif // ERASETHREAD_H
