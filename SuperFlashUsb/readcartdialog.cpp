#include "readcartdialog.h"
#include "ui_readcartdialog.h"

#include <QTimer>
#include <QMessageBox>

ReadCartDialog::ReadCartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadCartDialog)
{
    ui->setupUi(this);

    // this gives the Address lineEdit the focus
    QTimer::singleShot(0, ui->addrLineEdit, SLOT(setFocus()));
}

ReadCartDialog::~ReadCartDialog()
{
    delete ui;
}

void ReadCartDialog::accept()
{
    //
    QDialog::accept();
}

int ReadCartDialog::getStartAddress()
{
    bool ok;
    int hex = ui->addrLineEdit->text().toInt(&ok, 16);

    if (!ok)
    {
        QMessageBox::critical(this, "StartAddress Error","Could not Convert StartAddress from String to Base16");
        return -1;
    }

    return hex;

}

int ReadCartDialog::getNumBytes()
{
    bool ok;
    int hex = ui->numBytesLineEdit->text().toInt(&ok, 16);

    if (!ok)
    {
        QMessageBox::critical(this, "Numbytes Error","Could not Convert StartAddress from String to Base16");
        return -1;
    }

    return hex;
}

void ReadCartDialog::on_basicButton_clicked()
{
    //  Take me to Basic Mode
}
