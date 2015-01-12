#ifndef MAP_H
#define MAP_H

#define WIDTH 8
#define HEIGHT 5 //map和value也要跟着修改
#define WIDTH_S 7
#define HEIGHT_S 4 //map和value也要跟着修改
typedef struct {
	char map[HEIGHT][WIDTH];	//地图信息
	int (*value)[WIDTH];		//地块初始价值？
	int x,y;					//当前游标位置cur
}st_map_info;

typedef enum House{Level_0=0,Level_1,Level_2,Level_3}House_type;
#define MAX_LEVEL 4
st_map_info * init_map_info();
void deinit_map_info(st_map_info **p);
void print_map(st_map_info *p);
void get_n_pos(int n,int *x,int *y);
void set_map_cur(st_map_info *p,int x,int y);
void get_map_cru(st_map_info *p,int *x,int *y);
void set_map_e(st_map_info *p,char e,int x,int y);
char get_map_e(st_map_info *p,int x,int y);
int  get_map_value(st_map_info *p,int x,int y);
#endif