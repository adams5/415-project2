#include <fcntl.h>

pid_t groupID;
pid_t shellPID;
pid_t lastJob;
pid_t lastStopped;
int shmid;
char *lastJobCmd;
