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
		pid_t tmpPGID = getpgid(siginfo->si_pid);
		printf("Shell PID: %ld\n",(long)shellPID);
		
		printf("PID: %ld PGID: %ld changed state to %i\n",(long)siginfo->si_pid, (long)tmpPGID, siginfo->si_code);
		//detect the status of the process and generate a message
		
		//if finished
		if(siginfo->si_code == CLD_EXITED)
			printf("Finished: %s\n", searchProc(tmpPGID));
		//if stopped
		else if(siginfo->si_code == CLD_STOPPED){
			
			//let the user know what process stopped
			printf("Stopped: %s\n", searchProc(tmpPGID));
			
			//set the most recent stopped BG process
			setLastStoppedBG(siginfo->si_pid);
		}
		//if continued
		else if(siginfo->si_code == CLD_CONTINUED)
			printf("Continuing: %s\n", searchProc(tmpPGID));
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

		
		if(getpid()==shellPID){
			printf("\nshell recieved CTRL-Z\n");
		}
		else{
			printf("\nNon-shell recieved CTRL-Z, switching to shell\n");
			//switch the terminal control back to the shell
			sendShellToFG();
		}
	}
}
