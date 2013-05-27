#include "jobs.h"
#include "global.h"
#include "process_hash.h"
#include <stdio.h>
#include <unistd.h>

void bg(){	
	
	//add checking to make sure there's a background job
	
	//send SIGCONT to the most recently stopped background job, this will resume execution
	killpg(lastStoppedBG.pgid,SIGCONT);
}


void fg(){
	
	//add checking to make sure there's a last stopped job
	
	printf("%s\n",searchProc(lastBG.pgid));
	
	//bring the most recetly backgrounded job to the foreground
	bringLastBGtoFG();

	//send a SIGCONT in case the job is stopped
	killpg(lastBG.pgid,SIGCONT);
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
	//tcsetpgrp(0, lastStoppedJob);
	//tcsetpgrp(1, lastStoppedJob);	
	//tcsetpgrp(2, lastStoppedJob);
}

//switch the terminal control back to the shell
void sendShellToFG(){
	//tcsetpgrp(0, shellPID);
	//tcsetpgrp(1, shellPID);	
	//tcsetpgrp(2, shellPID);
}

void sendToBG(pid_t pid){
	setLastBG(pid);
	sendShellToFG();
}
