all: project2 

project2: project2.o tokenizer.o shellFunc.o utils.o shell_signals.o jobs.o
	gcc -o project2 project2.o tokenizer.o shellFunc.o utils.o shell_signals.o jobs.o

project2.o: project2.c shellFunc.h shell_signals.h utils.h global.h jobs.h
	gcc -o project2.o -c project2.c -std=c99 -g

shellFunc.o: shellFunc.c shellFunc.h global.h
	gcc -o shellFunc.o -c shellFunc.c -std=c99 -g

tokenizer.o: tokenizer.c 
	gcc -o tokenizer.o -c tokenizer.c -std=c99 -g

shell_signals.o: shell_signals.c
	gcc -o shell_signals.o -c shell_signals.c -std=c99 -g

utils.o: utils.c
	gcc -o utils.o -c utils.c -std=c99 -g

jobs.o: jobs.c
	gcc -o jobs.o -c jobs.c -std=c99 -g

clean:
	rm *.o project2
