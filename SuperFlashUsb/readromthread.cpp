#include "readromthread.h"
//#include "usbthread.h"
#include <libusb.h>
#include "pc/sflash.h"

void ReadRomThread::specialStart(QProgressBar *pb, QString filename)
{
    progressBar = pb;



    //file = new QFile(filename);
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadWrite))
    {
        QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Could not open file"));
        return;
    }


    start();
}

//#include "mainwindow.h"
//extern MainWindow *w;

void ReadRomThread::run ()
{
  end = false;

  Read(progressBar, &file, true, NULL);

  file.close();

  QMessageBox::information(NULL, "ROM Read Complete", "Transfer Complete!");
}

void
ReadRomThread::canceled (void)
{
  end = true;
}
