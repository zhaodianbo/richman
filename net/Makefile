CC=gcc
CFLAGS=-Wall -I ../inc  
VPATH=../inc 
all:rich_client.o rich_server.o
rich_client.o:rich_client.h wrap.h error.h
	$(CC) $(CFLAGS) -c rich_client.c wrap.c error.c 
#cp rich_client.o ../src	
rich_server.o:rich_server.h
	$(CC) $(CFLAGS) -o rich_server.out rich_server.c wrap.c 
	cp rich_server.out rich_client.o wrap.o ../src
	