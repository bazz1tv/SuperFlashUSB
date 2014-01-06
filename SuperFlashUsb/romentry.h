#ifndef ROMENTRY_H
#define ROMENTRY_H

#include "utility.h"
#include "game.h"
#include <QtWidgets>

class RomEntry : public QTextEdit
{
    Q_OBJECT
public:
    explicit RomEntry(QWidget *parent = 0);

    void updateText();
    void setString();
    void setNum(int n);

signals:

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);


public:
    Game game;
    QString finalString;
    int num;
    int QueryUSBRomHeader();

private:

};

#endif // ROMENTRY_H
