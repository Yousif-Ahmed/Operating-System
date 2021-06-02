#include "queue.h"
#include "headers.h"
#include "FirstComeFirstServe.h"
#include "ShortestJobFirst.h"
#include "ShortestRemainingTimeNext.h"
#include "Preemptive_HPF.h"





bool is_empty()
{

    // check that ready queue is empty
    return false;

}




int main(int argc, char *argv[])
{

    //TODO: implement the scheduler.
    //TODO: upon termination release the clock resources.

    initClk();
    
    FILE *in_file  = fopen("inputFile.txt", "r"); 
    if (in_file == NULL)
    {   
      printf("Error! Could not open file\n"); 
      exit(-1); 
    } 
    
    int process_count=0;
    int i=0;
     
    for (char c = getc(in_file); c != EOF; c = getc(in_file))
    {
    	if(c == '#')//ignore the line has #
    		while(c != '\n'  && c != EOF)
    			c = getc(in_file);

    	else if (c == '\n') // Increment count if this character is newline
            process_count += 1;
    	else if(c != '\n' && c != '\t')
    	{
	    switch(i)
	    {
	    	case 0:
		    	Ready_Queue[process_count].pid=c - '0';

		    	break;
	    	case 1:
		    	Ready_Queue[process_count].arraival_time=c - '0';

		    	break;
	    	case 2:
		    	Ready_Queue[process_count].running_time=c - '0';

		    	break;
	    	case 3:
		    	Ready_Queue[process_count].priority=c - '0';

		    	break;
	    }
	    
	    i++;
	    if(i == 4)i=0;
	    
    	}
    	
    	
    }
        
    printf("process_count at the end: %d \n",process_count);
    for(i=0; i<process_count; i++)    
    {
    	printf("%d |",Ready_Queue[i].pid);
    	printf("%d |",Ready_Queue[i].arraival_time);
    	printf("%d |",Ready_Queue[i].running_time);
    	printf("%d |",Ready_Queue[i].priority);
    	printf("\n");
    }
   	
    fclose(in_file);//close the file
    
    
    create(); //initialize Ready_Queue with front and rear with -1
    
    //cases scheduling 
    int choice; //1:FCFS 2:SJG 3:HPF 4:SRTN 5:RR
    printf("\nEnter the sceduling choice: 1:FCFS 2:SJG 3:HPF 4:SRTN 5:RR .. ");
    scanf("%d",&choice);
    switch(choice)
    {
    	case 3:
	    	Preemptive_HPF();
	    	break;
    
    	case 4:
	    	SRTN();
	    	break;
    }
    //destroyClk(true);
}



