#include <stdio.h>
#include <stdlib.h>
#include "fnd.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [option] [number]\n", argv[0]);
        printf("Options: s (static), t (time), c (count), o (off)\n");
        return 1;
    }

    char mode = argv[1][0];
    int number;

    switch (mode) {
        case 's':
            if (argc != 3) {
                printf("Usage: %s s [number]\n", argv[0]);
                return 1;
            }
            number = atoi(argv[2]);
            fndDisp(number, 0); 
            break;

        case 't':
            fndDispTime(); 
            break;

        case 'c':
            if (argc != 3) {
                printf("Usage: %s c [max]\n", argv[0]);
                return 1;
            }
            number = atoi(argv[2]);
            fndCountUp(number);  
            break;

        case 'o':
            fndOff(); 
            break;

        default:
            printf("Invalid mode.\n");
            return 1;
    }

    return 0;
}
