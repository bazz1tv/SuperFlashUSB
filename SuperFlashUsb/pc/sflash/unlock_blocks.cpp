#include "unlock_blocks.h"
#include "../USB.h"

void UnlockAllBlocks()
{
    r = SendPacket(OUT, UNLOCK_ALL_BLOCKS, 0,0,NULL,0,50);
    
    if (r == 0)
    {
        cout << "Packet Sent\n";
    }
    else cout << "HMMMM\n";
    
    
}