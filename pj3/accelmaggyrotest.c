#include <stdio.h>
#include "accel.h"
#include <unistd.h>

int main(void)
{
    if (!accelInit()) {
        printf("Accelerometer init failed\n");
        return -1;
    }

    sleep(1);
    printf("Accel Data: X=%d, Y=%d, Z=%d\n", g_accel_data[0], g_accel_data[1], g_accel_data[2]);

    accelExit();
    return 0;
}
