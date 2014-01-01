#ifndef ROMENTRY_H
#define ROMENTRY_H

#include "utility.h"
#include "rom_t.h"
#include <QtWidgets>

class RomEntry : public QTextEdit
{
    Q_OBJECT
public:
    explicit RomEntry(QWidget *parent = 0);

signals:

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);


public:
    ROM_t rom;
    int QueryUSBRomHeader();

private:

};

#endif // ROMENTRY_H
