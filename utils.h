#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>
#include <stdio.h>
#define MSG_MAX 256

typedef struct BGProc{
	pid_t pid;
	pid_t pgid;
	struct BGProc* next;
}bgproc;

typedef struct FGProc{
	pid_t pid;
	pid_t pgid;
	struct FGProc* next;
}fgproc;

char** msgqueue;
int mhead;
int mtail;
bgproc* bgqueue;

int cmp(char *s1, char *s2);

//Operations for message queue
int msgqueue_init();

int free_msgqueue();

int insertmsg(char* msg);
 
int removemsg(char** msg);

void removeallmsg();

int msgisempty();

int msgisfull();

#endif
