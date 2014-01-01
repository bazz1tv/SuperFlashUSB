#include "generalpurposethread.h"

void GeneralPurposeThread::run ()
{
  end = false;

  // DO STUFF

  emit setEnabledButtons(true);
}

void
GeneralPurposeThread::canceled (void)
{
  end = true;
}
