#ifndef __UTILS_H__
#define __UTILS_H__

#include <signal.h>


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


int cmp(char *s1, char *s2);

#endif
