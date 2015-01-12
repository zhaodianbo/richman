#include<stdio.h>
#include<memory.h>
#include<stdlib.h>

#include <unistd.h>
int set_disp_mode(int fd,int option);
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define MSG_TRY "try again\n"
int g_timeout=0;
int g_tstart=0;
void select_sleep(int i);

void *is_timeout(void *arg)
{

	
	char buf[10];
	int n;
	int flags;

	flags=fcntl(STDIN_FILENO,F_GETFL);
	flags|=O_NONBLOCK;
	if(fcntl(STDIN_FILENO,F_SETFL,flags)==-1){
		perror("fcntl");
		exit(1);
	}

	printf("time left:\n");
	int i;
	char c;
	int num=0;
	printf("enter:\n");
	printf("\033[s");			//保存光标
	printf("\033[?25l");		//隐藏光标
	system("stty raw -echo");	//变成单个字符输入模式  原模式为行输入模式
	set_disp_mode(STDIN_FILENO,0);
	g_tstart=1;
	#if 0
	for(i=9;i>=0;i--)
	{
		//printf("\033[2;11H");//把光标移动到第2行第11列
		//if(i==5)
		printf("\033[2;11H%d\n\033[u",i);
		//printf("\033[u");
		//c=getc(stdin);
		//if(c=='\n') break;
		//printf("\033[2J");
		select_sleep(2);
	}
	#endif
	while(g_tstart);
	system("stty -raw -echo");
	set_disp_mode(STDIN_FILENO,1);
	printf("\033[2J");		//清屏
	printf("\033[?25h");	//显示光标
	g_timeout=1;
}

int main()
{

/*数组指针和指针数组*/
#if 0
	int a[5]={1,2,3,4,5};	
	int *p1=a;
	int (*p2)[]=&a;
	int *p3[5];
	p3[0]=p1;
	int i;
	for(i=0;i<5;i++)
	{
		printf("%d ",*(*p2+i));
		printf("\n");
	}
	
	for(i=0;i<5;i++)
	{
		
		printf("%d ",*p3[i]);
		printf("\n");
	}
#endif


/*getc测试 缓冲区的大小?*/
#if 0
	int i=2;
	int n=0;
	#define LINE_MAX 10
	char cmd_line[LINE_MAX];
	memset(cmd_line,0,sizeof(cmd_line));
	while(((cmd_line[n]=getc(stdin))!=EOF) && (cmd_line[n]!='\n') && ((++n) < LINE_MAX) );
	cmd_line[n]='\0';
	printf("%s\n",cmd_line);
	printf("n=%d\n",n);
	for(;n>=0;n--)
		printf("%d",cmd_line[n]);
#endif

/*非阻塞方式读取终端*/
#if 1

#if 0	
tryagain:
	n=read(STDOUT_FILENO,buf,10);
	if(n<0){
		if(errno==EAGAIN){
			sleep(1);
			write(STDOUT_FILENO,MSG_TRY,strlen(MSG_TRY));
			goto tryagain;
		}
		perror("read stdin");
		exit(1);
	}
	write(STDOUT_FILENO,buf,n);
#endif //
#endif //非阻塞读

/*测试倒计时*/
#if 1

	char buf[10];
	int n=0;
	int flags;

	flags=fcntl(STDIN_FILENO,F_GETFL);
	flags|=O_NONBLOCK;
	if(fcntl(STDIN_FILENO,F_SETFL,flags)==-1){
		perror("fcntl");
		exit(1);
	}

	printf("time left:\n");
	
	char c;
	int num=0;
	printf("enter:\n");
	printf("\033[s");			//保存光标
	printf("\033[?25l");		//隐藏光标
	system("stty raw echo");	//变成单个字符输入模式  原模式为行输入模式
	//set_disp_mode(STDIN_FILENO,0);
	g_tstart=1;




	pthread_t ptime;
	//pthread_create(&ptime, NULL,is_timeout,NULL); 
	
	fd_set readfd;
	FD_ZERO(&readfd);
	FD_SET(STDOUT_FILENO,&readfd);

	struct timeval timeout,tpstart,tpend;

	gettimeofday(&tpstart,NULL);
	time_t tmp=tpstart.tv_sec*1000000+tpstart.tv_usec;

	int i=10;
	while(!g_timeout && i>0)
	{
	
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		//select(STDOUT_FILENO+1, &readfd, NULL, NULL, & timeout );
		//if(FD_ISSET(STDOUT_FILENO,&readfd))
			n=read(STDOUT_FILENO,&num,1);
		if(n==1)
			printf("%d",num-'0');
		//n=0;
		
		gettimeofday(&tpend,NULL);
		
		if((tpend.tv_sec*1000000+tpend.tv_usec -tmp )>1000000)
		{
			//select_sleep(1);
			printf("\033[2;11H%d\n\033[u",i--);
			gettimeofday(&tpstart,NULL);
			tmp=tpstart.tv_sec*1000000+tpstart.tv_usec;
		}
	}
	g_tstart=0;
	//pthread_join(ptime,NULL);


		system("stty -raw echo");
	//set_disp_mode(STDIN_FILENO,1);
	printf("\033[2J");		//清屏
	printf("\033[?25h");	//显示光标
	g_timeout=1;

#endif

/*产生小随机数*/
#if 0
#define x 3
#define y 4
#define NUM x+y 
	int range=6;
	int ret;
	int i;
	int definenum=NUM;
	unsigned int bucket_size=RAND_MAX/range;
	srand((unsigned) time(NULL));
	for(i=0;i<10;i++)
	{
		do ret=rand()/bucket_size+1;
		while(ret>range);
		printf("%d ",ret);
	}
	printf("\n");
#endif



	return 0;

}


int set_disp_mode(int fd,int option)
{
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

   int err;
   struct termios term;
   if(tcgetattr(fd,&term)==-1){
     perror("Cannot get the attribution of the terminal");
     return 1;
   }
   if(option)
       term.c_lflag|=ECHOFLAGS;
   else
       term.c_lflag &=~ECHOFLAGS;
   err=tcsetattr(fd,TCSAFLUSH,&term);
   if(err==-1 && err==EINTR){
       perror("Cannot set the attribution of the terminal");
       return 1;
   }
   return 0;
}

void select_sleep(int i)
{
struct timeval timeout;

timeout.tv_sec = i;
timeout.tv_usec = 0;

select( 0, NULL, NULL, NULL, & timeout );

}


