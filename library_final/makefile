exe:main.o book.o issue.o util.o
	cc main.o book.o issue.o util.o -o exe
main.o:main.c
	cc -c main.c
book.o:book.c
	cc -c book.c
issue.o:issue.c
	cc -c issue.c
util.o:util.c
	cc -c util.c
clean:
	rm -f main.o book.o issue.o util.o exe

