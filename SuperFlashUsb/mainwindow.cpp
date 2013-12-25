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
    CartTypeMap[0x13] =      QString("ROM+MarioChip1/ExpansionRAM (and hacked version of OBC1)");

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
    //ui->statusBar;

    //statusBar->showMessage("Used Space: 0Mb | Available Space: 64Mb");

    //ui->progressBar->reset();
    ui->progressBar->setValue(100);
    ui->progressBar->setFormat("Progress Bar");

    ui->textEdit1->rom.num = 1;
    ui->textEdit2->rom.num = 2;
    ui->textEdit3->rom.num = 3;
    ui->textEdit4->rom.num = 4;

    ui->textEdit1->rom.finalString = "<b>1) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->textEdit2->rom.finalString = "<b>2) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->textEdit3->rom.finalString = "<b>3) </b> &lt;DRAG ROM-FILE HERE&gt;";
    ui->textEdit4->rom.finalString = "<b>4) </b> &lt;DRAG ROM-FILE HERE&gt;";

    ui->textEdit->sram.finalString = "&lt;DRAG SAVE-FILE HERE&gt;";

    ui->textEdit->setHtml(ui->textEdit->sram.finalString);
    ui->textEdit1->setHtml(ui->textEdit1->rom.finalString);
    ui->textEdit2->setHtml(ui->textEdit2->rom.finalString);
    ui->textEdit3->setHtml(ui->textEdit3->rom.finalString);
    ui->textEdit4->setHtml(ui->textEdit4->rom.finalString);




    // init libUSB
    InitUSB();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(connect_USB()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    CloseUSBDevice();
    EndUSB();
    delete ui;
    delete timer;
}


// don't worry about it
//
void MainWindow::connect_USB()
{
    //InitUSB();
    if (OpenUSBDevice() < 0)
    {
        statusBar->showMessage("USB Device Not Connected or Not Found");
    }
    else
    {
        statusBar->showMessage("USB Device Connected");
        timer->stop();

        // Update the ROM Info's
        startaddr = 0x7fb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;

        QByteArray loromheader,hiromheader, *selectedheader;
        Read(progressBar, NULL, false, &loromheader);

        startaddr = 0xffb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;
        Read(progressBar, NULL, false, &hiromheader);

        bool hirom= isHirom2((uchar*)loromheader.data(), (uchar*)hiromheader.data());
        //int offset;
        if (hirom)
        {
            //offset = 1;
            selectedheader = &hiromheader;
            //QMessageBox::critical(this, "Derp", "HiRom");
        }
        else
        {
            selectedheader = &loromheader;
            //QMessageBox::critical(this, "Derp", "LoRom");
        }



        for (uchar i=0; i < 21; i++)
        {
            ui->textEdit1->rom.RomTitle[i] = selectedheader->at(0x10+i);
        }
        //RomTitle[21] = 0;
        // ROM Title All set
        ui->textEdit1->rom.CartTypeByte = selectedheader->at(0x16+0x10);

        // Get ROM Size
        ui->textEdit1->rom.RomSizeByte = selectedheader->at(0x17+0x10);


        // Get SRAM Size
        ui->textEdit1->rom.SramSizeByte = selectedheader->at(0x18+0x10);

        if (ui->textEdit1->rom.isValid())
        {
            ui->textEdit1->rom.setString();
            ui->textEdit1->setHtml(ui->textEdit1->rom.finalString);
        }



        ///////////
        // Update the ROM Info's
        startaddr = 0x207fb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;

        //QByteArray loromheader,hiromheader, *selectedheader;
        Read(progressBar, NULL, false, &loromheader);

        startaddr = 0x20ffb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;
        Read(progressBar, NULL, false, &hiromheader);

        hirom= isHirom2((uchar*)loromheader.data(), (uchar*)hiromheader.data());
        //int offset;
        if (hirom)
        {
            //offset = 1;
            selectedheader = &hiromheader;
            //QMessageBox::critical(this, "Derp", "HiRom");
        }
        else
        {
            selectedheader = &loromheader;
            //QMessageBox::critical(this, "Derp", "LoRom");
        }

        for (uchar i=0; i < 21; i++)
        {
            ui->textEdit2->rom.RomTitle[i] = selectedheader->at(0x10+i);
        }
        //RomTitle[21] = 0;
        // ROM Title All set
        ui->textEdit2->rom.CartTypeByte = selectedheader->at(0x16+0x10);

        // Get ROM Size
        ui->textEdit2->rom.RomSizeByte = selectedheader->at(0x17+0x10);


        // Get SRAM Size
        ui->textEdit2->rom.SramSizeByte = selectedheader->at(0x18+0x10);

        if (ui->textEdit2->rom.isValid())
        {
            ui->textEdit2->rom.setString();
            ui->textEdit2->setHtml(ui->textEdit2->rom.finalString);
        }
        ///////////////////////
        // Update the ROM Info's
        startaddr = 0x407fb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;

        //QByteArray loromheader,hiromheader, *selectedheader;
        Read(progressBar, NULL, false, &loromheader);

        startaddr = 0x40ffb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;
        Read(progressBar, NULL, false, &hiromheader);

        hirom= isHirom2((uchar*)loromheader.data(), (uchar*)hiromheader.data());
        //int offset;
        if (hirom)
        {
            //offset = 1;
            selectedheader = &hiromheader;
            //QMessageBox::critical(this, "Derp", "HiRom");
        }
        else
        {
            selectedheader = &loromheader;
            //QMessageBox::critical(this, "Derp", "LoRom");
        }

        for (uchar i=0; i < 21; i++)
        {
            ui->textEdit3->rom.RomTitle[i] = selectedheader->at(0x10+i);
        }
        //RomTitle[21] = 0;
        // ROM Title All set
        ui->textEdit3->rom.CartTypeByte = selectedheader->at(0x16+0x10);

        // Get ROM Size
        ui->textEdit3->rom.RomSizeByte = selectedheader->at(0x17+0x10);


        // Get SRAM Size
        ui->textEdit3->rom.SramSizeByte = selectedheader->at(0x18+0x10);

        if (ui->textEdit3->rom.isValid())
        {
            ui->textEdit3->rom.setString();
            ui->textEdit3->setHtml(ui->textEdit3->rom.finalString);
        }
        ////////////////////////////////////////
        // Update the ROM Info's
        startaddr = 0x607fb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;

        //QByteArray loromheader,hiromheader, *selectedheader;
        Read(progressBar, NULL, false, &loromheader);

        startaddr = 0x60ffb0;
        aal = startaddr&0xff;
        aah = (startaddr&0xff00)>>8;
        aab = (startaddr&0xff0000)>>16;

        numbytes = 0x50;
        Read(progressBar, NULL, false, &hiromheader);

        hirom= isHirom2((uchar*)loromheader.data(), (uchar*)hiromheader.data());
        //int offset;
        if (hirom)
        {
            //offset = 1;
            selectedheader = &hiromheader;
            //QMessageBox::critical(this, "Derp", "HiRom");
        }
        else
        {
            selectedheader = &loromheader;
            //QMessageBox::critical(this, "Derp", "LoRom");
        }

        for (uchar i=0; i < 21; i++)
        {
            ui->textEdit4->rom.RomTitle[i] = selectedheader->at(0x10+i);
        }
        //RomTitle[21] = 0;
        // ROM Title All set
        ui->textEdit4->rom.CartTypeByte = selectedheader->at(0x16+0x10);

        // Get ROM Size
        ui->textEdit4->rom.RomSizeByte = selectedheader->at(0x17+0x10);


        // Get SRAM Size
        ui->textEdit4->rom.SramSizeByte = selectedheader->at(0x18+0x10);

        if (ui->textEdit4->rom.isValid())
        {
            ui->textEdit4->rom.setString();
            ui->textEdit4->setHtml(ui->textEdit4->rom.finalString);
        }

    }

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
    //sleep(1000);
    //delete d;
}


