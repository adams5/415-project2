#ifndef __PROCESS_HASH_H__
#define __PROCESS_HASH_H__

#include <stdlib.h>
#include <signal.h>

typedef struct HProc{
	pid_t pid;
	pid_t pgid;
	char* command;
	struct HProc *next;
}HPROC;

int hash_init();

int free_hash();

int insertProc(pid_t pid, pid_t pgid, char* command);

int removeProc(pid_t pgid);

HPROC* searchProc(pid_t key);

char* toString();






#endif
