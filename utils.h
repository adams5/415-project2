#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdio.h>
#define PROC_MAX 256
#define MAX_BUFFER 1024

typedef enum {stopped=0, running}STATE;
typedef enum{background, foreground}VIS;

/**
 * Struct for Background Processes
*/
typedef struct BGProc{
	pid_t pid;
	pid_t pgid;
	char command[MAX_BUFFER];
	STATE state;
	VIS visibility;
	struct BGProc* next;
	struct BGProc* prev;
}bgproc;


/**
 * Stuct for Foreground Process
*/
typedef struct FGProc{
	pid_t pid;
	pid_t pgid;
	char command[MAX_BUFFER];
	struct FGProc* next;
}fgproc;

//Background and Foreground Process Struct Instances
bgproc qproc;
fgproc fproc;

/**
 * Compare two strings and return status of equality
 * @param Two strings to compare, expect non-NULL values
 * @return Integer regarding equality, 1-Equal  -1-Not Equal
*/
int cmp(char *s1, char *s2);


//Background Process queue
struct BGProc** queue;
struct BGProc* qhead;
struct BGProc* qtail;
/**
 * Initialize queue for background processes
 * @param None
 * @return Integer regarding status of memory allocation 1-Success -1-Failure
*/
int queue_init();


/**
 * Free allocated memory for background process queue
 * @param None
 * @return Integer regarding success of memory reclaiming 1-Success
*/
int free_queue();


/**
 * Insert a process in the background process queue, insert at head
 * @param Process PID, PGID, and command string--expects non-null values
 * @return Integer regarding success of enqueue; memory allocation and 
 * 			insertion  1-Success  -1-Failure
*/
int enqueue(pid_t pid, pid_t pgid, char* com);
 

 /**
  * Remove a process from the background process queue, remove from tail
  * @param Reference to BGProc object to set values in
  * @return Integer regarding success; 1-Success -1-Failure, queue is empty
 */
int dequeue(struct BGProc* dproc);


/**
 * Search for process and remove, possibly from middle of queue
 * @param Process ID to search for, Reference to BGProc struct
 * @return Integer regarding if queue is non-empty, Process is found, 
 * or Process is not found. 1-Found, -1-Empty/Proc does not exist
*/
int remqueue(pid_t pid, struct BGProc* dproc);


/**
 * Change the state of a process in queue, either running or stopped
 * @param Process ID of process to change, Integer value for State
 * 			enum of background process struct; 0-stopped 1-running
 * @return Integer regarding success of change; 1-Success  -1-Failure
*/
int qchangestate(pid_t pid, int i);


/**
 * Change the visibility of a process in queue, either background or foreground
 * @param Process ID of process to change, Integer value for Visibility
 * 			enum of background process struct; 0-Background 1-Foreground
 * @return Integer regarding success of change; 1-Success  -1-Failure
*/
int qchangevis(pid_t pid, int i);


/**
 * Search for a process in the queue and set param to process if found
 * @param Process ID to search for, Reference to BGProc struct to set
 * 			to found process
 * @return Integer regarding status of find; 1-Process Found  
 * 			-1-Process does not exist or queue is empty
*/
int findqproc(pid_t pid, struct BGProc* dproc);


/**
 * Remove process at the head of the background process queue 
 * @param Reference to BGProc struct to copy data to
 * @return Integer for success or failure of copy 1-Success  -1-Failure
*/
int removehead(struct BGProc* dproc);


/**
 * Output if background process queue is empty
 * @param None
 * @return Integer regarding status of queue 1-Empty  0-Not Empty
*/
int qisempty();


/**
 * Finds the last stopped backgrounded process
 * @param Reference to BGProc struct to copy process data to
 * @return Integer regarding success of find; 1-Success  
 * 			-1-No stopped processes or queue is empty
*/
int findlaststopped(struct BGProc* dproc);




//Background Process Message Buffer
char** bgmsgbuffer;
int msgbufferpos;
/**
 * Initialize a buffer to hold process output
 * while running in the background. 
 * @param None
 * @return integer value regarding success 1-success -1-failure
*/
int msgbuffer_init();


/**
 * Insert message into buffer of background process output
 * @param String regarding status of background process, should be non-null
 * @return Integer regarding success of inserting n_bytes-Success  -1-Failure
*/
int insert_msgbuffer(char* msg);


/**
 * Output buffered messages from background processes
 * @param NULL
 * @return Integer regarding success of outputting buffer
*/
int msgbuffer_tostring();


//Functions to get and set foreground process
/**
 * Sets a FGProc struct referencing the foreground process
 * @param Process PID, PGID, and command string
 * @return Integer regarding success; 1-Success  -1-Failure
*/
int setFGProc(pid_t pid, pid_t pgid, char* com);
 

/**
 * Get the foreground process data
 * @param Reference to FGProc struct to store data
 * @return Integer regarding status of data copy
*/
int getFGProc(fgproc* fg);




//No longer used since queue is list
//int qisfull();

/*
 * Deprecated functions for saving last foreground and background processes
 * int setBGProc(pid_t pid, pid_t pgid, char* com);
 * 
 * int getBGProc(bgproc* fg);
*/


#endif
