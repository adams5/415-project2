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
		int errorcode = 0;							//initialize variable for error codes						
		
		
		errorcode = read(STDIN_FILENO, input, MAX_BYTES);	//read input and store return value
		
		int numTokens = 0;							//value to hold how many tokens in input
		if(errorcode == -1)							//if failed to read, output error
			perror("READ ERROR");
		else{
			input[errorcode] = '\0';				//else, add null terminating char to input
			//printf("Input was: %s\n", input);
			
			numTokens = getTokens(input, tokens);	//create array of tokens
			
			//Debugging
			int j;
			for(j = 0; j < numTokens-1; j++){
				checkTok(tokens[j], j, tokens);
				printf("Token at %i is %s\n", j, tokens[j]);
			}




			free(tokens);
		}
	}
	
	
	
	
	
	
}
