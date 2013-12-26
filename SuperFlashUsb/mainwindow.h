#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "utility.h"
//#include "readcartdialog.h"
#include "usbthread.h"


namespace Ui {
class MainWindow;
//class ReadCartDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    //void derp(QDragEnterEvent *);
    //void derp2(QDropEvent *event);
    void on_actionQuit_triggered();
    void connect_USB();
    /*void ShowContextMenu1(const QPoint& pos);
    void ShowContextMenu2(const QPoint& pos);
    void ShowContextMenu3(const QPoint& pos);
    void ShowContextMenu4(const QPoint& pos);*/



    void on_pushButton_Cart_Read_clicked();

    //void on_pushButton_4_clicked();

    void on_pushButton_USBConnect_clicked();



private:
    void QueryUSBRomHeaders();

    Ui::MainWindow *ui;
    QStatusBar *statusBar;
    QTimer *timer;
    USBThread *usbthread;

signals:
    void cancelUSBThread(void);
};



#endif // MAINWINDOW_H
