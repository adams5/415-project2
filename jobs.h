#ifndef __JOBS_H__
#define __JOBS_H__

#include "global.h"
#include <signal.h>

void bg();
void fg();
void setLastBG(pid_t pid);
void setLastStoppedBG(pid_t pid);
void bringLastBGtoFG();
void sendShellToFG();
void sendToBG(pid_t pid, char* com);
void sendToFG(pid_t pid);

#endif
