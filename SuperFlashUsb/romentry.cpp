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

        if (rom.DoTheDo() < 0)
        {
            rom.finalString = "<b>"+QString("%1").arg(rom.num)+") </b>&lt;EMPTY&gt;";
        }
        else rom.setString();
        setHtml(rom.finalString);

        setStyleSheet("background-color: #fff");
        event->acceptProposedAction();  // This because otherwise the window holding the files that we dropped maintains focus.

    }
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
    QPoint globalPos = event->globalPos();

    QMenu myMenu;
    myMenu.addAction("Load Game");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        rom.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);; Any (*.*)"));
        rom.DoTheDo();
        rom.setString();
        setHtml(rom.finalString);
    }
    else
    {
        // nothing was chosen
    }
}
