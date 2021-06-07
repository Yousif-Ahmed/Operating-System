#include "queue.h"

void createQueue(){
    front = -1;
    rear = -1;
}
int getSize(){
    return rear-front;
}
void insert_by_priority(queue *qu, int data){
    if (rear >= MAX - 1)
    {
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }
    if ((front == -1) && (rear == -1))
    {
        front++;
        rear++;
        qu[rear].priority = data;
        return;
    }
    else
        check(qu, data);
    rear++;
}
void check(queue *qu, int data)
{
    int i, j;

    for (i = 0; i <= rear; i++)
    {
        if (data >= qu[i].priority)
        {
            for (j = rear + 1; j > i; j--)
            {
                qu[j].priority = qu[j - 1].priority;
            }
            qu[i].priority = data;
            return;
        }
    }
    qu[i].priority = data; //incase rear=-1
}
/* Function to delete an element from queue */
void delete_by_priority(queue *qu,int data)
{
    int i;

    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty no elements to delete");
        return;
    }

    for (i = 0; i <= rear; i++)
    {
        if (data == qu[i].priority)
        {
            for (; i < rear; i++)
            {
                qu[i].priority = qu[i + 1].priority;
            }

            qu[i].priority = -99;
            rear--;

            if (rear == -1)
                front = -1;
            return;
        }
    }
    printf("\n%d not found in queue to delete", data);
}

/* Function to display queue elements */
void display_pqueue(queue *qu)
{
    if ((front == -1) && (rear == -1))
    {
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++)
    {
        printf(" %d ", qu[front].priority);
    }

    front = 0;
}
bool is_empty()
{
	// check that ready queue is empty
	return (rear == front) ? true : false;
}
