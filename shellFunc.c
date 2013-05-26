#include "shellFunc.h"
pid_t groupID;

int checkPipe(char input[], int length){
	for(int i=0; i < length; i++){
		if(input[i] == '|')
			return i;
	}
	return -1;
}

void processPipe(char* process1, char* process2){	
	
	
	
	//printf("Process pipe entered\n");
	int pipeFD[2];	//create array to hold both file descriptor generated by pipe()
	 //pid;		//create a process id
	//groupID = getpid(); //save the groupID as the parent's pid
	
	pipe(pipeFD);	//make the two pipes
	
	/*
	 * 
	 * PROCESS 1
	 * 
	 */
	 
	pid_t pid = fork();	//fork the process

	if(pid == 0){						//in child process
	
		setpgid(getpid(),groupID);	//set the child'd group to be that of the it's parent
			
		close(pipeFD[0]); 				//close the read end of the pipe
		if(pipeFD[1] != 1) 	//if the pipeFD[1] is not already stdout
			dup2(pipeFD[1],1); //dup the pipeFD[1] to stdout
		
		
		//handle redirection if there's any
		
		
		//parse arguments
		
		//test with process 1 being "ls -l"
		char *process1Args[] = {"ls", "-l",NULL};	
		execvp("ls",process1Args);
		
		
		
	}	
	//parent
	else{
		close(pipeFD[1]);				//close the write end of the pipe	
		setpgid(getpid(),groupID); //set the parent's group id
	}	

	/*
	 * 
	 * PROCESS 2
	 * 
	 */
	
	pid = fork(); //fork the process
	
	//in child
	if(pid == 0){
		
		setpgid(getpid(),groupID);	//set the child'd group to be that of the it's parent
	
		close(pipeFD[1]);	 //close the read end of the pipe
		if(pipeFD[0] !=  0)		//if the pipeFD[0] is not already stdin
			dup2(pipeFD[0],0); //dup the pipeFD[0] to stdin
		close(pipeFD[0]); //close the write end of the pipe	
		
		//handle redirection if there's any
		
		
		//parse arguments
		
		
		//test with process 2 being "ws -l"
		char *process2Args[] = {"wc", "-l",NULL};
		execvp("wc",process2Args);
	}
	//parent
	else{
		setpgid(getpid(),groupID); //set the parent's group id

	}	
		
	waitpid(pid);

	
}

void checkRed(char** tokens, int proc){
	int i = 0;
	while(tokens[i] != NULL){
		
		char* arg = tokens[i];
		int j = 0;
		while(arg[j] != NULL){
			j++;
		}
		
		if(arg[j-1] == '>'){
			if(proc == 1){
				printf("Error: Overlapping Redirection\n");
			}
			else{
				printf("redirecting output\n");
				int new_out = open(tokens[i+1], O_WRONLY | O_CREAT , 0644);
				dup2(new_out, STDOUT_FILENO);
				printf("output redirected to %s\n", tokens[i+1]);
				arg[j-1] = '\0';
			}
		}
		else if(arg[j-1] == '<'){
			if(proc == 2){
				printf("Error: Overlapping Redirection\n");
			}
			else{
				int new_in = open(tokens[i-1], O_RDONLY);
				dup2(new_in, STDIN_FILENO);
				printf("input redirected\n");
				arg[j-1] = '\0';
			}
		}
	//else 
		//printf("Parse Error:");
		i++;
	}	
	return;
	
	
}

int getTokens(char* input, char** tokens){
	//create tokenizer
	tokenizer = init_tokenizer(input);
	
	int i = 0;
	while((tok = get_next_token(tokenizer)) != NULL){
		tokens[i++] = tok;
		printf("token is: %s\n", tokens[i-1]);
	}
	
	//int j;
	//for(j = 0; j < i; j++){
		//checkTok(tokens[j], j, tokens);
		//printf("Token at %i is %s\n", j, tokens[j]);
	//}
	
	free_tokenizer(tokenizer);
	
	return i +1;

}

int checkBG(char** command){
	int i =0;
	while(command[i] != NULL){ i++; }
	
	char* arg = command[i-1];
	i = 0;
	while(arg[i] != NULL){ i++; }
	
	if (arg[i-1] == '&'){
		//background process
		printf("found &\n");
		arg[i-1] = NULL;
		return 1;	
	}
	else{
		//forground process
		return 0;
	}
	
}

int execute(char** command, int nTokens, int bg){
	
	int status;
	pid_t pid = -1;
	
	printf("bg: %i\n",bg);
			
	status =  execvp(command[0], command);
	printf("bg: %i\n",bg);
	if (bg ==1){
		char** bgProc;
		bgProc[0] = "bg";
		bgProc[1] = getpgid();
		
		printf("Executing: %s\n", *bgProc);
		execvp(bgProc[0], bgProc);
		printf("Running: %s\n", *command);
	}
	return status;
}



