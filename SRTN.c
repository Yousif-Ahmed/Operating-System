// C++ program to implement Shortest Remaining Time First
// Shortest Remaining Time First (SRTF)

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include<time.h>


#define max_number_of_processes 50

typedef struct Process {
	int pid; // Process ID
	int bt; // Burst Time
	int art; // Arrival Time
	
};

// Function to find the waiting time for all
// processes
void findWaitingTime(Process proc[], int n,int wt[])
{
	int rt[n];

	// Copy the burst time into rt[]
	for (int i = 0; i < n; i++)
		rt[i] = proc[i].bt;

	int complete = 0, t = 0, minm = INT_MAX;
	int shortest = 0, finish_time;
	bool check = false;

	// Process until all processes gets
	// completed
	while (complete != n) {

		// Find process with minimum
		// remaining time among the
		// processes that arrives till the
		// current time`
		for (int j = 0; j < n; j++) {
			if ((proc[j].art <= t) &&
			(rt[j] < minm) && rt[j] > 0) {
				minm = rt[j];
				shortest = j;
				check = true;
			}
		}

		if (check == false) {
			t++;
			continue;
		}

		// Reduce remaining time by one
		rt[shortest]--;

		// Update minimum
		minm = rt[shortest];
		if (minm == 0)
			minm = INT_MAX;

		// If a process gets completely
		// executed
		if (rt[shortest] == 0) {

			// Increment complete
			complete++;
			check = false;

			// Find finish time of current
			// process
			finish_time = t + 1;

			// Calculate waiting time
			wt[shortest] = finish_time -
						proc[shortest].bt -
						proc[shortest].art;

			if (wt[shortest] < 0)
				wt[shortest] = 0;
		}
		// Increment time
		t++;
	}
}

// Function to calculate turn around time
void findTurnAroundTime(Process proc[], int n,int wt[], int tat[])
{
	// calculating turnaround time by adding
	// bt[i] + wt[i]
	for (int i = 0; i < n; i++)
		tat[i] = proc[i].bt + wt[i];
}

// Function to calculate average time
void findavgTime(Process proc[], int n)
{
	int wt[n], tat[n], total_wt = 0,total_tat = 0;

	// Function to find waiting time of all
	// processes
	findWaitingTime(proc, n, wt);

	// Function to find turn around time for
	// all processes
	findTurnAroundTime(proc, n, wt, tat);

	// Display processes along with all
	// details
	cout << "Processes "
		<< " Burst time "
		<< " Waiting time "
		<< " Turn around time\n";

	// Calculate total waiting time and
	// total turnaround time
	for (int i = 0; i < n; i++) {
		total_wt = total_wt + wt[i];
		total_tat = total_tat + tat[i];
		cout << " " << proc[i].pid << "\t\t"
			<< proc[i].bt << "\t\t " << wt[i]
			<< "\t\t " << tat[i] << endl;
	}

	cout << "\nAverage waiting time = "
		<< (float)total_wt / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)total_tat / (float)n;
}
struct CompareHeight {
    bool operator()
    {
        // return "true" if "p1" is ordered
        // before "p2", for example:
        return 1;
    }
};
int main()
{
	//Process proc[]= new int [max_number_of_processes];
	int i=0; //number of processes
	bool interrupt=0;
        priority_queue<Process, vector<Process>, CompareHeight> Q;
	
	while(true)
	{
		sleep(1000);
		printf("Do you want to add process? 1:yes, 0:no ..");
		scanf("%d",&interrupt);
		
		if(interrupt)//I need real interrupt
		{
			int pid,bt,art;
			printf("Enter process ID");
			scanf("%d",&pid);
			
		        printf("Enter process Burst Time");
			scanf("%d",&bt);
			
			printf("Enter process Arrival Time");
			scanf("%d",&art);

			Q.push(Process(pid, bt ,art));			
			
			//findavgTime(Q, i+1);
			
			i++;
			interrupt=0;
		}
		
	}
	
	return 0;
}

