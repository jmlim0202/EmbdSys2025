#include <stdio.h>
#include <stdlib.h>
#include "textlcd.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s [line(1|2)] [string]\n", argv[0]);
        return 1;
    }

    int line = atoi(argv[1]);
    const char *textStr = argv[2];

    if (line == 1)
        text(textStr, ""); 
    else if (line == 2)
        text("", textStr); 
    else {
        printf("Invalid line number (1 or 2).\n");
        return 1;
    }

    return 0;
}
