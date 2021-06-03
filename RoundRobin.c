#include "RoundRobin.h"

void RoundRobinAlgo(queue *Ready_Queue, int qTime, int *processIdx)
{
    int currentProcess = front;
    int prevProcess = 0;
    int currentTime = getClk();
    int lastTimeStamp = currentTime - 1;
    while (completedProcesses != process_count)
    {
        if ((currentTime = getClk()) - lastTimeStamp == 1)
        {
            lastTimeStamp = currentTime;
            //@TODO: Implement the function
            updateQueue();
            if(front==-1){              // empty time slot with no ready processes
                continue;
            }
            if (Ready_Queue[currentProcess].pcb->run_for_first)
            { // If this is the first time to execute the current Process
                Ready_Queue[currentProcess].pcb->run_for_first = false;                         // reset Flag
                Ready_Queue[currentProcess].pcb->processState = Started;

                int pID;
                if ((pID = fork()) == 0)                                                        // fork the process
                {
                    Ready_Queue[currentProcess].pid = pID;                                      // store the process ID so we can use it later
                    char *args[] = {"./process.out", NULL};
                execl("./process.out","./process.out",(char*) NULL);                            // run the process.c file
                                                               
                    exit(-1);                                                                   // if the process.c couldn't run, exit >> since this is the forked child not the parent "Scheduler"
                }

                // Send the data to the forked process
                parameter.mtype = Ready_Queue[currentProcess].pid;
                parameter.value = Ready_Queue[currentProcess].running_time;
                while (msgsnd(queueId, &parameter, sizeof(parameter.value), 0) == -1){}
                
                // update PCB of the process in the scheduler memory
                Ready_Queue[currentProcess].pcb->start_time = currentTime;
                Ready_Queue[currentProcess].pcb->remaining_time = Ready_Queue[currentProcess].running_time;
                continue;
            }
            if(Ready_Queue[currentProcess].pcb->processState == Stopped){
                Ready_Queue[currentProcess].pcb->processState = Resumed;

                kill(Ready_Queue[currentProcess].pid, SIGCONT);
            }
            // Previously processed process, Update its PCB
            Ready_Queue[currentProcess].pcb->remaining_time--;

            // Notify the process with its remaining Time
            parameter.mtype = Ready_Queue[currentProcess].pid;
            parameter.value = Ready_Queue[currentProcess].running_time;
            while (msgsnd(queueId, &parameter, sizeof(parameter.value), 0) == -1){}

            // Finished process? Delete its data from memory
            if (Ready_Queue[currentProcess].pcb->remaining_time == 0){
                Ready_Queue[currentProcess].pcb->processState =Finished;
                delete_by_priority(Ready_Queue, Ready_Queue[currentProcess].priority);
                completedProcesses++;
            }
            if (currentTime % quantumTime == 0 && currentTime != 0)         // New Time Slot for RR Algorithm >> stop this process and continue the next one
            {
                Ready_Queue[currentProcess].pcb->processState = Stopped;

                kill(Ready_Queue[currentProcess].pid, SIGSTOP);
                currentProcess = (currentProcess == rear) ? front : currentProcess + 1;
            }
        }
    }
}
