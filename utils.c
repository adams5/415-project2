#include <stdlib.h>
#include "utils.h"

//method to compare to char arrays
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

int queue_init(){
	if((long)(queue = calloc(PROC_MAX, sizeof(qproc))) != -1){
		qhead = 0;
		qtail = 0;
		return 1;
	}
	else
	{
		printf("Error: Message Queue failed to initialize\n");
		return -1;
	}
}

int free_queue(){
	bgproc dproc;
	while(!qisempty()){dequeue(&dproc);}
	free(queue);		///not sure what to do here
	return 1;	
}

int enqueue(pid_t pid, pid_t pgid, char* command){
	if(!qisfull()){
		bgproc* nproc = malloc(sizeof(bgproc));
		nproc->pid = pid;
		nproc->pgid = pgid;
		nproc->command = command;
		qhead = (qhead+1)%PROC_MAX;
		queue[qhead] = nproc;
		return 1;
	}
	else{
		printf("Queue is full, cannot insert background process: %s\n", command);
		return 0;
	}
}
 
int dequeue(bgproc* dproc){
	if(!qisempty()){
		dproc->pid = queue[qtail]->pid;
		dproc->pgid = queue[qtail]->pgid;
		dproc->command = queue[qtail]->command;
		free(queue[qtail]);
		qtail = (qtail+1)%PROC_MAX;
		return 1;
	}
	else
	{
		printf("Message Queue is empty. No messages waiting\n");
		return -1;
	}
		
}

int remqueue(pid_t pid, bgproc* dproc){
	int temptail = qtail;
	while(queue[temptail]->pid != pid && temptail != qhead){
		temptail = (temptail+1)%PROC_MAX;
	}
	
	if(queue[temptail]->pid == pid){
		dproc->pid = queue[temptail]->pid;
		dproc->pgid = queue[temptail]->pgid;
		dproc->command = queue[temptail]->command;
		return 1;
	}
	else
	{
		return -1;
	}
	

}

int findlaststopped(bgproc* dproc){
	int tempHead = qhead;
	while(tempHead != (qtail-1)%PROC_MAX){
		if(queue[tempHead]->state == Stopped){
			dproc->pid = queue[tempHead]->pid;
			dproc->pgid = queue[tempHead]->pgid;
			dproc->command = queue[tempHead]->command;
			return 1;
		}
		else
		{
			tempHead = (tempHead-1)%PROC_MAX;
		}
		
	}
	return -1;
}

int qpeekhead(bgproc* dproc){
	if(!qisempty()){
		dproc->pid = queue[qhead]->pid;
		dproc->pgid = queue[qhead]->pgid;
		dproc->command = queue[qhead]->command;
		dproc->state = queue[qhead]->state;
		return 1;
	}
	else
	{
		return -1;
	}
	
}

int qchangestate(pid_t pid, int i){
	if(!qisempty()){
		int tempHead = qhead;
		while(tempHead != (qtail-1)%PROC_MAX){
			if(queue[tempHead]->pid == pid){
				queue[tempHead]->state = i;
				return 1;
			}
			else
			{
				tempHead = (tempHead-1)%PROC_MAX;
			}
			
		}
		return -1;
	}
	else
	{
		return -1;
	}
}

int qisempty(){
	if(qhead==qtail)
		return 1;
	else
		return 0;
}

int qisfull(){
	if(qhead==(qtail-1))
		return 1;
	else
		return 0;
}
