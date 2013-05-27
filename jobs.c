#include "jobs.h"
#include "global.h"

void bg(){

	
	//send SIGCONT to the most recently stopped background job
	killpg(lastStoppedJob,SIGCONT);
}


void fg(){
	printf("%s\n",lastJobCmd);
	
	//bring the most recetly backgrounded job to the foreground
	tcsetpgrp(0, lastStoppedJob);

	//send a SIGCONT in case the job is stopped
	killpg(lastStoppedJob,SIGCONT);
}
