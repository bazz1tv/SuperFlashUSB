#include "readcartdialog.h"
#include "ui_readcartdialog.h"

#include <QTimer>

ReadCartDialog::ReadCartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadCartDialog)
{
    ui->setupUi(this);

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
