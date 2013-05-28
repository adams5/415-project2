#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdio.h>
#define PROC_MAX 256

typedef struct BGProc{
	pid_t pid;
	pid_t pgid;
	char* command;
	typedef int state = {"stopped" =0, "running"};
	struct BGProc* next;
}bgproc;

typedef struct FGProc{
	pid_t pid;
	pid_t pgid;
	char* command;
	struct FGProc* next;
}fgproc;

bgproc** queue;
int qhead;
int qtail;
bgproc* qproc;

int cmp(char *s1, char *s2);

//Operations for message queue
int queue_init();

int free_queue();

int enqueue(pid_t pid, pid_t pgid, char* com);
 
int dequeue(bgproc* dproc);

void removeallmsg();

int remqueue(pid_t pid, bgproc* dproc);

int qisempty();

int qisfull();

#endif
