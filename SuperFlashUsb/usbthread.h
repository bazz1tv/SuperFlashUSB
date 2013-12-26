#ifndef USBTHREAD_H
#define USBTHREAD_H

#include <QThread>
#include "pc/main.h"

class USBThread : public QThread
{
    Q_OBJECT
public:
    USBThread()
    {
    }
    virtual void run();
    bool end;

public slots:
    void canceled(void);
};

#endif // USBTHREAD_H
