#include "jobs.h"
#include "global.h"
//#include "process_hash.h"
#include <stdio.h>
#include <unistd.h>

void bg(){	
	
	//add checking to make sure there's a background job
	if(lastStoppedBG.pgid == 0)
		printf("Error, no job stopped in the background\n");
	//send SIGCONT to the most recently stopped background job, this will resume execution
	else{
		if(killpg(lastStoppedBG.pgid,SIGCONT) == -1)
			perror("killpg() error");
		else
				setLastBG(lastStoppedBG.pid);		
	}
}


void fg(){
	
	//add checking to make sure there's a last stopped job
	if(lastBG.pgid == 0){
		printf("Error, no job in the background\n");
	}
	else{
		printf("fg lastBG.pid: %ld lastBG.pgid: %ld\n",(long) lastBG.pid,(long) lastBG.pgid);
		printf("%s\n",searchProc(lastBG.pgid)->command);
		
		//bring the most recetly backgrounded job to the foreground
		bringLastBGtoFG();

		//send a SIGCONT in case the job is stopped
		if(killpg(lastBG.pgid,SIGCONT) == -1)
			perror("killpg() error");
	}
}

void setLastBG(pid_t pid){
	printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	lastBG.pid = pid;
	lastBG.pgid = getpgid(pid);
}

void setLastStoppedBG(pid_t pid){
	printf("lastStoppedBG lastStoppedBG.pid: %ld lastStoppedBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	lastStoppedBG.pid = pid;
	lastStoppedBG.pgid = getpgid(pid);
}

void bringLastBGtoFG(){
	tcsetpgrp(0, lastBG.pgid);
	currentfg.pgid = lastBG.pgid;
	currentfg.pid = lastBG.pid;
}

//switch the terminal control back to the shell
void sendShellToFG(){
	printf("call to send shell to fg\n");
	//printf("%s> ", shname);
	if(tcsetpgrp(0, shellPID) != -1){
		currentfg.pid = shellPID;
		currentfg.pgid = getpgid(shellPID);
	}
	else
		perror("setpgid() error");
}

void sendToFG(pid_t pid){
	//printf("Setting %ld to tc\n",(long) pid);
	if(tcsetpgrp(0, pid) != -1){
		currentfg.pid = pid;
		currentfg.pgid = getpgid(pid);
	}
	else
		perror("setpgid() error");
	
	//printf("\ntcgetpgrp: %ld\n",(long) tcgetpgrp(0));
}

void sendToBG(pid_t pid){
	setLastBG(pid);
	sendShellToFG();
	printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
}
