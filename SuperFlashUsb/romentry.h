#ifndef ROMENTRY_H
#define ROMENTRY_H

#include "utility.h"
#include <QtWidgets>

class RomEntry : public QTextEdit
{
    Q_OBJECT
public:
    explicit RomEntry(QWidget *parent = 0);

signals:

public slots:

protected:
    //bool	canInsertFromMimeData(const QMimeData * source) const;
    //void	insertFromMimeData(const QMimeData * source);
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent * event);
    void contextMenuEvent(QContextMenuEvent * event);


public:
    ROM rom;

};

#endif // ROMENTRY_H
