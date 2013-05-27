#include <fcntl.h>
#include <signal.h>

pid_t groupID;
pid_t shellPID;
pid_t lastJob;
pid_t lastStoppedJob;
int shmid;
char *lastJobCmd;
char *lastStoppedJobCmd;
struct sigaction sigAction;
