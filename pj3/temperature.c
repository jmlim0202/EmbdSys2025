#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define TEMP_DEV "/dev/spidev1.0"
static int temp_fd = -1;
static char gbuf[10];

int tempInit(void)
{
    __u8 mode;
    __u8 lsb;
    __u8 bits;
    __u32 speed = 20000;

    temp_fd = open(TEMP_DEV, O_RDWR);
    if (temp_fd < 0) {
        perror("Failed to open SPI device");
        return 0;
    }

    if (ioctl(temp_fd, SPI_IOC_RD_MODE, &mode) < 0 ||
        ioctl(temp_fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0 ||
        ioctl(temp_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        perror("SPI ioctl error");
        return 0;
    }

    return 1;
}

void tempExit(void)
{
    if (temp_fd >= 0) close(temp_fd);
    temp_fd = -1;
}

double tempRead(void)
{
    if (temp_fd < 0) return -999.0;

    memset(gbuf, 0, sizeof(gbuf));
    int len = read(temp_fd, gbuf, 2);
    if (len != 2) {
        perror("SPI read error");
        return -999.0;
    }

    int value = (gbuf[1] >> 3) & 0x1F;
    value |= (gbuf[0] << 5);
    double temp = (double)value * 0.0625;

    return temp;
}
