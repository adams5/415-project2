#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "tokenizer.h"



#include <stdio.h>

#define MAX_BYTES 1024
#define MAX_TOKENS 256

char* shname = "kinda-sh> \0";
char* tok;
int errorcode;
TOKENIZER *tokenizer;
char* tokens[MAX_TOKENS];


///need to prevent carriage return for prompt
///check into free(tok)
///handle '\n' attached to string arg
///possibly make token array non-global


void checkTok(char* tempTok, int pos){
	if(tempTok[0] == '>'){
		printf("redirecting output\n");
		int new_out = open(tokens[pos+1], O_WRONLY | O_CREAT, 0644);
		dup2(new_out, STDOUT_FILENO);
		printf("output redirected to %s\n", tokens[pos+1]);
	}
	else if(tempTok[0] == '<'){
		int new_in = open(tokens[pos-1], O_RDONLY);
		dup2(new_in, STDIN_FILENO);
		printf("input redirected\n");
	}
	else if(tempTok[0] == '|'){
		///do something
	}
	else
		printf("Parse Error:");
	
	
}

int main(int argc, char* args[]){
	
	while(1){
		printf("%s", shname);
		//printf("entered shell loop\n");
		char input[MAX_BYTES];
		//printf("buffer created\n");
		errorcode = 0;
		//printf("set error code to 0\n");
		//printf("array created\n");
		
		
		errorcode = read(STDIN_FILENO, input, MAX_BYTES);
		
		if(errorcode == -1)
			perror("READ ERROR");
		else{
			printf("Input was: %s", input);
			
			//create tokenizer
			tokenizer = init_tokenizer(input);
			
			int i = 0;
			while((tok = get_next_token(tokenizer)) != NULL){
				if(tok[0] != '\n'){
					tokens[i] = tok;
					free(tok);
					//printf("%s", tokens[i-1]);
				}
			}
			
			int j;
			for(j = 0; j <=i; j++){
				checkTok(tokens[j], j);
				printf("Token at %i is %s\n", j, tokens[j]);
			}
			
			free(tokenizer);
		}
	}
	
	
	
	
	
	
}
