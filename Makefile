all: interview

interview: net.o
	gcc -o interview interview.c net.o

net.o:	
	gcc -o net.o -c net.c

clean:
	rm -rf *.o interview
