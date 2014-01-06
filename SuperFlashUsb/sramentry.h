#ifndef SRAMENTRY_H
#define SRAMENTRY_H

#include "sram_t.h"
#include "utility.h"
#include <QtWidgets>
#include <QTextEdit>

class SramEntry : public QTextEdit
{
    Q_OBJECT
public:
    explicit SramEntry(QWidget *parent = 0);

    void updateText();
    void setString();

signals:

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);


public:
    SRAM_t sram;
    QString finalString;

};

#endif // SRAMENTRY_H
