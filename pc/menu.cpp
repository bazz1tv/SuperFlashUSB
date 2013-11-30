#include "menu.h"
#include "../common.h"
#include "util.h"

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


	scanf("%d",&input);
    
    if (input == 8)
        SetLED();
    
    // we subtract here because the definitions start from 0, not 1 (like the input numbers)
    input--;
    MajorCommand = input;
    
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
                printf ("Enter Block Address: ");
                scanf("%lx", &block_address);
                // Error Checking 
            }
            else if (input == 2)
            {
                MinorCommand = ERASE_BLOCK_USING_BLOCKNUM;
                printf ("Enter BlockNumber: ");
                scanf("%d", &blocknum);
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
    if (MajorCommand == WRITE || MajorCommand == READ)
    {
        GetStartAddress();
		GetNumBytes();
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
        }
        
        case WRITE:
        {
            Write();
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