#ifndef PLAYER_H
#define PLAYER_H
#define MAX_PLAYER_NUM 4
#define INIT_MONEY 1000
#define MAX_MONEY 10000
#define MAX_NAME_LEN 15
#include"util.h"
#define MAX_TOOL_NUM 10
typedef struct player{
	char symbol;	//玩家在地图上的标志
	char *name;		//玩家的名字
	int id;			//玩家id
	int x,y;		//玩家在地图上的位置pos	
	int money;		//玩家金钱数
	int step;		//玩家在地图相对起点的步数
	e_color color;	//玩家颜色
	int tool_num;	//玩家道具数量
	int tool_table[MAX_TOOL_NUM];//存储道具id
}st_player;

int  get_player_step(st_player *p);
int  get_player_id(st_player *p);
int  get_player_money(st_player *p);
void set_player_step(st_player *p,int step);
void deinit_player(st_player **p);
void get_player_pos(st_player *p,int *x,int *y);
void set_player_pos(st_player *p,int x,int y);
void set_player_money(st_player *p,int money);
void print_player_info(st_player *p);
char get_player_symbol(st_player *p);
char *get_player_name(st_player *p);
st_player *init_player(int id);
e_color get_player_color(st_player *p);

//道具相关
int get_player_tln(st_player *p,int index);
int get_player_tlnum(st_player *p);
void init_player_tool(st_player *p);
void del_player_tool(st_player *p,int id);
void add_player_tool(st_player *p,int id);
void set_player_tln(st_player *p,int index,int id);
void set_player_tlnum(st_player *p,int num);

#endif