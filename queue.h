#define MAX 300
int front;
int rear;


typedef struct
{

    int pid;
    int arraival_time;
    int running_time;
    int priority;

    struct PCB *P;
}queue;

void createQueue(queue *qu, int front, int rear);
void insert_by_priority(queue *qu, int data);
void check(queue *qu,int data);
void delete_by_priority(queue *qu,int data);
void display_pqueue(queue *qu);

