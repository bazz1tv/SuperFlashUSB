#ifndef READROMTHREAD_H
#define READROMTHREAD_H

#include <QProgressBar>
#include <QFile>
#include <QThread>
#include "pc/main.h"
#include "utility.h"

class ReadRomThread : public QThread
{
    Q_OBJECT
public:
    ReadRomThread()
    {
    }
    ~ReadRomThread()
    {

    }

    virtual void run();
    void specialStart(QString filename);
    void LoadReadBuffer();
    void Read();
    void InitRead();
    void ReadDataToFile();

    bool end;
    QFile file;
    uchar data[128];

public slots:
    void canceled(void);

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
    void setEnabledButtons(bool x);
};

#endif // READROMTHREAD_H
