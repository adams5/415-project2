#include "shell_signals.h"
int status;


void signal_handler(int signal)
{
	if(signal == SIGCHLD){
		waitpid(0,&status,0);
	}
	else if(signal == SIGTIN){
		printf("Recieved SIGTIN");
	}
	else if(signal == SIGTOU){
		printf("Recieved SIGTOU");
	}
	else if(signal == SIGTERM,){
		printf("Recieved SIGTERM");
	}	
	else if(signal == SIGTSTP){
		printf("Recieved SIGTSTP");
	}
	else if(signal == SIGCONT){
		printf("Recieved SIGCONT");
	}	
			
}
