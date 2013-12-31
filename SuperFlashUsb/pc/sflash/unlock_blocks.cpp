#include "unlock_blocks.h"
#include "../USB.h"

void UnlockAllBlocks()
{
    redo:
    r = SendPacket(OUT, UNLOCK_ALL_BLOCKS, 0,0,NULL,0,50);
    
    if (r == 0)
    {
        cout << "Unlock Blocks Packet Sent\n";
    }
    else
    {
        cout << "HMMMM\n";
        goto redo;
    }

    
    
}
