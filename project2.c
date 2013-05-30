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
	//char** tokens;									//declare array of tokens
	shname = "kinda-sh";						//initialize command line prompt string
	int length;
	pid_t pid = -1;


	memset (&sigAction, '\0', sizeof(sigAction)); 	//allocate memory for the  signal action struct

	sigAction.sa_sigaction = signal_handler; 			//set the handler for the signal action struct

	sigAction.sa_flags = SA_SIGINFO | SA_RESTART;	//flag that we want to collect information about the process when a signal is caught
													//and to restart interrupted syscalls

	queue_init();								//initialize queue for background messages
	msgbuffer_init();

	if(pid != 0){
		shellPID =  getpid();
		printf("shellPID: %i\n",shellPID);
	}
	while(1){

		//Output buffered messages from background processes
		msgbuffer_tostring();

		printf("%s> ", shname);						//output command line prompt
		fflush(stdout);								//flush the print buffer
		char input[MAX_BYTES];						//create buffer for input
		//tokens = calloc(MAX_TOKENS, sizeof *tokens);//allocate memory for array for tokens
		//int numTokens = 0;							//value to hold how many tokens in input
		int status = 0;

		//create new and old signal masks
		sigset_t newset;
		sigset_t oldset;
		
		//add signals to newset to block with signal mask
		sigemptyset(&newset);
		sigaddset(&newset, SIGINT);
		sigaddset(&newset, SIGTTOU);
		sigaddset(&newset, SIGTTIN);
		sigaddset(&newset, SIGTSTP);
		
		//block signals with signal mask while reading user input and after fork
		sigprocmask(SIG_BLOCK, &newset, &oldset);
		
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
			length = status;

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
			//else if(input[0] = '^'){
				//printf("Invalid command\n");
				//continue;
			//}
           // lastJobCmd = input;

			//fork process
			pid = fork();

		
			//child
			if(pid==0){
				//unblock signalmask for forking of child and parent
				sigprocmask(SIG_UNBLOCK, &newset, &oldset);

				sigaction (SIGTSTP, &sigAction, NULL);	//CTRL-Z stops the process as long as it's not shell

				//check for a pipe
				status = checkPipe(input, status);			//check for pipe in command line input

				//if there's a pipe
				if(status > -1){
					char p1[status];
					char p2[length - status];
					////debugging
					//printf("there's a pipe\n");
					//printf("the command line is: %s\n", input);

					input[status] = '\0';
					//printf("set pipe to null, input: %s\n", input);

					memcpy(p1, &input[0], status);
					//printf("copied first command to p1, input: %s\n", input);
					memcpy(p2, &input[status +1], length - status);
					//printf("copied second command to p2, input: %s\n", input);

					////debugging
					//printf("the first command is: %s\n", p1);
					//printf("the second command is: %s\n", p2);

					processPipe(p1, p2);
				}
				//if no pipe, run single command
				else if(status == -1){
					processCommand(input);
				}
				_exit(0);
			}
			//parent
			else if(pid > 0){
				//block the following signals
				signal(SIGTSTP, SIG_IGN);	//CTRL-Z stops the process as long as it's not shell
				signal(SIGTTOU, SIG_IGN);
				signal(SIGINT, SIG_IGN);
				signal(SIGQUIT, SIG_IGN);
				//signal(SIGTTIN, SIG_IGN);
				
				//sigaction (SIGCONT, &sigAction, NULL);	//will resume execution of the recieving process, don't think we need
				sigaction(SIGTERM, &sigAction, NULL);	//CTRL-C
				sigaction(SIGCHLD, &sigAction, NULL);	//child process changes state
				//sigaction(SIGTTIN, &sigAction, NULL);	//child process changes state

				setpgid(pid, pid);
				status = checkBG(input);
				if(status == 1){
					//printf("sending process to bg\n");
					//printf("command for fork is: %s\n", input);
					//qchangevis(pid, 0);
					sendToBG(pid, input);
					//setBGProc(pid, getpgid(pid), input);
					waitpid(pid, &status, WNOHANG|WUNTRACED);
				}
				else
				{
					setFGProc(pid, getpgid(pid), input);
					waitpid(pid, &status, WUNTRACED);
				}


				//currentfg.pid = pid;
				//currentfg.pgid= pid;
				//insertProc(pid, getpgid(pid), input);
				//printf("Running: %s", searchProc(pid)->command);
			}
			else
				printf("Error: Could not create child\n");
		}
	}
	free_queue();
	return 0;
}
