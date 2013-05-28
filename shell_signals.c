#include "shell_signals.h"
#include "global.h"
#include "process_hash.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "jobs.h"

int status;


void signal_handler(int signal, siginfo_t *siginfo, void *context)
{
	//if a process chages state this signal will be triggered
	if(signal == SIGCHLD){
		HPROC* tempProc = searchProc(siginfo->si_pid);
		pid_t tmpPGID = tempProc->pgid;
		//printf("Shell PID: %ld\n",(long)shellPID);
		
		//printf("PID: %ld PGID: %ld changed state to %i\n",(long)siginfo->si_pid, (long)tmpPGID, siginfo->si_code);
		//detect the status of the process and generate a message
		
		//if finished
		if(siginfo->si_code == CLD_EXITED){
			printf("\nFinished: %s", searchProc(tmpPGID)->command);
			
			//switch TC back to the shell
			sendShellToFG();
		}
		//if stopped
		else if(siginfo->si_code == CLD_STOPPED){
			
			//let the user know what process stopped
			printf("\nStopped: %s", searchProc(tmpPGID)->command);
			
			//set the most recent stopped BG process
			setLastStoppedBG(siginfo->si_pid);
			
			//switch TC back to the shell
			sendShellToFG();
		}
		//if continued
		else if(siginfo->si_code == CLD_CONTINUED)
			printf("\nRunning: %s", searchProc(tmpPGID)->command);
		else
			printf("Child had some other exit status\n");
		
		
		//if the process is in the background, queue  the  message
		
		//else, display the message
		
		
		waitpid(0,&status,0);
	}
	else if(signal == SIGTTIN){
		printf("Recieved SIGTIN");
	}
	else if(signal == SIGTTOU){
		printf("Recieved SIGTOU");
	}
	//CTRL-C
	else if(signal == SIGTERM){

		if(getpgid(0)==shellPID){
			printf("Shell recieved CTRL-C\n");
		}
		else{
			printf("Foreground recieved CTRL-C\n");
		}
	}
	//CTRL-Z
	else if(signal == SIGTSTP){
		//printf("\nCTRL-Z, siginfo->si_pid: %ld\n",(long) siginfo->si_pid);
		//printf("\nCTRL-Z, getpid(): %ld\n",(long) getpid());
		//printf("tcgetpgrp(0): %ld\n", tcgetpgrp(0));
		
		
		//killpg(siginfo->si_pid,SIGSTOP);
	//	if(tcgetpgrp(0)!=shellPID){
			//not the shell, do something
			//printf("do something\n");
		//}
		//else{
			//printf("do nothing\n");			
			//the shell do nothing
		//}	
	}
}
