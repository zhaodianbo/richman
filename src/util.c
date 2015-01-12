#include<stdio.h>
#include"util.h"
#include<sys/types.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
void print_color(char c,e_color color)
{
	#if 0
	switch(color)
	{
		case Green:
			printf("\033[1;40;32m%c\033[0m",c);	break;
		case Red:
			printf("\033[1;40;31m%c\033[0m",c);	break;
		case Blue:
			printf("\033[1;40;34m%c\033[0m",c);	break;
		case Yellow:
			printf("\033[1;40;33m%c\033[0m",c);	break;
		case None:
		default:
			printf("%c",c);
	}
	#endif
	printf("\033[1;40;%dm%c\033[0m",color,c);
}


void print_color_str(char *str,e_color color)
{
	if(str==NULL) return;
	#if 0
	switch(color)
	{
		case Green:
			printf("\033[1;40;32m%s\033[0m",str);	break;
		case Red:
			printf("\033[1;40;31m%s\033[0m",str);	break;
		case Blue:
			printf("\033[1;40;34m%s\033[0m",str);	break;
		case Yellow:
			printf("\033[1;40;33m%s\033[0m",str);	break;
		case None:
		default:
			printf("%s",str);
	}
	#endif
	printf("\033[1;40;%dm%s\033[0m",color,str);
}


int is_digtial(char c)
{
	if(c<='9' && c>='0') return 1;
	return 0;
}

int str2int(const char *str)
{
	int ret=0;
	while((*str)!='\0' )
	{
		if(!is_digtial(*str)) return ret;
		ret=ret*10+*str-'0';
		str++;
	}
	return ret;
}

int find_int(char *str)
{
	while(*str!='\0')
	{
		if(is_digtial(*str)) break;
		str++;
	}
	return str2int(str);
}

int window_col()
{
	struct winsize size;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);    
	//printf("%d\n",size.ws_col);
    return size.ws_col;
}

void print_line(char c,int num)
{
	while(num-- > 0)
		printf("%c",c);
}

//在屏幕中间打印
/*ccccstrcccc*/
void print_bar(char *str,char c,e_color color)
{
#ifdef LINUX_CONSOLE
	int win_col=window_col()/2;
	int str_len=strlen(str);
	int bar_len=win_col-str_len/2;
	while(bar_len-- > 0)
		printf("%c",c);
	print_color_str(str,color);
	bar_len=win_col-str_len+str_len/2;
	while(bar_len-- > 0)
	printf("%c",c);
#else
	print_color_str(str,color);
	printf("\n");
#endif 
}

int six_rand()
{
	/*n很小或很大时，用%生成的随机数并不非常随机*/
#if 0
	static  int n=0;
	if(n==0)
		srand((unsigned) time(NULL));
	if(n++>0xffff)n=0;
	int ret=rand();
	//ret=(ret>>4)&0xf;
	return (ret+n)%6+1;
#endif	
	int range=6,ret;

	/*将随机数区间分割成相同大小的桶*/
	unsigned int bucket_size=RAND_MAX/range;
	srand((unsigned) time(NULL));

	/*得到桶的编号，区间外的编号舍弃*/
	do ret=rand()/bucket_size+1;
	while(ret > range);
	return ret;
}

void *is_timeout(void *arg)
{
	printf("time left:\n");
	int i;
	char c;
	printf("enter:");
	for(i=9;i>=0;i--)
	{
		printf("\033[2;11H");//把光标移动到第2行第11列
		printf("%d\n",i);
		printf("\033[3;7H");
		//c=getc(stdin);
		//if(c=='\n') break;
		//printf("\033[2J");
		sleep(1);
	}
	printf("\033[2J");//清屏
}
#if 0
#include <unistd.h>
int main()
{
	char *p="1234fd3";
	print_color(*p,Red);
	print_color_str(p,Red);
	printf("%d\n",str2int(p));
	window_col();
	int i;
	for(i=0;i<20;i++)
		printf("%d ",six_rand());}

#endif

