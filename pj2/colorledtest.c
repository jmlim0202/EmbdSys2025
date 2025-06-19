#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "colorled.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [R] [G] [B] (0~100 percent)\n", argv[0]);
        return 1;
    }

    int r = atoi(argv[1]);
    int g = atoi(argv[2]);
    int b = atoi(argv[3]);

    if (r < 0 || r > 100 || g < 0 || g > 100 || b < 0 || b > 100) {
        printf("Invalid range. Each value must be between 0 and 100.\n");
        return 1;
    }

    colorLedInit();              
    colorLedSet(r, g, b);      
    sleep(2);                     
    colorLedExit();         

    return 0;
}
