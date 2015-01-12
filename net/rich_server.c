/* server.c */
/*
 * v0.1 服务器和客户端程序，服务器端通知指定客户端上传指令，接收后把cmd广播给每个客户端
 * 		当接收到字符'x'后切换客户端索引号
*/

/*说明
step	server		  			client
  1		write to N 		--->	 read  		服务器通知指定客户N响应  所有客户端read，
  2		read from N		<--- 	N write 	客户端N响应，将数据上传，其他客户等待下载
  3  	write to All	--->	 read  		所有客户端下载数据
    	... ...
    	
  		重复1 2 3,如果N上传的数据首为'x'，代表结束，服务器切换通知N+1响应
		如果step2中客户N断开连接，则使用del_client删除该sockfd
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "wrap.h"

//toupper
#include <ctype.h>

//inet_ntop
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "rich_server.h"
#include <signal.h>
#include "rich_client.h"

#define MAXLINE 80
#define SERV_PORT 8000

int maxi, maxfd, listenfd;
int client[FD_SETSIZE];
fd_set allset;
int g_currentIndex;
int g_index;
int player_num;

#if 1
int main(int argc, char **argv)
{	
	g_currentIndex=0;
	init_server();
	TELL_PARENT(getppid());
	wait_client(*argv[1]);//*argv[1]代表人数
	printf("You can begin the game!\n");
	server_run();
	return 0;
}
#endif


/*捕获write不成功时产生的sigpipe信号
 *write不成功是由于对方链接关闭引起的 
*/
static void
write_err(int signo)
{
    printf("a player quit\n");
	client[g_index]=-1;
	player_num--;
	return;
}


int init_server(void)
{	
	struct sockaddr_in servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);


	Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	Listen(listenfd, 20);

	maxfd = listenfd;		/* initialize */
	maxi = -1;			/* index into client[] array */
	int i;
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	if(signal(SIGPIPE, write_err) == SIG_ERR)
	{
		printf("server signal action init error\n");
		exit(0);
	}
	return 0;

}

int wait_client(int num)
{
	int client_num,i,nready,connfd;
	fd_set rset;
	socklen_t cliaddr_len;
	struct sockaddr_in	cliaddr;
	char str[INET_ADDRSTRLEN];
	for(client_num=0;client_num < num;)//等待其他2个客户端连接
	{
		rset = allset;	/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (nready < 0)
			perr_exit("select error");

		if (FD_ISSET(listenfd, &rset)) { /* new client connection */
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
			printf("A player join the game:\n");
			printf("received from %s at PORT %d\n",
			       inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
			       ntohs(cliaddr.sin_port));

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd; /* save descriptor */
					break;
				}
				
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}
			char send[2];
			send[0]=i;		//client id
			send[1]=num;	//player num
			Write(connfd,send,2);
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd; /* for select */
			if (i > maxi)
				maxi = i;	/* max index in client[] array */
			
			client_num++;

		}	

	}
	return 0;

}


/*
 * 客户端和服务器建立连接后，所有客户端等待服务器通知
 * 服务器只向当前被激活的客户端接收数据
 */
int server_run(void)
{
	ssize_t n;
	char send_buf[MAXLINE];
	char recv_buf[MAXLINE];
	int sockfd;
	player_num=maxi+1;/*maxi=-1 表示无连接*/
	g_currentIndex=0;
	while(player_num > 0)	
	{
	
		//printf("current index=%d\n",g_currentIndex);
		sockfd = client[g_currentIndex];//只对当前id的连接处理

		if(sockfd == -1) 
		{
			if(++g_currentIndex > maxi) 
				g_currentIndex=0;
			continue;
			//return -1;		//something wrong
		}
					
		send_buf[0]=g_currentIndex;
		
		for(g_index=0; g_index <= maxi; g_index++)		//通知指定客户端发送数据
			if(client[g_index]!=-1)
				Writen(client[g_index], send_buf, 1);	
		
		if ( (n = Read(sockfd, recv_buf, MAXLINE)) == 0) {
			/* connection closed by client */
			Close(sockfd);
			FD_CLR(sockfd, &allset);
			client[g_currentIndex]=-1;
			player_num--;
			//del_client(g_currentIndex);
		} 
		else 
		{
			ssize_t j;
			for (j = 0; j < n; j++)
				send_buf[j] =recv_buf[j];
			for(g_index=0; g_index <= maxi; g_index++)
			{
				if(client[g_index]!=-1)
					Writen(client[g_index], send_buf, n);	
				usleep(1000);	//测试中只有一块网卡 不加延时会丢失数据
			}
		
			if(recv_buf[0]!='x')	continue;//如果客户端不发送x，则继续
		}
		
		if(++g_currentIndex > maxi) g_currentIndex=0;	//下一个客户	

	}
	return 0;
}

#if 0
int notice_client(int fd)
{
	char c;
	Write(fd,&c, 1);
}

int recv_client(int fd)
{
	ssize_t n = Read(sockfd, buf, MAXLINE);
	if(n==0)
	{
		/* connection closed by client */
		Close(sockfd);
		FD_CLR(sockfd, &allset);
		del_client(g_currentIndex);

	}

}
#endif
/*当client断开连接后删除该客户*/
void del_client(int index)
{
	int i;
	for(i = index; i < maxi; i++)
		client[i]=client[i+1];
	client[maxi]=-1;
	maxi--;
}

