http : server.o tool.o
	cc server.o tool.o -o http

server.o : server.c
	cc -c server.c
tool.o : tool.c tool.h
	cc -c tool.c

cleanO :
	rm *.o

clean :
	rm *.o http

myTry :
	gcc -g -Wall server.c tool.h tool.c -o http
