#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usb.h"
#include "readcartdialog.h"

#include <QtWidgets>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    statusBar = QMainWindow::statusBar();
    //ui->statusBar;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(connect_USB()));
    timer->start(1000);
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
    USB::InitUSB();
}

MainWindow::~MainWindow()
{
    USB::CloseUSBDevice();
    USB::EndUSB();
    delete ui;
    delete timer;
}


// don't worry about it
//
void MainWindow::connect_USB()
{
    //InitUSB();
    if (USB::OpenUSBDevice() < 0)
    {
        statusBar->showMessage("USB Device Not Connected or Not Found");
    }
    else
    {
        statusBar->showMessage("USB Device Connected");
        timer->stop();
    }

}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}



void MainWindow::on_pushButton_Cart_Read_clicked()
{
    int accepted=0;
    // Create a Popup that selects the Start Address, how many bytes to Read From the Cartridge
    //QDialog dialog(this);
    ReadCartDialog d;// = new ReadCartDialog;
    accepted = d.exec();

    if (accepted == QDialog::Accepted)
    {
        ROM rom;
        // Query a Filename to Save to
        rom.filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), QString("derp.bin"),
                                                           QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
        if (rom.open() < 0)
        {
            QMessageBox::critical(this, "File Error", "Could not Open File");
            return;
        }
        int startaddr = d.getStartAddress();
        int numbytes = d.getNumBytes();

        // setup progress bar
        ui->progressBar->setMaximum(numbytes);
        ui->progressBar->setMinimum(0);
        ui->progressBar->resetFormat();

        // Now Call our PC Command line functions to Read the Cart




        // we are finished
    }
    else
    {
        // Nothing
    }
    //sleep(1000);
    //delete d;
}


