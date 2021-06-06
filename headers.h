#ifndef HEADER_H
#define HEADER_H

#include <stdio.h> //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "queue.h"

#define SHKEY 300



queue *shmaddress;
key_t communicationKey;
int shmId = -1;
int semId = -1;
union Semun semun;

struct processMessage
{
	long mtype;
	queue process;
} processData;

struct Message
{
	long mtype;
	int value;
} parameter; //for message queue

key_t queueKey;
int queueId;

//Global Variables
int process_count = 0;
queue AllProcesses[MAX];

int quantumTime = 0;
int completedProcesses = 0;
// Global times for our Scheduler

float avg_waiting_time;

float CPU_Utilization;
int total_waiting_time = 0;
int total_idle_time =0 ;
int total_turnaround_time =0 ;
int total_response_time =0 ;
int total_running_time =0 ;
int start_arr_index = 0;
bool running = false;

// Message struct to use for communication
struct message
{
    long mtype;
    char msgText[256];
};
union Semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
    struct seminfo *__buf;
    void *__pad;
};


///==============================
//don't mess with this variable//
int *shmaddr; //
//===============================

int getClk()
{
    return *shmaddr;
}

/*
 * All processes call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *)shmat(shmid, (void *)0, 0);
}

/*
 * All processes call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}



/* 
* All Algorithms in scheduler.c call this method to check if the process generator has sent a new process
*/

void updateQueue(){

}

#endif
