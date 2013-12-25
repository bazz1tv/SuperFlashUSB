#include "romentry.h"


RomEntry::RomEntry(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptDrops(true);
    setStyleSheet("background-color: #fff;");
    setHtml(" ");
}

void RomEntry::QueryUSBRomHeader()
{
    rom.QueryUSBRomHeader();
    setHtml(rom.finalString);
}



void RomEntry::dragEnterEvent(QDragEnterEvent * event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        setStyleSheet("background-color: #ccc");
        event->acceptProposedAction();
    }


    activateWindow();
}

void RomEntry::dropEvent(QDropEvent * event)
{
    // Little hack to remove the newline at the end + carriage return bullshit
    QUrl derp(event->mimeData()->text().remove("\x0d\x0a"));



    if (derp.isLocalFile())
    {
        rom.filename = derp.toLocalFile();

        // This is to get local filename into a file to see the HEX CONTENTS
        /*QFile merp("/home/bazz/derp.txt");
        merp.open(QIODevice::WriteOnly);
        merp.write(rom.filename.toUtf8().constData());
        merp.close();*/


        if (rom.DoTheDo() < 0)
        {
            rom.finalString = "<b>"+QString("%1").arg(rom.num)+") </b>&lt;EMPTY&gt;";
        }
        else rom.setString();
        setHtml(rom.finalString);

        setStyleSheet("background-color: #fff");
        //event->acceptProposedAction();
        event->acceptProposedAction();

        // This because otherwise the window holding the files that we dropped maintains focus.

    }

    //rom.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                 //QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
    //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);
    //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);

    this->activateWindow();

}

void RomEntry::dragLeaveEvent(QDragLeaveEvent * event)
{
    setStyleSheet("background-color: #fff");
    event->accept();
}

void RomEntry::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        //setStyleSheet("background-color: #ccc");
        event->acceptProposedAction();
    }
    activateWindow();

}

void RomEntry::contextMenuEvent(QContextMenuEvent * event)
{
    // for most widgets
    QPoint globalPos = event->globalPos();
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    //int offset=0;
    myMenu.addAction("Load Game");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        rom.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);; Any (*.*)"));
        //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);
        rom.DoTheDo();
        rom.setString();
        setHtml(rom.finalString);
    }
    else
    {
        // nothing was chosen
    }
}
