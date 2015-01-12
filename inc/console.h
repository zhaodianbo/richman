#ifndef CONSOLE_H
#define CONSOLE_H

#include<stdio.h>
#include<malloc.h>
#include<memory.h>
#include<stdlib.h>
#include"game.h"
#define LINE_MAX 256
#define CMD_MAX 50
#define LOCAL 0
#define NET_SERVER 1
#define NET_CLIENT 2

struct cmd_tbl_s {
	char	*name;		/* Command Name			*/
	int		(*cmd)(int, char *[]);
};

typedef struct cmd_tbl_s	cmd_tbl_t;

int ParseCmdLine(  int argc , char *argv[] );
void playing();
int init(int);
int cmd_help(int n, char *c[]);




#endif

