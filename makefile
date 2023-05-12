build: argparse.o builtin.o myshell.o
	gcc -g -o myshell argparse.o builtin.o myshell.o

argparse.o: argparse.c argparse.h
	gcc -g -c argparse.c

builtin.o: builtin.c builtin.h
	gcc -g -c builtin.c

myshell.o: myshell.c
	gcc -g -c myshell.c

clean:
	rm -f myshell
	rm -f *.o *.bak *~

