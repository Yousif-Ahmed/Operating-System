#include "headers.h"
#include "queue.h"



void clearResources(int);
void readProcessesFile(queue *AllProcesses, char *filePath);
void validateArguments();
void up(int sem);

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
        printf("%d |", AllProcesses[i].pid);
        printf("%d |", AllProcesses[i].arraival_time);
        printf("%d |", AllProcesses[i].running_time);
        printf("%d |", AllProcesses[i].priority);
        printf("\n");
    }

    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int choice = 6;

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
        }
        //@TODO: check for other parameters
    }
    validateArguments();

    // 3. Initiate and create the scheduler and clock processes.
    char *args[]={"./clk.out",NULL};
    execl(args[0], args);       // run clk program
    args[0] = "./scheduler.out";
    execl(args[0], args);
    //system("./scheduler.out"); // run scheduler

    // 4. Use this function after creating the clock process to initialize clock.
    initClk();

    int x = getClk(); // To get time use this function.
    printf("Current Time is %d\n", x);
    

    // Shared Memory & Semaphore for communication between procoess_generator and scheduler
    communicationKey = ftok("keyfile", 'M');

    while (shmid == -1 || semid == -1)
    {
        shmid = shmget(communicationKey, 5, 0666 | IPC_CREAT);
        semid = semget(communicationKey, 1, 0666 | IPC_CREAT);
    }
    semun.val = 0;
    semctl(semid, 0, SETVAL, semun);

    // 5. Create a data structure for processes and provide it with its parameters.

    // TODO Generation Main Loop
    int processIdx = 0;
    while (processIdx < process_count)
    {
        // 6. Send the information to the scheduler at the appropriate time.
        if (getClk() == AllProcesses[processIdx].arraival_time)
        {
            shmaddress = shmat(shmid, (void *)0, 0);
            shmaddress = AllProcesses[processIdx];              // write the process info in the shared memory, so the scheduler can read it
            shmdt(shmaddress);
            processIdx++;
            //up(semid);
        }
    }
    
    // 7. Clear clock resources
    destroyClk(true);
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

    for (char c = getc(in_file); c != EOF; c = getc(in_file))
    {
        if (c == '#') //ignore the line has #
            while (c != '\n' && c != EOF)
                c = getc(in_file);

        else if (c == '\n') // Increment count if this character is newline
            process_count += 1;
        else if (c != '\n' && c != '\t')
        {
            switch (i)
            {
            case 0:
                AllProcesses[process_count].pid = c - '0';
                break;
            case 1:
                AllProcesses[process_count].arraival_time = c - '0';
                break;
            case 2:
                AllProcesses[process_count].running_time = c - '0';
                break;
            case 3:
                AllProcesses[process_count].priority = c - '0';
                break;
            }
            i = (i == 4) ? 0 : i + 1;
        }
    }
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