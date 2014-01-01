#include "generalpurposethread.h"

void GeneralPurposeThread::specialStart(int operation)
{
    if (operation == )
}

void GeneralPurposeThread::run ()
{
  end = false;

  // DO STUFF

  emit setEnabledButtons(true);
}

void GeneralPurposeThread::canceled (void)
{
  end = true;
}

// LET'S ESTABLISH AN ORDER OF OPERATIONS FOR WHEN THE PROGRAMMER IS PLUGGED IN AT STARTUP or WHATNOT

/* First, let's get the Chip ID Babayyyy. if it doesn't match. ASK the user if he is not using a 64MB model


  Why don't we query the 32 MB chip ID as well. and suggest if that is the case, that 32MB support is not yet implemented


  */
