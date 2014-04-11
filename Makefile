all: interview

interview: fucker.o
	gcc -o interview interview.c fucker.o

fucker.o:	
	gcc -o fucker.o -c fucker.c

clean:
	rm -rf *.o interview
