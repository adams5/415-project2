#include "process_hash.h"

#define HASH_SIZE 10

HPROC** hp;

int hash_init(){
	if((hp = calloc(HASH_SIZE, sizeof *HPROC)) != NULL)
		return 1;
	else
		return -1;
}

int free_hash(){
	
	if(hp != NULL){
		HPROC* tempProc;
	
		int i = 0;
		while(i<HASH_SIZE){
			while(hp[i] != NULL){
				tempProc = hp[i];
				hp[i] = hp[i]->next;
				free(tempProc);
			}
			i++;
		}
		free(hp);
		return 1;
	}
	else
		return -1;
}

int insertProc(pid_t pgid, char* command){
	if((HPROC *new = malloc(sizeof *HPROC)) != NULL){
		HPROC *tempProc = hp[pgid%HASH_SIZE];
		new->next = tempProc;
		hp[pgid%HASH_SIZE] = new;
		return 1;
	}
	else return -1
}

int removeProc(pid_t pgid){
	int i = (int)pid%HASH_SIZE;
	HPROC *parent = hp[i];
	HPROC *tempProc = hp[i];
	
	
	while(tempProc != NULL){
		if(tempProc->pid == pid){
			parent->next = tempProc->next;
			free(tempProc);
			return 1;
		}
		else{
			parent = tempProc;
			tempProc = tempProc->next;
		}
	}
	return -1;
}

char* searchProc(pid_t key){
	HPROC* tempProc = hp;	
	int i = key%HASH_SIZE;
	
	while(tempProc[i] != NULL){
		if(tempProc[i]->pid == key)
			return tempProc[i]->command;
		else
			tempProc[i] = tempProc[i]->next;
		
	}
	return NULL;
}
