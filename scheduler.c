#include "headers.h"
#define MAX 300

int front, rear;
struct process
{
    // all process info from input file
    int pid;
    int arraival_time;
    int runing_time;
    int priority;

    struct PCB P;

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
    int prev = 0;
    int curr_process;
    int prev_process = -1;
    while (!(rear == -1 && front == -1)) // while queue is not empty -QA
    {
        if (current_time - prev == 1) // now where are in time slot for 1 sec
        {
            curr_process = front ;
            if (prev_process != -1 && prev != curr_process)
            {
                // contextSwitching  QA
                // save prev process PCB QA
                // state of prev is stopped 
                // state of curr is resumed or started 
                // state of prev is stoped -->sto
                Ready_Queue[prev_process].P.status ='sto';

            }
            if (Ready_Queue[front].P.run_for_first) // if run first set the start time
            {
                // Setting ST for the process
                Ready_Queue[front].P.start_time = current_time;
                // setting bool to false
                Ready_Queue[front].P.run_for_first = false;
                // total idle time for the process
                total_idle_time+= Ready_Queue[front].P.start_time-Ready_Queue[front].arraival_time ;
                // state of the current process is running now 
                 Ready_Queue[front].P.state = 'r' ;
                 // status of the current process start ;
                 Ready_Queue[front].P.status ='sta';
        
            }else{
                // process state resumed
                Ready_Queue[front].P.state ='res';
            }
            // we picking a process then we need to decreament or remaining time
            (Ready_Queue[front].P.remaining_time )--;
            prev = current_time;
            // process complete
            if (Ready_Queue[front].P.remaining_time == 0)
            {
                // TODO --> WTA
                // remember :the state----> TODO
                // Finish Time Calculation
                Ready_Queue[front].P.finish_time = current_time;
                // TA_time = FT -AT
                Ready_Queue[front].P.turnaround_time = Ready_Queue[front].P.finish_time - Ready_Queue[front].arraival_time;
                // WT = TA_time - AT
                Ready_Queue[front].P.Waiting_time = Ready_Queue[front].P.turnaround_time - Ready_Queue[front].arraival_time;

                // OUR GLOBAL TIMES
                total_waiting_time += Ready_Queue[front].P.Waiting_time ;
                total_turnaround_time+= Ready_Queue[front].P.turnaround_time;
                total_response_time += Ready_Queue[front].P.response_time ;

                // setting state as completed 
                Ready_Queue[front].P.state= 'fin' ;
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
    // case 3 for scheduling 
    Preemptive_HPF();

    destroyClk(true);
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
        if (data >= Ready_Queue[i].priority)
        {
            for (j = rear + 1; j > i; j--)
            {
                Ready_Queue[j].priority = Ready_Queue[j - 1].priority;
            }
            Ready_Queue[i].priority = data;
            return;
        }
    }
    Ready_Queue[i].priority = data;
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
