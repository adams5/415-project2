
typedef struct BGProc{
	pid = 0;
	pgid = 0;
	BGProc next = NULL;
}bgproc;

typedef struct FGProc{
	pid = 0;
	pgid = 0;
	FGProc next = NULL;
}fgproc;


int cmp(char *s1, char *s2);
