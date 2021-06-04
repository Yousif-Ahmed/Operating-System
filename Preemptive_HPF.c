#include "Preemptive_HPF.h"

void Preemptive_HPF(queue * Ready_Queue)
{
    //Preemtive Highest Priority first (HPF)
    // assume that ready queue has all information
    // pick the first process
    // get the highest priority
    // note that in higher priority 1 > 5 QA
    // process with priority 1 has the higher priority
    // picking the first process
    int current_time = getClk();
    int prev_time = 0;
    int curr_process;
    int prev_process = -1;
    int n =MAX ; // asssume that is number of processes 
    int completed =0;
    while (completed  != n) // while completed process not equal all processes
    {
        if (current_time - prev_time == 1) // now where are in time slot for 1 sec
        {
            curr_process = front ; // current process for 1 sec then check again
            if (prev_process != -1 && prev_process != curr_process)
            {
                // contextSwitching  QA
                // save prev process PCB QA
                // state of prev is stopped 
                // state of curr is resumed or started 
                // state of prev is stoped -->sto
                Ready_Queue[prev_process].pcb->status ="sto";

            }
            if (Ready_Queue[curr_process].pcb->run_for_first) // if run first set the start time
            {
                // Setting ST for the process
                Ready_Queue[curr_process].pcb->start_time = current_time;
                // setting bool to false
                Ready_Queue[curr_process].pcb->run_for_first = false;
                // total idle time for the process
                total_idle_time+= Ready_Queue[curr_process].pcb->start_time-Ready_Queue[curr_process].arraival_time ;
                // state of the current process is running now 
                 Ready_Queue[curr_process].pcb->state = 'r' ;
                 // status of the current process start ;
                 Ready_Queue[curr_process].pcb->status ="sta";

                 // total running time of all processes
                 total_running_time+= Ready_Queue[curr_process].running_time;
        
            }else{
                // process state resumed
                Ready_Queue[curr_process].pcb->status ="res";
            }
            // we picking a process then we need to decreament or remaining time
            (Ready_Queue[curr_process].pcb->remaining_time )--;
            prev_time = current_time;
            // process complete
            if (Ready_Queue[curr_process].pcb->remaining_time == 0)
            {
                // TODO --> WTA
                // remember :the state----> TODO
                // Finish Time Calculation
                Ready_Queue[curr_process].pcb->finish_time = current_time;
                // TA_time = FT -AT
                Ready_Queue[curr_process].pcb->turnaround_time = Ready_Queue[front].pcb->finish_time - Ready_Queue[front].arraival_time;
                // WT = TA_time - AT
                Ready_Queue[curr_process].pcb->Waiting_time = Ready_Queue[front].pcb->turnaround_time - Ready_Queue[front].running_time;

                // OUR GLOBAL TIMES
                total_waiting_time += Ready_Queue[front].pcb->Waiting_time ;
                total_turnaround_time+= Ready_Queue[front].pcb->turnaround_time;
                total_response_time += Ready_Queue[front].pcb->response_time ;

                // setting state as completed 
                Ready_Queue[front].pcb->status= "fin" ;

                //increase completed counter
                completed++ ;

                // remember : we should delete process data here 
            }
            current_time = getClk();
            prev_process = curr_process;
        }
    }
    int finish_time = getClk();
    // here all process finished 
    // AVT
    avg_waiting_time = (float) total_waiting_time / n; 

}