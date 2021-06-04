#include "FirstComeFirstServe.h"

void FCFS()
{

    // First come first serve (FCFS)
      
    // this array will receive all information from process_generator
    queue arr[MAX];
    int start_arr_index = 0;  // incremented when we procced in finishing processes
    int end_arr_index = -1;    // incremented when a new process comes


    int n =MAX ; // asssume that is number of processes 
    int completed =0;
    int running = false ;
    int prev_time = getClk()-1;

    while (completed  != n) // while completed process not equal all processes
    {
        if (getClk() - prev_time == 1 )
        {
        /*

        TO DO
        check here for the generator if it sent a new process to increment end_arr_index
        
        */
        prev_time = getClk();

        }
       

        if (end_arr_index - start_arr_index >= 0 && !running ) 
        {  
            // here there is an existing process
            
            running = true ;
           
            // fork it now

            // set its parameters  
            arr[start_arr_index].pcb->start_time = getClk() ;
            arr[start_arr_index].pcb->run_for_first = false ;
            arr[start_arr_index].pcb->Waiting_time = getClk() - arr[start_arr_index].arraival_time ;            
            // equal to waiting time
            arr[start_arr_index].pcb->response_time = arr[start_arr_index].pcb->start_time - arr[start_arr_index].arraival_time ;


            total_idle_time     +=  arr[start_arr_index].pcb->start_time - arr[start_arr_index].arraival_time ;
            total_response_time +=  arr[start_arr_index].pcb->start_time - arr[start_arr_index].arraival_time ;
            total_running_time  +=  arr[start_arr_index].running_time ;     

        }
        else if (running)
        {
            if (getClk() - arr[start_arr_index].pcb->start_time  == arr[start_arr_index].running_time)
            {
                arr[start_arr_index].pcb->finish_time = getClk() ;
                arr[start_arr_index].pcb->turnaround_time = arr[start_arr_index].pcb->finish_time - arr[start_arr_index].arraival_time  ;
                arr[start_arr_index].pcb->excution_time = arr[start_arr_index].pcb->finish_time - arr[start_arr_index].pcb->start_time  ;
                arr[start_arr_index].pcb->remaining_time = 0 ;

                // set statistics

                total_waiting_time    += arr[start_arr_index].pcb->Waiting_time ;
                total_turnaround_time += arr[start_arr_index].pcb->turnaround_time;                
                total_response_time   += arr[start_arr_index].pcb->response_time ;
                start_arr_index       += 1 ;
                completed             += 1 ;

                // send signal to the process to stop

                running = false ;
            }
            else
            {
                arr[start_arr_index].pcb->remaining_time = getClk() - arr[start_arr_index].pcb->start_time ;
            }
        }

    }

    int finish_time = getClk();
    avg_waiting_time = (float) total_waiting_time / n; 
}