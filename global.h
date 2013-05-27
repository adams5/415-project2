#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <fcntl.h>
#include <signal.h>
#include "utils.h"

pid_t groupID;
pid_t shellPID;
bgproc lastBG;
bgproc lastStoppedBG;

struct sigaction sigAction;

#endif
