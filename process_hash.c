#include "process_hash.h"
#include <unistd.h>
#include <stdio.h>

#define HASH_SIZE 10

struct HProc** hp;

int hash_init(){
	if((hp = calloc(HASH_SIZE, sizeof (struct HProc))) != NULL){
		int i = 0;
		while(i < HASH_SIZE){
			hp[i] = NULL;
			i++;
		}
		return 1;
	}
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

int insertProc(pid_t pid, pid_t pgid, char* command){
	//printf("pgid is: %i and command is: %s\n", pgid, command);
	
	HPROC *new;
	if((new = malloc(sizeof (struct HProc))) != NULL){
		HPROC *tempProc = hp[pgid%HASH_SIZE];
		new->pid = pid;
		new->pgid = pgid;
		new->command = command;
		new->next = tempProc;
		hp[pgid%HASH_SIZE] = new;
		/*if(new->next == NULL)
			printf("new proc pgid is: %i next proc pgid is: NULL stored in index: %i\n", new->pgid,
						pgid%HASH_SIZE);
		else
			printf("new proc pgid is: %i next proc pgid is: %i stored in index: %i\n", new->pgid,
						new->next->pgid, pgid%HASH_SIZE);
						
		printf("head of hash table at index: %i is command: %s\n", pgid%HASH_SIZE, hp[pgid%HASH_SIZE]->command);
		*/
		return 1;
	}
	else return -1;
}

int removeProc(pid_t pgid){
	int i = (int)pgid%HASH_SIZE;
	HPROC *parent = hp[i];
	HPROC *tempProc = hp[i];
	
	
	while(tempProc != NULL){
		if(tempProc->pgid == pgid){
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

HPROC* searchProc(pid_t key){
	int i = key%HASH_SIZE;
	HPROC* tempProc = hp[i];	
	
	while(tempProc != NULL){
		if(tempProc->pid == key)
			return tempProc;
		else
			tempProc = tempProc->next;
		
	}
	return NULL;
}

char* toString(){
	int i = 0;
	
	while(hp[i] != NULL){
		HPROC* tempProc = hp[i];
		
		while(tempProc != NULL){
			tempProc = tempProc->next;
			
		}
		i++;
	}
	return NULL; 
}
