#include "RoundRobin.h"
#include "headers.h"

void RoundRobin(queue *Ready_Queue, int qTime)
{
    initClk();
    int currentProcess = 0;
    while (currentProcess != getSize())
    { // Iterate on Ready Processes Queue
        while(Ready_Queue[currentProcess].pcb->remaining_time && currentProcess!= getSize()){currentProcess++;}
        if (Ready_Queue[currentProcess].pcb->run_for_first)
        {                                                      // If this is the first time to execute the current Process
            Ready_Queue[currentProcess].pcb->start_time = getClk(); // Set the process starting time to current time
            Ready_Queue[currentProcess].pcb->remaining_time = Ready_Queue[currentProcess].running_time;
        }
        int currentTime = getClk();
        if (currentTime % qTime == 0)
        {                                                           // New Time slot
            Ready_Queue[currentProcess].pcb->remaining_time -= qTime; // Process the

            if (Ready_Queue[currentProcess].pcb->remaining_time <= 0)
            {   Ready_Queue[currentProcess];
                Ready_Queue[currentProcess].pcb->finish_time = currentTime + Ready_Queue[currentProcess].pcb->remaining_time;
                Ready_Queue[currentProcess].pcb->remaining_time = 0;
            }
            currentProcess++;
        }
    }
}
