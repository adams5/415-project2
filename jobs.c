#include "jobs.h"
#include "global.h"
//#include "process_hash.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void bg(){	
	struct BGProc lsproc;
	
	if(findlaststopped(&lsproc) == 1){
		if(killpg(lsproc.pgid,SIGCONT) == -1)
			perror("killpg() error");
		else
		{
			qchangestate(lsproc.pid, 1);
			//setLastBG(lsproc.pid, lsproc.pgid, lsproc);	
			
			//remqueue(lsproc.pid, NULL);
			//enqueue(lsproc.pid, lsproc.pgid, lsproc.command);
		}
	}
	else
	{
		printf("Background Queue is empty or no background processes are stopped\n");
	}
}


void fg(){
	//bgproc lproc;
	//if(getBGProc(&lproc));
		
	//bring the most recetly backgrounded job to the foreground
	bringLastBGtoFG();
	
		//tcsetpgrp(0, shellPID);;

}

//void setLastBG(pid_t pid){
	////printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	//lastBG.pid = pid;
	//lastBG.pgid = getpgid(pid);
//}

//void setLastStoppedBG(pid_t pid){
	////printf("lastStoppedBG lastStoppedBG.pid: %ld lastStoppedBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	//lastStoppedBG.pid = pid;
	//lastStoppedBG.pgid = getpgid(pid);
//}

void bringLastBGtoFG(){
	int status;
	
	struct BGProc last;				//temporarily hold onto foregrounding process
	
	//remove last backgrounded process from queue and set foreground info to it
	removehead(&last);
	setFGProc(last.pid, last.pgid, last.command);
	
		//send a SIGCONT in case the job is stopped
	if(killpg(last.pgid,SIGCONT) == -1)
		perror("killpg() error");	
	else{
		tcsetpgrp(0, last.pgid);
		
		//setFGProc(last.pid, last.pgid, last.command);

		waitpid(last.pid, &status, WUNTRACED);

		tcsetpgrp(0, shellPID);
		tcsetpgrp(1, shellPID);	
		tcsetpgrp(2, shellPID);		
	}	
	//setFGProc(shellPID, last.pgid, last.command);

	//send a SIGCONT in case the job is stopped
	//if(killpg(last.pgid,SIGCONT) == -1)
	//	perror("killpg() error");	

	//currentfg.pgid = last.pgid;
	//currentfg.pid = last.pid;
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
	struct BGProc tofg;
	if(remqueue(pid, &tofg) == 1){
		if(tcsetpgrp(0, tofg.pgid) != -1){
			//currentfg.pid = pid;
			//currentfg.pgid = getpgid(pid);
			setFGProc(tofg.pid, tofg.pgid, tofg.command);
			//remqueue(currentfg.pid, NULL);
			//removeProc(currentfg.pgid);
		}
		else
			perror("setpgid() error");
	}
	else
	{
		printf("Process is not in background\n");
	}
	
	
	//printf("\ntcgetpgrp: %ld\n",(long) tcgetpgrp(0));
}

void sendToBG(pid_t pid, char* com){
	enqueue(pid, getpgid(pid), com);
	setFGProc(shellPID, shellPID, "Shell");
	//printf("Foreground proc set\n");
	//printf("setting currentfg, shellPID is: %i\n", shellPID);
	//currentfg.pgid = shellPID;
	//printf("setting currentfg.pid, shellPID is: %i\n", shellPID);
	//currentfg.pid = shellPID;
	//printf("shellPID in sendtobg is: %i\n", shellPID);
	//sendShellToFG();
	//printf("setLastBG lastBG.pid: %ld lastBG.pgid: %ld\n",(long) pid,(long) getpgid(pid));
	//printf("exiting sendToBG\n");
}
