all: project2 

project2: project2.o tokenizer.o shellFunc.o
	gcc -o project2 project2.o tokenizer.o shellFunc.o

project2.o: project2.c shellFunc.h
	gcc -o project2.o -c project2.c

shellFunc.o: shellFunc.c shellFunc.h
	gcc -o shellFunc.o -c shellFunc.c

tokenizer.o: tokenizer.c 
	gcc -o tokenizer.o -c tokenizer.c

clean:
	rm *.o project2
