#include <signal.h>
#include "tokenizer.h"
#include "shellFunc.h"

#define MAX_BYTES 1024
#define MAX_TOKENS 256

char* shname = "kinda-sh";
char* tok;
TOKENIZER *tokenizer;
char** tokens;


///need to prevent carriage return for prompt
///check into free(tok)
///handle '\n' attached to string arg
///possibly make token array non-global



int main(int argc, char* args[]){
	//char* tokens;
	int errorcode;
	
	while(1){
		printf("%s> ", shname);
		printf("");
		char input[MAX_BYTES];
		tokens = calloc(MAX_TOKENS, sizeof *tokens);
		//char* tokens[MAX_TOKENS];
		errorcode = 0;
		
		
		errorcode = read(STDIN_FILENO, input, MAX_BYTES);
		
		if(errorcode == -1)
			perror("READ ERROR");
		else{
			input[errorcode] = '\0';
			printf("Input was: %s\n", input);
			
			//create tokenizer
			tokenizer = init_tokenizer(input);
			
			int i = 0;
			while((tok = get_next_token(tokenizer)) != NULL){
				//if(!isspace(){
				if(tok[0] != '\n' && tok[0] != '\0' && tok[0] != ' '){
					tokens[i++] = tok;
					//free(tok);
					printf("token is: %s\n", tokens[i-1]);
				}
			}
			
			int j;
			for(j = 0; j < i; j++){
			//while(
				checkTok(tokens[j], j, tokens);
				printf("Token at %i is %s\n", j, tokens[j]);
			}
			
			free_tokenizer(tokenizer);
			free(tokens);
		}
	}
	
	
	
	
	
	
}
