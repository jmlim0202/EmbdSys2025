#include <stdio.h>
#include <stdlib.h>
#include "buzzer.h"

int main(int argc , char **argv) {
    if (argc < 2) {
        printf("Usage: buzzertest <0~8>\n");
        return 1;
    }

    int scale = atoi(argv[1]);
    buzzerInit();

    if (scale == 0)
        buzzerStopSong();
    else
        buzzerPlaySong(scale);

    buzzerExit();
    return 0;
}
