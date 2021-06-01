#include "headers.h"
#define MAX 300

int front, rear;//parameters of priority queue

struct process
{
    // all process info from input file
    int pid;
    int arraival_time;
    int runing_time;
    int priority;

    struct PCB *P;

} Ready_Queue[MAX];

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
                Ready_Queue[front].P->Waiting_time = Ready_Queue[front].P->turnaround_time - Ready_Queue[front].arraival_time;

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
    while (!(rear == -1 && front == -1)) // while queue is not empty -QA
    {
        if (current_time - prev_time == 1) // now where are in time slot for 1 sec
        {
            curr_process = front ;
            if (prev_process != -1 && prev_process != curr_process)
            {
                // contextSwitching  QA
                // save prev process PCB QA
                // state of prev is stopped 
                // state of curr is resumed or started 
                // state of prev is stoped -->sto
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
                 Ready_Queue[front].P->status ="sta";
        
            }else{
                // process state resumed
                Ready_Queue[front].P->status ="res";
            }
            // we picking a process then we need to decreament or remaining time
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
                Ready_Queue[front].P->Waiting_time = Ready_Queue[front].P->turnaround_time - Ready_Queue[front].arraival_time;

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
    struct process AllProcesses[MAX];
     
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
		    	AllProcesses[process_count].pid=c - '0';

		    	break;
	    	case 1:
		    	AllProcesses[process_count].arraival_time=c - '0';

		    	break;
	    	case 2:
		    	AllProcesses[process_count].runing_time=c - '0';

		    	break;
	    	case 3:
		    	AllProcesses[process_count].priority=c - '0';

		    	break;
	    }
	    
	    i++;
	    if(i == 4)i=0;
	    
    	}
    	
    	
    }
        
    printf("process_count at the end: %d \n",process_count);
    for(i=0; i<process_count; i++)    
    {
    	printf("%d |",AllProcesses[i].pid);
    	printf("%d |",AllProcesses[i].arraival_time);
    	printf("%d |",AllProcesses[i].runing_time);
    	printf("%d |",AllProcesses[i].priority);
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

/* Function to create an empty priority queue */
void create()
{
    front = rear = -1;
}

/* Function to insert value into priority queue */
void insert_by_priority(int data) 
{
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        Ready_Queue[rear].priority = data;
        return;
    }
    else
        check(data);
    rear++;
}

/* Function to check priority and place element */
void check(int data)
{
    int i, j;

    for (i = 0; i <= rear; i++)
    {
        if (data > Ready_Queue[i].priority)
        {
            for (j = rear + 1; j > i; j--)
            {
                Ready_Queue[j].priority = Ready_Queue[j - 1].priority;
            }
            Ready_Queue[i].priority = data;
            return;
        }
    }
    Ready_Queue[i].priority = data; //incase rear=-1
}

/* Function to delete an element from queue */
void delete_by_priority(int data)
{
    int i;

    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (data == Ready_Queue[i].priority)
        {
            for (; i < rear; i++)
            {
                Ready_Queue[i].priority = Ready_Queue[i + 1].priority;
            }

            Ready_Queue[i].priority = -99;
            rear--;

            if (rear == -1)
                front = -1;
            return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}

/* Function to display queue elements */
void display_pqueue()
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        printf(" %d ", Ready_Queue[front].priority);
    }

    front = 0;
}
