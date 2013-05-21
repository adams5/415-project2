all: project2 

project2: project2.o tokenizer.o
	gcc -o project2 project2.o tokenizer.o

project2.o: project2.c
	gcc -o project2.o -c project2.c

tokenizer.o: tokenizer.c 
	gcc -o tokenizer.o -c tokenizer.c

clean:
	rm *.o project2
