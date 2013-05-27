#include "jobs.h"
#include "global.h"

void bg(){	
	
	//add checking to make sure there's a background job
	
	//send SIGCONT to the most recently stopped background job, this will resume execution
	killpg(lastStoppedJob,SIGCONT);
}


void fg(){
	
	//add checking to make sure there's a last stopped job
	
	printf("%s\n",lastJobCmd);
	
	//bring the most recetly backgrounded job to the foreground
	tcsetpgrp(0, lastStoppedJob);
	tcsetpgrp(1, lastStoppedJob);	
	tcsetpgrp(2, lastStoppedJob);

	//send a SIGCONT in case the job is stopped
	killpg(lastStoppedJob,SIGCONT);
}
