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
		qhead = -1;
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
		printf("enqueing process\n");
		bgproc* nproc = malloc(sizeof(bgproc));
		nproc->pid = pid;
		nproc->pgid = pgid;
		nproc->command = command;
		nproc->state = running;
		nproc->visibility = foreground;
		//printf("nproc state: %i\n", nproc->state);
		qhead = (qhead+1)%PROC_MAX;
		queue[qhead] = nproc;
		printf("finished enqueing process\n");
		//printf("pid: %i pgid: %i com: %s state: %i vis: %i\n", nproc->pid, nproc->pgid, 
		//			nproc->command, nproc->state, nproc->visibility);
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
	if(!qisempty()){
		printf("in remqueue\n");
		printf("qtail is: %i\n", qtail);
		printf("qhead is: %i\n", qhead);
		int temptail = qtail;
		//if(queue[temptail] != NULL)
			//printf("first element in queue has pid: %i\n", queue[temptail]->pid);
		//else
			//printf("bg proc is null and shouldn't be\n");
		while((queue[temptail]->pid != pid) && (temptail != qhead)){
			//printf("in loop for %i time\n", temptail);
			temptail = (temptail+1)%PROC_MAX;
		}
		
		//printf("found struct or not there\n");
		if(queue[temptail]->pid == pid){
			dproc->pid = queue[temptail]->pid;
			dproc->pgid = queue[temptail]->pgid;
			dproc->command = queue[temptail]->command;
			dproc->state = queue[temptail]->state;
			dproc->visibility = queue[temptail]->visibility;
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		printf("Background queue is empty\n");
		return -1;
	}
	
	

}

//int findlaststopped(bgproc* dproc){
	//int tempHead = qhead;
	//while(tempHead != (qtail-1)%PROC_MAX){
		//if(queue[tempHead]->state == stopped){
			//dproc->pid = queue[tempHead]->pid;
			//dproc->pgid = queue[tempHead]->pgid;
			//dproc->command = queue[tempHead]->command;
			//return 1;
		//}
		//else
		//{
			//tempHead = (tempHead-1)%PROC_MAX;
		//}
		
	//}
	//return -1;
//}

//int qpeekhead(bgproc* dproc){
	//if(!qisempty()){
		//dproc->pid = queue[qhead]->pid;
		//dproc->pgid = queue[qhead]->pgid;
		//dproc->command = queue[qhead]->command;
		//dproc->state = queue[qhead]->state;
		//return 1;
	//}
	//else
	//{
		//return -1;
	//}
	
//}

//int qchangestate(pid_t pid, int i){
	//if(!qisempty()){
		//int tempHead = qhead;
		//while(tempHead != (qtail-1)%PROC_MAX){
			//if(queue[tempHead]->pid == pid){
				//queue[tempHead]->state = i;
				//return 1;
			//}
			//else
			//{
				//tempHead = (tempHead-1)%PROC_MAX;
			//}
			
		//}
		//return -1;
	//}
	//else
	//{
		//return -1;
	//}
//}

int qchangevis(pid_t pid, int i){
	if(!qisempty()){
		int tempHead = qhead;
		while(tempHead != (qtail-1)%PROC_MAX){
			if(queue[tempHead]->pid == pid){
				queue[tempHead]->visibility = i;
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

//int findqproc(pid_t pid, bgproc* dproc){
	//int tempHead = qhead;
	//while(tempHead != (qtail-1)%PROC_MAX){
		//if(queue[tempHead]->pid == pid){
			//dproc->pid = queue[tempHead]->pid;
			//dproc->pgid = queue[tempHead]->pgid;
			//dproc->command = queue[tempHead]->command;
			//dproc->state = queue[tempHead]->state;
			//dproc->visibility = queue[tempHead]->visibility;
			
			//return 1;
		//}
		//else
		//{
			//tempHead = (tempHead-1)%PROC_MAX;
		//}
		
	//}
	//return -1;
//}

int setFGProc(pid_t pid, pid_t pgid, char* com){
	fproc.pgid = pgid;
	fproc.pid = pid;
	fproc.command = com;
	return 1;
}

int getFGProc(fgproc* fg){
	fg->pid = fproc.pid;
	fg->pgid = fproc.pgid;
	fg->command = fproc.command;
}

int setBGProc(pid_t pid, pid_t pgid, char* com){
	qproc.pid = pid;
	qproc.pgid =pgid;
	qproc.state = running;
	qproc.command = com;
}

int getBGProc(bgproc* fg){
	if(qproc.pid > 0){
		printf("qproc is not null\n");
		fg->pid = qproc.pid;
		fg->pgid = qproc.pgid;
		fg->command = qproc.command;
		return 0;
	}
	else
	{
		printf("qproc is null\n");
		return -1;
	}

}

int qisempty(){
	if(((qhead+1)%PROC_MAX)==qtail)
		return 1;
	else
		return 0;
}

int qisfull(){
	if(((qhead+2)%PROC_MAX)==qtail)
		return 1;
	else
		return 0;
}

int msgbuffer_init(){
	msgbufferpos = 0;
	if((int)(bgmsgbuffer = calloc(MAX_BUFFER, sizeof(char*))) != -1)
		return 0;
	else
		return -1;
}

int insert_msgbuffer(char* msg){
	if(msgbufferpos < MAX_BUFFER){
		bgmsgbuffer[msgbufferpos++] = msg;
		return 1;
	}
	else
	{
		printf("Background message buffer is full\n");
		return -1;
	}
	
}

int msgbuffer_tostring(){
	if(msgbufferpos != 0){
		int i = 0;
		while(i <msgbufferpos){
			printf("%s\n",bgmsgbuffer[i++]);
		}
		msgbufferpos = 0;
		return 0;
	}
	else
	{
		return -1;
	}
	
}
