#include "../main.h"
#include "util.h"
#include "../graphics.h"

void ReadDataToFile();
void InitRead();

#include <QtWidgets>

extern QProgressBar *progressBar;
extern QFile *file;

void ReadHeader(QByteArray *data2);
void Read(QProgressBar *pb, QFile *f, bool tofile, QByteArray *data2);

void LoadReadBuffer();
void ReadDataToBuffer(QByteArray *data2);

#define DERP 8
