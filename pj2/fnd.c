#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "fnd.h"
#define FND_DRIVER_NAME "/dev/perifnd"


typedef struct FNDWriteDataForm_tag {
    char DataNumeric[FND_DATA_BUFF_LEN]; 
    char DataDot[FND_DATA_BUFF_LEN];   
    char DataValid[FND_DATA_BUFF_LEN];   
} stFndWriteForm;

int fndDisp(int num, int dotflag) {
    int fd;
    int temp, i;
    stFndWriteForm stWriteData;

    for (i = 0; i < MAX_FND_NUM; i++) {
        stWriteData.DataDot[i] = (dotflag & (0x1 << i)) ? 1 : 0;
        stWriteData.DataValid[i] = 1;
    }

    
    temp = num % 1000000; stWriteData.DataNumeric[0] = temp / 100000;
    temp = num % 100000;  stWriteData.DataNumeric[1] = temp / 10000;
    temp = num % 10000;   stWriteData.DataNumeric[2] = temp / 1000;
    temp = num % 1000;    stWriteData.DataNumeric[3] = temp / 100;
    temp = num % 100;     stWriteData.DataNumeric[4] = temp / 10;
    stWriteData.DataNumeric[5] = num % 10;

    fd = open(FND_DRIVER_NAME, O_RDWR);
    if (fd < 0) {
        perror("driver open error.\n");
        return 0;
    }

    write(fd, &stWriteData, sizeof(stFndWriteForm));
    close(fd);
    return 1;
}


int fndDispTime(void) {
    time_t t;
    struct tm *ptm;
    int timeNum;

    if (time(&t) == -1)
        return -1;

    ptm = localtime(&t);
    timeNum = ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec;

    return fndDisp(timeNum, 0b1010); 
}


int fndCountUp(int max) {
    for (int i = 0; i <= max; i++) {
        if (!fndDisp(i, 0))
            return -1;
        sleep(1);
    }
    return 0;
}

int fndOff(void) {
    int fd;
    stFndWriteForm stWriteData = {0};

    for (int i = 0; i < MAX_FND_NUM; i++) {
        stWriteData.DataValid[i] = 0;
        stWriteData.DataDot[i] = 0;
        stWriteData.DataNumeric[i] = 0;
    }

    fd = open(FND_DRIVER_NAME, O_RDWR);
    if (fd < 0) {
        perror("driver open error.\n");
        return 0;
    }

    write(fd, &stWriteData, sizeof(stFndWriteForm));
    close(fd);
    return 1;
}
