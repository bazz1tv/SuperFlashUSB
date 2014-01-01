#ifndef GENERALPURPOSETHREAD_H
#define GENERALPURPOSETHREAD_H

#include <QThread>

class GeneralPurposeThread : public QThread
{
    Q_OBJECT
public:
    GeneralPurposeThread() {}


    virtual void run();

    int operation;
    bool end;

public slots:
    void canceled(void);

signals:
    void setProgress(int x);
    void message(int msgtype, QString title, QString msg);
    void setEnabledButtons(bool x);
};

#endif // GENERALPURPOSETHREAD_H
