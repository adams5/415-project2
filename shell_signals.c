#include "shell_signals.h"
int status;


void signal_handler(int signal)
{
	if(signal == SIGCHLD){
		waitpid(0,&status,0);
	}
	
}
