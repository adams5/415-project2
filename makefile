all: project2 

project2: project2.o tokenizer.o shellFunc.o utils.o shell_signals.o jobs.o process_hash.o
	gcc -o project2 project2.o tokenizer.o shellFunc.o utils.o process_hash.o shell_signals.o jobs.o

project2.o: project2.c shellFunc.h shell_signals.h utils.h global.h jobs.h process_hash.h
	gcc -o project2.o -c project2.c -std=gnu99  -g

shellFunc.o: shellFunc.c shellFunc.h global.h
	gcc -o shellFunc.o -c shellFunc.c -std=gnu99  -g

tokenizer.o: tokenizer.c 
	gcc -o tokenizer.o -c tokenizer.c -std=gnu99  -g

shell_signals.o: shell_signals.c process_hash.h
	gcc -o shell_signals.o -c shell_signals.c -std=gnu99 -g

utils.o: utils.c utils.h
	gcc -o utils.o -c utils.c -std=gnu99  -g

jobs.o: jobs.c process_hash.h
	gcc -o jobs.o -c jobs.c -std=gnu99  -g

process_hash.o: process_hash.c process_hash.h
	gcc -o process_hash.o -c process_hash.c -std=gnu99 -g

clean:
	rm *.o project2
