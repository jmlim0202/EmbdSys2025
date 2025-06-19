#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "button.h"

int main(void) {
    BUTTON_MSG_T recvMsg;

    if (!buttonInit()) {
        printf("Button init failed\n");
        return 1;
    }

    printf("Listening for button events...\n");
    while (1) {
        msgrcv(msgget(MESSAGE_ID, 0666), &recvMsg, sizeof(recvMsg) - sizeof(long int), 0, 0);
        printf("Button: %d %s\n", recvMsg.keyInput, recvMsg.pressed ? "pressed" : "released");
    }

    buttonExit();
    return 0;
}
