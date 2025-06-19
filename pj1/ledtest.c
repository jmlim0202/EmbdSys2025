#include <stdio.h>
#include <stdlib.h>
#include "led.h"

int main(int argc , char **argv) {
    if (argc < 2) {
        printf("Usage: ledtest <hex>\n");
        return 1;
    }

    ledLibInit();
    unsigned int data = strtol(argv[1], NULL, 16);
    for (int i = 0; i < 8; i++) {
        ledOnOff(i, (data >> i) & 0x01);
    }
    ledLibExit();
    return 0;
}
