#include "datatypes/romentry.h"


RomEntry::RomEntry(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptDrops(true);
    setStyleSheet("background-color: #fff;");
    setHtml(" ");
}

void RomEntry::setNum(int n)
{
    num = n;
    game.setNum(n);
}

void RomEntry::updateText()
{
    setHtml(finalString);
}

void RomEntry::setString()
{
    finalString = game.rom.setString();
}

int RomEntry::QueryUSBRomHeader()
{
    int r;
    /*if ( (r=rom.QueryUSBRomHeader()) < 0)
        return r;
    setHtml(rom.finalString);*/

    return 0;
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
    QUrl fileurl(event->mimeData()->text().remove("\x0d\x0a"));



    if (fileurl.isLocalFile())
    {


        if (game.load(fileurl.toLocalFile()) < 0)
        {
            // THIS SHOULD GET INTEGRATED AND HAPPEN IN LOWER LEVEL
            finalString = "<b>"+QString("%1").arg(game.rom.num)+") </b>&lt;EMPTY&gt;";
        }
        else setString();
        setHtml(finalString);

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
    QString text;
    QMenu myMenu;

    if (game.rom.isAlreadyOnCart)
    {
        text = "Overwrite Game";

    }
    else
    {
        text = "Load Game";
    }
    myMenu.addAction(text);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        QString filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);; Any (*.*)"));
        if (filename == "")
        {
            return;
        }

        //rom.DoTheDo();
        if (game.load(filename) < 0)
        {
            finalString = "<b>"+QString("%1").arg(num)+") </b>&lt;EMPTY&gt;";
        }
        else setString();

        setHtml(finalString);
    }
    else
    {
        // nothing was chosen
    }
}
