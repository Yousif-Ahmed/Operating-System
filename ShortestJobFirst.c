#include "ShortestJobFirst.h"

queue get_min_running_time()
{
    // do some logic here
    queue s ;
    return s;
}

void SJF()
{
    // Shortest Job First (SJF)
      
    // this array will receive all information from process_generator
    // assume that there is a priority queue that is sorted depending on running time
    
    int current_time = getClk();
    int prev_time = 0;
    int prev_process = -1;
    int n =MAX ; // asssume that is number of processes 
    int completed =0;
    queue curr_process;

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
       

        if (!is_empty() && !running ) 
        {  
            // here there is an existing process
            
            running = true ;
           
            // fork it now

            curr_process = get_min_running_time() ;

            // set its parameters  
            curr_process.pcb->start_time = getClk() ;
            curr_process.pcb->run_for_first = false ;
            curr_process.pcb->Waiting_time = getClk() - curr_process.arraival_time ;            
            // equal to waiting time
            curr_process.pcb->response_time = curr_process.pcb->start_time - curr_process.arraival_time ;


            total_idle_time     +=  curr_process.pcb->start_time - curr_process.arraival_time ;
            total_response_time +=  curr_process.pcb->start_time - curr_process.arraival_time ;
            total_running_time  +=  curr_process.running_time ;     

        }
        else if (running)
        {
            if (getClk() - curr_process.pcb->start_time  == curr_process.running_time)
            {
                curr_process.pcb->finish_time = getClk() ;
                curr_process.pcb->turnaround_time = curr_process.pcb->finish_time - curr_process.arraival_time  ;
                curr_process.pcb->excution_time = curr_process.pcb->finish_time - curr_process.pcb->start_time  ;
                curr_process.pcb->remaining_time = 0 ;

                // set statistics

                total_waiting_time    += curr_process.pcb->Waiting_time ;
                total_turnaround_time += curr_process.pcb->turnaround_time;                
                total_response_time   += curr_process.pcb->response_time ;
                start_arr_index       += 1 ;
                completed             += 1 ;

                // send signal to the process to stop

                running = false ;
            }
            else
            {
                curr_process.pcb->remaining_time = getClk() - curr_process.pcb->start_time ;
            }
        }

    }

    int finish_time = getClk();

    avg_waiting_time = (float) total_waiting_time / n; 
    
}
