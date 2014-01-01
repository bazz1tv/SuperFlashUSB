#include "generalpurposethread.h"
#include "pc/sflash.h"

void GeneralPurposeThread::message_complete()
{
    msg_complete = true;
}

void GeneralPurposeThread::sendmessage(int msgtype, QString title, QString msg)
{
    msg_complete = false;
     emit message(msgtype, title, msg);

    while (!msg_complete);
        return;
}

void GeneralPurposeThread::specialStart(int operation)
{
    this->operation = operation;

    start(QThread::TimeCriticalPriority);
}

void GeneralPurposeThread::run ()
{
    end = false;

    if (operation == CONNECT)
    {
        bool success = true;
        uchar *manuf_id_codes = ::GetChipID();
        bool id_good = true;


        // MUST LOAD ONE AT A TIME, SINCE THE POINTERS USE TEH SAME BUFFER


        fprintf(stderr,"Chip ID: ");
        fprintf(stderr, "0x%x : ",manuf_id_codes[0]);
        if (manuf_id_codes[0] == 0x89)
        {
            fprintf(stderr,"Correct\n");
            // sendmessage(QMessageBox::Warning, "UH OH!", QString("The ROM Chip ID: 0x%1 does not match the expected Chip ID: 0x%2").arg(manuf_id_codes[0],2,16).arg(0x89,2,16));
        }
        else
        {
            fprintf(stderr,"Incorrect\n");
            success=false;
        }
        fprintf(stderr, "Device Code: 0x%x : ",manuf_id_codes[1]);
        if (manuf_id_codes[1] == 0x17)
        {
            fprintf(stderr,"Correct\n");
            // sendmessage(QMessageBox::Warning, "UH OH!", QString("The ROM Device-Code: 0x%1 does not match the expected code: 0x%2").arg(manuf_id_codes[1],2,16).arg(0x17,2,16));
        }
        else
        {
            fprintf(stderr,"Incorrect\n");
            success=false;
        }

        // ROM CHIP ID CHECK WENT ASTRAY
        if (!success && manuf_id_codes[0] == 0xff && manuf_id_codes[1] == 0xFF)
        {
            sendmessage(QMessageBox::Warning, "UH OH!", "You don't seem have to connected the Cartridge to the Programmer\n\n Please unplug the programmer (power it down), plug the cartridge, and restart the application");
            qApp->quit();
            return;
        }
        else if (!success)
        {
            QString derp("ROM CHIP ID CHECK WENT ASTRAY\n");

            derp += QString("Your ROM ChipID: 0x%1 VS. expected: 0x%2\n\n").arg(manuf_id_codes[0],2,16).arg(0x89,2,16);
            derp += QString("Your ROM Device-ID: 0x%1 VS. expected: 0x%2\n\n").arg(manuf_id_codes[1],2,16).arg(0x17,2,16);
            derp += "If you are indeed using a Tototek SuperFlash Cartridge, please identify to me the Flash ROM model number\nEmail to mbazzinotti@gmail.com";
            sendmessage(QMessageBox::Warning, "UH OH!", derp);
            qApp->quit();
        }

         //sendmessage(QMessageBox::Warning, "UH OH!", );
        ////////////////////////
        /////////////////////////

        bool second_check = false;
        bool any_blocks_locked;
        uchar *lockbits;
        redo:
        any_blocks_locked=false;

        lockbits = GetLockBits();



        for (int c=0; c < 64; c++)
        {
            if ((lockbits[c]&1) == 1)
            {
                any_blocks_locked = true;
            }
            fprintf (stderr,"Block ");
            if (c < 10)
                fprintf(stderr, " ");
            fprintf (stderr, "%d: %s\n",c, ( (lockbits[c] & 1) == 1) ? "Locked":"Unlocked");
        }
        if (any_blocks_locked && !second_check)
        {
            //sendmessage(QMessageBox::Warning, "OH NOES!", "There are Some Locked Blocks on Your ROM!!! Let's try to Unlock them!!");
            ::UnlockAllBlocks();
            second_check = true;
            goto redo;
        }
        else if (any_blocks_locked && second_check)
        {
            success=false;
            sendmessage(QMessageBox::Critical, "OG NOES!!", "I could not unlock the locked-blocks on your Flash ROM!! Not Good! Fatal!! Email me!!\n\nmbazzinotti@gmail.com\n\nBetter yet, don't email me, because I don't know how to fix this problem!");
        }
        else if (second_check && !any_blocks_locked)
        {
            fprintf(stderr, "YAY - I unlocked all yer BLOCKS!!\n");
             //sendmessage(QMessageBox::Information, "YAY!", "The blocks are all unlocked!!");
        }


        if (!success)
        {
             sendmessage(QMessageBox::Critical, "OG NO!", "You have encountered one or more critical errors. Is the Cartridge plugged in??\n\n");
        }
    }
}

void GeneralPurposeThread::canceled (void)
{
  end = true;
}

// LET'S ESTABLISH AN ORDER OF OPERATIONS FOR WHEN THE PROGRAMMER IS PLUGGED IN AT STARTUP or WHATNOT

/* First, let's get the Chip ID Babayyyy. if it doesn't match. ASK the user if he is not using a 64MB model


  Why don't we query the 32 MB chip ID as well. and suggest if that is the case, that 32MB support is not yet implemented


  */
