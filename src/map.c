#include<stdio.h>
#include<malloc.h>
#include<memory.h>
#include"map.h"

char map[HEIGHT][WIDTH]=
			   {{'s','1','2','3','4','5','6','7'},
				{'#',' ',' ',' ',' ',' ','4','2'},
				{'#',' ',' ',' ',' ',' ','4','2'},
				{'#','n','n','n','n','n','4','2'},
			    {'@','3','3','3','3','3','3','@'}};
			    	
int value[HEIGHT][WIDTH]={{0,1,1,1,1,1,1,0},
						  {5,0,0,0,0,0,0,2},
						  {5,0,0,0,0,0,0,2},
						  {5,0,0,0,0,0,0,2},
						  {0,3,3,3,3,3,3,0}};
int House_value[MAX_LEVEL]={1000,2000,3000,4000};	
					  	
						  	
//void print_map(char map[HEIGHT][WIDTH]);
void get_next_pos(int *x,int *y);
void get_prev_pos(int *x,int *y);

#if 0		    	
int main()
{
	//print_map(map);	
	int x=0,y=0;
	char (*p)[WIDTH]=map;
	printf("%c",p[x][7]);
	
	st_map_info *p_map=init_map_info();
	if(p_map==NULL) return;
		
	while(1)
	{
		get_n_pos(1,&x,&y);
		set_map_cur(p_map,x,y);
		set_map_e(p_map,'a',x,y);
		//printf("%c",p->map[x][y]);	
		print_map(p_map);
		getchar();
	}
	
}		
#endif 
void print_map(st_map_info *p)
{
	int i,j;
	for(i=0;i<HEIGHT;i++)
	{
		for(j=0;j<WIDTH;j++)
			printf("%c",p->map[i][j]);
		printf("\n");
	}	
}	   

/*
int x=0,y=0;
	while(1)
	{
		get_next_pos(&x,&y);
		printf("%c",map[x][y]);	
		getchar();
	}
*/
void get_next_pos(int *x,int *y)
{
	if(*x==0 && *y!=(WIDTH-1))
		(*y)++;
	else if(*y==(WIDTH-1) && *x!=(HEIGHT-1))
		(*x)++;
	else if(*x==(HEIGHT-1) && *y!=0)
		(*y)--;
	else if(*y==0 && *x!=0)
		(*x)--;
		
}

void get_prev_pos(int *x,int *y)
{
	if(*y==0 && *x!=(HEIGHT-1))
		(*x)++;
	else if(*y!=(WIDTH-1) && *x==(HEIGHT-1))
		(*y)++;
	else if(*x!=0 && *y==(WIDTH-1))
		(*x)--;
	else if(*y!=0 && *x==0)
		(*y)--;
		
}

//从当前(x,y)向前或向后走n步 得到新的(x,y)
void get_n_pos(int n,int *x,int *y)
{
	if(n>=0)
		while((n--)>0)
			get_next_pos(x,y);
	else
		while((n++)<0)
			get_prev_pos(x,y);
}

//初始化地图 当前位置为(0,0)
st_map_info * init_map_info()
{
	st_map_info *p=NULL;
	p=malloc(sizeof(st_map_info));
	if(p==NULL) return NULL;
	//p->map=map;
	memcpy(p->map,map,sizeof(map));
	p->value=value;
	p->x=0;
	p->y=0;
	return p;	
}

void deinit_map_info(st_map_info **p)
{
	if(*p==NULL) return;
	free(*p);
	*p=NULL;
}

//设置地图上的当前位置
void set_map_cur(st_map_info *p,int x,int y)
{
	if(p->map[x][y]==' ' || x<0 ||x >=HEIGHT || 
		y<0 || y>= WIDTH) return; //非法位置 设置失败
	p->x=x;
	p->y=y;
}

//获取地图当前位置
void get_map_cru(st_map_info *p,int *x,int *y)
{
	*x=p->x;
	*y=p->y;
}

//设置地图(x,y)元素的值
void set_map_e(st_map_info *p,char e,int x,int y)
{
	p->map[x][y]=e;	
}

//获取位置的值
char get_map_e(st_map_info *p,int x,int y)
{
	return p->map[x][y];
}

int get_map_value(st_map_info *p,int x,int y)
{
	return (p->value)[x][y];
}