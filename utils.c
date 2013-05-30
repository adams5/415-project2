#include <stdlib.h>
#include "utils.h"

/**
 * Compare two strings and return status of equality
 * @param Two strings to compare, expect non-NULL values
 * @return Integer regarding equality, 1-Equal  -1-Not Equal
*/
int cmp(char *s1, char *s2)
{
    //offset starts at 0
    int j = 0;

    //while the starting point + offset is null a null char
    while(*(s1 +j) != '\0')
    {
        //if the two char arrays + offset are not the same then return 0 for false
        if(*(s1 +j) != *(s2 +j))
            return 0; //not the same
        //these chars were the same so increase the offset
        j++;
    }
    //the char arrays were the same, truen 1 for true
    return 1; // the same
}

/**
 * Initialize queue for background processes
 * @param None
 * @return Integer regarding status of memory allocation 1-Success -1-Failure
*/
int queue_init(){
	//Allocate memory for queue and initialize head and tail pointers to NULL
	if((long)(queue = calloc(PROC_MAX, sizeof(struct BGProc))) != -1){
		qhead = NULL;
		qtail = NULL;
		return 1;
	}
	//memory allocation failed
	else
	{
		printf("Error: Message Queue failed to initialize\n");
		return -1;
	}
}

/**
 * Free allocated memory for background process queue
 * @param None
 * @return Integer regarding success of memory reclaiming 1-Success
*/
int free_queue(){
	//struct BGProc dproc;				//BGProc stack object for use in...not sure
	while(!qisempty()){dequeue(NULL);}	//release memory for processes, if they still exist
	free(queue);						//release memory for queue
	return 1;	
}

/**
 * Insert a process in the background process queue, insert at head
 * @param Process PID, PGID, and command string--expects non-null values
 * @return Integer regarding success of enqueue; memory allocation and 
 * 				insertion  1-Success  -1-Failure
*/
int enqueue(pid_t pid, pid_t pgid, char* command){
	struct BGProc* nproc;					//pointer to new process
	
	//Allocate memory and set values for new process struct
	if((long)(nproc = malloc(sizeof(struct BGProc))) != -1){
		nproc->pid = pid;
		nproc->pgid = pgid;
		nproc->state = running;
		nproc->visibility = background;
		nproc->next = NULL;
		nproc->prev = NULL;
		
		//deep copy of command string
		int i = 0;
		while(command[i] != '\0'){
			nproc->command[i] = command[i];
			i++;
		}
		nproc->command[i]='\0';				//explicit null terminate string

		//If queue is empty, inserting first process and set pointers
		//accordingly
		if(qisempty()){
			qtail = qhead = nproc;
			nproc->prev = NULL;
			nproc->next = NULL;
		}
		//Queue is not empty, set insert and set pointers accordingly
		else
		{
			nproc->next = qhead;
			qhead->prev = nproc;
		}
		qhead = nproc;					//Inserted process is always the head
		
		return 1;
	}
	//Memory allocation failed
	else{
		printf("Memory alocation failed\n");
		return -1;
	}
}
 
 /**
  * Remove a process from the background process queue, remove from tail
  * @param Reference to BGProc object to set values in
  * @return Integer regarding success; 1-Success -1-Failure, queue is empty
 */
int dequeue(struct BGProc* dproc){
	//if queue is not empty, set values to param and remove tail
	if(!qisempty()){
		//copy values to param
		dproc->pid = qtail->pid;
		dproc->pgid = qtail->pgid;
		dproc->state = qtail->state;
		dproc->visibility = qtail->visibility;
		
		//deep copy of command string
		int i = 0;
		while(qtail->command[i] != '\0'){
			dproc->command[i] = qtail->command[i];
			i++;
		}
		dproc->command[i] = '\0';					//explicit null terminate
		
		//change pointers for tail and free previous tail memory allocation
		//and return success
		struct BGProc* tempTail = qtail;
		qtail = qtail->prev;
		free(tempTail);
		return 1;
	}
	//Queue is empty
	else
	{
		printf("Message Queue is empty. No messages waiting\n");
		return -1;
	}
		
}

/**
 * Search for process and remove, possibly from middle of queue
 * @param Process ID to search for, Reference to BGProc struct
 * @return Integer regarding if queue is non-empty, Process is found, 
 * or Process is not found. 1-Found, -1-Empty/Proc does not exist
*/
int remqueue(pid_t pid, struct BGProc* dproc){
	//If queue is not empty, seach for pid, else return -1
	if(!qisempty()){
		//initialize pointers for iterating through queue
		struct BGProc* temptail = qtail;
		struct BGProc* tempProc = temptail;
		
		//search for process ID
		while(temptail != NULL && temptail->pid != pid){
			tempProc = temptail;
			temptail = temptail->prev;
		}
		
		//found pid or reached end of queue. if pid found copy to param
		if(temptail != NULL && temptail->pid == pid){
			dproc->pid = temptail->pid;
			dproc->pgid = temptail->pgid;
			dproc->state = temptail->state;
			dproc->visibility = temptail->visibility;
			tempProc->prev = temptail->prev;
			
			//deep copy command string
			int i = 0;
			while(qtail->command[i] != '\0'){
				dproc->command[i] = temptail->command[i];
				i++;
			}
			dproc->command[i] = '\0';			//explicit null terminate string
			
			//if process is not head of queue, change pointers around process
			//to remove
			if(tempProc->prev != NULL)
				tempProc->prev->next = temptail->next;
				
			//free memory of process to remove
			free(temptail);							
			return 1;
		}
		//pid not found
		else
		{
			printf("child not in queue\n");
			return -1;
		}
	}
	//Queue is empty
	else 											
	{
		printf("Background queue is empty\n");
		return -1;
	}
	
	

}

/**
 * Finds the last stopped backgrounded process
 * @param Reference to BGProc struct to copy process data to
 * @return Integer regarding success of find; 1-Success  
 * 			-1-No stopped processes or queue is empty
*/
int findlaststopped(struct BGProc* dproc){
	//if queue is not empty, search for last stopped process
	if(!qisempty()){
		struct BGProc* tempHead = qhead;		//BGProc pointer used for iterating queue
		
		//search for last stopped process
		while(tempHead != NULL){
			if(tempHead->state == stopped){
				//copy process data to param
				dproc->pid = tempHead->pid;
				dproc->pgid = tempHead->pgid;

				//deep copy command string
				int i = 0;
				while(tempHead->command[i] != '\0'){
					dproc->command[i] = tempHead->command[i];
					i++;
				}
				dproc->command[i] = '\0';			//explicit null terminate string

				return 1;
			}
			else
			{
				tempHead = tempHead->next;
			}
		}
		//no stopped processes
		return -1;
	}
	//queue is empty
	else
	{
		return -1;
	}
}


/**
 * Remove process at the head of the background process queue 
 * @param Reference to BGProc struct to copy data to
 * @return Integer for success or failure of copy 1-Success  -1-Failure
*/
int removehead(struct BGProc* dproc){
	//if queue is not empty, copy head process struct
	if(!qisempty()){
		dproc->pid = qhead->pid;
		dproc->pgid = qhead->pgid;
		dproc->state = qhead->state;
		dproc->visibility = qhead->visibility;

		//deep copy command string
		int i = 0;
		while(qhead->command[i] != '\0'){
			dproc->command[i] = qhead->command[i];
			i++;
		}
		dproc->command[i] = '\0';			//explicit null terminate string
		
		//remove head and release memory allocation
		struct BGProc* temphead = qhead;
		if(qhead->next != NULL)
			qhead->next->prev = qhead->prev;
		qhead = qhead->next;
		free(temphead);

		return 1;
	}
	//Queue is empty
	else
	{
		return -1;
	}
}


/**
 * Change the state of a process in queue, either running or stopped
 * @param Process ID of process to change, Integer value for State
 * 			enum of background process struct; 0-stopped 1-running
 * @return Integer regarding success of change; 1-Success  -1-Failure
*/
int qchangestate(pid_t pid, int state){
	//if queue is not empty, search for process and change state
	if(!qisempty()){
		struct BGProc* tempHead = qhead;		//BGProc pointer to use in iterating
		
		//Search for process in queue and change state if found
		while(tempHead != NULL){
			if(tempHead->pid == pid){
				tempHead->state = state;
				return 1;
			}
			else
			{
				tempHead = tempHead->next;
			}
			
		}
		//Process not found
		return -1;
	}
	//Queue is empty
	else
	{
		return -1;
	}
}

/**
 * Change the visibility of a process in queue, either background or foreground
 * @param Process ID of process to change, Integer value for Visibility
 * 			enum of background process struct; 0-Background 1-Foreground
 * @return Integer regarding success of change; 1-Success  -1-Failure
*/
int qchangevis(pid_t pid, int vis){
	//If queue is not empty, search for process and change visibility
	if(!qisempty()){
		struct BGProc* tempHead = qhead;	//BGProc pointer for iterating in queue
		
		//find process and change vis
		while(tempHead != NULL){
			if(tempHead->pid == pid){
				tempHead->visibility = vis;
				return 1;
			}
			else
			{
				tempHead = tempHead->next;
			}
		}
		//process not found
		return -1;
	}
	//Queue is empty
	else
	{
		return -1;
	}
}


/**
 * Search for a process in the queue and set param to process if found
 * @param Process ID to search for, Reference to BGProc struct to set
 * 			to found process
 * @return Integer regarding status of find; 1-Process Found  
 * 			-1-Process does not exist or queue is empty
*/
int findqproc(pid_t pid, bgproc* dproc){
	//if queue is not empty, search for process
	if(!qisempty()){
		struct BGProc* tempHead = qhead;		//BGProc pointer to use in iteration
		
		//while iterator is not null and process not found, search
		while(tempHead != NULL){
			if(tempHead->pid == pid){
				*dproc = *tempHead;
				return 1;
			}
			else
			{
				tempHead = tempHead->next;
			}
		}
		//process does not exist
		return -1;
	}
	//Queue is empty
	else
	{
		return -1;
	}
}


/**
 * Output if background process queue is empty
 * @param None
 * @return Integer regarding status of queue 1-Empty  0-Not Empty
*/
int qisempty(){
	if(qhead==NULL)			//if head is null, queue is empty
		return 1;
	else
		return 0;
}

/**
 * Initialize a buffer to hold process output
 * while running in the background. 
 * @param None
 * @return integer value regarding success 1-success -1-failure
*/
int msgbuffer_init(){
	msgbufferpos = 0;				//initialize buffer index pointer to 0
	if((long)(bgmsgbuffer = calloc(MAX_BUFFER, sizeof(char*))) != -1)
		return 1;					//if memory allocation succeeded, return 1
	else
		return -1;
}

/**
 * Insert message into buffer of background process output
 * @param String regarding status of background process, should be non-null
 * @return Integer regarding success of inserting n_bytes-Success  -1-Failure
*/
int insert_msgbuffer(char* msg){
	if(sizeof msg > 0){			//
		int i=0;
		char* com = malloc(sizeof msg+1);
		while(msg[i] != '\0'){				//insert param into buffer
			com[i] = msg[i];				//deep copy param string
			i++;
		}
		com[i] = '\0';
		bgmsgbuffer[msgbufferpos++] = com;
		printf("string buffered is: %s\n", bgmsgbuffer[msgbufferpos-1]);
		return sizeof com;
	}
	else
	{
		printf("Background message buffer is full\n");
		return -1;
	}
	
}

/**
 * Output buffered messages from background processes
 * @param NULL
 * @return Integer regarding success of outputting buffer
*/
int msgbuffer_tostring(){
	if(msgbufferpos != 0){
		printf("Queued messages from backgrounded processes\n");
		int i = 0;
		while(i <msgbufferpos){
			printf("%s\n",bgmsgbuffer[i]);
			free(bgmsgbuffer[i]);				//free allocated memory for status'
			i++;
		}
		msgbufferpos = 0;
		return 0;
	}
	else
	{
		return -1;
	}
	
}

//Functions to set and get the foreground procedure
/**
 * Sets a FGProc struct referencing the foreground process
 * @param Process PID, PGID, and command string
 * @return Integer regarding success; 1-Success  -1-Failure
*/
int setFGProc(pid_t pid, pid_t pgid, char* com){
	if(pid > 0){
		fproc.pgid = pgid;
		fproc.pid = pid;

		//deep copy command string
		int i = 0;
		while(com[i] != '\0'){
			fproc.command[i] = com[i];
			i++;
		}
		fproc.command[i] = '\0';			//explicit null terminate string
		
		return 1;
	}
	else
	{
		return -1;
	}
}

/**
 * Get the foreground process data
 * @param Reference to FGProc struct to store data
 * @return Integer regarding status of data copy
*/
int getFGProc(struct FGProc* fg){
	//if fgproc is not null, copy data to param
	if(fg != NULL){
		//copy fproc values
		fg->pid = fproc.pid;
		fg->pgid = fproc.pgid;

		//deep copy command string
		int i = 0;
		while(fproc.command[i] != '\0'){
			fg->command[i] = fproc.command[i];
			i++;
		}
		fg->command[i] = '\0';			//explicit null terminate string
		
		return 1;
	}
	//fproc has not been set
	else
	{
		return -1;
	}
	
}

//No longer used since queue changed to list
//int qisfull(){
	//if(((qhead+2)%PROC_MAX)==qtail)
		//return 1;
	//else
		//return 0;
//}



/*
 * Functions for setting and getting the most recent background
 * and foreground process. --Deprecated after implementation of queue
*/
//int setBGProc(pid_t pid, pid_t pgid, char* com){
	//qproc.pid = pid;
	//qproc.pgid =pgid;
	//qproc.state = running;
	//int i = 0;
	//while(qtail->command[i] != '\0'){
		//qproc.command[i] = com[i];
		//i++;
	//}
//}

//int getBGProc(bgproc* fg){
	//if(qproc.pid > 0){
		////printf("qproc is not null\n");
		//fg->pid = qproc.pid;
		//fg->pgid = qproc.pgid;
		//int i = 0;
		//while(qtail->command[i] != '\0'){
			//fg->command[i] = qproc.command[i];
			//i++;
		//}
		//return 0;
	//}
	//else
	//{
		//printf("qproc is null\n");
		//return -1;
	//}

//}

//--End Deprecated Fuctions
