#include "shell_signals.h"
#include "shellFunc.h"
#include "utils.h"
//#include "global.h"
#include "jobs.h"
#include <sys/wait.h>


#define MAX_BYTES 1024


///need to prevent carriage return for prompt	--DONE
///check into free(tok)							--DONE, WASN'T NEEDED. ACTUALLY MADE IT WORSE
///handle '\n' attached to string arg			--DONE, TAKEN CARE OF BY TOKENIZER
///possibly make token array non-global			--DONE
///need to check the wait commands in project2
///	and shellFunc to make sure being used
///	correctly after signalling added


///delete old code
///fix makfile
///replace constant values with flags
///comment
///clean up includes
///fix ignored signals to suspend or block(delayed)
///valgrind for memory leaks
///fix redirect/cat


int main(int argc, char* args[]){
	shname = "kinda-sh";	//initialize command line prompt string
	int length;				//length of input
	pid_t pid = -1;			//init a pid that will store the child pid

	memset (&sigAction, '\0', sizeof(sigAction)); 	//allocate memory for the  signal action struct
	sigAction.sa_sigaction = signal_handler; 		//set the handler for the signal action struct
	sigAction.sa_flags = SA_SIGINFO | SA_RESTART;	//flag that we want to collect information about the process when a signal is caught
													//and to restart interrupted syscalls


	queue_init();		//initialize queue for background process
	msgbuffer_init();	//initialize queue for messages waiting to be printed

	//if we're not in the child process
	if(pid != 0){
		shellPID = getpid();				//store the shellPID
	}
	while(1){
		
		//Output buffered messages from background processes
		msgbuffer_tostring();

		printf("%s> ", shname);						//output command line prompt
		fflush(stdout);								//flush the print buffer
		char input[MAX_BYTES];						//create buffer for input
		int status = 0;								//status for reading input

		sigaction(SIGTSTP, &sigAction, NULL);	//CTRL-Z stops the process as long as it's not shell
		sigaction(SIGINT, &sigAction, NULL);	//CTRL-C kill the process as long as it's not the shell
	
		
		//read input from command line
		status = read(STDIN_FILENO, input, MAX_BYTES);	//read input and store return value

		//Check if read failed
		if(status == -1){								//if failed to read, output error
			perror("READ ERROR");
		}
		else if(status <= 1){
			//printf("no input\n");
			continue;
		}
		else{
			input[status] = '\0';	//else, add null terminating char to input
			length = status;		//set the legth of to be num bytes read

			//check for fg, bg, help or exit
			if(cmp(input,"exit\n")){
				return 0;
			}
			else if(cmp(input,"help\n")){
				printf("help\n");
				continue;
			}
			else if(cmp(input,"bg\n")){
				bg();
				continue;
			}
			else if(cmp(input,"fg\n")){
				fg();
				continue;
			}
			else if(input[0] == SIGTSTP){
				fg();
				continue;
			}

			//fork process
			pid = fork();
		
			//child process
			if(pid==0){

				//check for a pipe, 1 if pipe, -1 if no pipe
				status = checkPipe(input, status);

				//if there's a pipe
				if(status > -1){
					char p1[status]; 			//space for the first cmd in the pipeline
					char p2[length - status]; 	//space for the second cmd in the pipeline
					
					input[status] = '\0'; 		//teminate the input

					//Prof. Butler told another student to look into using memcpy on piazza
					memcpy(p1, &input[0], status);	//copy the first cmd
					memcpy(p2, &input[status +1], length - status); //copy the second command

					processPipe(p1, p2); //process the pipe
				}
				//if no pipe, run single command
				else if(status == -1){
					processCommand(input);
				}
				_exit(0);
			}
			//parent
			else if(pid > 0){
				sigaction(SIGCHLD, &sigAction, NULL);	//child process changes state

				setpgid(pid, pid);		 //set the group ID of the single process to itself
				status = checkBG(input); //check for an &
				
				//if & then send the process to the background and wait without hanging
				if(status == 1){
					sendToBG(pid, input);
					waitpid(pid, &status, WNOHANG|WUNTRACED);
				}
				//else set the process to the foreground
				else{
					setFGProc(pid, getpgid(pid), input);
					waitpid(pid, &status, WUNTRACED);
				}
			}
			else
				printf("Error: Could not create child\n");
		}
	}
	free_queue(); //deallocate space
	return 0;
}
