#include "menu.h"
#include "../common.h"

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
    
	if ( argc > 1)
	{
    	if (input == READ || input == READ_SRAM )
    	{
    		printf ("Opening File %s for writing to\n", filename);
    		fh = fopen(filename, "wb");
    		if(fh == NULL) {
    			printf("Cant open source file\n");
    			return 1;
    		}
    		printf ("File Opened\n");
    	}
    	else if ( input == FLASH || input == WRITE_SRAM )
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
        printf ("File to ");
        
        if (input == FLASH)
        {
            printf ("write from: ");
            scanf ("%s",filename);
            OpenForReadBin(filename);
        }
        else if (input == READ)
        {
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
	printf ("5) Flash\n\n");
	printf ("6) Read SRAM\n");
	printf ("7) Write SRAM\n");


	scanf("%d",&input);
    
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
        }
        else if (input == 2)
        {
            printf ("1) Use Block Address\n");
            printf ("2) Use Block Number\n");
            
            scanf("%d",&input);
            
            if (input == 1)
            {
                MinorCommand = ERASE_BLOCK_USING_ADDRESS;  
            }
            else if (input == 2)
            {
                MinorCommand = ERASE_BLOCK_USING_BLOCKNUM;
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
    if (input == FLASH || input == READ)
    {
        GetStartAddress();
		GetNumBytes();
    }
    
    switch (input)
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
        
        default:
        break;
    }
}
int EnterTextInterface()
{
    while (1)
    {
        GetSelection();
        OpenFiles();
        ProcessSelection();
        
        if (fh)
            fclose(fh);
    }
    return 0;
}