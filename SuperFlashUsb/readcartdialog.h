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

public slots:
    void accept();

private slots:
    //void on_addrSlider_valueChanged(int value);

private:
    Ui::ReadCartDialog *ui;
};

#endif // READCARTDIALOG_H
