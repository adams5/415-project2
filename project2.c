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
		
		//Check if read failed
		if(status == -1)								//if failed to read, output error
			perror("READ ERROR");
		else{
			input[status] = '\0';						//else, add null terminating char to input			
			
			status = checkPipe(input, status);			//check for pipe in command line input
			
			if(status == -1){							//if no pipe, run single command
				numTokens = getTokens(input, tokens);	//create array of tokens
				status = checkBG(tokens);
				checkRed(tokens, 0);
			}
			else{
				input[status] = '\0';
				
				char* p1 = input[0];
				char* p2 = input[status +1];
				processPipe(p1, p2);
			}



			free(tokens);
		}
	}
	
	
	
	
	
	
}
