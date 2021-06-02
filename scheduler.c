#include "queue.h"
#include "headers.h"
#define MAX 300

int front, rear;//parameters of priority queue

queue Ready_Queue[MAX];

// process control block
struct PCB
{
    // r  -->for running
    // w  -->for waiting
    char state;
    // sta --> start
    // sto --> stopped
    // res -->resumed
    // fin --> finished
    char * status ; // in document need to konw started --> stopped -->resumed -->resumed -->finished

    int excution_time;
    int remaining_time; // should intialized with running time and decreament it
    int Waiting_time;
    int start_time;
    int turnaround_time;
    int finish_time;
    int response_time;
    //int idle_time ;

    bool run_for_first; // intialized by true
};

// Global times for our Scheduler

float avg_waiting_time;

float CPU_Utilization;
int total_waiting_time = 0;
int total_idle_time =0 ;
int total_turnaround_time =0 ;
int total_response_time =0 ;
int total_running_time =0 ;



void FCFS()
{

    // First come first serve (FCFS)
      
    // this array will receive all information from process_generator
    process arr[MAX];
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
            arr[start_arr_index].P->start_time = getClk() ;
            arr[start_arr_index].P->run_for_first = false ;
            arr[start_arr_index].P->Waiting_time = getClk() - arr[start_arr_index].arraival_time ;            
            // equal to waiting time
            arr[start_arr_index].P->response_time = arr[start_arr_index].P->start_time - arr[start_arr_index].arraival_time ;


            total_idle_time     +=  arr[start_arr_index].P->start_time - arr[start_arr_index].arraival_time ;
            total_response_time +=  arr[start_arr_index].P->start_time - arr[start_arr_index].arraival_time ;
            total_running_time  +=  arr[start_arr_index].running_time ;     

        }
        else if (running)
        {
            if (getClk() - arr[start_arr_index].p->start_time  == arr[start_arr_index].running_time)
            {
                arr[start_arr_index].P->finish_time = getClk() ;
                arr[start_arr_index].P->turnaround_time = arr[start_arr_index].p->finish_time - arr[start_arr_index].arraival_time  ;
                arr[start_arr_index].P->excution_time = arr[start_arr_index].p->finish_time - arr[start_arr_index].p->start_time  ;
                arr[start_arr_index].P->remaining_time = 0 ;

                // set statistics

                total_waiting_time    += arr[start_arr_index].P->Waiting_time ;
                total_turnaround_time += arr[start_arr_index].P->turnaround_time;                
                total_response_time   += arr[start_arr_index].P->response_time ;
                start_arr_index       += 1 ;
                completed             += 1 ;

                // send signal to the process to stop

                running = false ;
            }
            else
            {
                arr[start_arr_index].P->remaining_time = getClk() - arr[start_arr_index].p->start_time ;
            }
        }

    }

    int finish_time = getClk();
    avg_waiting_time = (float) total_waiting_time / n; 
}

struct process get_min_running_time()
{
    // do some logic here
    struct process s ;
    return s;
}


bool is_empty()
{

    // check that ready queue is empty
    return false;

}

void SJF()
{
    // Shortest Job First (SJF)
      
    // this array will receive all information from process_generator
    // assume that there is a priority queue that is sorted depending on running time
    
    int current_time = getClk();
    int prev_time = 0;
    int curr_process;
    int prev_process = -1;
    int n =MAX ; // asssume that is number of processes 
    int completed =0;
    struct process curr_process;

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
            curr_process.P->start_time = getClk() ;
            curr_process.P->run_for_first = false ;
            curr_process.P->Waiting_time = getClk() - curr_process.arraival_time ;            
            // equal to waiting time
            curr_process.P->response_time = curr_process.P->start_time - curr_process.arraival_time ;


            total_idle_time     +=  curr_process.P->start_time - curr_process.arraival_time ;
            total_response_time +=  curr_process.P->start_time - curr_process.arraival_time ;
            total_running_time  +=  curr_process.running_time ;     

        }
        else if (running)
        {
            if (getClk() - curr_process.p->start_time  == curr_process.running_time)
            {
                curr_process.P->finish_time = getClk() ;
                curr_process.P->turnaround_time = curr_process.P->finish_time - curr_process.arraival_time  ;
                curr_process.P->excution_time = curr_process.P->finish_time - curr_process.P->start_time  ;
                curr_process.P->remaining_time = 0 ;

                // set statistics

                total_waiting_time    += curr_process.P->Waiting_time ;
                total_turnaround_time += curr_process.P->turnaround_time;                
                total_response_time   += curr_process.P->response_time ;
                start_arr_index       += 1 ;
                completed             += 1 ;

                // send signal to the process to stop

                running = false ;
            }
            else
            {
                curr_process.P->remaining_time = getClk() - curr_process.P->start_time ;
            }
        }

    }

    int finish_time = getClk();

    avg_waiting_time = (float) total_waiting_time / n; 
    
}








void SRTN()
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
                Ready_Queue[curr_process].P->status ="sta";
                
                // state of prev_process is stoped -->sto
                Ready_Queue[prev_process].P->status ="sto";

            }
            if (Ready_Queue[front].P->run_for_first) // if run first set the start time
            {
                // Setting ST for the process
                Ready_Queue[front].P->start_time = current_time;
                // setting bool to false
                Ready_Queue[front].P->run_for_first = false;
                // total idle time for the process
                total_idle_time+= Ready_Queue[front].P->start_time-Ready_Queue[front].arraival_time ;
                // state of the current process is running now 
                 Ready_Queue[front].P->state = 'r' ;
                 // status of the current process start ;
                 // Ready_Queue[front].P->status ="sta"; //it's better to be in previous if condition. 
                 // because it should start immediatly, it doesn't matter if it's first run or otherwise
        
            }else{
                // process state resumed
                // Ready_Queue[front].P->status ="res"; //why?
            }
            // we picking a process then we need to decreament remaining time
            (Ready_Queue[front].P->remaining_time )--;
            prev_time = current_time;
            // process complete
            if (Ready_Queue[front].P->remaining_time == 0)
            {
                // TODO --> WTA
                // remember :the state----> TODO
                // Finish Time Calculation
                Ready_Queue[front].P->finish_time = current_time;
                // TA_time = FT -AT
                Ready_Queue[front].P->turnaround_time = Ready_Queue[front].P->finish_time - Ready_Queue[front].arraival_time;
                // WT = TA_time - AT
                Ready_Queue[front].P->Waiting_time = Ready_Queue[front].P->turnaround_time - Ready_Queue[front].running_time;

                // OUR GLOBAL TIMES
                total_waiting_time += Ready_Queue[front].P->Waiting_time ;
                total_turnaround_time+= Ready_Queue[front].P->turnaround_time;
                total_response_time += Ready_Queue[front].P->response_time ;

                // setting state as completed 
                Ready_Queue[front].P->status= "fin" ;
            }
            current_time = getClk();
            prev_process = curr_process;
        }
    }
}

void Preemptive_HPF()
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
                Ready_Queue[prev_process].P->status ="sto";

            }
            if (Ready_Queue[curr_process].P->run_for_first) // if run first set the start time
            {
                // Setting ST for the process
                Ready_Queue[curr_process].P->start_time = current_time;
                // setting bool to false
                Ready_Queue[curr_process].P->run_for_first = false;
                // total idle time for the process
                total_idle_time+= Ready_Queue[curr_process].P->start_time-Ready_Queue[curr_process].arraival_time ;
                // state of the current process is running now 
                 Ready_Queue[curr_process].P->state = 'r' ;
                 // status of the current process start ;
                 Ready_Queue[curr_process].P->status ="sta";

                 // total running time of all processes
                 total_running_time+= Ready_Queue[curr_process].running_time;
        
            }else{
                // process state resumed
                Ready_Queue[curr_process].P->status ="res";
            }
            // we picking a process then we need to decreament or remaining time
            (Ready_Queue[curr_process].P->remaining_time )--;
            prev_time = current_time;
            // process complete
            if (Ready_Queue[curr_process].P->remaining_time == 0)
            {
                // TODO --> WTA
                // remember :the state----> TODO
                // Finish Time Calculation
                Ready_Queue[curr_process].P->finish_time = current_time;
                // TA_time = FT -AT
                Ready_Queue[curr_process].P->turnaround_time = Ready_Queue[front].P->finish_time - Ready_Queue[front].arraival_time;
                // WT = TA_time - AT
                Ready_Queue[curr_process].P->Waiting_time = Ready_Queue[front].P->turnaround_time - Ready_Queue[front].running_time;

                // OUR GLOBAL TIMES
                total_waiting_time += Ready_Queue[front].P->Waiting_time ;
                total_turnaround_time+= Ready_Queue[front].P->turnaround_time;
                total_response_time += Ready_Queue[front].P->response_time ;

                // setting state as completed 
                Ready_Queue[front].P->status= "fin" ;

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
int main(int argc, char *argv[])
{

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    initClk();
    
    FILE *in_file  = fopen("inputFile.txt", "r"); 
    if (in_file == NULL)
    {   
      printf("Error! Could not open file\n"); 
      exit(-1); 
    } 
    
    int process_count=0;
    int i=0;
     
    for (char c = getc(in_file); c != EOF; c = getc(in_file))
    {
    	if(c == '#')//ignore the line has #
    		while(c != '\n'  && c != EOF)
    			c = getc(in_file);

    	else if (c == '\n') // Increment count if this character is newline
            process_count += 1;
    	else if(c != '\n' && c != '\t')
    	{
	    switch(i)
	    {
	    	case 0:
		    	Ready_Queue[process_count].pid=c - '0';

		    	break;
	    	case 1:
		    	Ready_Queue[process_count].arraival_time=c - '0';

		    	break;
	    	case 2:
		    	Ready_Queue[process_count].running_time=c - '0';

		    	break;
	    	case 3:
		    	Ready_Queue[process_count].priority=c - '0';

		    	break;
	    }
	    
	    i++;
	    if(i == 4)i=0;
	    
    	}
    	
    	
    }
        
    printf("process_count at the end: %d \n",process_count);
    for(i=0; i<process_count; i++)    
    {
    	printf("%d |",Ready_Queue[i].pid);
    	printf("%d |",Ready_Queue[i].arraival_time);
    	printf("%d |",Ready_Queue[i].running_time);
    	printf("%d |",Ready_Queue[i].priority);
    	printf("\n");
    }
   	
    fclose(in_file);//close the file
    
    
    create(); //initialize Ready_Queue with front and rear with -1
    
    //cases scheduling 
    int choice; //1:FCFS 2:SJG 3:HPF 4:SRTN 5:RR
    printf("\nEnter the sceduling choice: 1:FCFS 2:SJG 3:HPF 4:SRTN 5:RR .. ");
    scanf("%d",&choice);
    switch(choice)
    {
    	case 3:
	    	Preemptive_HPF();
	    	break;
    
    	case 4:
	    	SRTN();
	    	break;
    }
    //destroyClk(true);
}



