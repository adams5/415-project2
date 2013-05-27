#include "jobs.h"
#include "global.h"

void bg(){
	//send SIGCONT to the most recently stopped background job
	killpg(lastStopped,SIGCONT);
}


void fg(){
	//bring the most recetly backgrounded job to the foreground

	//send a SIGCONT in case the job is stopped
	killpg(lastStopped,SIGCONT);
}
