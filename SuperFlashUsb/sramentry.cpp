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
    if (event->mimeData()->hasFormat("text/plain"))
    {
        setStyleSheet("background-color: #ccc");
        event->acceptProposedAction();
    }


    activateWindow();
}

void SramEntry::dropEvent(QDropEvent * event)
{
    QUrl derp(event->mimeData()->text().remove("\x0d\x0a"));

    if (derp.isLocalFile())
        sram.filename = derp.toLocalFile();

    if (sram.setup() < 0)
    {
        sram.finalString = "&lt;EMPTY&gt;";
    }
    setHtml(sram.finalString);

    setStyleSheet("background-color: #fff");
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

    QMenu myMenu;
    myMenu.addAction("Load Save");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        sram.filename = QFileDialog::getOpenFileName(this, QObject::tr("Open File"), QString(),
                                                     QObject::tr("SRAM Files (*.sav *.srm);; Any (*.*)"));
        sram.setup();
        setHtml(sram.finalString);
    }
    else
    {
        // nothing was chosen
    }
}
