#ifndef __SHELL_SIGNALS_H__
#define __SHELL_SIGNALS_H__

#include <signal.h>
#include <stdio.h>
#include "global.h"

void signal_handler(int signal, siginfo_t *siginfo, void *context);

#endif
