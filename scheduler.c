#include "queue.h"
#include "headers.h"
#include "FirstComeFirstServe.h"
#include "ShortestJobFirst.h"
#include "ShortestRemainingTimeNext.h"
#include "Preemptive_HPF.h"

queue Ready_Queue[MAX];

bool is_empty()
{

	// check that ready queue is empty
	return false;
}



queue AllProcesses[MAX];

int main(int argc, char *argv[])
{

	//TODO: implement the scheduler.
	//TODO: upon termination release the clock resources.

	initClk();


	createQueue(); //initialize Ready_Queue with front and rear with -1

	//cases scheduling


	// switch (choice)
	// {
	// case 3:
	// 	Preemptive_HPF();
	// 	break;

	// case 4:
	// 	SRTN();
	// 	break;
	// }


	destroyClk(true);
}
