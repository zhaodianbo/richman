#ifndef RICH_SERVER_H
#define RICH_SERVER_H

int server_run(void);
int init_server(void);
int wait_client(int);
void del_client(int index);

#endif

