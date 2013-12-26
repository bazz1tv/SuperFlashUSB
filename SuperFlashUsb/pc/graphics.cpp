#include "graphics.h"
#include <iostream>
#include <stdio.h>

void loadBar(int x, int n, int r, int w)
{
	int i;
    // Only update r times.
    if ( x % (n/r) != 0 ) return;

    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );

    // Show the load bar.
    for (i=0; i<c; i++)
       printf("=");

    for (i=c; i<w; i++)
       printf(" ");

    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("]\r");
	//printf("]\n\033[F\033[J");
}
