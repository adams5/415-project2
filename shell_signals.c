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
			//printf("entered sigchld if\n");
		fgproc fgp;
		getFGProc(&fgp);
		
		bgproc bgp;

				//printf("Child had some other exit status. Exit status was: %i\n", siginfo->si_code);
	
	//if a process chages state this signal will be triggered
	if(sigNum == SIGCHLD){
		if(fgp.pid != siginfo->si_pid){
			printf("searching for pid: %i\n", siginfo->si_pid);
			remqueue(siginfo->si_pid, &bgp);
			printf("proc %i has a command of: %s\n", bgp.pid, bgp.command);
		}

		//if finished
		if(siginfo->si_code == CLD_EXITED){
			//printf("si_pid: %i fgp.pid: %i tcget: %i\n", siginfo->si_pid, fgp.pid, tcgetpgrp(0));
			if(siginfo->si_pid == fgp.pid){
				printf("Finished: %s\n", fgp.command);
			}
			else
			{
				//buffer messages from backgrounded process
				char* stat = "Finished: ";
				char* com = bgp.command;
				char bgmsg[MAX_MSG];
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
				bgmsg[i] = '\0';
				printf("string in bg array: %s\n", bgmsg);
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
			
			printf("child stopped\n");
			
		//if continued
		}
		else if(siginfo->si_code == CLD_CONTINUED){
			printf("\nRunning: %i", lastStoppedBG.pgid);
		}
		else if(siginfo->si_code == SIGTERM){
			setFGProc(shellPID, shellPID, "Shell");
			printf("\nTerminated: %i", currentfg.pgid);
		}
		else if(siginfo->si_code == SIGTTIN){
						//sendToBG(fproc.pid, fproc.command);
			//if(killpg(shellPID,SIGSTOP) == -1)
				//perror("killpg() error");	
			
			//let the user know what process stopped
			//printf("\nStopped: %s", fgp.command);
			
			//sendToBG(fproc.pid, fproc.command);

			//sendToFG(sh)
			//qchangestate(siginfo->si_pid, 0);
		}
		else if(siginfo->si_code == SIGTTOU){
						//sendToBG(fproc.pid, fproc.command);
			//if(killpg(shellPID,SIGSTOP) == -1)
				//perror("killpg() error");	
			
			//let the user know what process stopped
					//	printf("Recieved SIGTOU");
			//
			//sendToBG(fproc.pid, fproc.command);

			//sendToFG(sh)
			//qchangestate(siginfo->si_pid, 0);
		}		
		else
			//printf("Child had some other exit status. Exit status was: %i\n", siginfo->si_code);
		
		
		//if the process is in the background, queue  the  message
		
		//else, display the message
		
		waitpid(-1,&status,WUNTRACED|WNOHANG);
		//waitpid(0,&status,0);
	}
	else if(sigNum == SIGTTIN){
		//let the user know what process stopped
			printf("SIGTTIN found : %s\n", fgp.command);
						
			//set the most recent stopped BG process
			//sendToBG(fproc.pid, fproc.command);
			//qchangestate(siginfo->si_pid, 0);
			//waitpid(-1, &status, WNOHANG|WUNTRACED);
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
		//if(fproc.pid != shellPID){
			////not the shell, do something
			//printf("child stopped\n");
			////siginfo->si_code = CLD_STOPPED;
			//kill(shellPID, SIGCHLD);
			////kill(fproc.pid, SIGTSTP);
		//}
		//else{
			printf("do nothing\n");	
			waitpid(-1,&status,WUNTRACED);	
			//the shell do nothing
		//}	
	}
	//if shell is told to continue, send to foreground
	else if(sigNum == SIGCONT){
		printf("SIGCONT\n");
		//sendShellToFG();
			
	
		//signal(SIGTTOU, SIG_DFL);
		
	}	
}
