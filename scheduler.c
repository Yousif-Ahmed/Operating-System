
#include "algorithms.h"

#include "queue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// queue of processes
//queue Ready_Queue;
int currentProcess = 0;

struct msgbuff
{
	long mtype;
	char mtext[256];
};
void Intialize();

void down(int semid);

int main(int argc, char *argv[])
{
	// receving the algoritm number
	initClk();

	printf(" schedulerrrrrrrrrrrrrrrrrrr\n");

	///////////// communication /////////////////////////////////////////////
	key_t key_id1 = ftok("keyfile", 65);			  //create unique key
	int msgq_id1 = msgget(key_id1, 0666 | IPC_CREAT); //create message queue and return id
	if (msgq_id1 == -1)
	{
		perror("Error in create");
		exit(-1);
	}
	printf("Message Queue2 ID = %d\n", msgq_id1);
	struct msgbuff message;
	printf("herrrrrrrrrrrrrrrrrrrrrrr\n");
	////////////////////////////////////////////////////////////////////////
	int completedProcesses = 0;
	int prev_time = getClk();
	int current_time;

	while (completedProcesses != 10)
	{
		current_time = getClk();
		if (current_time - prev_time >= 1)
		{

			int rec_val = msgrcv(msgq_id1, &message, sizeof(message.mtext), message.mtype, !IPC_NOWAIT);

			if (rec_val == -1)
				perror("Error in receive\n");
			else
			{

				printf("Message received from autogen %ld : %s \n", message.mtype, message.mtext);
			}
			completedProcesses++;
		}
		prev_time = current_time;
	}

	//@TODO: initialize the message queue for communicating with processes
	/*queueKey = ftok("keyfile", 'Q');

	if ((queueId = msgget(queueKey, 0666 | IPC_CREAT)) == -1){exit(-1);}

	//@TODO: read process_count from process_generator
	while(msgrcv(queueId, &parameter, sizeof(parameter.value), 0, 0) == -1){}
	process_count = parameter.value;
	
	//@TODO: read Selected Algorithm and its parameters
	while(msgrcv(queueId, &parameter, sizeof(parameter.value), 0, 0) == -1){}
	Algo = parameter.value - 1;
	switch (Algo)						// check if the algorithm has parameters, go read them
	{
		case RoundRobin:
			while(msgrcv(queueId, &parameter, sizeof(parameter.value), 0, 0) == -1){}
			quantumTime = parameter.value;
			break;	
		default:
			break;
	}

	createQueue(); //initialize Ready_Queue with front and rear with -1
*/
	//cases scheduling
	/*switch (Algo)
	{
	case RoundRobin:
		while (true)
		{
			RoundRobinAlgo(Ready_Queue, quantumTime, &currentProcess);
		}
		break;
	case FirstComeFirstServe:
		while (true)
		{
			FCFS();
		}
		break;
	case PreemptiveHPF:
		while (true)
		{
			Preemptive_HPF(Ready_Queue);
		}
		break;
	case ShortestJobFirst:
		while (true)
		{
			SJF();
		}
		break;
	case ShortestRemainingTimeNext:
		while (true)
		{
			SRTN(Ready_Queue);
		}
		break;
	}*/
	//@TODO: upon termination release the clock resources.
	destroyClk(true);
}
void down(int semid)
{
	struct sembuf buf = {
		.sem_num = 0, // sem for sending
		.sem_op = -1, // up
		.sem_flg = !IPC_NOWAIT};

	if (semop(semid, &buf, 1) == -1)
	{
		perror("Error in down send\n");
	}
}
