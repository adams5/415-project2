#include "shell_signals.h"
#include "global.h"
#include <unistd.h>
#include <signal.h>

int status;


void signal_handler(int signal, siginfo_t *siginfo, void *context)
{
	//if a process chages state this signal will be triggered
	if(signal == SIGCHLD){
		printf("PID: %ld changed state to %i\n",(long)siginfo->si_pid, siginfo->si_code);
		//detect the status of the process and generate a message
		
		//if finished
		if(siginfo->si_code == CLD_EXITED)
			printf("Finished: %s\n", lastJobCmd);
		//if stopped
		else if(siginfo->si_code == CLD_STOPPED)
			printf("Stopped: %s\n", lastJobCmd);
		//if continued
		else if(siginfo->si_code == CLD_CONTINUED)
			printf("Continuing: %s\n", lastJobCmd);
		else
			printf("Child had some other exit status\n");
		
			
		//else if finished
		
		//else if stopped
		
		
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
		//if(getpgid(0)==shellPID){
			printf("\nStopped: %s", lastJobCmd);
		//}
		////else{
		//	printf("Foreground recieved CTRL-Z\n");
		//}
	}
	else if(signal == SIGCONT){
		printf("Recieved SIGCONT");
	}

}
