#include "romentry.h"


RomEntry::RomEntry(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptDrops(true);
    setStyleSheet("background-color: #fff;");
    setHtml(" ");
}

/*bool RomEntry::canInsertFromMimeData(const QMimeData * source) const
{
    //QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Derp4"));

    return true;
}

void RomEntry::insertFromMimeData(const QMimeData * source)
{
    QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Derp3"));
}*/

void RomEntry::dragEnterEvent(QDragEnterEvent * event)
{
    //QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Derp"));
    if (event->mimeData()->hasFormat("text/plain"))
    {
        setStyleSheet("background-color: #ccc");
        event->acceptProposedAction();
    }

    //event->accept();
    //event->acceptProposedAction();
    //event->setDropAction(Qt::MoveAction);
            //event->accept();
    //event->setDropAction(Qt::CopyAction);
}

void RomEntry::dropEvent(QDropEvent * event)
{

    //QMessageBox::critical(this, QObject::tr("Error"), event->mimeData()->text());
        //if (event->mimeData()->hasFormat("text/plain"))


    //rom.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                // QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

    // File won't open with "file://" or "0x0d0a" at End of filename
    rom.filename = event->mimeData()->text().remove("file://").remove("\x0d\x0a");

    /*QFile derp("/home/bazz/derp.txt");
    derp.open(QIODevice::WriteOnly);
    derp.write(rom.filename.toUtf8().constData());*/

    //rom.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                 //QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
    //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);
    //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);


    if (dothedo(rom) < 0)
    {
        rom.finalString = "<b>"+QString("%1").arg(rom.num)+") </b>&lt;EMPTY&gt;";
    }
    setHtml(rom.finalString);

    setStyleSheet("background-color: #fff");
    //event->acceptProposedAction();
    event->acceptProposedAction();

    // This because otherwise the window holding the files that we dropped maintains focus.
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
                                                     QObject::tr("ROM Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
        //QMessageBox::critical(this, QObject::tr("Error"), rom.filename);
        dothedo(rom);
        setHtml(rom.finalString);
    }
    else
    {
        // nothing was chosen
    }
}
