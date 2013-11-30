#include "unlock_blocks.h"


void UnlockAllBlocks()
{
    LatchStatus(1);
    WriteByteNoAddr(0x60);
    WriteByteNoAddr(0xD0);
    
    while (!(ReadStatusWithoutCommand() & 0x80));
    
    LatchStatus(0);
    
     
}