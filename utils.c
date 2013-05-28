#include <stdlib.h>
#include "utils.h"

//method to compare to char arrays
int cmp(char *s1, char *s2)
{
    //offset starts at 0
    int j = 0;

    //while the starting point + offset is null a null char
    while(*(s1 +j) != '\0')
    {
        //if the two char arrays + offset are not the same then return 0 for false
        if(*(s1 +j) != *(s2 +j))
            return 0; //not the same
        //these chars were the same so increase the offset
        j++;
    }
    //the char arrays were the same, truen 1 for true
    return 1; // the same
}

int msgqueue_init(){
	if((long)(msgqueue = calloc(MSG_MAX, sizeof(char*))) != -1){
		mhead = 0;
		mtail = 0;
		return 1;
	}
	else
	{
		printf("Error: Message Queue failed to initialize\n");
		return -1;
	}
}

int free_msgqueue(){
	free(msgqueue);		///not sure what to do here
	return 1;	
}

int insertmsg(char* msg){
	if(!msgisfull()){
		mhead = (mhead+1)%MSG_MAX;
		msgqueue[mhead] = msg;
		return 1;
	}
	else{
		printf("Message Queue is full, cannot insert message: %s\n", msg);
		return 0;
	}
}
 
int removemsg(char** msg){
	if(!msgisempty()){
		*msg = msgqueue[mtail];
		mtail = (mtail+1)%MSG_MAX;
		return 1;
	}
	else
	{
		printf("Message Queue is empty. No messages waiting\n");
		return 0;
	}
		
}

void removeallmsg(){
	while(!msgisempty()){
		printf("%s\n", msgqueue[mtail++]);
	}
}


int msgisempty(){
	if(mhead==mtail)
		return 1;
	else
		return 0;
}

int msgisfull(){
	if(mhead==(mtail-1))
		return 1;
	else
		return 0;
}
