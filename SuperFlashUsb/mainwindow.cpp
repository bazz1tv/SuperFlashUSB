#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    statusBar = QMainWindow::statusBar();
    //ui->statusBar;
    statusBar->showMessage("Used Space: 0Mb | Available Space: 64Mb");

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

    //ui->textEdit1->setSty
    //
    //ui->textEdit1->setAcceptDrops(true);
    /*ui->textEdit1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit1, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu1(const QPoint&)));


    ui->textEdit2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit2, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu2(const QPoint&)));

    ui->textEdit3->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit3, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu3(const QPoint&)));

    ui->textEdit4->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit4, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu4(const QPoint&)));*/

    //setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::derp(QDragEnterEvent *p)
{
    p->acceptProposedAction();
}
void MainWindow::derp2(QDropEvent *event)
{
    QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("HERP DERP"));
    event->acceptProposedAction();
}


void MainWindow::ShowContextMenu1(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = ui->textEdit1->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    //int offset=0;
    myMenu.addAction("Load Game1");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        rom1.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

        dothedo(rom1);
        ui->textEdit1->setHtml(rom1.finalString);
    }
    else
    {
        // nothing was chosen
    }
}

void MainWindow::ShowContextMenu2(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = ui->textEdit2->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPMenu Item 1os = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Load Game2");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        rom2.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

        dothedo(rom2,2);
        ui->textEdit2->setHtml(rom2.finalString);

    }
    else
    {
        // nothing was chosen
    }
}

void MainWindow::ShowContextMenu3(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = ui->textEdit3->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Load Game3");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        rom3.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

        dothedo(rom3, 3);
        ui->textEdit3->setHtml(rom3.finalString);
    }
    else
    {
        // nothing was chosen
    }
}

void MainWindow::ShowContextMenu4(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = ui->textEdit4->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Load Game4");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
        rom4.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                             QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

        dothedo(rom4, 4);
        ui->textEdit4->setHtml(rom4.finalString);
    }
    else
    {
        // nothing was chosen
    }
}*/

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

//#include <time.h>
#include "readcartdialog.h"
void MainWindow::on_pushButton_Cart_Read_clicked()
{
    int accepted=0;
    // Create a Popup that selects the Start Address, how many bytes to Read From the Cartridge
    //QDialog dialog(this);
    ReadCartDialog d;// = new ReadCartDialog;
    accepted = d.exec();

    if (accepted == QDialog::Accepted)
    {
        // Query a Filename to Save to
        QString filename = QFileDialog::getSaveFileName(this, QObject::tr("Save File"), QString("derp.bin"),
                                                           QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));


        //d.ui->addrLineEdit->text();
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


