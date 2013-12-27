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
    void specialStart(QProgressBar *pb, QString filename);

    bool end;
    QProgressBar *progressBar;
    QFile file;

public slots:
    void canceled(void);
};

#endif // READROMTHREAD_H
