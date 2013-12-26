#include "../main.h"
#include "util.h"
#include "../graphics.h"


void WriteDataFromFile();
void InitWrite();
void Write(QProgressBar *pb, QFile *f);
void LoadWriteBuffer();
int VerifyWrite();

#define WRITE_PACKET_SIZE 3
#define DERP_SIZE 8
