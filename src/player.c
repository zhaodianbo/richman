#include"player.h"
#include<stdio.h>
#include<malloc.h>
#include<string.h>
char *player_name[MAX_PLAYER_NUM]={"A Tubo","Sun Xiaomei","Qian Furen","Jin Beibei"};
char player_symbol[MAX_PLAYER_NUM]={'A','S','Q','J'};
e_color player_color[MAX_PLAYER_NUM]={Green,Red,Blue,Yellow};
#if 0
int main()
{
	st_player *p=init_player(0);
	print_player_info(p);
	deinit_player(&p);
	return 0;
}
#endif
//初始化该id玩家
st_player * init_player(int id)
{
	st_player *p=malloc(sizeof(st_player));
	if(p==NULL) return NULL;
	p->symbol=player_symbol[id];
	p->name=player_name[id];
	p->id=id;
	set_player_pos(p,0,0);
	set_player_step(p,0);
	set_player_money(p,INIT_MONEY);
	p->color=player_color[id];	
	init_player_tool(p);
	return p;
}
//销毁玩家
void deinit_player(st_player **p)
{
	if(*p==NULL) return;
	free(*p);
	*p=NULL;	
}

//获取玩家名字 linux下终端暂时不能显示中文
char * get_player_name(st_player *p)
{
	return p->name;
}

//获取玩家id
int get_player_id(st_player *p)
{
	return p->id;
}

//获取玩家地图上的位置
void get_player_pos(st_player *p,int *x,int *y)
{
	*x=p->x;
	*y=p->y;
}

//设置玩家位置
void set_player_pos(st_player *p,int x,int y)
{
	p->x=x;
	p->y=y;	
}

//获取玩家金钱数
int get_player_money(st_player *p)
{
	return p->money;	
}


//设置玩家金钱
void set_player_money(st_player *p,int money)
{
	p->money=money;
}

//获取玩家地图上的标志
char get_player_symbol(st_player *p)
{
	return p->symbol;	
}

//获取玩家相对相对起点的步数
int get_player_step(st_player *p)
{
	return p->step;
}

void set_player_step(st_player *p,int step)
{
	p->step=step;	
}

//获取玩家颜色
e_color get_player_color(st_player *p)
{
	return p->color;	
}
//获取玩家道具数量
int get_player_tlnum(st_player *p)
{
	return p->tool_num;
}
//设置玩家道具数量
void set_player_tlnum(st_player *p,int num)
{
	p->tool_num=num;
}
//获取第index个道具的id
int get_player_tln(st_player *p,int index)
{
	return (p->tool_table)[index];
}
//设置第index个道具的id
void set_player_tln(st_player *p,int index,int id)
{
	(p->tool_table)[index]=id;
}
//添加一个道具 道具table中id为MAX_TOOL_NUM表示空位置
void add_player_tool(st_player *p,int id)
{
	if(get_player_tlnum(p) == MAX_TOOL_NUM) return;
	int i=MAX_TOOL_NUM-1;
	for(;i>=0;i--)
		if(get_player_tln(p,i) == MAX_TOOL_NUM)
		{
			set_player_tln(p,i,id);
			break;
		}
}
void del_player_tool(st_player *p,int id)
{
	int i=MAX_TOOL_NUM-1;
	for(;i>=0;i--)
		if(get_player_tln(p,i) == id)
		{
			set_player_tln(p,i,MAX_TOOL_NUM);
			set_player_tlnum(p,get_player_tlnum(p)-1);
			break;
		}
}

void init_player_tool(st_player *p)
{
	set_player_tlnum(p,0);
	int i=MAX_TOOL_NUM-1;
	for(;i>=0;i--)
		set_player_tln(p,i,MAX_TOOL_NUM);

}
//打印玩家信息
void print_player_info(st_player *p)
{
	//int x,y;
	char *p_name=get_player_name(p);
	print_color_str(p_name,get_player_color(p));

	int i=MAX_NAME_LEN-strlen(p_name);
	for(;i>0;i--)
		printf(" ");
	printf("id=%d",get_player_id(p));
	printf("    pos=%-4d",get_player_step(p));
	//printf("       pos:x=%d,y=%d",x,y);
	printf("    money=%-5d",get_player_money(p));
	printf("    tool=%-2d",get_player_tlnum(p));
	//printf("       symbol=%c\n",get_player_symbol(p));	
}