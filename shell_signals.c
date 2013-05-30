#include "shell_signals.h"
#include "global.h"
//#include "process_hash.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "jobs.h"

#define MAX_MSG 256

int status;

void signal_handler(int sigNum, siginfo_t *siginfo, void *context)
{
	//set up struct objects to hold data durring signal handling
	bgproc bgp;
	fgproc fgp;
	getFGProc(&fgp);
	
	//if a process chages state this signal will be triggered
	if(sigNum == SIGCHLD){
		//if foreground process did not trigger signal, search background processes
		if(fgp.pid != siginfo->si_pid){
			if((findqproc(siginfo->si_pid, &bgp)) == -1){
				printf("could not find process for bg proc\n");
			}
		}

		//if finished
		if(siginfo->si_code == CLD_EXITED){
			//if exiting process was in foreground print to terminal
			if(siginfo->si_pid == fgp.pid){
				printf("Finished: %s\n", fgp.command);
			}
			//Else buffer messages to output later
			else
			{
				//buffer messages from backgrounded process
				char* stat = "Finished: ";
				char* com = bgp.command;		//process command
				char bgmsg[MAX_MSG];			//char array to hold string to buffer
				
				//Concatinate stat and com together into one string to buffer
				int i =0;
				while(stat[i] != '\0'){
					bgmsg[i] = stat[i];
					i++;
				}
				int j =0;
				printf("command to be queued: %s\n", com);
				while(com[j] != '\0'){
					bgmsg[i++] = com[j++];
				}
				bgmsg[i] = '\0';			//explicitly set null terminator
				
				//insert exiting message into buffer
				insert_msgbuffer(bgmsg);
			}
		}
		//if stopped
		else if(siginfo->si_code == CLD_STOPPED){
			
			//let the user know what process stopped
			printf("\nStopped: %s", fgp.command);
						
			//set the most recent stopped BG process
			sendToBG(fproc.pid, fproc.command);
			qchangestate(siginfo->si_pid, 0);
			
		}
		//if continued
		else if(siginfo->si_code == CLD_CONTINUED){
			
			//let user know what process is resuming running
			if(siginfo->si_pid == fgp.pid)
				printf("Running: %s", fgp.command);
			else if(siginfo->si_pid == bgp.pid)
				printf("Running: %s", bgp.command);
			else
				printf("No Information on Process Available\n");
		}
		//if process got terminate signal
		else if(siginfo->si_code == SIGTERM){
			setFGProc(shellPID, shellPID, "Shell");
			printf("\nTerminated: %s", fgp.command);
		}
		else if(siginfo->si_code == SIGTTIN){
		}
		else if(siginfo->si_code == SIGTTOU){
		}		
		else
		waitpid(-1,&status,WUNTRACED|WNOHANG);
	}
	else if(sigNum == SIGTTIN){
		//let the user know what process stopped
		printf("SIGTTIN found : %s\n", fgp.command);
	}
	else if(sigNum == SIGTTOU){
		printf("Recieved SIGTOU");
	}
	//CTRL-C
	else if(sigNum == SIGTERM){
		//should never reach this point
		if(getpgid(0)==shellPID){
			printf("Shell recieved CTRL-C\n");
		}
		else{
			printf("Foreground recieved CTRL-C\n");
		}
	}
	//CTRL-Z
	else if(sigNum == SIGTSTP){
		//if child receives terminal stop
		if(fproc.pid != shellPID && fproc.pid != 0){
			kill(fproc.pid, SIGTSTP);
		}
		else{
			//the shell do nothing
			waitpid(-1,&status,WUNTRACED|WNOHANG);	
		}	
	}
	//if foreground is told to continue, output message
	else if(sigNum == SIGCONT){
		printf("SIGCONT\n");
	}	
}
