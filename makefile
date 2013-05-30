all: project2 

project2: project2.o tokenizer.o shellFunc.o utils.o shell_signals.o jobs.o
	gcc -o project2 project2.o tokenizer.o shellFunc.o utils.o shell_signals.o jobs.o

project2.o: project2.c shellFunc.h shell_signals.h utils.h global.h jobs.h
	gcc -o project2.o -c project2.c -g -Wall

shellFunc.o: shellFunc.c shellFunc.h global.h tokenizer.h
	gcc -o shellFunc.o -c shellFunc.c -g -Wall

tokenizer.o: tokenizer.c tokenizer.h
	gcc -o tokenizer.o -c tokenizer.c -g -Wall

shell_signals.o: shell_signals.c shell_signals.h global.h
	gcc -o shell_signals.o -c shell_signals.c -g -Wall

utils.o: utils.c utils.h
	gcc -o utils.o -c utils.c -g -Wall

jobs.o: jobs.c jobs.h global.h
	gcc -o jobs.o -c jobs.c -g -Wall

 
clean:
	rm *.o project2
