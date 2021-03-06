#ifndef __SHELLFUNC_H__
#define	__SHELLFUNC_H__

#include <unistd.h>
#include <stdio.h>
#include "tokenizer.h"
#include "global.h"

TOKENIZER *tokenizer;
char* tok;

void checkRed(char** tokens, int proc);

int getTokens(char* input, char** tokens);

int checkBG(char* command);

int execute(char** command, int nTokens, int bg);

int checkPipe(char* input, int legth);

void processPipe();

int processCommand(char* command);

#endif
