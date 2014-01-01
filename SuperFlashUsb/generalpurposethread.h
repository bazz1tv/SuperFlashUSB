#ifndef GENERALPURPOSETHREAD_H
#define GENERALPURPOSETHREAD_H

#include <QThread>
//#include "mainwindow.h"
#include <QtWidgets>


//class MainWindow;


#define CONNECT 88

class GeneralPurposeThread : public QThread
{
    Q_OBJECT
public:
    GeneralPurposeThread()
    {

    }

    void specialStart(int operation);
    virtual void run();

    int operation;
    bool end;
    bool msg_complete;
    //QEventLoop ev;
    //extern MainWindow *w;
    void sendmessage(int msgtype, QString title, QString msg);

public slots:
    void canceled(void);
    void message_complete(void);

signals:
    void setProgress(int x);
    void setProgress(int min, int max, int val);
    void message(int msgtype, QString title, QString msg);
    void setEnabledButtons(bool x);
};

#endif // GENERALPURPOSETHREAD_H
