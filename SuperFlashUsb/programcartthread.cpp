#include "programcartthread.h"

extern QString bootloader;

void WriteMultiRomEntry(ROM_t *rom, QString &myBootLoader)
{
    int n, uses_DSP;
    uchar flags1, flags2;
    //static int slot = 0;

    // SKIP SDP STUFF FOR NOW DSP
    /*uses_DSP = snes_header.rom_type == 3 || snes_header.rom_type == 5 ||
               snes_header.rom_type == 0xf6;*/

    /*fseek (destfile, 0x4000 + (file_no - 1) * 0x20, SEEK_SET);
    fputc (0xff, destfile);                       // 0x0 = 0xff*/

    //memcpy (name, rominfo->name, 0x1c);
    int i=0x4000 + (rom->num-1) * 0x20;
    int x=0;

    myBootLoader[i++] = 0xff;
    for (x=0; x < 21; x++)
    {
        myBootLoader[i++] = rom->RomTitle[x];
    }
    for (; x <= 0x1c; x++)
    {
        myBootLoader[i++] = ' ';
    }

    //fwrite (name, 1, 0x1c, destfile);             // 0x1 - 0x1c = name

    if (rom->sramsizeinbytes)
      {
        if (rom->sramsizeinbytes == 2 * 1024)
          flags2 = 0x00;
        else if (rom->sramsizeinbytes == 8 * 1024)
          flags2 = 0x10;
        else if (rom->sramsizeinbytes == 32 * 1024)
          flags2 = 0x20;
        else // if (snes_sramsize == 128 * 1024)  // Default to 1024 kbit SRAM
          flags2 = 0x30;
      }
    else
      flags2 = 0x40;

    if (rom->romsizeinbytes > 0x400000)            // Enable Extended Map for >32 Mbit ROMs
      flags2 |= 0x80;

    flags1 = rom->hirom ? 0x10 : 0x00;

    /*if (!rom->hirom && uses_DSP)                  // Set LoROM DSP flag if necessary
      flags1 |= 0x01;*/

    if (rom->startaddr == 0)
      flags1 |= 0x00;
    else if (rom->startaddr == 0x200000)
      flags1 |= 0x40;
    else if (rom->startaddr == 0x400000)
      flags1 |= 0x20;
    else if (rom->startaddr == 0x600000)
      flags1 |= 0x60;

    //slot += (size + 16 * MBIT - 1) & ~(16 * MBIT - 1);

    //fputc (flags1, destfile);                     // 0x1d = mapping flags
    //fputc (flags2, destfile);                     // 0x1e = SRAM flags
    //fputc (size / 0x8000, destfile);              // 0x1f = ROM size (not used by loader)
}

void ProgramCartThread::run()
{
    if (rom1->isAlreadyOnCart && rom2->isAlreadyOnCart && rom3->isAlreadyOnCart && rom4->isAlreadyOnCart)
    {
        emit message(QMessageBox::Information, "No ROMS", "You have not specified any ROMS to upload");
        //QMessageBox::information(this, "No ROMS", "You have not specified any ROMS to upload");
        return;
    }

    // First, we Unlock All Blocks!! "Let's Just Do it! ALL THE TIME <3


    // First, we must see what Games are not alreadyonCart
    // We'll do this 4 times, for each ROM
    if (rom1->isAlreadyOnCart == false)
    {
        // Then Erase this Section of ROM
        // Check size of ROM, erase that many Blocks
        int romsizein128KBlocks; // 128 KB = 0x02:0000 bytes

        romsizein128KBlocks = (int)( (rom1->romsizeinbytes-1) / 0x20000);
        //if ( (rom1->romsizeinbytes % 0x20000 > 0) && romsizein128KBlocks > 0)
          //  romsizein128KBlocks += 1;    //

        // Now that we know the number of blocks, we need to add the base Block number

        int baseblock = (int)(rom1->startaddr / 0x20000);
        //int derp = romsizein128KBlocks+baseblock;

        // When doing erase, let's show a undefined value, loading progress bar:

        /* if minimum and maximum both are set to 0, the bar shows a busy indicator instead of a percentage of steps.
         * */
        //ui->progressBar->setMinimum(0);
        //ui->progressBar->setMaximum(0);
        emit setProgress(0,0,0);
        //int numblocktoerase = (romsizein128KBlocks == 0) ? 0:romsizein128KBlocks-1;

#ifdef DEBUG
        fprintf (stderr,"ROM1\n---------");
        fprintf (stderr,"Erase blocks %d -> %d\n", baseblock, romsizein128KBlocks+baseblock);
        //fprintf (stderr,"romsizein128K = %d\n", romsizein128KBlocks);
        //fprintf (stderr,"numblocktoerase = %d\n", numblocktoerase);
#endif
        //

        // Let's start the RULES

        //eraseThread->specialStart(baseblock, romsizein128KBlocks+baseblock);

        //ui->progressBar->setMaximum(numbytes);
        //ui->progressBar->setMinimum(0);
        emit setProgress(0,numbytes,0);
        //ui->progressBar->resetFormat();

        startaddr = rom1->startaddr;
        numbytes = rom1->romsizeinbytes;
        rom_or_sram = ROM;

        // SRAM Thread is done on purpose (can be used for ROM too)
        //writeSramThread->specialStart(rom1->file);
    }

    if (rom2->isAlreadyOnCart == false)
    {
        // Then Erase this Section of ROM
        // Check size of ROM, erase that many Blocks
        int romsizein128KBlocks; // 128 KB = 0x02:0000 bytes

        romsizein128KBlocks = (int)( (rom2->romsizeinbytes-1) / 0x20000);
        //if ( (rom1->romsizeinbytes % 0x20000 > 0) && romsizein128KBlocks > 0)
          //  romsizein128KBlocks += 1;    //

        // Now that we know the number of blocks, we need to add the base Block number

        int baseblock = (int)(rom2->startaddr / 0x20000);



        // When doing erase, let's show a undefined value, loading progress bar:

        /* if minimum and maximum both are set to 0, the bar shows a busy indicator instead of a percentage of steps.
         * */
        //ui->progressBar->setMinimum(0);
        //ui->progressBar->setMaximum(0);
        emit setProgress(0,0,0);
        //int numblocktoerase = (romsizein128KBlocks == 0) ? 0:romsizein128KBlocks-1;

#ifdef DEBUG
        fprintf (stderr,"ROM2\n---------");
        //fprintf (stderr,"baseblock = %d\n", baseblock);
        //fprintf (stderr,"romsizein128K = %d\n", romsizein128KBlocks);
        fprintf (stderr,"Erase blocks %d -> %d\n", baseblock, romsizein128KBlocks+baseblock);
        //fprintf (stderr,"numblocktoerase = %d\n", numblocktoerase);
#endif



    }

    if (rom3->isAlreadyOnCart == false)
    {
        // Then Erase this Section of ROM
        // Check size of ROM, erase that many Blocks
        int romsizein128KBlocks; // 128 KB = 0x02:0000 bytes

        romsizein128KBlocks = (int)( (rom3->romsizeinbytes-1) / 0x20000);
        //if ( (rom1->romsizeinbytes % 0x20000 > 0) && romsizein128KBlocks > 0)
          //  romsizein128KBlocks += 1;    //

        // Now that we know the number of blocks, we need to add the base Block number

        int baseblock = (int)(rom3->startaddr / 0x20000);



        // When doing erase, let's show a undefined value, loading progress bar:

        /* if minimum and maximum both are set to 0, the bar shows a busy indicator instead of a percentage of steps.
         * */
        //ui->progressBar->setMinimum(0);
        //ui->progressBar->setMaximum(0);
        emit setProgress(0,0,0);
        //int numblocktoerase = (romsizein128KBlocks == 0) ? 0:romsizein128KBlocks-1;

#ifdef DEBUG
        fprintf (stderr,"ROM3\n---------");
       // fprintf (stderr,"baseblock = %d\n", baseblock);
        //fprintf (stderr,"romsizein128K = %d\n", romsizein128KBlocks);
        fprintf (stderr,"Erase blocks %d -> %d\n", baseblock, romsizein128KBlocks+baseblock);
        //fprintf (stderr,"numblocktoerase = %d\n", numblocktoerase);
#endif



    }

    if (rom4->isAlreadyOnCart == false)
    {
        // Then Erase this Section of ROM
        // Check size of ROM, erase that many Blocks
        int romsizein128KBlocks; // 128 KB = 0x02:0000 bytes

        romsizein128KBlocks = (int)( (rom4->romsizeinbytes-1) / 0x20000);
        //if ( (rom1->romsizeinbytes % 0x20000 > 0) && romsizein128KBlocks > 0)
          //  romsizein128KBlocks += 1;    //

        // Now that we know the number of blocks, we need to add the base Block number

        int baseblock = (int)(rom4->startaddr / 0x20000);



        // When doing erase, let's show a undefined value, loading progress bar:

        /* if minimum and maximum both are set to 0, the bar shows a busy indicator instead of a percentage of steps.
         * */

        emit setProgress(0,0,0);
        //ui->progressBar->setMinimum(0);
        //ui->progressBar->setMaximum(0);

        //int numblocktoerase = (romsizein128KBlocks == 0) ? 0:romsizein128KBlocks-1;

#ifdef DEBUG
        fprintf (stderr,"ROM4\n---------");
        fprintf (stderr,"Erase blocks %d -> %d\n", baseblock, romsizein128KBlocks+baseblock);
        //fprintf (stderr,"baseblock = %d\n", baseblock);
        //fprintf (stderr,"romsizein128K = %d\n", romsizein128KBlocks);
        //fprintf (stderr,"numblocktoerase = %d\n", numblocktoerase);
#endif



    }

    // Now it is time to write a modified version of the BootLoader Thingy
    QString myBootLoader(bootloader);
    //myBootLoader.resize(0x8000);
    startaddr = 0x7f8000;
    numbytes = 0x8000;

    /*for (int i=0; i < 0x8000; i++)
    {
        //if (i == 0x)
        myBootLoader[i] = bootloader[i];
    }*/
}

void ProgramCartThread::specialStart(ROM_t *rom1, ROM_t *rom2, ROM_t *rom3, ROM_t *rom4)
{

    this->rom1 = rom1;
    this->rom2 = rom2;
    this->rom3 = rom3;
    this->rom4 = rom4;

    start(QThread::TimeCriticalPriority);

// do this even if Game 4
    // after it's all said and done
    // fill the next game table entry
     // fseek (destfile, 0x4000 + (file_no - 1) * 0x20, SEEK_SET);
     // fputc (0, destfile);                          // indicate no next game
      //fclose (destfile);
}
