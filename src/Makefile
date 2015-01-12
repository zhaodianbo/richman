#在src文件夹和net文件夹下是源程序 inc下是头文件

#Compile=gcc
#Flag=-Wall
CC=gcc
CFLAGS=-Wall -I ../inc -I ../net 
VPATH=../inc ../net
obj=game.o map.o player.o util.o console.o rich.o rich_client.o wrap.o error.o 
tar=rich
target:$(obj)
	$(CC) -o $(tar) $(obj) $(CFLAGS)
	$(CC) $(CFLAGS) -o rich_server.out ../net/rich_server.c ../net/wrap.c ../net/rich_client.c ../net/error.c
	rm $(obj)
game.o:game.h player.h map.h util.h rich_client.h
map.o:map.h
player.o:player.h
util.o:util.h
console.o:console.h game.h
wrap.o:wrap.h
error.o:error.h
rich_client.o:rich_client.h wrap.h error.h
rich.o:console.h rich_client.h
clean:
	rm rich *.o
