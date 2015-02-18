#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pc/main.h"
#include "readcartdialog.h"
#include "utility.h"
#include "pc/sflash/read.h"

#include <QtWidgets>

void MainWindow::setProgress(int x)
{
    ui->progressBar->setValue(x);
}

void MainWindow::setProgress(int min, int max, int val)
{
    ui->progressBar->setMinimum(min);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(val);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timeToClearAll = false;
    timeToUpdateRomHeaders = true;
    USBconnected = false;
    //
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
    CartTypeMap[0xF3] =      QString("ROM+Custom                 (CX4)");   // :D

    ui->setupUi(this);
    statusBar = QMainWindow::statusBar();


    //ui->progressBar->reset();
    ui->progressBar->setValue(100);
    ui->progressBar->setFormat("Progress Bar");

    ui->romEntry1->setNum(1);
    ui->romEntry2->setNum(2);
    ui->romEntry3->setNum(3);
    ui->romEntry4->setNum(4);

    ui->romEntry1->finalString = "<b>1) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEntry2->finalString = "<b>2) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEntry3->finalString = "<b>3) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->romEntry4->finalString = "<b>4) </b> &lt;DRAG ROM-FILE HERE&gt;";

    ui->sramEdit->finalString = "&lt;DRAG SAVE-FILE HERE&gt;";

    ui->sramEdit->updateText();
    ui->romEntry1->updateText();
    ui->romEntry2->updateText();
    ui->romEntry3->updateText();
    ui->romEntry4->updateText();




    // init libUSB
    InitUSB();

    if (OpenUSBDevice() >= 0)
        USBconnected = true;

    if (isHotPluggable)
    {
        usbthread = new USBThread;
        connect (this, SIGNAL (cancelUSBThread (void)), usbthread, SLOT (canceled (void)));
        connect (this, SIGNAL (cancelAll (void)), usbthread, SLOT (canceled (void)));
        usbthread->start();
    }

    readRomThread = new ReadRomThread;
    connect (this, SIGNAL(cancelReadRomThread(void)), readRomThread, SLOT(canceled()));
    connect (this, SIGNAL(cancelAll(void)), readRomThread, SLOT(canceled()));
    connect (readRomThread, SIGNAL(setProgress(int)), this, SLOT(setProgress(int)));
    connect (readRomThread, SIGNAL(message(int,QString,QString)), this, SLOT(message(int,QString,QString)));
    connect (readRomThread, SIGNAL(setEnabledButtons(bool)), this, SLOT(setEnabledButtons(bool)));
    writeSramThread = new WriteSramThread;
        connect (this, SIGNAL(cancelWriteSramThread(void)), writeSramThread, SLOT(canceled()));
        connect (this, SIGNAL(cancelAll(void)), writeSramThread, SLOT(canceled()));
        connect (writeSramThread, SIGNAL(setProgress(int)), this, SLOT(setProgress(int)));
        connect (writeSramThread, SIGNAL(message(int,QString,QString)), this, SLOT(message(int,QString,QString)));
        connect (writeSramThread, SIGNAL(setEnabledButtons(bool)), this, SLOT(setEnabledButtons(bool)));

    programCartThread = new ProgramCartThread;
        connect (this, SIGNAL(cancelProgramCartThread(void)), programCartThread, SLOT(canceled()));
        connect (this, SIGNAL(cancelAll()), programCartThread, SLOT(canceled()));
        connect (programCartThread, SIGNAL(setProgress(int,int,int)), this, SLOT(setProgress(int,int,int)));
        connect (programCartThread, SIGNAL(setProgress(int)), this, SLOT(setProgress(int)));
        connect (programCartThread, SIGNAL(message(int,QString,QString)), this, SLOT(message(int,QString,QString)));
        connect (programCartThread, SIGNAL(setEnabledButtons(bool)), this, SLOT(setEnabledButtons(bool)));

    gpThread = new GeneralPurposeThread;
        connect (this, SIGNAL(cancelgpThread(void)), gpThread, SLOT(canceled()));
        connect (this, SIGNAL(cancelAll()), gpThread, SLOT(canceled()));
        connect (gpThread, SIGNAL(setProgress(int,int,int)), this, SLOT(setProgress(int,int,int)));
        connect (gpThread, SIGNAL(setProgress(int)), this, SLOT(setProgress(int)));
        connect (gpThread, SIGNAL(message(int,QString,QString)), this, SLOT(message(int,QString,QString)));
        connect (gpThread, SIGNAL(setEnabledButtons(bool)), this, SLOT(setEnabledButtons(bool)));
        connect (this, SIGNAL(message_complete()), gpThread, SLOT(message_complete()));
        connect (gpThread, SIGNAL(queryRomHeaders()), this, SLOT(queryRomHeaders()));

         //connect()

    connect (ui->cancelButton, SIGNAL (clicked ()), readRomThread, SLOT (canceled ()));
    connect (ui->cancelButton, SIGNAL(clicked()), writeSramThread, SLOT(canceled()));
    connect (ui->cancelButton, SIGNAL(clicked()), programCartThread, SLOT(canceled()));
    connect (ui->cancelButton, SIGNAL(clicked()), gpThread, SLOT(canceled()));

    eventTimer = new QTimer(this);
    connect(eventTimer,SIGNAL(timeout()),this,SLOT(connect_USB()));

    eventTimer->start(1000);
    setEnabledButtons(true);
}

void MainWindow::message(int msgtype, QString title, QString msg)
{
    switch (msgtype)
    {
        case QMessageBox::Warning:
        QMessageBox::warning(this, title,msg);
        break;

    case QMessageBox::Information:
        QMessageBox::information(this, title,msg);
       break;
    case QMessageBox::Question:
        QMessageBox::question(this, title,msg);
        break;
      case QMessageBox::Critical:
        QMessageBox::critical(this, title, msg);
        break;
    }

    emit message_complete();
}

MainWindow::~MainWindow()
{
    emit cancelAll();

    //emit cancelUSBThread();
    CloseUSBDevice();




    EndUSB();
    delete usbthread;
    delete readRomThread;
    delete writeSramThread;
    delete programCartThread;
    delete ui;
    delete eventTimer;
}

void MainWindow::clearAll()
{
    ui->romEntry1->setHtml("<b>1)</b> ");
    ui->romEntry2->setHtml("<b>2)</b> ");
    ui->romEntry3->setHtml("<b>3)</b> ");
    ui->romEntry4->setHtml("<b>4)</b> ");
    ui->sramEdit->clear();
}

// don't worry about it
//
void MainWindow::connect_USB()
{
    if (timeToClearAll)
    {
        clearAll();
        timeToClearAll = false;
    }
    if (!isHotPluggable && !USBconnected)
    {
        if (OpenUSBDevice() < 0)
        {
            statusBar->showMessage("USB Device Not Connected or Not Found");
        }
        else
        {
            statusBar->showMessage("USB Device Connected");
            USBconnected = true;
        }
    }

    if (timeToUpdateRomHeaders && USBconnected)
    {

        timeToUpdateRomHeaders = false;
        //
        gpThread->specialStart(CONNECT);
        //QueryUSBRomHeaders();
        //queryRomHeaders();
    }

}

void MainWindow::queryGameEntries()
{

}

void MainWindow::queryRomHeaders()
{
    if (ui->romEntry1->QueryUSBRomHeader() == -2)
    {
        //ui->romEntry2->rom.deformed_header = true;
        ui->romEntry2->game.rom.isAlreadyOnCart = false;

        //ui->romEntry3->rom.deformed_header = true;
        ui->romEntry3->game.rom.isAlreadyOnCart = false;

        //ui->romEntry4->rom.deformed_header = true;
        ui->romEntry4->game.rom.isAlreadyOnCart = false;

        return;
    }
    ui->romEntry2->QueryUSBRomHeader();
    ui->romEntry3->QueryUSBRomHeader();
    ui->romEntry4->QueryUSBRomHeader();
}

void MainWindow::setEnabledButtons (bool state)
{
    ui->readSramButton->setEnabled(state);
    ui->programRomButton->setEnabled(state);
    ui->pushButton_Cart_Read->setEnabled(state);
    ui->pushButton_USBConnect->setEnabled(state);
    ui->pushButton_USBDisconnect->setEnabled(state);
    ui->writeSramButton->setEnabled(state);

    ui->cancelButton->setEnabled(!state);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}


void MainWindow::on_pushButton_Cart_Read_clicked()
{
    if (!USBconnected)
    {
        QMessageBox::warning(this, "ROM Read", "USB Programmer not connected!");
        return;
    }

    setEnabledButtons(false);

    int accepted=0;

    // Create a Popup that selects the Start Address, how many bytes to Read From the Cartridge
    ReadCartDialog d;// = new ReadCartDialog;
    accepted = d.exec();
    QString filename;

    if (accepted == QDialog::Accepted)
    {
        rom_or_sram = ROM;
        // Query a Filename to Save to
        filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), QString("derp.bin"),
                                                           QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
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

        // Call Read Thread HERE
        readRomThread->specialStart(filename);
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
        //eventTimer->start();
        if (OpenUSBDevice() < 0)
        {
            statusBar->showMessage("USB Device Not Found");
            QMessageBox::warning(this, "USB","Device not found!");
        }
        else
        {
            statusBar->showMessage("USB Device Connected");
            QMessageBox::information(this, "USB","Connected! :D");
        }
    }
    else
    {
        QMessageBox::information(this, "USB","Already Connected");
        statusBar->showMessage("USB Device Already Connected");
    }
}

void MainWindow::on_writeSramButton_clicked()
{
    if (!USBconnected)  // USBconnected GLobal variable from USB.cpp/h
    {
        QMessageBox::warning(this, "SRAM Write", "USB Programmer not connected!");
        return;
    }

    if (ui->sramEdit->sram.filename == "")
    {
        QMessageBox::warning(this, "SRAM Write", "Cannot write! No File Specified!");
        return;
    }

    // 0xfe0000 :)  Tototek SRAM Location $2000 bytes
    aal = aah = 0;
    aab = 0xfe;

    if (ui->sramEdit->sram.file->size() <= 0x2000)
    {
        numbytes = ui->sramEdit->sram.file->size();
    }
    else
    {
        QMessageBox::critical(this, "SRAM Write", "File too Big!");
        return;
    }

    ui->progressBar->setMaximum(numbytes);
    ui->progressBar->setMinimum(0);
    ui->progressBar->resetFormat();

    rom_or_sram = SRAM;

    //ui->sramEdit->sram.file->seek(ui->sramEdit->sram.off)

    writeSramThread->specialStart(ui->sramEdit->sram.file);
}



void MainWindow::on_readSramButton_clicked()
{
    if (!USBconnected)  // USBconnected GLobal variable from USB.cpp/h
    {
        QMessageBox::warning(this, "SRAM Read", "USB Programmer not connected!");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), QString("derp.srm"),
                                                       QObject::tr("SRAM Files (*.sav *.srm);; Any (*.*)"));
    startaddr = 0xfe0000;
    numbytes = 0x2000;

    // setup progress bar
    ui->progressBar->setMaximum(numbytes);
    ui->progressBar->setMinimum(0);
    ui->progressBar->resetFormat();

    aal = startaddr&0xff;
    aah = (startaddr&0xff00)>>8;
    aab = (startaddr&0xff0000)>>16;

    // Call Read Thread HERE
    readRomThread->specialStart(filename);

}

void MainWindow::on_programRomButton_clicked()
{
    if (!USBconnected)  // USBconnected GLobal variable from USB.cpp/h
    {
        QMessageBox::warning(this, "ROM Program", "USB Programmer not connected!");
        return;
    }

    programCartThread->specialStart(&ui->romEntry1->game.rom,&ui->romEntry2->game.rom,&ui->romEntry3->game.rom,&ui->romEntry4->game.rom);




}


void MainWindow::on_pushButton_USBDisconnect_clicked()
{
    if (::USBconnected)
    {
        ::CloseUSBDevice();
        QMessageBox::information(this,"USB Disconnect", "Device has been Disconnected");
        statusBar->showMessage("USB Device Disconnected");
    }
    else
    {
        QMessageBox::information(this, "USB Disconnect", "Already Disconnected");
        statusBar->showMessage("USB Device Already Disconnected");
    }
}
