#include "shell_signals.h"
#include "global.h"
//#include "process_hash.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "jobs.h"

int status;

void signal_handler(int sigNum, siginfo_t *siginfo, void *context)
{
	//if a process chages state this signal will be triggered
	if(sigNum == SIGCHLD){
		HPROC* tempProc = searchProc(siginfo->si_pid);
		pid_t tmpPGID = tempProc->pgid;
		//printf("Shell PID: %ld\n",(long)shellPID);
		
		//printf("PID: %ld PGID: %ld changed state to %i\n",(long)siginfo->si_pid, (long)tmpPGID, siginfo->si_code);
		//detect the status of the process and generate a message
		
		//if finished
		if(siginfo->si_code == CLD_EXITED){
			if(siginfo->si_pid == currentfg.pid)
				printf("\nFinished: %s", searchProc(tmpPGID)->command);
			else
			{
				char* com = searchProc(tmpPGID)->command;
				char* stat = "Finished: ";
				char* merge = malloc(sizeof com + sizeof stat);
				insertmsg(merge);
				removeallmsg();
				free(merge);
			}
			
			///need to add redirection of output if in background
			
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
			printf("\nRunning: %s", searchProc(lastStoppedBG.pgid)->command);
		else if(siginfo->si_code == SIGTERM)
			printf("\nTerminated: %s", searchProc(currentfg.pgid)->command);
		else
			printf("Child had some other exit status. Exit status was: %i\n", siginfo->si_code);
		
		
		//if the process is in the background, queue  the  message
		
		//else, display the message
		
		
		waitpid(0,&status,0);
	}
	else if(sigNum == SIGTTIN){
		printf("Recieved SIGTIN");
	}
	else if(sigNum == SIGTTOU){
		printf("Recieved SIGTOU");
	}
	//CTRL-C
	else if(sigNum == SIGTERM){

		if(getpgid(0)==shellPID){
			printf("Shell recieved CTRL-C\n");
		}
		else{
			printf("Foreground recieved CTRL-C\n");
		}
	}
	//CTRL-Z
	else if(sigNum == SIGTSTP){
					
		//printf("\nCTRL-Z, siginfo->si_pid: %ld\n",(long) siginfo->si_pid);
		//printf("\nCTRL-Z, getpid(): %ld\n",(long) getpid());
		//printf("tcgetpgrp(0): %ld\n", tcgetpgrp(0));
		
		
		//killpg(siginfo->si_pid,SIGSTOP);
		if(tcgetpgrp(0)!=shellPID){
			//not the shell, do something
			printf("do something\n");
		}
		else{
			printf("do nothing\n");			
			//the shell do nothing
		}	
	}
	//if shell is told to continue, send to foreground
	else if(sigNum == SIGCONT){
		printf("SIGCONT\n");
		sendShellToFG();
			
	
		//signal(SIGTTOU, SIG_DFL);
		
	}	
}
