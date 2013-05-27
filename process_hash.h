#ifndef __PROCESS_HASH_H__
#define __PROCESS_HASH_H__

#include <signal.h>

struct HProc{
	pid = 0;
	pgid = 0;
	char* command;
	HProc next = NULL;
};

int init_hash();

int free_hash();

int insertProc(pid_t pgid, char* command);

int removeProc(pid_t pid);

char* searchProc(pid_t key, void*);








#endif
