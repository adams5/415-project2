#include "jobs.h"
#include "global.h"
#include "process_hash.h"
#include <stdio.h>
#include <unistd.h>

void bg(){	
	
	//add checking to make sure there's a background job
	
	//send SIGCONT to the most recently stopped background job, this will resume execution
	if(killpg(lastStoppedBG.pgid,SIGCONT) == -1)
		perror("killpg() error");
		
}


void fg(){
	
	//add checking to make sure there's a last stopped job
	
	//printf("%s\n",searchProc(lastBG.pgid));
	
	//bring the most recetly backgrounded job to the foreground
	//bringLastBGtoFG();

	//send a SIGCONT in case the job is stopped
	if(killpg(lastBG.pgid,SIGCONT) == -1)
		perror("killpg() error");
}

void setLastBG(pid_t pid){
	lastBG.pid = pid;
	lastBG.pgid = getpgid(pid);
}

void setLastStoppedBG(pid_t pid){
	lastStoppedBG.pid = pid;
	lastStoppedBG.pgid = getpgid(pid);
}

void bringLastBGtoFG(){
	tcsetpgrp(0, lastStoppedBG.pgid);
		
}

//switch the terminal control back to the shell
void sendShellToFG(){
	if(tcsetpgrp(0, shellPID) == -1)
		perror("setpgid() error");
}

void sendToFG(pid_t pid){
	//printf("Setting %ld to tc\n",(long) pid);
	if(tcsetpgrp(0, pid) == -1)
		perror("setpgid() error");
	
	//printf("\ntcgetpgrp: %ld\n",(long) tcgetpgrp(0));
}

void sendToBG(pid_t pid){
	setLastBG(pid);
	sendShellToFG();
}
