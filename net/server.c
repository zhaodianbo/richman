/*server.c 
* 在uClinux下使用下面的命令编译
*	bfin-uclinux-gcc -Wl,-elf2flt  fft_test.c  bf_server.c -lm -lbfdsp -lpthread
* #define SEND_IP "192.168.88.51" 设置为回送的IP地址
*	v0.2 2012-5-29 16:00		同时收发有问题
*	v0.3 2012-5-29 20:19:20 能够收发同时计算fft，未在bf532上测试
*/


#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "fft_test.h"
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#define NUM 30001					//所传数据的点数

#define MAXLINE 80
#define SERV_PORT 8000
#define SEND_PORT (SERV_PORT+1)
#define MAXSPACE  0x2800  //max malloc len: 10k
#define FFT_STEP (NUMPOINTS/2)
#define OUT_LEN (((MAXSPACE/2)/(FFT_STEP)-1)*(NUMPOINTS)*2)  //FFT运算后数据的长度
#define SEND_IP "192.168.88.51"

unsigned char *g_pbuf_rev1;			//输入缓存1
unsigned char *g_pbuf_rev2;			//输入缓存2
unsigned short *g_pfft_out1;		//输出缓存1
unsigned short *g_pfft_out2;		//输出缓存2
unsigned int flag_in;						//输入缓存控制标志位
unsigned int flag_out;					//输出缓存控制标志位
pthread_mutex_t in_mutex[2];		//输入锁
pthread_mutex_t out_mutex[2];		//输出锁
volatile unsigned int busy;			//整体工作状态
volatile unsigned int fft_busy;	//fft工作状态 fft_busy=0：等待计算；1：1个buf待运算 2:2个buf待运算
volatile unsigned int send_busy;//发送工作状态 类似fft_busy
//int first_fft=0;								//首次FFT计算
//int first_send=0;								//首次发送数据

void *send_thread(void *arg);
void *FFT_thread(void *arg);
int net_init(void);
struct sockaddr_in servaddr, cliaddr;
struct timeval tpstart,tpend; 

int main(void)
{	
		char buf[MAXLINE]="receive ok!";
		int  connfd;
		int i,j, n,n_left;
		unsigned long int sum,total;
		unsigned char *prev;
		pthread_t tid_send,tid_fft;	
		
		g_pbuf_rev1=malloc(MAXSPACE);
		g_pbuf_rev2=malloc(MAXSPACE);
		g_pfft_out1=(unsigned short *)malloc(OUT_LEN);
		g_pfft_out2=(unsigned short *)malloc(OUT_LEN);

		connfd=net_init();
					
		i=0;	
		total=0;
		flag_in=1;
		fft_busy=0;
		busy=1;
			
		for (i = 0; i < 2; i++)//初始化锁
		{
	    pthread_mutex_init(&in_mutex[i], NULL);
	    pthread_mutex_init(&out_mutex[i], NULL);
	  }
	  //创建fft和发送线程
		pthread_create(&tid_fft, NULL,FFT_thread,NULL);	
		pthread_create(&tid_send, NULL,send_thread,NULL);	
		
		do{
			//sum=0;
			pthread_mutex_lock(&in_mutex[flag_in%2]);
			printf("recvive thread have in_mutex[%d]\n",flag_in%2);
			n_left=MAXSPACE;
			if(flag_in%2)
				prev=g_pbuf_rev1;
			else	 
				prev=g_pbuf_rev2;
			
			while(1){
				n = read(connfd, prev,n_left);
				prev+=n;
				n_left-=n;
				total+=n;
				if((n_left==0)||(n==0)) break;
				if(n<0 )
				{
					if(errno==EINTR)
					  n=0;
					else
						break;
				}
			}			
			flag_in++;
			fft_busy++;
			printf("fft_busy=%d\n",fft_busy);
			
			pthread_mutex_unlock(&in_mutex[flag_in%2]);
			
			printf("recvive thread release in_mutex[%d]\n",(flag_in-1)%2);
			while(fft_busy==2);
			printf("#");
			fflush(stdout);
			if (n <= 0) {
				printf("\n the other side has been closed.\n");
				printf("file size is %lu Bytes\n",total);
				busy=0;	
			}	 
		}while(busy); 
		//printf("\n");
		//printf("file size is %lu Bytes\n",sum);	
		printf("Finish!\n");

	  pthread_join(tid_fft,NULL);
	  pthread_join(tid_send,NULL);
	  
	  close(connfd);
		free(g_pbuf_rev1);
	  free(g_pbuf_rev2);
	  free(g_pfft_out1);
	  free(g_pfft_out2);
	/**********************send ******************************/

	return 0;
}

#if 1
void *send_thread(void *arg)
{
    struct sockaddr_in send_ddr;
    int sockfd, n,n_left;
    unsigned int send_flag,len;
    unsigned char *ptr;
    //sleep(1);  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&send_ddr, sizeof(send_ddr));
    send_ddr.sin_family = AF_INET;
    inet_pton(AF_INET, SEND_IP, &send_ddr.sin_addr);
    send_ddr.sin_port = htons(SEND_PORT);
    
    connect(sockfd, (struct sockaddr *)&send_ddr, sizeof(send_ddr));
    
    len=OUT_LEN;
    printf("fft out data len=%u\n",len);
    send_flag=1;
    //while(first_send==0);	//等待首次发送
    //printf("send start!\n");
	
    while(busy)
    {
        while(fft_busy)
        {
            while(send_busy){
                printf("send start!\n");
                n_left=len;
                pthread_mutex_lock(&out_mutex[send_flag%2]);//阻塞等待锁释放
                printf("send thread have out_mutex[%d]\n",send_flag%2);
                if(send_flag%2)
                	ptr=(unsigned char*)g_pfft_out1;
                else
                	ptr=(unsigned char*)g_pfft_out2;
                printf("busy=%d,send_busy=%d\n",busy,send_busy);	
                while(n_left!=0)
                {
                    printf("Sending\n");
                    n=write(sockfd, ptr, n_left);
                    if(n<=0)
                    {
                        if(n< 0 && errno==EINTR)
                        	n=0;
                        else
                        {
                        	pthread_mutex_unlock(&out_mutex[send_flag%2]);//解锁
                        	printf("send thread release out_mutex[%d]\n",(send_flag)%2); 
                        	close(sockfd);
                        	return;
                        }
                    }				
                    n_left-=n;
                    ptr+=n;
                }
                send_flag++;
                send_busy--;
                pthread_mutex_unlock(&out_mutex[send_flag%2]);//解锁
                printf("send thread release out_mutex[%d]\n",(send_flag-1)%2);
            }
        }
    }
    close(sockfd);
    return;
}


void *FFT_thread(void *arg)
{
	  unsigned int fft_in=1;
	  int i;
	  unsigned int data_len=MAXSPACE/2-NUMPOINTS;	
	  //unsigned short *tmp_out;
		//sleep(1);
		//first_send=1;
		#if 1
		FFT_init();
		gettimeofday(&tpstart,NULL);
		//while(first_fft==0);	//等待首次运行
		//fp_fft=fopen("fft_out.h","wb");
		while(busy){
			while(fft_busy){
				printf("FFT start!\n");
				printf("busy=%d,fft_busy=%d\n",busy,fft_busy);
				pthread_mutex_lock(&in_mutex[fft_in%2]);//阻塞等待锁释放
				printf("fft thread have in_mutex[%d]\n",fft_in%2);
				if(fft_in%2)
				{	
					pthread_mutex_lock(&out_mutex[fft_in%2]);//阻塞等待锁释放
					printf("fft thread have out_mutex[%d]\n",fft_in%2);
					for(i=0;i<data_len;i+=FFT_STEP)
					{
						FFT_f16((unsigned short *)(g_pbuf_rev1+i*2),g_pfft_out1+2*i);
					
					}
					printf("FFTing\n");
					pthread_mutex_unlock(&out_mutex[fft_in%2]);
					printf("fft thread release out_mutex[%d]\n",fft_in%2);
				}
				else
				{
					pthread_mutex_lock(&out_mutex[fft_in%2]);
					printf("fft thread have out_mutex[%d]\n",fft_in%2);
					for(i=0;i<data_len;i+=FFT_STEP)
					{
						FFT_f16((unsigned short *)(g_pbuf_rev2+i*2),g_pfft_out2+i*2);
					
					}
					printf("FFTing\n");
					pthread_mutex_unlock(&out_mutex[fft_in%2]);
					printf("fft thread release out_mutex[%d]\n",fft_in%2);
				}
				//sleep(1);
				fft_in++;
				fft_busy--;
				send_busy++;
				pthread_mutex_unlock(&in_mutex[fft_in%2]);
				printf("fft thread release in_mutex[%d]\n",(fft_in-1)%2);
				while(send_busy==2);
				//first_send=1;
			}
		}
		gettimeofday(&tpend,NULL);
		printf("FFT time pass: %ld msec\n",(tpend.tv_sec*1000000+tpend.tv_usec - tpstart.tv_sec*1000000-tpstart.tv_usec)/1000);
		//fclose(fp_fft);
		//system("tftp -p -l fft_out.h 192.168.88.51");
		//write(connfd, buf, MAXLINE);
		return;
		#endif	
}

#endif


int net_init(void)
{
		char str[INET_ADDRSTRLEN];
		socklen_t cliaddr_len;
		int listenfd, connfd;
		listenfd = socket(AF_INET, SOCK_STREAM, 0);

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(SERV_PORT);
	    
		bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
		listen(listenfd, 20);
	
		printf("Accepting connections ...\n");
	
		cliaddr_len = sizeof(cliaddr);
			connfd = accept(listenfd, 
					(struct sockaddr *)&cliaddr, &cliaddr_len);
			
			/************************** receive **********************************/		
			printf("receiving from %s at PORT %d\n",
						inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
						ntohs(cliaddr.sin_port));
		return connfd;
}
