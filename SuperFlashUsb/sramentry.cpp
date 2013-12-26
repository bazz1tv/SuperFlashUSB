#include "sramentry.h"

SramEntry::SramEntry(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptDrops(true);
    setStyleSheet("background-color: #fff;");
    setHtml(" ");
}

void SramEntry::dragEnterEvent(QDragEnterEvent * event)
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
    activateWindow();
}

void SramEntry::dropEvent(QDropEvent * event)
{

    //QMessageBox::critical(this, QObject::tr("Error"), event->mimeData()->text());
        //if (event->mimeData()->hasFormat("text/plain"))


    //sram.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                // QObject::tr("sram Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));

    // File won't open with "file://" or "0x0d0a" at End of filename
    QUrl derp(event->mimeData()->text().remove("\x0d\x0a"));

    if (derp.isLocalFile())
        sram.filename = derp.toLocalFile();

    /*QFile derp("/home/bazz/derp.txt");
    derp.open(QIODevice::WriteOnly);
    derp.write(sram.filename.toUtf8().constData());*/

    //sram.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                 //QObject::tr("sram Files (*.smc *.sfc *.fig *.bin);;SRAM Files (*.sav *.srm);; Any (*.*)"));
    //QMessageBox::critical(this, QObject::tr("Error"), sram.filename);
    //QMessageBox::critical(this, QObject::tr("Error"), sram.filename);


    if (sram.setup() < 0)
    {
        sram.finalString = "&lt;EMPTY&gt;";
    }
    setHtml(sram.finalString);

    setStyleSheet("background-color: #fff");
    //event->acceptProposedAction();
    event->acceptProposedAction();

    // This because otherwise the window holding the files that we dropped maintains focus.
    this->activateWindow();
}

void SramEntry::dragLeaveEvent(QDragLeaveEvent * event)
{
    setStyleSheet("background-color: #fff");
    event->accept();
}

void SramEntry::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        //setStyleSheet("background-color: #ccc");
        event->acceptProposedAction();
    }
    activateWindow();
}

void SramEntry::contextMenuEvent(QContextMenuEvent * event)
{
    // for most widgets
    QPoint globalPos = event->globalPos();
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    //int offset=0;
    myMenu.addAction("Load Save");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        sram.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("SRAM Files (*.sav *.srm);; Any (*.*)"));
        //QMessageBox::critical(this, QObject::tr("Error"), sram.filename);
        sram.setup();
        setHtml(sram.finalString);
    }
    else
    {
        // nothing was chosen
    }
}
