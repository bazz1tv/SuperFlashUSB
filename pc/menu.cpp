#include "menu.h"
#include "../common.h"
#include "sflash/util.h"
#include <stdlib.h>

int OpenForWriteBin(const char *filename)
{
    fh = fopen(filename, "wb");
	if(fh == NULL) {
		printf("Cant open source file\n");
		return 1;
	}
    
    return 0;
}

int OpenForReadBin(const char *filename)
{
    fh = fopen(filename, "rb");
	if(fh == NULL) {
		printf("Cant open source file\n");
		return 1;
	}
    
    return 0;
}

int OpenFiles()
{
    
	if ( ::argc > 1)
	{
    	if (MajorCommand == READ || MajorCommand == READ_SRAM )
    	{
    		printf ("Opening File %s for writing to\n", filename);
    		fh = fopen(filename, "wb");
    		if(fh == NULL) {
    			printf("Cant open source file\n");
    			return 1;
    		}
    		printf ("File Opened\n");
    	}
    	else if ( MajorCommand == WRITE || MajorCommand == WRITE_SRAM )
    	{
    		printf ("Opening File %s for reading from\n", filename);
    		fh = fopen(filename, "rb");
    		if(fh == NULL) {
    			printf("Cant open source file\n");
    			return 1;
    		}
    		printf ("File Opened\n");
    	}
	}
    else
    {
        // Input a filename
        
        
        if (MajorCommand == WRITE)
        {
            printf ("File to ");
            printf ("write from: ");
            scanf ("%s",filename);
            OpenForReadBin(filename);
        }
        else if (MajorCommand == READ)
        {
            printf ("File to ");
            printf ("read to: ");
            scanf ("%s",filename);
            
            OpenForWriteBin(filename);
        }
    }
    
	
		
	return 0;
}

int GetSelection()
{
    printf ("Select: \n");
	printf ("1) Get Flash Chip ID\n");
	printf ("2) Erase\n");
	//printf ("3) Erase Sector\n");
	printf ("4) Read\n");
	printf ("5) Write\n\n");
	printf ("6) Read SRAM\n");
	printf ("7) Write SRAM\n");
    printf ("8) SetLED\n");
    printf ("9) Unlock All Blocks\n");
    printf ("10) Lock Block\n");


	scanf("%d",&input);
      MajorCommand = input-1;
    
    if (input == 8)
    {
        SetLED();
        return 1;
    }
    else if (input == 9)
    {
        UnlockAllBlocks();
    }
    else if (input == 5)
    {
        rom_or_sram = ROM;
    }
    else if (input == 4)
    {
        rom_or_sram = ROM;
    }
    else if (input == 6)
    {
        MajorCommand = READ;
        rom_or_sram = SRAM;
    }
    else if (input == 7)
    {
        MajorCommand = WRITE;
        rom_or_sram = SRAM;
    }
    
    
    // we subtract here because the definitions start from 0, not 1 (like the input numbers)
    input--;
    
    if (input == ERASE)
    {
        MajorCommand = ERASE;
        
        printf ("1) Erase ROM Chip\n");
        printf ("2) Erase Block\n");
        
        scanf("%d",&input);
        
        if (input == 1)
        {
            // ERASE ROM CHIP PROCESSING
            blocknum_start = 0;
            blocknum_end = 63;
        }
        else if (input == 2)
        {
            printf ("1) Use Block Address\n");
            printf ("2) Use Block Number\n");
            
            scanf("%d",&input);
            
            if (input == 1)
            {
                MinorCommand = ERASE_BLOCK_USING_BLOCKNUM;
                printf ("Block Address range (x-y): ");
                scanf("%lx-%lx", &block_address_start, &block_address_end);
                
                blocknum_start = block_address_start / 0x20000;
                blocknum_end = block_address_end / 0x20000;
                // Error Checking 
            }
            else if (input == 2)
            {
                MinorCommand = ERASE_BLOCK_USING_BLOCKNUM;
                printf ("Enter BlockNumbers (x-y): ");
                scanf("%d-%d", &blocknum_start,&blocknum_end);
            }
            else
            {
                printf ("FAIL");
                exit(0);
            }
        }
    }
    return 0;
}

void GetStartAddress()
{
    printf ("Enter Start Address: 0x");
	scanf("%lx", &startaddr);
    aal = startaddr&0xff;
	aah = (startaddr&0xff00)>>8;
	aab = (startaddr&0xff0000)>>16;
}

void GetNumBytes()
{
	printf ("Enter Number of bytes: 0x");
	scanf ("%lx", &numbytes);

	l = numbytes&0xff;
	h = (numbytes&0xff00)>>8;
	b = (numbytes&0xff0000)>>16;
}

void ProcessSelection()
{
    if (rom_or_sram == ROM)
    {
        if (MajorCommand == WRITE || MajorCommand == READ)
        {
            GetStartAddress();
    		GetNumBytes();
        }
    }
    else if (rom_or_sram == SRAM)
    {
        if (MajorCommand == WRITE || MajorCommand == READ)
        {
            startaddr = 0xfe0000;
            aal = startaddr&0xff;
            aah = (startaddr&0xff00)>>8;
            aab = (startaddr&0xff0000)>>16;
            /*numbytes = 0x20000;
            l = numbytes&0xff;
            h = (numbytes&0xff00)>>8;
            b = (numbytes&0xff0000)>>16;*/
            GetNumBytes();
        }
    }
    
    switch (MajorCommand)
    {
        case READ_IDENTIFIER_CODES:
        {
            ReadIDCodes();  // sflash 
            break;
        }
            
        case READ:
        {
            Read();         // sflash module
            break;
        }
        
        case ERASE:
        {
            Erase();
            break;
        }
        
        case WRITE:
        {
            Write();
            break;
        }
        
        default:
        break;
    }
}
int EnterTextInterface()
{
    while (1)
    {
        beginning:
        if (GetSelection() == 1)
            goto beginning;
        OpenFiles();
        ProcessSelection();
        
        if (fh)
        {
            fclose(fh);
            fh = NULL;
        }
    }
    return 0;
}
