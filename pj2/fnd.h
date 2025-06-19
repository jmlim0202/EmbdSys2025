#ifndef __FND_H__
#define __FND_H__

#define MAX_FND_NUM 6
#define FND_DATA_BUFF_LEN (MAX_FND_NUM + 2)

int fndDisp(int num, int dotflag);
int fndDispTime(void);
int fndCountUp(int max);
int fndOff(void);

#endif 
