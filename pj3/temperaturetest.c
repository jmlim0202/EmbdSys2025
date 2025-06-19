#include <stdio.h>
#include "temperature.h"

int main(void)
{
    if (!tempInit()) {
        printf("Failed to initialize temperature sensor.\n");
        return -1;
    }

    double temp = tempRead();
    printf("Current Temperature: %.2f°C\n", temp);

    tempExit();
    return 0;
}
