#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pc/main.h"
#include "readcartdialog.h"
#include "utility.h"
#include "pc/sflash/read.h"

#include <QtWidgets>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    CartTypeMap[0x00] =      QString("ROM");                 // if gamecode="042J" --> ROM+SGB2
    CartTypeMap[0x01] =      QString("ROM+RAM");             // (if any such produced?)
    CartTypeMap[0x02] =      QString("ROM+RAM+Battery");     // ;if gamecode="XBND" --> ROM+RAM+Batt+XBandModem
                                                            // ;if gamecode="MENU" --> ROM+RAM+Batt+Nintendo Power
    CartTypeMap[0x03] =      QString("ROM+DSP");
    CartTypeMap[0x04] =      QString("ROM+DSP+RAM");         //  (no such produced)
    CartTypeMap[0x05] =      QString("ROM+DSP+RAM+Battery");
    CartTypeMap[0x13] =      QString("ROM+MarioChip1/ExpansionRAM (or hacked version of OBC1)");

    CartTypeMap[0x14] =      QString("ROM+GSU+RAM");         // ROM size up to 1MByte -> GSU1
    CartTypeMap[0x15] =      QString("ROM+GSU+RAM+Battery"); //            ;/ROM size above 1MByte -> GSU2
    CartTypeMap[0x1A] =      QString("ROM+GSU1+RAM+Battery+Fast Mode? (Stunt Race)");
    CartTypeMap[0x25] =      QString("ROM+OBC1+RAM+Battery");
    CartTypeMap[0x32] =      QString("ROM+SA1+RAM+Battery");    // F1 Grand Prix Sample (J)"
    CartTypeMap[0x34] =      QString("ROM+SA1+RAM"); // (?) "Dragon Ball Z - Hyper Dimension"
    CartTypeMap[0x35] =      QString("ROM+SA1+RAM+Battery");
    CartTypeMap[0x43] =      QString("ROM+S-DD1");
    CartTypeMap[0x45] =      QString("ROM+S-DD1+RAM+Battery");
    CartTypeMap[0x55] =      QString("ROM+S-RTC+RAM+Battery");
    CartTypeMap[0xE3] =      QString("ROM+Super Gameboy      (SGB)");
    CartTypeMap[0xE5] =      QString("ROM+Satellaview BIOS   (BS-X)");
    CartTypeMap[0xF5] =      QString("ROM+Custom+RAM+Battery     (SPC7110)");
    CartTypeMap[0xF9] =      QString("ROM+Custom+RAM+Battery+RTC (SPC7110+RTC)");
    CartTypeMap[0xF6] =      QString("ROM+Custom+Battery         (ST010/ST011)");
    CartTypeMap[0xF5] =      QString("ROM+Custom+RAM+Battery     (ST018)");
    CartTypeMap[0xF3] =      QString("ROM+Custom                 (CX4)");

    ui->setupUi(this);
    statusBar = QMainWindow::statusBar();


    //ui->progressBar->reset();
    ui->progressBar->setValue(100);
    ui->progressBar->setFormat("Progress Bar");

    ui->romEdit1->rom.num = 1;
     ui->romEdit1->rom.startaddr = 0;
    ui->romEdit2->rom.num = 2;
     ui->romEdit2->rom.startaddr = 0x200000;
    ui->romEdit3->rom.num = 3;
     ui->romEdit3->rom.startaddr = 0x400000;
    ui->romEdit4->rom.num = 4;
     ui->romEdit4->rom.startaddr = 0x600000;

    ui->romEdit1->rom.finalString = "<b>1) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEdit2->rom.finalString = "<b>2) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEdit3->rom.finalString = "<b>3) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEdit4->rom.finalString = "<b>4) </b> &lt;DRAG ROM-FILE HERE&gt;";

    ui->sramEdit->sram.finalString = "&lt;DRAG SAVE-FILE HERE&gt;";

    ui->sramEdit->setHtml(ui->sramEdit->sram.finalString);
    ui->romEdit1->setHtml(ui->romEdit1->rom.finalString);
    ui->romEdit2->setHtml(ui->romEdit2->rom.finalString);
    ui->romEdit3->setHtml(ui->romEdit3->rom.finalString);
    ui->romEdit4->setHtml(ui->romEdit4->rom.finalString);




    // init libUSB
    InitUSB();
    usbthread = new USBThread;
    usbthread->start();
    //OpenUSBDevice();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(connect_USB()));
    connect (this, SIGNAL (cancelUSBThread (void)), usbthread, SLOT (canceled (void)));
    //timer->start(1000);
}

MainWindow::~MainWindow()
{
    emit cancelUSBThread();
    CloseUSBDevice();




    EndUSB();
    delete usbthread;
    delete ui;
    delete timer;
}


// don't worry about it
//
void MainWindow::connect_USB()
{
    //InitUSB();
    /*if (OpenUSBDevice() < 0)
    {
        statusBar->showMessage("USB Device Not Connected or Not Found");
    }
    else
    {
        statusBar->showMessage("USB Device Connected");
        timer->stop();

        // Update the ROM Info's

        // main function
        QueryUSBRomHeaders();

        //statusBar->showMessage("Used Space: 0Mb | Available Space: 64Mb");

    }*/

    //libusb_handle_events(NULL);

    timeval tv;
    tv.tv_sec=0;
    tv.tv_usec=0;
    //libusb_handle_events(ctx);
    r =libusb_handle_events_timeout(ctx,&tv);
    if (r < 0)
       fprintf(stderr, "libusb_handle_events() failed: %s\n", libusb_error_name(r));

}

void MainWindow::QueryUSBRomHeaders()
{
    ui->romEdit1->QueryUSBRomHeader();
    ui->romEdit2->QueryUSBRomHeader();
    ui->romEdit3->QueryUSBRomHeader();
    ui->romEdit4->QueryUSBRomHeader();
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}


void MainWindow::on_pushButton_Cart_Read_clicked()
{
    int accepted=0;
    ROM_t rom;
    // Create a Popup that selects the Start Address, how many bytes to Read From the Cartridge
    //QDialog dialog(this);
    ReadCartDialog d;// = new ReadCartDialog;
    accepted = d.exec();

    if (accepted == QDialog::Accepted)
    {

        // Query a Filename to Save to
        rom.filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), QString("derp.bin"),
                                                           QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
        if (rom.open() < 0)
        {
            QMessageBox::critical(this, "File Error", "Could not Open File");
            return;
        }
        startaddr = d.getStartAddress();
        numbytes = d.getNumBytes();

        // setup progress bar
        ui->progressBar->setMaximum(numbytes);
        ui->progressBar->setMinimum(0);
        ui->progressBar->resetFormat();

        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        // Now Call our PC Command line functions to Read the Cart

        Read(ui->progressBar, rom.file, true, NULL);


        // we are finished
    }
    else
    {
        // Nothing
    }

}





void MainWindow::on_pushButton_USBConnect_clicked()
{
    if (dev_handle == NULL)
    {
        //timer->start();
    }
    else
    {
        QMessageBox::information(this, "USB","Already Connected");
    }
}
