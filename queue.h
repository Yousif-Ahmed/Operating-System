#ifndef QUEUE_H
#define QUEUE_H

typedef short bool;
#define true 1
#define false 0
#define MAX 300

int front;
int rear;


enum State {
		Started,
		Stopped,
		Resumed,	 	
		Finished
}state;

typedef struct Queue
{

    int id;
    int arraival_time;
    int running_time;
    int priority;

    struct PCB *pcb;
}queue;
// process control block
struct PCB
{
    // r  -->for running
    // w  -->for waiting
    char state;
    // Started --> start
    // Stopped --> stopped
    // Resumed -->resumed
    // Finished --> finished
    enum State processState;
    
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

void createQueue();
void insert_by_priority(queue *qu, int data);
void check(queue *qu,int data);
void delete_by_priority(queue *qu,int data);
void display_pqueue(queue *qu);
int getSize();
bool is_empty();
#endif
