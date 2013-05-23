#include "shellFunc.h"

void checkTok(char* tempTok, int pos, char* tokens[]){
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
		printf("This is a pipe\n");
		///do something
	}
	else
		//printf("Parse Error:");
		return;
	
	
}
