#include "jobs.h"
#include "global.h"
//#include "process_hash.h"
#include <stdio.h>
#include <unistd.h>

void bg(){	
	//if(!qisempty()){
		bgproc lsproc;
		if((getBGProc(&lsproc)) == -1)
			printf("Error, no job stopped in the background\n");
		//send SIGCONT to the most recently stopped background job, this will resume execution
		else{
			if(killpg(lsproc.pgid,SIGCONT) == -1)
				perror("killpg() error");
			else
			{
				setLastBG(lastStoppedBG.pid);	
				
				//remqueue(lsproc.pid, NULL);
				//enqueue(lsproc.pid, lsproc.pgid, lsproc.command);
			}
		}
	//}
	//else
	//{
		//printf("Background Queue is empty\n");
	//}
}


void fg(){
	//if(!qisempty()){
		bgproc lproc;
		getBGProc(&lproc);
		//qpeekhead(&lproc);
		
		//bgproc lproc;
		//if(lastBG.pgid == 0){
			//printf("Error, no job in the background\n");
		//}
		//else{
		//printf("fg lastBG.pid: %ld lastBG.pgid: %ld\n",(long) lastBG.pid,(long) lastBG.pgid);
		printf("%s\n",searchProc(lastBG.pgid)->command);
		
		//bring the most recetly backgrounded job to the foreground
		bringLastBGtoFG();

		//send a SIGCONT in case the job is stopped
		if(killpg(lproc.pgid,SIGCONT) == -1)
			perror("killpg() error");
		//}
	//}
	//else
	//{
		//printf("Background Queue is empty\n");
	//}
	
}

void setLastBG(pid_t pid){
	//printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	lastBG.pid = pid;
	lastBG.pgid = getpgid(pid);
}

void setLastStoppedBG(pid_t pid){
	//printf("lastStoppedBG lastStoppedBG.pid: %ld lastStoppedBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	lastStoppedBG.pid = pid;
	lastStoppedBG.pgid = getpgid(pid);
}

void bringLastBGtoFG(){
	bgproc last;
	getBGProc(&last);
	//dequeue(&last);
	tcsetpgrp(0, last.pgid);
	currentfg.pgid = last.pgid;
	currentfg.pid = last.pid;
}

//switch the terminal control back to the shell
//void sendShellToFG(){
	//printf("call to send shell to fg\n");
	////printf("%s> ", shname);
	//if(tcsetpgrp(0, shellPID) != -1){
		//tcsetpgrp(1, shellPID);
		//currentfg.pid = shellPID;
		//currentfg.pgid = getpgid(shellPID);
	//}
	//else
		//perror("setpgid() error");
//}

void sendToFG(pid_t pid){
	//printf("Setting %ld to tc\n",(long) pid);
	if(tcsetpgrp(0, pid) != -1){
		currentfg.pid = pid;
		currentfg.pgid = getpgid(pid);
		setFGProc(pid, getpgid(pid), NULL);
		//remqueue(currentfg.pid, NULL);
		//removeProc(currentfg.pgid);
	}
	else
		perror("setpgid() error");
	
	//printf("\ntcgetpgrp: %ld\n",(long) tcgetpgrp(0));
}

void sendToBG(pid_t pid, char* com){
	printf("enter sendtobg\n");
	//setLastBG(pid);
	//enqueue(pid, getpgid(pid), com);
	setFGProc(shellPID, shellPID, com);
	printf("Foreground proc set\n");
	//printf("setting currentfg, shellPID is: %i\n", shellPID);
	currentfg.pgid = shellPID;
	//printf("setting currentfg.pid, shellPID is: %i\n", shellPID);
	currentfg.pid = shellPID;
	//printf("shellPID in sendtobg is: %i\n", shellPID);
	//sendShellToFG();
	//printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	printf("exiting sendToBG\n");
}
