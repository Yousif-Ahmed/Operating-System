#include "queue.h"
#include "headers.h"
#include "FirstComeFirstServe.h"
#include "ShortestJobFirst.h"
#include "ShortestRemainingTimeNext.h"
#include "Preemptive_HPF.h"
#include "RoundRobin.h"



queue Ready_Queue[MAX];
int currentProcess = 0;

void down(int semid);

int main(int argc, char *argv[])
{

	initClk();

	//@TODO: initialize the message queue for communicating with processes
	queueKey = ftok("keyfile", 'Q');

	if ((queueId = msgget(queueKey, 0666 | IPC_CREAT)) == -1){exit(-1);}

	//@TODO: read process_count from process_generator
	while(msgrcv(queueId, &parameter, sizeof(parameter.value), 0, 0) == -1){}
	process_count = parameter.value;
	
	//@TODO: read Selected Algorithm and its parameters
	while(msgrcv(queueId, &parameter, sizeof(parameter.value), 0, 0) == -1){}
	Algo = parameter.value;
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

	//cases scheduling
	switch (Algo)
	{
	case RoundRobin:
		while (true)
		{
			RoundRobinAlgo(Ready_Queue, quantumTime, currentProcess);
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
			Preemptive_HPF();
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
	}
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