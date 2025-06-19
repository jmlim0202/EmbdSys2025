#ifndef __COLOR_LED_H__
#define __COLOR_LED_H__
#define PWM_COLOR_R 2
#define PWM_COLOR_G 1
#define PWM_COLOR_B 0

#define PWM_PERIOD_NS 1000000 

int colorLedInit(void);

int colorLedExit(void);

int colorLedSet(int r_percent, int g_percent, int b_percent);

int pwmActiveAll(void);
int pwmInactiveAll(void);
int pwmSetDuty(int dutyCycle, int pwmIndex);
int pwmSetPeriod(int Period, int pwmIndex);
int pwmSetPercent(int percent, int pwmIndex);
int pwmStartAll(void);

#endif
