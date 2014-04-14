all: interviewd

interviewd: net.o fork.o
	gcc -o interviewd interviewd.c net.o fork.o

net.o:	
	gcc -o net.o -c net.c

fork.o:
	gcc -o fork.o -c fork.c

clean:
	rm -rf *.o interviewd
