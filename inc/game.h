#ifndef GAME_H
#define GAME_H
#include"map.h"
#include"player.h"
#include"util.h"
#if 0	//测试小地图
#ifdef WIDTH
#undef WIDTH
#define WIDTH WIDTH_S
#endif
#ifdef HEIGHT
#undef HEIGHT
#define HEIGHT HEIGHT_S
#endif
#endif 
#define LAND_NUM (WIDTH*2+(HEIGHT-2)*2)
typedef struct land{
	int x,y;	//pos
	int price;					//土地的价格
	st_player *p_player;		//土地的所有者
	e_color owner_color;		//土地所有者的颜色
	House_type level;			//土地的等级	
	e_color color;				//土地的临时颜色 和站在上面的玩家颜色一致
	int tool_n;					//土地上道具的数量
	int tools[MAX_TOOL_NUM];	//土地上的道具
	char symbol;				//土地临时显示的符号
	
}st_land_info;

typedef struct game{
	st_player *p_player_tab[MAX_PLAYER_NUM];	//玩家table 声明成指针还是结构体好呢？
	st_map_info *p_map;							//地图指针
	st_land_info p_land[LAND_NUM];				//地块结构体
	int player_num;								//玩家数 ==1则玩家胜利 游戏结束
	int id;										//p_player_tab中的索引 指向当前玩家
	int my_turnflag;							//网络版中的标志 1:my turn 0:others turn
}st_game;

#ifndef MAX_TOOL_NUM
#define MAX_TOOL_NUM 10
#endif
#define SHOP 'H'
typedef struct tool{
	char symbol;	//标志
	char *name;		//名字
	int id;			//id
	int price;		//价格
	e_color color;	//颜色
	void (*usetool)(st_game *,int);	//使用道具
	void (*meettool)(st_game *,int); //在土地上遇到道具的效果 比如遇到路障
}st_tool;


/*使用静态表取代过多的if-else?*/
enum land_owner_type{Na=0,Owner,Other,Toolshop};
typedef enum land_owner_type e_LandOwnerType;

#define MAX_LAND_FUN 50
//land function table struct
struct land_ftbl_s{
	e_LandOwnerType owner_type;
	int (*f)(st_game *,int);
};
typedef struct land_ftbl_s	land_ftbl_t;

//从game结构体中直接获取当前玩家的信息
int  get_p_money(st_game *p);
int  get_p_id(st_game *p);
int  get_p_step(st_game *p);
void get_p_pos(st_game *p,int *x,int *y);
void set_p_pos(st_game *p,int x,int y);
void set_p_money(st_game *p,int money);
char get_p_symbol(st_game *p);
char *get_p_name(st_game *p);
e_color get_p_color(st_game *p);
void set_p_step(st_game *p,int step);

//打印地图相关函数
int  update_map(st_game *p,int n);
void print_game_map(st_game *p);
void print_name(st_game *p);
void print_land(st_game *p,int );
void print_p_info(st_game *game,int index);

//初始化
void init_land(st_land_info (*p_land)[]);
void init_shop(st_map_info *map);
void deinit_game(st_game **p);
st_game * init_game(int player_num);


//land结构体相关函数
int  get_land_price(st_land_info *land);
int  get_land_tlnum(st_land_info *land);
void set_land_player(st_land_info *land,st_player *p_player);
void set_land_color(st_land_info *land,e_color color);
void set_land_price(st_land_info *land,int price);
void set_land_level(st_land_info *land,House_type level);
void set_land_symbol(st_land_info *land,char symbol);
void set_land_ownclor(st_land_info *land,e_color color);
char get_land_symbol(st_land_info *land);
e_color get_land_color(st_land_info *land);
e_color get_land_ownclor(st_land_info *land);
st_player *get_land_player(st_land_info *land);
House_type get_land_level(st_land_info *land);
e_LandOwnerType whose_land(st_game *game,st_land_info *land);
void add_land_tool(st_land_info *land,int id);

//game结构体相关函数
int  get_game_pnum(st_game *p);
int  get_game_id(st_game *p);
void set_game_id(st_game *p,int id);
void set_game_pnum(st_game *p,int num);
void next_player(st_game *game);
st_player *get_player(st_game *p);
st_player *get_playern(st_game *p,int id);
st_land_info *get_game_land(st_game *p,int );

//cmd相关函数
int  buy_land(st_game *game,st_land_info *land);
int  upgrade_house(st_game *game,st_land_info *land);
int  is_yes();
int  pay_rent(st_game *game,st_land_info *pland);
void player_quit(st_game *game,int id);
void shop(st_game *game);
void use_tool(st_game *game);
int  explore_block(st_game *game,int n);
void meet_Block(st_game *game,int x);
void NeedU2realize(st_game *game,int n);
int get_num();
int is_myturn(st_game *p);
void clear_turnflag(st_game *p);
void set_game_turn(st_game *p,int turn);




#endif
