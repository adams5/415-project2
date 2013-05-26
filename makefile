all: project2 

project2: project2.o tokenizer.o shellFunc.o
	gcc -o project2 project2.o tokenizer.o shellFunc.o

project2.o: project2.c shellFunc.h
	gcc -o project2.o -c project2.c -std=c99

shellFunc.o: shellFunc.c shellFunc.h
	gcc -o shellFunc.o -c shellFunc.c -std=c99

tokenizer.o: tokenizer.c 
	gcc -o tokenizer.o -c tokenizer.c -std=c99

shell_signals.o: shell_signals.c
	gcc -o shell_signals.o -c shell_signals.c -std=c99

clean:
	rm *.o project2
