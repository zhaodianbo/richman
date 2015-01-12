#ifndef RICH_CLIENT_H
#define RICH_CLIENT_H
#include "wrap.h"


int init_client(int *);
int is_notice(int fd);
int upload_cmd(int sockfd);
int download_cmd(int sockfd,char *des);
int client_run(int sockfd,char *des_buf);
#define MAXLINE 80


void    TELL_WAIT(void);                /* parent/child from {Sec race_conditions} */
void    TELL_PARENT(pid_t);
void    TELL_CHILD(pid_t);
void    WAIT_PARENT(void);
void    WAIT_CHILD(void);

#endif

