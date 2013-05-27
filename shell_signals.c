#include "shell_signals.h"
#include "global.h"

int status;


void signal_handler(int signal)
{
	if(signal == SIGCHLD){
		waitpid(0,&status,0);
	}
	else if(signal == SIGTTIN){
		printf("Recieved SIGTIN");
	}
	else if(signal == SIGTTOU){
		printf("Recieved SIGTOU");
	}
	else if(signal == SIGTERM){

		if(getpgid(0)==shellPID){
			printf("Shell recieved CTRL-C\n");
		}
		else{
			printf("Foreground recieved CTRL-C\n");
		}
	}
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
