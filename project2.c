#include <signal.h>
//#include "tokenizer.h"
#include "shellFunc.h"

#define MAX_BYTES 1024
#define MAX_TOKENS 256


///need to prevent carriage return for prompt	--DONE
///check into free(tok)							--DONE, WASN'T NEEDED. ACTUALLY MADE IT WORSE
///handle '\n' attached to string arg			--DONE, TAKEN CARE OF BY TOKENIZER
///possibly make token array non-global			--DONE



int main(int argc, char* args[]){
	char** tokens;									//declare array of tokens
	char* shname = "kinda-sh";						//initialize command line prompt string
	
	while(1){
		printf("%s> ", shname);						//output command line prompt
		fflush(stdout);								//flush the print buffer
		char input[MAX_BYTES];						//create buffer for input
		tokens = calloc(MAX_TOKENS, sizeof *tokens);//allocate memory for array for tokens
		int numTokens = 0;							//value to hold how many tokens in input
		int status = 0;
		
		//read input from command line
		status = read(STDIN_FILENO, input, MAX_BYTES);	//read input and store return value
		//printf("bytes read: %i\n", status);
		
		//Check if read failed
		if(status == -1){								//if failed to read, output error
			perror("READ ERROR");
		}
		else if(status <= 1){
			//printf("no input\n");
			continue;
		}
		else{
			input[status] = '\0';						//else, add null terminating char to input			
			
			status = checkPipe(input, status);			//check for pipe in command line input
			
			if(status == -1){							//if no pipe, run single command
				numTokens = getTokens(input, tokens);	//create array of tokens
				status = checkBG(tokens);				//check for & for backgrounding a process
				checkRed(tokens, 0);					//check for and handle redirection
				pid_t pid;
				pid = fork();
				if(pid == 0){
					execute(tokens, 0, status);			//run single command
				}
				else if(pid > 0){
					if(status == 1)
						waitpid(pid, &status, 0);
				}
				else
					printf("Error: Could not create child\n");
			}
			else{
				input[status] = '\0';
				
				char* p1 = input[0];
				char* p2 = input[status +1];
				processPipe(p1, p2);
			}
		}
		free(tokens);
	}
	
	
	
	
	
	
}
