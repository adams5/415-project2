#include "process_hash.h"

#define HASH_SIZE 10

HProc** hp;

int hash_init(){
	if((hp = calloc(HASH_SIZE, sizeof HProc)) != NULL)
		return 1;
	else
		return -1;
}

int free_hash(){
	
	if(hp != NULL){
		HProc* tempProc = hp;
	
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
	if((HProc *new = malloc(sizeof *HProc)) != NULL){
		HProc *tempProc = hp[pgid%HASH_SIZE];
		new->next = tempProc;
		hp[pgid%HASH_SIZE] = new;
		return 1;
	}
	else return -1
}

int removeProc(pid_t pid){
	int i = (int)pid%HASH_SIZE;
	HProc *parent = hp[i];
	HProc *tempProc = hp[i];
	
	
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
	HProc* tempProc = hp;	
	int i = key%HASH_SIZE;
	
	while(tempProc[i] != NULL){
		if(tempProc[i]->pid == key)
			return tempProc[i]->command;
		else
			tempProc[i] = tempProc[i]->next;
		
	}
	return NULL;
}
