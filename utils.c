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
	if((long)(queue = calloc(PROC_MAX, sizeof(struct BGProc))) != -1){
		qhead = NULL;
		qtail = NULL;
		return 1;
	}
	else
	{
		printf("Error: Message Queue failed to initialize\n");
		return -1;
	}
}

int free_queue(){
	struct BGProc dproc;
	while(!qisempty()){dequeue(&dproc);}
	free(queue);		///not sure what to do here
	return 1;	
}

int enqueue(pid_t pid, pid_t pgid, char* command){
	struct BGProc* nproc;
	if((nproc = malloc(sizeof(struct BGProc))) != -1){
		//printf("enqueing process\n");
		nproc->pid = pid;
		nproc->pgid = pgid;
		nproc->state = running;
		nproc->visibility = foreground;
		nproc->next = NULL;
		nproc->prev = NULL;
		int i = 0;
		while(command[i] != '\0'){
			nproc->command[i] = command[i];
			i++;
		}
		nproc->command[i]='\0';
		printf("new proc to enqueue has command of: %s and pid of: %i\n", nproc->command, nproc->pid);
		//printf("nproc state: %i\n", nproc->state);
		if(qisempty()){
			qtail = qhead = nproc;
			nproc->prev = NULL;
			nproc->next = NULL;
		}
		else
		{
			nproc->next = qhead;
			qhead->prev = nproc;
		}
		qhead = nproc;
		return 1;
	}
	else{
		return 0;
	}
}
 
int dequeue(struct BGProc* dproc){
	if(!qisempty()){
		dproc->pid = qtail->pid;
		dproc->pgid = qtail->pgid;
		dproc->state = qtail->state;
		dproc->visibility = qtail->visibility;
		int i = 0;
		while(qtail->command[i] != '\0'){
			dproc->command[i] = qtail->command[i];
			i++;
		}
		
		struct BGProc* tempTail = qtail;
		qtail = qtail->prev;
		free(tempTail);
		return 1;
	}
	else
	{
		printf("Message Queue is empty. No messages waiting\n");
		return -1;
	}
		
}

int remqueue(pid_t pid, struct BGProc* dproc){
	if(!qisempty()){
		//printf("in remqueue\n");
		//printf("qtail is: %i\n", qtail);
		//printf("qhead is: %i\n", qhead);
		struct BGProc* temptail = qtail;
		struct BGProc* tempProc = temptail;
		
	//if(queue[temptail] != NULL)
			//printf("first element in queue has pid: %i\n", queue[temptail]->pid);
		//else
			//printf("bg proc is null and shouldn't be\n");
		while(temptail != NULL && temptail->pid != pid){
			printf("current pid is: %i\n", temptail->pid);
			tempProc = temptail;
			temptail = temptail->prev;
			printf("temptail com: %s tempProc com: %s\n", temptail->command, tempProc->command);
		}
		
		//printf("found struct or not there\n");
		if(temptail != NULL && temptail->pid == pid){
			dproc->pid = temptail->pid;
			dproc->pgid = temptail->pgid;
			dproc->state = temptail->state;
			dproc->visibility = temptail->visibility;
			tempProc->prev = temptail->prev;
			
			int i = 0;
			while(qtail->command[i] != '\0'){
				dproc->command[i] = temptail->command[i];
				i++;
			}
			
			if(tempProc->prev != NULL)
				tempProc->prev->next = temptail->next;
			free(temptail);
			return 1;
		}
		else
		{
			printf("child not in queue\n");
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
		struct BGProc* tempHead = qhead;
		while(tempHead != NULL){
			if(tempHead->pid == pid){
				tempHead->visibility = i;
				return 1;
			}
			else
			{
				tempHead = tempHead->next;
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
	int i = 0;
	while(qtail->command[i] != '\0'){
		qproc.command[i] = com[i];
		i++;
	}
}

int getBGProc(bgproc* fg){
	if(qproc.pid > 0){
		//printf("qproc is not null\n");
		fg->pid = qproc.pid;
		fg->pgid = qproc.pgid;
		int i = 0;
		while(qtail->command[i] != '\0'){
			fg->command[i] = qproc.command[i];
			i++;
		}
		return 0;
	}
	else
	{
		printf("qproc is null\n");
		return -1;
	}

}

int qisempty(){
	if(qhead==NULL)
		return 1;
	else
		return 0;
}

//int qisfull(){
	//if(((qhead+2)%PROC_MAX)==qtail)
		//return 1;
	//else
		//return 0;
//}

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
		printf("string buffered is: %s\n", bgmsgbuffer[msgbufferpos-1]);
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
		printf("Queued messages from backgrounded processes\n");
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
