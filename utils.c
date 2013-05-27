#include "utils.h"

struct BGProc{
	pid = 0;
	pgid = 0;
	BGProc next = NULL;
};


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
