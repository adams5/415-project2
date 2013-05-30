#include "jobs.h"
#include "global.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * check and try to send SIGCONT to the last stopped BG job
*/
void bg(){	
	struct BGProc lsproc;
	//find the last stopped process
	if(findlaststopped(&lsproc) == 1){
		//try to send a SIGCONT
		if(killpg(lsproc.pgid,SIGCONT) == -1)
			perror("killpg() error");
		else
		{
			//if signal is sent change the proc state
			qchangestate(lsproc.pid, 1);
		}
	}
	else
	{
		printf("Background Queue is empty or no background processes are stopped\n");
	}
}

//wrapper function to call bringLastBGtoFG()
void fg(){
	bringLastBGtoFG();
}

/**
 * bring the last backgrounded job to the foreground
*/
void bringLastBGtoFG(){
	int status;
	
	struct BGProc last;				//temporarily hold onto foregrounding process
	
	//remove last backgrounded process from queue and set foreground info to it
	removehead(&last);
	setFGProc(last.pid, last.pgid, last.command);
	
	//send a SIGCONT incase the job is stopped
	if(killpg(last.pgid,SIGCONT) == -1)
		perror("killpg() error");	
	//if signal sent
	else{
		tcsetpgrp(0, last.pgid); //change the termninal control

		//wait for this process to finish
		waitpid(last.pid, &status, WUNTRACED);

		//send shell back to FG
		tcsetpgrp(0, shellPID);
		tcsetpgrp(1, shellPID);	
		tcsetpgrp(2, shellPID);		
	}	
}
/**
 * send a given process to the foreground
 * @param pid_t to send to the foreground
*/
void sendToFG(pid_t pid){
	struct BGProc tofg;
	//try removing from queue, store pid by reference
	if(remqueue(pid, &tofg) == 1){
		//try setting the TC to the process from the queue
		if(tcsetpgrp(0, tofg.pgid) != -1){
			setFGProc(tofg.pid, tofg.pgid, tofg.command);
		}
		else
			perror("setpgid() error");
	}
	else
	{
		printf("Process is not in background\n");
	}
}

/**
 * send a given process to the background
 * @param pid_t to send to the background , char* of the command 
*/
void sendToBG(pid_t pid, char* com){
	//insert into the queue
	enqueue(pid, getpgid(pid), com);
	//the the shell to be in the FG
	setFGProc(shellPID, shellPID, "Shell");
}
