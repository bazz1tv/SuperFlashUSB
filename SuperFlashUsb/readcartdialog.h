#ifndef READCARTDIALOG_H
#define READCARTDIALOG_H

#include <QDialog>

namespace Ui {
class ReadCartDialog;
}

class ReadCartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReadCartDialog(QWidget *parent = 0);
    ~ReadCartDialog();
    int getStartAddress();
    int getNumBytes();

public slots:
    void accept();

private slots:
    void on_basicButton_clicked();

public:
    Ui::ReadCartDialog *ui;
};



#endif // READCARTDIALOG_H
