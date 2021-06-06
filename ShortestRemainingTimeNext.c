#include "ShortestRemainingTimeNext.h"


void SRTN(queue* Ready_Queue)
{
    //Shortest Remaining Time Next ---> SRTN
    // assume that ready queue has all information
    // pick the first process
    // check if any process has enter, add it to queue
    //check if remaining time of new process is smaller than working now
    
    int current_time = getClk();
    int prev_time = currentTime - 1; 
    int currentProcess=front;
    int prev_process = -1;
    
    while (completedProcesses != process_count) // while processes not finished 
    {	
        if ((currentTime = getClk()) - prev_time == 1)
        {
            prev_time = currentTime;
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
                if ((pID = fork()) == 0)    // fork the process to a child
                {
                    Ready_Queue[currentProcess].pid = pID;                                      // store the process ID so we can use it later //pID=0
                    char *args[] = {"./process.out", NULL};
                execl("./process.out","./process.out",(char*) NULL);                            // run the process.c file
                                                               
                    exit(-1);                                                                   // if the process.c couldn't run, exit >> since this is the forked child not the parent "Scheduler"
                }
                
		//the parent continue work in the algorithm
		
                // Send the data to the forked process
                parameter.mtype = Ready_Queue[currentProcess].pid;
                parameter.value = Ready_Queue[currentProcess].running_time;
                while (msgsnd(queueId, &parameter, sizeof(parameter.value), 0) == -1){}
                
                // update PCB of the process in the scheduler memory
                //initialization for the process first run
                Ready_Queue[currentProcess].pcb->start_time = currentTime;
                Ready_Queue[currentProcess].pcb->remaining_time = Ready_Queue[currentProcess].running_time;
                continue;
            }
            if(Ready_Queue[currentProcess].pcb->processState == Stopped){
                Ready_Queue[currentProcess].pcb->processState = Resumed;

                kill(Ready_Queue[currentProcess].pid, SIGCONT);//wake up from stop
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
            
            //main algorithm idea of SRTN
            if (currentTime % quantumTime == 0 && currentTime != 0) //stop this process and continue/begin the lower running time one
            {
                Ready_Queue[currentProcess].pcb->processState = Stopped;

                kill(Ready_Queue[currentProcess].pid, SIGSTOP);//resume the current process
                currentProcess = (currentProcess == rear) ? front : currentProcess + 1;
            }
        }
}
















