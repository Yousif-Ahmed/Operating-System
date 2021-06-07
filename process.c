#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

// SIGSTOP
// SIGCONT
//signal(SIGSTOP, )

int main(int agrc, char *argv[])
{
    initClk();
    if ((queueId = msgget(queueKey, 0666 | IPC_CREAT)) == -1){exit(-1);}
    //TODO The process needs to get the remaining time from somewhere
    while(msgrcv(queueId, &parameter, sizeof(parameter.value),getpid(), 0)==-1){}
    remainingtime = parameter.value;
    while (remainingtime > 0)
    {
        while(msgrcv(queueId, &parameter, sizeof(parameter.value),getpid(), 0)==-1){}
        remainingtime = parameter.value;
    }

    destroyClk(false);

    return 0;
}
