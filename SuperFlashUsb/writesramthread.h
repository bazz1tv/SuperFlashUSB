#ifndef WRITESRAMTHREAD_H
#define WRITESRAMTHREAD_H

#include <QFile>
#include <QThread>
#include "pc/main.h"
#include "utility.h"

class WriteSramThread : public QThread
{
    Q_OBJECT
public:
    WriteSramThread()
    {
    }
    ~WriteSramThread()
    {

    }

    int VerifyWrite();
    void InitWrite();
    void Write();
    void LoadWriteBuffer();
    void WriteDataFromFile();

    virtual void run();
    void specialStart(QFile *file);





    bool end;
    QFile *file;

public slots:
    void canceled(void);

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
};


#endif // WRITESRAMTHREAD_H
