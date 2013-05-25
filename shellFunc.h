#ifndef __SHELLFUNC_H__
#define	__SHELLFUNC_H__

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "tokenizer.h"


TOKENIZER *tokenizer;
char* tok;

void checkTok(char* tempTok, int pos, char** tokens);

int getTokens(char* input, char** tokens);

void processPipe();

#endif
