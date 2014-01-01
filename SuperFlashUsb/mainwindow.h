#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "utility.h"
#include "usbthread.h"
#include "readromthread.h"
#include "writesramthread.h"
#include "programcartthread.h"
#include "generalpurposethread.h"

extern QString bootloader;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void clearAll();
    QStatusBar *statusBar;

public slots:
    void setProgress(int x);
    void setProgress(int min, int max, int val);
    void message(int msgtype, QString title, QString msg);
    void setEnabledButtons (bool state);


private slots:
    void on_actionQuit_triggered();
    void connect_USB();
    void on_pushButton_Cart_Read_clicked();
    void on_pushButton_USBConnect_clicked();
    void on_writeSramButton_clicked();
    void on_readSramButton_clicked();
    void on_programRomButton_clicked();

public:
    void QueryUSBRomHeaders();

    Ui::MainWindow *ui;

    QTimer *eventTimer;
    USBThread *usbthread;
    ReadRomThread *readRomThread;
    WriteSramThread *writeSramThread;
    ProgramCartThread *programCartThread;
    GeneralPurposeThread *gpThread;
    bool timeToUpdateRomHeaders;
    bool timeToClearAll;
    //QEventLoop ev;



signals:
    void cancelUSBThread(void);
    void cancelReadRomThread(void);
    void cancelWriteSramThread(void);
    void cancelAll(void);
    void message_complete(void);
};



#endif // MAINWINDOW_H
