#include "algorithms.h"
#include "headers.h"
#include "queue.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
void clearResources(int);
void readProcessesFile(queue *AllProcesses, char *filePath);
void validateArguments();
void up(int sem);
struct msgbuff
{
    long mtype;
    queue processes;
};
int main(int argc, char *argv[])
{
    
    signal(SIGINT, clearResources);

    // 1. Read the input files.
    char *filePath = argv[1];
    readProcessesFile(AllProcesses, filePath);
    // Test successful reading
    printf("process_count at the end: %d \n", process_count);
    for (int i = 0; i < process_count; i++)
    {
        printf("%d |", AllProcesses[i].id);
        printf("%d |", AllProcesses[i].arraival_time);
        printf("%d |", AllProcesses[i].running_time);
        printf("%d |", AllProcesses[i].priority);
        printf("\n");
    }
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int choice = 6;

    // parameters
    int parameter = -1;

    for (int i = 2; i < argc; i += 2)
    { // +2 since each paramter is a pair, like: { "-sch", 2 }, { "-q",  5 }
        if (argv[i] == "-sch" && i + 1 < argc)
        {
            //           1:FCFS      2:SJG      3:HPF      4:SRTN        5:RR
            Algo = atoi(argv[i + 1]);
        }
        else if (argv[i] == "-q" && i + 1 < argc)
        { // Quantum Time for Round Robin Algorithm
            quantumTime = atoi(argv[i + 1]);
            parameter = quantumTime;
        }
        //@TODO: check for other parameters
    }
    validateArguments();
    // 3. Initiate and create the scheduler and clock processes.
    // creating clk proceses
    printf("%d %d %d ", Algo, parameter, process_count);
    char *process_num = process_count + '0';
    char *pram = parameter + '0';
    char *sch_args[] = {argv[4], NULL};
    int pid1, pid2;

    pid1 = fork();
    if (pid1 != 0)
    {
        printf("pid clk = %d\n", pid1);
        // run clk program
        execv("./clk.out", NULL);
        exit(0);
    }
    else
    {
        pid2 = fork();

        if (pid2 != 0)
        {

            // 0---> process_num 1--->Algo 2---->parameters
            printf("pid sch = %d\n", pid2);
            execv("./scheduler.out", sch_args);
            exit(0);
        }
        else
        {

            printf("parent pid = %d\n", getpid());
            // 4. Use this function after creating the clock process to initialize clock.
            initClk();
            int completed_process = 0;
            // 5. Create a data structure for processes and provide it with its parameters.

            ///////////// communication /////////////////////////////////////////////
            key_t key_id1 = ftok("keyfile", 65); //create unique key

            int msgq_id1 = msgget(key_id1, 0666 | IPC_CREAT); //create message queue and return id
            if (msgq_id1 == -1)
            {
                perror("Error in create");
                exit(-1);
            }
            printf("Message Queue1 ID = %d\n", msgq_id1);
            struct msgbuff message;
            ////////////////////////////////////////////////////////////////////////////////

            int current_time;
            int prev_time = getClk();
            int Index = 0;
            int x = getClk(); // To get time use this function.
            printf("Current Time is %d\n", x);
            // TODO Generation Main Loop
            while (completed_process != process_count)
            {
                current_time = getClk();
                if (current_time - prev_time >= 1)
                {
                    if (AllProcesses[Index].arraival_time == current_time)
                    {
                        printf("Current Time is %d\n", current_time);
                        message.processes =AllProcesses[Index];
                        //strcpy(message.mtext, "hi");
                        message.mtype = 7;
                        int send_val = msgsnd(msgq_id1, &message, sizeof(message.processes), IPC_NOWAIT);
                        printf(" send val = %d\n", send_val);
                        if (send_val == -1)
                            perror("Errror in send");
                        Index++;
                        completed_process++;    
                    }
                }
                prev_time = current_time;
            }
            // 7. Clear clock resources*/

            destroyClk(true);
        }
    }
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}

void validateArguments()
{
    if (Algo == RoundRobin && quantumTime <= 0)
    {
        printf("Please Enter valid quantum time for Round Robin Algorithm");
        exit(-1);
    }

    //@TODO: validate other Algorithms
}

void readProcessesFile(queue *AllProcesses, char *filePath)
{
    FILE *in_file = fopen(filePath, "r");
    if (in_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1);
    }

    int i = 0;
    int count_completeInt = 0;

    for (char c = getc(in_file); c != EOF; c = getc(in_file))
    {

        if (c == '#') //ignore the line has #
            while (c != '\n' && c != EOF)
                c = getc(in_file);

        else if (c == '\n') // Increment count if this character is newline
            process_count += 1;
        else if (c != '\n' && c != '\t')
        {
            char completeInt[10] = "";

            while (c != EOF && (c - '0' >= 0) && (c - '0' <= 9))
            {
                strncat(completeInt, &c, 1);
                c = getc(in_file);
            }

            if (count_completeInt == 4)
            {
                count_completeInt = 0;
                process_count += 1;
            }
            count_completeInt++;
            switch (i)
            {
            case 0:
                AllProcesses[process_count].id = atoi(completeInt);
                break;
            case 1:
                AllProcesses[process_count].arraival_time = atoi(completeInt);
                break;
            case 2:
                AllProcesses[process_count].running_time = atoi(completeInt);
                break;
            case 3:
                AllProcesses[process_count].priority = atoi(completeInt);
                break;
            }

            i++;
            if (i == 4)
                i = 0;
        }
    }
    process_count += 1;

    fclose(in_file); //close the file
}

void up(int semid)
{
    struct sembuf buf = {
        .sem_num = 1, // sem for sending
        .sem_op = 1,  // up
        .sem_flg = IPC_NOWAIT};

    if (semop(semid, &buf, 1) == -1)
    {
        perror("Error in up send\n");
    }
}
