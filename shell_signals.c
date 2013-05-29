#include "shell_signals.h"
#include "global.h"
#include "process_hash.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "jobs.h"

#define MAX_MSG 256

int status;

void signal_handler(int sigNum, siginfo_t *siginfo, void *context)
{
	//if a process chages state this signal will be triggered
	if(sigNum == SIGCHLD){
		printf("entered sigchld if\n");
		fgproc fgp;
		bgproc bgp;
		remqueue(siginfo->si_pid, &bgp);
		getFGProc(&fgp);

		//if finished
		if(siginfo->si_code == CLD_EXITED){
			printf("si_pid: %i fgp.pid: %i tcget: %i\n", siginfo->si_pid, fgp.pid, tcgetpgrp(0));
			if(siginfo->si_pid == fgp.pid){
				printf("\nFinished in fg: %s\n%s", fgp.command, shname);
			}
			else
			{
				//printf("enter bg of chldexit\n");
				
				char bgmsg[MAX_MSG];
				char* stat = "Finished: ";
				int i =0;
				while(stat[i] != '\0'){
					bgmsg[i] = stat[i];
					i++;
				}
				int j =0;
				char* com = bgp.command;
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
			setLastStoppedBG(siginfo->si_pid);
			
			printf("child stopped\n");
			
			//stop child process
			//if((status = killpg(fgp.pgid, SIGTSTP)) == -1){
				//perror("Child signal failed");
			//}
			//else
			//{
				//printf("Sent stop signal to child\n");
			//}
			
			////send stop signal to child
			//killpg(fgp.pgid, SIGTSTP);
			
			//add process to background queue
			//enqueue(fgp.pid, fgp.pgid, fgp.command);
			//qchangestate(fgp.pid, 0);
			
			//switch TC back to the shell
			//sendShellToFG();
		}
		//if continued
		else if(siginfo->si_code == CLD_CONTINUED){
			printf("\nRunning: %i", lastStoppedBG.pgid);
		}
		else if(siginfo->si_code == SIGTERM)
			printf("\nTerminated: %i", currentfg.pgid);
		else
			printf("Child had some other exit status. Exit status was: %i\n", siginfo->si_code);
		
		
		//if the process is in the background, queue  the  message
		
		//else, display the message
		
		waitpid(-1,&status,WUNTRACED|WNOHANG);
		//waitpid(0,&status,0);
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
			printf("child stopped\n");
		}
		else{
			printf("do nothing\n");	
			waitpid(-1,&status,WUNTRACED);	
			//the shell do nothing
		}	
	}
	//if shell is told to continue, send to foreground
	else if(sigNum == SIGCONT){
		printf("SIGCONT\n");
		//sendShellToFG();
			
	
		//signal(SIGTTOU, SIG_DFL);
		
	}	
}
