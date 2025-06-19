#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "colorled.h"
#include <string.h> 

#define COLOR_LED_DEV_R_ "/sys/class/pwm/pwmchip0/"
#define COLOR_LED_DEV_G_ "/sys/class/pwm/pwmchip1/"
#define COLOR_LED_DEV_B_ "/sys/class/pwm/pwmchip2/"

#define PWM_EXPORT "export"
#define PWM_UNEXPORT "unexport"
#define PWM_DUTY "pwm0/duty_cycle"
#define PWM_PERIOD "pwm0/period"
#define PWM_ENABLE "pwm0/enable"

static int pwmWrite(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) return -1;
    write(fd, value, strlen(value));
    close(fd);
    return 0;
}

int pwmActiveAll(void) {
    pwmWrite(COLOR_LED_DEV_R_ PWM_EXPORT, "0");
    pwmWrite(COLOR_LED_DEV_G_ PWM_EXPORT, "0");
    pwmWrite(COLOR_LED_DEV_B_ PWM_EXPORT, "0");
    return 1;
}

int pwmInactiveAll(void) {
    pwmWrite(COLOR_LED_DEV_R_ PWM_UNEXPORT, "0");
    pwmWrite(COLOR_LED_DEV_G_ PWM_UNEXPORT, "0");
    pwmWrite(COLOR_LED_DEV_B_ PWM_UNEXPORT, "0");
    return 1;
}

int pwmSetDuty(int dutyCycle, int pwmIndex) {
    const char *path = NULL;
    char buf[64];

    switch (pwmIndex) {
        case PWM_COLOR_R: path = COLOR_LED_DEV_R_ PWM_DUTY; break;
        case PWM_COLOR_G: path = COLOR_LED_DEV_G_ PWM_DUTY; break;
        case PWM_COLOR_B: path = COLOR_LED_DEV_B_ PWM_DUTY; break;
        default: return -1;
    }

    snprintf(buf, sizeof(buf), "%d", dutyCycle);
    return pwmWrite(path, buf);
}

int pwmSetPeriod(int period, int pwmIndex) {
    const char *path = NULL;
    char buf[64];

    switch (pwmIndex) {
        case PWM_COLOR_R: path = COLOR_LED_DEV_R_ PWM_PERIOD; break;
        case PWM_COLOR_G: path = COLOR_LED_DEV_G_ PWM_PERIOD; break;
        case PWM_COLOR_B: path = COLOR_LED_DEV_B_ PWM_PERIOD; break;
        default: return -1;
    }

    snprintf(buf, sizeof(buf), "%d", period);
    return pwmWrite(path, buf);
}

int pwmSetPercent(int percent, int pwmIndex) {
    if (percent < 0 || percent > 100) return -1;

    int duty = (100 - percent) * PWM_PERIOD_NS / 100;  
    return pwmSetDuty(duty, pwmIndex);
}

int pwmStartAll(void) {
    pwmWrite(COLOR_LED_DEV_R_ PWM_ENABLE, "1");
    pwmWrite(COLOR_LED_DEV_G_ PWM_ENABLE, "1");
    pwmWrite(COLOR_LED_DEV_B_ PWM_ENABLE, "1");
    return 1;
}

int colorLedInit(void) {
    pwmActiveAll();
    pwmSetDuty(0, PWM_COLOR_R);
    pwmSetDuty(0, PWM_COLOR_G);
    pwmSetDuty(0, PWM_COLOR_B);
    pwmSetPeriod(PWM_PERIOD_NS, PWM_COLOR_R);
    pwmSetPeriod(PWM_PERIOD_NS, PWM_COLOR_G);
    pwmSetPeriod(PWM_PERIOD_NS, PWM_COLOR_B);
    pwmStartAll();
    return 1;
}

int colorLedSet(int r_percent, int g_percent, int b_percent) {
    pwmSetPercent(r_percent, PWM_COLOR_R);
    pwmSetPercent(g_percent, PWM_COLOR_G);
    pwmSetPercent(b_percent, PWM_COLOR_B);
    return 1;
}

int colorLedExit(void) {
    return pwmInactiveAll();
}
