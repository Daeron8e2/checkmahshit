#include <stdio.h>
#include <stdlib.h>

#define ROWS 7
#define COLOUMNS 6
#define MEMORY_SIZE 20
#define SLEEP 0
#define END 1

// Linked list of all processes to be run
struct queue{
	int pID;				// Process ID
	int startTime;			// Start Time
	int size;				// Process Size
	int executionTime;		// Execution Interval
	int pState;				// Process State
	int endState;			// Process state at the end of execution
							// SLEEP = 0  :  END/Terminate = 1
	// Metrics used for performance comparisons
	int waitTime;
	int turnAround;
    struct queue *next;
}; 
struct queue *firstq, *currentq, *newq;

// Linked list of all processes in memory
struct memory{
	int pID;				// Process ID
	int startTime;			// Start Time
	int size;				// Process Size
	int executionTime;		// Execution Interval
	int pState;		// Process State
	int endState;			// Process state at the end of execution
							// SLEEP = 0  :  END/Terminate = 1
	// Metrics used for performance comparisons
	int waitTime;
	int turnAround;
    struct memory *next;
}; struct memory *firstp, *currentp, *newp;

int memory_fill = 0; // Keeps track of how full memory is

/* Operations/functions of this program */
void create_queue(int job[]); // Creates linked list for processes that will 
void add_to_memory(struct queue *queue_process, int memState);
void terminate(void); // Handles sleep and end/deallocate states
void swapping(void); // Scheduler uses this function to add from virtual memory to main memory
void listQueue(void);
void deallocateQueue(void); // This just removes the processes in the queue from actual memory
void calculateMetrics(); // Function to calculate the metrics for later analysis
void deallocate_from_memory(struct queue *queue_process, int processID);
void CPU(int time);
// Different scheduling algorithms that can be used by scheduler
void first_in_first_out(); void calculate_fifo(void);
void shortest_job_first();
void round_robin();

void check_array(int array[]){
	for(int x = 0; x < COLOUMNS; x++){
    	printf("%d", array[x]);
	}
	printf("\n");
}

// Driver Function
int main() {
	int k;
	/* array holding all the processes. As the process information is static,
	   all processes have to be added manually */
	int processes[ROWS][COLOUMNS] = { {1,1,2,7,0,1},{2,2,3,8,0,0},{3,3,4,6,0,1},
				{4,4,3,6,0,0},{5,5,2,9,0,0},{6,6,3,6,0,0},{7,7,2,6,0,0} };	// First 7 processes to be run
	int process[COLOUMNS];
	//listQueue();
	for(int i=0;i<=ROWS;i++)
	{
		k=0;
		for(int j=0;j<=COLOUMNS;j++)
		{
			process[k] = processes[i][j];
			k++;
		}
		create_queue(process);
	}
	listQueue();
	
	first_in_first_out();

	// shortest_job_first();
	// round_robin();

	deallocateQueue(); // Deallocate the queue from memory
	puts("Exiting");
	return 0;
}

// This schedules the processes to memory and calculates all of the necessary metrics
void first_in_first_out(){
	// First start with variables
	int numOfProcesses = 0, newProcessState = 0;
	int timeslice = 0;

	// Count how many processes
	while(currentq->next != NULL){
		numOfProcesses++;
		currentq = currentq->next;
	}

	calculate_fifo();

	currentq = firstq;
	// Run operations for the amount processes we have
	while(numOfProcesses > 0){ // We minus one every time a process finished execution
		while(currentq->next != NULL){
			// Check to see if 
			if(currentq->startTime <= timeslice){
				if(memory_fill >= currentq->size + memory_fill){ // If process fits into main memory
					add_to_memory(currentq, 1); // Set to ready state
				}
				else if(memory_fill <= currentq->size + memory_fill){
					add_to_memory(currentq, 4); // Set to suspend in the virtual memory
				}
			}
		}
		CPU(timeslice);
		timeslice++;
	}
}

/* To calculate the metrics for the fifo algorithm */
void calculate_fifo(void){
    int current_time = 0;
	while(currentq->next != NULL) {
    	//if arrival is < current time, wait time = current time - arrival
        if(currentq->startTime < current_time)
            currentq->waitTime = current_time - currentq->startTime;
    	//if arrival is >= current time, wait time = 0;
        else if(currentq->startTime >= current_time)
            currentq->waitTime = 0;
    	
        current_time = current_time + currentq->executionTime; //current time = current time + wait time + time to completion
        currentq->turnAround = currentq->waitTime + currentq->executionTime; //turnaround time = wait time + time to completion
    }


}

/* This function will handle the cpu allocation and checking of time */
void CPU(int time){
	int runState = 2, readyState = 1;
	if(firstp == NULL){
		puts("No processes to execute in the CPU");
		return;
	}

	if(firstp->pState != runState){
		firstp->pState = runState; // Set the first process to be executing
		printf("Process %d is now scheduled to execute in the CPU.", firstp->pID);
	}else if(firstp->startTime + firstp->executionTime >= time && firstp->pState == runState){
		printf("Process %d has now finished running and will now terminate.", firstp->pID);
		terminate();
	}
}

/* The function that removes the processes one at a time */
void terminate(void) {
	if(firstp==NULL)
        return;

	currentp = firstp; // points to start of list

	while(currentp != NULL){
		if(currentp->pState == 2){
			if(currentp->endState == SLEEP){
				firstp = currentp->next;
				sleepFunction(currentp);
				printf("The process %d is now in secondary memory waiting for its next execution.", currentp->pID);
				free(currentp);
			}else if(currentp->endState == END){
				firstp = currentp->next;
				printf("Process %d has now been terminated.", currentp->pID);
				free(currentp);
			}
		}
	}
}

/* Handles those functions that need to sleep */
void sleepFunction(struct queue *process){
	currentq = firstq; // Start from the top
	while(currentq->next != NULL){
		if(process->pID == currentq->pID){
			currentq->turnAround = process->turnAround;
			currentq->waitTime = process->waitTime;
			currentq->pState = 5; // Suspend blocked state
			return;
		}
		currentq = currentq->next;
	}
}

/* add a queue item to the memory */
void add_to_memory(struct queue *queue_process, int state){
	newp = (struct memory *)malloc(sizeof(struct memory));
    
    if(firstp==NULL) 
        firstp = currentp = newp;
    else
    {
        currentp = firstp;  /* points to start of list */
                            
        while(currentp->next != NULL)
            currentp = currentp->next;
                // The last process is found
        currentp->next = newp;  // save the address of new
        currentp = newp;        // make current record new
    }
	/* Filling in the processes information */
    currentp->pID = queue_process->pID;
	currentp->startTime = queue_process->startTime;
	currentp->size = queue_process->size;
	currentp->executionTime = queue_process->executionTime;
	currentp->pState = queue_process->pState;
	currentp->endState = state; // Set the state to ready, rather than new
	currentp->next = NULL;
	if(currentp->pState == 1){
		printf("Process number %d has now been added to \"Main Memory\".\n", currentp->pID);
	}else if(currentp->pState == 4){
		printf("Process number %d has been scheduled to \"Secondary Memory\". \nIt will be scheduled to main memory when applicable.\n", currentp->pID);
	}
}

/* This function creates the linked list of structures to be run */
void create_queue(int job[]) {

	newq = (struct queue *)malloc(sizeof(struct queue));
    
    if(firstq==NULL)
        firstq = currentq = newq;
    else
    {
        currentq = firstq;  // first record made current
                            // loop through all groceries:
                            
        while(currentq->next != NULL)
            currentq = currentq->next;
                // The last process is found
        currentq->next = newq;  // save the address of new
        currentq = newq;        // make current record new
    }
	/* Filling in the processes information */
    currentq->pID = job[0];
	currentq->startTime = job[1];
	currentq->size = job[2];
	currentq->executionTime = job[3];
	currentq->pState = job[4];
	currentq->endState = job[5];
	currentq->next = NULL;
}

/* function to list all of the elements in the list */
void listQueue(void)
{
    if(firstq==NULL)
        puts("There are no records to print!");
    else{
		printf("%6s %-15s %-15s %6s\n", "ID","StartTime","Size","ProcessTime");
        currentq=firstq;
        do{
            printf("%5d: %-15d %-15d %d\n",
                currentq->pID,
                currentq->startTime,
                currentq->size,
                currentq->executionTime);
				currentq = currentq->next;
        }
        while(currentq->next != NULL);
    }
}

/* This function just aims to clear the memory once finished.
 * Each process will be cleared automatically, but in the event
 * there are still processes left in the queue, then there needs
 * to be a way to check and clear it*/
void clearMemory()
{
	do{
		if(firstp==NULL){
			puts("There is no queue to deallocate");
			break;
		}
		else
		{
			currentp = firstp;  // first record made current
								// loop through all groceries:
			while(currentp->next != NULL)
				currentp = currentp->next;
					// The last one is found  
			currentp = firstp;
			firstp = currentp->next;
			free(currentp);
		}
	}while (firstp != NULL);
}

/* At the end of the program, the queue will be cleared*/
void deallocateQueue(void)
{
	do{
		if(firstq==NULL){
			puts("There is no queue to deallocate");
			break;
		}
		else
		{
			currentq = firstq;  // first record made current
								// loop through all groceries:
			while(currentq->next != NULL)
				currentq = currentq->next;
					// The last one is found  
			currentq = firstq;
			firstq = currentq->next;
			free(currentq);
		}
	}while (firstq != NULL);
}
