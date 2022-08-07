#To run, put this file together with lexer.h, and lexer.c
#in the same directory. Run "make". Then the executable
#is "schemer," which just takes a line of input and
#breaks it up into tokens.

scheme: main.o lexer.o parser.o SchemeFunctions.o
	gcc -o scheme main.o lexer.o parser.o SchemeFunctions.o

main.o: main.c
	gcc -c main.c

lextester.o: lextester.c
	gcc -c lextester.c

lexer.o: lexer.c
	gcc -c lexer.c

parser.o: parser.c
	gcc -c parser.c

ShemeFunctions.o: SchemeFunctions.c
	gcc -c SchemeFunctions.c

clean:
	rm -f *~ *.o *.a

#^^^^^^This space must be a TAB!!.
