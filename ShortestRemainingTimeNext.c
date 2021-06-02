#include "ShortestRemainingTimeNext.h"


void SRTN(queue* Ready_Queue)
{
    //Shortest Remaining Time Next ---> SRTN
    // assume that ready queue has all information
    // pick the first process
    // check if any process has enter, add it to queue
    //check if remaining time of new process is smaller than working now
    
    // picking the first process
    int current_time = getClk();
    int prev_time = 0; //previous time
    int curr_process;
    int prev_process = -1;
    
    while (!(rear == -1 && front == -1)) // while queue is not empty -QA
    {
    	
		
        if (current_time - prev_time == 1) // now we are in time slot for 1 sec
        {
            curr_process = front ;
            
            //if the previous process has higher remaining time than the current one
            if (prev_process != -1 && prev_process != curr_process)// I guess you meant prev_process instead of prev
            {
                // contextSwitching  QA
                // save prev process PCB QA //prev process params is arleady saved as we go through code, it's always updated
                
                // state of curr is started 
                Ready_Queue[curr_process].pcb->status ="sta";
                
                // state of prev_process is stoped -->sto
                Ready_Queue[prev_process].pcb->status ="sto";

            }
            if (Ready_Queue[front].pcb->run_for_first) // if run first set the start time
            {
                // Setting ST for the process
                Ready_Queue[front].pcb->start_time = current_time;
                // setting bool to false
                Ready_Queue[front].pcb->run_for_first = false;
                // total idle time for the process
                total_idle_time+= Ready_Queue[front].pcb->start_time-Ready_Queue[front].arraival_time ;
                // state of the current process is running now 
                 Ready_Queue[front].pcb->state = 'r' ;
                 // status of the current process start ;
                 // Ready_Queue[front].P->status ="sta"; //it's better to be in previous if condition. 
                 // because it should start immediatly, it doesn't matter if it's first run or otherwise
        
            }else{
                // process state resumed
                // Ready_Queue[front].P->status ="res"; //why?
            }
            // we picking a process then we need to decreament remaining time
            (Ready_Queue[front].pcb->remaining_time )--;
            prev_time = current_time;
            // process complete
            if (Ready_Queue[front].pcb->remaining_time == 0)
            {
                // TODO --> WTA
                // remember :the state----> TODO
                // Finish Time Calculation
                Ready_Queue[front].pcb->finish_time = current_time;
                // TA_time = FT -AT
                Ready_Queue[front].pcb->turnaround_time = Ready_Queue[front].pcb->finish_time - Ready_Queue[front].arraival_time;
                // WT = TA_time - AT
                Ready_Queue[front].pcb->Waiting_time = Ready_Queue[front].pcb->turnaround_time - Ready_Queue[front].running_time;

                // OUR GLOBAL TIMES
                total_waiting_time += Ready_Queue[front].pcb->Waiting_time ;
                total_turnaround_time+= Ready_Queue[front].pcb->turnaround_time;
                total_response_time += Ready_Queue[front].pcb->response_time ;

                // setting state as completed 
                Ready_Queue[front].pcb->status= "fin" ;
            }
            current_time = getClk();
            prev_process = curr_process;
        }
    }
}
